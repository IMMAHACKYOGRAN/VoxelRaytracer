project "Axel-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    files 
    {
        "src/**.h",
        "src/**.c",
        "src/**.hpp",
        "src/**.cpp",
    }

    includedirs
    {
        "../Axel-Core/src",
        "../Axel-Core/vendor",
        "../Axel-Core/vendor/spdlog/include",
        "../Axel-Core/vendor/GLFW/include",
        "../Axel-Core/vendor/imgui",
        "../Axel-Core/vendor/glm",
        "../Axel-Core/vendor/yaml-cpp/include",
    }

    links
    {
        "Axel-Core",
    }

    targetdir ("../bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../bin-int/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }

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
