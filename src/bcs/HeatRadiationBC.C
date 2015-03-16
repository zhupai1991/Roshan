
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
  params.addRequiredParam<Real>("ts", " ");
  params.addRequiredParam<Real>("tw0", " ");
  params.addRequiredParam<Real>("qc", " ");
  params.addParam<std::string>("data_file", "qc from data file");

  return params;
}

HeatRadiationBC::HeatRadiationBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  _sigma(getParam<Real>("sigma")),
  _epsilon(getParam<Real>("epsilon")),
  _ts(getParam<Real>("ts")),
  _tw0(getParam<Real>("tw0")),
  _qc(getParam<Real>("qc")),
  _data_file(isParamValid("data_file") ? getParam<std::string>("data_file") : "")
{
	if(_data_file != "")
	{
		parsedQcData();
		InverseDistanceInterpolation<LIBMESH_DIM> * idi = new InverseDistanceInterpolation<LIBMESH_DIM>(this->comm(), 8, 2);
//		InverseDistanceInterpolation<LIBMESH_DIM> * idi = new RadialBasisInterpolation<LIBMESH_DIM>(this->comm(), -1);
		idi->get_source_points() = _src_pts;
		idi->get_source_vals() = _src_qc;
	    std::vector<std::string> field_vars;
	    field_vars.push_back("qc");
	    idi->set_field_variables(field_vars);
	    idi->prepare_for_use();

		using namespace std;
	    vector<string> field_name;
	    field_name.push_back("qc");
	    vector<Point> tgt_pts;
	    tgt_pts.push_back(Point(0, 0.5 ,0));
	    vector<Real> tgt_vals;
	    tgt_vals.resize(1);
	    idi->interpolate_field_data(field_name, tgt_pts, tgt_vals);

	    cout << tgt_vals.front() << endl;
	    mooseError("stop");
	}

}

Real HeatRadiationBC::computeQpResidual()
{
  Real tw = _u[_qp];
  Real tw4 = tw*tw*tw*tw;
  return _test[_i][_qp]*(_ts - tw)/(_ts - _tw0)*_qc - _epsilon*_sigma*tw4;
}

Real HeatRadiationBC::computeQpJacobian()
{
  Real tw = _u[_qp];
  Real tw3 = tw*tw*tw;
  return _test[_i][_qp]*_phi[_j][_qp]*(-_qc/(_ts - _tw0) - 4*_epsilon*_sigma*tw3);
}

void HeatRadiationBC::parsedQcData()
{
	using namespace std;
	ifstream qc_file(_data_file.c_str());
	if(!qc_file.good())
	    mooseError("Error opening file '" + _data_file + "' from qc data.");

	string line;
	getline(qc_file, line);
	istringstream iss(line);
	Real  f;
	iss >> f;
	while(getline(qc_file, line))
	{
		istringstream iss(line);
		Real f;
		vector<Real> data;
		while(iss >> f)
			data.push_back(f);

		_src_pts.push_back(Point(data[0], data[1], data[2]));
		_src_qc.push_back(data[3]);
	}
}

