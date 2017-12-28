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
#include "../SpiralSound/Modules/EnvelopeModule/EnvelopeModule.h"
#include "../SpiralSound/Modules/LFOModule/LFOModule.h"
#include "../SpiralSound/Modules/OutputModule/OutputModule.h"

int main(int argc, char **argv)
{
    SpiralInfo *info;
    Synth *synth;
    MidiModule *midi;
    OscillatorModule *oscillator;
    FilterModule *filter;
    LFOModule *lfo1, *lfo2;
    EnvelopeModule *envelope;
    OutputModule *output;

    srand(time(NULL));
    info = new SpiralInfo();

    cerr << "create synth\n";
	synth = new Synth(info);

    cerr << "add MIDI" << endl;
    midi = new MidiModule(info, "SpiralSound");
    synth->addModule(midi);

    cerr << "add osc" << endl;
    oscillator = new OscillatorModule(info);
    synth->addModule(oscillator);

    cerr << "add filter" << endl;
    filter = new FilterModule(info);
    synth->addModule(filter);

    cerr << "add LFO 1" << endl;
    lfo1 = new LFOModule(info);
    lfo1->m_Freq = 1;
    synth->addModule(lfo1);

    cerr << "add LFO 2" << endl;
    lfo2 = new LFOModule(info);
    synth->addModule(lfo2);

    cerr << "add envelope" << endl;
    envelope = new EnvelopeModule(info);
    synth->addModule(envelope);

    cerr << "add output" << endl;
    output = new OutputModule(info);
    synth->addModule(output);

    cerr << "connect MIDI -> osc" << endl;
    synth->connect(midi, "Note", oscillator, "Frequency");

    cerr << "connect osc -> filter" << endl;
    synth->connect(oscillator, "Output", filter, "Input");

    cerr << "connect LFOs -> filter" << endl;
    synth->connect(lfo1, "Output", filter, "Cutoff");
    synth->connect(lfo2, "Output", filter, "Resonance");

    cerr << "connect filter -> envelope" << endl;
    synth->connect(filter, "Output", envelope, "Input");

    cerr << "connect MIDI -> envelope" << endl;
    synth->connect(midi, "Trigger", envelope, "Trigger");

    cerr << "connect envelope -> output" << endl;
    synth->connect(envelope, "Output", output, "Left Out");
    synth->connect(envelope, "Output", output, "Right Out");

    cerr << "run" << endl;
    synth->run();

	return 1;
}