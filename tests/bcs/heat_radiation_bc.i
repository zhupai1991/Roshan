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
    type = HeatRadiationBC
    variable = temp
    boundary = left
    sigma = 1E-06
    epsilon = 1E-06
    ts = 400
    tw0 = 300
    qc = 0.01
  [../]
  [./right]
    type = DirichletBC
    variable = temp
    boundary = right
    value = 2000
  [../]
  [./top]
    type = DirichletBC
    variable = temp
    boundary = top
    value = 2000
  [../]
 [./bottom]
    type = DirichletBC
    variable = temp
    boundary = bottom
    value = 2000
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
  dt = 1E-02
  num_steps = 1

  l_tol = 1e-04
  nl_rel_tol = 1e-05
  l_max_its = 10
  nl_max_its = 10
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  [./exodus]
    type = Exodus
    #refinements = 1
    output_on = 'initial timestep_end'
  [../]
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

