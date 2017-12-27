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

// for lrintf()
#define	_ISOC9X_SOURCE	1
#define _ISOC99_SOURCE	1
#include  <math.h>

#include "OutputModule.h"

using namespace std;

/* TODO: are these rubbish?
static const int IN_FREQ  = 0;
static const int IN_PW = 1;
static const int IN_SHLEN = 2;
static const int OUT_MAIN = 0;
*/

OutputModule::OutputModule(SpiralInfo* info) : AudioDriver(info)
{
    ao_sample_format aoSampleFormat;
    memset(&aoSampleFormat, 0, sizeof(aoSampleFormat));
    // Everything is going to be 16 bit stereo, little endian
    aoSampleFormat.byte_format = AO_FMT_LITTLE;
    aoSampleFormat.bits = 16;
    aoSampleFormat.channels = 2;
    // Sample Rate is dictated by SpiralInfo
    aoSampleFormat.rate = info->sampleRate;
    aoBufferSize = aoSampleFormat.bits / 8 * aoSampleFormat.channels
        * spiralInfo->bufferSize;
    aoBuffer = (char *)calloc(aoBufferSize, sizeof(char));
    ao_initialize();
    aoDevice = ao_open_live(ao_default_driver_id(), &aoSampleFormat, NULL);
    if (aoDevice == NULL) {
        cerr << "Error opening libao device" << endl;
        return;
    }
	m_IsTerminal = true; // we are an output.
	addInput("Left Out", Sample::AUDIO);
	addInput("Right Out", Sample::AUDIO);
    // TODO: we might care about cb_Blocking in the future
    // but it segfaults right now
    // cb_Blocking(m_Parent, true);
}

OutputModule::~OutputModule()
{
    // TODO: we might care about cb_Blocking in the future
    // but it segfaults right now
	//cb_Blocking(m_Parent, false);
    ao_close(aoDevice);
	ao_shutdown();
}

void OutputModule::Execute()
{
    const Sample *ldata, *rdata;
    int bufIdx, n, samp;
    ldata = GetInput(0);
    rdata = GetInput(1);
    bufIdx = 0;
    for (n = 0; n < spiralInfo->bufferSize; n++) {
        // stereo channels - interleave
        samp = clipData((*ldata)[n]);
        aoBuffer[bufIdx++] = samp & 0xff;
        aoBuffer[bufIdx++] = (samp >> 8) & 0xff;
        samp = clipData((*rdata)[n]);
        aoBuffer[bufIdx++] = samp & 0xff;
        aoBuffer[bufIdx++] = (samp >> 8) & 0xff;
    }
    ao_play(aoDevice, aoBuffer, aoBufferSize);
}

int OutputModule::clipData(float t) {
    return lrintf(
        0.5 * SHRT_MAX * (
            // These 0s were originally -1, but 0 seems to make more sense to me
            (t > 1) ? 1 : (t < 0) ? 0 : t
        )
    );
}