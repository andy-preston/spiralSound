####### Compiler, tools and options

CC		=	gcc
CXX		=	g++
INCPATH	=   /usr/include/c++/4.8/
CFLAGS	=
CXXFLAGS=	-funsigned-char
LINK	=	g++
LFLAGS	=
LIBS	=   -lao -lm -lasound

OBJECTS = \
	main.o \
	../SpiralSound/Synth.o \
	../SpiralSound/SpiralInfo.o \
	../SpiralSound/Sample.o \
	../SpiralSound/Modules/SpiralModule.o \
	../SpiralSound/Modules/MidiModule/MidiModule.o \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.o \
	../SpiralSound/Modules/FilterModule/FilterModule.o \
	../SpiralSound/Modules/FilterModule/iir_filter.o \
	../SpiralSound/Modules/LFOModule/LFOModule.o \
	../SpiralSound/Modules/OutputModule/OutputModule.o


TARGET = \
	demosynth

####### Implicit rules

.SUFFIXES: .C .c

.C.o:
	$(CXX) -c $(CXXFLAGS) -I $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) -I $(INCPATH) -o $@ $<

####### Build rules

demosynth: $(OBJECTS)
	$(LINK) $(LFLAGS) -o demosynth $(OBJECTS) $(LIBS)

clean:
	-rm -f $(OBJECTS) demosynth *~ core

####### Compile

main.o : \
	main.cpp

include ../SpiralSound/makerules
include ../SpiralSound/Modules/makerules
include ../SpiralSound/Modules/MidiModule/makerules
include ../SpiralSound/Modules/OscillatorModule/makerules
include ../SpiralSound/Modules/FilterModule/makerules
include ../SpiralSound/Modules/LFOModule/makerules
include ../SpiralSound/Modules/OutputModule/makerules