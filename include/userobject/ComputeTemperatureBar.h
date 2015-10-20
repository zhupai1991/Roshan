
#pragma once

#include "SideUserObject.h"
#include "RandomInterface.h"
#include "SideElement.h"
#include <vector>
using std::vector;

class RayLine;

class ComputeTemperatureBar :
public SideUserObject,
public RandomInterface
{

public:
	ComputeTemperatureBar(const InputParameters & parameters);
	ComputeTemperatureBar(const std::string & name, InputParameters parameters);
	int Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point);
	int Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec);

protected :
	virtual void initialSetup();
	virtual void initialize(){};
	virtual void finalize();
	virtual void execute();
	virtual void threadJoin(const UserObject & uo){};

	virtual void computeQpProperties();
	void computeRD();

	vector<SideElement*> _all_element;

	int _max_reflect_count;
	int _particle_count;
	Real _absorptivity;
	Real _diffuse_reflectivity;
	Real _mirrors_reflectivity;

	VariableValue &_temperature;
//	map<SideElement*, Real> temperature_bar;
	vector<Real> temperature_bar;

public:
	Real getTemBar(int i) const {return temperature_bar[i];}
};

template<>
InputParameters validParams<ComputeTemperatureBar>();