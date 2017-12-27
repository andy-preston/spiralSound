####### Compiler, tools and options

CC		=	gcc
CXX		=	g++
INCPATH	=   /usr/include/c++/4.8/
CFLAGS	=
CXXFLAGS=
LINK	=	g++
LFLAGS	=
LIBS	=   -lao -lm

####### Files

HEADERS = \
	../SpiralSound/Synth.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h \
	../SpiralSound/RiffWav.h \
	../SpiralSound/Modules/SpiralModule.h \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.h \
	../SpiralSound/Modules/FilterModule/FilterModule.h \
	../SpiralSound/Modules/FilterModule/iir_filter.h \
	../SpiralSound/Modules/LFOModule/LFOModule.h \
	../SpiralSound/Modules/OutputModule/OutputModule.h

SOURCES = \
	main.cpp \
	../SpiralSound/Synth.C \
	../SpiralSound/SpiralInfo.C \
	../SpiralSound/Sample.C \
	../SpiralSound/Modules/SpiralModule.C \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.C \
	../SpiralSound/Modules/FilterModule/FilterModule.C \
	../SpiralSound/Modules/FilterModule/iir_filter.C \
	../SpiralSound/Modules/LFOModule/LFOModule.C \
	../SpiralSound/Modules/OutputModule/OutputModule.C

OBJECTS = \
	main.o \
	../SpiralSound/Synth.o \
	../SpiralSound/SpiralInfo.o \
	../SpiralSound/Sample.o \
	../SpiralSound/Modules/SpiralModule.o \
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
	main.cpp \
	../SpiralSound/Synth.h

../SpiralSound/Synth.o: \
	../SpiralSound/Synth.C \
	../SpiralSound/Synth.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Modules/SpiralModule.h

../SpiralSound/SpiralInfo.o: \
	../SpiralSound/SpiralInfo.C \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/Sample.o: \
	../SpiralSound/Sample.C \
	../SpiralSound/Sample.h \
	../SpiralSound/SpiralInfo.h

../SpiralSound/Modules/SpiralModule.o: \
	../SpiralSound/Modules/SpiralModule.C \
	../SpiralSound/Modules/SpiralModule.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/Modules/OscillatorModule/OscillatorModule.o: \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.C \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/Modules/FilterModule/FilterModule.o: \
	../SpiralSound/Modules/FilterModule/FilterModule.C \
	../SpiralSound/Modules/FilterModule/FilterModule.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/Modules/FilterModule/iir_filter.o: \
	../SpiralSound/Modules/FilterModule/iir_filter.C \
	../SpiralSound/Modules/FilterModule/iir_filter.h

../SpiralSound/Modules/LFOModule/LFOModule.o: \
	../SpiralSound/Modules/LFOModule/LFOModule.C \
	../SpiralSound/Modules/LFOModule/LFOModule.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/Modules/OutputModule/OutputModule.o: \
	../SpiralSound/Modules/OutputModule/OutputModule.C \
	../SpiralSound/Modules/OutputModule/OutputModule.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h \
	../SpiralSound/RiffWav.h