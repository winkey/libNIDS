## File created by the gnome-build tools


www = @WWW@
wwwdisk = @WWWDISK@

bin_SCRIPTS = \
	radar.sh \
	makekey2.sh

CLEANFILES = $(bin_SCRIPTS)

EXTRA_DIST = \
	radar.in \
	makekey2.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g' \
							 -e 's,[@]wwwdisk[@],$(wwwdisk),g' \
							 -e 's,[@]www[@],$(www),g'
     
radar.sh: radar.in Makefile
	$(do_subst) < $(srcdir)/radar.in > radar.sh
	chmod +x radar.sh

makekey2.sh: makekey2.in Makefile
	$(do_subst) < $(srcdir)/makekey2.in > makekey2.sh
	chmod +x makekey2.sh
