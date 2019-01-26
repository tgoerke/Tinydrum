# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile
ARDMK_DIR = ~/src/arduino/Arduino-Makefile

# Attiny core from
# https://arduino-tiny.googlecode.com/files/arduino-tiny-0100-0018.zip
ALTERNATE_CORE_PATH = /usr/share/arduino/tiny

BOARD_TAG = attiny45at1
ISP_PORT = /dev/ttyACM0
ISP_PROG = arduino

ARDUINO_LIBS = AttinyBassDrum
USER_LIB_PATH = libraries

include $(ARDMK_DIR)/Arduino.mk

# !!! Important. You have to use make ispload to upload when using ISP programmer
