include ../Makefile.include

INCLUDE_DIRS=-I..
CFLAGS=$(PIC) $(PROFILE) $(OPTIMIZE) $(DEBUG)  -Wall $(INCLUDE_DIRS) $(shell gtk-config --cflags)
CPPFLAGS=$(CFLAGS)
LIB_OBJS=	juno_slider.o\
		juno_knob.o\
		juno_led.o\
		juno_button.o\
		juno_bender.o\
		juno_switch.o\
		Scope.o\
		scope2.o\
		juno_keyboard.o\
		alpha_led.o\
		juno_powerswitch.o\
		juno_lfotrig.o\
		juno_bender_pixmaps.o
			

LIBNAME=libgmoog.dll
LIBSTATIC=libgmoog.a
LIBMAJOR=1
LIBMINOR=0
LIBFULLNAME=$(LIBNAME).$(LIBMAJOR).$(LIBMINOR)
LIBDIR=./
LIB_LIBS=-L../moog -lmoog -L../util -lmoogutil $(shell gtk-config --libs)
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

clean:
	rm -f *.o core .depend $(LIBNAME)* Makefile.bak

.depend: Makefile
	touch .depend
	makedepend -f .depend $(INCLUDE_DIRS) -I/usr/include/g++-2 $(shell gtk-config --cflags) *.C *.c

.PHONY: objs

include .depend
