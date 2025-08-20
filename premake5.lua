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

project "raven_loader"
   kind "SharedLib"   
   language "C++"
   cppdialect "C++20" 
   targetdir "bin/%{cfg.buildcfg}"
   objdir "obj/%{cfg.buildcfg}"

   includedirs { 
      "utils",           -- Для файла ini.h
      "raven_loader"     -- Для заголовков в папке raven_loader
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