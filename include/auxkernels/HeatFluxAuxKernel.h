
#pragma once
#include "AuxKernel.h"

class HeatFluxAuxKernel : public AuxKernel
{
public:
	HeatFluxAuxKernel(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();

  MaterialProperty<Real> & _flux;
};

template<>
InputParameters validParams<HeatFluxAuxKernel>();
