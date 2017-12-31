#!/bin/bash

cd $1

rmdir opt var/www

cd etc

cat fstab | grep -v -e "/dev/root" > plop
mv plop fstab
echo -e "/dev/root\t/\text2\tro,noauto,noatime\t0\t1" >> fstab

for v in passwd shadow group
do
    cat $v | grep -v -e lp -e mail -e cdrom -e dialout -e floppy -e video -e tape -e www > plop
    mv plop $v
done

sed -e '/profile.d/,$d' -i profile
if [ -e profile.d/umask.sh ]
then
    cat profile.d/umask.sh >> profile
fi

cat inittab | grep -v -e tty1:: -e init.d/ -e "remount,rw" -e "::sysinit:/etc/rc.local" > plop
mv plop inittab
echo "::sysinit:/etc/rc.local" >> inittab

echo "/sbin/sysctl -w vm.swappiness=0" > rc.local
echo "echo performance > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor" >> rc.local
echo "echo performance > /sys/devices/system/cpu/cpu1/cpufreq/scaling_governor" >> rc.local
echo "echo performance > /sys/devices/system/cpu/cpu2/cpufreq/scaling_governor" >> rc.local
echo "echo performance > /sys/devices/system/cpu/cpu3/cpufreq/scaling_governor" >> rc.local

echo -e "pcm.!default {\n type hw card 0\n}\n" > asound.conf
echo -e "ctl.!default {\n type hw card 0\n}\n" >> asound.conf

for v in init.d network protocols services profile.d
do
    if [ -e $v ]
    then
        rm -rf $v
    fi
done

