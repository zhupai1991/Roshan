[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  ny = 20
[]

[Variables]
  [./temp]
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = FunctionIC
    function = mmt_function
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
  [./force_kernel]
    type = BodyForce
    function = force_function
    value = 1
    variable = temp
  [../]
[]

[BCs]
  [./left]
    type = FunctionDirichletBC
    variable = temp
    boundary = left
    function = mmt_function
  [../]
  [./right]
    type = FunctionDirichletBC
    variable = temp
    boundary = right
    function = mmt_function
  [../]
  [./top]
    type = FunctionDirichletBC
    variable = temp
    boundary = top
    function = mmt_function
  [../]
 [./bottom]
    type = FunctionDirichletBC
    variable = temp
    boundary = bottom
    function = mmt_function
  [../]
[]

[Materials]
  [./material1]
    type = HeatConductionMaterial
    temperature = temp
    block = 0
    t_list = '0 1'
    k_list = '1 1'
    cp_list = '1 1'
  [../]
[]

[Functions]
  [./mmt_function]
    type = ParsedFunction
    value = sin(2*pi*t)*(x+y)*(x+y)*(x+y)
    #value = t*t*t*(x*x+y*y)
  [../]
  [./force_function]
    type = ParsedFunction
    value = 2*pi*cos(2*pi*t)*(x+y)*(x+y)*(x+y)-12*sin(2*pi*t)*(x+y)
    #value = 3*t*t*(x*x+y*y)-4*t*t*t
  []
[]

[Postprocessors]
  [./L2_error]
    type = ElementL2Error
    variable = temp
    function = mmt_function
  [../]
[]

[Executioner]
  type = Transient
  solve_type = newton
  scheme = crank-nicolson
  dt = 1E-02
  end_time = 2
  num_steps = 10000

  l_tol = 1e-04
  nl_rel_tol = 1e-08
  l_max_its = 10
  nl_max_its = 10
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  [./exodus]
    type = Exodus
    output_on = 'initial timestep_end'
  [../]
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
[]

