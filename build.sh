
ARDUINO_DIR=/Users/mawe/work/arduino/Arduino.app/Contents/Java

GCC=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc
CXX=$ARDUINO_DIR/hardware/tools/avr/bin/avr-g++
AR=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc-ar
NM=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc-nm
RANLIB=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc-ranlib
OBJCOPY=$ARDUINO_DIR/hardware/tools/avr/bin/avr-objcopy
AVRDUDE=$ARDUINO_DIR/hardware/tools/avr/bin/avrdude
AVRSIZE=$ARDUINO_DIR/hardware/tools/avr/bin/avr-size

MCU=atmega328p
PORT=/dev/cu.wchusbserial1410

INCLUDES="-I $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/ -I $ARDUINO_DIR/hardware/arduino/avr/variants/standard"
CPPFLAGS="$INCLUDES -MMD -c -mmcu=$MCU -Wall -g -Os -ffunction-sections -fdata-sections -DF_CPU=16000000L -D__PROG_TYPES_COMPAT__ -fdiagnostics-color"
CFLAGS="$CPPFLAGS -std=gnu11 -flto -fno-fat-lto-objects"
CXXFLAGS="$CPPFLAGS -fno-threadsafe-statics -flto -fpermissive -fno-exceptions"
ASFLAGS="-x assembler-with-cpp -flto"
LDFLAGS="-w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=$MCU"

BUILD=./build
TARGET=kitchentimer

set -e

mkdir -p $BUILD/


#hack for testing
#rm $BUILD/libcore.a

function CompileLibrary
{
    local libname=$1
    local libbasename=tmplib
    local sourcedir=$2

    if [ ! -f "$libname" ]; then

        echo "BUILDING $libname"

        for f in $sourcedir/*.c
        do
            out=$BUILD/${libbasename}_$(basename $f).o
            echo "Compiling - $(basename $f) -> $out"
            $GCC $CFLAGS $f -o $out
        done

        for f in $sourcedir/*.S
        do
            out=$BUILD/${libbasename}_$(basename $f).o
            echo "Compiling - $(basename $f) -> $out"
            $CXX $CPPFLAGS $ASFLAGS $f -o $out
        done

        for f in $sourcedir/*.cpp
        do
            out=$BUILD/${libbasename}_$(basename $f).o
            echo "Compiling - $(basename $f) -> $out"
            $CXX $CXXFLAGS $f -o $out
        done

        $AR rcs $libname $BUILD/${libbasename}_*.o
        rm $BUILD/${libbasename}_*.o
        rm $BUILD/${libbasename}_*.d
    fi
}

# Building the Arduino core manually
CompileLibrary $BUILD/libcore.a $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/

echo "BUILDING SOURCE"
$CXX $CXXFLAGS src/main.cpp -o $BUILD/main.o

echo "LINKING $TARGET"
$GCC $LDFLAGS -o $BUILD/$TARGET.elf $BUILD/main.o $BUILD/libcore.a -lm

echo "CREATING HEX"
$OBJCOPY -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $BUILD/$TARGET.elf $BUILD/$TARGET.eep
$OBJCOPY -O ihex -R .eeprom  $BUILD/$TARGET.elf $BUILD/$TARGET.hex

echo "VERIFYING SIZE"
$AVRSIZE --mcu=$MCU -C --format=avr $BUILD/$TARGET.elf

echo "UPLOADING FLASH"
$AVRDUDE -q -V -C$ARDUINO_DIR/hardware/tools/avr/etc/avrdude.conf -p$MCU -carduino -P$PORT -b115200 -D -Uflash:w:$BUILD/$TARGET.hex:i
