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

#include "../SpiralPlugin.h"
#include "../../RiffWav.h"
#include <FL/Fl_Pixmap.H>

#ifndef StreamPLUGIN
#define StreamPLUGIN

class StreamPlugin : public SpiralPlugin {
   public:
      StreamPlugin();
      virtual ~StreamPlugin();
      virtual PluginInfo &Initialise (const HostInfo *Host);
      virtual SpiralGUIType *CreateGUI();
      virtual void Execute();
      virtual void ExecuteCommands();
      virtual void StreamOut (ostream &s);
      virtual void StreamIn (istream &s);
      enum GUICommands { NONE, SET_TIME, LOAD, RESTART, STOP, PLAY };
      // has to be defined in the plugin
      virtual void UpdateGUI() { Fl::check(); }
      float GetVolume (void) { return m_GUIArgs.Volume; }
      float GetPitch (void) { return m_GUIArgs.PitchMod; }
   private:
      struct GUIArgs {
	float  Volume;
	float  PitchMod;
	char FileName[256];
	float Time;
	float TimeOut;
	float MaxTime;
      };
      GUIArgs m_GUIArgs;
      WavFile m_File;
      Sample m_SampleL, m_SampleR;
      int m_SampleRate, m_SampleSize, m_StreamPos;
      float m_GlobalPos, m_Pitch, m_SamplePos, m_Pos;
      enum Mode { PLAYM, STOPM };
      Mode  m_Mode;
      // Internal functions
      float GetTime (void) { return m_GlobalPos / (float)m_SampleRate; }
      float GetLength (void);
      // Commands
      void SetTime (void);
      void OpenStream (void);
      void Restart (void) { m_StreamPos = 0; m_GlobalPos = 0; }
      void Stop (void) { m_Mode = STOPM; }
      void Play (void) { m_Mode = PLAYM; }
};

#endif
