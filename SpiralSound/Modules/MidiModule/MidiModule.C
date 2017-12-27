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

#include "MidiModule.h"
#include "../../NoteTable.h"
#include "../../Midi.h"

using namespace std;

MidiModule::MidiModule(const SpiralInfo *info) : SpiralModule(info)
{
    m_DeviceNum = 0;
    m_NoteLevel = 0;
    m_TriggerLevel = 0;
    m_PitchBendLevel = 0;
    m_ChannelPressureLevel = 0;
    m_AfterTouchLevel = 0;
    m_NoteCut = false;
    m_ContinuousNotes = false;
    m_CurrentNote = 0;

	MidiDevice::Init("SpiralModular", MidiDevice::READ);
    // TODO: set MIDI device
    // It'd be nice if we could use a library instead of SSMs MIDI code
    //MidiDevice::SetDeviceName(info->MidiDevice);

	addOutput("Note", Sample::AUDIO);
	addOutput("Trigger", Sample::AUDIO);
	addOutput("PitchBend", Sample::AUDIO);
	addOutput("ChannelPressure", Sample::AUDIO);
	addOutput("Aftertouch", Sample::AUDIO);
	addOutput("Clock", Sample::AUDIO);

	for (int n = 0; n < 128; n++) {
        m_ControlLevel[n] = 0;
    }

    /*
	m_AudioCH->Register("DeviceNum",&m_DeviceNum);
	m_AudioCH->Register("NoteCut",&m_NoteCut);
	m_AudioCH->Register("CC",&m_GUIArgs.s);
	m_AudioCH->RegisterData("Name",ChannelHandler::INPUT,
		&m_GUIArgs.Name,sizeof(m_GUIArgs.Name));
    */
}

MidiModule::~MidiModule()
{
	MidiDevice::PackUpAndGoHome();
}

void MidiModule::Execute()
{

	// Done to clear IsEmpty field...
	GetOutputBuf(0)->Zero();
	GetOutputBuf(1)->Zero();
	GetOutputBuf(2)->Zero();
	GetOutputBuf(3)->Zero();
	GetOutputBuf(4)->Zero();
	GetOutputBuf(5)->Zero();

	for (unsigned int c=0; c<m_ControlList.size(); c++)
	{
		GetOutputBuf(c+5)->Zero();
	}

	bool Triggered=false;

	// midi output
	if (InputExists(0) && InputExists(1))
	{
		static bool TriggeredOut=false;
		if (GetInput(1,0)>0)
		{
			if (!TriggeredOut) // note on
			{
				// get the midi note
				float Freq=GetInputPitch(0,0);
				int Note=0;
				for (int n=0; n<132; n++)
				{
					if (feq(Freq,NoteTable[n],0.01f))
					{
						Note=n;
						break;
					}
				}

				MidiEvent NewEvent(MidiEvent::ON,Note,GetInput(1,0)*128.0f);
				MidiDevice::Get()->SendEvent(m_DeviceNum,NewEvent);
				TriggeredOut=true;
			}
		}
		else
		{
			if (TriggeredOut) // note off
			{
				// get the midi note
				float Freq=GetInputPitch(0,0);
				int Note=0;
				for (int n=0; n<132; n++)
				{
					if (feq(Freq,NoteTable[n],0.01f))
					{
						Note=n;
						break;
					}
				}

				MidiEvent NewEvent(MidiEvent::OFF,Note,0.0f);
				MidiDevice::Get()->SendEvent(m_DeviceNum,NewEvent);
				TriggeredOut=false;
			}
		}
	}

	MidiEvent Event=MidiDevice::Get()->GetEvent(m_DeviceNum);
	// get all the midi events since the last check
	while(Event.GetType()!=MidiEvent::NONE)
	{
		if (Event.GetType()==MidiEvent::ON)
		{
			Triggered=true;
			m_CurrentNote=Event.GetNote();
			m_NoteLevel=NoteTable[m_CurrentNote];
			m_TriggerLevel=Event.GetVolume()/127.0f;
		}

		if (Event.GetType()==MidiEvent::OFF)
		{
			if (Event.GetNote()==m_CurrentNote)
			{
				m_TriggerLevel=0;
				if (m_NoteCut) m_NoteLevel=0;
			}
		}

		if (Event.GetType()==MidiEvent::PITCHBEND)
		{
			m_PitchBendLevel=Event.GetVolume()/127.0f*2.0f-1.0f;
		}

		if (Event.GetType()==MidiEvent::CHANNELPRESSURE)
		{
			m_ChannelPressureLevel=Event.GetVolume()/127.0f;
		}

		if (Event.GetType()==MidiEvent::AFTERTOUCH)
		{
			m_AfterTouchLevel=Event.GetVolume()/127.0f;
		}

		if (Event.GetType()==MidiEvent::PARAMETER)
		{
			// just to make sure
			if (Event.GetNote()>=0 && Event.GetNote()<128)
			{
				m_ControlLevel[Event.GetNote()]=Event.GetVolume()/127.0f;
			}
		}

		Event=MidiDevice::Get()->GetEvent(m_DeviceNum);
	}

	for (int n = 0; n < spiralInfo->bufferSize; n++) {
		SetOutputPitch(0, n, m_NoteLevel);
		SetOutput(1, n, m_TriggerLevel);
		SetOutput(2, n, m_PitchBendLevel);
		SetOutput(3, n, m_ChannelPressureLevel);
		SetOutput(4, n, m_AfterTouchLevel);
		SetOutput(5, n, MidiDevice::Get()->GetClock());
	}

	for (unsigned int c=0; c<m_ControlList.size(); c++)
	{
		GetOutputBuf(c+5)->Set(m_ControlLevel[m_ControlList[c]]);
	}

	// make sure the trigger is registered if it's
	// note is pressed before releasing the previous one.
	if (Triggered && !m_ContinuousNotes) SetOutput(1,0,0);
}

void MidiModule::addControl(int s, const char *name)
{
	m_ControlList.push_back(s);
	addOutput(name, Sample::AUDIO);
}