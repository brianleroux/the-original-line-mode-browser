!	Make WorldWideWeb line mode hypertext browser under VMS
!       =======================================================
!
! History:
!  14 Aug 91	Reconstituted (TBL)
!
!	Check the correct one is selected for your TCP/IP implementation:
!
.IFDEF UCX
LIBS = sys$library:ucx$ipc/lib		! For UCX
OPTION_FILE = 
CFLAGS = /DEBUG/DEFINE=DEBUG
.ENDIF
.IFDEF MULTINET
LIBS = multinet.opt/opt			! For Multinet
OPTION_FILE = multinet.opt
CFLAGS = /DEFINE=(DEBUG,MULTINET)
.ENDIF
.IFDEF WIN_TCP
LIBS = win_tcp.opt/opt			! For Wollongong TCP
OPTION_FILE = win_tcp.opt
CFLAGS = /DEFINE=(DEBUG,WIN_TCP)
.ENDIF

.IFDEF LIBS
.ELSE
LIBS = multinet.opt/opt			! (Default to multinet)
OPTION_FILE = multinet.opt
CFLAGS = /DEFINE=(DEBUG,MULTINET)
.ENDIF

OBJECTS = HTParse.obj, HTAccess.obj, HTBufferFile.obj, HTFormatText.obj, -
          HTTP.obj, HTFTP.obj, HTTCP.obj, HTFile.obj

OBJECTS_D = HTParse_d.obj, HTAccess_d.obj, HTBufferFile_d.obj, -
            HTFormatText_d.obj, HTTP_d.obj, HTFTP_d.obj, HTTCP_d.obj, -
            HTFile.obj

!___________________________________________________________________
! WWW

WWW.exe : WWW.obj $(OBJECTS) $(OPTION_FILE)
	link WWW, $(OBJECTS), $(LIBS)

WWW.obj : WWW.c HTParse.h HTAccess.h tcp.h -
          HTBufferFile.h HTFormatText.h WWW.h HTShort.h
	cc $(CFLAGS) WWW.c

WWW_d.exe : WWW_d.obj $(OBJECTS_D) $(OPTION_FILE)
	link/debug WWW_d, $(OBJECTS_D), $(LIBS)

WWW_d.obj : HTParse.h HTAccess.h tcp.h -
            HTBufferFile.h HTFormatText.h HTShort.h
	cc $(CFLAGS)/debug/obj=WWW_d.obj WWW.c

!____________________________________________________________________

update : www.exe setup.com
	copy www.exe [.works]
	copy setup.com [.works]

!___________________________________________________________________
! What depends on what


HTParse.obj   : HTParse.c HTParse.h HTUtils.h tcp.h
        cc $(CFLAGS)/obj=$*.obj HTParse.c
HTParse_d.obj : HTParse.c HTParse.h HTUtils.h tcp.h
	cc/debug $(CFLAGS)/obj=$*.obj HTParse.c

HTAccess.obj   : HTAccess.c HTAccess.h HTParse.h HTUtils.h WWW.h -
                 HTFTP.h HTTP.h HTFile.h
        cc $(CFLAGS)/obj=$*.obj HTAccess.c
HTAccess_d.obj : HTAccess.c HTAccess.h HTParse.h HTUtils.h WWW.h -
                 HTFTP.h HTTP.h HTFile.h
	cc/debug $(CFLAGS)/obj=$*.obj HTAccess.c

HTTP.obj   : HTTP.c HTTP.h HTParse.h HTUtils.h tcp.h
        cc $(CFLAGS)/obj=$*.obj HTTP.c
HTTP_d.obj : HTTP.c HTTP.h HTParse.h HTUtils.h tcp.h
	cc/debug $(CFLAGS)/obj=$*.obj HTTP.c

HTBufferFile.obj   :  HTBufferFile.c HTBufferFile.h tcp.h - 
                      HTAccess.h HTShort.h
        cc $(CFLAGS)/obj=$*.obj HTBufferFile.c
HTBufferFile_d.obj : HTBufferFile.c HTBufferFile.h tcp.h - 
                     HTAccess.h HTShort.h
        cc/debug $(CFLAGS)/obj=$*.obj HTBufferFile.c

HTFormatText.obj   : HTFormatText.c HTFormatText.h HTUtils.h tcp.h HTShort.h
        cc $(CFLAGS)/obj=$*.obj HTFormatText.c
HTFormatText_d.obj : HTFormatText.c HTFormatText.h HTUtils.h tcp.h HTShort.h
        cc/debug $(CFLAGS)/obj=$*.obj HTFormatText.c

HTFTP.obj   : HTFTP.c HTFTP.h HTParse.h HTUtils.h tcp.h HTTCP.h
        cc $(CFLAGS)/obj=$*.obj HTFTP.c
HTFTP_d.obj : HTFTP.c HTFTP.h HTParse.h HTUtils.h tcp.h HTTCP.h 
        cc/debug $(CFLAGS)/obj=$*.obj HTFTP.c
 
HTFile.obj   : HTFile.c HTFile.h HTUtils.h WWW.h HTParse.h tcp.h - 
               HTTCP.h HTFTP.h 
         cc $(CFLAGS)/obj=$*.obj HTFile.c
HTFile_d.obj : HTFile.c HTFile.h HTUtils.h WWW.h HTParse.h tcp.h -
               HTTCP.h HTFTP.h
         cc/debug $(CFLAGS)/obj=$*.obj HTFile.c
 
HTTCP.obj : HTTCP.c HTTCP.h HTUtils.h tcp.h
         cc $(CFLAGS)/obj=$*.obj HTTCP.c
HTTCP_d.obj : HTTCP.c HTTCP.h HTUtils.h tcp.h
         cc/debug $(CFLAGS)/obj=$*.obj HTTCP.c
  

!_______________________________________________________________
! If files have been updated on PRIAM copy over

setup.com : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/vms/setup.com"
	copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/vms/setup.com" -
	setup.com

www.c  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/www.c"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/www.c" - 
             www.c
multinet.opt  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/vms/multinet.opt"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/vms/multinet.opt" - 
             multinet.opt
win_tcp.opt  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/vms/win_tcp.opt"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/vms/win_tcp.opt" - 
             win_tcp.opt
HTFormatText.c  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTFormatText.c"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTFormatText.c" - 
             HTFormatText.c
HTFormatText.h  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTFormatText.h"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTFormatText.h" - 
             HTFormatText.h
HTBufferFile.h  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTBufferFile.h"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTBufferFile.h" - 
             HTBufferFile.h
HTBufferFile.c  : priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTBufferFile.c"
	     copy priam::"/userd/tbl/hypertext/WWW/LineMode/Implementation/HTBufferFile.c" - 
             HTBufferFile.c
HTParse.c  : priam::"/userd/tbl/hypertext/WWW/Implementation/HTParse.c"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTParse.c" - 
             HTParse.c
HTParse.h  : priam::"/userd/tbl/hypertext/WWW/Implementation/HTParse.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTParse.h" -
             HTParse.h
HTAccess.c : priam::"/userd/tbl/hypertext/WWW/Implementation/HTAccess.c"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTAccess.c" - 
             HTAccess.c
HTAccess.h : priam::"/userd/tbl/hypertext/WWW/Implementation/HTAccess.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTAccess.h" -
             HTAccess.h
HTTP.c     : priam::"/userd/tbl/hypertext/WWW/Implementation/HTTP.c"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTTP.c" -
             HTTP.c
HTTP.h     : priam::"/userd/tbl/hypertext/WWW/Implementation/HTTP.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTTP.h" -
             HTTP.h 
HTUtils.h  : priam::"/userd/tbl/hypertext/WWW/Implementation/HTUtils.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTUtils.h" -
             HTUtils.h
HTFTP.c    : priam::"/userd/tbl/hypertext/WWW/Implementation/HTFTP.c"
             copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTFTP.c" -
             HTFTP.c
HTFTP.h    : priam::"/userd/tbl/hypertext/WWW/Implementation/HTFTP.h"
             copy priam::"/userd/tbl/ hypertext/WWW/Implementation/HTFTP.h" - 
             HTFTP.h
TCP.c      : priam::"/userd/tbl/hypertext/WWW/Implementation/tcp.c"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/tcp.c" - 
             tcp.c
TCP.h      : priam::"/userd/tbl/hypertext/WWW/Implementation/tcp.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/tcp.h" - 
             tcp.h
HTTCP.c    : priam::"/userd/tbl/hypertext/WWW/Implementation/HTTCP.c"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTTCP.c" - 
             HTTCP.c
HTTCP.h    : priam::"/userd/tbl/hypertext/WWW/Implementation/HTTCP.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTTCP.h" - 
             HTTCP.h
HTFile.c   : priam::"/userd/tbl/hypertext/WWW/Implementation/HTFile.c"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTFile.c" - 
             HTFile.c
HTFile.h   : priam::"/userd/tbl/hypertext/WWW/Implementation/HTFile.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTFile.h" -
             HTFile.h
WWW.h      : priam::"/userd/tbl/hypertext/WWW/Implementation/WWW.h"
	     copy priam::"/userd/tbl/hypertext/WWW/Implementation/WWW.h" -
             WWW.h
HTShort.h  : priam::"/userd/tbl/hypertext/WWW/Implementation/HTShort.h"
             copy priam::"/userd/tbl/hypertext/WWW/Implementation/HTShort.h" -
             HTShort.h






