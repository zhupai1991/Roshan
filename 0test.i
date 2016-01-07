[Mesh]
  dim = 2
  file = 07test.exo
  uniform_refine = 3
  block_id = '1 2 3 4 5'
  block_name = 'left right bottom in top'
[]
[MeshModifiers]
  [./scale]
    type = Transform
    transform = SCALE
    vector_value = '1 1 1'
  [../]
  [./SideSets1]
    type = SideSetsBetweenSubdomains
    master_block = in
    paired_block = left
    new_boundary = in_left
  [../]
  [./SideSets2]
    type = SideSetsBetweenSubdomains
    master_block = in
    paired_block = right
    new_boundary = in_right
  [../]
  [./SideSets3]
    type = SideSetsBetweenSubdomains
    master_block = in
    paired_block = bottom
    new_boundary = in_bottom
  [../]
  [./SideSets4]
    type = SideSetsBetweenSubdomains
    master_block = in
    paired_block = top
    new_boundary = in_top
  [../]
[]

[Variables]
  [./u]
    order = FIRST
  [../]
[]

[UserObjects]
  [./montecarlo_material]
    type = ComputeTemperatureBar
    boundary = 'in_left in_right in_bottom in_top'
    boundary_groups = 'in_left in_right in_bottom in_top'
    block = 'in'
    max_reflect_count = 10
    particle_count=100000
    transmissivity='0.0'
    absorptivity='1.0'
    diffuse_reflectivity='0.5'
    mirrors_reflectivity='0.5'
    temperature = u
  [../]
[]

[Materials]
  [./material]
    type = MonteCarloRadiationMaterial
    temperature = u
    boundary = 'in_left in_right in_bottom in_top'
    monte_carlo = montecarlo_material
  [../]

  
[]

[Kernels]
  [./diff]
    type = Diffusion
    variable = u
  [../]
[]

[BCs]
  [./left]
    type = DirichletBC
    variable = u
    boundary = outleft
    value = 300
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = outright
    value = 0
  [../]
  [./bottom]
    type = HeatFluxBC
    variable = u
    boundary = outbottom
    value = 1
  [../]
[]

[Executioner]
  type = Steady
  solve_type = 'PJFNK'
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  exodus = true
[]
