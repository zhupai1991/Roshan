#include "HeatConductionTimeDerivative.h"

template<>
InputParameters validParams<HeatConductionTimeDerivative>()
{
  InputParameters params = validParams<TimeDerivative>();
  return params;
}

HeatConductionTimeDerivative::HeatConductionTimeDerivative(const std::string & name, InputParameters parameters) :
    TimeDerivative(name, parameters),
    _cp(getMaterialProperty<Real>("specific_heat")),
    _cp_dT(getMaterialProperty<Real>("specific_heat_dT")),
	_density(1)
//   _density(getMaterialProperty<Real>("density"))
{
}

Real HeatConductionTimeDerivative::computeQpResidual()
{
  return _density*_cp[_qp]*_u_dot[_qp]*_test[_i][_qp];
}

Real HeatConductionTimeDerivative::computeQpJacobian()
{
  Real r(0);
//  r = _density*(_cp[_qp] + _u[_qp]*_cp_dT[_qp])*_phi[_j][_qp]*_du_dot_du[_qp]*_test[_i][_qp];
  r = _density*(_cp[_qp])*_phi[_j][_qp]*_du_dot_du[_qp]*_test[_i][_qp];
  return r;
}
