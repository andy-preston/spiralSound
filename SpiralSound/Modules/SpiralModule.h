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
#include <math.h>
#include <stdlib.h>
#include "../SpiralInfo.h"
#include "../Sample.h"

static const float MAX_FREQ = 13000;

class InputPort
{
    public:
        InputPort(const char* name, Sample::SampleType type, const Sample* data);
        const Sample* Data;
        const char *Name;
        Sample::SampleType Type;
};

class OutputPort {
    public:
        OutputPort(const char* name, Sample::SampleType type, Sample* data);
        Sample* Data;
        const char *Name;
        Sample::SampleType Type;
};

/*
struct ModuleControl
{
    char* name;
};
*/

class SpiralModule {
    public:
	    SpiralModule(const SpiralInfo *info);

	    virtual ~SpiralModule();

        // execute the audio
	    virtual void Execute() = 0;

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

        void GetOutput(const char *name, Sample **s);
        void SetInput(const char *name, const Sample *s);

        const Sample* GetInput(unsigned int n)
        {
            return m_Input[n]->Data;
        }

        void SetParent(void *s) {
            m_Parent = s;
        }

        // is the Module connected to an external device (oss/alsa/jack)
        bool m_IsTerminal;

        virtual bool IsAudioDriver() {
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

        // TODO: these 3 will become redundant
        void addIntControl(const char* name, int* data);
        void addBoolControl(const char* name, bool* data);
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
        AudioDriver(const SpiralInfo *info) : SpiralModule(info) {}
    	virtual bool IsAudioDriver() { return true; }
    	virtual void ProcessAudio() {}
};

#endif
