
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
		public Material,
		public RandomInterface
{
public:
	MonteCarloRadiationMaterial(const std::string & name, InputParameters parameters);
	~MonteCarloRadiationMaterial(){};
//	int Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point);
//	int Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec);

protected:
//  virtual void initialSetup();
  virtual void computeQpProperties();
  virtual void computeProperties();
//  void computeRD();
//
//  vector<SideElement*> _all_element;
//
//  int _max_reflect_count;
//  int _particle_count;
//  Real _absorptivity;
//  Real _diffuse_reflectivity;
//  Real _mirrors_reflectivity;

  VariableValue &_temperature;
  const ComputeTemperatureBar & _uo;

};

template<>
InputParameters validParams<MonteCarloRadiationMaterial>();
