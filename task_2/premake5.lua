outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "task_2"
    startproject "task_2"

    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"


project "task_2"
    language "C++"
    architecture "x86_64"
    cppdialect "C++17"
    kind "ConsoleApp"

    location "."
    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    files {
        "src/**.h", "src/**.cpp"
    }