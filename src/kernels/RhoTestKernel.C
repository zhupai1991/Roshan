#include "../../include/kernels/RhoTestKernel.h"

template<>
InputParameters validParams<RhoTestKernel>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Test rho change");
  params.addRequiredCoupledVar("CoupledVar","Coupled Variable");
  return params;
}

 RhoTestKernel::RhoTestKernel(const std::string & name, InputParameters parameters) :
     Kernel(name, parameters),
     _rhov(3000),
     _rhoc(1500),
     _B(1E13),
     _ER(3E4),
     _nn(3),
	 _T(coupledValue("CoupledVar")),
    _T_num(coupled("CoupledVar"))

{
}



 Real RhoTestKernel::computeQpResidual()
{
//	std::cout << _k[_qp] <<std::endl;
//  Real tmp = (_u[_qp]-_rhoc)/_rhov;
//  Real r(0);
//  r = _B*exp(-_ER/_T0)*_rhov*pow(tmp,_nn)*_test[_i][_qp];
  return  Source(_u[_qp],_T[_qp]) *_test[_i][_qp];
}

Real RhoTestKernel::computeQpJacobian()
{
	Real epsi = 1E-08;
	Real source = Source(_u[_qp],_T[_qp]);
	Real source_new =  Source(_u[_qp]+epsi,_T[_qp]);
  return (source_new-source)/epsi*_phi[_j][_qp]*_test[_i][_qp];
}

 Real  RhoTestKernel::computeQpOffDiagJacobian(unsigned int jvar)
 {


	 if ( jvar == _T_num)
	    {
		 Real epsi = 1E-08;
	     Real source = Source(_u[_qp],_T[_qp]);
	 	 Real source_new =  Source(_u[_qp],_T[_qp]+epsi);
	    return (source_new-source)/epsi*_phi[_j][_qp]*_test[_i][_qp];
	    }
	 else
	 {
		 return (0);
	 }
 }
Real RhoTestKernel::Source(Real rho,Real T)
{
	 if (T>600)
	 {
      if ((rho-_rhoc)<1000)
    	  {
    	  return 0;
    	  }
      else
      {
	  Real tmp = (rho-_rhoc)/_rhov;
	  return  _B*exp(-_ER / T)*_rhov*pow(tmp,_nn);
      }
	 }
	 else
	 {
		 return 0;
	 }
}





