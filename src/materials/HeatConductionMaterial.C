#include "HeatConductionMaterial.h"

template<>
InputParameters validParams<HeatConductionMaterial>()
{
  InputParameters params = validParams<Material>();

  params.addRequiredCoupledVar("temperature", "Coupled Temperature");
//  params.addParam<std::vector<Real> >("t_list", "The vector of indepedent values for building the piecewise function");
//  params.addParam<std::vector<Real> >("k_list", "The vector of indepedent values for building the piecewise function");
  return params;
}

HeatConductionMaterial::HeatConductionMaterial(const std::string & name, InputParameters parameters) :
    Material(name, parameters),
    _temperature(coupledValue("temperature")),
    _k(declareProperty<Real>("thermal_conductivity")),
    _k_dT(declareProperty<Real>("thermal_conductivity_dT")),
    _cp(declareProperty<Real>("specific_heat")),
    _cp_dT( declareProperty<Real>("specific_heat_dT"))
//	_piecewise_func(getParam<std::vector<Real> >("independent_vals"), getParam<std::vector<Real> >("independent_vals"))
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
	_console << "111" <<std::endl;
	if(isParamValid("t_list"))
	{
	}
}

void HeatConductionMaterial::computeProperties()
{
  for (unsigned int qp(0); qp < _qrule->n_points(); ++qp)
  {
    _k[_qp] = 1.;
    _k_dT[_qp] = 0;
    _cp[_qp] = 1;
    _cp_dT[_qp] = 0;
    _console << _k[_qp] <<std::endl;
  }
}
