# Quick'n'dirty makefile [BC] v2

ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

LIBOGC_INC      :=      $(DEVKITPRO)/libogc/include
LIBOGC_LIB      :=      $(DEVKITPRO)/libogc/lib/wii

PORTLIBS_INC	:=		../../include
PORTLIBS_LIB	:=		../../lib

CFLAGS  := -O4 $(MACHDEP) -I$(LIBOGC_INC) -DHAVE_CONFIG_H

LIB     := ntfs
CFILES  := $(wildcard *.c)
OFILES  := $(CFILES:.c=.o)
ARC     := lib$(LIB).a
HDR     := ntfs.h

all : $(OFILES)
		$(AR) -r $(ARC) $(OFILES)

clean :
		rm -f $(OFILES) $(ARC)

install :
		mkdir -p  $(PORTLIBS_INC)  $(PORTLIBS_LIB)
		cp -f $(ARC) $(PORTLIBS_LIB)/
		cp -f $(HDR) $(PORTLIBS_INC)/

%.o : %.c
		$(CC) $(CFLAGS) -c $< -o $@