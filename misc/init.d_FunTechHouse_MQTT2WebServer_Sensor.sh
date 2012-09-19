#! /bin/sh

### BEGIN INIT INFO
# Provides:		FunTechHouse_MQTT2WebServer_Sensor
# Required-Start:	$remote_fs $syslog
# Required-Stop:	$remote_fs $syslog
# Default-Start:	2 3 4 5
# Default-Stop:		
# Short-Description:	FunTechHouse_MQTT2WebServer_Sensor
### END INIT INFO

M2W=/usr/local/bin/FunTechHouse_MQTT2WebServer_Sensor
M2W_PID=/var/run/m2w.pid
M2W_CFG=/etc/FunTech_M2W_config.xml

M2W_DESC="FunTechHouse_MQTT2WebServer_Sensor"
M2W_NAME=M2W_Sensor

set -e

# /etc/init.d/mosquitto: start and stop the mosquitto MQTT message broker

test -x $M2W || exit 0
#type $M2W 2>/dev/null >/dev/null || exit 1

if [ ! -f "$M2W_CFG" ]
then
    echo "Error: No config file found"
    exit 2
fi

umask 022

. /lib/lsb/init-functions

# Are we running from init?
run_by_init() {
    ([ "$previous" ] && [ "$runlevel" ]) || [ "$runlevel" = S ]
}

export PATH="${PATH:+$PATH:}/usr/local/bin:/sbin"

case "$1" in
  start)
	log_daemon_msg "Starting $M2W_DESC" "$M2W_NAME"
	if start-stop-daemon --start --quiet --oknodo --make-pidfile --pidfile $M2W_PID --background --exec $M2W -- -c $M2W_CFG ; then
	    log_end_msg 0
	else
	    log_end_msg 1
	fi
	;;
  stop)
	log_daemon_msg "Stopping $M2W_DESC" "$M2W_NAME"
	if start-stop-daemon --stop --quiet --oknodo --pidfile $M2W_PID; then
	    log_end_msg 0
	else
	    log_end_msg 1
	fi
	;;

  restart)
	log_daemon_msg "Restarting $M2W_DESC" "$M2W_NAME"
	start-stop-daemon --stop --quiet --oknodo --retry 30 --pidfile $M2W_PID
	#check_for_no_start log_end_msg
	#check_dev_null log_end_msg
	if start-stop-daemon --start --quiet --oknodo --pidfile $M2W_PID --background --exec $M2W -- -c $M2W_CFG ; then
	    log_end_msg 0
	else
	    log_end_msg 1
	fi
	;;

  try-restart)
	log_daemon_msg "Restarting $M2W_DESC" "$M2W_NAME"
	set +e
	start-stop-daemon --stop --quiet --retry 30 --pidfile $M2W_PID
	RET="$?"
	set -e
	case $RET in
	    0)
		# old daemon stopped
		check_for_no_start log_end_msg
		check_dev_null log_end_msg
		if start-stop-daemon --start --quiet --oknodo --pidfile $M2W_PID --exec $M2W -- -c $M2W_CFG ; then
		    log_end_msg 0
		else
		    log_end_msg 1
		fi
		;;
	    1)
		# daemon not running
		log_progress_msg "(not running)"
		log_end_msg 0
		;;
	    *)
		# failed to stop
		log_progress_msg "(failed to stop)"
		log_end_msg 1
		;;
	esac
	;;

  status)
	status_of_proc -p $M2W_PID $M2W $M2W_NAME && exit 0 || exit $?
	;;

  *)
	log_action_msg "Usage: /etc/init.d/$M2W_NAME {start|stop|restart|try-restart|status}"
	exit 1
esac

exit 0

