#############################################################################
# Makefile for building SpiralSound demo synth
#############################################################################

####### Compiler, tools and options

CC		=	gcc
CXX		=	g++
CFLAGS	=
CXXFLAGS=
INCPATH	=   /usr/include/c++/4.8/
LINK	=	g++
LFLAGS	=
LIBS	=   -lm

####### Files

HEADERS = \
	../SpiralSound/Synth.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h \
	../SpiralSound/RiffWav.h \
	../SpiralSound/Modules/SpiralModule.h \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.h \
	../SpiralSound/Modules/OutputModule/OutputModule.h

SOURCES = \
	main.cpp \
	../SpiralSound/Synth.C \
	../SpiralSound/SpiralInfo.C \
	../SpiralSound/Sample.C \
	../SpiralSound/Modules/SpiralModule.C \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.C \
	../SpiralSound/Modules/OutputModule/OutputModule.C

OBJECTS = \
	main.o \
	../SpiralSound/Synth.o \
	../SpiralSound/SpiralInfo.o \
	../SpiralSound/Sample.o \
	../SpiralSound/Modules/SpiralModule.o \
	../SpiralSound/Modules/OscillatorModule/OscillatorModule.o \
	../SpiralSound/Modules/OutputModule/OutputModule.o

TARGET = \
	demosynth

####### Implicit rules

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $@ $<

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

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

../SpiralSound/Modules/OutputModule/OutputModule.o: \
	../SpiralSound/Modules/OutputModule/OutputModule.C \
	../SpiralSound/Modules/OutputModule/OutputModule.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h \
	../SpiralSound/RiffWav.h
