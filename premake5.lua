workspace "raven"
   configurations { "Debug", "Release" }
   platforms { "Win32" }
   location "build"
   disablewarnings { "4251", "4005", "4244" }
   platforms { "x86", "x64" }
   staticruntime "Off" -- /MDd

filter "configurations:Debug"
   defines { "DEBUG", "SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG" }
   filter "system:windows" buildoptions { "/utf-8" }
   symbols "On"

filter "configurations:Release"
   defines { "NDEBUG" }
   filter "system:windows" buildoptions { "/utf-8" }
   optimize "On"

-- Orbit DLL
project "orbit"
   kind "SharedLib"         -- DLL
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}/dll"
   objdir "obj/%{cfg.buildcfg}/orbit"
   targetname "ubiorbitapi_r2_loader"
   architecture "x86"

   libdirs { "dep/minhook/lib" }
   links { "libMinHook.x86" }
   includedirs { "uplay/orbit", "dep", "common/include", "dep/minhook/include" }
   files { "uplay/orbit/**.h", "uplay/orbit/**.cpp", "uplay/orbit/hooks/**.h", "uplay/orbit/hooks/**.cpp",
           "common/**.h", "common/**.cpp", "dep/minhook/include/MinHook.h" }

   filter "toolset:msc"
      buildoptions { "/EHsc" }
      disablewarnings { "4251", "4005" }

-- R1 DLL
project "r1"
   kind "SharedLib"         -- DLL
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}/dll"
   objdir "obj/%{cfg.buildcfg}/r1"
   targetname "uplay_r1_loader"
   architecture "x86"

   includedirs { "uplay/r1" }
   files { "uplay/r1/**.h", "uplay/r1/**.cpp" }

   filter "toolset:msc"
      buildoptions { "/EHsc" }
      disablewarnings { "4251", "4005" }

-- Server project
project "server"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "obj/%{cfg.buildcfg}/server"
   architecture "x64"

   includedirs {
      "server",
      "common/include",
      "dep",
      "dep/miniz",
      "dep/cereal/include"
   }

   files {
      "dep/**.h",
      "dep/**.c",
      "dep/**.hpp",
      "dep/**.cpp",
      "common/**.h",
      "common/**.cpp",
      "server/**.h",
      "server/**.hpp",
      "server/**.cpp",
   }

   filter "toolset:msc"
      buildoptions { "/EHsc" }
      disablewarnings { "4251", "4005" }
