
#pragma once

#include "InitialCondition.h"

class TestIC : public InitialCondition
{
public:
  TestIC(const std::string & name, InputParameters parameters);
  virtual Real value(const Point & p);
};

template<>
InputParameters validParams<TestIC>();
