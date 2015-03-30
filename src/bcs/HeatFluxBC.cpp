#include "HeatFluxBC.h"

template<>
InputParameters validParams<HeatFluxBC>()
{
  InputParameters params = validParams<IntegratedBC>();
  params.addParam<Real>("value", 0.0, "The value of the gradient on the boundary.");
  return params;
}

HeatFluxBC::HeatFluxBC(const std::string & name, InputParameters parameters) :
  IntegratedBC(name, parameters),
  _value(getParam<Real>("value"))
{}

Real HeatFluxBC::HeatFluxBC()
{
  return -_test[_i][_qp]*_value;
}

