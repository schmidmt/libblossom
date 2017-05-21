lib_LTLIBRARIES = libblossom.la
libblossom_la_SOURCES = bloom.c blossom.h ../reqs/murmur3/murmur3.c
libblossom_la_LIBADD = -lm
include_HEADERS = blossom.h bitvector.h


@CODE_COVERAGE_RULES@
libblossom_la_LIBS = $(CODE_COVERAGE_LIBS)
libblossom_la_CPPFLAGS = $(CODE_COVERAGE_CPPFLAGS)
libblossom_la_CFLAGS = $(CODE_COVERAGE_CFLAGS)
libblossom_la_CXXFLAGS = $(CODE_COVERAGE_CXXFLAGS)
