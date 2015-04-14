#pragma once

#include "TimeDerivative.h"
#include "Material.h"


class HeatConductionTimeDerivative : public TimeDerivative
{
public:
  HeatConductionTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  MaterialProperty<Real> & _cp;
  MaterialProperty<Real> & _cp_dT;
  MaterialProperty<Real> & _rho;
  MaterialProperty<Real> & _rho_dT;
};

template<>
InputParameters validParams<HeatConductionTimeDerivative>();
