SpiralSound
===========

Last changed 15 October 2016 (Last time I was typing here it was 11-March-2004)

SpiralSound is open source software, distributed under the General
Public License (GPL). See the file COPYING.

See https://github.com/edgeeffect/spiralSound for more details.

SpiralSound is the same codebase as good old SpiralSynthModular reduced down
to a library to achieve the same results programatically rather than using a
GUI.

Where SSM was a straight software modular synth SpiralSound is more of a kit
for building your own soft-synths.

Warning
-------

Whilst I'm doing the basic reworking of SSM into SpiralSound - there's a lot
of GIT history rewriting going on. If you check this code out don't be surprised
if when you come to do a PULL everything goes kerpooie!


Dependancies
------------

LADSPA plugins  (www.ladspa.org)

Sound output can be provided by either OSS or JACK (and therefore alsa).

Installing
----------

(With such a massive rewrite in progress, your chances of this working are
minimal at best)

To build SpiralSound:

./configure
make
make install
spiralsynthmodular

LRDF support for LADSPA plugins
-------------------------------

http://plugin.org.uk/releases/lrdf/
http://www.redland.opensource.ac.uk/raptor/

If you have the lrdf library installed on your system,
the LADSPAPlugin will automatically use it.

To disable detection of this this, use the configure option:
./configure --disable-liblrdf

Jack support
------------

http://jackit.sourceforge.net

If you have jack installed on your system, you can use it via the
JackModule.

To disable detection of jack, use the configure option:
./configure --disable-jack

LibSndFile Support
------------------

http://www.zip.com.au/~erikd/libsndfile/

If you have libsndfile installed on you system; PoshSampler, Streamer,
and SpiralLoops load any audio format/file that libsndfile can.

To disable detection of this, use the configure option:
./configure --disable-sndfile


Setting up your .spiralmodular file
-----------------------------------

This file contains all the setup variables for the program. Thes values can be
edited with the options window whilst running SSM.
The following variables in the are the ones to play with to get a good playback
on your machine. This is usually tradeoff between quality and responsiveness
(latency). You can build designs and songs at low quality (22050 samplerate etc)
and then record them at higher settings. The wav file will sound fine, even if
the realtime output doesn't.

BufferSize        = 512         - Size of sample buffer to be calculated each cycle.
FragmentSize      = 256         - Subbuffer size sent to the soundcard
FragmentCount     = 8           - Subbuffer count, -1 computes a suitable size,
                                  setting to 4 or 8 gets better latencies.
Samplerate        = 44100       - Sets the samplerate


Disclaimer
----------

No representations are made about the suitability of this software
for any purpose. This software is provided "as is" without express
or implied warranty.
(Damage to ears, speakers or buildings are not my fault :))

Andy Preston (Maintainer) - edgeeffect@gmail.com
Dave Griffiths (Original Author) - dave@pawfal.org
