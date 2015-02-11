
#include "HeatRadiationBC.h"

template<>
InputParameters validParams<HeatRadiationBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addRequiredParam<Real>("sigma", " ");
  params.addRequiredParam<Real>("epsilon", " ");
  params.addRequiredParam<Real>("ts", " ");
  params.addRequiredParam<Real>("tw0", " ");
  params.addRequiredParam<Real>("qc", " ");

  return params;
}

HeatRadiationBC::HeatRadiationBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  _sigma(getParam<Real>("sigma")),
  _epsilon(getParam<Real>("epsilon")),
  _ts(getParam<Real>("ts")),
  _tw0(getParam<Real>("tw0")),
  _qc(getParam<Real>("qc"))
{}

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

