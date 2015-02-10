[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 100
  ny = 100
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
    value = 10
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
    #epsilon = -1
    sigma = 6
  [../]
[]

[BCs]
  active = 'all'

  [./all]
    type = DGFunctionDiffusionDirichletBC
    variable = temp
    boundary = '0 1 2 3'
    function = exact_fn
    epsilon = -1
    sigma = 6
  [../]

  [./bcs]
    type = DGDirichletBC
    variable = temp
    boundary = '0 1 2 3'
    value = 1
  [../]
[]

[Functions]
  [./exact_fn]
    type = ParsedGradFunction
    value = 1
    grad_x = 0
    grad_y = 0
  [../]

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
  dt = 1E-1
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

