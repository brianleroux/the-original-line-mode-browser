#  Make WWW for apollo with unwritable source tree linked under timbl
#
#CFLAGS = -I$(CMN) -I$(LM) -DDEBUG
CFLAGS = -I$(CMN) -I$(LM) -g  -DDEBUG

CC = cc

LFLAGS =

WWW = /user/timbl/userd/tbl/hypertext/WWW

CMN = $(WWW)/Implementation/

LM = $(WWW)/LineMode/Implementation/

# This directory:
HERE = /user/timbl/apollo_m68k

# The directory with the default home page in:
DEFAULTS = $(WWW)/LineMode/Defaults

# Directory for installed binary:
BINDIR = /usr/local/bin

include $(LM)CommonMakefile

update : www
	cp www $(BINDIR) 
	- mkdir /usr/local/lib
	cp $(DEFAULTS)/* /usr/local/lib/WWW

install : www
	cp www $(BINDIR) 
	- mkdir /usr/local/lib
	cp $(DEFAULTS)/* /usr/local/lib/WWW





