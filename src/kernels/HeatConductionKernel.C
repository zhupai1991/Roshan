#include "HeatConductionKernel.h"

template<>
InputParameters validParams<HeatConductionKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Compute heat conductivity kernel ");
  return params;
}

HeatConductionKernel::HeatConductionKernel(const std::string & name, InputParameters parameters) :
  Kernel(name, parameters),
  _k(getMaterialProperty<Real>("thermal_conductivity")),
  _k_dT(getMaterialProperty<Real>("thermal_conductivity_dT"))
{
}

Real HeatConductionKernel::computeQpResidual()
{
  Real r(0);
  r = _k[_qp]*_grad_u[_qp] * _grad_test[_i][_qp];
  return r;
}

Real HeatConductionKernel::computeQpJacobian()
{
  Real jac(0);
  jac = _k[_qp]*_grad_phi[_j][_qp] * _grad_test[_i][_qp];
  jac += _grad_u[_qp] * _grad_test[_i][_qp]*_k_dT[_qp]*_phi[_j][_qp];
  return jac;
}
