project('Flow', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=c++20'])

cmake = import('cmake')

inc_dir = []
subdir('include')

vulkan = dependency('vulkan')
glfw = dependency('glfw3', required : false)
glm = dependency('glm', required : false)

if not glfw.found()
    glfw_cmake = cmake.subproject('glfw')
    glfw = glfw_cmake.dependency('glfw')
endif

if not glm.found()
    glm_cmake = cmake.subproject('glm')
    glm = glm_cmake.dependency('glm')
endif

sources = []
subdir('src')
executable('Flow', sources, include_directories : inc_dir, dependencies : [vulkan, glfw, glm])