#                                               -*- Autoconf -*-
AC_PREREQ([2.64])

AC_INIT([libblossom],
        [m4_esyscmd_s([./get-version])],
        [schmidmt@gmail.com])
AC_CONFIG_SRCDIR([src/bloom.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile tests/Makefile])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign -Wall -Werror tar-ustar subdir-objects])
AM_MAINTAINER_MODE
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES(yes)])
m4_pattern_allow([AM_PROG_AR])
AM_PROG_AR


m4_include([m4/ax_code_coverage.m4])

# Configure libtool
LT_INIT
AC_ENABLE_SHARED
AC_ENABLE_STATIC


# Options for package
AC_ARG_ENABLE([tests], AS_HELP_STRING([--enable-tests], [Enable testing infrastructure]))
AM_CONDITIONAL([ENABLE_TESTS], [test "x$enable_tests" == "xyes"])


#######################
### Checks

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_EGREP

# Check some compiler flags
AX_CFLAGS_GCC_OPTION([-fdiagnostics-color],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-pipe],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wall],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-W],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wextra],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wformat],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wformat-security],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wfatal-errors],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wcast-align],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Winline],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-fstack-protector],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-D_FORTIFY_SOURCE=2],[MORE_CPPFLAGS])
AX_CFLAGS_GCC_OPTION([-Wno-unused-parameter],[MORE_CFLAGS])
AX_CFLAGS_GCC_OPTION([-Wno-missing-field-initializers],[MORE_CFLAGS])

# Check Headers
AC_CHECK_HEADERS([stdatomic.h], [], [AC_MSG_ERROR([No stdatomic.h, must use GCC >= 4.9])])

# Check libraries
dnl AC_CHECK_LIB(rt, sched_get_priority_min)

# Enable _GNU_SOURCE and the like
AX_CFLAGS_GCC_OPTION([-D_GNU_SOURCE],[MORE_CPPFLAGS])    dnl GNU systems (asprintf, ...)
case $host_os in
   solaris*)
     AX_CFLAGS_GCC_OPTION([-D__EXTENSIONS__],[MORE_CPPFLAGS]) dnl (CMSG_*)
     AX_CFLAGS_GCC_OPTION([-D_XPG4_2],[MORE_CPPFLAGS])        dnl (CMSG_*)
     ;;
   hpux*)
     AX_CFLAGS_GCC_OPTION([-D_XOPEN_SOURCE=500],[MORE_CPPFLAGS])      dnl HP-UX
     AX_CFLAGS_GCC_OPTION([-D_XOPEN_SOURCE_EXTENDED],[MORE_CPPFLAGS]) dnl HP-UX
     ;;
esac

AC_CACHE_SAVE

AS_IF([test "x$enable_tests" == "xyes"], [
    PKG_CHECK_MODULES([CHECK], [check >= 0.9.8])
])

# Enable coverage checks
AX_CODE_COVERAGE
AC_SUBST(CODE_COVERAGE_IGNORE_PATTERN, 'reqs/\*')

AC_CACHE_SAVE

AC_SUBST([MORE_CFLAGS])
AC_SUBST([MORE_CPPFLAGS])
AC_SUBST([MORE_LDFLAGS])
AC_OUTPUT

cat <<EOF

------------------ Summary ------------------
 $PACKAGE_NAME version $PACKAGE_VERSION
  Prefix.........: $prefix
  C Compiler.....: $CC $MORE_CFLAGS $MORE_CPPFLAGS $CFLAGS $CPPFLAGS
  Linker.........: $LD $MORE_LDFLAGS $LDFLAGS $LIBS
  Tests Enabled..: $enable_tests
---------------------------------------------

Check the above options and compile with:
 ${MAKE-make}

EOF
