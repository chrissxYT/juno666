include ../Makefile.include

INCLUDE_DIRS=-I..
DEFINES=-D_REENTRANT -D_GNU_SOURCE -DUSE_FASTMEMCPY
CFLAGS=$(PIC) $(PROFILE) $(OPTIMIZE) $(DEBUG) -Wall $(INCLUDE_DIRS) $(DEFINES)
CPPFLAGS=$(CFLAGS)


LIB_OBJS=ADSR.o\
	Attenuator.o\
	Balance.o\
	Chorus.o\
	ConnectionInfo.o\
	ConnectionList.o\
	ConnectionManager.o\
	DSPDeviceWin32.o\
	DSPOutput.o\
	DataBlock.o\
	Delay.o\
	GoObject.o\
	HPF.o\
	IIR2.o\
	Input.o\
	JunoPulse.o\
	JunoSaw.o\
	midiInputWin32.o\
	Mixer.o\
	MoogObject.o\
	NamedObject.o\
	Oscillator.o\
	Output.o\
	Rand.o\
	ResonantLowPass.o\
	Rms.o\
	SchedulerWin32.o\
	Settings.o\
	WaveGen.o\
	bilinear.o\
	pitch.o\
	ao_win32.o

LIBNAME=libmoog.dll
LIBSTATIC=libmoog.a
LIBMAJOR=2
LIBMINOR=0
LIBFULLNAME=$(LIBNAME).$(LIBMAJOR).$(LIBMINOR)
LIBDIR=./
LIB_LIBS=-L../util -lmoogutil -lpthread

DYN_FLAGS=-shared -Wl,-soname,"$(LIBNAME).$(LIBMAJOR)"

all: $(LIBFULLNAME)

objs: $(LIB_OBJS)

$(LIBFULLNAME): objs
	/bin/g++ $(DYN_FLAGS) -o $@ $(LIB_OBJS) $(LIB_LIBS)
	ln -f -s  $(LIBFULLNAME) $(LIBNAME)
	ln -f -s  $(LIBFULLNAME) $(LIBNAME).$(LIBMAJOR)

static: $(LIBSTATIC)

$(LIBSTATIC): $(LIB_OBJS)
	rm -f $(LIBSTATIC)
	ar r $(LIBSTATIC) $(LIB_OBJS)

install:
	cp $(LIBFULLNAME) $(LIBDIR)
	ln -f -s $(LIBDIR)/$(LIBFULLNAME) $(LIBDIR)/$(LIBNAME)

clean:
	rm -f *.o core .depend $(LIBNAME)* Makefile.bak

depend: .depend

.depend: Makefile
	touch .depend
	makedepend -f .depend $(INCLUDE_DIRS) -I/usr/include/g++-2 *.C *.c


.PHONY: objs depend

include .depend
