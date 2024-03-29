#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the avrbertos_user.mk file instead.
#

# Constants automatically defined by the selected modules

# Set to 1 for debug builds
avrbertos_DEBUG = 1
#avrbertos_CPPFLAGS += -DRELEASE

#avrbertos_CPPFLAGS += -DCONFIG_PRINTF=PRINTF_DISABLED

# Our target application
TRG += avrbertos

avrbertos_PREFIX = "/usr/local/avr/bin/avr-"

avrbertos_SUFFIX = ""

avrbertos_SRC_PATH = avrbertos

avrbertos_HW_PATH = avrbertos

# Files automatically generated by the wizard. DO NOT EDIT, USE avrbertos_USER_CSRC INSTEAD!
avrbertos_WIZARD_CSRC = \
	bertos/io/kfile.c \
	bertos/cpu/avr/drv/ser_mega.c \
	bertos/cpu/avr/drv/pwm_avr.c \
	bertos/cpu/avr/drv/timer_avr.c \
	bertos/cpu/avr/drv/timer_mega.c \
	bertos/cpu/avr/drv/ram_avr.c \
	bertos/cpu/avr/drv/eeprom_avr.c \
	bertos/drv/timer.c \
	bertos/drv/ser.c \
	bertos/drv/kbd.c \
	bertos/drv/lcd_ks0108.c \
	bertos/mware/formatwr.c \
	bertos/mware/fixedpoint.c \
	bertos/mware/hex.c \
	bertos/mware/sprintf.c \
	bertos/algo/crc_ccitt.c \
	bertos/net/afsk.c \
	bertos/net/ax25.c \
	bertos/net/nmea.c \
	bertos/net/nmeap/src/nmeap01.c \
	bertos/gfx/bitmap.c \
	bertos/gfx/text.c \
	bertos/gfx/text_format.c \
	bertos/gfx/line.c \
	bertos/icons/logo.c \
	bertos/fonts/mina.c \
	bertos/fonts/gohu.c \
	bertos/fonts/hp5.c \
	bertos/fonts/helvB10.c \
	bertos/verstag.c \
	bertos/gui/menu_pro.c \
	bertos/mware/event.c \
	bertos/drv/buttons.c \
	bertos/drv/onewire.c \
#	bertos/gui/micromenu.c \
#	bertos/drv/pwm.c \
#	bertos/kern/proc.c \
#	bertos/kern/signal.c \
#	bertos/struct/heap.c \
#	bertos/fonts/fixed6x8.c \
#	bertos/fonts/mona12.c \
#	bertos/fonts/mozart9.c \
#	bertos/fonts/draco11.c \
#	bertos/fonts/luBS14.c \
#	bertos/fonts/helvR8.c \

#	bertos/net/hdlc.c \
#	bertos/net/kiss.c \


#	bertos/icons/bertos.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE avrbertos_USER_PCSRC INSTEAD!
avrbertos_WIZARD_PCSRC = \
	bertos/gfx/text_format.c \
	bertos/mware/formatwr.c \
	bertos/mware/sprintf.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE avrbertos_USER_CPPASRC INSTEAD!
avrbertos_WIZARD_CPPASRC = \
	bertos/cpu/avr/hw/switch_ctx_avr.S \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE avrbertos_USER_CXXSRC INSTEAD!
avrbertos_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE avrbertos_USER_ASRC INSTEAD!
avrbertos_WIZARD_ASRC = \
	 \
	#

avrbertos_CPPFLAGS = -D'CPU_FREQ=(16000000UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(avrbertos_HW_PATH) -I$(avrbertos_SRC_PATH) $(avrbertos_CPU_CPPFLAGS) $(avrbertos_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_LDFLAGS = $(avrbertos_CPU_LDFLAGS) $(avrbertos_WIZARD_LDFLAGS) $(avrbertos_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_CPPAFLAGS = $(avrbertos_CPU_CPPAFLAGS) $(avrbertos_WIZARD_CPPAFLAGS) $(avrbertos_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_CSRC = $(avrbertos_CPU_CSRC) $(avrbertos_WIZARD_CSRC) $(avrbertos_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_PCSRC = $(avrbertos_CPU_PCSRC) $(avrbertos_WIZARD_PCSRC) $(avrbertos_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_CPPASRC = $(avrbertos_CPU_CPPASRC) $(avrbertos_WIZARD_CPPASRC) $(avrbertos_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_CXXSRC = $(avrbertos_CPU_CXXSRC) $(avrbertos_WIZARD_CXXSRC) $(avrbertos_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
avrbertos_ASRC = $(avrbertos_CPU_ASRC) $(avrbertos_WIZARD_ASRC) $(avrbertos_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!

avrbertos_DEBUG_SCRIPT = bertos/prg_scripts/nodebug.sh
avrbertos_PROGRAMMER_CPU = atmega2560
avrbertos_STOPDEBUG_SCRIPT = bertos/prg_scripts/none.sh
avrbertos_MCU = atmega2560
avrbertos_FLASH_SCRIPT = bertos/prg_scripts/avr/flash.sh
avrbertos_STOPFLASH_SCRIPT = bertos/prg_scripts/avr/stopflash.sh
avrbertos_CPU_CPPFLAGS = -Os -Ibertos/cpu/avr/

include $(avrbertos_SRC_PATH)/avrbertos_user.mk
