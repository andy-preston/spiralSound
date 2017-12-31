#!/bin/bash

# disable build in audio
sed -e "/^dtparam=audio=on/# dtparam=audio=on/g" -i "${BINARIES_DIR}/rpi-firmware/config.txt"

# enable iQaudio DAC+

grep -i -v -e iqaudio -e i2s-mmap "${BINARIES_DIR}/rpi-firmware/config.txt" > plop
mv plop "${BINARIES_DIR}/rpi-firmware/config.txt"
echo "# enable iQaudio DAC+" >> "${BINARIES_DIR}/rpi-firmware/config.txt"
echo "dtoverlay=i2s-mmap" >> "${BINARIES_DIR}/rpi-firmware/config.txt"
echo "dtoverlay=iqaudio-dacplus" >> "${BINARIES_DIR}/rpi-firmware/config.txt"
