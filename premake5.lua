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
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"
IncludeDir["glm"] = "Hazel/vendor/glm"
IncludeDir["stb"] = "Hazel/vendor/stb/include"

---相当于把GLFW目录下写的premake5.lua给include进来
include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"
    location "Hazel"
    --kind "SharedLib" --生成类型
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("%{wks.location}/bin/"..dirname.."/%{prj.name}")
    objdir ("%{wks.location}/bin-int/"..dirname.."/%{prj.name}")

    files
    {
        "%{wks.location}/%{prj.name}/src/**.h",
        "%{wks.location}/%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
    }

    includedirs
    {
        "%{wks.location}/%{prj.name}/src",
        "%{wks.location}/%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}"
    }

    links
    {
        "ImGui",
        "Glad",
        "GLFW",
        "opengl32.lib",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            --"HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

        -- postbuildcommands
		-- {
		-- 	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. dirname .. "/SandBox")
		-- }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "On"
  
    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "On"

project "SandBox"
    location "SandBox"
    kind "ConsoleApp" --生成类型
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "%{wks.location}/Hazel/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}"
    }

    libdirs 
    {
        "%{wks.location}/bin/"..dirname.."/Hazel"
    }
    links
    {
        "Hazel",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "On"
  
    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        -- buildoptions "/MD"
        optimize "On"