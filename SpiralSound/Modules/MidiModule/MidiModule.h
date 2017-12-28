/*
 * SpiralSound MIDI module
 *     - Copyleft (C) 2016 Andy Preston <edgeeffect@gmail.com>
 * based on SpiralSynthModular
 *     - Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef MIDI_MODULE
#define MIDI_MODULE

#include "../SpiralModule.h"
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <limits.h>
#include <queue>
#include <string>
#include <alsa/asoundlib.h>

using namespace std;

class MidiEvent
{
    public:
	    enum type {
            NONE, ON, OFF, AFTERTOUCH, PARAMETER, CHANNELPRESSURE, PITCHBEND
        };
	    MidiEvent(type t, int note, float v) {
            m_Type = t;
            m_Note = note;
            m_Volume = v;
        }
	    float m_Volume;
	    type m_Type;
	    int m_Note;
};

class MidiModule : public SpiralModule
{
    public:
     	MidiModule(const SpiralInfo *info, const char *name);
    	virtual ~MidiModule();
    	virtual void Execute();
        MidiEvent GetEvent();
        void addControl(int s, const char *name);
        const char *m_AppName;
        int m_midiChannel;
        bool m_NoteCut;
        bool m_ContinuousNotes;
    private:
        queue<MidiEvent> m_EventVec[16];
        void AlsaCollectEvents();
        snd_seq_t *handle;
    	float m_NoteLevel;
    	float m_TriggerLevel;
    	float m_PitchBendLevel;
    	float m_ChannelPressureLevel;
    	float m_AfterTouchLevel;
    	float m_ControlLevel[128];
    	int m_CurrentNote;
    	std::vector<int> m_ControlList;
};

#endif