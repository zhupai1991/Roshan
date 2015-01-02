#pragma once

#include "Kernel.h"
#include "Material.h"

class HeatConductionKernel : public Kernel
{
public:

  HeatConductionKernel(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  MaterialProperty<Real> & _k;
  MaterialProperty<Real> & _k_dT;
};

template<>
InputParameters validParams<HeatConductionKernel>();
