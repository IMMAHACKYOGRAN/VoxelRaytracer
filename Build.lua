workspace "Axel"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Axel-Editor"

    filter "system:windows"
       buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
    include "Axel-Core/vendor/GLFW"
    include "Axel-Core/vendor/Glad"
    include "Axel-Core/vendor/imgui"
group ""

include "Axel-Core/Build-Core.lua"
include "Axel-Editor/Build-Editor.lua"