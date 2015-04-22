[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./rho]
    family = LAGRANGE
    order = FIRST
  [../]

[]

[ICs]

  [./temp_rho]
    variable = rho
    type = ConstantIC
    value = 3000
  [../]
[]

[Kernels]
  [./rho_dT]
    type = TimeDerivative
    variable = rho
  [../]
  [./temporal_temp]
    type = Rhotestkernel
    variable = rho
  [../]
  
[]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 5E-02
  scheme = bdf2
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

