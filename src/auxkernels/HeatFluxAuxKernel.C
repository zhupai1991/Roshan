
#include "HeatFluxAuxKernel.h"

template<>
InputParameters validParams<HeatFluxAuxKernel>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

HeatFluxAuxKernel::HeatFluxAuxKernel(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
	_flux(getMaterialProperty<Real>("heat_flux"))
{
}

Real HeatFluxAuxKernel::computeValue()
{
  return _flux[_qp];
}
