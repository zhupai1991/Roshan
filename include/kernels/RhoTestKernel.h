#pragma once

#include "Kernel.h"

class RhoTestKernel : public Kernel
{
public:

	 RhoTestKernel(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
    Real _rhov;
    Real _rhoc;
    Real _B;
    Real _ER;
    Real _T0;
    int _nn;

};

template<>
InputParameters validParams<RhoTestKernel>();
