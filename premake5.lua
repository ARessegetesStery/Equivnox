-- Spark Engine config

workspace "Equivnox"
    architecture "x64"
    configurations
    {
        "Debug",   -- dev mode
        "Release"  -- release mode
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Equivnox"
    location "Equivnox"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp"
    }

    includedirs
    {
        "%{prj.name}",
        "%{prj.name}/src",
        "%{prj.name}/vendor",
    }

    filter "system:Windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        defines "EQX_DEBUG"
        symbols "On"

        defines
        {
            "EQX_DEBUG_ON"
        }

    filter "configurations:Release"
        defines "EQX_RELEASE"
        optimize "On"