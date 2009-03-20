## File created by the gnome-build tools


www = @WWW@
wwwdisk = @WWWDISK@

bin_SCRIPTS = \
	radar.sh

CLEANFILES = $(bin_SCRIPTS)

EXTRA_DIST = \
	radar.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
radar.sh: radar.in Makefile
	$(do_subst) < $(srcdir)/radar.in > radar.sh
	chmod +x radar.sh
