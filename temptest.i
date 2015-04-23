[Mesh]
  type = GeneratedMesh
  dim = 2
   xmin = -0.5
  ymin = -0.5
  xmax = 0.5
  ymax = 0.5
  nx = 10
  ny = 10
[]

[Variables]
  [./temp]
    family = LAGRANGE
    order = FIRST
  [../]

[]
[BCs]
  [./left]
    type = IsoThermalBC
    variable = temp
    boundary = left
    value = 300
  [../]
  [./right]
    type = IsoThermalBC
    variable = temp
    boundary = right
    value = 300
  [../]
  [./top]
    type = IsoThermalBC
    variable = temp
    boundary = right
    value = 500
  [../]
 [./bottom]
    type = HeatFluxBC
    variable = temp
    boundary = bottom
    value = 0
  [../]
[]

[ICs]

  [./temperature]
    variable = temp
    type = ConstantIC
    value = 300
  [../]
[]

[Kernels]
  [./temp_dT]
    type = TempTimeDerivative
    variable = temp
  [../]
  [./temporal_temp]
    type = TempTestKernel
    variable = temp
  [../]
  
[]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 1E-04
  #scheme = bdf2
  num_steps = 5000

  l_tol = 1e-04
  nl_rel_tol = 1e-05
  l_max_its = 10
  nl_max_its = 10
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
  output_on = 'initial timestep_end'
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

