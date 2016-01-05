[Mesh]
  dim = 2
  file =  0test_in.e
  uniform_refine = 0
  block_id = '1 2 3 4 5'
  block_name = 'left right bottom in top'
[]

[MeshModifiers]
  [./scale]
    type = Transform
    transform = SCALE
    vector_value = '0.1 0.1 0.1'
  [../]
    [./SideSets1]
    type = SideSetsBetweenSubdomains
    master_block = left
    paired_block = in
    new_boundary = in_left
  [../]
  [./SideSets2]
    type = SideSetsBetweenSubdomains
    master_block = right
    paired_block = in
    new_boundary = in_right
  [../]
  [./SideSets3]
    type = SideSetsBetweenSubdomains
    master_block = bottom
    paired_block = in
    new_boundary = in_bottom
  [../]
  [./SideSets4]
    type = SideSetsBetweenSubdomains
    master_block = top
    paired_block = in
    new_boundary = in_top
  [../]
[]

[Problem]
  kernel_coverage_check = false
[]

[Variables]
  [./temp]
  [../]
[]

[AuxVariables]
  [./heat_flux]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[AuxKernels]
  [./heat_flux_aux_kernel]
    type = HeatFluxAuxKernel
    variable = heat_flux
    boundary = 'in_left in_right in_bottom in_top'
  [../]
[]

[ICs]
  [./temp_ic]
    variable = temp
    type = ConstantIC
    value = 300
    block = '1 2 3 5'
  [../]
  [./temp_ic1]
    variable = temp
    type = ConstantIC
    value = 300
    block = '4'
  [../]
[]

[Kernels]
  [./temporal]
    type = HeatConductionTimeDerivative
    variable = temp
    block = '1 2 3 5'
  [../]
  [./diff]
    type = HeatConductionKernel
    variable = temp
    block = '1 2 3 5'
  [../]
[]

[BCs]
  [./out300]
    type = HeatFluxBC
    variable = temp
    boundary = 'outleft outtop outbottom '
    value = 0
  [../]
  [./out500]
    type = HeatFluxBC
    variable = temp
    boundary = outright
    value = 1000000
  [../]
  [./inner]
    type = HeatRadiationBC
    variable = temp
    boundary = 'in_left in_right in_bottom in_top'
  [../]

[]


[UserObjects]
  [./montecarlo_userobject]
    type = ComputeTemperatureBar
    boundary = 'in_left in_right in_bottom in_top'
    boundary_groups = 'in_left in_right in_bottom in_top'
    block = 'in'
    max_reflect_count = 10
    particle_count=10000
    transmissivity='0.0'
    absorptivity='1.0'
    diffuse_reflectivity='0.5'
    mirrors_reflectivity='0.5'
    temperature = temp
  [../]
[]

[Materials]
  [./material1]
    type = HeatConductionMaterial
    temperature = temp
    block = '1 2 3 4 5'
    t_list =  '100 200'
    roe_list = '400 400'
    k_list =  '5 5'
    cp_list = '100 100'
    sigma = 1
  [../]

  [./material_monte]
    type = MonteCarloRadiationMaterial
    temperature = temp
    monte_carlo = montecarlo_userobject
    epsilon = 1.0
    boundary = 'in_left in_right in_bottom in_top'
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
  [./console]
    type = Console
    perf_log = true
    output_on = 'timestep_end failed nonlinear linear'
  [../]
  [./exodus]
    type = Exodus
    perf_log = true
    output_on = 'initial timestep_end'
  [../]
[./tecplot]
    type = Tecplot
    interval = 1000
    perf_log = true
    output_on = 'timestep_end'
  [../]
[]

