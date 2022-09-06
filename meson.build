project('Flow', 'c', 'cpp', default_options : ['c_std=c11', 'cpp_std=gnu++20', 'build.cpp_std=gnu++20'])

cmake = import('cmake')
cpp = meson.get_compiler('cpp')
inc_dir = []
subdir('include')

message(cpp.version())

# vulkan = dependency('vulkan', method: 'system')
glfw = dependency('glfw3', required : false)
glm = dependency('glm', required : false)

deps = [glfw, glm]

if not glfw.found()
    glfw_cmake = cmake.subproject('glfw')
    glfw = glfw_cmake.dependency('glfw')
endif

if not glm.found()
    glm_cmake = cmake.subproject('glm')
    glm = glm_cmake.dependency('glm')
endif

if build_machine.system() == 'darwin'
    moltenvk = cpp.find_library('MoltenVK', required: true)

    deps += moltenvk
else
    deps += dependency('vulkan', method: 'system')
endif

sources = []
subdir('src')
executable('Flow', sources, include_directories : inc_dir, dependencies : deps)