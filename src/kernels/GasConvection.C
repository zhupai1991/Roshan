
#include "GasConvection.h"

template<>
InputParameters validParams<GasConvection>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("pressure", "The pressure field.");
  return params;
}

GasConvection::GasConvection(const std::string & name, InputParameters parameters) :
		Kernel(name, parameters),
	_rhog(10),
	_kp( 8.968e-10),
	_viscosity(1.98e-5),
	_poro(0.3),
    _pressure_gradient(coupledGradient("pressure")),
    _pressure_gradient_num(coupled("pressure"))
{
}

Real
GasConvection::computeQpResidual()
{
  return -_rhog*_kp/_viscosity/_poro*_pressure_gradient[_qp]*_grad_u[_qp]*_test[_i][_qp];
}
Real GasConvection::computeQpJacobian()
{
  return -_rhog*_kp/_viscosity/_poro*_pressure_gradient[_qp]*_grad_phi[_j][_qp]*_test[_i][_qp];
}
Real  GasConvection::computeQpOffDiagJacobian(unsigned int jvar)
 {


	 if ( jvar == _pressure_gradient_num)
	    {
	 	  return -_rhog*_kp/_viscosity/_poro*_grad_phi[_j][_qp]*_grad_u[_qp]*_test[_i][_qp];
		  }
		 else
		  {
			 return 0;
		  }


 }

