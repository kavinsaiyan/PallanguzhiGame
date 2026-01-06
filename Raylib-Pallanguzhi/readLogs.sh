id=$(adb shell pidof -s com.pallanguzhi.game)
adb logcat --pid=$id > logs.txt
