#!/bin/sh
# ______________________________________________________________________________
#
#  Compile raylib project for Android with FreeType and HarfBuzz
# ______________________________________________________________________________

# Stop on error and display each command as it gets executed.
set -xe

# NOTE: If you excluded any ABIs in the previous steps, remove them from this list too
ABIS="arm64-v8a armeabi-v7a x86 x86_64"

# --- CONFIGURATION PATHS ---
BUILD_TOOLS=android/sdk/build-tools/29.0.3
TOOLCHAIN=android/ndk/toolchains/llvm/prebuilt/linux-x86_64
NATIVE_APP_GLUE=android/ndk/sources/android/native_app_glue

# --- EXTERNAL LIBRARY PATHS ---
FREETYPE_SRC="freetype"
HARFBUZZ_SRC="harfbuzz"

# --- COMMON FLAGS ---
FLAGS="-ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall \
    -Wformat -Werror=format-security -no-canonical-prefixes \
    -DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=29"

# --- INCLUDES: ADD FREETYPE AND HARFBUZZ HEADERS ---
INCLUDES="-I. -Iinclude -I../include -I$NATIVE_APP_GLUE \
    -I$TOOLCHAIN/sysroot/usr/include \
    -I$FREETYPE_SRC \
    -I$FREETYPE_SRC/include \
    -I$HARFBUZZ_SRC/src"

# --- FREETYPE AND HARFBUZZ SPECIFIC FLAGS ---
FREETYPE_FLAGS="-DFT2_BUILD_LIBRARY -DFT_CONFIG_OPTION_SYSTEM_ZLIB"
HARFBUZZ_FLAGS="-DHB_HAVE_FREETYPE -DHAVE_FREETYPE"
    
# Copy icons and other assets
cp assets/icon_ldpi.png android/build/res/drawable-ldpi/icon.png
cp assets/icon_mdpi.png android/build/res/drawable-mdpi/icon.png
cp assets/icon_hdpi.png android/build/res/drawable-hdpi/icon.png
cp assets/icon_xhdpi.png android/build/res/drawable-xhdpi/icon.png
cp assets/* android/build/assets

# ______________________________________________________________________________
#
#  Compile and Link
# ______________________________________________________________________________
#
for ABI in $ABIS; do
    case "$ABI" in
        "armeabi-v7a")
            CCTYPE="armv7a-linux-androideabi"
            ARCH="arm"
            LIBPATH="arm-linux-androideabi"
            ABI_FLAGS="-std=c99 -march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
            ;;

        "arm64-v8a")
            CCTYPE="aarch64-linux-android"
            ARCH="aarch64"
            LIBPATH="aarch64-linux-android"
            ABI_FLAGS="-std=c99 -target aarch64 -mfix-cortex-a53-835769"
            ;;

        "x86")
            CCTYPE="i686-linux-android"
            ARCH="i386"
            LIBPATH="i686-linux-android"
            ABI_FLAGS=""
            ;;

        "x86_64")
            CCTYPE="x86_64-linux-android"
            ARCH="x86_64"
            LIBPATH="x86_64-linux-android"
            ABI_FLAGS=""
            ;;
    esac
    
    # Define compiler and archiver based on current ABI
    CC="$TOOLCHAIN/bin/${CCTYPE}29-clang"
    CXX="$TOOLCHAIN/bin/${CCTYPE}29-clang++"
    AR="$TOOLCHAIN/bin/llvm-ar"
    
    # Create ABI specific lib and build directories
    mkdir -p lib/$ABI
    mkdir -p build/$ABI/freetype
    mkdir -p build/$ABI/harfbuzz
    mkdir -p build/$ABI/src
    mkdir -p build/$ABI/native_app_glue

    # --- 1. COMPILE NATIVE APP GLUE ---
    $CC -c $NATIVE_APP_GLUE/android_native_app_glue.c -o build/$ABI/native_app_glue/native_app_glue.o \
        $INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS
    $AR rcs lib/$ABI/libnative_app_glue.a build/$ABI/native_app_glue/native_app_glue.o

    # --- 2. COMPILE FREETYPE ---
    # Compile essential FreeType modules
    FREETYPE_OBJ_FILES=""
    
    # Core base files
    for file in \
        $FREETYPE_SRC/src/base/ftsystem.c \
        $FREETYPE_SRC/src/base/ftinit.c \
        $FREETYPE_SRC/src/base/ftdebug.c \
        $FREETYPE_SRC/src/base/ftbase.c \
        $FREETYPE_SRC/src/base/ftbbox.c \
        $FREETYPE_SRC/src/base/ftglyph.c \
        $FREETYPE_SRC/src/base/ftbdf.c \
        $FREETYPE_SRC/src/base/ftbitmap.c \
        $FREETYPE_SRC/src/base/ftcid.c \
        $FREETYPE_SRC/src/base/ftfstype.c \
        $FREETYPE_SRC/src/base/ftgasp.c \
        $FREETYPE_SRC/src/base/ftgxval.c \
        $FREETYPE_SRC/src/base/ftmm.c \
        $FREETYPE_SRC/src/base/ftotval.c \
        $FREETYPE_SRC/src/base/ftpatent.c \
        $FREETYPE_SRC/src/base/ftpfr.c \
        $FREETYPE_SRC/src/base/ftstroke.c \
        $FREETYPE_SRC/src/base/ftsynth.c \
        $FREETYPE_SRC/src/base/fttype1.c \
        $FREETYPE_SRC/src/base/ftwinfnt.c; do
        
        if [ -f "$file" ]; then
            BASENAME=$(basename "$file" .c)
            $CC -c $file -o build/$ABI/freetype/${BASENAME}.o \
                $INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS $FREETYPE_FLAGS || exit 1
            FREETYPE_OBJ_FILES="$FREETYPE_OBJ_FILES build/$ABI/freetype/${BASENAME}.o"
        fi
    done
    
    # Font driver modules
    for module in autofit truetype cff cid psaux psnames pshinter raster sfnt smooth bdf pcf pfr type1 type42 winfonts gzip lzw; do
        file="$FREETYPE_SRC/src/$module/$module.c"
        if [ -f "$file" ]; then
            $CC -c $file -o build/$ABI/freetype/${module}.o \
                $INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS $FREETYPE_FLAGS || exit 1
            FREETYPE_OBJ_FILES="$FREETYPE_OBJ_FILES build/$ABI/freetype/${module}.o"
        fi
    done
    
    # Create FreeType static library
    $AR rcs lib/$ABI/libfreetype.a $FREETYPE_OBJ_FILES
    
    # --- 3. COMPILE HARFBUZZ ---
    # HarfBuzz uses C++, so we use CXX compiler
    HARFBUZZ_OBJ_FILES=""
    HARFBUZZ_CXX_FLAGS="-std=c++11"
    
    # Compile HarfBuzz source files
    for file in $HARFBUZZ_SRC/src/harfbuzz.cc; do
        if [ -f "$file" ]; then
            BASENAME=$(basename "$file" .cc)
            $CXX -c $file -o build/$ABI/harfbuzz/${BASENAME}.o \
                $INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $HARFBUZZ_CXX_FLAGS $HARFBUZZ_FLAGS || exit 1
            HARFBUZZ_OBJ_FILES="$HARFBUZZ_OBJ_FILES build/$ABI/harfbuzz/${BASENAME}.o"
        fi
    done
    
    # If harfbuzz.cc doesn't exist, compile individual source files
    if [ -z "$HARFBUZZ_OBJ_FILES" ]; then
        for file in $HARFBUZZ_SRC/src/*.cc; do
            if [ -f "$file" ] && [ "$(basename "$file")" != "main.cc" ]; then
                BASENAME=$(basename "$file" .cc)
                $CXX -c $file -o build/$ABI/harfbuzz/${BASENAME}.o \
                    $INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $HARFBUZZ_CXX_FLAGS $HARFBUZZ_FLAGS || exit 1
                HARFBUZZ_OBJ_FILES="$HARFBUZZ_OBJ_FILES build/$ABI/harfbuzz/${BASENAME}.o"
            fi
        done
    fi
    
    # Create HarfBuzz static library
    $AR rcs lib/$ABI/libharfbuzz.a $HARFBUZZ_OBJ_FILES

    # --- 4. COMPILE PROJECT (Your Source Files) ---
    for file in src/*.c; do
        if [ -f "$file" ]; then
            BASENAME=$(basename "$file" .c)
            $CC -c $file -o build/$ABI/src/${BASENAME}.o \
                $INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $FLAGS $ABI_FLAGS || exit 1
        fi
    done

    # --- 5. LINK THE FINAL PROJECT ---
    # Note: Linker order is CRITICAL: libmain -> harfbuzz -> freetype
    $TOOLCHAIN/bin/ld.lld build/$ABI/src/*.o -o android/build/lib/$ABI/libmain.so -shared \
        --exclude-libs libatomic.a --build-id \
        -z noexecstack -z relro -z now \
        --warn-shared-textrel --fatal-warnings -u ANativeActivity_onCreate \
        -L$TOOLCHAIN/sysroot/usr/lib/$LIBPATH/29 \
        -L$TOOLCHAIN/sysroot/usr/lib/$LIBPATH \
        -L$TOOLCHAIN/lib/clang/17/lib/linux/$ARCH \
        -L. -Landroid/build/obj -Llib/$ABI \
        -lraylib -lharfbuzz -lfreetype -lnative_app_glue -llog -landroid -lEGL -lGLESv2 -lOpenSLES -lc -lm -ldl -lc++_static -lc++abi
done

# ______________________________________________________________________________
#
#  Build APK
# ______________________________________________________________________________
#
$BUILD_TOOLS/aapt package -f -m \
    -S android/build/res -J android/build/src -M android/build/AndroidManifest.xml \
    -I android/sdk/platforms/android-29/android.jar

# Compile NativeLoader.java
javac -verbose -source 1.8 -target 1.8 -d android/build/obj \
    -bootclasspath jre/lib/rt.jar \
    -classpath android/sdk/platforms/android-29/android.jar:android/build/obj \
    -sourcepath src android/build/src/com/pallanguzhi/game/R.java \
    android/build/src/com/pallanguzhi/game/NativeLoader.java

$BUILD_TOOLS/dx --verbose --dex --output=android/build/dex/classes.dex android/build/obj

# Add resources and assets to APK
$BUILD_TOOLS/aapt package -f \
    -M android/build/AndroidManifest.xml -S android/build/res -A assets \
    -I android/sdk/platforms/android-29/android.jar -F game.apk android/build/dex

# Add libraries to APK
cd android/build
for ABI in $ABIS; do
    ../../$BUILD_TOOLS/aapt add ../../game.apk lib/$ABI/libmain.so
done
cd ../..

# Zipalign APK
$BUILD_TOOLS/zipalign -f 4 game.apk game.final.apk
mv -f game.final.apk game.apk

# Sign APK - Use environment variable for keystore password if available
if [ -n "$KEYSTORE_PASS" ]; then
    apksigner sign --ks android/saiyan.keystore --out my-app-release.apk --ks-pass pass:$KEYSTORE_PASS game.apk
else
    echo "WARNING: KEYSTORE_PASS environment variable not set. Please sign manually or set the password."
    echo "You can sign with: apksigner sign --ks android/saiyan.keystore --out my-app-release.apk game.apk"
    exit 1
fi

mv my-app-release.apk game.apk

# Install to device or emulator
android/sdk/platform-tools/adb install -r game.apk
