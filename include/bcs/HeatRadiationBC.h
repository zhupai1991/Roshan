#pragma once

#include "IntegratedBC.h"
#include "libmesh/meshfree_interpolation.h"

class HeatRadiationBC : public IntegratedBC
{
public:
	HeatRadiationBC(const std::string & name, InputParameters parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  void parsedQcData();
  void interpolate(std::vector<Real> &qc, std::vector<Real>  &ts, std::vector<Point> &pts, Real t);

  Real _sigma;
  Real _epsilon;
  Real _ts;
  Real _tw0;
  Real _qc;

  MooseSharedPointer<InverseDistanceInterpolation<LIBMESH_DIM> > _idi;
  std::string _data_file;
  std::vector<Point> _src_pts;
  std::vector<std::vector<Real> > _src_qc;
  std::vector<std::vector<Real> > _src_ts;
  std::vector<Real> _time_step;
  std::vector<std::string> _field_name;
  int _num_pts;
  int _num_time_step;
};

template<>
InputParameters validParams<HeatRadiationBC>();
