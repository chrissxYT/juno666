MAKEFLAGS="USE_STATIC=y USE_OPTIMIZE=max USE_DEBUG=n"

[ -d objs/ ] || mkdir objs/
[ -L util ] || ln -s libmoogutil util
[ -L moog ] || ln -s libmoog moog
[ -L gmoog ] || ln -s libgmoog gmoog

cd objs/ || exit 1
rm -f *

export OPTIMIZE='-DHAVE_SSE -mms-bitfields -mno-cygwin -DDOMAIN -msse -m3dnow -mfpmath=sse,387 -march=athlon-xp -O3 -ffast-math -malign-double -pipe -fomit-frame-pointer -mfpmath=387 -m128bit-long-double -minline-all-stringops'
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
#rm juno_background*.*
/bin/g++ -mms-bitfields -mno-cygwin  -o juno6athlon-xp.exe -L. -lgdk-0 -lgtk-0 -lglib-2.0-0 -lgthread-2.0-0 -lgui *.o /lib/w32api/libwinmm.a
#/bin/g++ -mms-bitfields -mno-cygwin -o juno6athlon-xp.exe -L. -lpthread -lgdk-0 -lgtk-0 -lgthread-2.0-0 *.o ../.libs/libglib-2.0.a ../.libs/libiconv.a ../.libs/libintl.a ../.libs/libiconv.a /lib/w32api/libwinmm.a
strip juno6athlon-xp.exe
cp juno6athlon-xp.exe ../bin



echo
echo Binary in objs/ directory.
echo

