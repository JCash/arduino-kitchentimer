ARDUINO_DIR=/Users/mawe/work/arduino/Arduino.app/Contents/Java
ARDUINO_TOOLS_DIR=$ARDUINO_DIR/hardware/tools/avr/bin
#ARDUINO_TOOLS_DIR=/usr/local/bin/

#AVR_DEBUG=~/work/external/avr_debug

GCC=$ARDUINO_TOOLS_DIR/avr-gcc
CXX=$ARDUINO_TOOLS_DIR/avr-g++
AR=$ARDUINO_TOOLS_DIR/avr-gcc-ar
NM=$ARDUINO_TOOLS_DIR/avr-gcc-nm
RANLIB=$ARDUINO_TOOLS_DIR/avr-gcc-ranlib
OBJCOPY=$ARDUINO_TOOLS_DIR/avr-objcopy
AVRDUDE=$ARDUINO_TOOLS_DIR/avrdude
#AVRSIZE=$ARDUINO_TOOLS_DIR/avr-size
AVRSIZE=~/work/external/toolchain-avr/objdir/bin/avr-size

MCU=atmega328p
PORT=/dev/cu.wchusbserial1420

BUILD=./build
TARGET=kitchentimer

INCLUDES="-I$ARDUINO_DIR/hardware/arduino/avr/cores/arduino/ -I$ARDUINO_DIR/hardware/arduino/avr/variants/standard -I$ARDUINO_DIR/hardware/arduino/avr/libraries/Wire/src/"
CPPOPT="-Os"
CPPFLAGS="-MMD -c -mmcu=$MCU -Wall -g -gdwarf-2 -ffunction-sections -fdata-sections -DF_CPU=16000000L -D__PROG_TYPES_COMPAT__ -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR -fdiagnostics-color"
#AVR_DEBUG  CPPFLAGS="-c -g -Os -w -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -MMD -flto -mmcu=$MCU -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_UNO -DARDUINO_ARCH_AVR"
CFLAGS="-std=gnu11 -flto -fno-fat-lto-objects"
CXXFLAGS="$CPPFLAGS -fno-threadsafe-statics -flto -fpermissive -fno-exceptions"
ASFLAGS="-x assembler-with-cpp -flto"
LDFLAGS="-w -Os -g -gdwarf-2 -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=$MCU -L$BUILD"
LIBS="-lcore -lwire -lwireutility -lm"

set -e

mkdir -p $BUILD/


#hack for testing
# if [ -e "$BUILD/libcore.a" ]; then
#     rm $BUILD/libcore.a
#     rm $BUILD/libavrdebug.a
# fi

function CompileLibrary
{
    local libname=$1
    local libbasename=tmplib
    local sourcedir=$2

    if [ ! -f "$libname" ]; then

        echo "BUILDING $libname"

        for f in $sourcedir/*.c
        do
            [ -f "$f" ] || continue
            if [ ! -z "$AVR_DEBUG" ]; then
                if [ "$(basename $f)" == "WInterrupts.c" ]; then
                    f=$AVR_DEBUG/arduino/1.8.1/WInterrupts.c
                fi
            fi
            out=$BUILD/${libbasename}_$(basename $f).o
            ##echo "Compiling - $(basename $f) -> $out"
            echo "Compiling - $f -> $out"
            $GCC $INCLUDES $CPPOPT $CPPFLAGS $CFLAGS $f -o $out
        done

        for f in $sourcedir/*.S
        do
            [ -f "$f" ] || continue
            out=$BUILD/${libbasename}_$(basename $f).o
            echo "Compiling - $(basename $f) -> $out"
            $CXX $INCLUDES $CPPOPT $CPPFLAGS $ASFLAGS $f -o $out
        done

        for f in $sourcedir/*.cpp
        do
            [ -f "$f" ] || continue
            if [ ! -z "$AVR_DEBUG" ]; then
                if [ "$(basename $f)" == "HardwareSerial0.cpp" ]; then
                    continue
                fi
                if [ "$(basename $f)" == "Print.cpp" ]; then
                    continue
                fi
            fi
            out=$BUILD/${libbasename}_$(basename $f).o
            echo "Compiling - $(basename $f) -> $out"
            $CXX $INCLUDES $CPPOPT $CPPFLAGS $CXXFLAGS $f -o $out
        done

        $AR rcs $libname $BUILD/${libbasename}_*.o
        rm $BUILD/${libbasename}_*.o
        rm $BUILD/${libbasename}_*.d
    fi
}

if [ ! -z "$AVR_DEBUG" ]; then
    echo "DEBUG ENABLED"
    CPPOPT="-Og"
    CPPFLAGS="-DAVR_DEBUG=1 $CPPFLAGS"
    LIBS="-lavrdebug $LIBS"
    INCLUDES="-I$AVR_DEBUG $INCLUDES"

    echo "BUILDING" $BUILD/libavrdebug.a
    CompileLibrary $BUILD/libavrdebug.a $AVR_DEBUG/avr8-stub
fi

# Building the Arduino core manually
CompileLibrary $BUILD/libcore.a $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/

# Wire
CompileLibrary $BUILD/libwire.a $ARDUINO_DIR/hardware/arduino/avr/libraries/Wire/src/
CompileLibrary $BUILD/libwireutility.a $ARDUINO_DIR/hardware/arduino/avr/libraries/Wire/src/utility

echo "BUILDING SOURCE"
$CXX $INCLUDES $CPPOPT $CPPFLAGS $CXXFLAGS src/main.cpp -o $BUILD/main.o

echo "LINKING $TARGET"
$GCC $LDFLAGS -o $BUILD/$TARGET.elf $BUILD/main.o $LIBS -lm

echo "CREATING HEX"
$OBJCOPY -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $BUILD/$TARGET.elf $BUILD/$TARGET.eep
$OBJCOPY -O ihex -R .eeprom  $BUILD/$TARGET.elf $BUILD/$TARGET.hex

echo "VERIFYING SIZE"
$AVRSIZE --mcu=$MCU -C $BUILD/$TARGET.elf

echo "UPLOADING FLASH"
$AVRDUDE -V -C$ARDUINO_DIR/hardware/tools/avr/etc/avrdude.conf -p$MCU -carduino -P$PORT -b115200 -D -Uflash:w:$BUILD/$TARGET.hex:i
