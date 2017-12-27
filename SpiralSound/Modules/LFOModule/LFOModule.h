/* SpiralSound LFO module
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

#ifndef LFO_MODULE
#define LFO_MODULE

#include "../SpiralModule.h"

static const int NUM_TABLES = 6;
static const int DEFAULT_TABLE_LEN = 1024;

class LFOModule : public SpiralModule {
    public:
        LFOModule(const SpiralInfo *info);
        virtual ~LFOModule();
        virtual void Execute();
        enum Type {SINE, TRIANGLE, SQUARE, SAW};
        void WriteWaves();
        void NoteTrigger(int V, int s, int v);
        float m_Freq;
        int m_Type;
    private:
        float AdjustPos (float pos);
        // Voice specific parameter
        int m_Note;
        float m_CyclePos;
        // Common voice parameters
        Sample m_Table[NUM_TABLES];
        int m_TableLength;
};

#endif