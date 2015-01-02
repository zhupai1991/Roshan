#include "TestIC.h"

template<>
InputParameters validParams<TestIC>()
{
  InputParameters params = validParams<InitialCondition>();
  return params;
}

TestIC::TestIC(const std::string & name, InputParameters parameters) :
    InitialCondition(name, parameters)
{}

Real TestIC::value(const Point & p)
{
  return std::sin(p.size());
}
