-- premake5.lua
workspace "raven"
   configurations { "Debug", "Release" }
   platforms { "Win32" }
   location "build"

filter "configurations:Debug"
   defines { "DEBUG" }
   symbols "On"

filter "configurations:Release"
   defines { "NDEBUG" }
   optimize "On"

-- Custom loader for the game
project "ubiorbitapi_r2_loader"
   kind "SharedLib"   
   language "C++"
   cppdialect "C++20" 
   targetdir "bin/%{cfg.buildcfg}"
   objdir "obj/%{cfg.buildcfg}"

   includedirs { 
      "utils",
      "raven_loader"
   }

   files { 
      "utils/**.h",
      "utils/**.hpp",
      "utils/**.cpp",
      "raven_loader/**.hpp", 
      "raven_loader/**.cpp" 
   }

   defines { 
      "ORBIT_EXPORT"  
   }

   filter "toolset:msc"
      buildoptions { "/EHsc" }
      disablewarnings { "4251", "4005" }

-- raven_server (EXE)
project "server"
   kind "ConsoleApp"         -- .exe
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   objdir "obj/%{cfg.buildcfg}/server"

   includedirs {
      "server"
   }

   files {
      "server/proto/**.hpp",
      "server/proto/**.cpp",
      "server/quazal/**.cpp",
      "server/quazal/**.hpp",
	  "server/services/**.hpp",
	  "server/services/**.cpp",
      "server/**.h",
      "server/**.hpp",
      "server/**.cpp",
   }

   filter "toolset:msc"
      buildoptions { "/EHsc" }
      disablewarnings { "4251", "4005" }
