[Mesh]
  type = GeneratedMesh
  dim = 1
  xmin = 0
  xmax =0.01
  nx = 40
[]

[Variables]
  [./temprature]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temprature
    type = ConstantIC
    value = 300
  [../]
[]

[Kernels]
  [./temporal]
    type = HeatConductionTimeDerivative
    variable = temprature
  [../]
  [./diff]
    type = HeatConductionKernel
    variable = temprature
  [../]
[]

[BCs]
  [./left]
    type = HeatFluxBC
    variable = temprature
    boundary = left
    value = 7.5E+05
  [../]
  [./right]
    type = HeatFluxBC
    variable = temprature
    boundary = right
    value = 0
  [../]
[]

[Materials]
  [./material]
    type = HeatConductionMaterial
    temperature = temprature
    block = 0
    t_list = '300 1300'
    roe_list = '8000 8000'
    k_list = '10 100'
    cp_list = '500 5000'
  [../]
[]


[Functions]
  [./analy_solution]
    type = AnalyticSolution
  [../]
[]


[Postprocessors]
  [./error]
    type = ThetaL2Error
    function = analy_solution
    variable = temprature
  [../]
[] 

[Executioner]
  type = Transient
  solve_type = newton
  scheme = bdf2
  dt = 1E-02
  num_steps = 200

  l_tol = 1e-06
  nl_rel_tol = 1e-08
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

