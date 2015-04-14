#include "RoshanApp.h"
#include "RoshanRevision.h"

#include "Moose.h"
#include "AppFactory.h"
#include "ActionFactory.h"
#include "Syntax.h"

#include "TestIC.h"
#include "strange.h"

#include "HeatConductionTimeDerivative.h"
#include "HeatConductionKernel.h"

#include "HeatConductionMaterial.h"


//DG Kernel
#include "HeatFaceKernel.h"

//DG BC
#include "DGDirichletBC.h"
#include "IsoThermalBC.h"
#include "HeatFluxBC.h"
#include "HeatTransferBC.h"
#include "HeatRadiationBC.h"

#include "CosAuxKernel.h"
//#include "OutputTest.h"

#include "AnalyticSolution.h"

#include "ThetaL2Error.h"

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

  printHeader();
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
	registerInitialCondition(Strange);

	registerKernel(HeatConductionTimeDerivative);
	registerKernel(HeatConductionKernel);

	registerMaterial(HeatConductionMaterial);

	registerDGKernel(HeatFaceKernel);

	registerBoundaryCondition(DGDirichletBC);
	registerBoundaryCondition(IsoThermalBC);
	registerBoundaryCondition(HeatFluxBC);
	registerBoundaryCondition(HeatTransferBC);
	registerBoundaryCondition(HeatRadiationBC);

	registerAux(CosAuxKernel);

	registerFunction(AnalyticSolution);

	registerPostprocessor(ThetaL2Error);

//	registerOutput(OutputTest);
}

void
RoshanApp::associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
}

void RoshanApp::printHeader()
{
	std::string line("*********************************\n\n");
	Moose::out << COLOR_CYAN << line << COLOR_DEFAULT;
	Moose::out << "热传导方程连续有限元解算器 Roshan \n\n";
	Moose::out << "当前版本: " <<  COLOR_MAGENTA << ROSHAN_REVISION << COLOR_DEFAULT << std::endl;
	Moose::out << COLOR_CYAN << line << COLOR_DEFAULT;
}
