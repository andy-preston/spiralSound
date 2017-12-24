/*  SpiralSound (c) Copyleft 2017 Andy Preston <edgeeffect@gmail.com>
 *  based on SpiralSynth Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
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

#ifndef SPIRALSOUND
#define SPIRALSOUND

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <list>
#include "SpiralInfo.h"
#include "Modules/SpiralModule.h"

class Synth
{
    public:
	    Synth(SpiralInfo *info);
	    ~Synth();
        void addModule(SpiralModule* module);
        void run();
        // TODO: rename this to updateModulesNow
        void UpdateDataNow();
        void UpdateHostInfo();
        bool IsBlockingOutputModuleReady() {
            return m_BlockingOutputModuleIsReady;
        }
        void connect(SpiralModule* sourceMod, string sourcePort,
            SpiralModule* destMod, string destPort);
    private:
        SpiralInfo *spiralInfo;
        std::list<SpiralModule*> moduleList;
        static bool m_BlockingOutputModuleIsReady;
};

#endif