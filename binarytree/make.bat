@echo off
cl /nologo /c /Fo:main.obj main.c
link /nologo main.obj /out:btree.exe