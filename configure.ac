AC_PREREQ([2.69])
AC_INIT([GigaTraj],[0.8],[Leslie.R.Lait@nasa.gov])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_SRCDIR([lib/Parcel.cc])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_PROG_AR([ar])

AC_ARG_ENABLE([double],
[  --enable-double	Use double-precision instead of regular floating-point numbers ],
[case "${enableval}" in
 yes) do_double=true ;;
 no)  do_double=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-double]) ;;
 esac], [do_double=false])
AM_CONDITIONAL([DOUBLE], [test x$do_double = xtrue])
AC_SUBST([DO_DOUBLE],[0])
if test x$do_double = xtrue ; then
AC_SUBST([DO_DOUBLE], [1])
fi

AC_ARG_ENABLE([mpi],
[  --enable-mpi		Construct a version of the library that uses MPI for parallel processing ],
[case "${enableval}" in
 yes) do_mpi=true ;;
 no)  do_mpi=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-mpi]) ;;
 esac], [do_mpi=false])
AM_CONDITIONAL([MPI], [test x$do_mpi = xtrue])
AC_SUBST([DO_MPI],[0])
if test x$do_mpi = xtrue ; then
AC_SUBST([DO_MPI], [1])
fi

AC_ARG_ENABLE([wrap0],
[  --enable-wrap0		by default, set longitudes to wrap at 0 degrees, making a range of 0 to 360 ],
[case "${enableval}" in
 yes) do_wrap0=true ;;
 no)  do_wrap0=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-wrap0]) ;;
 esac], [do_wrap0=false])
AM_CONDITIONAL([WRAP0], [test x$do_wrap0 = xtrue])
AC_SUBST([DO_WRAP0],[0])
if test x$do_wrap0 = xtrue ; then
AC_SUBST([DO_WRAP0], [1])
fi

AC_ARG_ENABLE([wrap180],
[  --enable-wrap180		by default, set longitudes to wrap at 180 degrees, making a range of -180 to 180 ],
[case "${enableval}" in
 yes) do_wrap180=true ;;
 no)  do_wrap180=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-wrap180]) ;;
 esac], [do_wrap180=false])
AM_CONDITIONAL([WRAP180], [test x$do_wrap180 = xtrue])
AC_SUBST([DO_WRAP180],[0])
if test x$do_wrap180 = xtrue ; then
AC_SUBST([DO_WRAP180], [1])
fi

AC_ARG_ENABLE([merra],
[  --enable-merra		Add class for reading NASA's GMAO MERRA meteorological data ],
[case "${enableval}" in
 yes) do_merra=true ;;
 no)  do_merra=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-merra]) ;;
 esac], [do_merra=false])
AM_CONDITIONAL([MERRA], [test x$do_merra = xtrue])
AC_SUBST([DO_MERRA],[0])
if test x$do_merra = xtrue ; then
AC_SUBST([DO_MERRA], [1])
fi

AC_ARG_ENABLE([merra2],
[  --enable-merra2		Add class for reading NASA's GMAO MERRA2 meteorological data ],
[case "${enableval}" in
 yes) do_merra2=true ;;
 no)  do_merra2=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-merra2]) ;;
 esac], [do_merra2=false])
AM_CONDITIONAL([MERRA2], [test x$do_merra2 = xtrue])
AC_SUBST([DO_MERRA2],[0])
if test x$do_merra2 = xtrue ; then
AC_SUBST([DO_MERRA2], [1])
fi

AC_ARG_ENABLE([geosfp],
[  --enable-geosfp		Add class for reading NASA's GMAO GEOS fp meteorological data ],
[case "${enableval}" in
 yes) do_geosfp=true ;;
 no)  do_geosfp=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-geosfp]) ;;
 esac], [do_geosfp=false])
AM_CONDITIONAL([GEOSFP], [test x$do_geosfp = xtrue])
AC_SUBST([DO_GEOSFP],[0])
if test x$do_geosfp = xtrue ; then
AC_SUBST([DO_GEOSFP], [1])
fi

AC_ARG_ENABLE([doxygen],
[  --enable-doxygen		Allows generation of documentation files ],
[case "${enableval}" in
 yes) do_doxygen=true ;;
 no)  do_doxygen=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-doxygen]) ;;
 esac], [do_doxygen=false])
AM_CONDITIONAL([DOXYGEN], [test x$do_doxygen = xtrue])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
# Can we run MPI programs for parallel programming?
if test x$do_mpi = xtrue ; then
   AC_ARG_VAR([MPIRUN], [The program used to run MPI programs (used to run the test suite)])
   AC_PATH_PROG([MPIRUN], [mpirun] )
fi
# Can we run the Doxygen documentation generator software?
if test x$do_doxygen = xtrue ; then

   AC_ARG_VAR([DOXYGENRUN], [The program used to run doxygen])
   AC_PATH_PROG([DOXYGENRUN], [doxygen])
   AS_IF([test "x$DOXYGENRUN" = x],
     [AC_MSG_ERROR([doxygen is enabled but the doxygen command is missing])] )

   # do we have the components for making LaTeX documentation?
   AC_ARG_VAR([DOX_RUN_LATEX], [The program used to run latex])
   AC_PATH_PROG([DOX_RUN_LATEX], [latex] )

   AC_ARG_VAR([DOX_RUN_MAKEINDEX], [The program to make latex indices])
   AC_PATH_PROG([DOX_RUN_MAKEINDEX], [makeindex] )

   AC_CHECK_PROG([DOX_PDFLATEX], [pdflatex], [YES], [NO] )


fi
# Define which forms of documentation Doxygen should generate
AM_CONDITIONAL([DOX_DOES_HTML], [true])
AM_CONDITIONAL([DOX_DOES_LATEX], [test x${DOX_RUN_LATEX} != x ])
AM_CONDITIONAL([DOX_DOES_MAN], [true])
AM_CONDITIONAL([DOX_DOES_RTF], [true])
AM_CONDITIONAL([DOX_DOES_XML], [false])
AM_CONDITIONAL([DOX_DOES_PERLMOD], [false])
AM_CONDITIONAL([DOX_DOES_AUTOGEN], [false])
# The base doxygen config file, doc/gigatraj_base.dconf.in, 
# refers to certain C Macro definitions to determine which
# sections of the documentation are to be generated.
# We set some definitions here to assist:
AS_IF([test "x${do_double}" = xtrue],
   [AC_SUBST([DOXDEF_DOUBLE], [USE_DOUBLE])],
   [AC_SUBST([DOXDEF_DOUBLE], [ ])])
AS_IF([test "x${do_mpi}" = xtrue],
   [AC_SUBST([DOXDEF_MPI], [USE_MPI])],
   [AC_SUBST([DOXDEF_MPI], [ ])])


# If we are using the g++ compiler, then we should turn off
# variable tracking when compiling MERRA_Directory.cc, because
# of the huge number of entries in the MERRA variable set.
# same applies for MERRA2 and GEOSFP
MDIRFLAG=""
if test x$do_merra = xtrue || test x$do_merra2 = xtrue || test x$do_geosfp = xtrue ; then
   case $CXX in
   *g++*) MDIRFLAG="-fno-var-tracking";;
   *);;
   esac
fi

# Checks for header files.
AC_CHECK_HEADER([math.h])
AC_CHECK_HEADERS([stdio.h])
# Do this for malloc/calloc
AC_CHECK_HEADERS([stdlib.h])
AC_CHECK_HEADERS([time.h])
AC_CHECK_HEADERS([fcntl.h])
AC_CHECK_HEADERS([unistd.h])
AC_CHECK_HEADERS([sys/types.h])
AC_CHECK_HEADERS([sys/stat.h])

# check for long arguments
AC_CHECK_HEADERS([getopt.h],[do_longargs=true], [do_longargs=false] )
AM_CONDITIONAL([LONGARGS], [test x$do_longargs = xtrue])
AC_SUBST([DO_LONGARGS], [0])
if test x$do_longargs = xtrue ; then
AC_SUBST([DO_LONGARGS], [1])
fi

#    only check for MPI if we are using MPI
if test x$do_mpi = xtrue ; then
AC_CHECK_HEADERS([mpi.h], [], [AC_MSG_ERROR(no MPI header file was  found)])
AC_SEARCH_LIBS([MPI_Init],[mpi], [], [AC_MSG_ERROR(no MPI library file was found)] )
AC_CHECK_LIB([mpi_cxx], [MPI_Init], [], [AC_MSG_ERROR(no MPI_CXX library file was found)] )
fi

#    check for netcdf v4 if we are using MERRA, MERRAS2, or GEOSFP
if test x$do_merra = xtrue || test x$do_merra2 = xtrue || test x$do_geosfp = xtrue ; then
AC_CHECK_HEADERS([netcdf.h], [], [AC_MSG_ERROR(no NetCDF header file was  found)])
AC_SEARCH_LIBS([nc_put_var1_uint], [netcdf], [], [AC_MSG_ERROR([You need to install netcdf v4 or greater (with dap enabled), or the libdap and libnc-dap libraries from http://opendap.org/download/index.html])])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_FUNC_ALLOCA
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AC_CONFIG_FILES([Makefile
                 include/gigatraj/Makefile
                 include/gigatraj/gigatraj.hh
                 lib/Makefile
                 lib/filters/Makefile
                 lib/metsources/Makefile
                 src/Makefile
                 test/Makefile
                 doc/gigatraj_base.dconf:doc/gigatraj_base.dconf.in
                 doc/latex_yes.dconf:doc/latex_yes.dconf.in
                 doc/Makefile])
AC_OUTPUT
