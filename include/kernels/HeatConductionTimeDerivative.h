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
//  const MaterialProperty<Real> & _specific_heat;
//  const MaterialProperty<Real> & _density;
};

template<>
InputParameters validParams<HeatConductionTimeDerivative>();
