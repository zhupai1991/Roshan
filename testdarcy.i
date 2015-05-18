[Mesh]
 type = GeneratedMesh
  dim = 2
  xmin = 0
  ymin = 0
  xmax = 0.0362
  ymax = 0.0362
  nx = 60
  ny = 60
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
  #[./GasVelocity_x]
    #order = CONSTANT
   # family = MONOMIAL
  #[../]
  #[./GasVelocity_y]
    #order = CONSTANT
    #family = MONOMIAL
  #[../]
  #[./GasVelocity_z]
   # order = CONSTANT
   # family = MONOMIAL
  #[../]
 

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
  #[./gasvelocity_x]
    #type = PyrolysisGasVelocity
    #variable = GasVelocity_x
    #component = x 
    #pressure = pressure  
 # [../]
  #[./gasvelocity_y]
   # type = PyrolysisGasVelocity
   # variable = GasVelocity_y
   # component = y
   # pressure = pressure  
  #[../]
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
    tensor_kp = '0 0 0 
                 0 8.9e-9 0
                 0 0 0'   
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
    type = HeatTransferBC
    variable = temp
    boundary = left
    h = 750
    Ta = 2000
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
    h = 0
    Ta = 0
  [../]
  [./bottom]
    type = HeatTransferBC
    variable = temp
    boundary = bottom
    h = 0
    Ta = 0
  [../]
  [./pressuretop]
    type = HeatFluxBC
    variable =  pressure
    boundary = top
    value = 0
  [../]
 [./pressurebottom]
   type = HeatFluxBC
    variable =  pressure
    boundary = bottom
    value = 0
  [../]
 [./pressureleft]
    type = DirichletBC
    variable =  pressure
    boundary = left
    value = 0
  [../]
 [./pressureright]
    type = HeatFluxBC
    variable =  pressure
    boundary = right
    value = 0
  [../]
[]


[Executioner]
  type = Transient
  solve_type = newton
  dt = 1E-02
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

