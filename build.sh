
ARDUINO_DIR=/Users/mawe/work/arduino/Arduino.app/Contents/Java

GCC=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc
CXX=$ARDUINO_DIR/hardware/tools/avr/bin/avr-g++
AR=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc-ar
NM=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc-nm
RANLIB=$ARDUINO_DIR/hardware/tools/avr/bin/avr-gcc-ranlib
OBJCOPY=$ARDUINO_DIR/hardware/tools/avr/bin/avr-objcopy
AVRDUDE=$ARDUINO_DIR/hardware/tools/avr/bin/avrdude


MCU=atmega328p

INCLUDES="-I $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/ -I $ARDUINO_DIR/hardware/arduino/avr/variants/standard"
CPPFLAGS="$INCLUDES -MMD -c -mmcu=$MCU -Wall -Os -ffunction-sections -fdata-sections -DF_CPU=16000000L -D__PROG_TYPES_COMPAT__"
CFLAGS="$CPPFLAGS -std=gnu11 -flto -fno-fat-lto-objects"
CXXFLAGS="$CPPFLAGS -fno-threadsafe-statics -flto -fpermissive -fno-exceptions"
LDFLAGS="-w -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=$MCU"

BUILD=./build
TARGET=kitchentimer

set -e

mkdir -p $BUILD

echo "BUILDING CORE"
# Building the Arduino core stuff manually
$GCC $CFLAGS $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/hooks.c -o $BUILD/_hooks.o
$GCC $CFLAGS $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/wiring.c -o $BUILD/_wiring.o
$GCC $CFLAGS $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/wiring_digital.c -o $BUILD/_wiring_digital.o

rm $BUILD/libcore.a
$AR rcs $BUILD/libcore.a $BUILD/_hooks.o $BUILD/_wiring.o $BUILD/_wiring_digital.o

# Build the Arduino main.cpp
$CXX $CXXFLAGS $ARDUINO_DIR/hardware/arduino/avr/cores/arduino/main.cpp -o $BUILD/_main.o

echo "BUILDING SOURCE"
$CXX $CXXFLAGS src/main.cpp -o $BUILD/main.o

echo "LINKING $TARGET"
$GCC $LDFLAGS -o $BUILD/$TARGET.elf $BUILD/_main.o $BUILD/main.o $BUILD/libcore.a -lm

echo "CREATING HEX"
#$OBJCOPY -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0  $BUILD/$TARGET.elf $BUILD/$TARGET.eep
$OBJCOPY -O ihex -R .eeprom  $BUILD/$TARGET.elf $BUILD/$TARGET.hex


echo "UPLOADING FLASH"
$AVRDUDE -v -C$ARDUINO_DIR/hardware/tools/avr/etc/avrdude.conf -patmega328p -carduino -P/dev/cu.wchusbserial1410 -b115200 -D -Uflash:w:$BUILD/$TARGET.hex:i
