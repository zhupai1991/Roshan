[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 2
  ny = 2
  nz = 2
[]

[Variables]
  [./u]
  [../]
[]

[UserObjects]
  [./tempBar]
    type = ComputeTemperatureBar
    boundary = '0 1 2 3 4 5'
    boundary_groups = '0 1 2 3 4, 5'
    block = '0'
    max_reflect_count = 10
    particle_count=100000
    transmissivity='0.0 0.5'
    absorptivity='1.0 0.5'
    diffuse_reflectivity='0.5 0.5'
    mirrors_reflectivity='0.5 0.5'
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
    boundary = left
    value = 0
  [../]
  [./right]
    type = DirichletBC
    variable = u
    boundary = right
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
