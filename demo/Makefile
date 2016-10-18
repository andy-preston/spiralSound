#############################################################################
# Makefile for building SpiralSound demo synth
#############################################################################


####### Compiler, tools and options

CC	=	gcc
CXX	=	g++
CFLAGS	=	@CFLAGS@ @FLTK_CFLAGS@
CXXFLAGS=	@CXXFLAGS@ @FLTK_CXXFLAGS@
INCPATH	=
LINK	=	g++
LFLAGS	=
LIBS	=   @FLTK_LIBS@ -lm -ldl -lpthread
CC = gcc

####### Files

HEADERS = \
	LittleSynth \
	../SpiralSound/GraphSort.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h \
	../SpiralSound/PluginManager.h \
	../SpiralSound/ChannelHandler.h \
	../SpiralSound/Plugins/SpiralPlugin.h \

SOURCES = \
	main.cpp \
	LittleSynth.C \
	../SpiralSound/GraphSort.C \
	../SpiralSound/SpiralInfo.C \
	../SpiralSound/Sample.C \
	../SpiralSound/PluginManager.C \
	../SpiralSound/ChannelHandler.C \
	../SpiralSound/Plugins/SpiralPlugin.C \

OBJECTS = \
	main.o \
	LittleSynth.o \
	../SpiralSound/GraphSort.o \
	../SpiralSound/SpiralInfo.o \
	../SpiralSound/Sample.o \
	../SpiralSound/PluginManager.o \
	../SpiralSound/ChannelHandler.o \
	../SpiralSound/Plugins/SpiralPlugin.o \
	../SpiralSound/Plugins/SpiralPluginGUI.o

TARGET = \
	littlesynth

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

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	-rm -f $(OBJECTS) $(TARGET)
	-rm -f *~ core

####### Compile

main.o : \
	main.cpp \
	LittleSynth.h

LittleSynth.o: \
	LittleSynth.C \
	LittleSynth.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/PluginManager.h \
	../SpiralSound/Plugins/SpiralPlugin.h

GraphSort.o: \
	GraphSort.C \
	GraphSort.h

../SpiralSound/ChannelHandler.o: \
	SpiralSound/ChannelHandler.C \
	SpiralSound/ChannelHandler.h

../SpiralSound/SpiralInfo.o: \
	../SpiralSound/SpiralInfo.C \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/Sample.o: \
	../SpiralSound/Sample.C \
	../SpiralSound/Sample.h \
	../SpiralSound/SpiralInfo.h

SpiralSound/Plugins/SpiralPlugin.o: \
	../SpiralSound/Plugins/SpiralPlugin.C \
	../SpiralSound/Plugins/SpiralPlugin.h \
	../SpiralSound/Sample.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h

../SpiralSound/PluginManager.o: \
	../SpiralSound/PluginManager.C \
	../SpiralSound/PluginManager.h \
	../SpiralSound/Sample.h \
	../SpiralSound/SpiralInfo.h \
	../SpiralSound/Sample.h \
	../SpiralSound/Plugins/SpiralPlugin.h
