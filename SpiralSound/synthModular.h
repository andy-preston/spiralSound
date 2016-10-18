/*  SpiralSynthModular
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

#ifndef SPIRALSOUNDDEMO
#define SPIRALSOUNDDEMO

#include <stdio.h>
#include "../SpiralSound/SpiralInfo.h"
#include "../SpiralSound/Modules/SpiralModule.h"
#include "../SpiralSound/ChannelHandler.h"

class SynthModular
{
    public:
	    SynthModular(SpiralInfo *info);
	    ~SynthModular();
	    void Update();
	    void AddDevice(int n, int x, int y);
        void ClearUp();
        void UpdateHostInfo();
        bool CallbackMode() {
            return m_CallbackUpdateMode;
        }
        bool IsBlockingOutputPluginReady() {
            return m_BlockingOutputPluginIsReady;
        }
        void LoadPatch(const char *fn);
        void FreezeAll() {
            m_CH.Set("Frozen", true);
            m_CH.Wait();
        }
        void ThawAll() {
            m_CH.Set("Frozen", false);
        }
        void PauseAudio() {
            m_Info.PAUSED = true;
        }
        void ResumeAudio() {
            m_Info.PAUSED = false;
        }
        void ResetAudio() {
            if (! m_ResetingAudioThread) {
                FreezeAll();
                m_ResetingAudioThread = true;
                ThawAll();
		    }
	    }
        // only for audio thread
        bool IsFrozen() { return m_Frozen; }
    private:
        SpiralInfo *m_spiralInfo;
        HostInfo m_Info;
	    bool m_ResetingAudioThread, m_HostNeedsUpdate, m_Frozen;
	    // currently static, to allow the callback
	    // cb_UpdatePluginInfo to access the map.
        int m_NextID;
        static bool m_CallbackUpdateMode;
        static bool m_BlockingOutputPluginIsReady;
        // Main GUI stuff
        ChannelHandler m_CH; // used for threadsafe communication
        inline void cb_ChangeBufferAndSampleRate_i(long int NewBufferSize, long int NewSamplerate);
        static void cb_ChangeBufferAndSampleRate(long unsigned int NewBufferSize, long unsigned int NewSamplerate, void *o)
        {
		    ((SynthModular*)o)->cb_ChangeBufferAndSampleRate_i(NewBufferSize, NewSamplerate);
        }
};

#endif