[Mesh]
 type = GeneratedMesh
  dim = 3
  xmin = 0
  ymin = 0
  zmin = 0
  xmax = 0.0254
  ymax = 0.0762
  zmax = 0.0254
  nx = 10
  ny = 20
  nz = 10
  uniform_refine = 1 
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
  [./rhog]
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
  [./GasVelocity_x]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./GasVelocity_y]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./GasVelocity_z]
    order = CONSTANT
    family = MONOMIAL
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
  [./rhog_ics]
    variable = rhog
    type = ConstantIC
    value = 0
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
  [./gasvelocity_x]
    type = PyrolysisGasVelocity
    variable = GasVelocity_x
    component = x 
    pressure = pressure  
  [../]
  [./gasvelocity_y]
    type = PyrolysisGasVelocity
    variable = GasVelocity_y
    component = y
    pressure = pressure   
  [../]
  [./gasvelocity_z]
    type = PyrolysisGasVelocity
    variable = GasVelocity_z
    component = z 
    pressure = pressure   
  [../]

[]

[Kernels]
  [./rho_change]
    type = PyrolysisDensityKernel
    variable = rho
    temperature = temp
  [../]
  [./temptimederivative]
    type = PyroTempTimeDerivative
    variable = temp
    rho = rho
  [../]
  [./tempdiffusion]
    type = TempDiffusionKernel
    variable = temp
  [../]
  [./gasconvection]
    type = GasConvection
    variable = temp
    pressure = pressure   
  [../]
  [./ Pyrolysis_GasDensity]
    type =  PyrolysisGasRhochange
    variable = rhog
    rho = rho
  [../] 
  [./ Darcy_GasPressure]
    type =  NoSourcePressure
    variable = pressure
  [../]
  [./ Pyrolysis_Gassource]
    type =  PyrolysisGasSource
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
    Ta = 0
  [../]
  [./top]
    type = HeatTransferBC
    variable = temp
    boundary = top
    h = 750
    Ta = 2000
  [../]
  [./bottom]
    type = DirichletBC
    variable = temp
    boundary = bottom
    value = 300
  [../]
  [./front]
    type = HeatFluxBC
    variable = temp
    boundary = front
    value = 0
  [../]
  [./back]
    type = HeatFluxBC
    variable = temp
    boundary = back
    value = 0
  [../]
  [./pressuretop]
    type = DirichletBC
    variable =  pressure
    boundary = top
    value = 0
  [../]
 [./pressurebottom]
    type = DirichletBC
    variable =  pressure
    boundary = bottom
    value = 0
  [../]
[]


[Executioner]
  type = Transient
  solve_type = newton
  dt = 1E-02
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

