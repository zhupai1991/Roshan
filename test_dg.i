[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./temp]
    family = MONOMIAL
    order = FIRST
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = ConstantIC
    value = 300
  [../]
[]

[Kernels]
  [./temporal]
    type = HeatConductionTimeDerivative
    variable = temp
  [../]
  [./diff]
    type = HeatConductionKernel
    variable = temp
  [../]
[]

[DGKernels]
  active = 'dg_diff'
  [./dg_diff]
    type = HeatFaceKernel
    variable = temp
  [../]
[]

[BCs]
  [./bcs]
    type = DGDirichletBC
    variable = temp
    boundary = '0 1 2 3'
    value = 2000
    sigma = 100
  [../]
[]

[Materials]
  [./material]
    type = HeatConductionMaterial
    temperature = temp
    block = 0
    t_list = '0 0.5 1'
    k_list = '1 1 1'
    cp_list = '1 1 1'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 1E-03
  num_steps = 10000

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

