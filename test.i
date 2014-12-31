[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./temp]
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = ConstantIC
    value = 100
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

[BCs]
  [./left]
    type = DirichletBC
    variable = temp
    boundary = left
    value = 1
  [../]
  [./right]
    type = DirichletBC
    variable = temp
    boundary = right
    value = 1
  [../]
  [./top]
    type = NeumannBC
    variable = temp
    boundary = top
    value = 0
  [../]
 [./bottom]
    type = NeumannBC
    variable = temp
    boundary = bottom
    value = 0
  [../]
[]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 20000000000
  num_steps = 1000

  l_tol = 1e-08
  nl_rel_tol = 1e-09
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

