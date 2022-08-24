workspace "Lighthouse"
  architecture "x64"

  configurations
  {
    "Debug",
    "Release"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Lighthouse"
  location "Lighthouse"
  kind "SharedLib"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
  
  pchheader "lhpch.h"
  pchsource "Lighthouse/src/lhpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{prj.name}/vendor/spdlog/include"
  }

  filter "system:windows"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "LH_PLATFORM_WINDOWS",
      "LH_BUILD_DLL"
    }

    postbuildcommands
    {
      ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
    }

  filter "configurations:Debug"
    defines "HZ_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "HZ_RELEASE"
    optimize "On"

  filter { "system:windows", "configurations:Release" }
    buildoptions "/MT"

project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
  
  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp"
  }

  includedirs
  {
    "Lighthouse/vendor/spdlog/include",
    "Lighthouse/src"
  }

  links
  {
    "Lighthouse"
  }

  filter "system:windows"
    cppdialect "C++20"
    staticruntime "On"
    systemversion "latest"

    defines
    {
      "LH_PLATFORM_WINDOWS"
    }

  filter "configurations:Debug"
    defines "HZ_DEBUG"
    symbols "On"

  filter "configurations:Release"
    defines "HZ_RELEASE"
    optimize "On"

  filter { "system:windows", "configurations:Release" }
    buildoptions "/MT"

