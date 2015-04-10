#pragma once

#include "IntegratedBC.h"
#include "libmesh/meshfree_interpolation.h"

using std::vector;
using std::string;

class HeatRadiationBC : public IntegratedBC
{
public:
	HeatRadiationBC(const std::string & name, InputParameters parameters);

protected:
  virtual void computeResidual();
  virtual void computeJacobian();
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  void interpolate(std::vector<Real> &qc, std::vector<Real>  &ts, std::vector<Point> &pts, Real t);


  Real _sigma;
  Real _epsilon;
  Real _tw0;
  Real _ts[20];
  Real _qc[20];

  MooseSharedPointer<InverseDistanceInterpolation<LIBMESH_DIM> > _idi;
  string _data_file;
 string _qc_file;
  vector<vector<Real> > _src_qc;
  vector<vector<Real> > _src_ts;
  vector<vector<Point> >_src_pts;
  vector<Real> _time_step;
  vector<string> _field_name;
  int _num_pts;
  int _num_time_step;
private:
  void readqcfile();
};

template<>
InputParameters validParams<HeatRadiationBC>();
