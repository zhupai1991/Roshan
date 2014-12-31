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
//  MaterialProperty<Real> & _thermal_conductivity;
//  MaterialProperty<Real> * const _diffusion_coefficient_dT;

};

template<>
InputParameters validParams<HeatConductionKernel>();
