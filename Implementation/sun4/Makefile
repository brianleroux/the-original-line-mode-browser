#  Make WWW under unix for sun 4
#
# This directory:
HERE = /userd/tbl/hypertext/WWW/LineMode/Implementation/sun4

# The directory with the deafult home page in:
DEFAULTS = /userd/tbl/hypertext/WWW/LineMode/Defaults

# Directory for installed binary:
BINDIR = /usr/local/bin


CMN = ../../../Implementation/
LM = ../
COMMON = HTParse.o HTAccess.o HTTP.o
OBJS = www.o $(COMMON) HTBufferFile.o

CFLAGS = -I$(CMN) -I$(LM) -DDEBUG
#CFLAGS = -I$(CMN) -I$(LM) -g  -DDEBUG

CC = cc

include $(LM)CommonMakefile

