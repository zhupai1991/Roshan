
#pragma once
#include "AuxKernel.h"

class GetSigma : public AuxKernel
{
public:
	GetSigma(const InputParameters & parameters);

protected:
  virtual Real computeValue();

  const MaterialProperty<Real> & _sigma;
};

template<>
InputParameters validParams<GetSigma>();
