


-------------------------------------------------
------ DO NOT TOUCH ENIRE PROJECT BUILD IS ------
---- HELD TOGERTHER WITH DUCTAPE AND A DREAM ----
---- I GENUINLY HAVE NO CLUE WHY THIS BUILDS ----
-------------------------------------------------

workspace "VoxelRaytracer"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "VoxelRaytracer/vendor/GLFW/include"
IncludeDir["GLEW"] = "VoxelRaytracer/vendor/GLEW/include"
IncludeDir["glm"] = "VoxelRaytracer/vendor/glm"
IncludeDir["ImGui"] = "VoxelRaytracer/vendor/imgui"

project "VoxelRaytracer"
    location "VoxelRaytracer"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        --"%{prj.name}/vendor/glm/glm/**.h",
        --"%{prj.name}/vendor/glm/glm/**.hpp",
        --"%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/imgui/**.h",
        "%{prj.name}/vendor/imgui/**.cpp",
    }
    
    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLEW_STATIC",
    }

    includedirs
    {
        "$(SolutionDir)%{IncludeDir.GLFW}",
        "$(SolutionDir)%{IncludeDir.GLEW}",
        "$(SolutionDir)%{IncludeDir.glm}",
        "$(SolutionDir)%{IncludeDir.ImGui}",
    }

    links
    { 
        "$(SolutionDir)%{prj.name}/vendor/GLFW/lib-vc2022/glfw3_mt",
        "$(SolutionDir)%{prj.name}/vendor/GLEW/lib/Release/x64/glew32s",
        "opengl32"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
        buildoptions "/MTd"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        buildoptions "/MT"