outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "task_3"
    startproject "task_3"

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


project "task_3"
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

    filter "not system:windows"
        links {
            "pthread"
        }
