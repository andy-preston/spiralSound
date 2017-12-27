/*
 * SpiralSound filter module
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

#include "FilterModule.h"
#include <cmath>

using namespace std;

static const float MAX_RES = 10;
static const float MIN_RES = 1;
static const float MAX_CUTOFF = 3000;
static const float MIN_CUTOFF = 500;
static const int FILTERGRAN = 50;
static const int NUM_CHANNELS = 4;

FilterModule::FilterModule(const SpiralInfo *info) : SpiralModule(info)
{
    fs = 0; // Sampling frequency
    fc = 100; // Filter cutoff
    Q = 1; // Resonance
    m_LastFC = 0;
    m_LastQ = 1;
    k = 1.0; // Set overall filter gain
    m_RevCutoffMod = false;
    m_RevResonanceMod = false;

    fs = info->sampleRate;

    ProtoCoef[0].a0 = 1.0;
	ProtoCoef[0].a1 = 0;
	ProtoCoef[0].a2 = 0;
	ProtoCoef[0].b0 = 1.0;
	ProtoCoef[0].b1 = 0.765367;
	ProtoCoef[0].b2 = 1.0;

	ProtoCoef[1].a0 = 1.0;
	ProtoCoef[1].a1 = 0;
	ProtoCoef[1].a2 = 0;
	ProtoCoef[1].b0 = 1.0;
	ProtoCoef[1].b1 = 1.847759;
	ProtoCoef[1].b2 = 1.0;

    iir.length = FILTER_SECTIONS;
    iir.coef = (float *) calloc(4 * iir.length + 1 , sizeof(float));

	if (!iir.coef) {
        cerr << "Unable to allocate coef array" << endl;
        exit(1);
    }

    addInput("Input", Sample::AUDIO);
    addInput("Cutoff", Sample::AUDIO);
    addInput("Resonance", Sample::AUDIO);
    addOutput("Output", Sample::AUDIO);

	addFloatControl("Cutoff", &fc);
	addFloatControl("Resonance", &Q);
	addBoolControl("RevC", &m_RevCutoffMod);
	addBoolControl("RevR", &m_RevResonanceMod);
}

FilterModule::~FilterModule()
{
}

void FilterModule::Execute()
{
	float Cutoff, Resonance, out, in;
    int n;

	if (fc < 0) {
        return;
    }

	for (n = 0; n < spiralInfo->bufferSize; n++) {
		//reset memory if disconnected, and skip out (prevents CPU spike)
		if (! InputExists(0)) {
			out = 0;
		} else {
			in = GetInput(0, n);
			// work around denormal calculation CPU spikes where in --> 0
			if ((in >= 0) && (in < 0.000000001)) {
				in += 0.000000001;
            } else if ((in <= 0) && (in > -0.000000001)) {
                in -= 0.000000001;
            }
			coef = iir.coef + 1; /* Skip k, or gain */
			k = 0.25;
			Cutoff = fc + (GetInput(1, n) * 1000);
			Resonance = Q + GetInput(2, n);
			Cutoff /= 2;
			if (Resonance > MAX_RES) {
                Resonance = MAX_RES;
            }
			if (Cutoff > MAX_CUTOFF) {
                Cutoff = MAX_CUTOFF;
            }
			if (Resonance < MIN_RES) {
                Resonance = MIN_RES;
            }
			if (Cutoff < MIN_CUTOFF) {
                Cutoff = MIN_CUTOFF;
            }
			if (n % FILTERGRAN == 0) {
				for (nInd = 0; nInd < iir.length; nInd++) {
					a2 = ProtoCoef[nInd].a2;
					a0 = ProtoCoef[nInd].a0;
					a1 = ProtoCoef[nInd].a1;

					b0 = ProtoCoef[nInd].b0;
					b1 = ProtoCoef[nInd].b1 / Resonance;
					b2 = ProtoCoef[nInd].b2;

					szxform(&a0, &a1, &a2, &b0, &b1, &b2,
                        Cutoff * (Cutoff / 1000.0f), fs, &k, coef);
					coef += 4;
					iir.coef[0] = k;
					m_LastQ = Q;
					m_LastFC = fc;
				}
			}
		 	out = iir_filter(in / 0.5f, &iir);
		}
	 	SetOutput(0, n, out);
	}
}