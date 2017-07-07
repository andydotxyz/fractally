MAINTAINERCLEANFILES = Makefile.in
CLEANFILES = check-results.xml

if EFL_HAVE_TESTS

check_PROGRAMS = fractally_tests

fractally_tests_SOURCES = test_fractally.c
ractally_tests_CPPFLAGS = -I$(top_builddir)/src/lib/ \
-I$(top_srcdir)/src/lib \
-DPACKAGE_TESTS_DIR=\"$(top_srcdir)/src/tests/\" \
-DPACKAGE_BUILD_DIR=\"`pwd`/$(top_builddir)/src/tests/\" \
@EFL_CFLAGS@ \
@CHECK_CFLAGS@
fractally_tests_LDADD = @CHECK_LIBS@
fractally_tests_DEPENDENCIES =

endif

EXTRA_DIST = test_fractally.c
