# CodeRed-Extensions-ImGui

An ImGui backend implement using CodeRed.

## How to use

You can build this project to a library or copy the `ImGuiContext` file to your code folder and build them with your code. 

Before you build the project, you need clone the [ImGui](https://github.com/ocornut/imgui) to [ThirdPaties](https://github.com/LinkClinton/Code-Red/tree/master/ThirdParties) folder or using sub modules.

- `ImGuiContext::setRenderState()` : set the render state before we draw the ui.

- `ImGuiContext::draw()` : draw all ui objects that from `ImGui::GetDrawData()`;
