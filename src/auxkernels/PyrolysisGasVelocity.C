
#include "PyrolysisGasVelocity.h"

template<>
InputParameters validParams<PyrolysisGasVelocity>()
{
  InputParameters params = validParams<AuxKernel>();
  MooseEnum component("x y z");

  params.addRequiredParam<MooseEnum>("component", component, "The desired component of velocity.");
  params.addRequiredCoupledVar("pressure", "The pressure field.");
//  params.addRequiredParam<VariableName>("pressure", "The pressure field.");
  return params;
}

PyrolysisGasVelocity::PyrolysisGasVelocity(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
	_rhog(10),
	_kp( 8.968e-9),
	_viscosity(1.98e-5),
	_poro(0.3),
    _component(getParam<MooseEnum>("component")),
    _pressure_gradient(coupledGradient("pressure"))
{
}

Real
PyrolysisGasVelocity::computeValue()
{

  return -_kp/_viscosity/_poro*_pressure_gradient[_qp](_component);
}
