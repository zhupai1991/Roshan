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

 [./pressure]
    family = LAGRANGE
    order = FIRST
  [../]
[]

[AuxVariables]
  [./rho_dt]
    order = FIRST
    family = LAGRANGE
  [../]

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
 [./pressure_ics]
    variable = pressure
    type = ConstantIC
    value = 0
  [../]
[]

[AuxKernels]
  [./rho_dt]
    type = RhoTimeDerivative
    variable =rho_dt
    rho = rho   
  [../]
[]

[Kernels]
  [./rho_change]
    type = PyrolysisDensityKernel
    variable = rho
    temperature = temp
  [../]
  [./energy]
    type = PyrolysisEnergeKernel
    variable = temp
    rho = rho
  [../]
  [./ Pyrolysis_GasPressure]
    type =  PyrolysisGasPressure
    variable = pressure
  [../]
  [./Pyrolysis_GasSource]
    type = PyrolysisGasSource
    variable = pressure
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
    h = 0
    Ta = 1000
  [../]
  [./top]
    type = HeatTransferBC
    variable = temp
    boundary = top
    h = 1500
    Ta = 1500
  [../]
 [./bottom]
    type = HeatFluxBC
    variable = temp
    boundary = bottom
    value = 0
  [../]
 [./pressureleft]
    type = HeatFluxBC
    variable = pressure
    boundary = left
    value = 0
  [../]
  [./pressureright]
    type = HeatFluxBC
    variable = pressure
    boundary = right
    value = 0
  [../]
  [./pressuretop]
     type = DirichletBC
    variable = pressure
    boundary = top
    value = 100000
  [../]
 [./pressurebottom]
   type =  DirichletBC
    variable = pressure
    boundary = bottom
    value = 50000
  [../]
[]


[Executioner]
  type = Transient
  solve_type = newton
  dt = 5E-02
  scheme = bdf2
  num_steps = 2000

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

