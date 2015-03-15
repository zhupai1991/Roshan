#pragma once

#include "IntegratedBC.h"

class HeatRadiationBC : public IntegratedBC
{
public:
	HeatRadiationBC(const std::string & name, InputParameters parameters);
protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  void parsedQcData();

  Real _sigma;
  Real _epsilon;
  Real _ts;
  Real _tw0;
  Real _qc;

  std::string _data_file;
  std::vector<Point> _src_pts;
  std::vector<Real> _src_qc;
};

template<>
InputParameters validParams<HeatRadiationBC>();
