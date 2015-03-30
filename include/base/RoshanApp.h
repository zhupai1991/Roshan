#ifndef ROSHANAPP_H
#define ROSHANAPP_H

#include "MooseApp.h"

class RoshanApp;

template<>
InputParameters validParams<RoshanApp>();

class RoshanApp : public MooseApp
{
public:
  RoshanApp(const std::string & name, InputParameters parameters);
  virtual ~RoshanApp();

  virtual void printHeader();

  static void registerApps();
  static void registerObjects(Factory & factory);
  static void associateSyntax(Syntax & syntax, ActionFactory & action_factory);
};

#endif /* ROSHANAPP_H */
