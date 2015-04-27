#pragma once

#include "Kernel.h"

class TempDiffusionKernel : public Kernel
{
public:

	 TempDiffusionKernel(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
    Real _k;
};

template<>
InputParameters validParams< TempDiffusionKernel>();
