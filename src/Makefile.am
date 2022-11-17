
AM_CPPFLAGS  = -I$(top_srcdir)/include/
LDADD = -lgigatraj
AM_LDFLAGS = -L ../lib/

# We create this file when any of the tools is made.
# Another makefile in the doc subdirectory uses
# this to decide whether to re-make the documentation
TRIGGER_DOCS_REMAKE = ../doc/.remake_trigger
noinst_DATA = $(TRIGGER_DOCS_REMAKE)
EXTRA_DIST = tools.dox

$(TRIGGER_DOCS_REMAKE): tools.dox \
                        gt_fill_met_cache.cc \
                        gt_generate_parcels.cc \
                        gtmodel_s01.cc \
                        gtmodel_s02.cc 
	touch $@

bin_PROGRAMS = gt_fill_met_cache \
               gt_generate_parcels \
               gtmodel_s01 \
               gtmodel_s02
dist_bin_SCRIPTS = gtmodel_m01 gtmodel_m02

gt_fill_met_cache_SOURCES = gt_fill_met_cache.cc
gt_fill_met_cache_DEPENDENCIES = ../lib/libgigatraj.a

gt_generate_parcels_SOURCES = gt_generate_parcels.cc
gt_generate_parcels_DEPENDENCIES = ../lib/libgigatraj.a

gtmodel_s01_SOURCES = gtmodel_s01.cc
gtmodel_s01_DEPENDENCIES = ../lib/libgigatraj.a

gtmodel_s02_SOURCES = gtmodel_s02.cc
gtmodel_s02_DEPENDENCIES = ../lib/libgigatraj.a
