/*  SpiralPlugin
 *  Copyleft (C) 2000 David Griffiths <dave@pawfal.org>
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

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Slider.H>

#include "DelayPlugin.h"
#include "../SpiralPluginGUI.h"
#include "../Widgets/Fl_Knob.H"

#ifndef FilterGUI
#define FilterGUI


class DelayPluginGUI : public SpiralPluginGUI
{
public:
	DelayPluginGUI(int w, int h, DelayPlugin *o,ChannelHandler *ch,const HostInfo *Info);
	
	virtual void UpdateValues(SpiralPlugin *o);
		
private:
	
	Fl_Slider 		*m_Delay;
	Fl_Knob 		*m_Mix;
	
	//// Callbacks ////
	
	inline void cb_Delay_i(Fl_Slider* o, void* v);
    static void cb_Delay(Fl_Slider*, void*);
	inline void cb_Mix_i(Fl_Knob* o, void* v);
	static void cb_Mix(Fl_Knob* o, void* v); 

};

#endif
