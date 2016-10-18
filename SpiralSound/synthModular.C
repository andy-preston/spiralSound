/*
 * SpiralSound demo synth
 *     - Copyleft (C) 2016 Andy Preston <edgeeffect@gmail.com
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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#include "LittleSynth.h"
#include "../SpiralSound/ModuleManager.h"
#include "../SpiralSound/Thread.h"

//#define DEBUG_MODULES
//#define DEBUG_STREAM

using namespace std;

bool SynthModular::m_CallbackUpdateMode = false;
bool SynthModular::m_BlockingOutputModuleIsReady = false;

//////////////////////////////////////////////////////////

SynthModular::SynthModular(SpiralInfo *info) {
    m_spiralInfo = info;
    m_Frozen = false;
    m_NextID = 0;

	/* Shared Audio State Information  */
	m_Info.BUFSIZE = m_spiralInfo->BUFSIZE;
	m_Info.SAMPLERATE = m_spiralInfo->SAMPLERATE;
	m_Info.PAUSED = false;

	/* obsolete - REMOVE SOON  */
	m_Info.FRAGSIZE = m_spiralInfo->FRAGSIZE;
	m_Info.FRAGCOUNT = m_spiralInfo->FRAGCOUNT;
	m_Info.OUTPUTFILE = m_spiralInfo->OUTPUTFILE;
	m_Info.MIDIFILE = m_spiralInfo->MIDIFILE;
	m_Info.POLY = m_spiralInfo->POLY;

	m_CH.Register("Frozen", &m_Frozen);
}

//////////////////////////////////////////////////////////

SynthModular::~SynthModular()
{
	ClearUp();
	ModuleManager::Get()->PackUpAndGoHome();
}

//////////////////////////////////////////////////////////

void SynthModular::ClearUp()
{
	FreezeAll();
	for(map<int,DeviceWin*>::iterator i=m_DeviceWinMap.begin();
		i!=m_DeviceWinMap.end(); i++)
	{
        //Stop processing of audio if any
		if (i->second->m_Device) {
			if (i->second->m_Device->Kill());
		}
		//Delete Device
		delete i->second->m_Device;
		i->second->m_Device=NULL;
	}
	m_DeviceWinMap.clear();
	m_NextID=0;
	ThawAll();
}

//////////////////////////////////////////////////////////
void SynthModular::Update()
{
	m_CH.UpdateDataNow();

	if (m_Frozen) return;

	// for all the Modules
	for(map<int,DeviceWin*>::iterator i=m_DeviceWinMap.begin();
		i!=m_DeviceWinMap.end(); i++)
	{
		if (i->second->m_Device && i->second->m_Device->IsDead())
		{
			//Delete Device
			delete i->second->m_Device;
			i->second->m_Device=NULL;

			//Erase Device from DeviceWinMap
			m_DeviceWinMap.erase(i);
		}
		else if (i->second->m_Device) // if it's not a comment
		{
			#ifdef DEBUG_MODULES
			cerr<<"Updating channelhandler of Module "<<i->second->m_ModuleID<<endl;
			#endif

			// updates the data from the gui thread, if it's not blocking
 			i->second->m_Device->UpdateChannelHandler();

			#ifdef DEBUG_MODULES
			cerr<<"Finished updating"<<endl;
			#endif

			// If this is an audio device see if we always need to ProcessAudio here
			if ((!m_ResetingAudioThread))
			{
				if (i->second->m_Device->IsAudioDriver())
				{
					AudioDriver *driver = ((AudioDriver *)i->second->m_Device);

					if (driver->ProcessType() == AudioDriver::ALWAYS)
					{
						driver->ProcessAudio();
					}
				}

				// run any commands we've received from the GUI's
				i->second->m_Device->ExecuteCommands();
			}
		}
	}

	// run the Modules (only ones connected to anything)
	list<int> ExecutionOrder = m_Canvas->GetGraph()->GetSortedList();
	for (list<int>::reverse_iterator i=ExecutionOrder.rbegin();
		 i!=ExecutionOrder.rend(); i++)
	{
		// use the graphsort order to remove internal latency
		map<int,DeviceWin*>::iterator di=m_DeviceWinMap.find(*i);
		if (di!=m_DeviceWinMap.end() && di->second->m_Device  && (! di->second->m_Device->IsDead()) && (!m_Info.PAUSED || m_ResetingAudioThread))
		{
			#ifdef DEBUG_MODULES
			cerr<<"Executing Module "<<di->second->m_ModuleID<<endl;
			#endif

			if (m_ResetingAudioThread)
			{
				di->second->m_Device->Reset();
			}
			else
			{
				di->second->m_Device->Execute();

				// If this is an audio device see if we need to ProcessAudio here
				if (di->second->m_Device->IsAudioDriver())
				{
					AudioDriver *driver = ((AudioDriver *)di->second->m_Device);

					if (driver->ProcessType() == AudioDriver::MANUAL)
					{
						driver->ProcessAudio();
					}
				}
			}

			#ifdef DEBUG_MODULES
			cerr<<"Finished executing"<<endl;
			#endif
		}
	}

	//we can safely turn this off here
	m_ResetingAudioThread = false;
}

//////////////////////////////////////////////////////////


void SynthModular::LoadModules (string modulePath) {
     int ID=-1;
     vector<string> ModuleVector;
     if (m_spiralInfo->USEModuleLIST) ModuleVector = m_spiralInfo->ModuleVEC;
     else {
        if (ModulePath.empty()) ModuleVector = BuildModuleList (m_spiralInfo->Module_PATH);
        else {
           string::iterator i = ModulePath.end() - 1;
           if (*i != '/') ModulePath += '/';
           ModuleVector = BuildModuleList (ModulePath);
        }
     }
     for (vector<string>::iterator i=ModuleVector.begin(); i!=ModuleVector.end(); i++) {
         string Fullpath;
         if (ModulePath=="") Fullpath=m_spiralInfo->Module_PATH+*i;
         else Fullpath = ModulePath + *"/" + *i;
         ID = ModuleManager::Get()->LoadModule (Fullpath.c_str());
     }
     map<string,Fl_Pack*>::iterator PlugGrp;
     for (PlugGrp = m_ModuleGroupMap.begin(); PlugGrp!= m_ModuleGroupMap.end(); ++PlugGrp) {
         m_GroupTab->add (PlugGrp->second);
         PlugGrp->second->add (new Fl_Box (0, 0, 600, 100, ""));
     }
     // try to show the SpiralSound group
     PlugGrp = m_ModuleGroupMap.find("SpiralSound");
     // can't find it - show the first Module group
     if (PlugGrp==m_ModuleGroupMap.end()) PlugGrp=m_ModuleGroupMap.begin();
     m_GroupTab->value(PlugGrp->second);
     bool found_dummy;
     int i;
     do {
        found_dummy = false;
        for (i=0; i<m_MainMenu->size(); i++) {
            if (m_MainMenu->text (i) != NULL) {
               found_dummy = (strcmp ("dummy", m_MainMenu->text (i)) == 0);
               if (found_dummy) break;
            }
        }
        if (found_dummy) m_MainMenu->remove (i);
     } while (found_dummy);
     Splash->hide();
     delete Splash;
}

//////////////////////////////////////////////////////////


DeviceWin* SynthModular::NewDeviceWin(int n, int x, int y)
{
	DeviceWin *nlw = new DeviceWin;
	const HostsideInfo* Module=ModuleManager::Get()->GetModule(n);

	if (!Module) return NULL;

	nlw->m_Device=Module->CreateInstance();

	if (!nlw->m_Device) return NULL;

        nlw->m_Device->SetBlockingCallback(cb_Blocking);
	nlw->m_Device->SetUpdateCallback(cb_Update);
	nlw->m_Device->SetParent((void*)this);

	if ( nlw->m_Device->IsAudioDriver() )
	{
		AudioDriver *driver = ((AudioDriver*)nlw->m_Device);
		driver->SetChangeBufferAndSampleRateCallback(cb_ChangeBufferAndSampleRate);
	}

	ModuleInfo PInfo    = nlw->m_Device->Initialise(&m_Info);
	nlw->m_ModuleID     = n;

	if (temp) temp->position(x+10,y);

	return nlw;
}

//////////////////////////////////////////////////////////

void SynthModular::cb_ChangeBufferAndSampleRate_i(long int NewBufferSize, long int NewSamplerate)
{
	if (m_spiralInfo->BUFSIZE != NewBufferSize)
	{
		// update the settings
		m_spiralInfo->BUFSIZE    = NewBufferSize;
		m_HostNeedsUpdate = true;
	}

	if (m_spiralInfo->SAMPLERATE != NewSamplerate)
	{
		m_spiralInfo->SAMPLERATE = NewSamplerate;
		m_HostNeedsUpdate = true;
	}
}


void SynthModular::UpdateHostInfo()
{
	/* Pause Audio */
	FreezeAll();

	/* update the settings */
	m_Info.BUFSIZE    = m_spiralInfo->BUFSIZE;
	m_Info.SAMPLERATE = m_spiralInfo->SAMPLERATE;

	/* obsolete - REMOVE SOON  */
	m_Info.FRAGSIZE   = m_spiralInfo->FRAGSIZE;
	m_Info.FRAGCOUNT  = m_spiralInfo->FRAGCOUNT;
	m_Info.OUTPUTFILE = m_spiralInfo->OUTPUTFILE;
	m_Info.MIDIFILE   = m_spiralInfo->MIDIFILE;
	m_Info.POLY       = m_spiralInfo->POLY;

	/* Reset all Module ports/buffers befure Resuming */
	ResetAudio();
}

//////////////////////////////////////////////////////////

// called when a callback output Module wants to run the audio thread
void SynthModular::cb_Update(void* o, bool mode)
{
	m_CallbackUpdateMode=mode;
	((SynthModular*)o)->Update();
}

// called by a blocking output Module to notify the engine its ready to
// take control of the update timing (so take the brakes off)
void SynthModular::cb_Blocking(void* o, bool mode)
{
	m_BlockingOutputModuleIsReady=mode;
}

//////////////////////////////////////////////////////////

iostream &SynthModular::StreamPatchIn(iostream &s, bool paste, bool merge)
{
	//if we are merging as opposed to loading a new patch
	//we have no need to pause audio
	if (!merge && !paste)
		FreezeAll();

	//if we are pasting we don't have any of the file version
	//or saving information. since its internal we didn't
	//need it, but we do have other things we might need to load

	bool has_file_path;
	char file_path[1024];
	string m_FromFilePath;

	string dummy,dummy2;
	int ver;

	if (paste)
	{
		m_Copied.devices>>has_file_path;

		if (has_file_path)
		{
   		  m_Copied.devices.getline(file_path, 1024);
   		  m_FromFilePath = file_path;
   		  cerr << file_path << endl;
   		}

	}
	else
	{
		s>>dummy>>dummy>>dummy>>ver;

		if (ver>FILE_VERSION)
		{
			m_spiralInfo->Alert("Bad file, or more recent version.");
			ThawAll();
			return s;
		}

		if (ver>2)
		{
			int MainWinX,MainWinY,MainWinW,MainWinH;
			int EditWinX,EditWinY,EditWinW,EditWinH;

			s>>MainWinX>>MainWinY>>MainWinW>>MainWinH;
			s>>EditWinX>>EditWinY>>EditWinW>>EditWinH;

			//o.m_MainWindow->resize(MainWinX,MainWinY,MainWinW,MainWinH);
			//o.m_EditorWindow->resize(EditWinX,EditWinY,EditWinW,EditWinH);
		}

		if (merge)
			m_FromFilePath = m_MergeFilePath;
	}

	//wether pasting or merging we need to clear the current
	//selection so we can replace it with the new devices
	if (paste || merge)
		Fl_Canvas::ClearSelection(m_Canvas);

	int Num, ID, ModuleID, x,y,ps,px,py;

	if (paste)
	{
		Num = m_Copied.devicecount;
	}
	else
	{
		s>>dummy>>Num;
	}

	for(int n=0; n<Num; n++)
	{
		#ifdef DEBUG_STREAM
		cerr<<"Loading Device "<<n<<endl;
		#endif

		s>>dummy; // "Device"
		s>>ID;
		s>>dummy2; // "Module"
		s>>ModuleID;
		s>>x>>y;

		string Name;

		if (paste || ver>3)
		{
			// load the device name
			int size;
			char Buf[1024];
			s>>size;
			s.ignore(1);
			if (size > 0) {
				s.get(Buf,size+1);
				Name=Buf;
			} else {
				Name = "";
			}
		}

		#ifdef DEBUG_STREAM
		cerr<<dummy<<" "<<ID<<" "<<dummy2<<" "<<ModuleID<<" "<<x<<" "<<y<<endl;
		#endif

		if (paste || ver>1) s>>ps>>px>>py;

		//if we are merging a patch or pasting we will change duplicate ID's
		if (!paste && !merge)
		{
			// Check we're not duplicating an ID
			if (m_DeviceWinMap.find(ID)!=m_DeviceWinMap.end())
			{
				m_spiralInfo->Alert("Duplicate device ID found in file - aborting load");
				ThawAll();
				return s;
			}
		}

		if (ModuleID==COMMENT_ID)
		{
			DeviceWin* temp = NewComment(ModuleID, x, y);
			if (temp)
			{
				if (paste || merge)
				{
					m_Copied.m_DeviceIds[ID] = m_NextID++;
					ID = m_Copied.m_DeviceIds[ID];
				}

				temp->m_DeviceGUI->SetID(ID);
				m_DeviceWinMap[ID]=temp;
				((Fl_CommentGUI*)(m_DeviceWinMap[ID]->m_DeviceGUI))->StreamIn(s); // load the Module

				if (paste || merge)
					Fl_Canvas::AppendSelection(ID, m_Canvas);
				else
					if (m_NextID<=ID) m_NextID=ID+1;

			}
		}
		else
		{
			DeviceWin* temp = NewDeviceWin(ModuleID, x, y);
			if (temp)
			{
				int oldID=ID;
				if (paste || merge)
				{
					m_Copied.m_DeviceIds[ID] = m_NextID++;

					ID = m_Copied.m_DeviceIds[ID];
				}

				temp->m_DeviceGUI->SetID(ID);

				if (paste || ver>3)
				{
					// set the titlebars
					temp->m_DeviceGUI->SetName(Name);
				}

				temp->m_Device->SetUpdateInfoCallback(ID,cb_UpdateModuleInfo);
				m_DeviceWinMap[ID]=temp;
				m_DeviceWinMap[ID]->m_Device->StreamIn(s); // load the Module

				// load external files
				if (paste || merge)
					m_DeviceWinMap[ID]->m_Device->LoadExternalFiles(m_FromFilePath+"_files/", oldID);
				else
					m_DeviceWinMap[ID]->m_Device->LoadExternalFiles(m_FilePath+"_files/");

				if ((paste || ver>1) && m_DeviceWinMap[ID]->m_DeviceGUI->GetModuleWindow())
				{
					// set the GUI up with the loaded values
					// looks messy, but if we do it here, the Module and it's gui can remain
					// totally seperated.
					((SpiralModuleGUI*)(m_DeviceWinMap[ID]->m_DeviceGUI->GetModuleWindow()))->
						UpdateValues(m_DeviceWinMap[ID]->m_Device);

					// updates the data in the channel buffers, so the values don't
					// get overwritten in the next tick. (should maybe be somewhere else)
					m_DeviceWinMap[ID]->m_Device->GetChannelHandler()->FlushChannels();

					// position the Module window in the main window
					//m_DeviceWinMap[ID]->m_DeviceGUI->GetModuleWindow()->position(px,py);

					if (ps)
					{
						m_DeviceWinMap[ID]->m_DeviceGUI->Maximise();
						// reposition after maximise
						m_DeviceWinMap[ID]->m_DeviceGUI->position(x,y);
					}
					else m_DeviceWinMap[ID]->m_DeviceGUI->Minimise();

					if (paste || merge)
						Fl_Canvas::AppendSelection(ID, m_Canvas);
				}

				if (!paste && !merge)
					if (m_NextID<=ID) m_NextID=ID+1;
			}
			else
			{
				// can't really recover if the Module ID doesn't match a Module, as
			    // we have no idea how much data in the stream belongs to this Module
				m_spiralInfo->Alert("Error in stream, can't really recover data from here on.");
				return s;
			}
		}
	}

	if (!paste && !merge)
	{
		s>>*m_Canvas;
		ThawAll();
	}

        return s;
}

iostream &operator>>(iostream &s, SynthModular &o)
{
	return o.StreamPatchIn(s, false, false);

}

//////////////////////////////////////////////////////////

ostream &operator<<(ostream &s, SynthModular &o)
{
	o.FreezeAll();

	s<<"SpiralSynthModular File Ver "<<FILE_VERSION<<endl;

	// make external files dir
	bool ExternalDirUsed=false;
	string command("mkdir '"+o.m_FilePath+"_files'");
	system(command.c_str());

	if (FILE_VERSION>2)
	{
		s<<o.m_TopWindow->x()<<" "<<o.m_TopWindow->y()<<" ";
		s<<o.m_TopWindow->w()<<" "<<o.m_TopWindow->h()<<" ";
		s<<0<<" "<<0<<" ";
		s<<0<<" "<<0<<endl;
	}

	// save out the SynthModular
	s<<"SectionList"<<endl;
	s<<o.m_DeviceWinMap.size()<<endl;

	for(map<int,DeviceWin*>::iterator i=o.m_DeviceWinMap.begin();
		i!=o.m_DeviceWinMap.end(); i++)
	{
		if (i->second->m_DeviceGUI && ((i->second->m_Device) || (i->second->m_ModuleID==COMMENT_ID)))
		{
			s<<endl;
			s<<"Device ";
			s<<i->first<<" "; // save the id
			s<<"Module ";
			s<<i->second->m_ModuleID<<endl;
			s<<i->second->m_DeviceGUI->x()<<" ";
			s<<i->second->m_DeviceGUI->y()<<" ";
			s<<i->second->m_DeviceGUI->GetName().size()<<" ";
			s<<i->second->m_DeviceGUI->GetName()<<" ";

			if (i->second->m_DeviceGUI->GetModuleWindow())
			{
				s<<i->second->m_DeviceGUI->GetModuleWindow()->visible()<<" ";
				s<<i->second->m_DeviceGUI->GetModuleWindow()->x()<<" ";
				s<<i->second->m_DeviceGUI->GetModuleWindow()->y()<<" ";
			}
			else
			{
				s<<0<<" "<<0<<" "<<0;
			}

			s<<endl;

			if (i->second->m_ModuleID==COMMENT_ID)
			{
				// save the comment gui
				((Fl_CommentGUI*)(i->second->m_DeviceGUI))->StreamOut(s);
			}
			else
			{
				// save the Module
				i->second->m_Device->StreamOut(s);
			}
			s<<endl;

			// save external files
			if (i->second->m_Device && i->second->m_Device->SaveExternalFiles(o.m_FilePath+"_files/"))
			{
				ExternalDirUsed=true;
			}
		}
	}

	s<<endl<<*o.m_Canvas<<endl;

	// remove it if it wasn't used
	if (!ExternalDirUsed)
	{
		// i guess rmdir won't work if there is something in the dir
		// anyway, but best to be on the safe side. (could do rm -rf) :)
		string command("rmdir "+o.m_FilePath+"_files");
		system(command.c_str());
	}

	o.ThawAll();

	return s;
}

//////////////////////////////////////////////////////////////////////////////////////////

// Callbacks

/////////////////////////////////
// File Menu & associated buttons

// New

inline void SynthModular::cb_New_i (Fl_Widget *o, void *v) {
       if (m_DeviceWinMap.size()>0 && !Pawfal_YesNo ("New - Lose changes to current patch?"))
          return;
       m_TopWindow->label (TITLEBAR.c_str());
       ClearUp();
}

void SynthModular::cb_New (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_New_i (o, v);
}

// Load

inline void SynthModular::cb_Load_i (Fl_Widget *o, void *v) {
       if (m_DeviceWinMap.size()>0 && !Pawfal_YesNo ("Load - Lose changes to current patch?"))
          return;
       char *fn=fl_file_chooser ("Load a patch", "*.ssm", NULL);
       if (fn && fn!='\0') {
          ifstream in (fn);
          if (in) {
             fstream inf;
             inf.open (fn, ios::in);
             m_FilePath = fn;
             ClearUp();
             inf >> *this;
             inf.close();
             TITLEBAR = LABEL + " " + fn;
             m_TopWindow->label (TITLEBAR.c_str());
          }
       }
}

void SynthModular::cb_Load(Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_Load_i (o, v);
}

// Save

inline void SynthModular::cb_Save_i (Fl_Widget *o, void *v) {
       char *fn=fl_file_chooser("Save a patch", "*.ssm", NULL);
       if (fn && fn!='\0') {
          ifstream ifl (fn);
          if (ifl) {
             if (!Pawfal_YesNo ("File [%s] exists, overwrite?", fn))
                return;
          }
          ofstream of (fn);
          if (of) {
             m_FilePath = fn;
             of << *this;
             TITLEBAR = LABEL + " " + fn;
             m_TopWindow->label (TITLEBAR.c_str());
          }
          else {
              fl_message ( "%s", string ("Error saving " + string(fn)).c_str());
          }
       }
}

void SynthModular::cb_Save (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_Save_i (o, v);
}

// Merge

inline void SynthModular::cb_Merge_i (Fl_Widget *o, void *v) {
       char *fn = fl_file_chooser ("Merge a patch", "*.ssm", NULL);
       if (fn && fn!='\0') {
          ifstream in (fn);
          if (in) {
             fstream inf;
             inf.open (fn, ios::in);
             m_MergeFilePath = fn;
             StreamPatchIn (inf, false, true);
             m_Canvas->StreamSelectionWiresIn (inf, m_Copied.m_DeviceIds, true, false);
             inf.close();
          }
       }
}

void SynthModular::cb_Merge (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->parent()->user_data()))->cb_Merge_i (o, v);
}

// Close

inline void SynthModular::cb_Close_i (Fl_Widget *o, void *v) {
       m_SettingsWindow->hide();
       delete m_SettingsWindow;
       m_TopWindow->hide();
       delete m_TopWindow;
}

void SynthModular::cb_Close (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_Close_i (o, v);
}

/////////////////////////////////
// Edit Menu

// Cut

inline void SynthModular::cb_Cut_i(Fl_Widget *o, void *v) {
       if (! m_Canvas->HaveSelection()) return;
       // show some warning here
       cb_Copy_i (o, v);  // should we be calling an inline function here??????
       for (unsigned int i=0; i<m_Canvas->Selection().m_DeviceIds.size(); i++) {
           int ID = m_Canvas->Selection().m_DeviceIds[i];
           Fl_DeviceGUI::Kill(m_DeviceWinMap[ID]->m_DeviceGUI);
       }
       Fl_Canvas::ClearSelection(m_Canvas);
}

void SynthModular::cb_Cut (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->parent()->user_data()))->cb_Cut_i (o, v);
}

// Copy

inline void SynthModular::cb_Copy_i (Fl_Widget *o, void *v) {
       if (! m_Canvas->HaveSelection()) return;
       m_Copied.devices.open ("___temp.ssmcopytmp", ios::out);
       m_Copied.devicecount = 0;
       m_Copied.m_DeviceIds.clear();
       if (m_FilePath != "") {
           m_Copied.devices << true << " "  << m_FilePath << endl;
       }
       else m_Copied.devices << false << endl;
       for (unsigned int i=0; i<m_Canvas->Selection().m_DeviceIds.size(); i++) {
           int ID = m_Canvas->Selection().m_DeviceIds[i];
           std::map<int,DeviceWin*>::iterator j = m_DeviceWinMap.find(ID);
           m_Copied.m_DeviceIds[ID] = ID;
           m_Copied.devicecount += 1;
           m_Copied.devices << "Device " << j->first << " " ; // save the id
           m_Copied.devices << "Module " <<j->second->m_ModuleID << endl;
           m_Copied.devices << j->second->m_DeviceGUI->x() << " ";
           m_Copied.devices << j->second->m_DeviceGUI->y() << " ";
           m_Copied.devices << j->second->m_DeviceGUI->GetName().size() << " ";
           m_Copied.devices << j->second->m_DeviceGUI->GetName() << " ";
           if (j->second->m_DeviceGUI->GetModuleWindow()) {
              m_Copied.devices << j->second->m_DeviceGUI->GetModuleWindow()->visible() << " ";
              m_Copied.devices << j->second->m_DeviceGUI->GetModuleWindow()->x() << " ";
              m_Copied.devices << j->second->m_DeviceGUI->GetModuleWindow()->y() << " ";
           }
           else m_Copied.devices << 0 << " " << 0 << " " << 0;
           m_Copied.devices << endl;
           if (j->second->m_ModuleID == COMMENT_ID) {
              // save the comment gui
              ((Fl_CommentGUI*)(j->second->m_DeviceGUI))->StreamOut (m_Copied.devices);
           }
           else {
              // save the Module
              j->second->m_Device->StreamOut (m_Copied.devices);
           }
           m_Copied.devices<<endl;
       }
       m_Canvas->StreamSelectionWiresOut(m_Copied.devices);
       m_Copied.devices.close();
       Fl_Canvas::EnablePaste (m_Canvas);
}

void SynthModular::cb_Copy (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->parent()->user_data()))->cb_Copy_i (o, v);
}

// Paste

inline void SynthModular::cb_Paste_i (Fl_Widget *o, void *v) {
       if (m_Copied.devicecount <= 0) return;
       m_Copied.devices.open ("___temp.ssmcopytmp", ios::in);
       StreamPatchIn(m_Copied.devices, true, false);
       m_Canvas->StreamSelectionWiresIn (m_Copied.devices, m_Copied.m_DeviceIds, false, true);
       m_Copied.devices.close();
}

void SynthModular::cb_Paste (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->parent()->user_data()))->cb_Paste_i (o, v);
}

// Delete

inline void SynthModular::cb_Delete_i (Fl_Widget *o, void *v) {
       m_Canvas->DeleteSelection();
}

void SynthModular::cb_Delete (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_Delete_i (o, v);
}

// Options

inline void SynthModular::cb_Options_i (Fl_Widget *o, void *v) {
       m_SettingsWindow->show();
}

void SynthModular::cb_Options (Fl_Widget* o, void* v) {
     ((SynthModular*)(o->user_data()))->cb_Options_i (o, v);
}

/////////////////////////////////
// Module Menu

// This callback has the name that the callback for the canvas menu
// used to have please note - that is now NewDeviceFromCanvasMenu

inline void SynthModular::cb_NewDeviceFromMenu_i (Fl_Widget *o, void *v) {
       AddDevice (*((int*)v));
}

void SynthModular::cb_NewDeviceFromMenu (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_NewDeviceFromMenu_i (o, v);
}

// (Module Buttons)

inline void SynthModular::cb_NewDevice_i (Fl_Button *o, void *v) {
       AddDevice (*((int*)v));
}

void SynthModular::cb_NewDevice (Fl_Button *o, void *v) {
     ((SynthModular*)(o->parent()->user_data()))->cb_NewDevice_i (o, v);
}

// (Module Canvas Menu)

inline void SynthModular::cb_NewDeviceFromCanvasMenu_i (Fl_Canvas* o, void* v) {
       AddDevice(*((int*)v),*((int*)v+1),*((int*)v+2));
}

void SynthModular::cb_NewDeviceFromCanvasMenu(Fl_Canvas* o, void* v) {
     ((SynthModular*)(o->user_data()))->cb_NewDeviceFromCanvasMenu_i(o,v);
}


/////////////////////////////////
// Audio Menu

// Play / Pause

inline void SynthModular::cb_PlayPause_i (Fl_Widget *o, void *v) {
       string oldname = m_PlayPause->tooltip ();
       if (m_Info.PAUSED) {
          m_PlayPause->label ("@||");
          m_PlayPause->tooltip ("Pause");
          ResumeAudio();
       }
       else {
          m_PlayPause->label ("@>");
          m_PlayPause->tooltip ("Play");
          PauseAudio();
       }
       for (int i=0; i<m_MainMenu->size(); i++) {
            if (m_MainMenu->text (i) != NULL) {
               if (oldname == m_MainMenu->text (i)) {
                  m_MainMenu->replace (i, m_PlayPause->tooltip());
                  break;
               }
            }
        }

}

void SynthModular::cb_PlayPause (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_PlayPause_i (o, v);
}

// Reset

inline void SynthModular::cb_Reset_i (Fl_Widget *o, void *v) {
       ResetAudio();
}

void SynthModular::cb_Reset (Fl_Widget *o, void *v) {
     ((SynthModular*)(o->user_data()))->cb_Reset_i (o, v);
}

//////////////////////////////////////////////////////////

inline void SynthModular::cb_NewComment_i(Fl_Button* o, void* v)
{
	AddComment(-1);
}
void SynthModular::cb_NewComment(Fl_Button* o, void* v)
{((SynthModular*)(o->parent()->user_data()))->cb_NewComment_i(o,v);}

//////////////////////////////////////////////////////////

inline void SynthModular::cb_GroupTab_i(Fl_Tabs* o, void* v)
{
        m_GroupTab->redraw();
}

void SynthModular::cb_GroupTab(Fl_Tabs* o, void* v)
{((SynthModular*)(o->parent()->user_data()))->cb_GroupTab_i(o,v);}

//////////////////////////////////////////////////////////

inline void SynthModular::cb_Connection_i(Fl_Canvas* o, void* v)
{
	CanvasWire *Wire;
	Wire=(CanvasWire*)v;

	map<int,DeviceWin*>::iterator si=m_DeviceWinMap.find(Wire->OutputID);
	if (si==m_DeviceWinMap.end())
	{
		char num[32];
		sprintf(num,"%d",Wire->OutputID);
		m_spiralInfo->Alert("Warning: Connection problem - can't find source "+string(num));
		return;
	}

	map<int,DeviceWin*>::iterator di=m_DeviceWinMap.find(Wire->InputID);
	if (di==m_DeviceWinMap.end())
	{
		char num[32];
		sprintf(num,"%d",Wire->InputID);
		m_spiralInfo->Alert("Warning: Connection problem - can't find destination "+string(num));
		return;
	}

	Sample *sample=NULL;

	if (!si->second->m_Device->GetOutput(Wire->OutputPort,&sample))
	{
		char num[32];
		sprintf(num,"%d,%d",Wire->OutputID,Wire->OutputPort);
		m_spiralInfo->Alert("Warning: Connection problem - can't find source output "+string(num));
		return;
	}

	if (!di->second->m_Device->SetInput(Wire->InputPort,(const Sample*)sample))
	{
		char num[32];
		sprintf(num,"%d,%d",Wire->InputID,Wire->InputPort);
		m_spiralInfo->Alert("Warning: Connection problem - can't find source input "+string(num));
		return;
	}
}
void SynthModular::cb_Connection(Fl_Canvas* o, void* v)
{((SynthModular*)(o->user_data()))->cb_Connection_i(o,v);}

//////////////////////////////////////////////////////////

inline void SynthModular::cb_Unconnect_i(Fl_Canvas* o, void* v)
{
	CanvasWire *Wire;
	Wire=(CanvasWire*)v;

	//cerr<<Wire->InputID<<" "<<Wire->InputPort<<endl;

	map<int,DeviceWin*>::iterator di=m_DeviceWinMap.find(Wire->InputID);
	if (di==m_DeviceWinMap.end())
	{
		//cerr<<"Can't find destination device "<<Wire->InputID<<endl;
		return;
	}

	SpiralModule *Module=di->second->m_Device;
	if (Module && !Module->SetInput(Wire->InputPort,NULL))
	{ cerr<<"Can't find destination device's Input"<<endl; return;	}
}
void SynthModular::cb_Unconnect(Fl_Canvas* o, void* v)
{((SynthModular*)(o->user_data()))->cb_Unconnect_i(o,v);}

//////////////////////////////////////////////////////////

void SynthModular::cb_UpdateModuleInfo(int ID, void *PInfo)
{
	map<int,DeviceWin*>::iterator i=m_DeviceWinMap.find(ID);
	if (i!=m_DeviceWinMap.end())
	{
		DeviceGUIInfo Info=BuildDeviceGUIInfo(*((ModuleInfo*)PInfo));

		(*i).second->m_DeviceGUI->Setup(Info);
		(*i).second->m_DeviceGUI->redraw();
	}
}

//////////////////////////////////////////////////////////

void SynthModular::LoadPatch(const char *fn)
{
	ifstream in(fn);

	if (in)
	{
		fstream	inf;
		inf.open(fn, std::ios::in);

		m_FilePath=fn;

		ClearUp();
		inf>>*this;

		inf.close();

		TITLEBAR=LABEL+" "+fn;
		m_TopWindow->label(TITLEBAR.c_str());
	}
}

