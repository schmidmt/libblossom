if ENABLE_TESTS

TESTS = check_bloom check_bitvector
check_PROGRAMS = check_bloom check_bitvector

check_bloom_SOURCES = check_bloom.c $(top_builddir)/src/bloom.h
check_bloom_CFLAGS = @CHECK_CFLAGS@
check_bloom_LDADD = $(top_builddir)/src/libblossom.la @CHECK_LIBS@

check_bitvector_SOURCES = check_bitvector.c $(top_builddir)/src/bitvector.h
check_bitvector_CFLAGS = @CHECK_CFLAGS@
check_bitvector_LDADD = @CHECK_LIBS@

else

check:
	echo "Testing not enabled (run ./configure --enable-tests)"; false

endif
