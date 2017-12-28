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
#include "unistd.h"
#include "sys/types.h"
#include "signal.h"

using namespace std;

static const int MIDI_SCANBUFSIZE=256;
static const int MIDI_KEYOFFSET=0;

static const unsigned char STATUS_START            = 0x80;
static const unsigned char STATUS_NOTE_OFF         = 0x80;
static const unsigned char STATUS_NOTE_ON          = 0x90;
static const unsigned char STATUS_AFTERTOUCH       = 0xa0;
static const unsigned char STATUS_CONTROL_CHANGE   = 0xb0;
static const unsigned char STATUS_PROG_CHANGE      = 0xc0;
static const unsigned char STATUS_CHANNEL_PRESSURE = 0xd0;
static const unsigned char STATUS_PITCH_WHEEL      = 0xe0;
static const unsigned char STATUS_END              = 0xf0;
static const unsigned char SYSEX_START             = 0xf0;
static const unsigned char SYSEX_TERMINATOR        = 0xf7;
static const unsigned char MIDI_CLOCK              = 0xf8;
static const unsigned char ACTIVE_SENSE            = 0xfe;

static int NKEYS = 30;

MidiModule::MidiModule(const SpiralInfo *info, const char *name) :
SpiralModule(info)
{
    int clientId, portId;

    m_midiChannel = 0;
    m_NoteLevel = 0;
    m_TriggerLevel = 0;
    m_PitchBendLevel = 0;
    m_ChannelPressureLevel = 0;
    m_AfterTouchLevel = 0;
    m_NoteCut = false;
    m_ContinuousNotes = false;
    m_CurrentNote = 0;
    m_AppName = name;

    //open input handle
    if (snd_seq_open(&handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0) {
    	cerr << "Error opening ALSA input sequencer." << endl;
    	exit(1);
    }

    snd_seq_set_client_name(handle, m_AppName);
    clientId = snd_seq_client_id(handle);
    portId = snd_seq_create_simple_port(
        handle,
        m_AppName,
        SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION
    );

    if (portId < 0) {
    	cerr << "Error creating input sequencer port." << endl;
        exit(1);
    }

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
	m_AudioCH->Register("DeviceNum",&m_midiChannel);
	m_AudioCH->Register("NoteCut",&m_NoteCut);
	m_AudioCH->Register("CC",&m_GUIArgs.s);
	m_AudioCH->RegisterData("Name",ChannelHandler::INPUT,
		&m_GUIArgs.Name,sizeof(m_GUIArgs.Name));
    */
}

MidiModule::~MidiModule()
{
    snd_seq_close (handle);
}

void MidiModule::Execute()
{
    bool triggered;
    unsigned int c;

	// Done to clear IsEmpty field...
	GetOutputBuf(0)->Zero();
	GetOutputBuf(1)->Zero();
	GetOutputBuf(2)->Zero();
	GetOutputBuf(3)->Zero();
	GetOutputBuf(4)->Zero();
	GetOutputBuf(5)->Zero();
	for (c = 0; c < m_ControlList.size(); c++) {
		GetOutputBuf(c+5)->Zero();
	}

	triggered = false;
	MidiEvent Event = /* *new MidiEvent(MidiEvent::ON, 40, 0); // */ GetEvent();

	if (Event.m_Type != MidiEvent::NONE) {
        switch (Event.m_Type) {
            case MidiEvent::ON:
                triggered = true;
                m_CurrentNote = Event.m_Note;
                m_NoteLevel = NoteTable[m_CurrentNote];
                m_TriggerLevel = Event.m_Volume / 127.0f;
                break;
            case MidiEvent::OFF:
                if (Event.m_Note == m_CurrentNote) {
                    m_TriggerLevel = 0;
                    if (m_NoteCut) {
                        m_NoteLevel = 0;
                    }
                }
                break;
            case MidiEvent::PITCHBEND:
                m_PitchBendLevel = Event.m_Volume / 127.0f * 2.0f - 1.0f;
                break;
            case MidiEvent::CHANNELPRESSURE:
                m_ChannelPressureLevel = Event.m_Volume / 127.0f;
                break;
            case MidiEvent::AFTERTOUCH:
                m_AfterTouchLevel = Event.m_Volume / 127.0f;
                break;
            case MidiEvent::PARAMETER:
                // just to make sure
                if (Event.m_Note >= 0 && Event.m_Note < 128) {
                    m_ControlLevel[Event.m_Note] = Event.m_Volume / 127.0f;
			    }
                break;
		}
		// Event = GetEvent();
	}

	for (int n = 0; n < spiralInfo->bufferSize; n++) {
		SetOutputPitch(0, n, m_NoteLevel);
		SetOutput(1, n, m_TriggerLevel);
		SetOutput(2, n, m_PitchBendLevel);
		SetOutput(3, n, m_ChannelPressureLevel);
		SetOutput(4, n, m_AfterTouchLevel);
        // TODO: this should be GetClock() but I've broken it
		SetOutput(5, n, 0);
	}

	for (unsigned int c = 0; c < m_ControlList.size(); c++) {
		GetOutputBuf(c + 5)->Set(m_ControlLevel[m_ControlList[c]]);
	}

	// make sure the trigger is registered if it's
	// note is pressed before releasing the previous one.
	if (triggered && !m_ContinuousNotes) {
        SetOutput(1, 0, 0);
    }
}

void MidiModule::addControl(int s, const char *name)
{
	m_ControlList.push_back(s);
	addOutput(name, Sample::AUDIO);
}

// returns the next event off the list, or an
// empty event if the list is exhausted
MidiEvent MidiModule::GetEvent()
{
	if (m_midiChannel < 0 || m_midiChannel > 15) {
		cerr << "GetEvent: Invalid Midi channel " << m_midiChannel << endl;
        exit(1);
	}

    // Fill up m_EventVec with ALL pending messages
    AlsaCollectEvents();

	if (m_EventVec[m_midiChannel].size() == 0) {
		return MidiEvent(MidiEvent::NONE, 0, 0);
	}

	MidiEvent event(m_EventVec[m_midiChannel].front());
	m_EventVec[m_midiChannel].pop();

	return event;
}

// code taken and modified from jack_miniFMsynth

void MidiModule::AlsaCollectEvents () {
    int seq_nfds, l1;
    struct pollfd *pfds;

    // get descriptors count to find out how many events are
    // waiting to be processed
    seq_nfds = snd_seq_poll_descriptors_count(handle, POLLIN);
    cerr << seq_nfds << endl;

    // poll the descriptors to be proccessed and loop through them
    pfds = new struct pollfd[seq_nfds];
    snd_seq_poll_descriptors(handle, pfds, seq_nfds, POLLIN);
    if (poll (pfds, seq_nfds, 1000) > 0) {
        for (l1 = 0; l1 < seq_nfds; l1++) {
            if (pfds[l1].revents > 0) {
                snd_seq_event_t *ev;
                MidiEvent::type MessageType=MidiEvent::NONE;
                int Volume=0, Note=0, EventDevice=0;
                // do {
                    snd_seq_event_input (handle, &ev);
                    if (
                        (ev->type == SND_SEQ_EVENT_NOTEON) &&
                        (ev->data.note.velocity == 0)
                    ) {
                        ev->type = SND_SEQ_EVENT_NOTEOFF;
                    }
                    switch (ev->type) {
                        case SND_SEQ_EVENT_PITCHBEND:
                            MessageType=MidiEvent::PITCHBEND;
                            Volume = (char)(
                                (ev->data.control.value / 8192.0) * 256
                            );
                            break;
                        case SND_SEQ_EVENT_CONTROLLER:
                            MessageType=MidiEvent::PARAMETER;
                            Note = ev->data.control.param;
                            Volume = ev->data.control.value;
                            break;
                        case SND_SEQ_EVENT_NOTEON:
                            MessageType=MidiEvent::ON;
                            EventDevice = ev->data.control.channel;
                            Note = ev->data.note.note;
                            Volume = ev->data.note.velocity;
                            break;
                        case SND_SEQ_EVENT_NOTEOFF:
                            MessageType=MidiEvent::ON;
                            EventDevice = ev->data.control.channel;
                            Note = ev->data.note.note;
                            break;
                    }
                    m_EventVec[EventDevice].push (
                        MidiEvent (MessageType, Note, Volume)
                    );
                    snd_seq_free_event (ev);
                // } while (snd_seq_event_input_pending(handle, 0) > 0);
            }
        }
    }
    delete [] pfds;
}