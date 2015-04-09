#include "HeatConductionMaterial.h"
#include<iostream>
using std::cout;
using std::endl;

template<>
InputParameters validParams<HeatConductionMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temperature", "Coupled Temperature");
  params.addParam<std::vector<Real> >("t_list", "The vector of temperature values for building the piecewise function");
  params.addParam<std::vector<Real> >("roe_list", "The vector of rho values for building the piecewise function");
params.addParam<std::vector<Real> >("k_list", "The vector of thermal conductivity values for building the piecewise function");
  params.addParam<std::vector<Real> >("cp_list", "The vector of specific heat values for building the piecewise function");
  params.addParam<std::string>("property_file","The name of the property file");
  return params;
}

HeatConductionMaterial::HeatConductionMaterial(const std::string & name, InputParameters parameters) :
      Material(name, parameters),
    _property_file(isParamValid("property_file") ? getParam<std::string>("property_file") : "" ),
    _temperature(coupledValue("temperature")),
    _k(declareProperty<Real>("thermal_conductivity")),
    _k_dT(declareProperty<Real>("thermal_conductivity_dT")),
    _cp(declareProperty<Real>("specific_heat")),
    _cp_dT( declareProperty<Real>("specific_heat_dT"))

{
		if (_property_file == "")
		{
			_roe_list= getParam<std::vector<Real> >("roe_list");
			_T_list = getParam<std::vector<Real> >("t_list");
			_cp_list= getParam<std::vector<Real> >("cp_list");
			_k_list = getParam<std::vector<Real> > ("k_list");
//		_func_k_T=LinearInterpolation(getParam<std::vector<Real> >("t_list"), getParam<std::vector<Real> >("k_list"));
//		_func_cp_T=LinearInterpolation(getParam<std::vector<Real> >("t_list"), getParam<std::vector<Real> >("cp_list"));
		}
		else
		{
			readFile();
		}
	_func_roe_T =  LinearInterpolation(_T_list, _roe_list);
	_func_k_T =  LinearInterpolation(_T_list, _k_list);
	_func_cp_T =  LinearInterpolation(_T_list, _cp_list);
}

void HeatConductionMaterial::readFile()
{
	    using namespace std;
//        cout <<"reading file"<<endl;
		ifstream read_file(_property_file.c_str());
		if(!read_file.good())
		    mooseError("Error opening file '" +_property_file + "' from qc data.");

		    string line;
			getline(read_file, line);
			istringstream(line) >> _tpoint;

			for(int i = 0; i < _tpoint; ++i)
			{
				int j=0;
				vector<Real> data;
				getline(read_file, line);
				istringstream iss(line);
				Real f;
				while(iss >> f)
				{
				 data.push_back(f);
//				 cout<<"data["<<j<<"]="<<data[j]<<endl;
				  j+=1;
				}
                _T_list.push_back(data[0]);
                _roe_list.push_back(data[1]);
                _k_list.push_back(data[2]);
                _cp_list.push_back(data[3]);
//				cout<<"T["<<i<<"]= "<<_T_list[i]<<endl;
//				cout<<"roe["<<i<<"]= "<<_roe_list[i]<<endl;
//				cout<<"k["<<i<<"]= "<<_k_list[i]<<endl;
//				cout<<"cp["<<i<<"]= "<<_cp_list[i]<<endl;
			}

			check();
}

void HeatConductionMaterial::computeProperties()
{
  Real epsi = 1E-08;
  for (unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    _k[qp] = _func_k_T.sample(_temperature[qp]);
    _k_dT[qp] =  (_func_k_T.sample(_temperature[qp]+epsi) - _func_k_T.sample(_temperature[qp]-epsi))/2/epsi ;
    _cp[qp] =  _func_cp_T.sample(_temperature[qp]);
    _cp_dT[qp] = (_func_cp_T.sample(_temperature[qp]+epsi) - _func_cp_T.sample(_temperature[qp]-epsi))/2/epsi ;
  }
}

void HeatConductionMaterial::check()
{
	if(_tpoint != _T_list.size() )
		mooseError("温度点个数不匹配");
}
