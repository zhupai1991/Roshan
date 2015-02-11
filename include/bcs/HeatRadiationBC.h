#pragma once

#include "IntegratedBC.h"

class HeatRadiationBC : public IntegratedBC
{
public:
	HeatRadiationBC(const std::string & name, InputParameters parameters);
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  Real _sigma;
  Real _epsilon;
  Real _ts;
  Real _tw0;
  Real _qc;
};

template<>
InputParameters validParams<HeatRadiationBC>();
