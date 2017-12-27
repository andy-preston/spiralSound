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

#ifndef FILTER_MODULE
#define FILTER_MODULE

#include "../SpiralModule.h"
#include "iir_filter.h"

class FilterModule : public SpiralModule
{
    public:
     	FilterModule(const SpiralInfo *info);
    	virtual ~FilterModule();
    	virtual void Execute();
    	double GetCutoff() { return fc; }
    	double GetResonance() { return Q; }
    	bool GetRevCutoffMod() { return m_RevCutoffMod; }
    	bool GetRevResonanceMod() { return m_RevResonanceMod; }
    private:
    	// Voice specifics
    	FILTER iir;
    	// Voice independant
    	float *coef;
        float fs, fc; // Sampling frequency, cutoff frequency
        float Q; // Resonance > 1.0 < 1000
    	float m_LastFC;
    	float m_LastQ;
        unsigned nInd;
        double a0, a1, a2, b0, b1, b2;
        double k; // overall gain factor
    	bool m_RevCutoffMod;
    	bool m_RevResonanceMod;
};

#endif