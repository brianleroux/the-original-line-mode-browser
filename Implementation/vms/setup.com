$ !
$ !   Set up WorldWideWeb line mode browser and a few aliass
$ !
$ ! WWW_HOME is the default start page if no parameter is given.
$ define/nolog WWW_HOME "http://info.cern.ch./hypertext/WWW/LineMode/Defaults/default.html"
$ define/nolog cernvax priam::
$ www    == "$"+f$parse("www.exe.0", f$environment("procedure"))
$!
$ xfind :== 'www' """http://crnvmc.cern.ch./FIND"""
$ xnews :== 'www' """http://crnvmc.cern.ch./NEWS"""
$ stu*dent :== 'www' """http://crnvmc.cern.ch./NEWS/student"""
$ yellow :== 'www' """http://crnvmc.cern.ch./FIND/yellow"""
$ rpcu*ser :== 'www' """http://info.cern.ch./rpc/doc/User/UserGuide.html"""
$ !
