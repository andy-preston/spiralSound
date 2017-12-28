/*
 * SpiralSound oscillator module
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

#include <limits.h>
#include <stdlib.h>
#include "OscillatorModule.h"
#include "../../Sample.h"

using namespace std;

static const int IN_FREQ  = 0;
static const int IN_PW = 1;
static const int IN_SHLEN = 2;
static const int OUT_MAIN = 0;

OscillatorModule::OscillatorModule(const SpiralInfo *info) : SpiralModule(info)
{
    m_Type = SQUARE;
    m_Octave = 0;
    m_FineFreq = 1.0f;
    m_PulseWidth = 0.5f;
    m_SHLen = 0.1f;
    m_ModAmount = 1.0f;
    m_Noisev = 0;
    m_FreqModBuf = NULL;
    m_PulseWidthModBuf = NULL;
    m_SHModBuf = NULL;

	m_CyclePos = 0;
	m_Note = 0;
	m_LastFreq = 0;

    // Input types might be wrong as these are CV inputs not audio
    addInput("Frequency");
    addInput("PulseWidth");
    addInput("Sample & Hold length");
    addOutput("Output");
	addIntControl("Octave", &m_Octave);
	addFloatControl("FineFreq", &m_FineFreq);
	addFloatControl("PulseWidth", &m_PulseWidth);
	addIntControl("Type", &m_Type);
	addFloatControl("SHLen", &m_SHLen);
	addFloatControl("ModAmount", &m_ModAmount);
}

OscillatorModule::~OscillatorModule()
{
}

void OscillatorModule::Execute()
{
	short noisev = 0;
	float Freq = 0;
	float CycleLen = 0;
	int samplelen, PW;
	switch (m_Type) {
	    case SQUARE:
            for (int n=0; n<spiralInfo->bufferSize; n++) {
                Freq = InputExists(0) ? GetInputPitch(0,n) : 110;
                Freq *= m_FineFreq;
                if (m_Octave > 0) {
                    Freq *= 1 << (m_Octave);
                }
                if (m_Octave<0) {
                    Freq /= 1 << (-m_Octave);
                }
                CycleLen = spiralInfo->sampleRate / Freq;
                PW = (int)((m_PulseWidth + GetInput(IN_PW, n) * m_ModAmount) * CycleLen);
                // calculate square wave pattern
                m_CyclePos++;
                if (m_CyclePos > CycleLen) {
                    m_CyclePos = 0;
                }
                if (m_CyclePos < PW) {
                    SetOutput(OUT_MAIN, n, 1);
                } else {
                    SetOutput(OUT_MAIN, n, -1);
                }
            }
            break;
        case SAW:
            for (int n=0; n<spiralInfo->bufferSize; n++) {
                Freq = InputExists(0) ? GetInputPitch(0,n) : 110;
                Freq *= m_FineFreq;
                if (m_Octave > 0) {
                    Freq *= 1 << (m_Octave);
                }
                if (m_Octave<0) {
                    Freq /= 1 << (-m_Octave);
                }
                CycleLen = spiralInfo->sampleRate / Freq;
                PW = (int)((m_PulseWidth + GetInput(IN_PW, n) * m_ModAmount) * CycleLen);
                // get normailise position between cycle
                m_CyclePos++;
                if (m_CyclePos > CycleLen) {
                    m_CyclePos = 0;
                }
                if (m_CyclePos < PW) {
                    // before pw -1->1
                    SetOutput(OUT_MAIN, n, Linear(0, PW, m_CyclePos, -1, 1));
                } else {
                    // after pw 1->-1
    				SetOutput(OUT_MAIN, n, Linear(PW, CycleLen, m_CyclePos, 1, -1));
                }
            }
            break;
        case NOISE:
            for (int n = 0; n < spiralInfo->bufferSize; n++) {
                m_CyclePos++;
                //modulate the sample & hold length
                samplelen = (int)(
                    (m_SHLen + GetInput(IN_SHLEN, n) * m_ModAmount)
                        * spiralInfo->sampleRate
                );
                // do sample & hold on the noise
                if (m_CyclePos>samplelen) {
                    m_Noisev = (short)((rand() % SHRT_MAX * 2) - SHRT_MAX);
                    m_CyclePos = 0;
                }
                SetOutput(OUT_MAIN, n, m_Noisev / (float)SHRT_MAX);
            }
            break;
        case NONE:
            break;
    }
}