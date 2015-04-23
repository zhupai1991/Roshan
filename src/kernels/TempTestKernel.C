#include "../../include/kernels/TempTestKernel.h"

template<>
InputParameters validParams<TempTestKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Test rho change");
  return params;
}

TempTestKernel::TempTestKernel(const std::string & name, InputParameters parameters) :
     Kernel(name, parameters),
     _k(50)
{
}

Real TempTestKernel::computeQpResidual()
{
//	std::cout << _k[_qp] <<std::endl;
  Real r(0);
  r = _k*_grad_u[_qp]*_grad_test[_i][_qp];
  return r;
}

Real TempTestKernel::computeQpJacobian()
{
	  return _k*_grad_phi[_j][_qp] * _grad_test[_i][_qp];
}





