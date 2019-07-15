#
# User makefile.
# Edit this file to change compiler options and related stuff.
#

# Programmer interface configuration, see http://dev.bertos.org/wiki/ProgrammerInterface for help
avrbertos_PROGRAMMER_TYPE = none
avrbertos_PROGRAMMER_PORT = none

# Files included by the user.
avrbertos_USER_CSRC = \
	$(avrbertos_SRC_PATH)/main.c \
	$(avrbertos_SRC_PATH)/hw/hw_afsk.c \
	$(avrbertos_SRC_PATH)/aprs.c \
	$(avrbertos_SRC_PATH)/menu.c \
	$(avrbertos_SRC_PATH)/gps.c \
	$(avrbertos_SRC_PATH)/sensors.c \

#	$(avrbertos_SRC_PATH)/afsk_dcd.c \
#	$(avrbertos_SRC_PATH)/mobilinkd_error.c \
#	$(avrbertos_SRC_PATH)/battery.c \
	#

# Files included by the user.
avrbertos_USER_PCSRC = \
	#

# Files included by the user.
avrbertos_USER_CPPASRC = \
	#

# Files included by the user.
avrbertos_USER_CXXSRC = \
	#

# Files included by the user.
avrbertos_USER_ASRC = \
	#

# Flags included by the user.
avrbertos_USER_LDFLAGS = \
	#

# Flags included by the user.
avrbertos_USER_CPPAFLAGS = \
	#

# Flags included by the user.
avrbertos_USER_CPPFLAGS = \
	-fno-strict-aliasing \
	-fwrapv \
	-fPIC \
	#
