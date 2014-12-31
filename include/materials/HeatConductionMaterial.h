
#pragma once

#include "Material.h"

class Function;

class HeatConductionMaterial : public Material
{
public:
  HeatConductionMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void computeProperties();

  const bool _has_temp;
  VariableValue & _temperature;

  const Real _my_thermal_conductivity;
  const Real _my_specific_heat;

  MaterialProperty<Real> & _thermal_conductivity;
  MaterialProperty<Real> & _thermal_conductivity_dT;
  Function * const _thermal_conductivity_temperature_function;

  MaterialProperty<Real> & _specific_heat;
  Function * const _specific_heat_temperature_function;

};

template<>
InputParameters validParams<HeatConductionMaterial>();
