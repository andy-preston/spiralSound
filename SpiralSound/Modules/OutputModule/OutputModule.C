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

// TODO: compiling in parameters like this might make more of spiralInfo
// redundent.
#define BUF_SIZE 4096


// for lrintf()
#define	_ISOC9X_SOURCE	1
#define _ISOC99_SOURCE	1
#include  <math.h>

// TODO: prune these includes!
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/soundcard.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <ao/ao.h>

#include "OutputModule.h"

using namespace std;

/* TODO: are these rubbish?
static const int IN_FREQ  = 0;
static const int IN_PW = 1;
static const int IN_SHLEN = 2;
static const int OUT_MAIN = 0;
*/

int OutputModule::m_RefCount = 0;
int OutputModule::m_NoExecuted = 0;
OutputModule::Mode OutputModule::m_Mode = NO_MODE;

OutputModule::OutputModule(SpiralInfo* info) : AudioDriver(info)
{
	m_RefCount++;
	// we are an output.
	m_IsTerminal = true;
    m_NotifyOpenOut = false;
	addInput("Left Out", Sample::AUDIO);
	addInput("Right Out", Sample::AUDIO);
}

OutputModule::~OutputModule()
{
	m_RefCount--;
	if (m_RefCount == 0) {
		cb_Blocking(m_Parent, false);
        ao_close(aoDevice);
    	ao_shutdown();
		m_Mode = NO_MODE;
	}
}

void OutputModule::Execute()
{
    if (m_Mode == NO_MODE && m_RefCount == 1) {
        ao_initialize();
        memset(&aoSampleFormat, 0, sizeof(aoSampleFormat));
        aoSampleFormat.bits = 16;
        aoSampleFormat.channels = 2;
        aoSampleFormat.rate = 44100;
        aoSampleFormat.byte_format = AO_FMT_LITTLE;
        aoDevice = ao_open_live(ao_default_driver_id(), &aoSampleFormat, NULL);
    	if (aoDevice == NULL) {
    		cerr << "Error opening libao device." << endl;
    		return;
    	}
        aoBufferSize = aoSampleFormat.bits / 8
            * aoSampleFormat.channels
            * aoSampleFormat.rate;
        aoBuffer = (char *)calloc(aoBufferSize, sizeof(char));

        cb_Blocking(m_Parent, true);
        m_Mode = OUTPUT;
        m_NotifyOpenOut = true;
    }

	if (m_Mode == OUTPUT) {

        OSSClient::Get()->SendStereo(GetInput(0), GetInput(1));
	}
}

void OutputModule::ProcessAudio()
{
	// Only Play() once per set of modules
	m_NoExecuted--;
	if (m_NoExecuted <= 0) {
		if (m_Mode == OUTPUT) {
            ao_play(aoDevice, aoBuffer, aoBufferSize);
        }
		m_NoExecuted = m_RefCount;
	}
}