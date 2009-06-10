www = @WWW@
wwwdisk = @WWWDISK@
cgidisk = @CGIDISK@

cgidir = ${cgidisk}

cgi_DATA = \
	radar \
	radarwrap \
	radar_wms

EXTRA_DIST = \
	radar.in \
	radarwrap.in \
	radar_wms.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]cgidisk[@],$(cgidisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
radar: radar.in Makefile
	$(do_subst) < $(srcdir)/radar.in > radar
	chmod +x radar

radarwrap: radarwrap.in Makefile
	$(do_subst) < $(srcdir)/radarwrap.in > radarwrap
	chmod +x radarwrap

radar_wms: radar_wms.in Makefile
	$(do_subst) < $(srcdir)/radar_wms.in > radar_wms
	chmod +x radar_wms

