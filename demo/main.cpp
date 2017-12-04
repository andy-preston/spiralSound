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
#include "../SpiralSound/Modules/OscillatorModule/OscillatorModule.h"
#include "../SpiralSound/Modules/OutputModule/OutputModule.h"

int main(int argc, char **argv)
{
    SpiralInfo *info;
    Synth *synth;
    OscillatorModule *oscillator;
    OutputModule *output;

    srand(time(NULL));
    info = new SpiralInfo();
    info->OUTPUTFILE = "/dev/dsp";

    cout << "create synth\n";
	synth = new Synth(info);

    cout << "add osc\n";
    oscillator = new OscillatorModule(info);
    synth->addModule(oscillator);

    cout << "add output\n";
    output = new OutputModule(info);
    synth->addModule(output);

    cout << "connect\n";
    synth->connect(oscillator, "Output", output, "Left Out");
    synth->connect(oscillator, "Output", output, "Right Out");

    cout << "run\n";
    synth->run();

	return 1;
}