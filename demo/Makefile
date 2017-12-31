####### Compiler, tools and options

CC                 = gcc
CXX                = g++
CFLAGS_FOR_BUILD   = -O2
CXXFLAGS_FOR_BUILD = -I/usr/include/c++/4.8/
CFLAGS             =
CXXFLAGS           =
LINK               = $(CXX)
LFLAGS             =
LIBS               = -lao -lm -lasound -lpthread

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
	../SpiralSound/Modules/EnvelopeModule/EnvelopeModule.o \
	../SpiralSound/Modules/LFOModule/LFOModule.o \
	../SpiralSound/Modules/OutputModule/OutputModule.o

TARGET = \
	demosynth

####### Implicit rules

.SUFFIXES: .C .c

.C.o:
	$(CXX) -c $(CXXFLAGS) $(CXXFLAGS_FOR_BUILD) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(CFLAGS_FOR_BUILD) -o $@ $<

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
include ../SpiralSound/Modules/EnvelopeModule/makerules
include ../SpiralSound/Modules/FilterModule/makerules
include ../SpiralSound/Modules/LFOModule/makerules
include ../SpiralSound/Modules/OutputModule/makerules
