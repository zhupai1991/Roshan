#include "HeatConductionMaterial.h"

template<>
InputParameters validParams<HeatConductionMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temperature", "Coupled Temperature");
  params.addParam<std::vector<Real> >("t_list", "The vector of temperature values for building the piecewise function");
  params.addParam<std::vector<Real> >("k_list", "The vector of thermal conductivity values for building the piecewise function");
  params.addParam<std::vector<Real> >("cp_list", "The vector of specific heat values for building the piecewise function");
  params.addRequiredCoupledVar("a","just a test");
  return params;
}

HeatConductionMaterial::HeatConductionMaterial(const std::string & name, InputParameters parameters) :
    Material(name, parameters),
    _temperature(coupledValue("temperature")),
    _k(declareProperty<Real>("thermal_conductivity")),
    _k_dT(declareProperty<Real>("thermal_conductivity_dT")),
    _cp(declareProperty<Real>("specific_heat")),
    _cp_dT( declareProperty<Real>("specific_heat_dT")),
	_func_k_T(getParam<std::vector<Real> >("t_list"), getParam<std::vector<Real> >("k_list")),
	_func_cp_T(getParam<std::vector<Real> >("t_list"), getParam<std::vector<Real> >("cp_list"))
{
//  if (_thermal_conductivity_temperature_function && !_has_temp)
//  {
//    mooseError("Must couple with temperature if using thermal conductivity function");
//  }
//  if (isParamValid("thermal_conductivity") && _thermal_conductivity_temperature_function)
//  {
//    mooseError("Cannot define both thermal conductivity and thermal conductivity temperature function");
//  }
//  if (_specific_heat_temperature_function && !_has_temp)
//  {
//    mooseError("Must couple with temperature if using specific heat function");
//  }
//  if (isParamValid("specific_heat") && _specific_heat_temperature_function)
//  {
//    mooseError("Cannot define both specific heat and specific heat temperature function");
//  }
	if(isParamValid("t_list"))
	{
	}
}

void HeatConductionMaterial::computeProperties()
{
  Real epsi = 1E-08;
  for (unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    _k[qp] = _func_k_T.sample(_temperature[qp]);
    _k_dT[qp] = (_func_k_T.sample(_temperature[qp]+epsi) - _func_k_T.sample(_temperature[qp]-epsi))/2/epsi ;
    _cp[qp] = _func_cp_T.sample(_temperature[qp]);
    _cp_dT[qp] = (_func_cp_T.sample(_temperature[qp]+epsi) - _func_cp_T.sample(_temperature[qp]-epsi))/2/epsi ;
  }
}
