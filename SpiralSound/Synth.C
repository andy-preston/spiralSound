/*
 * SpiralSound Synth Engine
 *     - Copyleft (C) 2016 Andy Preston <edgeeffect@gmail.com
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

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "Synth.h"

using namespace std;

bool Synth::m_BlockingOutputModuleIsReady = false;

//////////////////////////////////////////////////////////

Synth::Synth(SpiralInfo *info)
{
    spiralInfo = info;
}

Synth::~Synth()
{
}

void Synth::run()
{
    endlessloop:

    // TODO - we need something to do the job of the ChannelHandler
    // and pass commands on to the modules
	//m_CH.UpdateDataNow();

	for(list<SpiralModule*>::iterator module = moduleList.begin();
            module != moduleList.end(); module++) {

        // TODO - we need something to do the job of the ChannelHandler
        // and pass commands on to the modules
		//module->UpdateChannelHandler();

		if ((*module)->IsAudioDriver()) {
			((AudioDriver *)(*module))->ProcessAudio();
		}
		// run any commands we've received from the Control Panel
        // (formerly from the GUI)
		/*
        (*module)->ExecuteCommands();
        */

        // SpiralSynth used a graph-sort here to determine the execution
        // order and thus remove latency.

		if ((module != moduleList.end())) {
			(*module)->Execute();
		}
	}
    // put the brakes on if there is no blocking output running
    if (!IsBlockingOutputModuleReady()) {
        usleep(10000);
    }

    goto endlessloop;
}

void Synth::addModule(SpiralModule* module)
{
    module->SetParent((void*)this);
    moduleList.push_back(module);
    /*
    if (module->IsAudioDriver()) {
        AudioDriver *driver = ((AudioDriver*)module);
    	driver->SetChangeBufferAndSampleRateCallback(
            cb_ChangeBufferAndSampleRate
        );
    }
    */
}

void Synth::connect(SpiralModule* sourceMod, const char *sourcePort,
    SpiralModule* destMod, const char *destPort)
{
	Sample *sample=NULL;
	sourceMod->GetOutput(sourcePort, &sample);
    destMod->SetInput(destPort, (const Sample*)sample);
}
