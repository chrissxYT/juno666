@echo off
set PATH=c:\mingw\bin;%PATH%
set MAKEFLAGS="USE_STATIC=y USE_OPTIMIZE=max USE_DEBUG=n"
mkdir objs
del /q /s .depend*.*
cd objs
del  /q *.* 


cd ..\libmoogutil 
mingw32-make -f makefile.mingw %MAKEFLAGS% objs
copy *.o ..\objs
cd ..\libmoog
mingw32-make -f makefile.mingw %MAKEFLAGS% objs
copy *.o ..\objs
cd ..\libgmoog
mingw32-make -f makefile.mingw %MAKEFLAGS% objs
copy *.o ..\objs
cd ..\juno6 
mingw32-make -f makefile.mingw %MAKEFLAGS% objs
copy *.o ..\objs
cd ..\objs

del /q copy_patch.o list_patches.o 

g++ -mms-bitfields -o juno6athlon-xp.exe -L. -L../lib -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 *.o ..\..\lib\libwinmm.a
strip juno6athlon-xp.exe
mkdir ..\bin
copy juno6athlon-xp.exe ..\bin
cd ..

