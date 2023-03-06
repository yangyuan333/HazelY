-- -- premake5.lua

workspace "HazelY"
    architecture "x64"
    configurations
    {
        "Debug", 
        "Release", 
        "Dist"
    }

dirname = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"

---相当于把GLFW目录下写的premake5.lua给include进来
include "Hazel/vendor/GLFW"


project "Hazel"
    location "Hazel"
    kind "SharedLib" --生成类型
    language "C++"
    
    targetdir ("%{wks.location}/bin/"..dirname.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..dirname.."/%{prj.name}")

    files
    {
        "%{wks.location}/%{prj.name}/src/**.h",
        "%{wks.location}/%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/%{prj.name}/src",
        "%{wks.location}/%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        staticruntime "On"

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL"
        }

        postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. dirname .. "/SandBox")
		}

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        buildoptions "/MDd"
        symbols "On"
  
    filter "configurations:Release"
        defines "HZ_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        buildoptions "/MD"
        optimize "On"

project "SandBox"
    location "SandBox"
    kind "ConsoleApp" --生成类型
    language "C++"

    targetdir ("%{wks.location}/bin/"..dirname.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..dirname.."/%{prj.name}")
    
    files
    {
        "%{wks.location}/%{prj.name}/src/**.h",
        "%{wks.location}/%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/Hazel/vendor/spdlog/include",
        "%{wks.location}/Hazel/src"
    }

    libdirs 
    {
        "%{wks.location}/bin/"..dirname.."/Hazel"
    }
    links
    {
        "Hazel"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        staticruntime "On"

        defines
        {
            "HZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        buildoptions "/MDd"
        symbols "On"
  
    filter "configurations:Release"
        defines "HZ_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        buildoptions "/MD"
        optimize "On"