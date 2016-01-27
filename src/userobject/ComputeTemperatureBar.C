
#include "ComputeTemperatureBar.h"
#include "FEProblem.h"
#include "libmesh/elem.h"
#include "libmesh/fe.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/quadrature_gauss.h"
#include "libmesh/plane.h"
#include "MooseMesh.h"
#include <fstream>
//#include <mpi.h>
//#include <omp.h>
//#include "mutex"
//#include "thread"
//#include "semaphore.h"
//#include "async"

#include "RayLine.h"
#include "MooseRandom.h"
#include "UserDefinedMesh.h"
#include "UserDefinedElem.h"
#include "UserDefinedSideElem.h"

//#include "MemData.h"
using namespace std;
using namespace Roshan;

template<>
InputParameters validParams<ComputeTemperatureBar>()
{
	InputParameters params = validParams<SideUserObject>();
	params += validParams<RandomInterface>();
	params.addParam<int> ("max_reflect_count", 10, "最大反射次数");
	params.addParam< int > ("particle_count", 1000000, "单元发射粒子数");
	params.addParam< vector<Real> > ("transmissivity", "透射率");
	params.addParam< vector<Real> > ("absorptivity", "吸收率");
	params.addParam< vector<Real> > ("diffuse_reflectivity", "漫反射百分比");
	params.addParam< vector<Real> > ("mirrors_reflectivity", "镜反射百分比");
	params.addParam< std::vector<SubdomainName> >("block", "The list of boundary IDs from the mesh where this boundary condition applies");
	params.addParam< string >("boundary_groups", "The list of boundary groups");
	params.addParam< string >("filename", "The name of file_RD");
	params.addParam< bool >("write", "The list of boundary groups");
	params.addParam< bool>("read", "The list of boundary groups");
	params.addCoupledVar("temperature", "温度场");

	return params;
}

ComputeTemperatureBar::ComputeTemperatureBar(const InputParameters & parameters) :
	SideUserObject(parameters),
	RandomInterface(parameters, *parameters.get<FEProblem *>("_fe_problem"), parameters.get<THREAD_ID>("_tid"), false),
	_max_reflect_count(getParam<int> ("max_reflect_count")),
	_particle_count(getParam<int> ("particle_count")),
	_transmissivity(getParam< vector<Real> > ("transmissivity")),
	_absorptivity(getParam< vector<Real> > ("absorptivity")),
	_diffuse_reflectivity(getParam< vector<Real> > ("diffuse_reflectivity")),
	_mirrors_reflectivity(getParam< vector<Real> > ("mirrors_reflectivity")),
	_filename(getParam< string > ("filename")),
	_write(getParam< bool > ("write")),
	_read(getParam< bool > ("read")),
	_temperature(coupledValue("temperature"))
{
	vector<SubdomainName> block = getParam<std::vector<SubdomainName> >("block");
	for(vector<SubdomainName>::iterator it = block.begin(); it != block.end(); ++it)
	{
		SubdomainID id = _mesh.getSubdomainID(*it);
		_block_ids.insert(id);
	}
}

ComputeTemperatureBar::~ComputeTemperatureBar()
{
	for(int i=0; i<_all_element.size(); i++)
	{
//		if(_all_element[i])
		delete _all_element[i]._elem;
	}
}

//void ComputeTemperatureBar::initialSetup()
//{
//	vector<BoundaryName> boundary = getParam<std::vector<BoundaryName> >("boundary");
//	std::set<BoundaryID> boundary_ids;
//	for(vector<BoundaryName>::iterator it = boundary.begin(); it != boundary.end(); ++it)
//	{
//		BoundaryID id = _mesh.getBoundaryID(*it);
//		boundary_ids.insert(id);
//	}
//
//	MeshBase & mesh = _mesh.getMesh();
//	const BoundaryInfo &bnd_info = mesh.get_boundary_info();
//	MeshBase::const_element_iterator   el  = mesh.active_elements_begin();
//	const MeshBase::const_element_iterator end_el = mesh.active_elements_end();
//	for ( ; el != end_el ; ++el)
//	{
//		const Elem *elem = *el;
//
//		for (unsigned int side=0; side < elem->n_sides(); ++side)
//		{
//			if (elem->neighbor(side))
//				continue;
//
//			Elem *elem_side = elem->build_side(side).release();
//			int bnd_id = bnd_info.boundary_id(elem, side);
//			if(find(boundary_ids.begin(), boundary_ids.end(), bnd_id) == boundary_ids.end())
//				continue;
//
//			unsigned int dim = _mesh.dimension();
//			FEType fe_type(Utility::string_to_enum<Order>("CONSTANT"), Utility::string_to_enum<FEFamily>("MONOMIAL"));
//			FEBase * _fe_face = (FEBase::build(dim, fe_type)).release();
//			QGauss * _qface = new QGauss(dim-1, FIRST);
//			_fe_face->attach_quadrature_rule(_qface);
//			_fe_face->reinit(elem, side);
//			const std::vector<Point> normals = _fe_face->get_normals();
//
//			_all_element.push_back(new SideElement(elem_side, normals[0], _absorptivity, _diffuse_reflectivity, _mirrors_reflectivity));
//
//			delete _fe_face;
//			delete _qface;
//		}
//	}
//	temperature_pow4_bar.resize(_all_element.size(), 8.1e+9);
//	temperature_pow3_bar.resize(_all_element.size(), 2.7e+7);
//	flux_radiation.resize(_all_element.size(), 0);
//	flux_radiation_jacobi.resize(_all_element.size(), 0);
//	computeRD();
//}

void ComputeTemperatureBar::initialSetup()
{
	string boundary_groups = getParam<string>("boundary_groups");
	std::cout <<boundary_groups << std::endl;

	vector<BoundaryName> group;
	vector< vector<BoundaryName> > group_set;
	MooseUtils::tokenize<BoundaryName>(boundary_groups, group, 1, ",");
	group_set.resize(group.size());
	for(int i = 0; i < group.size(); ++i)
	{
//		vector<BoundaryName> group_set;
		MooseUtils::tokenize<BoundaryName>(group[i], group_set[i], 1, " ");
//		for(int j = 0; j < group_set[i].size(); ++j)
//		{
//			cout << group_set[i][j] << endl;
//		}
//		cout << group[i] << endl;
	}

	for(int i = 0; i < group.size(); ++i)
	{
		cout << group[i] << endl;
		for(int j = 0; j < group_set[i].size(); ++j)
		{
			cout << group_set[i][j] << endl;
		}
	}

	vector<BoundaryName> boundary = getParam<std::vector<BoundaryName> >("boundary");
	std::set<BoundaryID> boundary_ids;
	for(vector<BoundaryName>::iterator it = boundary.begin(); it != boundary.end(); ++it)
	{
		BoundaryID id = _mesh.getBoundaryID(*it);
		boundary_ids.insert(id);
	}

	int count_sideelement = 0;

	MeshBase & mesh = _mesh.getMesh();
//	UserDefinedMesh * mymesh = new UserDefinedMesh(_mesh);
	UserDefinedMesh  mymesh(_mesh);

	const BoundaryInfo &bnd_info = mesh.get_boundary_info();
	int meshelems = mesh.n_elem();
//	MeshBase::const_element_iterator it  = mesh.active_subdomain_elements_begin(*_block_ids.begin());
//    const MeshBase::const_element_iterator end_el = mesh.active_subdomain_elements_end(*_block_ids.begin());;
//	for(; it != end_el ; it++ )
//	{
//		nelems++;
//	}
//	cout << "nelems:" << nelems << endl;
//	mymesh->_userDefinedElem.resize(nelems);

	map<dof_id_type, int> transform;
	int myelemcount = 0;

	for (int nelem =0; nelem<meshelems; nelem++)
	{
		const Elem * elem = mesh.elem(nelem);
		SubdomainID curr_subdomain = elem->subdomain_id();
		if( ElemInBlock(curr_subdomain,_block_ids))
		{
			UserDefinedElem newelem;
			newelem._elem = elem;
			transform[elem->id()] = myelemcount;
//			cout << "transform:" << transform[elem->id()] << endl;
			myelemcount++;
			mymesh._userDefinedElem.push_back(newelem);
		}
	}

	int nelems = mymesh._userDefinedElem.size();
//	SideElement newsideelement(NULL, Point(0,0,0), _transmissivity[0], _absorptivity[0], _diffuse_reflectivity[0], _mirrors_reflectivity[0]);
//    _all_element.resize(64,newsideelement);
//    _all_element.reserve(64);
	for (int nelem =0; nelem<nelems; nelem++)
	{
//		const Elem * elem = mesh.elem(nelem);
//		SubdomainID curr_subdomain = elem->subdomain_id();
//		if (curr_subdomain != *_block_ids.begin())
//		      continue;
//		cout << "point:" << elem->centroid() << endl;
		UserDefinedElem * myelem = &(mymesh._userDefinedElem[nelem]);
		const Elem * elem = myelem->_elem;
		int nsides = elem->n_sides();
		myelem->_userDefinedSideElem.resize(nsides);
		for (int nside=0; nside < nsides; nside++)
		{
			UserDefinedSideElem newsideelem;
			myelem->_userDefinedSideElem[nside] = newsideelem;
		}
		myelem->_haveWhichSideElement.resize(nsides, -1);

		for (int nside=0; nside < nsides; nside++)
		{
			UserDefinedSideElem * mysideelem = &(mymesh._userDefinedElem[nelem]._userDefinedSideElem[nside]);
			mysideelem->_elem = elem->side(nside).get();//release();
			mysideelem->_left_element = &(mymesh._userDefinedElem[nelem]);

			if( ElemHaveNeighborInBlock(elem->neighbor(nside), _block_ids) )
//				mysideelem->_right_element = mymesh->_userDefinedElem[elem->neighbor(nside)->id()];
			{
//				cout << "transform:" << transform[elem->neighbor(nside)->id()] << endl;
				mysideelem->_right_element = &(mymesh._userDefinedElem[ transform[elem->neighbor(nside)->id()] ]);
			}

			else
			{
				mysideelem->_right_element = NULL;

				Elem *elem_side = elem->build_side(nside).release();
				int bnd_id = bnd_info.boundary_id(elem, nside);

				int bnd_in_which_group =-1;
				for(int i = 0; i < group.size(); ++i)
				{
					for(int j = 0; j < group_set[i].size(); ++j)
					{
						if(_mesh.getBoundaryID(group_set[i][j]) == bnd_id)
						{
							bnd_in_which_group = i;
							break;
						}
					}
				}

				if(find(boundary_ids.begin(), boundary_ids.end(), bnd_id) == boundary_ids.end())
				{
					delete elem_side;
					continue;
				}

				unsigned int dim = _mesh.dimension();
				FEType fe_type(Utility::string_to_enum<Order>("CONSTANT"), Utility::string_to_enum<FEFamily>("MONOMIAL"));
			    FEBase * _fe_face = (FEBase::build(dim, fe_type)).release();
			    QGauss * _qface = new QGauss(dim-1, FIRST);
				_fe_face->attach_quadrature_rule(_qface);
				_fe_face->reinit(elem, nside);
				const std::vector<Point> normals = _fe_face->get_normals();
				SideElement newsideelement(elem_side, -normals[0], _transmissivity[bnd_in_which_group], _absorptivity[bnd_in_which_group], _diffuse_reflectivity[bnd_in_which_group], _mirrors_reflectivity[bnd_in_which_group]);
				_all_element.push_back(newsideelement);
//				_all_element[count_sideelement]=newsideelement;
				cout << "count_sideelement" << count_sideelement << endl;
				_all_element[count_sideelement]._belong_to_which_elem = myelem;
				_all_element[count_sideelement]._is_which_sideelem = nside;
				myelem->_haveWhichSideElement[nside] = count_sideelement;

				count_sideelement++;

				delete _fe_face;
				delete _qface;

			}
		}
	}
	cout << "size" << _all_element.size() << endl;

	temperature_pow4_bar.resize(_all_element.size(), 0);
	temperature_pow3_bar.resize(_all_element.size(), 0);
	flux_radiation.resize(_all_element.size(), 0);
	flux_radiation_jacobi.resize(_all_element.size(), 0);

	if (_write)
	{
		_communicator.barrier();

		for (int ii=0;ii<_all_element.size();ii++)
		{
			_all_element[ii].local_RD.resize(_all_element.size(), 0);
		}

		int local_p;
		int quotient = _particle_count/n_processors();
		int remainder = _particle_count%n_processors();
		if(processor_id() < remainder)
		{
			local_p = quotient + 1;
		}
		else
		{
			local_p = quotient;
		}

		newcomputeRD(local_p);

		for(int i  = 0; i < _all_element.size(); i++)
		{
			_communicator.sum<Real>(_all_element[i].local_RD);
		}

		_communicator.barrier();

		for (int ii=0;ii<_all_element.size();ii++)
		{
			for (int i=0;i<_all_element.size();i++)
			{
				_all_element[ii].local_RD[i] /= _particle_count;
			}
		}

		_communicator.barrier();

		ofstream rd_write;
		rd_write.open(_filename.c_str(),ios::trunc);
		if(!rd_write.is_open())
			mooseError("Error opening file '" + _filename + "' fail.");

		rd_write << _all_element.size() << " " << _all_element.size() << endl;

		for (int ii=0;ii<_all_element.size();ii++)
		{
			cout << "当前单元中心点：" << _all_element[ii]._elem->centroid() << endl;;
			for (int i=0;i<_all_element.size();i++)
			{
				cout << _all_element[ii]._elem->centroid() << "  RD:  " << _all_element[ii].local_RD[i] << endl;
				rd_write << _all_element[ii].local_RD[i] << " " ;
			}
			rd_write << endl;
		}

		rd_write.close();
	}

	if(_read)
	{
		_communicator.barrier();

		for (int ii=0;ii<_all_element.size();ii++)
		{
			_all_element[ii].local_RD.resize(_all_element.size(), 0);
		}

		ifstream rd_read(_filename.c_str());
		if(!rd_read.good())
			mooseError("Error opening file '" + _filename + "' from  data.");

		string line;
		getline(rd_read, line);
		istringstream iss(line);
		Real f;
		vector<Real> head;
		while(iss >> f)
			head.push_back(f);

		int _num_i_of_RDij = head[0];
		int _num_j_of_RDij = head[1];

		if ( (_num_i_of_RDij!=_all_element.size()) || (_num_j_of_RDij!=_all_element.size()) )
			mooseError("Error file '" + _filename + "' connot march.");

		for (int i = 0; i < _all_element.size(); i++)
		{
			getline(rd_read, line);
			int j=0;
			istringstream transline(line);
			Real d;
			while (transline >> d)
			{
				_all_element[i].local_RD[j] = d;
				j+=1;
			}
		}

		_communicator.barrier();

		for(int i  = 0; i < _all_element.size(); i++)
		{
//			_communicator.sum<Real>(_all_element[i].local_RD);
			_communicator.broadcast<Real>(_all_element[0].local_RD);
		}
		_communicator.barrier();
	}

//	for (int ii=0;ii<_all_element.size();ii++)
//	{
//		cout << "当前单元中心点：" << _all_element[ii]._elem->centroid() << endl;;
//		for (int i=0;i<_all_element.size();i++)
//		{
//			cout << _all_element[ii]._elem->centroid() << "  RD:  " << _all_element[ii].local_RD[i] << endl;
//		}
//	}

//	computeRD();
//	if(processor_id() == 0)
//	ompComputeRD();
//	mpiComputeRD();
//	printfunction();
//	pthreadsComputeRD();
}

bool ComputeTemperatureBar::ElemHaveNeighborInBlock(Elem * elem, set<SubdomainID> block_ids)
{
	if (elem)
	{
//		cout << "elem->processor_id():" << elem->subdomain_id() << endl;
		set<SubdomainID>::iterator it;
		for(it = block_ids.begin(); it != block_ids.end(); it++)
		{
//			cout << "it:" << *it << endl;
			if(elem->subdomain_id() == *it)
			{
				return true;
			}

			else
			{
				continue;
			}
		}
		return false;
	}

	else
	{
		return false;
	}
}

bool ComputeTemperatureBar::ElemInBlock(SubdomainID curr_SubdomainID, set<SubdomainID> block_ids)
{
	for (set<SubdomainID>::iterator it = block_ids.begin(); it != block_ids.end(); ++it)
	{
		if ( curr_SubdomainID != *it )
			continue;
		return true;
	}
	return false;
}

void ComputeTemperatureBar::initialize()
{
//	computeRadiationFlux();
}

void ComputeTemperatureBar::execute()
{
//    MemData mcount;
//    mcount.start();

//    mcount.stop();
//    std::cout  << processor_id() << "MEM (" << COLOR_YELLOW << mcount.delta() << "kB" << COLOR_DEFAULT << ")"<< std::endl;

	int findi=Find_i(_current_side_elem);
	Real temp_pow4_bar(0);
	Real temp_pow3_bar(0);

	for(int _qp = 0; _qp < _q_point.size(); ++_qp)
	{
		temp_pow4_bar += (_JxW[_qp]*pow(_temperature[_qp],4) );
		temp_pow3_bar += (_JxW[_qp]*pow(_temperature[_qp],3) );
	}

	temp_pow4_bar /= _current_side_volume;
	temp_pow3_bar /= _current_side_volume;
	temperature_pow4_bar[findi] = temp_pow4_bar;
	temperature_pow3_bar[findi] = temp_pow3_bar;
//	cout <<  "side_element_centre:" << _all_element[findi]->_elem->centroid() << findi << "    T_pow4_bar:" << temperature_pow4_bar[findi] << endl;

}

void ComputeTemperatureBar::finalize()
{
	computeRadiationFlux();
}

//void ComputeTemperatureBar::computeRD()
//{
//	for(int ii  = 0; ii < _all_element.size(); ii++)
//	{
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=0;
//		}
//
//		SideElement * cse = _all_element[ii];
//
//		for (int j=0;j<_particle_count;j++)
//		{
//			int j_of_RDij=-1;
//
//			j_of_RDij=Find_j_of_RDij(cse, _all_element);
//
//			if (j_of_RDij == -1)
//				continue;
//
//			else
//				_all_element[ii]->_RD[ _all_element[j_of_RDij] ]=_all_element[ii]->_RD[ _all_element[j_of_RDij] ]+1.0;
//		}
//
//		cout << endl << "单元计算结果：" << endl;
//		cout << "当前单元中心点：" << _all_element[ii]->_elem->centroid() <<endl;
//		cout << "normal:" << _all_element[ii]->getSideElementNormal() << endl;
//
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=_all_element[ii]->_RD[ _all_element[i] ]/_particle_count;
//			cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << _all_element[ii]->_RD[ _all_element[i] ] << endl;
//		}
////		mooseError("产生随机位置时不支持的网格形状：");
//	}
//}

void ComputeTemperatureBar::newcomputeRD(int local_particle_count)
{
//	MooseRandom::seed( pow(2,24)*processor_id() );
	for(int ii  = 0; ii < _all_element.size(); ii++)
	{
		SideElement * cse = &(_all_element[ii]);

		for (int j=0;j<local_particle_count;j++)
		{
			int j_of_RDij=-1;

			j_of_RDij=Find_j_of_RDij(cse, _all_element);

			if (j_of_RDij == -1)
				continue;

			else
				_all_element[ii].local_RD[j_of_RDij]=_all_element[ii].local_RD[j_of_RDij]+1.0;
		}

//		mooseError("产生随机位置时不支持的网格形状：");
	}
}

//void ComputeTemperatureBar::ompComputeRD()
//{
//#  pragma omp parallel for num_threads(n_processors()) \
//	schedule(static,1)
//	for(int ii  = 0; ii < _all_element.size(); ii++)
//	{
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=0;
//		}
//
//		SideElement * cse = _all_element[ii];
//
//		for (int j=0;j<_particle_count;j++)
//		{
//			int j_of_RDij=-1;
//
//			j_of_RDij=Find_j_of_RDij(cse, _all_element);
//
//			if (j_of_RDij == -1)
//				continue;
//
//			else
//				_all_element[ii]->_RD[ _all_element[j_of_RDij] ]=_all_element[ii]->_RD[ _all_element[j_of_RDij] ]+1.0;
//		}
//
////		cout << endl << "单元计算结果：" << endl;
////		cout << "当前单元中心点：" << _all_element[ii]->_elem->centroid() <<endl;
////		cout << "normal:" << _all_element[ii]->getSideElementNormal() << endl;
////
////		for (int i=0;i<_all_element.size();i++)
////		{
////			_all_element[ii]->_RD[ _all_element[i] ]=_all_element[ii]->_RD[ _all_element[i] ]/_particle_count;
////			cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << _all_element[ii]->_RD[ _all_element[i] ] << endl;
////		}
////		mooseError("产生随机位置时不支持的网格形状：");
//	}
//
//	for(int ii  = 0; ii < _all_element.size(); ii++)
//	{
//		cout << endl << "单元计算结果：" << endl;
//		cout << "当前单元中心点：" << _all_element[ii]->_elem->centroid() <<endl;
//		cout << "normal:" << _all_element[ii]->getSideElementNormal() << endl;
//
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=_all_element[ii]->_RD[ _all_element[i] ]/_particle_count;
//			cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << _all_element[ii]->_RD[ _all_element[i] ] << endl;
//		}
//		//		mooseError("产生随机位置时不支持的网格形状：");
//	}
//}

//void ComputeTemperatureBar::mpiComputeRD()
//{
//	int local_begin, local_end, local_n;
//
////	int my_rank, comm_sz;
////	MPI_Comm comm;
//
////	MPI_Init(NULL, NULL);
////	comm = MPI_COMM_WORLD;
////	MPI_Comm_size(comm, &comm_sz);
////	MPI_Comm_rank(comm, &my_rank);
//
//	int quotient = _particle_count/n_processors();
//	int remainder = _particle_count%n_processors();
//	if(processor_id() < remainder)
//	{
//		local_n = quotient + 1;
//		local_begin = processor_id()*local_n;
//		local_end = local_begin + local_n;
//	}
//	else
//	{
//		local_n = quotient;
//		local_begin = processor_id()*local_n + remainder;
//		local_end = local_begin + local_n;
//	}
//
//#ifdef DEBUG
//		printf("Proc %d > number of particle = %d, local number = %d\n", processor_id(), _particle_count, _local_particle_count);
//#endif
//
//	for(int ii  = local_begin; ii < local_end; ii++)
//	{
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=0;
//		}
//
//		SideElement * cse = _all_element[ii];
//
//		for (int j=0; j<_particle_count; j++)
//		{
//			int j_of_RDij=-1;
//
//			j_of_RDij=Find_j_of_RDij(cse, _all_element);
//
//			if (j_of_RDij == -1)
//				continue;
//
//			else
//				_all_element[ii]->_RD[ _all_element[j_of_RDij] ] = _all_element[ii]->_RD[ _all_element[j_of_RDij] ] + 1.0;
//		}
//	}
////	MPI_Finalize();
//
////	for (int i=0;i<_all_element.size();i++)
//	for(int i  = local_begin; i < local_end; i++)
//	{
//		cout << endl << "单元计算结果：" << endl;
//		cout << "当前单元中心点：" << _all_element[i]->_elem->centroid() <<endl;
//		cout << "normal:" << _all_element[i]->getSideElementNormal() << endl;
//
//		for (int j=0; j<_all_element.size(); j++)
//		{
//			_all_element[i]->_RD[ _all_element[j] ]=_all_element[i]->_RD[ _all_element[j] ]/_particle_count;
//			cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << _all_element[i]->_RD[ _all_element[j] ] << endl;
//		}
//	}
////	mooseError("产生随机位置时不支持的网格形状：");
//}

//void ComputeTemperatureBar::printfunction()
//{
//	for (int i=0;i<_all_element.size();i++)
//	{
//		cout << endl << "单元计算结果：" << endl;
//		cout << "当前单元中心点：" << _all_element[i]->_elem->centroid() <<endl;
//		cout << "normal:" << _all_element[i]->getSideElementNormal() << endl;
//
//		for (int j=0; j<_all_element.size(); j++)
//		{
//			_all_element[i]->_RD[ _all_element[j] ]=_all_element[i]->_RD[ _all_element[j] ]/_particle_count;
//			cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << _all_element[i]->_RD[ _all_element[j] ] << endl;
//		}
//	}
//}
//
//void ComputeTemperatureBar::mpiComputeRD()
//{
//	cout << "mpimpi" << endl;
//	cout << "n_processors" << n_processors() <<endl;
//	cout << "processor_id" << processor_id() <<endl;
//
//	//	MPI_Finalize();
//
////	mooseError("产生随机位置时不支持的网格形状：");
//}

//void ComputeTemperatureBar::mpiComputeRD()
//{
//	cout << "mpimpi" << endl;
//
//	int local_begin, local_end, local_n;
//
////	int my_rank, comm_sz;
////	MPI_Comm comm;
//
////	MPI_Init(NULL, NULL);
////	comm = MPI_COMM_WORLD;
////	MPI_Comm_size(comm, &comm_sz);
////	MPI_Comm_rank(comm, &my_rank);
//
//	int quotient = _particle_count/n_processors();
//	int remainder = _particle_count%n_processors();
//	if(processor_id() < remainder)
//	{
//		local_n = quotient + 1;
//		local_begin = processor_id()*local_n;
//		local_end = local_begin + local_n;
//	}
//	else
//	{
//		local_n = quotient;
//		local_begin = processor_id()*local_n + remainder;
//		local_end = local_begin + local_n;
//	}
//
//#ifdef DEBUG
//		printf("Proc %d > number of particle = %d, local number = %d\n", processor_id(), _particle_count, _local_particle_count);
//#endif
//
//	for(int ii  = local_begin; ii < local_end; ii++)
//	{
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=0;
//		}
//
//		SideElement * cse = _all_element[ii];
//
//		for (int j=0; j<_particle_count; j++)
//		{
//			int j_of_RDij=-1;
//
//			j_of_RDij=Find_j_of_RDij(cse, _all_element);
//
//			if (j_of_RDij == -1)
//				continue;
//
//			else
//				_all_element[ii]->_RD[ _all_element[j_of_RDij] ] = _all_element[ii]->_RD[ _all_element[j_of_RDij] ] + 1.0;
//		}
//	}
////	MPI_Finalize();
//
////	mooseError("产生随机位置时不支持的网格形状：");
//}

//void* ComputeTemperatureBar::pth_com_RD(void* rank)
//{
//	long my_rank = (long) rank;
//
//	int thread_count = n_processors();
//	int local_begin, local_end, local_n;
//	int quotient = _particle_count/thread_count;
//	int remainder = _particle_count%thread_count;
//	if(my_rank < remainder)
//	{
//		local_n = quotient + 1;
//		local_begin = my_rank*local_n;
//		local_end = local_begin + local_n;
//	}
//	else
//	{
//		local_n = quotient;
//		local_begin = my_rank*local_n + remainder;
//		local_end = local_begin + local_n;
//	}
//
//	for(int ii  = local_begin; ii < local_end; ii++)
//	{
//		for (int i=0;i<_all_element.size();i++)
//		{
//			_all_element[ii]->_RD[ _all_element[i] ]=0;
//		}
//
//		SideElement * cse = _all_element[ii];
//
//		for (int j=0; j<_particle_count; j++)
//		{
//			int j_of_RDij=-1;
//
//			j_of_RDij=Find_j_of_RDij(cse, _all_element);
//
//			if (j_of_RDij == -1)
//				continue;
//
//			else
//				_all_element[ii]->_RD[ _all_element[j_of_RDij] ] = _all_element[ii]->_RD[ _all_element[j_of_RDij] ] + 1.0;
//		}
//	}
//
//	return NULL;
//}
//
//void ComputeTemperatureBar::pthreadsComputeRD()
//{
//	long thread;
//	pthread_t * thread_handles;
//
////	thread_count = strtol(argv[1], NULL, 10);
//	int thread_count = n_processors();
//
//	thread_handles = malloc(thread_count*sizeof(pthread_t));
//
//	for(thread = 0; thread < thread_count; thread++)
//		pthread_create(&thread_handles[thread], NULL, pth_com_RD, (void*) thread);
//
//	for(thread = 0; thread < thread_count; thread++)
//		pthread_join(thread_handles[thread], NULL);
//
//	free(thread_handles);
//
//	for (int i=0;i<_all_element.size();i++)
//	{
//		cout << endl << "单元计算结果：" << endl;
//		cout << "当前单元中心点：" << _all_element[i]->_elem->centroid() <<endl;
//		cout << "normal:" << _all_element[i]->getSideElementNormal() << endl;
//
//		for (int j=0; j<_all_element.size(); j++)
//		{
//			_all_element[i]->_RD[ _all_element[j] ]=_all_element[i]->_RD[ _all_element[j] ]/_particle_count;
//			cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << "        RD:" << _all_element[i]->_RD[ _all_element[j] ] << endl;
//		}
//	}
////		mooseError("产生随机位置时不支持的网格形状：");
//}

void ComputeTemperatureBar::computeRadiationFlux()
{
	Real epsilon=5.67e-8;
	Real Flux_Rad=0.0;

	{
		_communicator.sum<Real>(temperature_pow4_bar);
		_communicator.sum<Real>(temperature_pow3_bar);
	}

	for (int i=0;i<_all_element.size();i++)
	{
		cout << "temperature_pow4_bar:  " << temperature_pow4_bar[i] << endl;
	}

	for (int i=0;i<_all_element.size();i++)
	{
		Flux_Rad=0.0;
		for (int j=0;j<_all_element.size();j++)
		{
			Flux_Rad += (_all_element[j].local_RD[i])*(_all_element[j]._elem->volume())*_all_element[i]._absorptivity*temperature_pow4_bar[j];
		}

//		flux_radiation[i]= epsilon*Flux_Rad/_all_element[i]->_elem->volume()-epsilon*_absorptivity*temperature_pow4_bar[i];
		flux_radiation[i]= epsilon*Flux_Rad/_all_element[i]._elem->volume();
		flux_radiation_jacobi[i]= 4*epsilon*(_all_element[i].local_RD[i])*_all_element[i]._absorptivity*temperature_pow3_bar[i];
//		cout << "side_element_centre:" << _all_element[i]->_elem->centroid() << i << "      Flux:" << flux_radiation[i]  << endl;
	}

	for (int i=0;i<_all_element.size();i++)
	{
		cout << "flux_radiation:  " << flux_radiation[i] << endl;
	}

	for (int i=0;i<_all_element.size();i++)
	{
		temperature_pow4_bar[i] = 0;
		temperature_pow3_bar[i] = 0;
	}
}

int ComputeTemperatureBar::Find_i(const Elem * elem) const
{
	int findi=-1;

	for (int i=0;i<_all_element.size();i++)
	{
		if( (_all_element[i]._elem->centroid()-elem->centroid()).size()<TOLERANCE )
		{
			findi=i;
			break;
		}
	}
	return findi;
}

//int ComputeTemperatureBar::Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point)
//{
//	int j_max=sideelement_vec.size();
//	int j_wanted=-1;
//	Point pp=ray._p0;
//	point=ray._p1;
//
//	for(int j=0; j<j_max; j++)
//	{
////		if( (sideelement_vec[j]->_elem->centroid()-sideelement_i->_elem->centroid()).size()<TOLERANCE )
//		if( ((sideelement_vec[j]->getSideElementNormal())*(sideelement_i->getSideElementNormal()) > TOLERANCE)  )
//			continue;
//
//		else if(!(ray.sideIntersectedByLine(sideelement_vec[j]->_elem,pp)))
//			continue;
//
//		else if((pp-ray._p0).size()>(point-ray._p0).size())
//			continue;
//
//		else
//		{
//			j_wanted=j;
//			point=pp;
//		}
//	}
//	return j_wanted;
//}
//
//int ComputeTemperatureBar::Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec)
//{
//	int j=0;
//	int j_of_RDij=-1;
//	int k=0;
//	bool charge=true;
//	SideElement * current_elem= sideelement_i;
//	RayLine rayline_in;
//	RayLine rayline_out;
//	RayLine* ray_in=&rayline_in;
//	RayLine* ray_out=&rayline_out;
//	Point p(0,0,0);
//
//	rayline_in=(*current_elem).sendRay();
//	while (charge && (k < _max_reflect_count) )
//	{
//		j=Which_SideelementIntersectedByLine( rayline_in, current_elem, sideelement_vec, p);
//
//		if(j==-1)
//			return -1;
//
//		else if(MooseRandom::rand()<sideelement_vec[j]->_absorptivity)
//		{
////			cout << "Absorptivity" << endl;
//			charge=false;
//			j_of_RDij=j;
//			break;
//		}
//
//		else if(MooseRandom::rand()<sideelement_vec[j]->_diffuse_reflectivity)
//		{
////			cout << "Diffuse_Reflectivity" << endl;
//			rayline_out=sideelement_vec[j]->diffuseReflectRay(ray_in,p);
//			current_elem=sideelement_vec[j];
//			rayline_in=rayline_out;
//			j_of_RDij=j;
//			k++;
//			continue;
//		}
//
//		else
//		{
////			cout << "Mirrors_ReflectRay" << endl;
//			rayline_out=sideelement_vec[j]->mirrorsReflectRay(ray_in,p);
//			current_elem=sideelement_vec[j];
//			rayline_in=rayline_out;
//			j_of_RDij=j;
//			k++;
//			continue;
//		}
//	}
//
//	if(!charge)
//		return j_of_RDij;
//
//	else if(k == _max_reflect_count)
//		return j_of_RDij;
//
//	else
//		return -1;
//}

int ComputeTemperatureBar::Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement> &sideelement_vec)
{
	int j=0;
	int j_of_RDij=-1;
	int k=0;
	bool charge=true;
//	vector<SideElement*> sideelement_ve=sideelement_vec;
	SideElement * current_elem= sideelement_i;
	RayLine rayline_in;
	RayLine rayline_out;
	RayLine* ray_in=&rayline_in;
	RayLine* ray_out=&rayline_out;
	Point p(0,0,0);

	rayline_in=(*current_elem).sendRay();
	while (charge && (k < _max_reflect_count) )
	{
		j=findFinalSideId(rayline_in, p, current_elem);

		double rand_num=MooseRandom::rand();

		if(j==-1 || rand_num<=sideelement_vec[j]._transmissivity)
			return -1;

		else if(rand_num<=(sideelement_vec[j]._absorptivity+sideelement_vec[j]._transmissivity))
		{
//			cout << "Absorptivity" << endl;
			charge=false;
			j_of_RDij=j;
			break;
		}

		else if(MooseRandom::rand()<=sideelement_vec[j]._diffuse_reflectivity)
		{
//			cout << "Diffuse_Reflectivity" << endl;
			rayline_out=sideelement_vec[j].diffuseReflectRay(ray_in,p);
			current_elem=&(sideelement_vec[j]);
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}

		else
		{
//			cout << "Mirrors_ReflectRay" << endl;
			rayline_out=sideelement_vec[j].mirrorsReflectRay(ray_in,p);
			current_elem=&(sideelement_vec[j]);
			rayline_in=rayline_out;
			j_of_RDij=j;
			k++;
			continue;
		}
	}

//	cout << "k:" << k << endl;

	if(!charge)
		return j_of_RDij;

	else if(k == _max_reflect_count)
		return j_of_RDij;

	else
		return -1;
}

namespace Roshan
{

int sideIntersectedByLine(const Elem * elem, int not_side, RayLine & ray_line, Point & intersection_point)
{
	unsigned int n_sides = elem->n_sides();

	// A Point to pass to the intersection method
	//  Point intersection_point;

	// Whether or not they intersect
	bool intersect = false;

	unsigned int dim = elem->dim();

	for (unsigned int i=0; i<n_sides; i++)
	{
		if (static_cast<int>(i) == not_side) // Don't search the "not_side"
			continue;

		// Get a simplified side element
		UniquePtr<Elem> side_elem = elem->side(i);

		if (dim == 3)
		{
			// Make a plane out of the first three nodes on the side
			Plane plane(side_elem->point(0), side_elem->point(1), side_elem->point(2));

			// See if they intersect
			intersect = ray_line.intersect(plane, intersection_point);
		}
		else if (dim == 2)
		{
			// Make a Line Segment out of the first two nodes on the side
			RayLine ray_line_side(side_elem->point(0), side_elem->point(1), 1);

			// See if they intersect
			intersect = ray_line.intersect(ray_line_side, intersection_point);
		}
		else // 1D
		{
			// See if the line segment contains the point
			intersect = ray_line.contains_point(side_elem->point(0));

			// If it does then save off that one point as the intersection point
			if (intersect)
				intersection_point = side_elem->point(0);
		}

		if (intersect)
			if (side_elem->contains_point(intersection_point, 1e-12))
				return i;
	}

// Didn't find one
	return -1;
}

/**
 * Returns the side number for elem that neighbor is on
 *
 * Returns -1 if the neighbor can't be found to be a neighbor
 */
int sideNeighborIsOn(const UserDefinedElem * elem, UserDefinedElem * neighbor)
{
  unsigned int n_sides = neighbor->_elem->n_sides();

  for (unsigned int i=0; i<n_sides; i++)
  {
	  if (neighbor->_userDefinedSideElem[i]._right_element == elem)
		  return i;
  }

  return -1;
}

int pointInWhichSide(const Elem * elem, Point & point)
{
	unsigned int n_sides = elem->n_sides();

	for (unsigned int i=0; i<n_sides; i++)
	{
		if( (*(elem->side(i))).contains_point(point) )
			return i;
	}
	return -1;
}

int findFinalSideId(RayLine & ray_line, Point & point, SideElement * sideelement)
//Point findFinalSideId(RayLine & ray_line, const MeshBase & mesh, Point & point)
{
	int incoming_side = -1;
	UserDefinedElem * neighbor;

	const UserDefinedElem * first_elem = sideelement->_belong_to_which_elem;

	if (!first_elem)
	{
		cout << "first_elem" << endl;
		return -1;
	}

	else
	{
		const UserDefinedElem * current_elem = first_elem;
		incoming_side = sideelement->_is_which_sideelem;
//		cout << incoming_side << endl;
		bool charge = true;
		while (charge)
		{
			int intersected_side = sideIntersectedByLine(current_elem->_elem, incoming_side, ray_line, point);
//			cout << intersected_side << endl;
			if (intersected_side != -1) // -1 means that we didn't find any side
			{
				neighbor = current_elem->_userDefinedSideElem[intersected_side]._right_element;

//				cout << neighbor << endl;
				if (neighbor)
				{
//					cout << "intersecedside:" << intersected_side << endl;
					incoming_side = sideNeighborIsOn(current_elem, neighbor);
					current_elem = neighbor;
				}

				else
				{
//					cout << "intersecedside:" << intersected_side << endl;
//					cout << "hhhh" << current_elem->_haveWhichSideElement[intersected_side] << endl;
					return current_elem->_haveWhichSideElement[intersected_side];
				}
			}
			else
			{
//				cout << "else" << endl;
				/*如需增加介质，在此返回吸收单元*/
				return -1;
			}
		}
	}
	return -1;
}

}
