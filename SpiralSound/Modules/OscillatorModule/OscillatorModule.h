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

#include "../SpiralModule.h"

#ifndef OSCILLATOR_MODULE
#define OSCILLATOR_MODULE

class OscillatorModule : public SpiralModule
{
    public:
        OscillatorModule();
        virtual ~OscillatorModule();
        virtual ModuleInfo &Initialise(const HostInfo *Host);
        virtual void Execute();
        virtual void Reset();
        virtual char *StreamOut();
        virtual void StreamIn(char *data);
        typedef char Type;
        enum { NONE, SQUARE, SAW, NOISE };
        void ModulateFreq(Sample *data)
        {
            m_FreqModBuf = data;
        }
        void ModulatePulseWidth(Sample *data)
        {
            m_PulseWidthModBuf = data;
        }
        void ModulateSHLen(Sample *data)
        {
            m_SHModBuf = data;
        }
        void NoteTrigger(int V, int s, int v);
        int m_Octave;
        float m_FineFreq;
        float m_PulseWidth;
        Type m_Type;
        float m_SHLen;
        float m_ModAmount;
    private:
        // Voice specific parameter
        int m_Note;
        int m_CyclePos;
        float m_LastFreq;
        // Common voice parameters
        short m_Noisev;
        Sample *m_FreqModBuf;
        Sample *m_PulseWidthModBuf;
        Sample *m_SHModBuf;
        static const int FIXED;
};

#endif