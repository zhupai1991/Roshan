
#pragma once

#include "SideUserObject.h"
#include "RandomInterface.h"
#include "SideElement.h"
#include <vector>
#include <set>
using std::vector;
using std::set;

namespace Roshan
{
int findFinalSideId(RayLine & ray_line, Point & point, SideElement * sideelement);
}

class RayLine;

class ComputeTemperatureBar :
public SideUserObject,
public RandomInterface
{
public:
	ComputeTemperatureBar(const InputParameters & parameters);
	int Which_SideelementIntersectedByLine(RayLine& ray, SideElement * sideelement_i, vector<SideElement*> sideelement_vec, Point & point);
	int Find_j_of_RDij(SideElement * sideelement_i, vector<SideElement*> sideelement_vec);
	int Find_i(const Elem * elem) const;
	Real getTemBar(int i) const {return temperature_pow4_bar[i];}
	Real getRadiationFlux(int i)  const  {return flux_radiation[i];}
	Real getRadiationFlux(const Elem * elem)  const  {return flux_radiation[Find_i(elem)];}
	Real getRadiationFluxJacobi(const Elem * elem)  const  {return flux_radiation_jacobi[Find_i(elem)];}
	~ComputeTemperatureBar();
protected :
	virtual void initialSetup();
	virtual void initialize();
	virtual void finalize();
	virtual void execute();
	virtual void threadJoin(const UserObject & uo){};

	virtual void computeRadiationFlux();
//	void computeRD();
	void ompComputeRD();
//	void mpiComputeRD();
//	void printfunction();
//	void pthreadsComputeRD();
//	void* pth_com_RD(void* rank);

	vector<SideElement*> _all_element;

	int _max_reflect_count;
	int _particle_count;
	vector<Real> _transmissivity;
	vector<Real> _absorptivity;
	vector<Real> _diffuse_reflectivity;
	vector<Real> _mirrors_reflectivity;

	set<SubdomainID> _block_ids;
	bool ElemHaveNeighborInBlock(Elem * elem, set<SubdomainID> block_ids);
	bool ElemInBlock(SubdomainID curr_SubdomainID, set<SubdomainID> block_ids);

	VariableValue &_temperature;
	vector<Real> temperature_pow4_bar;
	vector<Real> temperature_pow3_bar;
	vector<Real> flux_radiation;
	vector<Real> flux_radiation_jacobi;
};


template<>
InputParameters validParams<ComputeTemperatureBar>();
