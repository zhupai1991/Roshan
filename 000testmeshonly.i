[Mesh]
  dim = 2
  file = 07test.exo
  uniform_refine = 1
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


