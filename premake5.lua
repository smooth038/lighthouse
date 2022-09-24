workspace "Lighthouse"
  architecture "x64"
  startproject "Sandbox"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "Lighthouse/vendor/GLFW/include"
includeDir["GLEW"] = "Lighthouse/vendor/GLEW/include"
includeDir["glm"] = "Lighthouse/vendor/glm"
includeDir["stb_image"] = "Lighthouse/vendor/stb_image"

libDir = {}
libDir["GLEW"] = "Lighthouse/vendor/GLEW/lib/Release/x64"

include "Lighthouse/vendor/GLFW"

project "Lighthouse"
  location "Lighthouse"
  kind "SharedLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "off"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
  debugdir "%{prj.name}"
  
  pchheader "lhpch.h"
  pchsource "Lighthouse/src/lhpch.cpp"

  defines
  {
    "GLFW_INCLUDE_NONE",
    "GLEW_STATIC",
  }

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/src/**.shader",
    "%{prj.name}/vendor/stb_image/**.h",
    "%{prj.name}/vendor/stb_image/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include",
    "%{includeDir.GLFW}",
    "%{includeDir.GLEW}",
    "%{includeDir.glm}",
    "%{includeDir.stb_image}"
  }

  libdirs
  {
    "%{libDir.GLEW}"
  }

  links
  {
    "GLFW",
    "opengl32.lib",
    "glew32s.lib"
  }

  filter "system:windows"
    cppdialect "C++20"
    staticruntime "off"
    systemversion "latest"

    defines
    {
      "LH_PLATFORM_WINDOWS",
      "LH_BUILD_DLL"
    }

    postbuildcommands
    {
      ("mkdir ..\\bin\\" .. outputdir .. "\\Sandbox"),
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
    defines "LH_DEBUG"
    symbols "on"
    runtime "Debug"

  filter "configurations:Release"
    defines "LH_RELEASE"
    optimize "on"

  filter "configurations:Dist"
    defines "LH_DIST"
    optimize "on"
    symbols "off"

  filter { "system:windows", "configurations:Release" }
    buildoptions "/MT"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"
  staticruntime "off"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
  debugdir "%{prj.name}"
  
  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
  }

  includedirs
  {
    "Lighthouse/vendor/spdlog/include",
    "Lighthouse/src",
    "%{includeDir.GLFW}",
    "%{includeDir.glm}"
  }

  links
  {
    "Lighthouse"
  }

  filter "system:windows"
    cppdialect "C++20"
    staticruntime "off"
    systemversion "latest"

    defines
    {
      "LH_PLATFORM_WINDOWS"
    }

  filter "configurations:Debug"
    defines "LH_DEBUG"
    symbols "on"
    runtime "Debug"

  filter "configurations:Release"
    defines "LH_RELEASE"
    optimize "on"

  filter "configurations:Release"
    defines "LH_DIST"
    optimize "on"
    symbols "off"

  filter { "system:windows", "configurations:Release" }
    buildoptions "/MT"

