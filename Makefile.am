## Process this file with automake to produce Makefile.in

NODOCSUBDIRS = lib include/gigatraj confiles src test
SUBDIRS = $(NODOCSUBDIRS) doc

nodoc: 
	for MDIR in $(NODOCSUBDIRS) ; do $(MAKE) $(AM_MAKEFLAGS) -C $${MDIR} ; done
	@echo "Nodoc done"

    
