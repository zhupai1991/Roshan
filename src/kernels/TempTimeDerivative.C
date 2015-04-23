#include "../../include/kernels/TempTimeDerivative.h"

template<>
InputParameters validParams<TempTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  params.addClassDescription("Test temperature change");
  params.addRequiredCoupledVar("CoupledRho","Coupled Variable");
  return params;
}

TempTimeDerivative::TempTimeDerivative(const std::string & name, InputParameters parameters) :
  TimeDerivative(name, parameters),
   _cp(30),
   _Rho(coupledValue("CoupledRho")),
   _Rho_num(coupled("CoupledRho"))
{
}

Real TempTimeDerivative::computeQpResidual()
{
//	std::cout << _k[_qp] <<std::endl;
  Real r(0);
//   r = _Rho0 *_cp *_du_dot_du[_qp]*_test[_i][_qp];
   r = _Rho[_qp]*_cp *_u_dot[_qp]*_test[_i][_qp];
  return r;
}

Real TempTimeDerivative::computeQpJacobian()
{

	return  _Rho[_qp] *_cp *  _test[_i][_qp]*_phi[_j][_qp]*_du_dot_du[_qp];
}





