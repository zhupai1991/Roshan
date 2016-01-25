
#pragma once

#include "Material.h"
#include "SideUserObject.h"
#include "RandomInterface.h"
#include "SideElement.h"
#include <vector>
using std::vector;

class RayLine;
class ComputeTemperatureBar;

class MonteCarloRadiationMaterial :
		public Material
//		public RandomInterface
{
public:
	MonteCarloRadiationMaterial(const InputParameters & parameters);
	~MonteCarloRadiationMaterial(){};

protected:
  virtual void computeQpProperties();

  const Elem * & _current_side_elem;

  VariableValue &_temperature;
  const ComputeTemperatureBar & _uo;
  Real _epsilon;
  MaterialProperty<Real> & _flux;
  MaterialProperty<Real> & _flux_jacobi;

};

template<>
InputParameters validParams<MonteCarloRadiationMaterial>();
