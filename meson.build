project('Flow', 'cpp', default_options : ['cpp_std=c++20'])

cpp = meson.get_compiler('cpp')

incdir = include_directories('include',
  'include/platforms',
  'include/engine',
  'include/engine/vulkan',
  'include/engine/vulkan/systems',
  'include/engine/vulkan/components')

sources = files('src/platforms/macos.cpp',
  'src/platforms/windows.cpp',
  'src/engine/app.cpp',
  'src/engine/vulkan/components/instance.cpp',
  'src/engine/vulkan/components/devices.cpp',
  'src/engine/vulkan/systems/init.cpp')

deps = [
  dependency('sdl2'),
  dependency('glm'),
  dependency('vulkan'),
  dependency('entt')
]

executable('Flow',
           'src/main.cpp',
            sources,
           dependencies: deps,
           include_directories: incdir)
