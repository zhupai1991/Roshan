[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 10
  ny = 10
[]

[Variables]
  [./rho]
    family = LAGRANGE
    order = FIRST
  [../]

  [./temp]
    family = LAGRANGE
    order = FIRST  
  [../]
[]

[ICs]
  [./rho_ics]
    variable = rho
    type = ConstantIC
    value = 3000
  [../]

  [./temp_ics]
    variable = temp
    type = ConstantIC
    value = 300
  [../]
[]

[Kernels]
  [./rho_dT]
    type = TimeDerivative
    variable = rho   
  [../]
  [./source_rho]
    type = RhoTestKernel
    variable = rho
    CoupledVar = temp
  [../]
  [./temp_dt]
    type = TempTimeDerivative
    variable = temp
    CoupledRho = rho
  [../]
  [./diff_temp]
    type = TempTestKernel
    variable = temp
  [../] 
  
[]
[BCs]
  [./left]
    type = HeatFluxBC
    variable = temp
    boundary = left
    value = 0
  [../]
  [./right]
    type = HeatFluxBC
    variable = temp
    boundary = right
    value = 0
  [../]
  [./top]
    type = HeatFluxBC
    variable = temp
    boundary = top
    value = 90000
  [../]
 [./bottom]
    type = IsoThermalBC
    variable = temp
    boundary = bottom
    value = 300
  [../]
[]



[Executioner]
  type = Transient
  solve_type = newton
  dt = 5E-02
  scheme = bdf2
  num_steps = 5000

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

