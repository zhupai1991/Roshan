#pragma once

#include "Kernel.h"

class TempTestKernel : public Kernel
{
public:

	 TempTestKernel(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
    Real _k;

};

template<>
InputParameters validParams< TempTestKernel>();
