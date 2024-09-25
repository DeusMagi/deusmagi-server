#!/bin/sh
#
# This is the Atrinik loop and log script

CMDNAME="./atrinik-server"
maxrestart=500
logdir="data/log"

logcount=0
date=`/bin/date +%y%m%d_%H-%M-%S_%Z`

if [ -d $logdir ]; then
	tar -cvzf "logs_$date.tar.gz" $logdir > /dev/null
fi

rm -rf $logdir
mkdir $logdir
python ../tools/collect.py --out lib/

while [ ! "$logcount"x = "$maxrestart"x ]; do
	echo "Starting Atrinik `date` for the $logcount time..." 1>$logdir/$logcount 2>&1
	$CMDNAME -d 2>>$logdir/$logcount 1>&2

	if [ -f core ]; then
		mv core core.$logcount
		/bin/gzip core.$logcount
	fi

	logcount=`expr $logcount + 1`
	sleep 5
done
