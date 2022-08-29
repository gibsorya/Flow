project('Flow', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++20'])
inc_dir = []
subdir('include')
vulkan = dependency('vulkan')
glfw = dependency('glfw3')
glm = dependency('glm')

sources = []
subdir('src')
executable('Flow', sources, include_directories : inc_dir, dependencies : [vulkan, glfw, glm])