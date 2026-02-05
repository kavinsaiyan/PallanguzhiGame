cd ~/Code/GitHub/PallanguzhiGame/Raylib-Pallanguzhi/app/src/main/cpp/deps/zeromq/czmq/builds/android

# Set the -fPIC flags
export ANDROID_BUILD_EXTRA_CFLAGS="-fPIC"
export ANDROID_BUILD_EXTRA_CXXFLAGS="-fPIC"

#If needed change this ndk root to correct location
export ANDROID_NDK_ROOT=/home/kavin/Code/GitHub/PallanguzhiGame/Raylib-Pallanguzhi/sdk/ndk/26.1.10909125/

./build.sh arm64

./build.sh arm

./build.sh x86

./build.sh x86_64
