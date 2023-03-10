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
#include "ScopePlugin.h"
#include "ScopePluginGUI.h"
#include <FL/Fl_Button.H>
#include "SpiralIcon.xpm"

using namespace std;

extern "C"
{
SpiralPlugin* SpiralPlugin_CreateInstance() { return new ScopePlugin; }

char** SpiralPlugin_GetIcon() { return SpiralIcon_xpm; }

int SpiralPlugin_GetID() { return 0x0001; }

string SpiralPlugin_GetGroupName() { return "InputOutput"; }
}

///////////////////////////////////////////////////////

ScopePlugin::ScopePlugin():
m_DataReady(false),
m_DataSize(0)
{
     m_PluginInfo.Name = "Scope";
     m_PluginInfo.Width = 260;
     m_PluginInfo.Height = 115;
     m_PluginInfo.NumInputs = 1;
     m_PluginInfo.NumOutputs = 1;
     m_PluginInfo.PortTips.push_back("Input");
     m_PluginInfo.PortTips.push_back("Output");
     m_AudioCH->Register ("DataReady", &m_DataReady, ChannelHandler::OUTPUT);
     m_AudioCH->Register ("DataSizeChanged", &m_DataSizeChanged, ChannelHandler::OUTPUT);
     m_AudioCH->Register ("DataSize", &m_DataSize, ChannelHandler::OUTPUT);
}

ScopePlugin::~ScopePlugin()
{
}

PluginInfo &ScopePlugin::Initialise(const HostInfo *Host)
{
	PluginInfo& Info = SpiralPlugin::Initialise(Host);
	m_DataSize = m_HostInfo->BUFSIZE;
	m_Data = new float[m_DataSize];
	m_AudioCH->RegisterData("AudioData",ChannelHandler::OUTPUT,m_Data,m_DataSize*sizeof(float));
	return Info;
}

SpiralGUIType *ScopePlugin::CreateGUI()
{
	return new ScopePluginGUI(m_PluginInfo.Width, m_PluginInfo.Height, this, m_AudioCH, m_HostInfo);
}

void ScopePlugin::Reset()
{
	ResetPorts();
	m_DataReady = false;
	delete m_Data;
	m_DataSize = m_HostInfo->BUFSIZE;
	m_Data = new float[m_DataSize];
	m_DataSizeChanged = true;
}

void ScopePlugin::Execute() {
     // Just copy the data through.
     m_DataReady = InputExists (0);
     if (GetOutputBuf (0)) GetOutputBuf (0)->Zero();
     if (m_DataReady) {
        GetOutputBuf (0)->Mix (*GetInput(0), 0);
        memcpy (m_Data, GetInput (0)->GetBuffer (), m_DataSize * sizeof (float));
     }
}

void ScopePlugin::ExecuteCommands()
{
	if (m_AudioCH->IsCommandWaiting())
	{
		switch (m_AudioCH->GetCommand()) {
			case UPDATEDATASIZE :
			{
				m_AudioCH->ReplaceData("AudioData", m_Data, m_DataSize*sizeof(float));
				m_DataSizeChanged = false;			
			}	
			break;
			
			default:
			{
			}
		}	
	}		
}
