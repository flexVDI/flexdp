#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([flexVDI Desktop Protocol], [2.2.3], [devel@flexvdi.com], [flexdp], [https://www.flexvdi.com])
AM_INIT_AUTOMAKE

AC_CONFIG_SRCDIR([flexdp.h])
AC_PROG_INSTALL

# Checks for header files.
AC_CHECK_HEADERS([stdint.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for libraries.
PKG_CHECK_MODULES([SPICE], [spice-protocol >= 0.12])
AC_SUBST([SPICE_CFLAGS])

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([flexdp.pc])
AC_OUTPUT

