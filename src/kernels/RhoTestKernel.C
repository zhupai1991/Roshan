#include "../../include/kernels/RhoTestKernel.h"

template<>
InputParameters validParams<RhoTestKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Test rho change");
  return params;
}

 RhoTestKernel::RhoTestKernel(const std::string & name, InputParameters parameters) :
     Kernel(name, parameters),
     _rhov(3000),
     _rhoc(1500),
     _B(1),
     _ER(5),
     _T0(800),
     _nn(2)

{
}

Real RhoTestKernel::computeQpResidual()
{
//	std::cout << _k[_qp] <<std::endl;
  Real tmp = (_u[_qp]-_rhoc)/_rhov;
  Real r(0);
  r = _B*exp(-_ER/_T0)*_rhov*pow(tmp,_nn)*_test[_i][_qp];
  return r;
}

Real RhoTestKernel::computeQpJacobian()
{

  return 0;
}





