
#include "HeatRadiationBC.h"
#include "libmesh/meshfree_interpolation.h"
#include "libmesh/system.h"
#include "libmesh/radial_basis_interpolation.h"

template<>
InputParameters validParams<HeatRadiationBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<Real>("sigma", " ");
  params.addRequiredParam<Real>("epsilon", " ");
  params.addRequiredParam<Real>("tw0", " ");
  params.addParam<Real>("ts", "const ts ");
  params.addParam<Real>("qc", "const qc");
  params.addParam<std::string>("data_file", "field data from file");
  params.addParam<std::string>("qc_file", "qc data from file");
  params.addParam<Real>("power", 2, "distance power");
  params.addParam<int>("interp_pts", 8, "number of interpolate points");

  return params;
}

HeatRadiationBC::HeatRadiationBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  _sigma(getParam<Real>("sigma")),
  _epsilon(getParam<Real>("epsilon")),
  _tw0(getParam<Real>("tw0")),
  _data_file(isParamValid("data_file") ? getParam<std::string>("data_file") : ""),
  _qc_file(isParamValid("qc_file") ? getParam<std::string>("qc_file") : "")
{
	using namespace std;
	readqcfile();
//	if(_data_file != "")
//	{
//		parsedFieldData();
//
//		Real power = getParam<Real>("power");
//		int n_interp_pts = getParam<int>("interp_pts");
//		_idi = MooseSharedPointer<InverseDistanceInterpolation<LIBMESH_DIM> > (new InverseDistanceInterpolation<LIBMESH_DIM>(this->comm(), n_interp_pts, power));
//
//		_field_name.push_back("qc");
//		_field_name.push_back("ts");
//		_idi->set_field_variables(_field_name);
//		_idi->get_source_points() = _src_pts;

//		vector<Real> qc, ts;
//		vector<Point> pts;
//		pts.push_back(Point(0, 0.55, 0));
//		interpolate(qc, ts, pts, -1);
//        cout << *_idi << endl;
//	}
}

void HeatRadiationBC::computeResidual()
{
	std::vector<Real> qc, ts;
	std::vector<Point> pts;
	for(int ip = 0; ip < _q_point.size(); ++ip)
	{
		pts.push_back(_q_point[ip]);
	}
	interpolate(qc, ts, pts, _t);

	IntegratedBC::computeResidual();
}

void HeatRadiationBC::computeJacobian()
{
	std::vector<Real> qc, ts;
	std::vector<Point> pts;
	for(int ip = 0; ip < _q_point.size(); ++ip)
	{
		pts.push_back(_q_point[ip]);
	}
	interpolate(qc, ts, pts, _t);

	IntegratedBC::computeJacobian();
}

Real HeatRadiationBC::computeQpResidual()
{
  Real tw = _u[_qp];
  Real tw4 = Utility::pow<4>(tw);
  return _test[_i][_qp]*(_ts[_qp] - tw)/(_ts[_qp] - _tw0)*_qc[_qp] - _epsilon*_sigma*tw4;
}

Real HeatRadiationBC::computeQpJacobian()
{
  Real tw = _u[_qp];
  Real tw3 = Utility::pow<3>(tw);
  return _test[_i][_qp]*_phi[_j][_qp]*(-_qc[_qp]/(_ts[_qp] - _tw0) - 4*_epsilon*_sigma*tw3);
}

void HeatRadiationBC::parsedFieldData()
{
	using namespace std;
	ifstream qc_file(_data_file.c_str());
	if(!qc_file.good())
	    mooseError("Error opening file '" + _data_file + "' from qc data.");

	string line;
	getline(qc_file, line);
	istringstream(line) >> _num_pts;
	for(int i = 0; i < _num_pts; ++i)
	{
		getline(qc_file, line);
		istringstream iss(line);
		Real f;
		vector<Real> data;
		while(iss >> f)
			data.push_back(f);

		_src_pts.push_back(Point(data[0], data[1], data[2]));
	}

	getline(qc_file, line);
	istringstream(line) >> _num_time_step;
	_src_qc.resize(_num_time_step);
	_src_ts.resize(_num_time_step);
	for(int t = 0; t < _num_time_step; ++t)
	{
		Real time_step;
		getline(qc_file, line);
		istringstream(line) >> time_step;
		_time_step.push_back(time_step);
		for(int i = 0; i < _num_pts; ++i)
		{
			getline(qc_file, line);
			istringstream iss(line);
			Real f;
			vector<Real> data;
			while(iss >> f)
				data.push_back(f);

			_src_qc[t].push_back(data[0]);
			_src_ts[t].push_back(data[1]);
		}
	}
	for(int t = 1; t < _num_time_step; ++t)
		if(_time_step[t] <= _time_step[t-1])
			mooseError("时间序列必须递增！");
}

void HeatRadiationBC::readqcfile()
{
	using namespace std;
	ifstream qc_read(_qc_file.c_str());
    if(!qc_read.good())
	     mooseError("Error opening file '" + _qc_file + "' from  data.");
            string line;
            getline(qc_read, line);
    		istringstream iss(line);
    		Real f;
    		vector<Real> head;
    		while(iss >> f)
    		head.push_back(f);
    	   for(int i = 0; i < (head[0] * head[1]); ++i)
    	   {
    		   int j = 0;
    		    std::vector<Real>_qc_d;
    		    std::vector<Real>_ts_d;
    		   getline(qc_read, line);
   		       istringstream transline(line);
   		       Real d;
   		       vector<Real> store;
   		       while (transline >> d)
   		       {
   		    	   store.push_back(d);
   		    	   cout<<"store["<<j<<"]="<<store[j]<<endl;
 				  j+=1;
                }
   		      Point pp(store[0],store[1],store[2]);
		    _qc_d.push_back(store[3]);
		    _qc_d.push_back( store[7]);
		    _ts_d.push_back( store[4]);
		    _ts_d.push_back( store[7]);
		    _src_qc.push_back( _qc_d);
		    _src_ts.push_back(_ts_d);
		    _src_pts.push_back(pp);

    	   }


}


void HeatRadiationBC::interpolate(std::vector<Real> &qc, std::vector<Real>  &ts, std::vector<Point> &pts, Real t)
{
	if(_data_file == "")
	{
		for(int ip = 0; ip < pts.size(); ++ip)
		{
			qc.push_back(getParam<Real>("qc"));
			ts.push_back(getParam<Real>("ts"));
		}

		return;
	}

	int lower, upper;
	if(t < _time_step.front())
	{
		lower = 0;
		upper = 0;
	}
	else if(t > _time_step.back())
	{
		lower = _num_time_step - 1;
		upper = _num_time_step - 1;
	}
	else
	{
	    std::vector<double>::iterator up = std::lower_bound(_time_step.begin(), _time_step.end(), t);
	    upper = static_cast<unsigned int>(std::distance(_time_step.begin(), up));
	    if (_time_step[upper] == t)
	      lower = upper;
	    else
	      lower = upper - 1;
	}
	Real lam = 0.5;
	if(lower != upper)
	{
		lam = (_time_step[upper] - t)/(_time_step[upper] - _time_step[lower]);
	}
	std::vector<Real> src_vals, tgt_vals;
    for(int i  = 0; i < _num_pts; ++i)
    {
    	src_vals.push_back(lam*_src_qc[lower][i] + (1-lam)*_src_qc[upper][i]);
    	src_vals.push_back(lam*_src_ts[lower][i] + (1-lam)*_src_ts[upper][i]);
    }
	_idi->get_source_vals() = src_vals;
	_idi->interpolate_field_data(_field_name, pts, tgt_vals);

	int num_val = _field_name.size();
	for(int ip = 0; ip < pts.size(); ++ip)
	{
		qc.push_back(tgt_vals[num_val*ip]);
		ts.push_back(tgt_vals[num_val*ip+1]);
	}
}
