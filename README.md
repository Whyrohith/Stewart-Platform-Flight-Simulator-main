# Stewart Platform Flight Simulator
 Stewart Platform - Flight Simulator | Source code.

![Image](https://github.com/MrsRina/Stewart-Platform-Flight-Simulator/blob/main/splash/showcase-2.png?raw=true)

---

# Running

Linux x86_x64: Install SDL2 & GLEW using `pacman | apt` go to `/build-linux/spf_simulator64` and run it.

Windows 11/10 x86_x64: `/build/spf_simulator64.exe` run it.

# Info & Details

The SPF Simulator is a simple program written in C++ using OpenGL 4, SDL2, [Amogpu](https://github.com/MrsRina/amogpu) & Freetype. The project is not 100% accurate - stewart platform - the project let 22 days to finish.

The written GUI was built with UI internal API, the UI is rendered using dynamic allocations provided by `dynamic_batching`, the 3D context is draw using normal method invocations (instanced rendering and per buffer GPU).

# Environment

Want to change the scale of superior plane? go into impl/editor/impl_editor_module and look @create_models method, change the `this->superior->set_scale(x, y, z)`.

First the project start at `main.cpp`, running the core and context from `impl`, everything into the `impl` is implementation of API core, the client initialize the API and handle the main loop of the application.

~~~
-- Feature:
Base of all connected objects.
-- Module:
Module is a mini-program handled by the API.
-- Service:
Handler components into the application.
-- Context:
Main program application.
-- UI:
Create UI elements.
-- World:
The world environment.
-- Core:
The client core.
~~~

Module service:

~~~c++
// Module system:

/*
 * Create a class that extend feature.
 */
class ur_module : public feature {
public:
    ur_module() {
        this->tag = "name-of-module";            
    }
    
    ~ur_module(); // des-constructor
    
    void on_enable() override;
    void on_disable() override;
    void on_event(SDL_Event &sdl_event) override;
    void on_update() override;
    void on_render() override;
};

// ...
// Invoke in context @on_enable
client::core.module_service.registry(new ur_module());

// --
// Task system:

/*
 * Remember to run it only one time.
 */
client::core.module_service.registry(new module_task("name-of-task", feature_object, [](feature* main) {
    // Do here whatever you want.
    return 0;
}));

// ...
// If you want to invoke the task just do:
client::core.module_service.callback("name-of-task");
// You can run it in any running ticks place.
~~~

UI system:

~~~c++
The UI system should be create one time only.
~~~


~~~c++
// There is 7 widgets:
// Abstract: base.
ui::category (...); // Frame to add other elements.
ui::checkbox (...); // Checkbox button.
ui::sider3d  (...); // Drag x, y value.
ui::slider   (...); // Drag x value.
ui::button   (...); // Button.
ui::label    (...); // Label: Draw texts into screen:

ui::place::axis;
ui::place::next;
ui::place::fill;

ui::slider(min, max, "name-or-empty", ui::place::);
ui::button("name", ui::place::);
ui::checkbox("name", ui::place::);
ui::slider3d("name", range, ui::place::);
ui::label("text", ui::place::);

// Also you can get/set extra fields (vars) into the UI, just look into the api/ui/ui.hpp.
~~~

Place system:

![Text Image](/splash/ui-docs-1.png?raw=true)
![Text Image2](https://github.com/MrsRina/Stewart-Platform-Flight-Simulator/blob/main/splash/showcase-3-ui.png?raw=true)
