[Mesh]
  file = test1.exo
  uniform_refine = 1
  second_order = true
[]

[Variables]
  [./temprature]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[AuxVariables]
  [./a]
  [../]
[]

[AuxKernels]
  [./aukernel_for_a]
    type = CosAuxKernel
    variable = a
    T = temprature
    coef = 1
  []
[]

[ICs]
  [./temp_ic]
    variable = temprature
    type = Strange
    value = 0
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
  [./llleft]
    type = IsoThermalBC
    variable = temprature
    boundary = left
    value = 10
  [../]
  [./right]
    type = IsoThermalBC
    variable = temprature
    boundary = right
    value = 100
  [../]
  [./top]
    type = HeatFluxBC
    variable = temprature
    boundary = top
    value = 0
  [../]
 [./bottom]
    type = HeatFluxBC
    variable = temprature
    boundary = bottom
    value = 0
  [../]
[]

[Materials]
  [./material1]
    type = HeatConductionMaterial
    temperature = temprature
    block = 1
    t_list = '0 1 2'
    k_list = '0.01 0.02 0.03'
    cp_list = '1 1 1'
    property_file = k(T).data
    
  [../]
  [./materia2]
    type = HeatConductionMaterial
    temperature = temprature
    block = 2
    t_list = '0 1'
    k_list = '1 1'
    cp_list = '1 1'
    property_file = k(T).data
    
  [../]
  [./materia3]
    type = HeatConductionMaterial
    temperature =temprature
    block = 3
    t_list = '0 1'
    k_list = '10 10'
    cp_list = '1 1'
    property_file = k(T).data
    
  [../]
  [./material4]
    type = HeatConductionMaterial
    temperature = temprature
    block = 4
    t_list = '0 1'
    k_list = '100 100'
    cp_list = '1 1'
    property_file = k(T).data
    
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
  exodus = true
  output_on = 'initial timestep_end'
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

