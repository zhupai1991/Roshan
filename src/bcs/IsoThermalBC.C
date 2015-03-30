
#include "IsoThermalBC.h"

template<>
InputParameters validParams<IsoThermalBC>()
{
  InputParameters parameter = validParams<NodalBC>();
  parameter.addRequiredParam<Real>("value", "Value of the BC");
  return parameter;
}

IsoThermalBC::IsoThermalBC(const std::string & name, InputParameters parameters) :
  NodalBC(name, parameters),
  _value(getParam<Real>("value"))
{}

Real IsoThermalBC::computeQpResidual()
{
  return _u[_qp] - _value;
}
