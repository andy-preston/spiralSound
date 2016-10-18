/*  SpiralSound
 *  Copyleft (C) 2002 David Griffiths <dave@pawfal.org>
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

#include <iostream>
#include <vector>
#include "Modules/SpiralModule.h"

struct HostsideInfo
{
	void* Handle;
	int ID;
	SpiralModule *(*CreateInstance)(void);
	int (*GetID)(void);
};

//////////////////////////////////////////////////////////

typedef int ModuleID;
#define ModuleError -1

class ModuleManager
{
    public:
        static ModuleManager *Get()
        {
            if (!m_Singleton) {
                m_Singleton = new ModuleManager;
            }
            return m_Singleton;
        }
        static void PackUpAndGoHome()
        {
            if (m_Singleton) {
                delete m_Singleton;
            }
        }
        ModuleID LoadModule(const char *ModuleName);
        void UnLoadModule(ModuleID ID);
        void UnloadAll();
        const HostsideInfo* GetModule(ModuleID ID);
        bool IsValid(ModuleID ID);
    private:
        ModuleManager();
        ~ModuleManager();
        HostsideInfo *GetModule_i(ModuleID ID);
        vector<HostsideInfo*> m_ModuleVec;
        static ModuleManager *m_Singleton;
};