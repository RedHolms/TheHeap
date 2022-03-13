@echo off
cl /c /nologo src\*.c /D_X86_
if not exist lib mkdir lib
cd lib
link /nologo ..\*.obj /DLL ntdll.lib
cd ..
del *.obj
if not exist include mkdir include
copy src\*.h include\