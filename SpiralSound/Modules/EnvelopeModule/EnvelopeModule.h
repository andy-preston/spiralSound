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

#include "../SpiralModule.h"

#ifndef ENVELOPE_MODULE
#define ENVELOPE_MODULE

class EnvelopeModule : public SpiralModule
{
    public:
     	EnvelopeModule(SpiralInfo *info);
    	virtual ~EnvelopeModule();
    	virtual void Execute();
        float m_Attack;
    	float m_Decay;
    	float m_Sustain;
    	float m_Release;
    	float m_Volume;
    	float m_TrigThresh;
    private:
    	// Voice specific parameters
    	bool m_Trigger;
    	float m_time;
    	// Common voice parameters
    	float m_SampleTime;
    	float m_Current;
};

#endif