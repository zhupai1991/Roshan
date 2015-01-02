[Mesh]
  file = cube-hole.e
[]

[Variables]
  [./temp]
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = TestIC
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
    value = 0
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
    value = -1
  [../]
 [./bottom]
    type = NeumannBC
    variable = temp
    boundary = bottom
    value = 0
  [../]
[]

[Materials]
  [./material]
    type = HeatConductionMaterial
    temperature = temp
    block = 1
    t_list =  '1 2'
    k_list =  '1 1'
    cp_list = '1 1'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 1E-01
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

