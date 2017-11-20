/*  SpiralSound Copyleft (C) 2017 Andy Preston <edgeeffect@gmail.com>
 *  Based on SpiralSynthModular Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
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

#include <sstream>
#include "SpiralModule.h"

using namespace std;

InputPort::InputPort(const char* name, Sample::SampleType type, const Sample* data)
{
    Name = name;
    Type = type;
    Data = data;
}

OutputPort::OutputPort(const char* name, Sample::SampleType type, Sample* data)
{
    Name = name;
    Type = type;
    Data = data;
}

SpiralModule::SpiralModule(const SpiralInfo *info)
{
	UpdateInfo = NULL;
	cb_Update = NULL;
	m_Parent = NULL;
	m_HostID = -1;
	m_IsTerminal = false;
    spiralInfo = info;
}

SpiralModule::~SpiralModule()
{
}

/*
void SpiralModule::UpdateChannelHandler()
{
    m_AudioCH->UpdateDataNow();
}
*/

void SpiralModule::addOutput(const char* name, Sample::SampleType type)
{
	Sample* sample = new Sample(spiralInfo->BUFSIZE);
    OutputPort* port = new OutputPort(name, type, sample);
	m_Output.push_back(port);
}

void SpiralModule::addInput(const char* name, Sample::SampleType type)
{
    InputPort* port = new InputPort(name, type, NULL);
    m_Input.push_back(port);
}

void SpiralModule::addIntControl(const char* name, int* data)
{
    // TODO
}

void SpiralModule::addFloatControl(const char* name, float* data)
{
    // TODO
}
