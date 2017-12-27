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

#include "../SpiralModule.h"

#ifndef MIDI_PLUGIN
#define MIDI_PLUGIN

class MidiModule : public SpiralModule
{
    public:
     	MidiModule(const SpiralInfo *info);
    	virtual ~MidiModule();
    	virtual void Execute();
    	int GetDeviceNum() { return m_DeviceNum; }
    	bool GetNoteCut() { return m_NoteCut; }
    	bool GetContinuousNotes() { return m_ContinuousNotes; }
    private:
    	void addControl(int s, const char *name);
    	int m_DeviceNum;
    	float m_NoteLevel;
    	float m_TriggerLevel;
    	float m_PitchBendLevel;
    	float m_ChannelPressureLevel;
    	float m_AfterTouchLevel;
    	float m_ControlLevel[128];
    	bool m_NoteCut;
    	bool m_ContinuousNotes;
    	int m_CurrentNote;
    	std::vector<int> m_ControlList;
};

#endif