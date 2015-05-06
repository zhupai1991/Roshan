
#include "AuxKernel.h"

class PyrolysisGasVelocity;

template<>
InputParameters validParams<PyrolysisGasVelocity>();

class PyrolysisGasVelocity : public AuxKernel
{
public:
	PyrolysisGasVelocity(const std::string & name, InputParameters parameters);

    virtual ~PyrolysisGasVelocity() {}

protected:
  virtual Real computeValue();
  Real _rhog;
  Real _kp;
  Real _viscosity;
  Real _poro;
  int _component;
  VariableGradient & _pressure_gradient;
};


