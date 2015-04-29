
#include "RhoTimeDerivative.h"

template<>
InputParameters validParams<RhoTimeDerivative>()
{
  InputParameters params = validParams<AuxKernel>();
  params.addRequiredCoupledVar("rho","Coupled Variable");
  return params;
}

RhoTimeDerivative::RhoTimeDerivative(const std::string & name, InputParameters parameters) :
    AuxKernel(name, parameters),
	_rho_dt(coupledDot("rho")),
    _rho_num(coupled("rho"))
{
}

Real RhoTimeDerivative::computeValue()
{
   return abs(_rho_dt[_qp]);
}
