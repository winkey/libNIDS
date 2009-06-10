www = @WWW@
wwwdisk = @WWWDISK@
cgidisk = @CGIDISK@

mapdir = ${wwwdisk}/radar

map_DATA = \
	radar.map

EXTRA_DIST = \
	radar.map.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]cgidisk[@],$(cgidisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
radar.map: radar.map.in Makefile
	$(do_subst) < $(srcdir)/radar.map.in > radar.map


