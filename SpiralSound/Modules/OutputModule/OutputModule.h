/*  SpiralSound Copyleft (C) 2017 Andy Preston <edgeeffect@gmail.com>
 *  Based on SpiralSynthModular Copyleft (C) 2001 David Griffiths <dave@pawfal.org>
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
#include "../../RiffWav.h"

#ifndef OUTPUT_MODULE
#define OUTPUT_MODULE

class OutputModule : public AudioDriver
{
    public:
	    enum Mode {NO_MODE, OUTPUT, CLOSED};
 	    OutputModule(SpiralInfo* info);
	    virtual ~OutputModule();
	    //virtual PluginInfo& Initialise(const HostInfo *Host);
	    virtual void Execute();
	    // virtual void ExecuteCommands();
        virtual void ProcessAudio();
        Mode GetMode() { return m_Mode; }
    private:
	    static int m_RefCount;
	    static int m_NoExecuted;
	    static Mode m_Mode;
        bool m_NotifyOpenOut;
	    bool m_CheckedAlready;

        // libao stuffs
        ao_device *aoDevice;
        ao_sample_format aoSampleFormat;
        char *aoBuffer;
        int aoBufferSize;
};

#endif