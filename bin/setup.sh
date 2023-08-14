# Install dependencies
mkdir subprojects
meson wrap install sdl2
meson wrap install vulkan
meson wrap install glm
meson wrap install entt

# Setup build directory for the project
meson setup build