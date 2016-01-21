[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
  #second_order = true
[]

[Variables]
  [./temp]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = ConstantIC
    value = 300
    block = '0'
  [../]
[]

[Kernels]
  [./temporal]
    type = HeatConductionTimeDerivative
    variable = temp
    block = '0'
  [../]
  [./diff]
    type = HeatConductionKernel
    variable = temp
    block = '0'
  [../]
[]

[BCs]
  [./out300]
    type = HeatFluxBC
    variable = temp
    boundary = '1 2 3'
    value = 0
  [../]
  [./out500]
    type = HeatFluxBC
    variable = temp
    boundary = '0'
    value = 1000000
  [../]
[]

[Materials]
  [./material1]
    type = HeatConductionMaterial
    temperature = temp
    block = '0'
    t_list =  '100 200'
    roe_list = '400 400'
    k_list =  '5 5'
    cp_list = '100 100'
    sigma = 1
  [../]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 1E-02
  num_steps = 1000

  l_tol = 1e-04
  nl_rel_tol = 1e-05
  l_max_its = 10
  nl_max_its = 10
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'bjacobi boomeramg'
[]

[Outputs]
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
  [./exodus]
    type = Exodus
    perf_log = true
    output_on = 'initial timestep_end'
  [../]
[]
