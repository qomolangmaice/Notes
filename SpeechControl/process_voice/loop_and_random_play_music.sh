#!/bin/bash
# Usage:
# ./loop_and_random_play_music.sh start | stop | restart
# Author: kevin

state=/media/kevin/SeagateBackup1/Music/state
list=/media/kevin/SeagateBackup1/Music/list

[ -z "$1" ] && echo "./loop_and_random_play_music.sh [start | stop | restart]" && exit 1

arg="$1"
case "$arg" in
    start)
        if [ -f "$state" ]; then
            chmod 777 "$state"
            pid="$(awk '{print $0}' "$state")"
            if [ -z "$(ps -ef | grep "$pid" | grep "mplayer" | grep -v "grep")" ]; then
		#grep -v
                rm -f "$state"
            else
                echo "Music are already playing!"
                exit 1
            fi
        fi
        touch "$state"
        stime=`date +"%H:%M"`
	if [ -f "$list" ]; then
	    echo "Music list has already generated!"
	else
	    find /media/kevin/SeagateBackup1/Music -name *.mp3 > "$list"
	fi
	# 单次循环随机播放list列表中的歌曲
        mplayer -shuffle -loop 0 -playlist "$list"</dev/null>/dev/null 2>&1 &
        ps -ef|awk '/\ '$$'\ /&&/\ mplayer\ /&&/\ '$stime'\ / {print $2}'>"$state"
	#awk '{print $0}' "$state"
        #chmod 000 "$state"
        ;;
    stop)
        if [ -f "$state" ]; then
            #chmod 777 "$state"
            pid="$(awk '{print $0}' "$state")"

	    # 查找当前的mplayer进程
            if [ -z "$(ps -ef|grep "$pid"|grep "mplayer"|grep -v "grep")" ]; then
                #rm -f "$state"
                echo "Music are not playing!"
                exit 1
            else
                kill "$pid"	# 杀掉mplayer进程
            fi
        else 
            echo "Music are not playing!"
            exit 1
        fi
        ;;
    restart)
        "$0" stop
        [ $? -eq 1 ] && exit 1
        sleep 1
        "$0" start
        ;;
    *)
        echo "Undefined!"
        ;;
esac
