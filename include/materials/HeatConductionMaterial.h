
#pragma once

#include "Material.h"
#include "LinearInterpolation.h"

class Function;

class HeatConductionMaterial : public Material
{
public:
  HeatConductionMaterial(const std::string & name, InputParameters parameters);

protected:
  virtual void computeProperties();

private:
  VariableValue & _temperature;
  MaterialProperty<Real> & _k;
  MaterialProperty<Real> & _k_dT;
  MaterialProperty<Real> & _cp;
  MaterialProperty<Real> & _cp_dT;
  LinearInterpolation _piecewise_func;
};

template<>
InputParameters validParams<HeatConductionMaterial>();
