/*  SpiralSound
 *  Copyleft (C) 2001 David Griffiths <dave@pawfal.org>
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
#include "CounterPlugin.h"
#include "CounterPluginGUI.h"
#include <FL/Fl_Button.H>
#include "SpiralIcon.xpm"
#include "../../NoteTable.h"

using namespace std;

extern "C" {
SpiralPlugin* SpiralPlugin_CreateInstance()
{
	return new CounterPlugin;
}

char** SpiralPlugin_GetIcon()
{
	return SpiralIcon_xpm;
}

int SpiralPlugin_GetID()
{
	return 45;
}

string SpiralPlugin_GetGroupName()
{
	return "Maths/Logic";
}
}

///////////////////////////////////////////////////////

CounterPlugin::CounterPlugin() :
m_Count(4),
m_Current(0),
m_Triggered(false),
m_CurrentLevel(1.0f)
{
	m_PluginInfo.Name="Counter";
	m_PluginInfo.Width=80;
	m_PluginInfo.Height=50;
	m_PluginInfo.NumInputs=1;
	m_PluginInfo.NumOutputs=1;
	m_PluginInfo.PortTips.push_back("Input");	
	m_PluginInfo.PortTips.push_back("Output");

	m_AudioCH->Register("Count",&m_Count);
}

CounterPlugin::~CounterPlugin()
{
}

PluginInfo &CounterPlugin::Initialise(const HostInfo *Host)
{	
	return SpiralPlugin::Initialise(Host);
}

SpiralGUIType *CounterPlugin::CreateGUI()
{
	return new CounterPluginGUI(m_PluginInfo.Width,
						     m_PluginInfo.Height,
							 this,m_AudioCH,m_HostInfo);
}

void CounterPlugin::Reset()
{
	ResetPorts();
	m_Current = 0;
	m_Triggered = false;
	m_CurrentLevel = 1.0f;
}


void CounterPlugin::Execute()
{	
	bool Triggered;
	
	for (int n=0; n<m_HostInfo->BUFSIZE; n++)
	{
		if (GetInput(0,n)>0)
		{
			if(!m_Triggered)
			{
				m_Triggered=true;
				m_Current++;
			}
		}
		else
		{
			if (m_Triggered)
			{
				m_Triggered=false;
				m_Current++;
			}
		}

		if (m_Current>=m_Count)
		{
			m_CurrentLevel=-m_CurrentLevel;
			m_Current=0;
		}
		
		SetOutput(0,n,m_CurrentLevel);
	}
}

void CounterPlugin::ExecuteCommands()
{
}
	
void CounterPlugin::StreamOut(ostream &s) 
{
	s<<m_Version<<endl;
	s<<m_Count<<" "<<m_Current<<" ";
}

void CounterPlugin::StreamIn(istream &s) 
{
	int version;
	s>>version;
	s>>m_Count>>m_Current;
}
