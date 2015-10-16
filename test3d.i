[Mesh]
  type = GeneratedMesh
  dim = 3
  xmin = -0.05
  ymin = -0.05
  zmin = -0.05
  xmax = 0.05
  ymax = 0.05
  zmax = 0.05
  nx = 10
  ny = 10
  nz = 10

[Variables]
  [./temp]
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = TestIC
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
    type = HeatTransferBC
    variable = temp
    boundary = left
    h = 1000
    Ta = 5000
  [../]
  [./right]
    type = HeatTransferBC
    variable = temp
    boundary = right
    h = 1000
    Ta = 100
  [../]
  [./top]
    type = HeatTransferBC
    variable = temp
    boundary = top
    h = 1000
    Ta = 700
  [../]
 [./bottom]
    type = DirichletBC
    variable = temp
    boundary = bottom
    value = 300
  [../]
 [./front]
    type = HeatTransferBC
    variable = temp
    boundary = front
    h = 1000
    Ta = 100
  [../]
  [./back]
   type = HeatTransferBC
    variable = temp
    boundary = back
    h = 1500
    Ta = 2000
  [../]
[]

[Materials]
  [./material]
    type = HeatConductionMaterial
    temperature = temp
    block = 0
    t_list =  '100 200'
    roe_list = '4000 4000'
    k_list =  '1 1'
    cp_list = '500 500'
    sigma = 1
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

