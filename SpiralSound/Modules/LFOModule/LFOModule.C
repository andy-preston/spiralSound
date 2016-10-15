/*  SpiralSound
 *  Copyleft (C) 2002 Andy Preston <edgeeffect@gmail.com>
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

#include "LFOModule.h"

using namespace std;

///////////////////////////////////////////////////////

LFOModule::LFOModule() 
{
    m_Type = SINE;
    m_Freq = 0.1;
    m_TableLength = DEFAULT_TABLE_LEN;
    m_CyclePos = 0;
    m_Note = 0;
    m_ModuleInfo.Name = "LFO";
    m_ModuleInfo.NumInputs = 0;
    m_ModuleInfo.NumOutputs = 3;
    m_ModuleInfo.PortTips.push_back ("Output");
    m_ModuleInfo.PortTips.push_back ("'Cosine' Output");
    m_ModuleInfo.PortTips.push_back ("Inverted Output");
    m_AudioCH->Register("Freq", &m_Freq);
    m_AudioCH->Register("Type", (char*)&m_Type);
}

LFOModule::~LFOModule() {
}

ModuleInfo &LFOModule::Initialise (const HostInfo *Host) {
    ModuleInfo& Info= SpiralModule::Initialise (Host);
    for (int n=0; n<NUM_TABLES; n++) {
        m_Table[n].Allocate (m_TableLength);
    }
    WriteWaves();
    return Info;
}

void LFOModule::WriteWaves() {
    float RadCycle = (M_PI/180) * 360;
    float Pos = 0;
    float v = 0;
    float HalfTab = m_TableLength / 2;
    int QuatTab = m_TableLength / 4;
    int ThreeQuatTab = m_TableLength - QuatTab;
    int Shift;
    for (int n = 0; n < m_TableLength; n++) {
        if (n == 0) {
            Pos = 0;
        } else {
            Pos = (n / (float)m_TableLength) * RadCycle;
        }
        m_Table[SINE].Set (n, sin(Pos));
        if (n < QuatTab) {
            Shift = n + ThreeQuatTab;
        } else {
            Shift = n - QuatTab;
        }
        if (n < QuatTab || n > ThreeQuatTab) {
            v = (((Shift - HalfTab) / HalfTab) * 2) - 1;
        } else {
            v = 1 - (Shift / HalfTab * 2);
        }
        m_Table[TRIANGLE].Set(n, v);
        if (n < m_TableLength / 2) {
            m_Table[SQUARE].Set(n, 1.0f);
        } else {
            m_Table[SQUARE].Set(n, -1);
        }
        m_Table[SAW].Set(n, 1.0f - (n / (float)m_TableLength) * 2.0f);
     }
}

float LFOModule::AdjustPos (float pos) {
    while (pos >= m_TableLength) {
        pos -= m_TableLength;
    }
    if (pos < 0 || pos >= m_TableLength) {
        pos = 0;
    }
    return pos;
}

void LFOModule::Reset()
{
	ResetPorts();
	for (int n = 0; n < NUM_TABLES; n++) {
	    m_Table[n].Allocate(m_TableLength);
	}
	WriteWaves();
	m_CyclePos = 0;
	m_Note = 0;
}

void LFOModule::Execute() {
    float Incr, Pos;
    for (int n = 0; n < m_HostInfo->BUFSIZE; n++) {
        Incr = m_Freq * (m_TableLength / (float)m_HostInfo->SAMPLERATE);
        // Raw Output
        m_CyclePos = AdjustPos (m_CyclePos + Incr);
        SetOutput(0, n, m_Table[m_Type][m_CyclePos]);
        // 45 degree out of phase 'Cosine' Output
        Pos = AdjustPos (m_CyclePos + (m_TableLength * 0.25));
        SetOutput(1, n, m_Table[m_Type][Pos]);
        // Inverted Output
        Pos = AdjustPos (m_TableLength - m_CyclePos);
        SetOutput(2, n, m_Table[m_Type][Pos]);
    }
}

void LFOModule::StreamOut(ostream &s) {
    // If this is going to end up in an embedded system
    // this is going to have to be binary not text
    s << m_Version << " " << (int)m_Type << " " << m_Freq << " ";
}

void LFOModule::StreamIn(istream &s) {
     int version;
     s >> version;
     s >> (int&)m_Type >> m_Freq;
}
