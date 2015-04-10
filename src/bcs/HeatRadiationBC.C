
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
  _qc_file(isParamValid("qc_file") ? getParam<std::string>("qc_file") : "")
{
	readqcfile();
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

    _num_pts = head[0];
    _num_time_step = head[1];
    _src_qc.resize(_num_time_step);
    _src_ts.resize(_num_time_step);
	_src_pts.resize(_num_time_step);
    for (int t = 0; t < _num_time_step; ++t)
    {
    	vector<Real> store;
    	for(int i = 0; i < _num_pts ; ++i)
    	{
    		store.resize(_num_pts);
    	   	int j = 0;
    		getline(qc_read, line);
    		istringstream transline(line);
    		Real d;
    		while (transline >> d)
    		{
    			store.push_back(d);
    			store[j] = d;
    			cout<<"store["<<j<<"]="<<store[j]<<endl;
    			j+=1;
    		}
    	   Point pp(store[0],store[1],store[2]);
    		_src_qc[t].push_back(store[3]);
    		_src_ts[t].push_back(store[4]);
    		_src_pts[t].push_back(pp);
    		cout<<"_src_qc["<<t<<"]"<<"["<<i<<"]="<<_src_qc[t][i]<<endl;
    		cout<<"_src_ts["<<t<<"]"<<"["<<i<<"]="<<_src_ts[t][i]<<endl;
    	}
    	_time_step.push_back(store[7]);
	}

}


void HeatRadiationBC::interpolate(std::vector<Real> &qc, std::vector<Real>  &ts, std::vector<Point> &pts, Real t)
{
//	if(_data_file == "")
//	{
//		for(int ip = 0; ip < pts.size(); ++ip)
//		{
//			qc.push_back(getParam<Real>("qc"));
//			ts.push_back(getParam<Real>("ts"));
//		}
//
//		return;
//	}

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
