project "Axel-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "vendor/glm/**.hpp",
        "vendor/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE",
    }

    includedirs
    {
        "src",
        "vendor/spdlog/include",
        "vendor/glm",
        "vendor/GLFW/include",
        "vendor/Glad/include",
        "vendor/imgui",
    }

    links
    {
        "GLFW",
        "Glad",
        "imgui",
        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"