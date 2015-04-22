[Mesh]
  file = test1.exo
[]

[Variables]
  [./temprature]
    family = LAGRANGE
    order = FIRST
  [../]

  [./density]
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

  [./temp_rho]
    variable = density
    type = ConstantIC
    value = 1
  [../]
[]

[Kernels]
  [./temporal_temp]
    type = HeatConductionTimeDerivative
    variable = temprature
  [../]
  [./diff]
    type = HeatConductionKernel
    variable = temprature
  [../]

  [./temporal_rho]
    type = TimeDerivative
    variable = density
  [../]

  [./temporal_rho]
    type = DensityKernel
    variable = density
  [../]
[]

[BCs]
  [./llleft]
    type = IsoThermalBC
    variable = temprature
    boundary = left
    value = 300
  [../]
  [./right]
    type = IsoThermalBC
    variable = temprature
    boundary = right
    value = 300
  [../]
  [./top]
    type = HeatFluxBC
    variable = temprature
    boundary = top
    value = 1e6
  [../]
 [./bottom]
    type = HeatFluxBC
    variable = temprature
    boundary = bottom
    value = -1e4
  [../]
[]



  [./material1]
    type = PyrolysisMaterial
    temperature = temprature
    block = '1 2 3 4'
    rhov = 3000
    rhoc = 1500
    t_list = '300 500 700'
    k_list = '1 2 3'
    cp_list = '100 100 100'
  [../]
[]

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

