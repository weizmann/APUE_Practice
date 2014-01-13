#for easy find in current repo tree
OKINAWA_DIR=/opt/android-source/packages/trunk/
TOUCHPAL_DIR=/home/zhengwei/WAVE_ONEHAND/cleanTrunk/trunk/
export OKINAWA_DIR
export TOUCHPAL_DIR

function ogrep {
search_dir=
search_item=
arg_err=
if [ $# -eq 1 ] ; then 
    search_dir="."
    search_item=$1
else
    case $1 in 
        -t)
            search_dir=$TOUCHPAL_DIR/TouchPal/jni/
            search_item=$2
            ;;
        -o)
            search_dir=$OKINAWA_DIR/src/
            search_item=$2
            ;;
        -d)
            search_dir=$2
            search_item=$3
            ;;
        -*)
            arg_err="Usage $0: $1 Unrecognized option"
            ;;
        *)
            arg_err="Usage $0 -tod <dir> item"
            ;;
    esac
fi
if [ -z "$arg_err" ] ; then
    grep -Rn $search_item $search_dir | grep -v svn | grep $search_item
else
    echo arg_err
fi
}   
