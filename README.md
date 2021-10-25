# Flow Engine

Flow is a 3D Vulkan based engine built to allow developers to maximize their workflow. Currently, Flow only contains a barebones implementation of the Vulkan API and should not be considered for game development, as it's still missing many major components (like audio, user input, physics, and more). If you and your team aren't concerned with the amount of time it will take to implement those components yourself, then go on ahead! Of course, if you do implement your own custom componenets, do consider contributing!

## Data-Oriented Design
The industry has been moving to a more data-oriented paradigm with game development in recent years, and so Flow was built with data in mind. Every aspect of Flow is built around its data, and its data around every aspect of Flow. This is to allow developers to manipulate the data the way they need to. Even the core Vulkan components are stored in arrays to give the developers more flexibility.

## TO-DO
Right now, Flow is in pre-alpha. In order for Flow to reach its alpha stage, the following tasks need to be done:

- [ ] Restructure project. Since the project is in its very early stages, this can be done. Checkout `CONTRIBUTING.md` for more info.
- [ ] Figure out how a GUI will be implemented. Right now the idea is to us Imgui, but maybe it will be better to have a custom GUI implementation.
- [ ] Possible switch window from GLFW to a custom implementation?
- [ ] Allow people to place objects in the world, and move them around.
- [ ] Implement basic lighting
- [ ] Implement simple animations
- [ ] Implement user input
- [ ] Implement audio
- [ ] Implement basic physics
- [ ] Create a logo for the engine... Important, but not necessary for alpha. (it would be cool though)

### The Future
Once alpha has been reached, this is what could be worked on:
- Implement a shader graph, so no person ever has to hard-code shaders
- Create an API to allow programmers to more easily interface with Flow.
- Add 2D capabilities
- Add more complex lighting tools.
- Create a custom entity-componenet system.
- And more!

## Contributing
Check out the [contribution guide](https://github.com/gibsorya/Flow/blob/main/CONTRIBUTING.md) on rules and instructions on how to contribute!
