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

[Outputs]
  exodus = true
[]
