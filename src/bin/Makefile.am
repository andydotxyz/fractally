MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = fractally

AM_CPPFLAGS = -DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
-I$(top_builddir)/src/bin/ \
-I$(top_srcdir)/src/bin/ \
-I$(top_builddir)/src/lib/ \
-I$(top_srcdir)/src/lib/ \
@EFL_CFLAGS@

fractally_SOURCES = \
  fractally_render.c \
  fractally_main.c
fractally_LDADD = @EFL_LIBS@

localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

EXTRA_DIST = fractally_private.h

