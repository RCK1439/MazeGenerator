workspace "MazeGenerator"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "MazeGenerator"
    location "MazeGenerator"
    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir)
    objdir ("%{wks.location}/bin-int/" .. outputdir)

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.location}/src/",
        "%{prj.location}/vendor/raylib/include/"
    }

    links
    {
        "raylib.lib",
        "winmm.lib",
        "gdi32.lib",
        "user32.lib",
        "shell32.lib"
    }

    filter "configurations:Debug"
        kind "ConsoleApp"

        defines
        {
            "MG_DEBUG",
            "_CRT_SECURE_NO_WARNINGS"
        }
        symbols "On"

        libdirs
        {
            "%{prj.location}/vendor/raylib/lib/Debug/"
        }

    filter "configurations:Release"
        kind "WindowedApp"

        defines
        {
            "MG_RELEASE",
            "_CRT_SECURE_NO_WARNINGS"
        }
        optimize "Full"

        libdirs
        {
            "%{prj.location}/vendor/raylib/lib/Release/"
        }

