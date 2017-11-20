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

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "SpiralInfo.h"

//#define DEBUG_CONFIG

float RandFloat (float s, float e) {
      return s + ((rand() % 10000 / 10000.0) * (e - s));
}

SpiralInfo* SpiralInfo::m_SpiralInfo = NULL;

SpiralInfo* SpiralInfo::Get() {
   if (!m_SpiralInfo) {
      m_SpiralInfo = new SpiralInfo;
   }
   return m_SpiralInfo;
}

SpiralInfo::SpiralInfo()
{
    BUFSIZE = 512;

    /* obsolete - REMOVE SOON  */
    FRAGSIZE = 256;
    FRAGCOUNT = 8;

    SAMPLERATE = 44100;
    MAXSAMPLE = 32767;
    VALUECONV = 1.0f / MAXSAMPLE;
    FILTERGRAN = 50;
}