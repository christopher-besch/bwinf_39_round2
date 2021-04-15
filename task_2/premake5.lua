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
    
    filter "system:linux"
        defines { "LINUX" }
    filter "system:windows"
        defines { "WINDOWS" }


project "task_2"
    language "C++"
    architecture "x86_64"
    cppdialect "c++latest"
    kind "ConsoleApp"

    location "."
    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    files {
        "src/**.h", "src/**.cpp"
    }
