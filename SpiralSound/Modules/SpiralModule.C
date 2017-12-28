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

#include "SpiralModule.h"

using namespace std;

InputPort::InputPort(const char* name, const Sample* data)
{
    Name = name;
    Data = data;
}

OutputPort::OutputPort(const char* name, Sample* data)
{
    Name = name;
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

void SpiralModule::addOutput(const char* name)
{
	Sample* sample = new Sample(spiralInfo->bufferSize);
    OutputPort* port = new OutputPort(name, sample);
	m_Output.push_back(port);
}

void SpiralModule::addInput(const char* name)
{
    InputPort* port = new InputPort(name, NULL);
    m_Input.push_back(port);
}

void SpiralModule::GetOutput(const char *name, Sample **s)
{
    for(std::vector<OutputPort*>::iterator outputPort = m_Output.begin();
            outputPort != m_Output.end(); ++outputPort) {
        if (strcmp((*outputPort)->Name, name) == 0) {
            *s = (*outputPort)->Data;
            return;
        }
    }
    cerr << "Can't find output: " << name << endl;
    exit(1);
}

void SpiralModule::SetInput(const char *name, const Sample *s)
{
    for(std::vector<InputPort*>::iterator inputPort = m_Input.begin();
            inputPort != m_Input.end(); ++inputPort) {
        if (strcmp((*inputPort)->Name, name) == 0) {
            (*inputPort)->Data = s;
            return;
        }
    }
    cerr << "Can't find input: " << name << endl;
    exit(1);
}

void SpiralModule::addIntControl(const char* name, int* data)
{
    // TODO
}

void SpiralModule::addBoolControl(const char* name, bool* data)
{
    // TODO
}

void SpiralModule::addFloatControl(const char* name, float* data)
{
    // TODO
}
