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
    type = RandomIC
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
    type = IsoThermalBC
    variable = temp
    boundary = left
    value = 0
  [../]
  [./right]
    type = IsoThermalBC
    variable = temp
    boundary = right
    value = 1
  [../]
  [./top]
    type = HeatFluxBC
    variable = temp
    boundary = top
    value = 0
  [../]
 [./bottom]
    type = HeatFluxBC
    variable = temp
    boundary = bottom
    value = 0
  [../]
[]

[Materials]
  [./material]
    type = HeatConductionMaterial
    temperature = temp
    block = 0
    t_list = '0 0.5 1'
    k_list = '1 50 100'
    cp_list = '1 1 1'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = newton
  dt = 0.001
  num_steps = 100

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

