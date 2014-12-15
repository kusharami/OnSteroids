pushd $(dirname $(readlink -f $BASH_SOURCE)) > /dev/null
#
UnixizeText "gdb.setup"
#
#
ndk-gdb --force --adb=/cygdrive/h/android-sdk-windows/platform-tools/adb
#
popd > /dev/null