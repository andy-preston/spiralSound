/*  SpiralSound
 *  Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
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

#ifndef SPIRAL_MODULE
#define SPIRAL_MODULE

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "../SpiralInfo.h"
#include "../Sample.h"

static const float MAX_FREQ = 13000;

class InputPort
{
    public:
        InputPort(const char* Name, Sample::SampleType Type, const Sample* Data);
        const Sample* Data;
        const char* Name;
        Sample::SampleType Type;
};

class OutputPort {
    public:
        OutputPort(const char* Name, Sample::SampleType Type, Sample* Data);
        Sample* Data;
        const char* Name;
        Sample::SampleType Type;
};

struct ModuleControl
{
    char* name;
};

class SpiralModule {
    public:
	    SpiralModule(const SpiralInfo *info);

	    virtual ~SpiralModule();

        // execute the audio
	    virtual void Execute() = 0;

        // run the commands from the Control panel
        /*
        virtual void ExecuteCommands()
        {
        }
        */

        // stuff here gets saved in filename_files directory
        // you must return true if this feature is used.
        virtual bool SaveExternalFiles(const char *Dir)
        {
            return false;
        }

	    virtual void LoadExternalFiles(const char *Dir, int withID=-1)
        {
        }

        const SpiralInfo* GetInfo()
        {
            return spiralInfo;
        }

        void GetOutput(string name, Sample **s);
        void SetInput(string name, const Sample *s);

        const Sample* GetInput(unsigned int n)
        {
            return m_Input[n]->Data;
        }

        void SetUpdateCallback(void (*s)(void*,bool m))
        {
            cb_Update = s;
        }

        void SetBlockingCallback(void (*s)(void*,bool m))
        {
            cb_Blocking = s;
        }

        void SetParent(void *s)
        {
            m_Parent = s;
        }

        // is the Module connected to an external device (oss/alsa/jack)
        bool m_IsTerminal;

        virtual bool IsAudioDriver()
        {
            return false;
        }

    protected:

        void SetOutput(int n, int p, float s)
		{
            if (m_Output[n]->Data) {
                m_Output[n]->Data->Set(p, s);
            }
        }

        float GetInput(int n, int p)
        {
            if (m_Input[n]->Data) {
                return (*m_Input[n]->Data)[p];
            }
            return 0.0;
        }

        void  SetOutputPitch(int n, int p, float s)
        {
            if (m_Output[n]->Data) {
                m_Output[n]->Data->Set(p, (s / MAX_FREQ *2 ) - 1.0f);
            }
        }

        float GetInputPitch(int n, int p) {
            if (m_Input[n]->Data) {
                return ((*m_Input[n]->Data)[p] + 1.0f) * MAX_FREQ / 2;
            }
            return 0.0;
        }

        void  MixOutput(int n, int p, float s)
		{
            if (m_Output[n]) {
                m_Output[n]->Data->Set(p, s + (*m_Output[n]->Data)[p]);
            }
        }

        bool InputExists(int n)
        {
            return m_Input[n]->Data != NULL;
        }

        bool OutputExists(int n) {
            return m_Output[n]->Data != NULL;
        }

        void addOutput(const char* name, Sample::SampleType type);

        void addInput(const char* name, Sample::SampleType type);

        void addIntControl(const char* name, int* data);

        void addFloatControl(const char* name, float* data);

        Sample* GetOutputBuf(int n)
        {
            return m_Output[n]->Data;
        }

        const SpiralInfo *spiralInfo;

        // needed for jack
        void (*cb_Update)(void*o ,bool m);

        void *m_Parent;

        // tell the engine that we are taking control of the
        // timing for output.
        void (*cb_Blocking)(void*o, bool m);

    private:

        std::vector<InputPort*> m_Input;

        std::vector<OutputPort*> m_Output;

        void (*UpdateInfo)(int n,void *);

        int m_HostID;
};

class AudioDriver : public SpiralModule
{
    public:

        AudioDriver(const SpiralInfo *info) : SpiralModule(info)
        {
        }


    	/* if this is an ALWAYS process then ProcessAudio must
    	   always be called regardless whether it has something to
    	   process or not, so it is run along side GUI commands,
    	   ala ExecuteCommands, and is run even if paused.

    	   If its MANUAL we are the ones doing the pushing of data
    	   so we don't have to worry about it if we aren't hooked up,
    	   and we do have to worry about synchronization with other
    	   Modules, so ProcessAudio should be run along side of
    	   regular audio updates, ala Execute. This is eg. for
    	   a File Output driver.

    	   NEVER means we never need to run ProcessAudio, eg,
    	   a dummy audio driver.
    	 */
    	enum AudioProcessType { ALWAYS, MANUAL, NEVER };

    	virtual bool IsAudioDriver() { return true; }

    	virtual void ProcessAudio()=0;

    	virtual AudioProcessType ProcessType() { return NEVER; }

    	// Callbacks to main engine. Should only called by Module hosts.
    	void SetChangeBufferAndSampleRateCallback(void(*s)(long unsigned int, long unsigned int, void *)) { ChangeBufferAndSampleRate = s; } ;

    protected:
    	void (*ChangeBufferAndSampleRate)(long unsigned int BufferSize, long unsigned int SampleRate, void *);
};

#endif
