#include "HeatConductionKernel.h"

template<>
InputParameters validParams<HeatConductionKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Compute heat conductivity kernel ");
//  params.addParam<std::string>("diffusion_coefficient_name", "thermal_conductivity", "Property name of the diffusivity (Default: thermal_conductivity");
//  params.addParam<std::string>("diffusion_coefficient_dT_name", "thermal_conductivity_dT", "Property name of the derivative of the diffusivity with respect to the variable (Default: thermal_conductivity_dT");
  return params;
}

HeatConductionKernel::HeatConductionKernel(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters)
//  _thermal_conductivity(getMaterialProperty<Real>(getParam<std::string>("diffusion_coefficient_name"))),
//  _diffusion_coefficient_dT(hasMaterialProperty<Real>(getParam<std::string>("diffusion_coefficient_dT_name")) ? &getMaterialProperty<Real>(getParam<std::string>("diffusion_coefficient_dT_name")) : NULL)
{
}

Real HeatConductionKernel::computeQpResidual()
{
  Real r(0);
  r = _grad_u[_qp] * _grad_test[_i][_qp];
  return r;
}

Real HeatConductionKernel::computeQpJacobian()
{
  Real jac(0);
  jac = _grad_phi[_j][_qp] * _grad_test[_i][_qp];
  return jac;
}
