#pragma once

#include "TimeDerivative.h"

class TempTimeDerivative : public TimeDerivative
{
public:

	TempTimeDerivative(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
    Real _cp;
    VariableValue & _Rho;
    unsigned int _Rho_num;
};

template<>
InputParameters validParams< TempTimeDerivative>();
