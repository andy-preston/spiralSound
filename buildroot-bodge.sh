#!/bin/bash

cd $1

rmdir opt var/www

cd etc

cat fstab | sed -e 's/rw,noauto/ro,noauto/g' > plop
mv plop fstab

for v in passwd shadow group
do
    cat $v | grep -v -e lp -e mail -e cdrom -e dialout -e floppy -e video -e tape -e www > plop
    mv plop $v
done

cat inittab | grep -v -e tty1:: -e init.d/ -e "remount,rw" > plop
mv plop inittab

for v in init.d network protocols services
do
    if [ -e $v ]
    then
        rm -rf $v
    fi
done

