www = @WWW@
wwwdisk = @WWWDISK@

wwwdir = ${wwwdisk}/kml

www_DATA = \
	radar.kml

EXTRA_DIST = \
	radar.kml.in
	
do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
radar.kml: radar.kml.in Makefile
	$(do_subst) < $(srcdir)/radar.kml.in > radar.kml
