/*
 * SpiralSound demo synth
 *     - Copyleft (C) 2017 Andy Preston <edgeeffect@gmail.com>
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

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "../SpiralSound/Synth.h"
#include "../SpiralSound/SpiralInfo.h"
#include "../SpiralSound/Modules/MidiModule/MidiModule.h"
#include "../SpiralSound/Modules/OscillatorModule/OscillatorModule.h"
#include "../SpiralSound/Modules/FilterModule/FilterModule.h"
#include "../SpiralSound/Modules/LFOModule/LFOModule.h"
#include "../SpiralSound/Modules/OutputModule/OutputModule.h"

int main(int argc, char **argv)
{
    SpiralInfo *info;
    Synth *synth;
    OscillatorModule *oscillator;
    FilterModule *filter;
    LFOModule *lfo1, *lfo2;
    OutputModule *output;

    srand(time(NULL));
    info = new SpiralInfo();

    cout << "create synth\n";
	synth = new Synth(info);

    cout << "add osc\n";
    oscillator = new OscillatorModule(info);
    synth->addModule(oscillator);

    cout << "add filter\n";
    filter = new FilterModule(info);
    synth->addModule(filter);

    cout << "add LFO 1\n";
    lfo1 = new LFOModule(info);
    lfo1->m_Freq = 1;
    synth->addModule(lfo1);

    cout << "add LFO 2\n";
    lfo2 = new LFOModule(info);
    synth->addModule(lfo2);

    cout << "add output\n";
    output = new OutputModule(info);
    synth->addModule(output);

    cout << "connect\n";
    synth->connect(oscillator, "Output", filter, "Input");
    synth->connect(lfo1, "Output", filter, "Cutoff");
    synth->connect(lfo2, "Output", filter, "Resonance");
    synth->connect(filter, "Output", output, "Left Out");
    synth->connect(filter, "Output", output, "Right Out");

    cout << "run\n";
    synth->run();

	return 1;
}