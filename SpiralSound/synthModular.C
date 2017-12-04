/*
 * SpiralSound demo synth
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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#include "synthModular.h"

//#define DEBUG_MODULES
//#define DEBUG_STREAM

using namespace std;

bool SynthModular::m_BlockingOutputModuleIsReady = false;

//////////////////////////////////////////////////////////

SynthModular::SynthModular(SpiralInfo *info) {
    spiralInfo = info;
}

//////////////////////////////////////////////////////////

SynthModular::~SynthModular()
{
}

//////////////////////////////////////////////////////////

void SynthModular::run()
{
    for (;;) {
        Update();
        // put the brakes on if there is no blocking output running
        if (!IsBlockingOutputModuleReady()) {
            usleep(10000);
        }
    }
}

void SynthModular::Update()
{
    // TODO - we need something to do the job of the ChannelHandler
    // and pass commands on to the modules
	//m_CH.UpdateDataNow();

	for(map<int,SpiralModule*>::iterator i=deviceMap.begin();
            i!=deviceMap.end(); i++) {

        #ifdef DEBUG_MODULES
            cerr << "Updating channelhandler of Module " << i->second->moduleID << endl;
		#endif

        // TODO - we need something to do the job of the ChannelHandler
        // and pass commands on to the modules
		//i->second->UpdateChannelHandler();

		#ifdef DEBUG_MODULES
            cerr << "Finished updating" << endl;
		#endif

		// If this is an audio device see if we always need to ProcessAudio here
		if (i->second->IsAudioDriver()) {
			AudioDriver *driver = ((AudioDriver *)i->second);
			if (driver->ProcessType() == AudioDriver::ALWAYS) {
				driver->ProcessAudio();
			}
		}
		// run any commands we've received from the Control Panel
        // (formerly from the GUI)
		/*
        i->second->ExecuteCommands();
        */

        // SpiralSynthModular used a graph-sort here to determine the execution
        // order and thus remove latency.

		if ((i != deviceMap.end())) {

			#ifdef DEBUG_MODULES
                cerr << "Executing Module " << i->second->moduleID << endl;
			#endif

			i->second->Execute();
			// If this is an audio device see if we need to ProcessAudio here
			if (i->second->IsAudioDriver()) {
				AudioDriver *driver = ((AudioDriver *)i->second);
				if (driver->ProcessType() == AudioDriver::MANUAL) {
					driver->ProcessAudio();
				}
			}

			#ifdef DEBUG_MODULES
                cerr << "Finished executing" << endl;
			#endif
		}
	}
}

void SynthModular::addModule(SpiralModule* module)
{
    module->SetParent((void*)this);
    /*
    if (module->IsAudioDriver()) {
        AudioDriver *driver = ((AudioDriver*)module);
    	driver->SetChangeBufferAndSampleRateCallback(
            cb_ChangeBufferAndSampleRate
        );
    }
    */
}