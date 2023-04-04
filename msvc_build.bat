
#C:\VisualStudio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat x64

cl /I include /I src /I vendor\include src/build.cpp /link gdi32.lib opengl32.lib user32.lib vendor\lib\libopenal.lib
