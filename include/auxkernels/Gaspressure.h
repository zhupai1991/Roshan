
#pragma once
#include "AuxKernel.h"

class Gaspressure : public AuxKernel
{
public:
	Gaspressure(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeValue();

private:
    Real _R;
    Real _mg;
    VariableValue & _rhog;
    unsigned int _rhog_num;
    VariableValue & _T;
    unsigned int _T_num;
};

template<>
InputParameters validParams<Gaspressure>();
