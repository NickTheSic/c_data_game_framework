workspace "MyGameFramework"
   configurations { "Debug", "Release" }

project "Project"
   kind "ConsoleApp"
   language "C++"
   targetdir "VS2019bin/%{cfg.buildcfg}"

   files { "**.h", "**.cpp" }
   removefiles {"src/build.cpp", "src/nl_platform_web.cpp"}

   includedirs {"include", "vendor/include"}

   links {"opengl32", "user32", "gdi32"}

   filter "configurations:Debug"
      defines { "DEBUG", "VS2019_PROJECT" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG", "VS2019_PROJECT"}
      optimize "On"