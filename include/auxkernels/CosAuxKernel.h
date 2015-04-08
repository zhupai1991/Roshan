
#pragma once
#include "AuxKernel.h"

class CosAuxKernel : public AuxKernel
{
public:
	CosAuxKernel(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();

  Real _coef;
  VariableValue &_temperature;
};

template<>
InputParameters validParams<CosAuxKernel>();
