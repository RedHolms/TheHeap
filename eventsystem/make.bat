@echo off
cl /c /nologo /EHsc Window.cpp Dispatcher.cpp main.cpp
link /nologo Window.obj Dispatcher.obj main.obj User32.lib /out:es.exe