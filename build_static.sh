MAKEFLAGS="USE_STATIC=y USE_OPTIMIZE=max USE_DEBUG=n"

[ -d objs/ ] || mkdir objs/
[ -L util ] || ln -s libmoogutil util
[ -L moog ] || ln -s libmoog moog
[ -L gmoog ] || ln -s libgmoog gmoog

cd objs/ || exit 1
rm -f *
cd ..
make clean
cd objs
export OPTIMIZE='-mno-cygwin -march=i386 -O6 -ffast-math -malign-double  -pipe -fomit-frame-pointer -mfpmath=387'
MAKEFLAGS="USE_STATIC=y USE_DEBUG=n"
(cd ../libmoogutil; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libgmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../juno6; make $MAKEFLAGS $1 objs; cp *.o ../objs)

#
# hack: in juno6, there may be .o files we don't want:
rm -f copy_patch.o list_patches.o
#mv juno6.o juno6.startup
cp ../.libs/*.dll .
/bin/gcc -mno-cygwin  -shared -o libgui.dll juno_background_mask.o juno_background.o juno_bender_pixmaps.o
strip libgui.dll
cp libgui.dll ../.libs
rm juno_background*.*
rm juno_bender_pix*.*

/bin/g++ -mno-cygwin  -shared -o juno6i386.dll -L. -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 -lgui *.o  /lib/w32api/libwinmm.a
strip juno6i386.dll
cp juno6i386.dll ../bin

cd ..
make clean
cd objs

export OPTIMIZE='-mno-cygwin -DUSE_SSE -DHAVE_3DNOW -DHAVE_MMX -DHAVE_MMX2 -DHAVE_SSE -march=athlon-xp -O6 -ffast-math -malign-double  -pipe -fomit-frame-pointer -m3dnow -msse -mmmx -mfpmath=sse,387'
MAKEFLAGS="USE_STATIC=y USE_DEBUG=n"
(cd ../libmoogutil; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libgmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../juno6; make $MAKEFLAGS $1 objs; cp *.o ../objs)

#
# hack: in juno6, there may be .o files we don't want:
rm -f copy_patch.o list_patches.o
#mv juno6.o juno6.startup
cp ../.libs/*.dll .
rm juno_background*.*
rm juno_bender_pix*.*

/bin/g++ -mno-cygwin  -shared -o juno6athlon-xp.dll -L. -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 -lgui *.o /lib/w32api/libwinmm.a
strip juno6athlon-xp.dll
cp juno6athlon-xp.dll ../bin


cd ..
make clean
cd objs
export OPTIMIZE='-mno-cygwin -DUSE_SSE -DHAVE_SSE2 -DHAVE_MMX -DHAVE_MMX2 -DHAVE_SSE -march=pentium4 -O6 -ffast-math -malign-double  -pipe -fomit-frame-pointer -msse2 -msse -mmmx -mfpmath=sse,387'
MAKEFLAGS="USE_STATIC=y USE_DEBUG=n"
(cd ../libmoogutil; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libgmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../juno6; make $MAKEFLAGS $1 objs; cp *.o ../objs)

#
# hack: in juno6, there may be .o files we don't want:
rm -f copy_patch.o list_patches.o
#mv juno6.o juno6.startup
cp ../.libs/*.dll .
rm juno_background*.*
rm juno_bender_pix*.*

/bin/g++ -mno-cygwin  -shared -o juno6p4.dll -L. -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 -lgui *.o /lib/w32api/libwinmm.a
strip juno6p4.dll
cp juno6p4.dll ../bin


cd ..
make clean
cd objs
export OPTIMIZE='-mno-cygwin -DUSE_SSE -march=pentium4 -O6 -ffast-math -malign-double  -pipe -fomit-frame-pointer -mpni -msse2 -msse -mmmx -mfpmath=sse,387'
MAKEFLAGS="USE_STATIC=y USE_DEBUG=n"
(cd ../libmoogutil; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libgmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../juno6; make $MAKEFLAGS $1 objs; cp *.o ../objs)

#
# hack: in juno6, there may be .o files we don't want:
rm -f copy_patch.o list_patches.o
#mv juno6.o juno6.startup
cp ../.libs/*.dll .
rm juno_background*.*
rm juno_bender_pix*.*

/bin/g++ -mno-cygwin  -shared -o juno6p4pni.dll -L. -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 -lgui *.o /lib/w32api/libwinmm.a
strip juno6p4pni.dll
cp juno6p4pni.dll ../bin


cd ..
make clean
cd objs
export OPTIMIZE='-mno-cygwin -DUSE_SSE -mtune=k8 -march=k8 -mcpu=k8 -O6 -ffast-math -malign-double -mno-push-args  -pipe -fomit-frame-pointer -m3dnow -msse2 -msse -mmmx -mfpmath=sse,387'
MAKEFLAGS="USE_STATIC=y USE_DEBUG=n"
(cd ../libmoogutil; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../libgmoog; make $MAKEFLAGS $1 objs; cp *.o ../objs)
(cd ../juno6; make $MAKEFLAGS $1 objs; cp *.o ../objs)

#
# hack: in juno6, there may be .o files we don't want:
rm -f copy_patch.o list_patches.o
#mv juno6.o juno6.startup
cp ../.libs/*.dll .
rm juno_background*.*
rm juno_bender_pix*.*

/bin/g++ -mno-cygwin  -shared -o juno6a64.dll -L. -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 -lgui *.o /lib/w32api/libwinmm.a
strip juno6a64.dll
cp juno6a64.dll ../bin

cd ../juno6
/bin/gcc -mno-cygwin  -o copypatch -I../ copy_patch.c juno_patch.c ../util/debug.c ../util/sio.c
strip copypatch.exe
/bin/gcc -mno-cygwin  -o listpatches -I../ list_patches.c juno_patch.c ../util/debug.c ../util/sio.c
strip listpatches.exe
cp *.exe ../bin
cd ..
/bin/gcc -mno-cygwin  -I./ -o bin/startup ./startup/startup.cpp ./util/cpudetect.c osdep/timer-win32.c -lwinmm
echo
echo Binary in objs/ directory.
echo

