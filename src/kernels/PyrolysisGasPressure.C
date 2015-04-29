#include "PyrolysisGasPressure.h"

template<>
InputParameters validParams<PyrolysisGasPressure>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Test rho change");
//  params.addRequiredCoupledVar("rho","Coupled Variable");
  return params;
}

PyrolysisGasPressure::PyrolysisGasPressure(const std::string & name, InputParameters parameters) :
     Kernel(name, parameters)
//	 _rho(coupledValue("rho")),
//	 _rho_num(coupled("rho"))

{
	for (int i=0; i<3; i++)
	{
		for (int j = 0 ;j < 3 ; j++)
		{
			if (i == j)
			{
				_kp(i, j)=2e-9;
			}
			else
			{
				_kp(i, j)=0;
			}
		}
	}

	std::cout << _kp;
}

Real PyrolysisGasPressure::computeQpResidual()
{
//	std::cout << _k[_qp] <<std::endl;
     Real r(0);
	  r = _kp*_grad_u[_qp]*_grad_test[_i][_qp];
      return r;
}



Real PyrolysisGasPressure::computeQpJacobian()
{
	  Real r(0);
	  return (_kp * _grad_phi[ _j ][ _qp]* _grad_test[ _i][ _qp] ) ;
	  return r;
}





