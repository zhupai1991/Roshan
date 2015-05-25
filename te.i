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

[Materials]
  [./material1]
    type = HeatConductionMaterial
    temperature = temprature
    block = 1
    t_list = '300 500 700'
    roe_list = '1000 1000 1000'
    k_list = '1 2 3'
    cp_list = '100 100 100'
    property_file = k(T1).data
    
  [../]
  [./materia2]
    type = HeatConductionMaterial
    temperature = temprature
    block = 2
    t_list = '0 1'
    roe_list = '2000 2000'
    k_list = '50 50'
    cp_list = '100 100'
    property_file = k(T2).data   
  [../]
  [./materia3]
    type = HeatConductionMaterial
    temperature =temprature
    block = 3
    t_list = '0 1'
    roe_list = '1500 1000'
    k_list = '10 10'
    cp_list = '1 1'
    property_file = k(T3).data
    
  [../]
  [./material4]
    type = HeatConductionMaterial
    temperature = temprature
    block = 4
    t_list = '0 1'
    roe_list = '3000 2000'
    k_list = '100 100'
    cp_list = '1 1'
    property_file = k(T4).data
    
  [../]
[]

[Executioner]
  type = Transient
  solve_type = PJFNK
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

