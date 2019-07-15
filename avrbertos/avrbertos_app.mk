#
# $Id: demo.mk 18234 2007-10-08 13:39:48Z rasky $
# Copyright 2003, 2004, 2005, 2006 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Makefile fragment for DevLib demo application.
#
# Author: Bernardo Innocenti <bernie@develer.com>
#

include bertos/fonts/fonts.mk
include bertos/emul/emul.mk

# Set to 1 for debug builds
avrbertos_DEBUG = 1
#avrbertos_CPPFLAGS += -DRELEASE

# This is an hosted application
avrbertos_HOSTED = 1

# Our target application
TRG += avrbertos

# FIXME: we want to use g++ for C source too
CC = g++

avrbertos_CXXSRC = \
	bertos/emul/emul.cpp \
	bertos/emul/emulwin.cpp \
	bertos/drv/lcd_gfx_qt.cpp \
	bertos/emul/emulkbd.cpp \

avrbertos_CSRC = \
	bertos/io/kfile.c \
	bertos/os/hptime.c \
	bertos/gfx/bitmap.c \
	bertos/gfx/line.c \
	bertos/gfx/win.c \
	bertos/gfx/text.c \
	bertos/gfx/text_format.c \
	bertos/fonts/mina.c \
	bertos/fonts/helvB10.c \
	bertos/fonts/gohu.c \
	bertos/fonts/hp5.c \
	bertos/icons/logo.c \
	bertos/drv/timer.c \
	bertos/drv/timer_test.c \
	bertos/drv/ser.c \
	bertos/emul/ser_posix.c \
	bertos/emul/kfile_posix.c \
	bertos/mware/formatwr.c \
	bertos/mware/hex.c \
	bertos/mware/event.c \
	bertos/mware/observer.c \
	bertos/mware/resource.c \
	bertos/mware/sprintf.c \
	bertos/struct/heap.c \
	bertos/struct/kfile_mem.c \
	bertos/drv/kbd.c \
	bertos/gui/menu_pro.c \
	bertos/kern/irq.c \
	bertos/kern/proc.c \
	bertos/kern/proc_test.c \
	bertos/kern/signal.c \
	bertos/kern/monitor.c \
	bertos/kern/sem.c \
	bertos/kern/sem_test.c \
	bertos/net/nmeap/src/nmeap01.c \
	bertos/net/nmea.c \
	bertos/verstag.c \
	avrbertos/main.c \
	avrbertos/aprs.c \
	avrbertos/gps.c \
	bertos/net/ax25.c \
	avrbertos/menu.c \
#	bertos/gui/micromenu.c \
#	bertos/net/afsk.c \
#	bertos/fonts/helvR8.c \
#	bertos/drv/buzzer.c \

avrbertos_CPPASRC = \
	bertos/emul/switch_ctx_emul.S

# FIXME: maybe this junk should go in emul/emul.mk?
$(OBJDIR)/bertos/emul/emulwin.o: bertos/emul/emulwin_moc.cpp
$(OBJDIR)/bertos/drv/lcd_gfx_qt.o: bertos/drv/lcd_gfx_qt_moc.cpp
$(OBJDIR)/bertos/drv/timer.o: bertos/emul/timer_qt_moc.cpp
$(OBJDIR)/bertos/emul/emulkbd.o: bertos/emul/emulkbd_moc.cpp

#FIXME: isn't there a way to avoid repeating the pattern rule?
bertos/emul/timer_qt_moc.cpp: bertos/emul/timer_qt.c
	$(QT_MOC) -o $@ $<


avrbertos_CPPAFLAGS =
avrbertos_CFLAGS = -D_QT=4 -D'ARCH=ARCH_EMUL' -Iavrbertos $(EMUL_CFLAGS)
avrbertos_CXXFLAGS = -D_QT=4 -D'ARCH=ARCH_EMUL' -Iavrbertos $(EMUL_CFLAGS)
avrbertos_LDFLAGS = $(EMUL_LDFLAGS)

# Debug stuff
ifeq ($(avrbertos_DEBUG),1)
	avrbertos_CFLAGS += -D_DEBUG
	avrbertos_CXXFLAGS += -D_DEBUG
#	avrbertos_CSRC += bertos/drv/kdebug.c
else
	avrbertos_CFLAGS += -Os
	avrbertos_CXXFLAGS += -Os
endif

