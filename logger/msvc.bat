@echo off
copy src\*.hpp test
cl /c /nologo src/*.cpp test/*.cpp /D_X86_ /EHsc /std:c++17
del test\*.hpp
link /nologo *.obj /out:run.exe
del *.obj