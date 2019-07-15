#
# $Id$
# Copyright 2002, 2003, 2004, 2006 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Author: Bernie Innocenti <bernie@codewiz.org>
#

# Set to 1 for verbose build output, 0 for terse output
V := 1

default: all

include bertos/config.mk

# include avrbertos/avrbertos_avr.mk
include avrbertos/avrbertos_app.mk


include bertos/rules.mk
