#!/bin/bash
# Usage:
# ./loop_and_random_play_movie.sh start | stop | restart
# Author: kevin

movie_state=/media/kevin/SeagateBackup1/Video/Movie/movie_state
movie_list=/media/kevin/SeagateBackup1/Video/Movie/movie_list

[ -z "$1" ] && echo "./loop_and_random_play_movie.sh [start | stop | restart]" && exit 1

arg="$1"
case "$arg" in
    start)
        if [ -f "$movie_state" ]; then
            chmod 777 "$movie_state"
            pid="$(awk '{print $0}' "$movie_state")"
            if [ -z "$(ps -ef | grep "$pid" | grep "mplayer" | grep -v "grep")" ]; then
		#grep -v
                rm -f "$movie_state"
            else
                echo "Movie are already playing!"
                exit 1
            fi
        fi
        touch "$movie_state"
        stime=`date +"%H:%M"`
	if [ -f "$movie_list" ]; then
	    echo "Movie list has already generated!"
	else
		rm "$movie_list"
		touch "$movie_list"
	    find /media/kevin/SeagateBackup1/Video/Movie/ -name *.mp4 >> "$movie_list"
	    find /media/kevin/SeagateBackup1/Video/Movie/ -name *.mkv >> "$movie_list"
	    find /media/kevin/SeagateBackup1/Video/Movie/ -name *.rmvb >> "$movie_list"
	fi
	# 单次循环随机播放movie_list列表中的歌曲
        mplayer -shuffle -loop 0 -playlist "$movie_list"</dev/null>/dev/null 2>&1 &
        ps -ef|awk '/\ '$$'\ /&&/\ mplayer\ /&&/\ '$stime'\ / {print $2}'>"$movie_state"
	#awk '{print $0}' "$movie_state"
        #chmod 000 "$movie_state"
        ;;
    stop)
        if [ -f "$movie_state" ]; then
            #chmod 777 "$movie_state"
            pid="$(awk '{print $0}' "$movie_state")"

	    # 查找当前的mplayer进程
            if [ -z "$(ps -ef|grep "$pid"|grep "mplayer"|grep -v "grep")" ]; then
                #rm -f "$movie_state"
                echo "Movie are not playing!"
                exit 1
            else
                kill "$pid"	# 杀掉mplayer进程
            fi
        else 
            echo "Movie are not playing!"
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
