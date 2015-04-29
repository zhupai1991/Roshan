#pragma once

#include "Kernel.h"

class PyrolysisGasPressure : public Kernel
{
public:

	PyrolysisGasPressure(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
//  VariableValue & _rho;
//  unsigned int _rho_num;
  RealTensor _kp;
};

template<>
InputParameters validParams< PyrolysisGasPressure>();
