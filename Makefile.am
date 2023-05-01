## Process this file with automake to produce Makefile.in

NODOCSUBDIRS = lib include/gigatraj src test
SUBDIRS = lib include/gigatraj src test doc

nodoc: 
	for MDIR in $(NODOCSUBDIRS) ; do $(MAKE) $(AM_MAKEFLAGS) -C $${MDIR} ; done
	@echo "Nodoc done"

    
