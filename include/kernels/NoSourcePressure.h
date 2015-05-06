#pragma once

#include "Kernel.h"

class NoSourcePressure : public Kernel
{
public:

	NoSourcePressure(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
    Real _kp;
    Real _viscosity;
    Real _poro;

};

template<>
InputParameters validParams<NoSourcePressure>();
