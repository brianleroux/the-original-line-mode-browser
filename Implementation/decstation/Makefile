#  Make WWW under unix for VAX/ultrix using cc
#
# This directory:
HERE = /usr/users/jfg/userd/tbl/hypertext/WWW/LineMode/Implementation/decstation

# The directory with the deafult home page in:
DEFAULTS = $(HERE)/../../Defaults

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
