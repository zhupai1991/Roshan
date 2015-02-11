#include "RoshanApp.h"

#include "Moose.h"
#include "AppFactory.h"
#include "ActionFactory.h"
#include "Syntax.h"

#include "TestIC.h"

#include "HeatConductionTimeDerivative.h"
#include "HeatConductionKernel.h"

#include "HeatConductionMaterial.h"


//DG Kernel
#include "HeatFaceKernel.h"
#include "HeatTransferBC.h"

//DG BC
#include "DGDirichletBC.h"

template<>
InputParameters validParams<RoshanApp>()
{
  InputParameters params = validParams<MooseApp>();

  params.set<bool>("use_legacy_uo_initialization") = false;
  params.set<bool>("use_legacy_uo_aux_computation") = false;
  return params;
}

RoshanApp::RoshanApp(const std::string & name, InputParameters parameters) :
    MooseApp(name, parameters)
{
  srand(processor_id());

  Moose::registerObjects(_factory);
  RoshanApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  RoshanApp::associateSyntax(_syntax, _action_factory);
}

RoshanApp::~RoshanApp()
{
}

void
RoshanApp::registerApps()
{
  registerApp(RoshanApp);
}

void
RoshanApp::registerObjects(Factory & factory)
{
	registerInitialCondition(TestIC);

	registerKernel(HeatConductionTimeDerivative);
	registerKernel(HeatConductionKernel);

	registerMaterial(HeatConductionMaterial);

	registerDGKernel(HeatFaceKernel);

	registerBoundaryCondition(DGDirichletBC);
	registerBoundaryCondition(HeatTransferBC);
}

void
RoshanApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}
