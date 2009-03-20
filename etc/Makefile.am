## File created by the gnome-build tools



sysconf_DATA = \
	pqact.libNIDS.conf

CLEANFILES = $(sysconf_DATA)

EXTRA_DIST = \
	pqact.libNIDS.conf.in

do_subst = sed -e 's,[@]bindir[@],$(bindir),g'

pqact.libNIDS.conf: pqact.libNIDS.conf.in Makefile
	$(do_subst) < $(srcdir)/pqact.libNIDS.conf.in > pqact.libNIDS.conf
