project('Flow', 'cpp')
inc_dir = include_directories('include')
vulkan = dependency('vulkan')
glfw = dependency('glfw3')
glm = dependency('glm')

sources = []
subdir('src')
executable('Flow', sources, include_directories : inc_dir, dependencies : [vulkan, glfw, glm])