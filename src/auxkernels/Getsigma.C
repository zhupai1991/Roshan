
#include "GetSigma.h"

template<>
InputParameters validParams<GetSigma>()
{
  InputParameters params = validParams<AuxKernel>();
  return params;
}

GetSigma::GetSigma(const InputParameters &parameters) :
    AuxKernel(parameters),
	_sigma(getMaterialProperty<Real>("sigma"))
{
}

Real GetSigma::computeValue()
{
	Real sigma = _sigma[_qp];
   return sigma;
}
