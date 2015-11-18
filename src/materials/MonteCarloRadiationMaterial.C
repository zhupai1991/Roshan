#include "MonteCarloRadiationMaterial.h"
#include <iostream>
#include "FEProblem.h"
#include "libmesh/elem.h"
#include "libmesh/fe.h"
#include "libmesh/string_to_enum.h"
#include "libmesh/quadrature_gauss.h"
#include "libmesh/plane.h"

#include "ComputeTemperatureBar.h"

#include "RayLine.h"
#include "MooseRandom.h"

using namespace std;

template<>
InputParameters validParams<MonteCarloRadiationMaterial>()
{
  InputParameters params = validParams<Material>();
  params += validParams<RandomInterface>();
  params.addCoupledVar("temperature", "温度场");
  params.addRequiredParam<UserObjectName>("monte_carlo", "Monte Carlo");
  params.addParam<Real> ("epsilon", 0.5, "发射率");
  return params;
}

MonteCarloRadiationMaterial::MonteCarloRadiationMaterial(const InputParameters & parameters) :
      Material(parameters),
	  RandomInterface(parameters, *parameters.get<FEProblem *>("_fe_problem"), parameters.get<THREAD_ID>("_tid"), false),
	  _current_side_elem(_assembly.sideElem()),
	  _temperature(coupledValue("temperature")),
	  _uo((getUserObject<ComputeTemperatureBar>("monte_carlo"))),
	  _epsilon(getParam<Real> ("epsilon")),
	  _flux(declareProperty<Real>("heat_flux")),
      _flux_jacobi(declareProperty<Real>("heat_flux_jacobi"))
{
}

void MonteCarloRadiationMaterial::computeQpProperties()
{
//	std::cout << "flu"  << std::endl;
//	std::cout << "热流: " << _uo.getRadiationFlux(_current_side_elem) << std::endl;
//	mooseError("d");
	Real sigma=5.67e-8;

	Real flux =  _uo.getRadiationFlux(_current_side_elem) ;
	Real flux_jacobi =  _uo.getRadiationFluxJacobi(_current_side_elem) ;

	_flux[_qp] = flux-sigma*_epsilon*pow(_temperature[_qp],4);
	_flux_jacobi[_qp] = flux_jacobi-4*sigma*_epsilon*pow(_temperature[_qp], 3);
//	cout << "side_element_centre:" << _current_side_elem->centroid() << "      Flux:" << _flux[_qp]  << endl;
}
