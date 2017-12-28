/* SpiralSound Envelope module
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

#include "EnvelopeModule.h"

using namespace std;

// a bit of a crap filter to smooth clicks
static float SMOOTH = 0.99;
static float ONEMINUS_SMOOTH = 1 - SMOOTH;

EnvelopeModule::EnvelopeModule(SpiralInfo *info) : SpiralModule(info)
{
    m_Trigger = false;
    m_time = 1.0f;
    m_Attack = 0.0f;
    m_Decay = 0.5f;
    m_Sustain = 1.0f;
    m_Release = 1.0f;
    m_Volume = 0.5f;
    m_TrigThresh = 0.01;
    m_Current = 0;
    m_SampleTime = 1.0 / (float)(info->sampleRate);

	addInput("Trigger", Sample::AUDIO);
    addInput("Input", Sample::AUDIO);
	addInput("Input", Sample::AUDIO);
	addOutput("CV", Sample::AUDIO);
	addOutput("Output", Sample::AUDIO);

    /*
	m_AudioCH->Register("Attack",&m_Attack);
	m_AudioCH->Register("Decay",&m_Decay);
	m_AudioCH->Register("Sustain",&m_Sustain);
	m_AudioCH->Register("Release",&m_Release);
	m_AudioCH->Register("Volume",&m_Volume);
	m_AudioCH->Register("Trig",&m_TrigThresh);
    */
}

EnvelopeModule::~EnvelopeModule()
{
}

void EnvelopeModule::Execute()
{
	float nt, volume = 0;
	bool Freeze = false;
    int n;
	for (n = 0; n < spiralInfo->bufferSize; n++) {
		// Check the trigger CV values
		if (GetInput(0, n) > m_TrigThresh) {
			if (m_Trigger == false) {
				m_time = 0.0f;
				m_Trigger = true;
			}
		} else {
			m_Trigger = false;
		}
		// if we are in the envelope...
		if (m_time >= 0 && m_time < m_Attack + m_Decay + m_Release) {
			// find out what part of the envelope we are in
			if (m_time < m_Attack) {
				// get normalised position to
				// get the volume between 0 and 1
				volume = m_time / m_Attack;
			} else if (m_time < m_Attack + m_Decay) {
				// normalised position in m_Attack->m_Decay range
				nt = (m_time - m_Attack) / m_Decay;
				// volume between 1 and m_Sustain
				volume = (1 - nt) + (m_Sustain * nt);
			} else {
				// normalised position in m_Decay->m_Release range
				nt = (m_time - (m_Attack + m_Decay)) / m_Release;
				// volume between m_Sustain and 0
				volume = m_Sustain * (1 - nt);
				if (m_Release < 0.2f) {
					volume = m_Sustain;
				}
				if (m_Trigger) {
                    Freeze = true;
                }
			}
			volume *= m_Volume;
			if (!feq(volume, m_Current, 0.01)) {
				// only filter if necc
				volume = (volume * ONEMINUS_SMOOTH + m_Current * SMOOTH);
			}
			SetOutput(0, n, volume);
			SetOutput(1, n, GetInput(1, n) * volume);
			m_Current = volume;
			if (!Freeze) {
                m_time += m_SampleTime;
            }
		} else {
			if (!feq(volume, m_Current, 0.01)) {
				volume = m_Current * SMOOTH;
			}
			SetOutput(0, n, volume);
			SetOutput(1, n, GetInput(1, n) * volume);
			m_Current = volume;
			// if we've run off the end
			if (m_time > m_Attack + m_Decay + m_Release) {
				m_time = -1;
				return;
			}
		}
	}
}