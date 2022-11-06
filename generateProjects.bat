copy GLFW.lua Lighthouse\vendor\GLFW\premake5.lua
copy ImGui.lua Lighthouse\vendor\imgui\premake5.lua
call vendor\bin\premake\premake5.exe vs2022
del /Q Lighthouse\vendor\GLFW\premake5.lua
del /Q Lighthouse\vendor\ImGui\premake5.lua
PAUSE
