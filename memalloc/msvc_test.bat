@echo off
cd test
cl /c /nologo *.c /I"../include"
link /nologo *.obj ..\lib\mem.lib /out:test.exe
del *.obj
cd ..
copy lib\mem.dll test\