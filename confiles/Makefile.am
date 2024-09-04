
# yes, we can use $(dataroot) here, but this matches
# the definition in gigatraj.hh.in, which cannot use $dataroot.
datadir=$(prefix)/share/$(PACKAGE)/cat

dist_data_DATA =  example_Catalog.cat

if MERRA
   dist_data_DATA += MERRA.cat
endif
if MERRA2
   dist_data_DATA += MERRA2.cat 
endif
if GEOSFP
   dist_data_DATA += GEOSfpAssim.cat 
   dist_data_DATA += GEOSfpFcast.cat 
   dist_data_DATA += GEOSfp.cat 
endif


