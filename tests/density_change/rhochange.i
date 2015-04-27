[Mesh]
 type = GeneratedMesh
  dim = 2
  xmin = -0.005
  ymin = -0.005
  xmax = 0.005
  ymax = 0.005
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
    value = 1448
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
    type = PyroRhoChangeKernel
    variable = rho
    temperature = temp
  [../]
  [./temp_dt]
    type = PyroTempTimeDerivative
    variable = temp
    rho = rho
  [../]
  [./diff_temp]
    type = TempDiffusionKernel
    variable = temp
  [../] 
  [./pyrolysis_source]
    type = PyrolysisSource
    variable = temp
    rho = rho
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
     type = HeatTransferBC
    variable = temp
    boundary = right
    h = 1500
    Ta = 1000
  [../]
  [./top]
    type = HeatTransferBC
    variable = temp
    boundary = top
    h = 1500
    Ta = 2000
  [../]
 [./bottom]
    type = HeatFluxBC
    variable = temp
    boundary = bottom
    value = 0
  [../]
[]




[Executioner]
  type = Transient
  solve_type = newton
  dt = 5E-02
  scheme = bdf2
  num_steps = 2

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

