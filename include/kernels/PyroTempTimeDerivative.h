#pragma once

#include "TimeDerivative.h"

class PyroTempTimeDerivative : public TimeDerivative
{
public:

	PyroTempTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
    Real _cp;
    VariableValue & _rho;
    unsigned int _rho_num;
};

template<>
InputParameters validParams< PyroTempTimeDerivative>();
