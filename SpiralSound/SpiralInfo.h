/*  SpiralSound
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

#ifndef SpiralINFO
#define SpiralINFO

#include <vector>
#include <stdlib.h>
#include "Sample.h"

using namespace std;

// todo: better place for these util funcs
float RandFloat (float s = 0.0f, float e = 1.0f);

// Loads info from the resource file

class SpiralInfo {
    public:
        SpiralInfo();
        ~SpiralInfo() {}
        int BUFSIZE;
        int FRAGSIZE;
        int FRAGCOUNT;
        int SAMPLERATE;
        long MAXSAMPLE;
        float VALUECONV;
        int FILTERGRAN;
        static SpiralInfo* Get();
    private:
        static SpiralInfo *m_SpiralInfo;
};

#endif