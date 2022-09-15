/*
 This file of directory information for the MERRA data has been
 generated automatically from the script catalog_merra.py.
 This file should not be edited by hand.
*/
      cf_names["ducmass25"] = "Dust Column Mass Density - PM 2.5";
      gmao_names["Dust Column Mass Density - PM 2.5"] = "ducmass25";
      gmao_vars["ducmass25"] = new MVarDesc( "ducmass25", 3, "Dust Column Mass Density - PM 2.5", "dust column mass density - pm 2.5 __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["qian"] = new MVarDesc( "qian", 4, "", "mass_fraction_of_convective_cloud_ice_water", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      gmao_vars["nh3smass"] = new MVarDesc( "nh3smass", 3, "", "ammonia surface mass concentration __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["coclnbna"] = new MVarDesc( "coclnbna", 3, "", "co column burden (north american non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["tdrop"] = new MVarDesc( "tdrop", 3, "", "temperature_drop_across_cool_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["tpsnow"] = "surface_temperature_of_snow";
      gmao_names["surface_temperature_of_snow"] = "tpsnow";
      gmao_vars["tpsnow"] = new MVarDesc( "tpsnow", 3, "surface_temperature_of_snow", "surface_temperature_of_snow", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["lwland"] = "Net_longwave_land";
      gmao_names["Net_longwave_land"] = "lwland";
      gmao_vars["lwland"] = new MVarDesc( "lwland", 3, "Net_longwave_land", "net_longwave_land", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["tauhgh"] = "in_cloud_optical_thickness_of_high_clouds(EXPORT)";
      gmao_names["in_cloud_optical_thickness_of_high_clouds(EXPORT)"] = "tauhgh";
      gmao_vars["tauhgh"] = new MVarDesc( "tauhgh", 3, "in_cloud_optical_thickness_of_high_clouds(EXPORT)", "in_cloud_optical_thickness_of_high_clouds(export)", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["oceman"] = "Organic Carbon Anthropogenic Emissions __ENSEMBLE__";
      gmao_names["Organic Carbon Anthropogenic Emissions __ENSEMBLE__"] = "oceman";
      gmao_vars["oceman"] = new MVarDesc( "oceman", 3, "Organic Carbon Anthropogenic Emissions __ENSEMBLE__", "organic carbon anthropogenic emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ts"] = "surface_skin_temperature";
      gmao_names["surface_skin_temperature"] = "ts";
      gmao_vars["ts"] = new MVarDesc( "ts", 3, "surface_skin_temperature", "surface_skin_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["v10m"] = "10-meter_northward_wind";
      gmao_names["10-meter_northward_wind"] = "v10m";
      gmao_vars["v10m"] = new MVarDesc( "v10m", 3, "10-meter_northward_wind", "10-meter_northward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_smp_Nx,tavg3_2d_smp_Nx.%s,0,0,3,3,90" );

      cf_names["dudtana"] = "total_eastward_wind_analysis_tendency";
      gmao_names["total_eastward_wind_analysis_tendency"] = "dudtana";
      gmao_vars["dudtana"] = new MVarDesc( "dudtana", 4, "total_eastward_wind_analysis_tendency", "total_eastward_wind_analysis_tendency", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["suangstr"] = "SO4 Angstrom parameter [470-870 nm] __ENSEMBLE__";
      gmao_names["SO4 Angstrom parameter [470-870 nm] __ENSEMBLE__"] = "suangstr";
      gmao_vars["suangstr"] = new MVarDesc( "suangstr", 3, "SO4 Angstrom parameter [470-870 nm] __ENSEMBLE__", "so4 angstrom parameter [470-870 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["qlls"] = new MVarDesc( "qlls", 4, "", "mass_fraction_of_large_scale_cloud_liquid_water", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["pficu"] = "3D_flux_of_ice_convective_precipitation";
      gmao_names["3D_flux_of_ice_convective_precipitation"] = "pficu";
      gmao_vars["pficu"] = new MVarDesc( "pficu", 4, "3D_flux_of_ice_convective_precipitation", "3d_flux_of_ice_convective_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90" );

      cf_names["ochyphil"] = "Organic Carbon Hydrophobic to Hydrophilic __ENSEMBLE__";
      gmao_names["Organic Carbon Hydrophobic to Hydrophilic __ENSEMBLE__"] = "ochyphil";
      gmao_vars["ochyphil"] = new MVarDesc( "ochyphil", 3, "Organic Carbon Hydrophobic to Hydrophilic __ENSEMBLE__", "organic carbon hydrophobic to hydrophilic __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["suscatau"] = "SO4 Scattering AOT [550 nm] __ENSEMBLE__";
      gmao_names["SO4 Scattering AOT [550 nm] __ENSEMBLE__"] = "suscatau";
      gmao_vars["suscatau"] = new MVarDesc( "suscatau", 3, "SO4 Scattering AOT [550 nm] __ENSEMBLE__", "so4 scattering aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dusd003"] = "Dust Sedimentation Bin 003";
      gmao_names["Dust Sedimentation Bin 003"] = "dusd003";
      gmao_vars["dusd003"] = new MVarDesc( "dusd003", 3, "Dust Sedimentation Bin 003", "dust sedimentation bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["delp"] = "pressure_thickness";
      gmao_names["pressure_thickness"] = "delp";
      gmao_vars["delp"] = new MVarDesc( "delp", 4, "pressure_thickness", "pressure_thickness", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_chm_Nv,inst3_3d_chm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Nv,tavg3_3d_lsf_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["dusd001"] = "Dust Sedimentation Bin 001";
      gmao_names["Dust Sedimentation Bin 001"] = "dusd001";
      gmao_vars["dusd001"] = new MVarDesc( "dusd001", 3, "Dust Sedimentation Bin 001", "dust sedimentation bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dusd005"] = "Dust Sedimentation Bin 005";
      gmao_names["Dust Sedimentation Bin 005"] = "dusd005";
      gmao_vars["dusd005"] = new MVarDesc( "dusd005", 3, "Dust Sedimentation Bin 005", "dust sedimentation bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dusd004"] = "Dust Sedimentation Bin 004";
      gmao_names["Dust Sedimentation Bin 004"] = "dusd004";
      gmao_vars["dusd004"] = new MVarDesc( "dusd004", 3, "Dust Sedimentation Bin 004", "dust sedimentation bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dqvdtchm"] = "tendency_of_water_vapor_mixing_ratio_due_to_chemistry";
      gmao_names["tendency_of_water_vapor_mixing_ratio_due_to_chemistry"] = "dqvdtchm";
      gmao_vars["dqvdtchm"] = new MVarDesc( "dqvdtchm", 4, "tendency_of_water_vapor_mixing_ratio_due_to_chemistry", "tendency_of_water_vapor_mixing_ratio_due_to_chemistry", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["tlml"] = "surface_air_temperature";
      gmao_names["surface_air_temperature"] = "tlml";
      gmao_vars["tlml"] = new MVarDesc( "tlml", 3, "surface_air_temperature", "surface_air_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_lfo_Nx,inst1_2d_lfo_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["cldmid"] = "cloud_area_fraction_for_middle_clouds";
      gmao_names["cloud_area_fraction_for_middle_clouds"] = "cldmid";
      gmao_vars["cldmid"] = new MVarDesc( "cldmid", 3, "cloud_area_fraction_for_middle_clouds", "cloud_area_fraction_for_middle_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["omega500"] = "omega_at_500_hPa";
      gmao_names["omega_at_500_hPa"] = "omega500";
      gmao_vars["omega500"] = new MVarDesc( "omega500", 3, "omega_at_500_hPa", "omega_at_500_hpa", "Pa s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["pfllsan"] = "3D_flux_of_liquid_nonconvective_precipitation";
      gmao_names["3D_flux_of_liquid_nonconvective_precipitation"] = "pfllsan";
      gmao_vars["pfllsan"] = new MVarDesc( "pfllsan", 4, "3D_flux_of_liquid_nonconvective_precipitation", "3d_flux_of_liquid_nonconvective_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90" );

      cf_names["tskinice"] = "sea_ice_skin_temperature";
      gmao_names["sea_ice_skin_temperature"] = "tskinice";
      gmao_vars["tskinice"] = new MVarDesc( "tskinice", 3, "sea_ice_skin_temperature", "sea_ice_skin_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["ocem002"] = "Organic Carbon Emission Bin 002 __ENSEMBLE__";
      gmao_names["Organic Carbon Emission Bin 002 __ENSEMBLE__"] = "ocem002";
      gmao_vars["ocem002"] = new MVarDesc( "ocem002", 3, "Organic Carbon Emission Bin 002 __ENSEMBLE__", "organic carbon emission bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ocem001"] = "Organic Carbon Emission Bin 001 __ENSEMBLE__";
      gmao_names["Organic Carbon Emission Bin 001 __ENSEMBLE__"] = "ocem001";
      gmao_vars["ocem001"] = new MVarDesc( "ocem001", 3, "Organic Carbon Emission Bin 001 __ENSEMBLE__", "organic carbon emission bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dqrcu"] = "convective_rainwater_source";
      gmao_names["convective_rainwater_source"] = "dqrcu";
      gmao_vars["dqrcu"] = new MVarDesc( "dqrcu", 4, "convective_rainwater_source", "convective_rainwater_source", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90" );

      cf_names["t850"] = "air_temperature_at_850_hPa";
      gmao_names["air_temperature_at_850_hPa"] = "t850";
      gmao_vars["t850"] = new MVarDesc( "t850", 3, "air_temperature_at_850_hPa", "air_temperature_at_850_hpa", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["dqidtdyn"] = "tendency_of_ice_water_due_to_dynamics";
      gmao_names["tendency_of_ice_water_due_to_dynamics"] = "dqidtdyn";
      gmao_vars["dqidtdyn"] = new MVarDesc( "dqidtdyn", 4, "tendency_of_ice_water_due_to_dynamics", "tendency_of_ice_water_due_to_dynamics", "kg/kg/s", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["cosc"] = "CO Surface Concentration in ppbv __ENSEMBLE__";
      gmao_names["CO Surface Concentration in ppbv __ENSEMBLE__"] = "cosc";
      gmao_vars["cosc"] = new MVarDesc( "cosc", 3, "CO Surface Concentration in ppbv __ENSEMBLE__", "co surface concentration in ppbv __ensemble__", "1e-9", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["qv10m"] = "10-meter_specific_humidity";
      gmao_names["10-meter_specific_humidity"] = "qv10m";
      gmao_vars["qv10m"] = new MVarDesc( "qv10m", 3, "10-meter_specific_humidity", "10-meter_specific_humidity", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["t250"] = "air_temperature_at_250_hPa";
      gmao_names["air_temperature_at_250_hPa"] = "t250";
      gmao_vars["t250"] = new MVarDesc( "t250", 3, "air_temperature_at_250_hPa", "air_temperature_at_250_hpa", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["dvdtmst"] = "meridional_wind_tendency_due_to_moist";
      gmao_names["meridional_wind_tendency_due_to_moist"] = "dvdtmst";
      gmao_vars["dvdtmst"] = new MVarDesc( "dvdtmst", 4, "meridional_wind_tendency_due_to_moist", "meridional_wind_tendency_due_to_moist", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["cobbna"] = new MVarDesc( "cobbna", 4, "", "co north america biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["t500"] = "air_temperature_at_500_hPa";
      gmao_names["air_temperature_at_500_hPa"] = "t500";
      gmao_vars["t500"] = new MVarDesc( "t500", 3, "air_temperature_at_500_hPa", "air_temperature_at_500_hpa", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["cosz"] = new MVarDesc( "cosz", 3, "", "cosine_of_the_solar_zenith_angle", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["t2m"] = "2-meter_air_temperature";
      gmao_names["2-meter_air_temperature"] = "t2m";
      gmao_vars["t2m"] = new MVarDesc( "t2m", 3, "2-meter_air_temperature", "2-meter_air_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_smp_Nx,inst3_2d_smp_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["ocwt001"] = "Organic Carbon Wet Deposition Bin 001 __ENSEMBLE__";
      gmao_names["Organic Carbon Wet Deposition Bin 001 __ENSEMBLE__"] = "ocwt001";
      gmao_vars["ocwt001"] = new MVarDesc( "ocwt001", 3, "Organic Carbon Wet Deposition Bin 001 __ENSEMBLE__", "organic carbon wet deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssem005"] = "Sea Salt Emission Bin 005";
      gmao_names["Sea Salt Emission Bin 005"] = "ssem005";
      gmao_vars["ssem005"] = new MVarDesc( "ssem005", 3, "Sea Salt Emission Bin 005", "sea salt emission bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["precsno"] = "snowfall";
      gmao_names["snowfall"] = "precsno";
      gmao_vars["precsno"] = new MVarDesc( "precsno", 3, "snowfall", "snowfall", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["twlt"] = "surface_temperature_of_wilted_zone";
      gmao_names["surface_temperature_of_wilted_zone"] = "twlt";
      gmao_vars["twlt"] = new MVarDesc( "twlt", 3, "surface_temperature_of_wilted_zone", "surface_temperature_of_wilted_zone", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["dtdtmst"] = "tendency_of_air_temperature_due_to_moist_processes";
      gmao_names["tendency_of_air_temperature_due_to_moist_processes"] = "dtdtmst";
      gmao_vars["dtdtmst"] = new MVarDesc( "dtdtmst", 4, "tendency_of_air_temperature_due_to_moist_processes", "tendency_of_air_temperature_due_to_moist_processes", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      gmao_vars["langm"] = new MVarDesc( "langm", 3, "", "langmuir_number", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["tsoil3"] = "soil_temperatures_layer_3";
      gmao_names["soil_temperatures_layer_3"] = "tsoil3";
      gmao_vars["tsoil3"] = new MVarDesc( "tsoil3", 3, "soil_temperatures_layer_3", "soil_temperatures_layer_3", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["hfluxice"] = "sea_ice_upward_sensible_heat_flux";
      gmao_names["sea_ice_upward_sensible_heat_flux"] = "hfluxice";
      gmao_vars["hfluxice"] = new MVarDesc( "hfluxice", 3, "sea_ice_upward_sensible_heat_flux", "sea_ice_upward_sensible_heat_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["v"] = "northward_wind";
      gmao_names["northward_wind"] = "v";
      gmao_vars["v"] = new MVarDesc( "v", 4, "northward_wind", "northward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["dwarm"] = new MVarDesc( "dwarm", 3, "", "depth_at_base_of_warm_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["dtdtana"] = "total_temperature_analysis_tendency";
      gmao_names["total_temperature_analysis_tendency"] = "dtdtana";
      gmao_vars["dtdtana"] = new MVarDesc( "dtdtana", 4, "total_temperature_analysis_tendency", "total_temperature_analysis_tendency", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["bcsmass"] = "Black Carbon Surface Mass Concentration";
      gmao_names["Black Carbon Surface Mass Concentration"] = "bcsmass";
      gmao_vars["bcsmass"] = new MVarDesc( "bcsmass", 3, "Black Carbon Surface Mass Concentration", "black carbon surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["coscnbgl"] = new MVarDesc( "coscnbgl", 3, "", "co surface concentration in ppbv (global non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["bcexttau"] = "Black Carbon Extinction AOT [550 nm]";
      gmao_names["Black Carbon Extinction AOT [550 nm]"] = "bcexttau";
      gmao_vars["bcexttau"] = new MVarDesc( "bcexttau", 3, "Black Carbon Extinction AOT [550 nm]", "black carbon extinction aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["bchyphil"] = "Black Carbon Hydrophobic to Hydrophilic";
      gmao_names["Black Carbon Hydrophobic to Hydrophilic"] = "bchyphil";
      gmao_vars["bchyphil"] = new MVarDesc( "bchyphil", 3, "Black Carbon Hydrophobic to Hydrophilic", "black carbon hydrophobic to hydrophilic __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ss001"] = "Sea Salt Mixing Ratio (bin 001)";
      gmao_names["Sea Salt Mixing Ratio (bin 001)"] = "ss001";
      gmao_vars["ss001"] = new MVarDesc( "ss001", 4, "Sea Salt Mixing Ratio (bin 001)", "sea salt mixing ratio (bin 001)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["prectot"] = "total_precipitation";
      gmao_names["total_precipitation"] = "prectot";
      gmao_vars["prectot"] = new MVarDesc( "prectot", 3, "total_precipitation", "total_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_smp_Nx,tavg3_2d_smp_Nx.%s,0,0,3,3,90" );

      cf_names["bcwt002"] = "Black Carbon Wet Deposition Bin 002";
      gmao_names["Black Carbon Wet Deposition Bin 002"] = "bcwt002";
      gmao_vars["bcwt002"] = new MVarDesc( "bcwt002", 3, "Black Carbon Wet Deposition Bin 002", "black carbon wet deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bcwt001"] = "Black Carbon Wet Deposition Bin 001";
      gmao_names["Black Carbon Wet Deposition Bin 001"] = "bcwt001";
      gmao_vars["bcwt001"] = new MVarDesc( "bcwt001", 3, "Black Carbon Wet Deposition Bin 001", "black carbon wet deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sssd005"] = "Sea Salt Sedimentation Bin 005";
      gmao_names["Sea Salt Sedimentation Bin 005"] = "sssd005";
      gmao_vars["sssd005"] = new MVarDesc( "sssd005", 3, "Sea Salt Sedimentation Bin 005", "sea salt sedimentation bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sssd002"] = "Sea Salt Sedimentation Bin 002";
      gmao_names["Sea Salt Sedimentation Bin 002"] = "sssd002";
      gmao_vars["sssd002"] = new MVarDesc( "sssd002", 3, "Sea Salt Sedimentation Bin 002", "sea salt sedimentation bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["gwettop"] = "surface_soil_wetness";
      gmao_names["surface_soil_wetness"] = "gwettop";
      gmao_vars["gwettop"] = new MVarDesc( "gwettop", 3, "surface_soil_wetness", "surface_soil_wetness", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_smp_Nx,inst3_2d_smp_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["hfluxwtr"] = "open_water_upward_sensible_heat_flux";
      gmao_names["open_water_upward_sensible_heat_flux"] = "hfluxwtr";
      gmao_vars["hfluxwtr"] = new MVarDesc( "hfluxwtr", 3, "open_water_upward_sensible_heat_flux", "open_water_upward_sensible_heat_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["sssd001"] = "Sea Salt Sedimentation Bin 001";
      gmao_names["Sea Salt Sedimentation Bin 001"] = "sssd001";
      gmao_vars["sssd001"] = new MVarDesc( "sssd001", 3, "Sea Salt Sedimentation Bin 001", "sea salt sedimentation bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dtdtrad"] = "tendency_of_air_temperature_due_to_radiation";
      gmao_names["tendency_of_air_temperature_due_to_radiation"] = "dtdtrad";
      gmao_vars["dtdtrad"] = new MVarDesc( "dtdtrad", 4, "tendency_of_air_temperature_due_to_radiation", "tendency_of_air_temperature_due_to_radiation", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["runoff"] = "overland_runoff_including_throughflow";
      gmao_names["overland_runoff_including_throughflow"] = "runoff";
      gmao_vars["runoff"] = new MVarDesc( "runoff", 3, "overland_runoff_including_throughflow", "overland_runoff_including_throughflow", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["ssdp004"] = "Sea Salt Dry Deposition Bin 004";
      gmao_names["Sea Salt Dry Deposition Bin 004"] = "ssdp004";
      gmao_vars["ssdp004"] = new MVarDesc( "ssdp004", 3, "Sea Salt Dry Deposition Bin 004", "sea salt dry deposition bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["nisd003"] = new MVarDesc( "nisd003", 3, "", "nitrate sedimentation bin 003 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["nisd002"] = new MVarDesc( "nisd002", 3, "", "nitrate sedimentation bin 002 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssdp001"] = "Sea Salt Dry Deposition Bin 001";
      gmao_names["Sea Salt Dry Deposition Bin 001"] = "ssdp001";
      gmao_vars["ssdp001"] = new MVarDesc( "ssdp001", 3, "Sea Salt Dry Deposition Bin 001", "sea salt dry deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["vlml"] = "surface_northward_wind";
      gmao_names["surface_northward_wind"] = "vlml";
      gmao_vars["vlml"] = new MVarDesc( "vlml", 3, "surface_northward_wind", "surface_northward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["ssdp003"] = "Sea Salt Dry Deposition Bin 003";
      gmao_names["Sea Salt Dry Deposition Bin 003"] = "ssdp003";
      gmao_vars["ssdp003"] = new MVarDesc( "ssdp003", 3, "Sea Salt Dry Deposition Bin 003", "sea salt dry deposition bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["tauyice"] = "northward_stress_over_ice";
      gmao_names["northward_stress_over_ice"] = "tauyice";
      gmao_vars["tauyice"] = new MVarDesc( "tauyice", 3, "northward_stress_over_ice", "northward_stress_over_ice", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["preccon"] = "convective_precipitation";
      gmao_names["convective_precipitation"] = "preccon";
      gmao_vars["preccon"] = new MVarDesc( "preccon", 3, "convective_precipitation", "convective_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["ssaeridx"] = "Sea Salt TOMS UV Aerosol Index";
      gmao_names["Sea Salt TOMS UV Aerosol Index"] = "ssaeridx";
      gmao_vars["ssaeridx"] = new MVarDesc( "ssaeridx", 3, "Sea Salt TOMS UV Aerosol Index", "sea salt toms uv aerosol index __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["frocean"] = new MVarDesc( "frocean", 3, "", "fraction_of_ocean", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["coclbbna"] = new MVarDesc( "coclbbna", 3, "", "co column burden (north america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["so2cmass"] = "SO2 Column Mass Density __ENSEMBLE__";
      gmao_names["SO2 Column Mass Density __ENSEMBLE__"] = "so2cmass";
      gmao_vars["so2cmass"] = new MVarDesc( "so2cmass", 3, "SO2 Column Mass Density __ENSEMBLE__", "so2 column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["evpsoil"] = "baresoil_evap_energy_flux";
      gmao_names["baresoil_evap_energy_flux"] = "evpsoil";
      gmao_vars["evpsoil"] = new MVarDesc( "evpsoil", 3, "baresoil_evap_energy_flux", "baresoil_evap_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["aodinc"] = "Aerosol Optical Depth Analysis Increment";
      gmao_names["Aerosol Optical Depth Analysis Increment"] = "aodinc";
      gmao_vars["aodinc"] = new MVarDesc( "aodinc", 3, "Aerosol Optical Depth Analysis Increment", "aerosol optical depth analysis increment", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_gas_Nx,inst3_2d_gas_Nx.%s,0,0,3,0,0" );

      cf_names["cloud"] = "cloud_fraction_for_radiation";
      gmao_names["cloud_fraction_for_radiation"] = "cloud";
      gmao_vars["cloud"] = new MVarDesc( "cloud", 4, "cloud_fraction_for_radiation", "cloud_fraction_for_radiation", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Cp,tavg3_3d_rad_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90" );

      cf_names["doxdtana"] = "total_ozone_analysis_tendency";
      gmao_names["total_ozone_analysis_tendency"] = "doxdtana";
      gmao_vars["doxdtana"] = new MVarDesc( "doxdtana", 4, "total_ozone_analysis_tendency", "total_ozone_analysis_tendency", "mol mol-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Cp,tavg3_3d_odt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90" );

      cf_names["sudp004"] = "Sulfate Dry Deposition Bin 004 __ENSEMBLE__";
      gmao_names["Sulfate Dry Deposition Bin 004 __ENSEMBLE__"] = "sudp004";
      gmao_vars["sudp004"] = new MVarDesc( "sudp004", 3, "Sulfate Dry Deposition Bin 004 __ENSEMBLE__", "sulfate dry deposition bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sudp003"] = "Sulfate Dry Deposition Bin 003 __ENSEMBLE__";
      gmao_names["Sulfate Dry Deposition Bin 003 __ENSEMBLE__"] = "sudp003";
      gmao_vars["sudp003"] = new MVarDesc( "sudp003", 3, "Sulfate Dry Deposition Bin 003 __ENSEMBLE__", "sulfate dry deposition bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sudp002"] = "Sulfate Dry Deposition Bin 002 __ENSEMBLE__";
      gmao_names["Sulfate Dry Deposition Bin 002 __ENSEMBLE__"] = "sudp002";
      gmao_vars["sudp002"] = new MVarDesc( "sudp002", 3, "Sulfate Dry Deposition Bin 002 __ENSEMBLE__", "sulfate dry deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sudp001"] = "Sulfate Dry Deposition Bin 001 __ENSEMBLE__";
      gmao_names["Sulfate Dry Deposition Bin 001 __ENSEMBLE__"] = "sudp001";
      gmao_vars["sudp001"] = new MVarDesc( "sudp001", 3, "Sulfate Dry Deposition Bin 001 __ENSEMBLE__", "sulfate dry deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["evpsbln"] = "snow_ice_evaporation_energy_flux";
      gmao_names["snow_ice_evaporation_energy_flux"] = "evpsbln";
      gmao_vars["evpsbln"] = new MVarDesc( "evpsbln", 3, "snow_ice_evaporation_energy_flux", "snow_ice_evaporation_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["ssfluxv"] = "Sea Salt column v-wind mass flux";
      gmao_names["Sea Salt column v-wind mass flux"] = "ssfluxv";
      gmao_vars["ssfluxv"] = new MVarDesc( "ssfluxv", 3, "Sea Salt column v-wind mass flux", "sea salt column v-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["coclbbgl"] = new MVarDesc( "coclbbgl", 3, "", "co column burden (global biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["ssfluxu"] = "Sea Salt column u-wind mass flux";
      gmao_names["Sea Salt column u-wind mass flux"] = "ssfluxu";
      gmao_vars["ssfluxu"] = new MVarDesc( "ssfluxu", 3, "Sea Salt column u-wind mass flux", "sea salt column u-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["nh4wt"] = new MVarDesc( "nh4wt", 3, "", "ammonium wet deposition __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["nh4sd"] = new MVarDesc( "nh4sd", 3, "", "ammonium settling __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["lwi"] = "land(1)_water(0)_ice(2)_flag";
      gmao_names["land(1)_water(0)_ice(2)_flag"] = "lwi";
      gmao_vars["lwi"] = new MVarDesc( "lwi", 3, "land(1)_water(0)_ice(2)_flag", "land(1)_water(0)_ice(2)_flag", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["ulml"] = "surface_eastward_wind";
      gmao_names["surface_eastward_wind"] = "ulml";
      gmao_vars["ulml"] = new MVarDesc( "ulml", 3, "surface_eastward_wind", "surface_eastward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["frccn"] = "areal_fraction_of_convective_showers";
      gmao_names["areal_fraction_of_convective_showers"] = "frccn";
      gmao_vars["frccn"] = new MVarDesc( "frccn", 3, "areal_fraction_of_convective_showers", "areal_fraction_of_convective_showers", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["nh4sv"] = new MVarDesc( "nh4sv", 3, "", "ammonium convective scavenging __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["lwgntice"] = "sea_ice_net_downward_longwave_flux";
      gmao_names["sea_ice_net_downward_longwave_flux"] = "lwgntice";
      gmao_vars["lwgntice"] = new MVarDesc( "lwgntice", 3, "sea_ice_net_downward_longwave_flux", "sea_ice_net_downward_longwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["msa"] = "Methanesulphonic acid";
      gmao_names["Methanesulphonic acid"] = "msa";
      gmao_vars["msa"] = new MVarDesc( "msa", 4, "Methanesulphonic acid", "methanesulphonic acid", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["nipnh4aq"] = new MVarDesc( "nipnh4aq", 3, "", "ammonium production from aqueous chemistry __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bccmass"] = "Black Carbon Column Mass Density";
      gmao_names["Black Carbon Column Mass Density"] = "bccmass";
      gmao_vars["bccmass"] = new MVarDesc( "bccmass", 3, "Black Carbon Column Mass Density", "black carbon column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["bcool"] = new MVarDesc( "bcool", 3, "", "buoyancy_generation_in_cool_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["v850"] = "northward_wind_at_850_hPa";
      gmao_names["northward_wind_at_850_hPa"] = "v850";
      gmao_vars["v850"] = new MVarDesc( "v850", 3, "northward_wind_at_850_hPa", "northward_wind_at_850_hpa", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["suwt001"] = "Sulfate Wet Deposition Bin 001 __ENSEMBLE__";
      gmao_names["Sulfate Wet Deposition Bin 001 __ENSEMBLE__"] = "suwt001";
      gmao_vars["suwt001"] = new MVarDesc( "suwt001", 3, "Sulfate Wet Deposition Bin 001 __ENSEMBLE__", "sulfate wet deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["suwt002"] = "Sulfate Wet Deposition Bin 002 __ENSEMBLE__";
      gmao_names["Sulfate Wet Deposition Bin 002 __ENSEMBLE__"] = "suwt002";
      gmao_vars["suwt002"] = new MVarDesc( "suwt002", 3, "Sulfate Wet Deposition Bin 002 __ENSEMBLE__", "sulfate wet deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["suwt003"] = "Sulfate Wet Deposition Bin 003 __ENSEMBLE__";
      gmao_names["Sulfate Wet Deposition Bin 003 __ENSEMBLE__"] = "suwt003";
      gmao_vars["suwt003"] = new MVarDesc( "suwt003", 3, "Sulfate Wet Deposition Bin 003 __ENSEMBLE__", "sulfate wet deposition bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["suwt004"] = "Sulfate Wet Deposition Bin 004 __ENSEMBLE__";
      gmao_names["Sulfate Wet Deposition Bin 004 __ENSEMBLE__"] = "suwt004";
      gmao_vars["suwt004"] = new MVarDesc( "suwt004", 3, "Sulfate Wet Deposition Bin 004 __ENSEMBLE__", "sulfate wet deposition bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ducmass"] = "Dust Column Mass Density";
      gmao_names["Dust Column Mass Density"] = "ducmass";
      gmao_vars["ducmass"] = new MVarDesc( "ducmass", 3, "Dust Column Mass Density", "dust column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dudtdyn"] = "tendency_of_eastward_wind_due_to_dynamics";
      gmao_names["tendency_of_eastward_wind_due_to_dynamics"] = "dudtdyn";
      gmao_vars["dudtdyn"] = new MVarDesc( "dudtdyn", 4, "tendency_of_eastward_wind_due_to_dynamics", "tendency_of_eastward_wind_due_to_dynamics", "m/s/s", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["supso4g"] = "SO4 Prod from Gaseous SO2 Oxidation [column] __ENSEMBLE__";
      gmao_names["SO4 Prod from Gaseous SO2 Oxidation [column] __ENSEMBLE__"] = "supso4g";
      gmao_vars["supso4g"] = new MVarDesc( "supso4g", 3, "SO4 Prod from Gaseous SO2 Oxidation [column] __ENSEMBLE__", "so4 prod from gaseous so2 oxidation [column] __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["lwtupclrcln"] = "upwelling_longwave_flux_at_toa_assuming_clear_sky_and_no_aerosol";
      gmao_names["upwelling_longwave_flux_at_toa_assuming_clear_sky_and_no_aerosol"] = "lwtupclrcln";
      gmao_vars["lwtupclrcln"] = new MVarDesc( "lwtupclrcln", 3, "upwelling_longwave_flux_at_toa_assuming_clear_sky_and_no_aerosol", "upwelling_longwave_flux_at_toa_assuming_clear_sky_and_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["cfcu"] = "updraft_areal_fraction";
      gmao_names["updraft_areal_fraction"] = "cfcu";
      gmao_vars["cfcu"] = new MVarDesc( "cfcu", 4, "updraft_areal_fraction", "updraft_areal_fraction", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["disph"] = "zero_plane_displacement_height";
      gmao_names["zero_plane_displacement_height"] = "disph";
      gmao_vars["disph"] = new MVarDesc( "disph", 3, "zero_plane_displacement_height", "zero_plane_displacement_height", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["u250"] = "eastward_wind_at_250_hPa";
      gmao_names["eastward_wind_at_250_hPa"] = "u250";
      gmao_vars["u250"] = new MVarDesc( "u250", 3, "eastward_wind_at_250_hPa", "eastward_wind_at_250_hpa", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["dmssmass"] = "DMS Surface Mass Concentration __ENSEMBLE__";
      gmao_names["DMS Surface Mass Concentration __ENSEMBLE__"] = "dmssmass";
      gmao_vars["dmssmass"] = new MVarDesc( "dmssmass", 3, "DMS Surface Mass Concentration __ENSEMBLE__", "dms surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["totexttau"] = "Total Aerosol Extinction AOT [550 nm]";
      gmao_names["Total Aerosol Extinction AOT [550 nm]"] = "totexttau";
      gmao_vars["totexttau"] = new MVarDesc( "totexttau", 3, "Total Aerosol Extinction AOT [550 nm]", "total aerosol extinction aot [550 nm]", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["sscmass"] = "Sea Salt Column Mass Density";
      gmao_names["Sea Salt Column Mass Density"] = "sscmass";
      gmao_vars["sscmass"] = new MVarDesc( "sscmass", 3, "Sea Salt Column Mass Density", "sea salt column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["du004"] = "Dust Mixing Ratio (bin 004)";
      gmao_names["Dust Mixing Ratio (bin 004)"] = "du004";
      gmao_vars["du004"] = new MVarDesc( "du004", 4, "Dust Mixing Ratio (bin 004)", "dust mixing ratio (bin 004)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["du005"] = "Dust Mixing Ratio (bin 005)";
      gmao_names["Dust Mixing Ratio (bin 005)"] = "du005";
      gmao_vars["du005"] = new MVarDesc( "du005", 4, "Dust Mixing Ratio (bin 005)", "dust mixing ratio (bin 005)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["du002"] = "Dust Mixing Ratio (bin 002)";
      gmao_names["Dust Mixing Ratio (bin 002)"] = "du002";
      gmao_vars["du002"] = new MVarDesc( "du002", 4, "Dust Mixing Ratio (bin 002)", "dust mixing ratio (bin 002)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["du003"] = "Dust Mixing Ratio (bin 003)";
      gmao_names["Dust Mixing Ratio (bin 003)"] = "du003";
      gmao_vars["du003"] = new MVarDesc( "du003", 4, "Dust Mixing Ratio (bin 003)", "dust mixing ratio (bin 003)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["du001"] = "Dust Mixing Ratio (bin 001)";
      gmao_names["Dust Mixing Ratio (bin 001)"] = "du001";
      gmao_vars["du001"] = new MVarDesc( "du001", 4, "Dust Mixing Ratio (bin 001)", "dust mixing ratio (bin 001)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["hflux"] = "sensible_heat_flux_from_turbulence";
      gmao_names["sensible_heat_flux_from_turbulence"] = "hflux";
      gmao_vars["hflux"] = new MVarDesc( "hflux", 3, "sensible_heat_flux_from_turbulence", "sensible_heat_flux_from_turbulence", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["cobbgl"] = new MVarDesc( "cobbgl", 4, "", "co global biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["taux"] = "eastward_surface_stress";
      gmao_names["eastward_surface_stress"] = "taux";
      gmao_vars["taux"] = new MVarDesc( "taux", 3, "eastward_surface_stress", "eastward_surface_stress", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["tauy"] = "northward_surface_stress";
      gmao_names["northward_surface_stress"] = "tauy";
      gmao_vars["tauy"] = new MVarDesc( "tauy", 3, "northward_surface_stress", "northward_surface_stress", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["efluxice"] = "sea_ice_latent_energy_flux";
      gmao_names["sea_ice_latent_energy_flux"] = "efluxice";
      gmao_vars["efluxice"] = new MVarDesc( "efluxice", 3, "sea_ice_latent_energy_flux", "sea_ice_latent_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["qlan"] = new MVarDesc( "qlan", 4, "", "mass_fraction_of_convective_cloud_liquid_water", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      gmao_vars["coclnbas"] = new MVarDesc( "coclnbas", 3, "", "co column burden (asia non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["cldlow"] = "cloud_area_fraction_for_low_clouds";
      gmao_names["cloud_area_fraction_for_low_clouds"] = "cldlow";
      gmao_vars["cldlow"] = new MVarDesc( "cldlow", 3, "cloud_area_fraction_for_low_clouds", "cloud_area_fraction_for_low_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["swgntclrcln"] = "surface_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol";
      gmao_names["surface_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol"] = "swgntclrcln";
      gmao_vars["swgntclrcln"] = new MVarDesc( "swgntclrcln", 3, "surface_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol", "surface_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["susv"] = new MVarDesc( "susv", 3, "", "sulfate_tendency_due_to_conv_scav", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["so4cmass"] = "SO4 Column Mass Density __ENSEMBLE__";
      gmao_names["SO4 Column Mass Density __ENSEMBLE__"] = "so4cmass";
      gmao_vars["so4cmass"] = new MVarDesc( "so4cmass", 3, "SO4 Column Mass Density __ENSEMBLE__", "so4 column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["cfc12strat"] = new MVarDesc( "cfc12strat", 3, "", "stratospheric cfc-12 column", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["so2emve"] = "SO2 Volcanic (explosive) Emissions __ENSEMBLE__";
      gmao_names["SO2 Volcanic (explosive) Emissions __ENSEMBLE__"] = "so2emve";
      gmao_vars["so2emve"] = new MVarDesc( "so2emve", 3, "SO2 Volcanic (explosive) Emissions __ENSEMBLE__", "so2 volcanic (explosive) emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["hlml"] = "surface_layer_height";
      gmao_names["surface_layer_height"] = "hlml";
      gmao_vars["hlml"] = new MVarDesc( "hlml", 3, "surface_layer_height", "surface_layer_height", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_lfo_Nx,inst1_2d_lfo_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["lwtupclr"] = "upwelling_longwave_flux_at_toa_assuming_clear_sky";
      gmao_names["upwelling_longwave_flux_at_toa_assuming_clear_sky"] = "lwtupclr";
      gmao_vars["lwtupclr"] = new MVarDesc( "lwtupclr", 3, "upwelling_longwave_flux_at_toa_assuming_clear_sky", "upwelling_longwave_flux_at_toa_assuming_clear_sky", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["nirdr"] = "surface_downwelling_nearinfrared_beam_flux";
      gmao_names["surface_downwelling_nearinfrared_beam_flux"] = "nirdr";
      gmao_vars["nirdr"] = new MVarDesc( "nirdr", 3, "surface_downwelling_nearinfrared_beam_flux", "surface_downwelling_nearinfrared_beam_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["so2emvn"] = "SO2 Volcanic (non-explosive) Emissions __ENSEMBLE__";
      gmao_names["SO2 Volcanic (non-explosive) Emissions __ENSEMBLE__"] = "so2emvn";
      gmao_vars["so2emvn"] = new MVarDesc( "so2emvn", 3, "SO2 Volcanic (non-explosive) Emissions __ENSEMBLE__", "so2 volcanic (non-explosive) emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["lwgntclr"] = "surface_net_downward_longwave_flux_assuming_clear_sky";
      gmao_names["surface_net_downward_longwave_flux_assuming_clear_sky"] = "lwgntclr";
      gmao_vars["lwgntclr"] = new MVarDesc( "lwgntclr", 3, "surface_net_downward_longwave_flux_assuming_clear_sky", "surface_net_downward_longwave_flux_assuming_clear_sky", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["dtdtswr"] = "air_temperature_tendency_due_to_shortwave";
      gmao_names["air_temperature_tendency_due_to_shortwave"] = "dtdtswr";
      gmao_vars["dtdtswr"] = new MVarDesc( "dtdtswr", 4, "air_temperature_tendency_due_to_shortwave", "air_temperature_tendency_due_to_shortwave", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Cp,tavg3_3d_rad_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90" );

      cf_names["dvdtgwd"] = "tendency_of_northward_wind_due_to_GWD";
      gmao_names["tendency_of_northward_wind_due_to_GWD"] = "dvdtgwd";
      gmao_vars["dvdtgwd"] = new MVarDesc( "dvdtgwd", 4, "tendency_of_northward_wind_due_to_GWD", "tendency_of_northward_wind_due_to_gwd", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["nirdf"] = "surface_downwelling_nearinfrared_diffuse_flux";
      gmao_names["surface_downwelling_nearinfrared_diffuse_flux"] = "nirdf";
      gmao_vars["nirdf"] = new MVarDesc( "nirdf", 3, "surface_downwelling_nearinfrared_diffuse_flux", "surface_downwelling_nearinfrared_diffuse_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["colsbbaf"] = new MVarDesc( "colsbbaf", 3, "", "co chemical loss (africa biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["colsbbae"] = new MVarDesc( "colsbbae", 3, "", "co chemical loss (asia and europe biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["conbgl"] = new MVarDesc( "conbgl", 4, "", "co global non-biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      gmao_vars["tdel"] = new MVarDesc( "tdel", 3, "", "temperature_at_base_of_cool_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["coemnbgl"] = new MVarDesc( "coemnbgl", 3, "", "co emission (global non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["pbltop"] = "pbltop_pressure";
      gmao_names["pbltop_pressure"] = "pbltop";
      gmao_vars["pbltop"] = new MVarDesc( "pbltop", 3, "pbltop_pressure", "pbltop_pressure", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["suinc"] = "Sulfate Mixing Ratio Analysis Increments";
      gmao_names["Sulfate Mixing Ratio Analysis Increments"] = "suinc";
      gmao_vars["suinc"] = new MVarDesc( "suinc", 4, "Sulfate Mixing Ratio Analysis Increments", "sulfate mixing ratio analysis increments", "kg/kg", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0" );

      cf_names["pardr"] = "surface_downwelling_par_beam_flux";
      gmao_names["surface_downwelling_par_beam_flux"] = "pardr";
      gmao_vars["pardr"] = new MVarDesc( "pardr", 3, "surface_downwelling_par_beam_flux", "surface_downwelling_par_beam_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["conbna"] = new MVarDesc( "conbna", 4, "", "co north american non-biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["dusd002"] = "Dust Sedimentation Bin 002";
      gmao_names["Dust Sedimentation Bin 002"] = "dusd002";
      gmao_vars["dusd002"] = new MVarDesc( "dusd002", 3, "Dust Sedimentation Bin 002", "dust sedimentation bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["tbar"] = new MVarDesc( "tbar", 3, "", "mean_temperature_of_interface_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["ts_found"] = new MVarDesc( "ts_found", 3, "", "foundation_temperature_for_interface_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["q500"] = "specific_humidity_at_500_hPa";
      gmao_names["specific_humidity_at_500_hPa"] = "q500";
      gmao_vars["q500"] = new MVarDesc( "q500", 3, "specific_humidity_at_500_hPa", "specific_humidity_at_500_hpa", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["ocinc"] = "Organic Carbon Mixing Ratio Analysis Increments";
      gmao_names["Organic Carbon Mixing Ratio Analysis Increments"] = "ocinc";
      gmao_vars["ocinc"] = new MVarDesc( "ocinc", 4, "Organic Carbon Mixing Ratio Analysis Increments", "organic carbon mixing ratio analysis increments", "kg/kg", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0" );

      cf_names["tsat"] = "surface_temperature_of_saturated_zone";
      gmao_names["surface_temperature_of_saturated_zone"] = "tsat";
      gmao_vars["tsat"] = new MVarDesc( "tsat", 3, "surface_temperature_of_saturated_zone", "surface_temperature_of_saturated_zone", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["pardf"] = "surface_downwelling_par_diffuse_flux";
      gmao_names["surface_downwelling_par_diffuse_flux"] = "pardf";
      gmao_vars["pardf"] = new MVarDesc( "pardf", 3, "surface_downwelling_par_diffuse_flux", "surface_downwelling_par_diffuse_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["ssangstr"] = "Sea Salt Angstrom parameter [470-870 nm]";
      gmao_names["Sea Salt Angstrom parameter [470-870 nm]"] = "ssangstr";
      gmao_vars["ssangstr"] = new MVarDesc( "ssangstr", 3, "Sea Salt Angstrom parameter [470-870 nm]", "sea salt angstrom parameter [470-870 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["pfilsan"] = "3D_flux_of_ice_nonconvective_precipitation";
      gmao_names["3D_flux_of_ice_nonconvective_precipitation"] = "pfilsan";
      gmao_vars["pfilsan"] = new MVarDesc( "pfilsan", 4, "3D_flux_of_ice_nonconvective_precipitation", "3d_flux_of_ice_nonconvective_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90" );

      cf_names["tauxice"] = "eastward_stress_over_ice";
      gmao_names["eastward_stress_over_ice"] = "tauxice";
      gmao_vars["tauxice"] = new MVarDesc( "tauxice", 3, "eastward_stress_over_ice", "eastward_stress_over_ice", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["qwarm"] = new MVarDesc( "qwarm", 3, "", "net_heating_in_warm_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["cols"] = "CO Chemical Loss __ENSEMBLE__";
      gmao_names["CO Chemical Loss __ENSEMBLE__"] = "cols";
      gmao_vars["cols"] = new MVarDesc( "cols", 3, "CO Chemical Loss __ENSEMBLE__", "co chemical loss __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["frsno"] = "fractional_area_of_land_snowcover";
      gmao_names["fractional_area_of_land_snowcover"] = "frsno";
      gmao_vars["frsno"] = new MVarDesc( "frsno", 3, "fractional_area_of_land_snowcover", "fractional_area_of_land_snowcover", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["nismass25"] = new MVarDesc( "nismass25", 3, "", "nitrate surface mass concentration [pm2.5] __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["nh4a"] = new MVarDesc( "nh4a", 4, "", "ammonium ion (nh4+, aerosol phase)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["sufluxv"] = "SO4 column v-wind mass flux __ENSEMBLE__";
      gmao_names["SO4 column v-wind mass flux __ENSEMBLE__"] = "sufluxv";
      gmao_vars["sufluxv"] = new MVarDesc( "sufluxv", 3, "SO4 column v-wind mass flux __ENSEMBLE__", "so4 column v-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["doxdttrb"] = "tendency_of_odd_oxygen_due_to_turbulence";
      gmao_names["tendency_of_odd_oxygen_due_to_turbulence"] = "doxdttrb";
      gmao_vars["doxdttrb"] = new MVarDesc( "doxdttrb", 4, "tendency_of_odd_oxygen_due_to_turbulence", "tendency_of_odd_oxygen_due_to_turbulence", "mol mol-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Cp,tavg3_3d_odt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90" );

      gmao_vars["dtdtfric"] = new MVarDesc( "dtdtfric", 4, "", "tendency_of_air_temperature_due_to_moist_processes_friction", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["ssexttfm"] = "Sea Salt Extinction AOT [550 nm] - PM 1.0 um";
      gmao_names["Sea Salt Extinction AOT [550 nm] - PM 1.0 um"] = "ssexttfm";
      gmao_vars["ssexttfm"] = new MVarDesc( "ssexttfm", 3, "Sea Salt Extinction AOT [550 nm] - PM 1.0 um", "sea salt extinction aot [550 nm] - pm 1.0 um __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["tsh"] = "effective_surface_skin_temperature";
      gmao_names["effective_surface_skin_temperature"] = "tsh";
      gmao_vars["tsh"] = new MVarDesc( "tsh", 3, "effective_surface_skin_temperature", "effective_surface_skin_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["wchange"] = "rate_of_change_of_total_land_water";
      gmao_names["rate_of_change_of_total_land_water"] = "wchange";
      gmao_vars["wchange"] = new MVarDesc( "wchange", 3, "rate_of_change_of_total_land_water", "rate_of_change_of_total_land_water", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["niexttau"] = new MVarDesc( "niexttau", 3, "", "nitrate extinction aot [550 nm] __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["coembbgl"] = new MVarDesc( "coembbgl", 3, "", "co emission (global biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["dqidtmst"] = "total_ice_water_tendency_due_to_moist";
      gmao_names["total_ice_water_tendency_due_to_moist"] = "dqidtmst";
      gmao_vars["dqidtmst"] = new MVarDesc( "dqidtmst", 4, "total_ice_water_tendency_due_to_moist", "total_ice_water_tendency_due_to_moist", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      gmao_vars["cfc12trop"] = new MVarDesc( "cfc12trop", 3, "", "tropospheric cfc-12 column", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["baseflow"] = "baseflow_flux";
      gmao_names["baseflow_flux"] = "baseflow";
      gmao_vars["baseflow"] = new MVarDesc( "baseflow", 3, "baseflow_flux", "baseflow_flux", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["qv2m"] = "2-meter_specific_humidity";
      gmao_names["2-meter_specific_humidity"] = "qv2m";
      gmao_vars["qv2m"] = new MVarDesc( "qv2m", 3, "2-meter_specific_humidity", "2-meter_specific_humidity", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_smp_Nx,inst3_2d_smp_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["u10m"] = "10-meter_eastward_wind";
      gmao_names["10-meter_eastward_wind"] = "u10m";
      gmao_vars["u10m"] = new MVarDesc( "u10m", 3, "10-meter_eastward_wind", "10-meter_eastward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_smp_Nx,tavg3_2d_smp_Nx.%s,0,0,3,3,90" );

      cf_names["swgnt"] = "surface_net_downward_shortwave_flux";
      gmao_names["surface_net_downward_shortwave_flux"] = "swgnt";
      gmao_vars["swgnt"] = new MVarDesc( "swgnt", 3, "surface_net_downward_shortwave_flux", "surface_net_downward_shortwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["lhland"] = "Latent_heat_flux_land";
      gmao_names["Latent_heat_flux_land"] = "lhland";
      gmao_vars["lhland"] = new MVarDesc( "lhland", 3, "Latent_heat_flux_land", "latent_heat_flux_land", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["colsnbgl"] = new MVarDesc( "colsnbgl", 3, "", "co chemical loss (global non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["sssv"] = new MVarDesc( "sssv", 3, "", "sea_salt_tendency_due_to_conv_scav", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["coclbbae"] = new MVarDesc( "coclbbae", 3, "", "co column burden (asia and europe biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["taumid"] = "in_cloud_optical_thickness_of_middle_clouds";
      gmao_names["in_cloud_optical_thickness_of_middle_clouds"] = "taumid";
      gmao_vars["taumid"] = new MVarDesc( "taumid", 3, "in_cloud_optical_thickness_of_middle_clouds", "in_cloud_optical_thickness_of_middle_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["duaeridx"] = "Dust TOMS UV Aerosol Index";
      gmao_names["Dust TOMS UV Aerosol Index"] = "duaeridx";
      gmao_vars["duaeridx"] = new MVarDesc( "duaeridx", 3, "Dust TOMS UV Aerosol Index", "dust toms uv aerosol index __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["cmfmc"] = "cumulative_mass_flux";
      gmao_names["cumulative_mass_flux"] = "cmfmc";
      gmao_vars["cmfmc"] = new MVarDesc( "cmfmc", 4, "cumulative_mass_flux", "cumulative_mass_flux", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90" );

      cf_names["cldtot"] = "total_cloud_area_fraction";
      gmao_names["total_cloud_area_fraction"] = "cldtot";
      gmao_vars["cldtot"] = new MVarDesc( "cldtot", 3, "total_cloud_area_fraction", "total_cloud_area_fraction", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["no3an1"] = new MVarDesc( "no3an1", 4, "", "nitrate size bin 001", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["no3an3"] = new MVarDesc( "no3an3", 4, "", "nitrate size bin 003", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["no3an2"] = new MVarDesc( "no3an2", 4, "", "nitrate size bin 002", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["swgntclr"] = "surface_net_downward_shortwave_flux_assuming_clear_sky";
      gmao_names["surface_net_downward_shortwave_flux_assuming_clear_sky"] = "swgntclr";
      gmao_vars["swgntclr"] = new MVarDesc( "swgntclr", 3, "surface_net_downward_shortwave_flux_assuming_clear_sky", "surface_net_downward_shortwave_flux_assuming_clear_sky", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["tstar"] = "surface_temperature_scale";
      gmao_names["surface_temperature_scale"] = "tstar";
      gmao_vars["tstar"] = new MVarDesc( "tstar", 3, "surface_temperature_scale", "surface_temperature_scale", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["swland"] = "Net_shortwave_land";
      gmao_names["Net_shortwave_land"] = "swland";
      gmao_vars["swland"] = new MVarDesc( "swland", 3, "Net_shortwave_land", "net_shortwave_land", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["tropt"] = "tropopause_temperature_using_blended_TROPP_estimate";
      gmao_names["tropopause_temperature_using_blended_TROPP_estimate"] = "tropt";
      gmao_vars["tropt"] = new MVarDesc( "tropt", 3, "tropopause_temperature_using_blended_TROPP_estimate", "tropopause_temperature_using_blended_tropp_estimate", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["swgntcln"] = "surface_net_downward_shortwave_flux_assuming_no_aerosol";
      gmao_names["surface_net_downward_shortwave_flux_assuming_no_aerosol"] = "swgntcln";
      gmao_vars["swgntcln"] = new MVarDesc( "swgntcln", 3, "surface_net_downward_shortwave_flux_assuming_no_aerosol", "surface_net_downward_shortwave_flux_assuming_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["coembbna"] = new MVarDesc( "coembbna", 3, "", "co emission (north america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["dtdttrb"] = "tendency_of_air_temperature_due_to_turbulence";
      gmao_names["tendency_of_air_temperature_due_to_turbulence"] = "dtdttrb";
      gmao_vars["dtdttrb"] = new MVarDesc( "dtdttrb", 4, "tendency_of_air_temperature_due_to_turbulence", "tendency_of_air_temperature_due_to_turbulence", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["u50m"] = "eastward_wind_at_50_meters";
      gmao_names["eastward_wind_at_50_meters"] = "u50m";
      gmao_vars["u50m"] = new MVarDesc( "u50m", 3, "eastward_wind_at_50_meters", "50-meter_eastward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["doxdtmst"] = "tendency_of_odd_oxygen_due_to_moist_processes";
      gmao_names["tendency_of_odd_oxygen_due_to_moist_processes"] = "doxdtmst";
      gmao_vars["doxdtmst"] = new MVarDesc( "doxdtmst", 4, "tendency_of_odd_oxygen_due_to_moist_processes", "tendency_of_odd_oxygen_due_to_moist_processes", "mol mol-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Cp,tavg3_3d_odt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90" );

      cf_names["suem004"] = "Sulfate Emission Bin 004 __ENSEMBLE__";
      gmao_names["Sulfate Emission Bin 004 __ENSEMBLE__"] = "suem004";
      gmao_vars["suem004"] = new MVarDesc( "suem004", 3, "Sulfate Emission Bin 004 __ENSEMBLE__", "sulfate emission bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sssmass25"] = "Sea Salt Surface Mass Concentration - PM 2.5";
      gmao_names["Sea Salt Surface Mass Concentration - PM 2.5"] = "sssmass25";
      gmao_vars["sssmass25"] = new MVarDesc( "sssmass25", 3, "Sea Salt Surface Mass Concentration - PM 2.5", "sea salt surface mass concentration - pm 2.5 __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["suem001"] = "Sulfate Emission Bin 001 __ENSEMBLE__";
      gmao_names["Sulfate Emission Bin 001 __ENSEMBLE__"] = "suem001";
      gmao_vars["suem001"] = new MVarDesc( "suem001", 3, "Sulfate Emission Bin 001 __ENSEMBLE__", "sulfate emission bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["lwgnt"] = "surface_net_downward_longwave_flux";
      gmao_names["surface_net_downward_longwave_flux"] = "lwgnt";
      gmao_vars["lwgnt"] = new MVarDesc( "lwgnt", 3, "surface_net_downward_longwave_flux", "surface_net_downward_longwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["suem003"] = "Sulfate Emission Bin 003 __ENSEMBLE__";
      gmao_names["Sulfate Emission Bin 003 __ENSEMBLE__"] = "suem003";
      gmao_vars["suem003"] = new MVarDesc( "suem003", 3, "Sulfate Emission Bin 003 __ENSEMBLE__", "sulfate emission bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["suem002"] = "Sulfate Emission Bin 002 __ENSEMBLE__";
      gmao_names["Sulfate Emission Bin 002 __ENSEMBLE__"] = "suem002";
      gmao_vars["suem002"] = new MVarDesc( "suem002", 3, "Sulfate Emission Bin 002 __ENSEMBLE__", "sulfate emission bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssscatfm"] = "Sea Salt Scattering AOT [550 nm] - PM 1.0 um";
      gmao_names["Sea Salt Scattering AOT [550 nm] - PM 1.0 um"] = "ssscatfm";
      gmao_vars["ssscatfm"] = new MVarDesc( "ssscatfm", 3, "Sea Salt Scattering AOT [550 nm] - PM 1.0 um", "sea salt scattering aot [550 nm] - pm 1.0 um __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["swcool"] = new MVarDesc( "swcool", 3, "", "solar_heating_in_cool_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["swgntwtr"] = "open_water_net_downward_shortwave_flux";
      gmao_names["open_water_net_downward_shortwave_flux"] = "swgntwtr";
      gmao_vars["swgntwtr"] = new MVarDesc( "swgntwtr", 3, "open_water_net_downward_shortwave_flux", "open_water_net_downward_shortwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["dusmass"] = "Dust Surface Mass Concentration";
      gmao_names["Dust Surface Mass Concentration"] = "dusmass";
      gmao_vars["dusmass"] = new MVarDesc( "dusmass", 3, "Dust Surface Mass Concentration", "dust surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["ssinc"] = "Sea-salt Mixing Ratio Analysis Increments";
      gmao_names["Sea-salt Mixing Ratio Analysis Increments"] = "ssinc";
      gmao_vars["ssinc"] = new MVarDesc( "ssinc", 4, "Sea-salt Mixing Ratio Analysis Increments", "sea-salt mixing ratio analysis increments", "kg/kg", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0" );

      cf_names["u2m"] = "2-meter_eastward_wind";
      gmao_names["2-meter_eastward_wind"] = "u2m";
      gmao_vars["u2m"] = new MVarDesc( "u2m", 3, "2-meter_eastward_wind", "2-meter_eastward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["niwt001"] = new MVarDesc( "niwt001", 3, "", "nitrate wet deposition bin 001 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["niwt003"] = new MVarDesc( "niwt003", 3, "", "nitrate wet deposition bin 003 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["niwt002"] = new MVarDesc( "niwt002", 3, "", "nitrate wet deposition bin 002 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["duextt25"] = "Dust Extinction AOT [550 nm] - PM 2.5";
      gmao_names["Dust Extinction AOT [550 nm] - PM 2.5"] = "duextt25";
      gmao_vars["duextt25"] = new MVarDesc( "duextt25", 3, "Dust Extinction AOT [550 nm] - PM 2.5", "dust extinction aot [550 nm] - pm 2.5 __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dqidttrb"] = "tendency_of_frozen_condensate_due_to_turbulence";
      gmao_names["tendency_of_frozen_condensate_due_to_turbulence"] = "dqidttrb";
      gmao_vars["dqidttrb"] = new MVarDesc( "dqidttrb", 4, "tendency_of_frozen_condensate_due_to_turbulence", "tendency_of_frozen_condensate_due_to_turbulence", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["lwgab"] = "surface_absorbed_longwave_radiation";
      gmao_names["surface_absorbed_longwave_radiation"] = "lwgab";
      gmao_vars["lwgab"] = new MVarDesc( "lwgab", 3, "surface_absorbed_longwave_radiation", "surface_absorbed_longwave_radiation", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["lai"] = "leaf_area_index";
      gmao_names["leaf_area_index"] = "lai";
      gmao_vars["lai"] = new MVarDesc( "lai", 3, "leaf_area_index", "leaf_area_index", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["rh"] = "relative_humidity_after_moist";
      gmao_names["relative_humidity_after_moist"] = "rh";
      gmao_vars["rh"] = new MVarDesc( "rh", 4, "relative_humidity_after_moist", "relative_humidity_after_moist", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["ri"] = "Richardson_number_from_Louis";
      gmao_names["Richardson_number_from_Louis"] = "ri";
      gmao_vars["ri"] = new MVarDesc( "ri", 4, "Richardson_number_from_Louis", "richardson_number_from_louis", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      gmao_vars["zeta_w"] = new MVarDesc( "zeta_w", 3, "", "stability_parameter_in_warm_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["aodana"] = "Aerosol Optical Depth Analysis";
      gmao_names["Aerosol Optical Depth Analysis"] = "aodana";
      gmao_vars["aodana"] = new MVarDesc( "aodana", 3, "Aerosol Optical Depth Analysis", "aerosol optical depth analysis", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_gas_Nx,inst3_2d_gas_Nx.%s,0,0,3,0,0" );

      cf_names["sswt002"] = "Sea Salt Wet Deposition Bin 002";
      gmao_names["Sea Salt Wet Deposition Bin 002"] = "sswt002";
      gmao_vars["sswt002"] = new MVarDesc( "sswt002", 3, "Sea Salt Wet Deposition Bin 002", "sea salt wet deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["coem"] = "CO Emission __ENSEMBLE__";
      gmao_names["CO Emission __ENSEMBLE__"] = "coem";
      gmao_vars["coem"] = new MVarDesc( "coem", 3, "CO Emission __ENSEMBLE__", "co emission __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["khrad"] = "radiation_driven_scalar_diffusivity_from_Lock_scheme";
      gmao_names["radiation_driven_scalar_diffusivity_from_Lock_scheme"] = "khrad";
      gmao_vars["khrad"] = new MVarDesc( "khrad", 4, "radiation_driven_scalar_diffusivity_from_Lock_scheme", "radiation_driven_scalar_diffusivity_from_lock_scheme", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["rhoa"] = "air_density_at_surface";
      gmao_names["air_density_at_surface"] = "rhoa";
      gmao_vars["rhoa"] = new MVarDesc( "rhoa", 3, "air_density_at_surface", "air_density_at_surface", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["u500"] = "eastward_wind_at_500_hPa";
      gmao_names["eastward_wind_at_500_hPa"] = "u500";
      gmao_vars["u500"] = new MVarDesc( "u500", 3, "eastward_wind_at_500_hPa", "eastward_wind_at_500_hpa", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["ocembb"] = "Organic Carbon Biomass Burning Emissions __ENSEMBLE__";
      gmao_names["Organic Carbon Biomass Burning Emissions __ENSEMBLE__"] = "ocembb";
      gmao_vars["ocembb"] = new MVarDesc( "ocembb", 3, "Organic Carbon Biomass Burning Emissions __ENSEMBLE__", "organic carbon biomass burning emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["lwtup"] = "upwelling_longwave_flux_at_toa";
      gmao_names["upwelling_longwave_flux_at_toa"] = "lwtup";
      gmao_vars["lwtup"] = new MVarDesc( "lwtup", 3, "upwelling_longwave_flux_at_toa", "upwelling_longwave_flux_at_toa", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["ocembf"] = "Organic Carbon Biofuel Emissions __ENSEMBLE__";
      gmao_names["Organic Carbon Biofuel Emissions __ENSEMBLE__"] = "ocembf";
      gmao_vars["ocembf"] = new MVarDesc( "ocembf", 3, "Organic Carbon Biofuel Emissions __ENSEMBLE__", "organic carbon biofuel emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ocembg"] = "Organic Carbon Biogenic Emissions __ENSEMBLE__";
      gmao_names["Organic Carbon Biogenic Emissions __ENSEMBLE__"] = "ocembg";
      gmao_vars["ocembg"] = new MVarDesc( "ocembg", 3, "Organic Carbon Biogenic Emissions __ENSEMBLE__", "organic carbon biogenic emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["khlk"] = "entrainment_heat_diffusivity_from_Lock";
      gmao_names["entrainment_heat_diffusivity_from_Lock"] = "khlk";
      gmao_vars["khlk"] = new MVarDesc( "khlk", 4, "entrainment_heat_diffusivity_from_Lock", "entrainment_heat_diffusivity_from_lock", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["prevtot"] = "Total_column_re-evap/subl_of_precipitation";
      gmao_names["Total_column_re-evap/subl_of_precipitation"] = "prevtot";
      gmao_vars["prevtot"] = new MVarDesc( "prevtot", 3, "Total_column_re-evap/subl_of_precipitation", "total_column_re-evap/subl_of_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["h500"] = "height_at_500_hPa";
      gmao_names["height_at_500_hPa"] = "h500";
      gmao_vars["h500"] = new MVarDesc( "h500", 3, "height_at_500_hPa", "height_at_500_hpa", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["khls"] = "scalar_diffusivity_from_Louis";
      gmao_names["scalar_diffusivity_from_Louis"] = "khls";
      gmao_vars["khls"] = new MVarDesc( "khls", 4, "scalar_diffusivity_from_Louis", "scalar_diffusivity_from_louis", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["duangstr"] = "Dust Angstrom parameter [470-870 nm]";
      gmao_names["Dust Angstrom parameter [470-870 nm]"] = "duangstr";
      gmao_vars["duangstr"] = new MVarDesc( "duangstr", 3, "Dust Angstrom parameter [470-870 nm]", "dust angstrom parameter [470-870 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["ssexttau"] = "Sea Salt Extinction AOT [550 nm]";
      gmao_names["Sea Salt Extinction AOT [550 nm]"] = "ssexttau";
      gmao_vars["ssexttau"] = new MVarDesc( "ssexttau", 3, "Sea Salt Extinction AOT [550 nm]", "sea salt extinction aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["prmc"] = "water_profile";
      gmao_names["water_profile"] = "prmc";
      gmao_vars["prmc"] = new MVarDesc( "prmc", 3, "water_profile", "water_profile", "m-3 m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["coscbbna"] = new MVarDesc( "coscbbna", 3, "", "co surface concentration in ppbv (north america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["dudtmst"] = "zonal_wind_tendency_due_to_moist";
      gmao_names["zonal_wind_tendency_due_to_moist"] = "dudtmst";
      gmao_vars["dudtmst"] = new MVarDesc( "dudtmst", 4, "zonal_wind_tendency_due_to_moist", "zonal_wind_tendency_due_to_moist", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["niht002"] = new MVarDesc( "niht002", 3, "", "nitrate production from het chem bin 002 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["niht003"] = new MVarDesc( "niht003", 3, "", "nitrate production from het chem bin 003 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["niht001"] = new MVarDesc( "niht001", 3, "", "nitrate production from het chem bin 001 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["co2sc"] = new MVarDesc( "co2sc", 3, "", "co2 surface concentration bin 001", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["swgntice"] = "sea_ice_net_downward_shortwave_flux";
      gmao_names["sea_ice_net_downward_shortwave_flux"] = "swgntice";
      gmao_vars["swgntice"] = new MVarDesc( "swgntice", 3, "sea_ice_net_downward_shortwave_flux", "sea_ice_net_downward_shortwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["sswt003"] = "Sea Salt Wet Deposition Bin 003";
      gmao_names["Sea Salt Wet Deposition Bin 003"] = "sswt003";
      gmao_vars["sswt003"] = new MVarDesc( "sswt003", 3, "Sea Salt Wet Deposition Bin 003", "sea salt wet deposition bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["tox"] = "total_column_odd_oxygen";
      gmao_names["total_column_odd_oxygen"] = "tox";
      gmao_vars["tox"] = new MVarDesc( "tox", 3, "total_column_odd_oxygen", "total_column_odd_oxygen", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["sswt001"] = "Sea Salt Wet Deposition Bin 001";
      gmao_names["Sea Salt Wet Deposition Bin 001"] = "sswt001";
      gmao_vars["sswt001"] = new MVarDesc( "sswt001", 3, "Sea Salt Wet Deposition Bin 001", "sea salt wet deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["niscatau"] = new MVarDesc( "niscatau", 3, "", "nitrate scattering aot [550 nm] __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["copdbbna"] = new MVarDesc( "copdbbna", 3, "", "co chemical production (north america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["sswt004"] = "Sea Salt Wet Deposition Bin 004";
      gmao_names["Sea Salt Wet Deposition Bin 004"] = "sswt004";
      gmao_vars["sswt004"] = new MVarDesc( "sswt004", 3, "Sea Salt Wet Deposition Bin 004", "sea salt wet deposition bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["sswt005"] = "Sea Salt Wet Deposition Bin 005";
      gmao_names["Sea Salt Wet Deposition Bin 005"] = "sswt005";
      gmao_vars["sswt005"] = new MVarDesc( "sswt005", 3, "Sea Salt Wet Deposition Bin 005", "sea salt wet deposition bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["teland"] = "Total_energy_storage_land";
      gmao_names["Total_energy_storage_land"] = "teland";
      gmao_vars["teland"] = new MVarDesc( "teland", 3, "Total_energy_storage_land", "total_energy_storage_land", "J m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["duscat25"] = "Dust Scattering AOT [550 nm] - PM 2.5";
      gmao_names["Dust Scattering AOT [550 nm] - PM 2.5"] = "duscat25";
      gmao_vars["duscat25"] = new MVarDesc( "duscat25", 3, "Dust Scattering AOT [550 nm] - PM 2.5", "dust scattering aot [550 nm] - pm 2.5 __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["v250"] = "northward_wind_at_250_hPa";
      gmao_names["northward_wind_at_250_hPa"] = "v250";
      gmao_vars["v250"] = new MVarDesc( "v250", 3, "northward_wind_at_250_hPa", "northward_wind_at_250_hpa", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["copd"] = "CO Chemical Production __ENSEMBLE__";
      gmao_names["CO Chemical Production __ENSEMBLE__"] = "copd";
      gmao_vars["copd"] = new MVarDesc( "copd", 3, "CO Chemical Production __ENSEMBLE__", "co chemical production __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["ocscatau"] = "Organic Carbon Scattering AOT [550 nm] __ENSEMBLE__";
      gmao_names["Organic Carbon Scattering AOT [550 nm] __ENSEMBLE__"] = "ocscatau";
      gmao_vars["ocscatau"] = new MVarDesc( "ocscatau", 3, "Organic Carbon Scattering AOT [550 nm] __ENSEMBLE__", "organic carbon scattering aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["copdbbae"] = new MVarDesc( "copdbbae", 3, "", "co chemical production (asia and europe biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["copdbbaf"] = new MVarDesc( "copdbbaf", 3, "", "co chemical production (africa biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["coemnbna"] = new MVarDesc( "coemnbna", 3, "", "co emission (north american non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["nh3"] = new MVarDesc( "nh3", 4, "", "ammonia (nh3, gas phase)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["nh4cmass"] = new MVarDesc( "nh4cmass", 3, "", "ammonium column mass density __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dtdtfri"] = "tendency_of_air_temperature_due_to_friction";
      gmao_names["tendency_of_air_temperature_due_to_friction"] = "dtdtfri";
      gmao_vars["dtdtfri"] = new MVarDesc( "dtdtfri", 4, "tendency_of_air_temperature_due_to_friction", "tendency_of_air_temperature_due_to_friction", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["totangstr"] = "Total Aerosol Angstrom parameter [470-870 nm]";
      gmao_names["Total Aerosol Angstrom parameter [470-870 nm]"] = "totangstr";
      gmao_vars["totangstr"] = new MVarDesc( "totangstr", 3, "Total Aerosol Angstrom parameter [470-870 nm]", "total aerosol angstrom parameter [470-870 nm]", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["to3"] = "total_column_ozone";
      gmao_names["total_column_ozone"] = "to3";
      gmao_vars["to3"] = new MVarDesc( "to3", 3, "total_column_ozone", "total_column_ozone", "Dobsons", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["preclsc"] = "nonanvil_large_scale_precipitation";
      gmao_names["nonanvil_large_scale_precipitation"] = "preclsc";
      gmao_vars["preclsc"] = new MVarDesc( "preclsc", 3, "nonanvil_large_scale_precipitation", "nonanvil_large_scale_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["niinc"] = new MVarDesc( "niinc", 4, "", "nitrate mixing ratio analysis increments", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0" );

      gmao_vars["qils"] = new MVarDesc( "qils", 4, "", "mass_fraction_of_large_scale_cloud_ice_water", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["tprecmax"] = "total_precipitation";
      gmao_names["total_precipitation"] = "tprecmax";
      gmao_vars["tprecmax"] = new MVarDesc( "tprecmax", 3, "total_precipitation", "total_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0" );

      cf_names["pblh"] = "planetary_boundary_layer_height";
      gmao_names["planetary_boundary_layer_height"] = "pblh";
      gmao_vars["pblh"] = new MVarDesc( "pblh", 3, "planetary_boundary_layer_height", "planetary_boundary_layer_height", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["bcfluxv"] = "Black Carbon column v-wind mass flux";
      gmao_names["Black Carbon column v-wind mass flux"] = "bcfluxv";
      gmao_vars["bcfluxv"] = new MVarDesc( "bcfluxv", 3, "Black Carbon column v-wind mass flux", "black carbon column v-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["bcfluxu"] = "Black Carbon column u-wind mass flux";
      gmao_names["Black Carbon column u-wind mass flux"] = "bcfluxu";
      gmao_vars["bcfluxu"] = new MVarDesc( "bcfluxu", 3, "Black Carbon column u-wind mass flux", "black carbon column u-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["albnirdr"] = "surface_albedo_for_near_infrared_beam";
      gmao_names["surface_albedo_for_near_infrared_beam"] = "albnirdr";
      gmao_vars["albnirdr"] = new MVarDesc( "albnirdr", 3, "surface_albedo_for_near_infrared_beam", "surface_albedo_for_near_infrared_beam", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["albnirdf"] = "surface_albedo_for_near_infrared_diffuse";
      gmao_names["surface_albedo_for_near_infrared_diffuse"] = "albnirdf";
      gmao_vars["albnirdf"] = new MVarDesc( "albnirdf", 3, "surface_albedo_for_near_infrared_diffuse", "surface_albedo_for_near_infrared_diffuse", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["albedo"] = "surface_albedo";
      gmao_names["surface_albedo"] = "albedo";
      gmao_vars["albedo"] = new MVarDesc( "albedo", 3, "surface_albedo", "surface_albedo", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["taucli"] = "in_cloud_optical_thickness_for_ice_clouds";
      gmao_names["in_cloud_optical_thickness_for_ice_clouds"] = "taucli";
      gmao_vars["taucli"] = new MVarDesc( "taucli", 4, "in_cloud_optical_thickness_for_ice_clouds", "in_cloud_optical_thickness_for_ice_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      gmao_vars["co2em"] = new MVarDesc( "co2em", 3, "", "co2 emission bin 001", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["doxdtchm"] = "tendency_of_odd_oxygen_mixing_ratio_due_to_chemistry";
      gmao_names["tendency_of_odd_oxygen_mixing_ratio_due_to_chemistry"] = "doxdtchm";
      gmao_vars["doxdtchm"] = new MVarDesc( "doxdtchm", 4, "tendency_of_odd_oxygen_mixing_ratio_due_to_chemistry", "tendency_of_odd_oxygen_mixing_ratio_due_to_chemistry", "mol mol-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Cp,tavg3_3d_odt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90" );

      gmao_vars["cfls"] = new MVarDesc( "cfls", 4, "", "large_scale_cloud_area_fraction", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["lwgem"] = "longwave_flux_emitted_from_surface";
      gmao_names["longwave_flux_emitted_from_surface"] = "lwgem";
      gmao_vars["lwgem"] = new MVarDesc( "lwgem", 3, "longwave_flux_emitted_from_surface", "longwave_flux_emitted_from_surface", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["tauclw"] = "in_cloud_optical_thickness_for_liquid_clouds";
      gmao_names["in_cloud_optical_thickness_for_liquid_clouds"] = "tauclw";
      gmao_vars["tauclw"] = new MVarDesc( "tauclw", 4, "in_cloud_optical_thickness_for_liquid_clouds", "in_cloud_optical_thickness_for_liquid_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["qinfil"] = "Soil_water_infiltration_rate";
      gmao_names["Soil_water_infiltration_rate"] = "qinfil";
      gmao_vars["qinfil"] = new MVarDesc( "qinfil", 3, "Soil_water_infiltration_rate", "soil_water_infiltration_rate", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["gwetprof"] = "ave_prof_soil_moisture";
      gmao_names["ave_prof_soil_moisture"] = "gwetprof";
      gmao_vars["gwetprof"] = new MVarDesc( "gwetprof", 3, "ave_prof_soil_moisture", "ave_prof_soil_moisture", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["dqldtdyn"] = "tendency_of_liquid_water_due_to_dynamics";
      gmao_names["tendency_of_liquid_water_due_to_dynamics"] = "dqldtdyn";
      gmao_vars["dqldtdyn"] = new MVarDesc( "dqldtdyn", 4, "tendency_of_liquid_water_due_to_dynamics", "tendency_of_liquid_water_due_to_dynamics", "kg/kg/s", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["duem002"] = "Dust Emission Bin 002";
      gmao_names["Dust Emission Bin 002"] = "duem002";
      gmao_vars["duem002"] = new MVarDesc( "duem002", 3, "Dust Emission Bin 002", "dust emission bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["duem003"] = "Dust Emission Bin 003";
      gmao_names["Dust Emission Bin 003"] = "duem003";
      gmao_vars["duem003"] = new MVarDesc( "duem003", 3, "Dust Emission Bin 003", "dust emission bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["duem001"] = "Dust Emission Bin 001";
      gmao_names["Dust Emission Bin 001"] = "duem001";
      gmao_vars["duem001"] = new MVarDesc( "duem001", 3, "Dust Emission Bin 001", "dust emission bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["pgentot"] = "Total_column_production_of_precipitation";
      gmao_names["Total_column_production_of_precipitation"] = "pgentot";
      gmao_vars["pgentot"] = new MVarDesc( "pgentot", 3, "Total_column_production_of_precipitation", "total_column_production_of_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["duem004"] = "Dust Emission Bin 004";
      gmao_names["Dust Emission Bin 004"] = "duem004";
      gmao_vars["duem004"] = new MVarDesc( "duem004", 3, "Dust Emission Bin 004", "dust emission bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["duem005"] = "Dust Emission Bin 005";
      gmao_names["Dust Emission Bin 005"] = "duem005";
      gmao_vars["duem005"] = new MVarDesc( "duem005", 3, "Dust Emission Bin 005", "dust emission bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssscatau"] = "Sea Salt Scattering AOT [550 nm]";
      gmao_names["Sea Salt Scattering AOT [550 nm]"] = "ssscatau";
      gmao_vars["ssscatau"] = new MVarDesc( "ssscatau", 3, "Sea Salt Scattering AOT [550 nm]", "sea salt scattering aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["nh3dp"] = new MVarDesc( "nh3dp", 3, "", "ammonia dry deposition __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["qcool"] = new MVarDesc( "qcool", 3, "", "net_cooling_in_cool_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["precanv"] = "anvil_precipitation";
      gmao_names["anvil_precipitation"] = "precanv";
      gmao_vars["precanv"] = new MVarDesc( "precanv", 3, "anvil_precipitation", "anvil_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["niexttfm"] = new MVarDesc( "niexttfm", 3, "", "nitrate extinction aot [550 nm] - pm 1.0 um __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["km"] = "total_momentum_diffusivity";
      gmao_names["total_momentum_diffusivity"] = "km";
      gmao_vars["km"] = new MVarDesc( "km", 4, "total_momentum_diffusivity", "total_momentum_diffusivity", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      gmao_vars["frlake"] = new MVarDesc( "frlake", 3, "", "fraction_of_lake", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["phiw"] = new MVarDesc( "phiw", 3, "", "similarity_function_in_warm_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["phis"] = "surface geopotential height";
      gmao_names["surface geopotential height"] = "phis";
      gmao_vars["phis"] = new MVarDesc( "phis", 3, "surface geopotential height", "surface geopotential height", "m+2 s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      cf_names["h250"] = "height_at_250_hPa";
      gmao_names["height_at_250_hPa"] = "h250";
      gmao_vars["h250"] = new MVarDesc( "h250", 3, "height_at_250_hPa", "height_at_250_hpa", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["nidp002"] = new MVarDesc( "nidp002", 3, "", "nitrate dry deposition bin 002 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["rainocn"] = "ocean_rainfall";
      gmao_names["ocean_rainfall"] = "rainocn";
      gmao_vars["rainocn"] = new MVarDesc( "rainocn", 3, "ocean_rainfall", "ocean_rainfall", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["nidp001"] = new MVarDesc( "nidp001", 3, "", "nitrate dry deposition bin 001 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dqrlsan"] = "large_scale_rainwater_source";
      gmao_names["large_scale_rainwater_source"] = "dqrlsan";
      gmao_vars["dqrlsan"] = new MVarDesc( "dqrlsan", 4, "large_scale_rainwater_source", "large_scale_rainwater_source", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90" );

      cf_names["cdm"] = "surface_exchange_coefficient_for_momentum";
      gmao_names["surface_exchange_coefficient_for_momentum"] = "cdm";
      gmao_vars["cdm"] = new MVarDesc( "cdm", 3, "surface_exchange_coefficient_for_momentum", "surface_exchange_coefficient_for_momentum", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["cdh"] = "surface_exchange_coefficient_for_heat";
      gmao_names["surface_exchange_coefficient_for_heat"] = "cdh";
      gmao_vars["cdh"] = new MVarDesc( "cdh", 3, "surface_exchange_coefficient_for_heat", "surface_exchange_coefficient_for_heat", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["duwt005"] = "Dust Wet Deposition Bin 005";
      gmao_names["Dust Wet Deposition Bin 005"] = "duwt005";
      gmao_vars["duwt005"] = new MVarDesc( "duwt005", 3, "Dust Wet Deposition Bin 005", "dust wet deposition bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["duwt004"] = "Dust Wet Deposition Bin 004";
      gmao_names["Dust Wet Deposition Bin 004"] = "duwt004";
      gmao_vars["duwt004"] = new MVarDesc( "duwt004", 3, "Dust Wet Deposition Bin 004", "dust wet deposition bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["duwt003"] = "Dust Wet Deposition Bin 003";
      gmao_names["Dust Wet Deposition Bin 003"] = "duwt003";
      gmao_vars["duwt003"] = new MVarDesc( "duwt003", 3, "Dust Wet Deposition Bin 003", "dust wet deposition bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["tunst"] = "surface_temperature_of_unsaturated_zone";
      gmao_names["surface_temperature_of_unsaturated_zone"] = "tunst";
      gmao_vars["tunst"] = new MVarDesc( "tunst", 3, "surface_temperature_of_unsaturated_zone", "surface_temperature_of_unsaturated_zone", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["duwt001"] = "Dust Wet Deposition Bin 001";
      gmao_names["Dust Wet Deposition Bin 001"] = "duwt001";
      gmao_vars["duwt001"] = new MVarDesc( "duwt001", 3, "Dust Wet Deposition Bin 001", "dust wet deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["coscbbaf"] = new MVarDesc( "coscbbaf", 3, "", "co surface concentration in ppbv (africa biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["coscbbae"] = new MVarDesc( "coscbbae", 3, "", "co surface concentration in ppbv (asia and europe biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["so4smass"] = "SO4 Surface Mass Concentration __ENSEMBLE__";
      gmao_names["SO4 Surface Mass Concentration __ENSEMBLE__"] = "so4smass";
      gmao_vars["so4smass"] = new MVarDesc( "so4smass", 3, "SO4 Surface Mass Concentration __ENSEMBLE__", "so4 surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["twland"] = "Avail_water_storage_land";
      gmao_names["Avail_water_storage_land"] = "twland";
      gmao_vars["twland"] = new MVarDesc( "twland", 3, "Avail_water_storage_land", "avail_water_storage_land", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["coclnbgl"] = new MVarDesc( "coclnbgl", 3, "", "co column burden (global non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["ocexttau"] = "Organic Carbon Extinction AOT [550 nm] __ENSEMBLE__";
      gmao_names["Organic Carbon Extinction AOT [550 nm] __ENSEMBLE__"] = "ocexttau";
      gmao_vars["ocexttau"] = new MVarDesc( "ocexttau", 3, "Organic Carbon Extinction AOT [550 nm] __ENSEMBLE__", "organic carbon extinction aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["bstar"] = "surface_bouyancy_scale";
      gmao_names["surface_bouyancy_scale"] = "bstar";
      gmao_vars["bstar"] = new MVarDesc( "bstar", 3, "surface_bouyancy_scale", "surface_buoyancy_scale", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["dtdtswrclr"] = "air_temperature_tendency_due_to_shortwave_for_clear_skies";
      gmao_names["air_temperature_tendency_due_to_shortwave_for_clear_skies"] = "dtdtswrclr";
      gmao_vars["dtdtswrclr"] = new MVarDesc( "dtdtswrclr", 4, "air_temperature_tendency_due_to_shortwave_for_clear_skies", "air_temperature_tendency_due_to_shortwave_for_clear_skies", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Cp,tavg3_3d_rad_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90" );

      cf_names["dusmass25"] = "Dust Surface Mass Concentration - PM 2.5";
      gmao_names["Dust Surface Mass Concentration - PM 2.5"] = "dusmass25";
      gmao_vars["dusmass25"] = new MVarDesc( "dusmass25", 3, "Dust Surface Mass Concentration - PM 2.5", "dust surface mass concentration - pm 2.5 __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["dtsdt_ana"] = new MVarDesc( "dtsdt_ana", 3, "", "total_skin_temperature_tendency", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["coemnbas"] = new MVarDesc( "coemnbas", 3, "", "co emission (asia non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["sufluxu"] = "SO4 column u-wind mass flux __ENSEMBLE__";
      gmao_names["SO4 column u-wind mass flux __ENSEMBLE__"] = "sufluxu";
      gmao_vars["sufluxu"] = new MVarDesc( "sufluxu", 3, "SO4 column u-wind mass flux __ENSEMBLE__", "so4 column u-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dudtgwd"] = "tendency_of_eastward_wind_due_to_GWD";
      gmao_names["tendency_of_eastward_wind_due_to_GWD"] = "dudtgwd";
      gmao_vars["dudtgwd"] = new MVarDesc( "dudtgwd", 4, "tendency_of_eastward_wind_due_to_GWD", "tendency_of_eastward_wind_due_to_gwd", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["qs"] = new MVarDesc( "qs", 4, "", "mass_fraction_of_falling_snow", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0" );

      gmao_vars["qr"] = new MVarDesc( "qr", 4, "", "mass_fraction_of_falling_rain", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0" );

      cf_names["qv"] = "specific_humidity";
      gmao_names["specific_humidity"] = "qv";
      gmao_vars["qv"] = new MVarDesc( "qv", 4, "specific_humidity", "specific_humidity", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      cf_names["h"] = "edge_heights";
      gmao_names["edge_heights"] = "h";
      gmao_vars["h"] = new MVarDesc( "h", 4, "edge_heights", "edge_heights", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_nav_Nv,tavg3_3d_nav_Nv.%s,0,1,3,3,90" );

      cf_names["dtdtlwrclr"] = "air_temperature_tendency_due_to_longwave_for_clear_skies";
      gmao_names["air_temperature_tendency_due_to_longwave_for_clear_skies"] = "dtdtlwrclr";
      gmao_vars["dtdtlwrclr"] = new MVarDesc( "dtdtlwrclr", 4, "air_temperature_tendency_due_to_longwave_for_clear_skies", "air_temperature_tendency_due_to_longwave_for_clear_skies", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Cp,tavg3_3d_rad_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90" );

      cf_names["qi"] = "mass_fraction_of_cloud_ice_water";
      gmao_names["mass_fraction_of_cloud_ice_water"] = "qi";
      gmao_vars["qi"] = new MVarDesc( "qi", 4, "mass_fraction_of_cloud_ice_water", "mass_fraction_of_cloud_ice_water", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      gmao_vars["hno3cmass"] = new MVarDesc( "hno3cmass", 3, "", "nitric acid column mass density __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["ql"] = "mass_fraction_of_cloud_liquid_water";
      gmao_names["mass_fraction_of_cloud_liquid_water"] = "ql";
      gmao_vars["ql"] = new MVarDesc( "ql", 4, "mass_fraction_of_cloud_liquid_water", "mass_fraction_of_cloud_liquid_water", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      cf_names["totscatau"] = "Total Aerosol Scattering AOT [550 nm]";
      gmao_names["Total Aerosol Scattering AOT [550 nm]"] = "totscatau";
      gmao_vars["totscatau"] = new MVarDesc( "totscatau", 3, "Total Aerosol Scattering AOT [550 nm]", "total aerosol scattering aot [550 nm]", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["swgdn"] = "Incident_shortwave_land";
      gmao_names["Incident_shortwave_land"] = "swgdn";
      gmao_vars["swgdn"] = new MVarDesc( "swgdn", 3, "Incident_shortwave_land", "surface_incoming_shortwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["evland"] = "Evaporation_land";
      gmao_names["Evaporation_land"] = "evland";
      gmao_vars["evland"] = new MVarDesc( "evland", 3, "Evaporation_land", "evaporation_land", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["tautot"] = "in_cloud_optical_thickness_of_all_clouds";
      gmao_names["in_cloud_optical_thickness_of_all_clouds"] = "tautot";
      gmao_vars["tautot"] = new MVarDesc( "tautot", 3, "in_cloud_optical_thickness_of_all_clouds", "in_cloud_optical_thickness_of_all_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["ocsv"] = new MVarDesc( "ocsv", 3, "", "organic_carbon_tendency_due_to_conv_scav", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["coclbbla"] = new MVarDesc( "coclbbla", 3, "", "co column burden (central and south america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["frland"] = new MVarDesc( "frland", 3, "", "fraction_of_land", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["co2"] = new MVarDesc( "co2", 4, "", "carbon dioxide (all sources)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_chm_Nv,inst3_3d_chm_Nv.%s,0,1,3,0,0" );

      gmao_vars["tautw"] = new MVarDesc( "tautw", 3, "", "relaxation_time_of_tw_to_ts_found", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["mfyc"] = new MVarDesc( "mfyc", 4, "", "pressure_weighted_accumulated_northward_mass_flux", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Nv,tavg3_3d_lsf_Nv.%s,0,1,3,3,90" );

      gmao_vars["hno3smass"] = new MVarDesc( "hno3smass", 3, "", "nitric acid surface mass concentration __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["coscnbna"] = new MVarDesc( "coscnbna", 3, "", "co surface concentration in ppbv (north american non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["frcan"] = "areal_fraction_of_anvil_showers";
      gmao_names["areal_fraction_of_anvil_showers"] = "frcan";
      gmao_vars["frcan"] = new MVarDesc( "frcan", 3, "areal_fraction_of_anvil_showers", "areal_fraction_of_anvil_showers", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["dqvdtmst"] = "specific_humidity_tendency_due_to_moist";
      gmao_names["specific_humidity_tendency_due_to_moist"] = "dqvdtmst";
      gmao_vars["dqvdtmst"] = new MVarDesc( "dqvdtmst", 4, "specific_humidity_tendency_due_to_moist", "specific_humidity_tendency_due_to_moist", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      gmao_vars["copdnbeu"] = new MVarDesc( "copdnbeu", 3, "", "co chemical production (european non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["speed"] = "surface_wind_speed";
      gmao_names["surface_wind_speed"] = "speed";
      gmao_vars["speed"] = new MVarDesc( "speed", 3, "surface_wind_speed", "surface_ventilation_velocity", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["bcphilic"] = "Hydrophilic Black Carbon";
      gmao_names["Hydrophilic Black Carbon"] = "bcphilic";
      gmao_vars["bcphilic"] = new MVarDesc( "bcphilic", 4, "Hydrophilic Black Carbon", "hydrophilic black carbon", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["cfan"] = new MVarDesc( "cfan", 4, "", "convective_cloud_area_fraction", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["airdens"] = "air_density";
      gmao_names["air_density"] = "airdens";
      gmao_vars["airdens"] = new MVarDesc( "airdens", 4, "air_density", "moist_air_density", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_chm_Nv,inst3_3d_chm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["frsat"] = "fractional_area_of_saturated_zone";
      gmao_names["fractional_area_of_saturated_zone"] = "frsat";
      gmao_vars["frsat"] = new MVarDesc( "frsat", 3, "fractional_area_of_saturated_zone", "fractional_area_of_saturated_zone", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["troppb"] = "tropopause_pressure_based_on_blended_estimate";
      gmao_names["tropopause_pressure_based_on_blended_estimate"] = "troppb";
      gmao_vars["troppb"] = new MVarDesc( "troppb", 3, "tropopause_pressure_based_on_blended_estimate", "tropopause_pressure_based_on_blended_estimate", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["dudttrb"] = "tendency_of_eastward_wind_due_to_turbulence";
      gmao_names["tendency_of_eastward_wind_due_to_turbulence"] = "dudttrb";
      gmao_vars["dudttrb"] = new MVarDesc( "dudttrb", 4, "tendency_of_eastward_wind_due_to_turbulence", "tendency_of_eastward_wind_due_to_turbulence", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["coscnbeu"] = new MVarDesc( "coscnbeu", 3, "", "co surface concentration in ppbv (european non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["gwetroot"] = "root_zone_soil_wetness";
      gmao_names["root_zone_soil_wetness"] = "gwetroot";
      gmao_vars["gwetroot"] = new MVarDesc( "gwetroot", 3, "root_zone_soil_wetness", "root_zone_soil_wetness", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["troppt"] = "tropopause_pressure_based_on_thermal_estimate";
      gmao_names["tropopause_pressure_based_on_thermal_estimate"] = "troppt";
      gmao_vars["troppt"] = new MVarDesc( "troppt", 3, "tropopause_pressure_based_on_thermal_estimate", "tropopause_pressure_based_on_thermal_estimate", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["troppv"] = "tropopause_pressure_based_on_EPV_estimate";
      gmao_names["tropopause_pressure_based_on_EPV_estimate"] = "troppv";
      gmao_vars["troppv"] = new MVarDesc( "troppv", 3, "tropopause_pressure_based_on_EPV_estimate", "tropopause_pressure_based_on_epv_estimate", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["cldhgh"] = "cloud_area_fraction_for_high_clouds";
      gmao_names["cloud_area_fraction_for_high_clouds"] = "cldhgh";
      gmao_vars["cldhgh"] = new MVarDesc( "cldhgh", 3, "cloud_area_fraction_for_high_clouds", "cloud_area_fraction_for_high_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["lwgntclrcln"] = "surface_net_downward_longwave_flux_assuming_clear_sky_and_no_aerosol";
      gmao_names["surface_net_downward_longwave_flux_assuming_clear_sky_and_no_aerosol"] = "lwgntclrcln";
      gmao_vars["lwgntclrcln"] = new MVarDesc( "lwgntclrcln", 3, "surface_net_downward_longwave_flux_assuming_clear_sky_and_no_aerosol", "surface_net_downward_longwave_flux_assuming_clear_sky_and_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["duinc"] = "Dust Mixing Ratio Analysis Increments";
      gmao_names["Dust Mixing Ratio Analysis Increments"] = "duinc";
      gmao_vars["duinc"] = new MVarDesc( "duinc", 4, "Dust Mixing Ratio Analysis Increments", "dust mixing ratio analysis increments", "kg/kg", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0" );

      cf_names["co"] = "Carbon Monoxide (All Sources)";
      gmao_names["Carbon Monoxide (All Sources)"] = "co";
      gmao_vars["co"] = new MVarDesc( "co", 4, "Carbon Monoxide (All Sources)", "carbon monoxide (all sources)", "mol mol-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_chm_Nv,inst3_3d_chm_Nv.%s,0,1,3,0,0" );

      cf_names["cn"] = "surface_neutral_drag_coefficient";
      gmao_names["surface_neutral_drag_coefficient"] = "cn";
      gmao_vars["cn"] = new MVarDesc( "cn", 3, "surface_neutral_drag_coefficient", "surface_neutral_drag_coefficient", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["ocfluxv"] = "Organic Carbon column v-wind mass flux __ENSEMBLE__";
      gmao_names["Organic Carbon column v-wind mass flux __ENSEMBLE__"] = "ocfluxv";
      gmao_vars["ocfluxv"] = new MVarDesc( "ocfluxv", 3, "Organic Carbon column v-wind mass flux __ENSEMBLE__", "organic carbon column v-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["tauxwtr"] = "eastward_stress_over_water";
      gmao_names["eastward_stress_over_water"] = "tauxwtr";
      gmao_vars["tauxwtr"] = new MVarDesc( "tauxwtr", 3, "eastward_stress_over_water", "eastward_stress_over_water", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["risfc"] = "surface_bulk_richardson_number";
      gmao_names["surface_bulk_richardson_number"] = "risfc";
      gmao_vars["risfc"] = new MVarDesc( "risfc", 3, "surface_bulk_richardson_number", "surface_bulk_richardson_number", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["ustarw"] = new MVarDesc( "ustarw", 3, "", "ustar_over_water_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["duwt002"] = "Dust Wet Deposition Bin 002";
      gmao_names["Dust Wet Deposition Bin 002"] = "duwt002";
      gmao_vars["duwt002"] = new MVarDesc( "duwt002", 3, "Dust Wet Deposition Bin 002", "dust wet deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["v2m"] = "2-meter_northward_wind";
      gmao_names["2-meter_northward_wind"] = "v2m";
      gmao_vars["v2m"] = new MVarDesc( "v2m", 3, "2-meter_northward_wind", "2-meter_northward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["ps"] = "surface_pressure";
      gmao_names["surface_pressure"] = "ps";
      gmao_vars["ps"] = new MVarDesc( "ps", 3, "surface_pressure", "surface_pressure", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_chm_Nv,inst3_3d_chm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Nv,tavg3_3d_lsf_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_lfo_Nx,inst1_2d_lfo_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_smp_Nx,inst3_2d_smp_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["sscmass25"] = "Sea Salt Column Mass Density - PM 2.5";
      gmao_names["Sea Salt Column Mass Density - PM 2.5"] = "sscmass25";
      gmao_vars["sscmass25"] = new MVarDesc( "sscmass25", 3, "Sea Salt Column Mass Density - PM 2.5", "sea salt column mass density - pm 2.5 __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["ocphobic"] = "Hydrophobic Organic Carbon (Particulate Matter)";
      gmao_names["Hydrophobic Organic Carbon (Particulate Matter)"] = "ocphobic";
      gmao_vars["ocphobic"] = new MVarDesc( "ocphobic", 4, "Hydrophobic Organic Carbon (Particulate Matter)", "hydrophobic organic carbon (particulate matter)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["frwlt"] = "fractional_area_of_wilting_zone";
      gmao_names["fractional_area_of_wilting_zone"] = "frwlt";
      gmao_vars["frwlt"] = new MVarDesc( "frwlt", 3, "fractional_area_of_wilting_zone", "fractional_area_of_wilting_zone", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["pflcu"] = "3D_flux_of_liquid_convective_precipitation";
      gmao_names["3D_flux_of_liquid_convective_precipitation"] = "pflcu";
      gmao_vars["pflcu"] = new MVarDesc( "pflcu", 4, "3D_flux_of_liquid_convective_precipitation", "3d_flux_of_liquid_convective_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90" );

      cf_names["echange"] = "rate_of_change_of_total_land_energy";
      gmao_names["rate_of_change_of_total_land_energy"] = "echange";
      gmao_vars["echange"] = new MVarDesc( "echange", 3, "rate_of_change_of_total_land_energy", "rate_of_change_of_total_land_energy", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["copdnbna"] = new MVarDesc( "copdnbna", 3, "", "co chemical production (north american non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["occmass"] = "Organic Carbon Column Mass Density __ENSEMBLE__";
      gmao_names["Organic Carbon Column Mass Density __ENSEMBLE__"] = "occmass";
      gmao_vars["occmass"] = new MVarDesc( "occmass", 3, "Organic Carbon Column Mass Density __ENSEMBLE__", "organic carbon column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["bcangstr"] = "Black Carbon Angstrom parameter [470-870 nm]";
      gmao_names["Black Carbon Angstrom parameter [470-870 nm]"] = "bcangstr";
      gmao_vars["bcangstr"] = new MVarDesc( "bcangstr", 3, "Black Carbon Angstrom parameter [470-870 nm]", "black carbon angstrom parameter [470-870 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["lcool"] = new MVarDesc( "lcool", 3, "", "saunders_parameter", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["pl"] = "mid_level_pressure";
      gmao_names["mid_level_pressure"] = "pl";
      gmao_vars["pl"] = new MVarDesc( "pl", 4, "mid_level_pressure", "mid_level_pressure", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_nav_Nv,tavg3_3d_nav_Nv.%s,0,1,3,3,90" );

      cf_names["precls"] = "large_scale_rainfall";
      gmao_names["large_scale_rainfall"] = "precls";
      gmao_vars["precls"] = new MVarDesc( "precls", 3, "large_scale_rainfall", "liquid_water_large_scale_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30" );

      cf_names["bcphobic"] = "Hydrophobic Black Carbon";
      gmao_names["Hydrophobic Black Carbon"] = "bcphobic";
      gmao_vars["bcphobic"] = new MVarDesc( "bcphobic", 4, "Hydrophobic Black Carbon", "hydrophobic black carbon", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["dtdtdyn"] = "tendency_of_air_temperature_due_to_dynamics";
      gmao_names["tendency_of_air_temperature_due_to_dynamics"] = "dtdtdyn";
      gmao_vars["dtdtdyn"] = new MVarDesc( "dtdtdyn", 4, "tendency_of_air_temperature_due_to_dynamics", "tendency_of_air_temperature_due_to_dynamics", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["qsh"] = "effective_surface_specific_humidity";
      gmao_names["effective_surface_specific_humidity"] = "qsh";
      gmao_vars["qsh"] = new MVarDesc( "qsh", 3, "effective_surface_specific_humidity", "effective_surface_specific_humidity", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["sssd004"] = "Sea Salt Sedimentation Bin 004";
      gmao_names["Sea Salt Sedimentation Bin 004"] = "sssd004";
      gmao_vars["sssd004"] = new MVarDesc( "sssd004", 3, "Sea Salt Sedimentation Bin 004", "sea salt sedimentation bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["swgdnclr"] = "surface_incoming_shortwave_flux_assuming_clear_sky";
      gmao_names["surface_incoming_shortwave_flux_assuming_clear_sky"] = "swgdnclr";
      gmao_vars["swgdnclr"] = new MVarDesc( "swgdnclr", 3, "surface_incoming_shortwave_flux_assuming_clear_sky", "surface_incoming_shortwave_flux_assuming_clear_sky", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["coemnbeu"] = new MVarDesc( "coemnbeu", 3, "", "co emission (european non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["smland"] = "Snowmelt_flux_land";
      gmao_names["Snowmelt_flux_land"] = "smland";
      gmao_vars["smland"] = new MVarDesc( "smland", 3, "Snowmelt_flux_land", "snowmelt_flux_land", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["cldprs"] = "cloud_top_pressure";
      gmao_names["cloud_top_pressure"] = "cldprs";
      gmao_vars["cldprs"] = new MVarDesc( "cldprs", 3, "cloud_top_pressure", "cloud_top_pressure", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["conbas"] = new MVarDesc( "conbas", 4, "", "co asia non-biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["dufluxv"] = "Dust column v-wind mass flux";
      gmao_names["Dust column v-wind mass flux"] = "dufluxv";
      gmao_vars["dufluxv"] = new MVarDesc( "dufluxv", 3, "Dust column v-wind mass flux", "dust column v-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dufluxu"] = "Dust column u-wind mass flux";
      gmao_names["Dust column u-wind mass flux"] = "dufluxu";
      gmao_vars["dufluxu"] = new MVarDesc( "dufluxu", 3, "Dust column u-wind mass flux", "dust column u-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["sssd003"] = "Sea Salt Sedimentation Bin 003";
      gmao_names["Sea Salt Sedimentation Bin 003"] = "sssd003";
      gmao_vars["sssd003"] = new MVarDesc( "sssd003", 3, "Sea Salt Sedimentation Bin 003", "sea salt sedimentation bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["efluxwtr"] = "open_water_latent_energy_flux";
      gmao_names["open_water_latent_energy_flux"] = "efluxwtr";
      gmao_vars["efluxwtr"] = new MVarDesc( "efluxwtr", 3, "open_water_latent_energy_flux", "open_water_latent_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["q850"] = "specific_humidity_at_850_hPa";
      gmao_names["specific_humidity_at_850_hPa"] = "q850";
      gmao_vars["q850"] = new MVarDesc( "q850", 3, "specific_humidity_at_850_hPa", "specific_humidity_at_850_hpa", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["nifluxu"] = new MVarDesc( "nifluxu", 3, "", "nitrate column u-wind mass flux __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["slp"] = "sea_level_pressure";
      gmao_names["sea_level_pressure"] = "slp";
      gmao_vars["slp"] = new MVarDesc( "slp", 3, "sea_level_pressure", "sea_level_pressure", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["suexttau"] = "SO4 Extinction AOT [550 nm] __ENSEMBLE__";
      gmao_names["SO4 Extinction AOT [550 nm] __ENSEMBLE__"] = "suexttau";
      gmao_vars["suexttau"] = new MVarDesc( "suexttau", 3, "SO4 Extinction AOT [550 nm] __ENSEMBLE__", "so4 extinction aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["tsoil5"] = "soil_temperatures_layer_5";
      gmao_names["soil_temperatures_layer_5"] = "tsoil5";
      gmao_vars["tsoil5"] = new MVarDesc( "tsoil5", 3, "soil_temperatures_layer_5", "soil_temperatures_layer_5", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["tsoil4"] = "soil_temperatures_layer_4";
      gmao_names["soil_temperatures_layer_4"] = "tsoil4";
      gmao_vars["tsoil4"] = new MVarDesc( "tsoil4", 3, "soil_temperatures_layer_4", "soil_temperatures_layer_4", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["tsoil6"] = "soil_temperatures_layer_6";
      gmao_names["soil_temperatures_layer_6"] = "tsoil6";
      gmao_vars["tsoil6"] = new MVarDesc( "tsoil6", 3, "soil_temperatures_layer_6", "soil_temperatures_layer_6", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["tsoil1"] = "soil_temperatures_layer_1";
      gmao_names["soil_temperatures_layer_1"] = "tsoil1";
      gmao_vars["tsoil1"] = new MVarDesc( "tsoil1", 3, "soil_temperatures_layer_1", "soil_temperatures_layer_1", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["q250"] = "specific_humidity_at_250_hPa";
      gmao_names["specific_humidity_at_250_hPa"] = "q250";
      gmao_vars["q250"] = new MVarDesc( "q250", 3, "specific_humidity_at_250_hPa", "specific_humidity_at_250_hpa", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["tsoil2"] = "soil_temperatures_layer_2";
      gmao_names["soil_temperatures_layer_2"] = "tsoil2";
      gmao_vars["tsoil2"] = new MVarDesc( "tsoil2", 3, "soil_temperatures_layer_2", "soil_temperatures_layer_2", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["ss003"] = "Sea Salt Mixing Ratio (bin 003)";
      gmao_names["Sea Salt Mixing Ratio (bin 003)"] = "ss003";
      gmao_vars["ss003"] = new MVarDesc( "ss003", 4, "Sea Salt Mixing Ratio (bin 003)", "sea salt mixing ratio (bin 003)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["ss002"] = "Sea Salt Mixing Ratio (bin 002)";
      gmao_names["Sea Salt Mixing Ratio (bin 002)"] = "ss002";
      gmao_vars["ss002"] = new MVarDesc( "ss002", 4, "Sea Salt Mixing Ratio (bin 002)", "sea salt mixing ratio (bin 002)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["niangstr"] = new MVarDesc( "niangstr", 3, "", "nitrate angstrom parameter [470-870 nm] __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["cocl"] = "CO Column Burden __ENSEMBLE__";
      gmao_names["CO Column Burden __ENSEMBLE__"] = "cocl";
      gmao_vars["cocl"] = new MVarDesc( "cocl", 3, "CO Column Burden __ENSEMBLE__", "co column burden __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      cf_names["ss005"] = "Sea Salt Mixing Ratio (bin 005)";
      gmao_names["Sea Salt Mixing Ratio (bin 005)"] = "ss005";
      gmao_vars["ss005"] = new MVarDesc( "ss005", 4, "Sea Salt Mixing Ratio (bin 005)", "sea salt mixing ratio (bin 005)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["ss004"] = "Sea Salt Mixing Ratio (bin 004)";
      gmao_names["Sea Salt Mixing Ratio (bin 004)"] = "ss004";
      gmao_vars["ss004"] = new MVarDesc( "ss004", 4, "Sea Salt Mixing Ratio (bin 004)", "sea salt mixing ratio (bin 004)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["t2mmin"] = "2-meter_air_temperature";
      gmao_names["2-meter_air_temperature"] = "t2mmin";
      gmao_vars["t2mmin"] = new MVarDesc( "t2mmin", 3, "2-meter_air_temperature", "2-meter_air_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0" );

      cf_names["kmls"] = "momentum_diffusivity_from_Louis";
      gmao_names["momentum_diffusivity_from_Louis"] = "kmls";
      gmao_vars["kmls"] = new MVarDesc( "kmls", 4, "momentum_diffusivity_from_Louis", "momentum_diffusivity_from_louis", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      gmao_vars["so4mass"] = new MVarDesc( "so4mass", 4, "", "so4 aerosol mass mixing ratio __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0" );

      cf_names["supso2"] = "SO2 Prod from DMS Oxidation [column] __ENSEMBLE__";
      gmao_names["SO2 Prod from DMS Oxidation [column] __ENSEMBLE__"] = "supso2";
      gmao_vars["supso2"] = new MVarDesc( "supso2", 3, "SO2 Prod from DMS Oxidation [column] __ENSEMBLE__", "so2 prod from dms oxidation [column] __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["kmlk"] = "entrainment_momentum_diffusivity_from_Lock";
      gmao_names["entrainment_momentum_diffusivity_from_Lock"] = "kmlk";
      gmao_vars["kmlk"] = new MVarDesc( "kmlk", 4, "entrainment_momentum_diffusivity_from_Lock", "entrainment_momentum_diffusivity_from_lock", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["qlml"] = "surface_specific_humidity";
      gmao_names["surface_specific_humidity"] = "qlml";
      gmao_vars["qlml"] = new MVarDesc( "qlml", 3, "surface_specific_humidity", "surface_specific_humidity", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_lfo_Nx,inst1_2d_lfo_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["dudp001"] = "Dust Dry Deposition Bin 001";
      gmao_names["Dust Dry Deposition Bin 001"] = "dudp001";
      gmao_vars["dudp001"] = new MVarDesc( "dudp001", 3, "Dust Dry Deposition Bin 001", "dust dry deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dudp002"] = "Dust Dry Deposition Bin 002";
      gmao_names["Dust Dry Deposition Bin 002"] = "dudp002";
      gmao_vars["dudp002"] = new MVarDesc( "dudp002", 3, "Dust Dry Deposition Bin 002", "dust dry deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dudp003"] = "Dust Dry Deposition Bin 003";
      gmao_names["Dust Dry Deposition Bin 003"] = "dudp003";
      gmao_vars["dudp003"] = new MVarDesc( "dudp003", 3, "Dust Dry Deposition Bin 003", "dust dry deposition bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dudp004"] = "Dust Dry Deposition Bin 004";
      gmao_names["Dust Dry Deposition Bin 004"] = "dudp004";
      gmao_vars["dudp004"] = new MVarDesc( "dudp004", 3, "Dust Dry Deposition Bin 004", "dust dry deposition bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dudp005"] = "Dust Dry Deposition Bin 005";
      gmao_names["Dust Dry Deposition Bin 005"] = "dudp005";
      gmao_vars["dudp005"] = new MVarDesc( "dudp005", 3, "Dust Dry Deposition Bin 005", "dust dry deposition bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dvdtana"] = "total_northward_wind_analysis_tendency";
      gmao_names["total_northward_wind_analysis_tendency"] = "dvdtana";
      gmao_vars["dvdtana"] = new MVarDesc( "dvdtana", 4, "total_northward_wind_analysis_tendency", "total_northward_wind_analysis_tendency", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["tql"] = "total_precipitable_liquid_water";
      gmao_names["total_precipitable_liquid_water"] = "tql";
      gmao_vars["tql"] = new MVarDesc( "tql", 3, "total_precipitable_liquid_water", "total_precipitable_liquid_water", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["nh3wt"] = new MVarDesc( "nh3wt", 3, "", "ammonia wet deposition __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["so2eman"] = "SO2 Anthropogenic Emissions __ENSEMBLE__";
      gmao_names["SO2 Anthropogenic Emissions __ENSEMBLE__"] = "so2eman";
      gmao_vars["so2eman"] = new MVarDesc( "so2eman", 3, "SO2 Anthropogenic Emissions __ENSEMBLE__", "so2 anthropogenic emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["tqi"] = "total_precipitable_ice_water";
      gmao_names["total_precipitable_ice_water"] = "tqi";
      gmao_vars["tqi"] = new MVarDesc( "tqi", 3, "total_precipitable_ice_water", "total_precipitable_ice_water", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["swtntcln"] = "toa_net_downward_shortwave_flux_assuming_no_aerosol";
      gmao_names["toa_net_downward_shortwave_flux_assuming_no_aerosol"] = "swtntcln";
      gmao_vars["swtntcln"] = new MVarDesc( "swtntcln", 3, "toa_net_downward_shortwave_flux_assuming_no_aerosol", "toa_net_downward_shortwave_flux_assuming_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["tqv"] = "total_precipitable_water_vapor";
      gmao_names["total_precipitable_water_vapor"] = "tqv";
      gmao_vars["tqv"] = new MVarDesc( "tqv", 3, "total_precipitable_water_vapor", "total_precipitable_water_vapor", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["lwgabclr"] = "surface_absorbed_longwave_radiation_assuming_clear_sky";
      gmao_names["surface_absorbed_longwave_radiation_assuming_clear_sky"] = "lwgabclr";
      gmao_vars["lwgabclr"] = new MVarDesc( "lwgabclr", 3, "surface_absorbed_longwave_radiation_assuming_clear_sky", "surface_absorbed_longwave_radiation_assuming_clear_sky", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["copdnbas"] = new MVarDesc( "copdnbas", 3, "", "co chemical production (asia non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["dmscmass"] = "DMS Column Mass Density __ENSEMBLE__";
      gmao_names["DMS Column Mass Density __ENSEMBLE__"] = "dmscmass";
      gmao_vars["dmscmass"] = new MVarDesc( "dmscmass", 3, "DMS Column Mass Density __ENSEMBLE__", "dms column mass density __ensemble__", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["swtntclr"] = "toa_net_downward_shortwave_flux_assuming_clear_sky";
      gmao_names["toa_net_downward_shortwave_flux_assuming_clear_sky"] = "swtntclr";
      gmao_vars["swtntclr"] = new MVarDesc( "swtntclr", 3, "toa_net_downward_shortwave_flux_assuming_clear_sky", "toa_net_downward_shortwave_flux_assuming_clear_sky", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["coclbbaf"] = new MVarDesc( "coclbbaf", 3, "", "co column burden (africa biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["nicmass25"] = new MVarDesc( "nicmass25", 3, "", "nitrate column mass density [pm2.5] __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["spwatr"] = "rate_of_spurious_land_water_source";
      gmao_names["rate_of_spurious_land_water_source"] = "spwatr";
      gmao_vars["spwatr"] = new MVarDesc( "spwatr", 3, "rate_of_spurious_land_water_source", "rate_of_spurious_land_water_source", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["duscatau"] = "Dust Scattering AOT [550 nm]";
      gmao_names["Dust Scattering AOT [550 nm]"] = "duscatau";
      gmao_vars["duscatau"] = new MVarDesc( "duscatau", 3, "Dust Scattering AOT [550 nm]", "dust scattering aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dqvdtana"] = "total_specific_humidity_analysis_tendency";
      gmao_names["total_specific_humidity_analysis_tendency"] = "dqvdtana";
      gmao_vars["dqvdtana"] = new MVarDesc( "dqvdtana", 4, "total_specific_humidity_analysis_tendency", "total_specific_humidity_vapor_analysis_tendency", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["so4eman"] = "SO4 Anthropogenic Emissions __ENSEMBLE__";
      gmao_names["SO4 Anthropogenic Emissions __ENSEMBLE__"] = "so4eman";
      gmao_vars["so4eman"] = new MVarDesc( "so4eman", 3, "SO4 Anthropogenic Emissions __ENSEMBLE__", "so4 anthropogenic emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["dusv"] = new MVarDesc( "dusv", 3, "", "dust_tendency_due_to_conv_scav", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["snodp"] = "snow_depth";
      gmao_names["snow_depth"] = "snodp";
      gmao_vars["snodp"] = new MVarDesc( "snodp", 3, "snow_depth", "snow_depth", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["nisd001"] = new MVarDesc( "nisd001", 3, "", "nitrate sedimentation bin 001 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["h1000"] = "height_at_1000_mb";
      gmao_names["height_at_1000_mb"] = "h1000";
      gmao_vars["h1000"] = new MVarDesc( "h1000", 3, "height_at_1000_mb", "height_at_1000_mb", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["epv"] = "ertels_potential_vorticity";
      gmao_names["ertels_potential_vorticity"] = "epv";
      gmao_vars["epv"] = new MVarDesc( "epv", 4, "ertels_potential_vorticity", "ertels_potential_vorticity", "K m+2 kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      cf_names["cdq"] = "surface_exchange_coefficient_for_moisture";
      gmao_names["surface_exchange_coefficient_for_moisture"] = "cdq";
      gmao_vars["cdq"] = new MVarDesc( "cdq", 3, "surface_exchange_coefficient_for_moisture", "surface_exchange_coefficient_for_moisture", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["swtntclrcln"] = "toa_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol";
      gmao_names["toa_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol"] = "swtntclrcln";
      gmao_vars["swtntclrcln"] = new MVarDesc( "swtntclrcln", 3, "toa_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol", "toa_net_downward_shortwave_flux_assuming_clear_sky_and_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["coembbla"] = new MVarDesc( "coembbla", 3, "", "co emission (central and south america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["niscatfm"] = new MVarDesc( "niscatfm", 3, "", "nitrate scattering aot [550 nm] - pm 1.0 um __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["swwarm"] = new MVarDesc( "swwarm", 3, "", "solar_heating_in_warm_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["frunst"] = "fractional_area_of_unsaturated_zone";
      gmao_names["fractional_area_of_unsaturated_zone"] = "frunst";
      gmao_vars["frunst"] = new MVarDesc( "frunst", 3, "fractional_area_of_unsaturated_zone", "fractional_area_of_unsaturated_zone", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["duexttfm"] = "Dust Extinction AOT [550 nm] - PM 1.0 um";
      gmao_names["Dust Extinction AOT [550 nm] - PM 1.0 um"] = "duexttfm";
      gmao_vars["duexttfm"] = new MVarDesc( "duexttfm", 3, "Dust Extinction AOT [550 nm] - PM 1.0 um", "dust extinction aot [550 nm] - pm 1.0 um __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssextt25"] = "Sea Salt Extinction AOT [550 nm] - PM 2.5";
      gmao_names["Sea Salt Extinction AOT [550 nm] - PM 2.5"] = "ssextt25";
      gmao_vars["ssextt25"] = new MVarDesc( "ssextt25", 3, "Sea Salt Extinction AOT [550 nm] - PM 2.5", "sea salt extinction aot [550 nm] - pm 2.5 __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["tskinwtr"] = "open_water_skin_temperature";
      gmao_names["open_water_skin_temperature"] = "tskinwtr";
      gmao_vars["tskinwtr"] = new MVarDesc( "tskinwtr", 3, "open_water_skin_temperature", "open_water_skin_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["mfz"] = new MVarDesc( "mfz", 4, "", "vertical_mass_flux", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Ne,tavg3_3d_lsf_Ne.%s,0,3,3,3,90" );

      cf_names["khsfc"] = "surface_driven_scalar_diffusivity_from_Lock_scheme";
      gmao_names["surface_driven_scalar_diffusivity_from_Lock_scheme"] = "khsfc";
      gmao_vars["khsfc"] = new MVarDesc( "khsfc", 4, "surface_driven_scalar_diffusivity_from_Lock_scheme", "surface_driven_scalar_diffusivity_from_lock_scheme", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["reevapcn"] = "evap_subl_of_convective_precipitation";
      gmao_names["evap_subl_of_convective_precipitation"] = "reevapcn";
      gmao_vars["reevapcn"] = new MVarDesc( "reevapcn", 4, "evap_subl_of_convective_precipitation", "evap_subl_of_convective_precipitation", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90" );

      gmao_vars["qccu"] = new MVarDesc( "qccu", 4, "", "grid_mean_convective_condensate", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Cp,tavg3_3d_cld_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      cf_names["speedlml"] = "surface_wind_speed";
      gmao_names["surface_wind_speed"] = "speedlml";
      gmao_vars["speedlml"] = new MVarDesc( "speedlml", 3, "surface_wind_speed", "surface_wind_speed", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_lfo_Nx,inst1_2d_lfo_Nx.%s,0,0,1,0,0" );

      cf_names["ocfluxu"] = "Organic Carbon column u-wind mass flux __ENSEMBLE__";
      gmao_names["Organic Carbon column u-wind mass flux __ENSEMBLE__"] = "ocfluxu";
      gmao_vars["ocfluxu"] = new MVarDesc( "ocfluxu", 3, "Organic Carbon column u-wind mass flux __ENSEMBLE__", "organic carbon column u-wind mass flux __ensemble__", "kg m-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["bcembf"] = "Black Carbon Biofuel Emissions";
      gmao_names["Black Carbon Biofuel Emissions"] = "bcembf";
      gmao_vars["bcembf"] = new MVarDesc( "bcembf", 3, "Black Carbon Biofuel Emissions", "black carbon biofuel emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bcembb"] = "Black Carbon Biomass Burning Emissions";
      gmao_names["Black Carbon Biomass Burning Emissions"] = "bcembb";
      gmao_vars["bcembb"] = new MVarDesc( "bcembb", 3, "Black Carbon Biomass Burning Emissions", "black carbon biomass burning emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["colsbbna"] = new MVarDesc( "colsbbna", 3, "", "co chemical loss (north america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["cobbla"] = new MVarDesc( "cobbla", 4, "", "co central and south america biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["duexttau"] = "Dust Extinction AOT [550 nm]";
      gmao_names["Dust Extinction AOT [550 nm]"] = "duexttau";
      gmao_vars["duexttau"] = new MVarDesc( "duexttau", 3, "Dust Extinction AOT [550 nm]", "dust extinction aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dqldtmst"] = "total_liq_water_tendency_due_to_moist";
      gmao_names["total_liq_water_tendency_due_to_moist"] = "dqldtmst";
      gmao_vars["dqldtmst"] = new MVarDesc( "dqldtmst", 4, "total_liq_water_tendency_due_to_moist", "total_liq_water_tendency_due_to_moist", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["dtdtlwr"] = "air_temperature_tendency_due_to_longwave";
      gmao_names["air_temperature_tendency_due_to_longwave"] = "dtdtlwr";
      gmao_vars["dtdtlwr"] = new MVarDesc( "dtdtlwr", 4, "air_temperature_tendency_due_to_longwave", "air_temperature_tendency_due_to_longwave", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Cp,tavg3_3d_rad_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90" );

      gmao_vars["nifluxv"] = new MVarDesc( "nifluxv", 3, "", "nitrate column v-wind mass flux __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["conbeu"] = new MVarDesc( "conbeu", 4, "", "co european non-biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["so2smass"] = "SO2 Surface Mass Concentration __ENSEMBLE__";
      gmao_names["SO2 Surface Mass Concentration __ENSEMBLE__"] = "so2smass";
      gmao_vars["so2smass"] = new MVarDesc( "so2smass", 3, "SO2 Surface Mass Concentration __ENSEMBLE__", "so2 surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dqvdttrb"] = "tendency_of_specific_humidity_due_to_turbulence";
      gmao_names["tendency_of_specific_humidity_due_to_turbulence"] = "dqvdttrb";
      gmao_vars["dqvdttrb"] = new MVarDesc( "dqvdttrb", 4, "tendency_of_specific_humidity_due_to_turbulence", "tendency_of_specific_humidity_due_to_turbulence", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      gmao_vars["colsbbgl"] = new MVarDesc( "colsbbgl", 3, "", "co chemical loss (global biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["grn"] = "greeness_fraction";
      gmao_names["greeness_fraction"] = "grn";
      gmao_vars["grn"] = new MVarDesc( "grn", 3, "greeness_fraction", "greenness_fraction", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["dcool"] = new MVarDesc( "dcool", 3, "", "depth_of_cool_layer", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["ustar"] = "surface_velocity_scale";
      gmao_names["surface_velocity_scale"] = "ustar";
      gmao_vars["ustar"] = new MVarDesc( "ustar", 3, "surface_velocity_scale", "surface_velocity_scale", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["zle"] = "edge_heights";
      gmao_names["edge_heights"] = "zle";
      gmao_vars["zle"] = new MVarDesc( "zle", 4, "edge_heights", "edge_heights", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_nav_Ne,tavg3_3d_nav_Ne.%s,0,3,3,3,90" );

      cf_names["dtrain"] = "detraining_mass_flux";
      gmao_names["detraining_mass_flux"] = "dtrain";
      gmao_vars["dtrain"] = new MVarDesc( "dtrain", 4, "detraining_mass_flux", "detraining_mass_flux", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90" );

      gmao_vars["copdnbgl"] = new MVarDesc( "copdnbgl", 3, "", "co chemical production (global non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["hournorain"] = "time-during_an_hour_with_no_precipitation";
      gmao_names["time-during_an_hour_with_no_precipitation"] = "hournorain";
      gmao_vars["hournorain"] = new MVarDesc( "hournorain", 3, "time-during_an_hour_with_no_precipitation", "time-during_an_hour_with_no_precipitation", "s", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0" );

      cf_names["z0h"] = "surface_roughness_for_heat";
      gmao_names["surface_roughness_for_heat"] = "z0h";
      gmao_vars["z0h"] = new MVarDesc( "z0h", 3, "surface_roughness_for_heat", "surface_roughness_for_heat", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["z0m"] = "surface_roughness";
      gmao_names["surface_roughness"] = "z0m";
      gmao_vars["z0m"] = new MVarDesc( "z0m", 3, "surface_roughness", "surface_roughness", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["t"] = "air_temperature";
      gmao_names["air_temperature"] = "t";
      gmao_vars["t"] = new MVarDesc( "t", 4, "air_temperature", "air_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      cf_names["tropq"] = "tropopause_specific_humidity_using_blended_TROPP_estimate";
      gmao_names["tropopause_specific_humidity_using_blended_TROPP_estimate"] = "tropq";
      gmao_vars["tropq"] = new MVarDesc( "tropq", 3, "tropopause_specific_humidity_using_blended_TROPP_estimate", "tropopause_specific_humidity_using_blended_tropp_estimate", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["frcls"] = "areal_fraction_of_nonanvil_large_scale_showers";
      gmao_names["areal_fraction_of_nonanvil_large_scale_showers"] = "frcls";
      gmao_vars["frcls"] = new MVarDesc( "frcls", 3, "areal_fraction_of_nonanvil_large_scale_showers", "areal_fraction_of_nonanvil_large_scale_showers", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["supso4wt"] = "SO4 Prod from Aqueous SO2 Oxidation (wet dep) [column] __ENSEMBLE__";
      gmao_names["SO4 Prod from Aqueous SO2 Oxidation (wet dep) [column] __ENSEMBLE__"] = "supso4wt";
      gmao_vars["supso4wt"] = new MVarDesc( "supso4wt", 3, "SO4 Prod from Aqueous SO2 Oxidation (wet dep) [column] __ENSEMBLE__", "so4 prod from aqueous so2 oxidation (wet dep) [column] __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssem003"] = "Sea Salt Emission Bin 003";
      gmao_names["Sea Salt Emission Bin 003"] = "ssem003";
      gmao_vars["ssem003"] = new MVarDesc( "ssem003", 3, "Sea Salt Emission Bin 003", "sea salt emission bin 003 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssem002"] = "Sea Salt Emission Bin 002";
      gmao_names["Sea Salt Emission Bin 002"] = "ssem002";
      gmao_vars["ssem002"] = new MVarDesc( "ssem002", 3, "Sea Salt Emission Bin 002", "sea salt emission bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssem001"] = "Sea Salt Emission Bin 001";
      gmao_names["Sea Salt Emission Bin 001"] = "ssem001";
      gmao_vars["ssem001"] = new MVarDesc( "ssem001", 3, "Sea Salt Emission Bin 001", "sea salt emission bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bcscatau"] = "Black Carbon Scattering AOT [550 nm]";
      gmao_names["Black Carbon Scattering AOT [550 nm]"] = "bcscatau";
      gmao_vars["bcscatau"] = new MVarDesc( "bcscatau", 3, "Black Carbon Scattering AOT [550 nm]", "black carbon scattering aot [550 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["ssem004"] = "Sea Salt Emission Bin 004";
      gmao_names["Sea Salt Emission Bin 004"] = "ssem004";
      gmao_vars["ssem004"] = new MVarDesc( "ssem004", 3, "Sea Salt Emission Bin 004", "sea salt emission bin 004 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["coscbbla"] = new MVarDesc( "coscbbla", 3, "", "co surface concentration in ppbv (central and south america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["precsnoocn"] = "ocean_snowfall";
      gmao_names["ocean_snowfall"] = "precsnoocn";
      gmao_vars["precsnoocn"] = new MVarDesc( "precsnoocn", 3, "ocean_snowfall", "ocean_snowfall", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["duscatfm"] = "Dust Scattering AOT [550 nm] - PM 1.0 um";
      gmao_names["Dust Scattering AOT [550 nm] - PM 1.0 um"] = "duscatfm";
      gmao_vars["duscatfm"] = new MVarDesc( "duscatfm", 3, "Dust Scattering AOT [550 nm] - PM 1.0 um", "dust scattering aot [550 nm] - pm 1.0 um __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["shland"] = "Sensible_heat_flux_land";
      gmao_names["Sensible_heat_flux_land"] = "shland";
      gmao_vars["shland"] = new MVarDesc( "shland", 3, "Sensible_heat_flux_land", "sensible_heat_flux_land", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["swtnt"] = "toa_net_downward_shortwave_flux";
      gmao_names["toa_net_downward_shortwave_flux"] = "swtnt";
      gmao_vars["swtnt"] = new MVarDesc( "swtnt", 3, "toa_net_downward_shortwave_flux", "toa_net_downward_shortwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["nh3sv"] = new MVarDesc( "nh3sv", 3, "", "ammonia convective scavenging __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["so2embb"] = "SO2 Biomass Burning Emissions __ENSEMBLE__";
      gmao_names["SO2 Biomass Burning Emissions __ENSEMBLE__"] = "so2embb";
      gmao_vars["so2embb"] = new MVarDesc( "so2embb", 3, "SO2 Biomass Burning Emissions __ENSEMBLE__", "so2 biomass burning emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["nisv003"] = new MVarDesc( "nisv003", 3, "", "nitrate convective scavenging bin 003 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ssdp005"] = "Sea Salt Dry Deposition Bin 005";
      gmao_names["Sea Salt Dry Deposition Bin 005"] = "ssdp005";
      gmao_vars["ssdp005"] = new MVarDesc( "ssdp005", 3, "Sea Salt Dry Deposition Bin 005", "sea salt dry deposition bin 005 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["nisv001"] = new MVarDesc( "nisv001", 3, "", "nitrate convective scavenging bin 001 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["coembbaf"] = new MVarDesc( "coembbaf", 3, "", "co emission (africa biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["coembbae"] = new MVarDesc( "coembbae", 3, "", "co emission (asia and europe biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["nicmass"] = new MVarDesc( "nicmass", 3, "", "nitrate column mass density __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["dvdtdyn"] = "tendency_of_northward_wind_due_to_dynamics";
      gmao_names["tendency_of_northward_wind_due_to_dynamics"] = "dvdtdyn";
      gmao_vars["dvdtdyn"] = new MVarDesc( "dvdtdyn", 4, "tendency_of_northward_wind_due_to_dynamics", "tendency_of_northward_wind_due_to_dynamics", "m/s/s", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["ocwt002"] = "Organic Carbon Wet Deposition Bin 002 __ENSEMBLE__";
      gmao_names["Organic Carbon Wet Deposition Bin 002 __ENSEMBLE__"] = "ocwt002";
      gmao_vars["ocwt002"] = new MVarDesc( "ocwt002", 3, "Organic Carbon Wet Deposition Bin 002 __ENSEMBLE__", "organic carbon wet deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["o3"] = "ozone_mass_mixing_ratio";
      gmao_names["ozone_mass_mixing_ratio"] = "o3";
      gmao_vars["o3"] = new MVarDesc( "o3", 4, "ozone_mass_mixing_ratio", "ozone_mass_mixing_ratio", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90" );

      cf_names["bcdp001"] = "Black Carbon Dry Deposition Bin 001";
      gmao_names["Black Carbon Dry Deposition Bin 001"] = "bcdp001";
      gmao_vars["bcdp001"] = new MVarDesc( "bcdp001", 3, "Black Carbon Dry Deposition Bin 001", "black carbon dry deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bcdp002"] = "Black Carbon Dry Deposition Bin 002";
      gmao_names["Black Carbon Dry Deposition Bin 002"] = "bcdp002";
      gmao_vars["bcdp002"] = new MVarDesc( "bcdp002", 3, "Black Carbon Dry Deposition Bin 002", "black carbon dry deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["evptrns"] = "transpiration_energy_flux";
      gmao_names["transpiration_energy_flux"] = "evptrns";
      gmao_vars["evptrns"] = new MVarDesc( "evptrns", 3, "transpiration_energy_flux", "transpiration_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["taulow"] = "in_cloud_optical_thickness_of_low_clouds";
      gmao_names["in_cloud_optical_thickness_of_low_clouds"] = "taulow";
      gmao_vars["taulow"] = new MVarDesc( "taulow", 3, "in_cloud_optical_thickness_of_low_clouds", "in_cloud_optical_thickness_of_low_clouds", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["nh4dp"] = new MVarDesc( "nh4dp", 3, "", "ammonium dry deposition __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["cobbaf"] = new MVarDesc( "cobbaf", 4, "", "co africa biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      gmao_vars["cobbae"] = new MVarDesc( "cobbae", 4, "", "co asia and europe biomass burning", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_tag_Nv,inst3_3d_tag_Nv.%s,0,1,3,0,0" );

      cf_names["supmsa"] = "MSA Prod from DMS Oxidation [column] __ENSEMBLE__";
      gmao_names["MSA Prod from DMS Oxidation [column] __ENSEMBLE__"] = "supmsa";
      gmao_vars["supmsa"] = new MVarDesc( "supmsa", 3, "MSA Prod from DMS Oxidation [column] __ENSEMBLE__", "msa prod from dms oxidation [column] __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["frlandice"] = new MVarDesc( "frlandice", 3, "", "fraction_of_land_ice", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["ocsmass"] = "Organic Carbon Surface Mass Concentration __ENSEMBLE__";
      gmao_names["Organic Carbon Surface Mass Concentration __ENSEMBLE__"] = "ocsmass";
      gmao_vars["ocsmass"] = new MVarDesc( "ocsmass", 3, "Organic Carbon Surface Mass Concentration __ENSEMBLE__", "organic carbon surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      gmao_vars["colsnbeu"] = new MVarDesc( "colsnbeu", 3, "", "co chemical loss (european non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["ple"] = "edge_pressure";
      gmao_names["edge_pressure"] = "ple";
      gmao_vars["ple"] = new MVarDesc( "ple", 4, "edge_pressure", "edge_pressure", "Pa", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Ne,tavg3_3d_lsf_Ne.%s,0,3,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_nav_Ne,tavg3_3d_nav_Ne.%s,0,3,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      gmao_vars["evapout"] = new MVarDesc( "evapout", 3, "", "evaporation", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["sssmass"] = "Sea Salt Surface Mass Concentration";
      gmao_names["Sea Salt Surface Mass Concentration"] = "sssmass";
      gmao_vars["sssmass"] = new MVarDesc( "sssmass", 3, "Sea Salt Surface Mass Concentration", "sea salt surface mass concentration __ensemble__", "kg m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["cldtmp"] = "cloud_top_temperature";
      gmao_names["cloud_top_temperature"] = "cldtmp";
      gmao_vars["cldtmp"] = new MVarDesc( "cldtmp", 3, "cloud_top_temperature", "cloud_top_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["colsnbna"] = new MVarDesc( "colsnbna", 3, "", "co chemical loss (north american non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["nh3em"] = new MVarDesc( "nh3em", 3, "", "ammonia emission __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["h850"] = "height_at_850_hPa";
      gmao_names["height_at_850_hPa"] = "h850";
      gmao_vars["h850"] = new MVarDesc( "h850", 3, "height_at_850_hPa", "height_at_850_hpa", "m", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["swtdn"] = "toa_incoming_shortwave_flux";
      gmao_names["toa_incoming_shortwave_flux"] = "swtdn";
      gmao_vars["swtdn"] = new MVarDesc( "swtdn", 3, "toa_incoming_shortwave_flux", "toa_incoming_shortwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["ocangstr"] = "Organic Carbon Angstrom parameter [470-870 nm] __ENSEMBLE__";
      gmao_names["Organic Carbon Angstrom parameter [470-870 nm] __ENSEMBLE__"] = "ocangstr";
      gmao_vars["ocangstr"] = new MVarDesc( "ocangstr", 3, "Organic Carbon Angstrom parameter [470-870 nm] __ENSEMBLE__", "organic carbon angstrom parameter [470-870 nm] __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["frseaice"] = "ice_covered_fraction_of_tile";
      gmao_names["ice_covered_fraction_of_tile"] = "frseaice";
      gmao_vars["frseaice"] = new MVarDesc( "frseaice", 3, "ice_covered_fraction_of_tile", "ice_covered_fraction_of_tile", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      cf_names["ocphilic"] = "Hydrophilic Organic Carbon (Particulate Matter)";
      gmao_names["Hydrophilic Organic Carbon (Particulate Matter)"] = "ocphilic";
      gmao_vars["ocphilic"] = new MVarDesc( "ocphilic", 4, "Hydrophilic Organic Carbon (Particulate Matter)", "hydrophilic organic carbon (particulate matter)", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["spsnow"] = "rate_of_spurious_snow_energy";
      gmao_names["rate_of_spurious_snow_energy"] = "spsnow";
      gmao_vars["spsnow"] = new MVarDesc( "spsnow", 3, "rate_of_spurious_snow_energy", "rate_of_spurious_snow_energy", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["sfmc"] = "water_surface_layer";
      gmao_names["water_surface_layer"] = "sfmc";
      gmao_vars["sfmc"] = new MVarDesc( "sfmc", 3, "water_surface_layer", "water_surface_layer", "m-3 m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["co2cl"] = new MVarDesc( "co2cl", 3, "", "co2 bulk mixing ratio (column mass/ps) bin 001", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90" );

      gmao_vars["bcsv"] = new MVarDesc( "bcsv", 3, "", "black_carbon_tendency_due_to_conv_scav", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["reevaplsan"] = "evap_subl_of_non_convective_precipitation";
      gmao_names["evap_subl_of_non_convective_precipitation"] = "reevaplsan";
      gmao_vars["reevaplsan"] = new MVarDesc( "reevaplsan", 4, "evap_subl_of_non_convective_precipitation", "evap_subl_of_non_convective_precipitation", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Cp,tavg3_3d_mst_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90" );

      cf_names["preccu"] = "convective_rainfall";
      gmao_names["convective_rainfall"] = "preccu";
      gmao_vars["preccu"] = new MVarDesc( "preccu", 3, "convective_rainfall", "liquid_water_convective_precipitation", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30" );

      cf_names["u850"] = "eastward_wind_at_850_hPa";
      gmao_names["eastward_wind_at_850_hPa"] = "u850";
      gmao_vars["u850"] = new MVarDesc( "u850", 3, "eastward_wind_at_850_hPa", "eastward_wind_at_850_hpa", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["ssscat25"] = "Sea Salt Scattering AOT [550 nm] - PM 2.5";
      gmao_names["Sea Salt Scattering AOT [550 nm] - PM 2.5"] = "ssscat25";
      gmao_vars["ssscat25"] = new MVarDesc( "ssscat25", 3, "Sea Salt Scattering AOT [550 nm] - PM 2.5", "sea salt scattering aot [550 nm] - pm 2.5 __ensemble__", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["taugwx"] = "surface_eastward_gravity_wave_stress";
      gmao_names["surface_eastward_gravity_wave_stress"] = "taugwx";
      gmao_vars["taugwx"] = new MVarDesc( "taugwx", 3, "surface_eastward_gravity_wave_stress", "surface_eastward_gravity_wave_stress", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["taugwy"] = "surface_northward_gravity_wave_stress";
      gmao_names["surface_northward_gravity_wave_stress"] = "taugwy";
      gmao_vars["taugwy"] = new MVarDesc( "taugwy", 3, "surface_northward_gravity_wave_stress", "surface_northward_gravity_wave_stress", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["delts"] = new MVarDesc( "delts", 3, "", "change_of_surface_skin_temperature", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["colsbbla"] = new MVarDesc( "colsbbla", 3, "", "co chemical loss (central and south america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["dqldttrb"] = "tendency_of_liquid_condensate_due_to_turbulence";
      gmao_names["tendency_of_liquid_condensate_due_to_turbulence"] = "dqldttrb";
      gmao_vars["dqldttrb"] = new MVarDesc( "dqldttrb", 4, "tendency_of_liquid_condensate_due_to_turbulence", "tendency_of_liquid_condensate_due_to_turbulence", "kg kg-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["so2"] = "Sulphur dioxide";
      gmao_names["Sulphur dioxide"] = "so2";
      gmao_vars["so2"] = new MVarDesc( "so2", 4, "Sulphur dioxide", "sulphur dioxide", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["so4"] = "Sulphate aerosol";
      gmao_names["Sulphate aerosol"] = "so4";
      gmao_vars["so4"] = new MVarDesc( "so4", 4, "Sulphate aerosol", "sulphate aerosol", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      cf_names["lwgabclrcln"] = "surface_absorbed_longwave_radiation_assuming_clear_sky_and_no_aerosol";
      gmao_names["surface_absorbed_longwave_radiation_assuming_clear_sky_and_no_aerosol"] = "lwgabclrcln";
      gmao_vars["lwgabclrcln"] = new MVarDesc( "lwgabclrcln", 3, "surface_absorbed_longwave_radiation_assuming_clear_sky_and_no_aerosol", "surface_absorbed_longwave_radiation_assuming_clear_sky_and_no_aerosol", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["tsurf"] = "surface_temperature_of_land_incl_snow";
      gmao_names["surface_temperature_of_land_incl_snow"] = "tsurf";
      gmao_vars["tsurf"] = new MVarDesc( "tsurf", 3, "surface_temperature_of_land_incl_snow", "surface_temperature_of_land_incl_snow", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["snomas"] = "Total_snow_storage_land";
      gmao_names["Total_snow_storage_land"] = "snomas";
      gmao_vars["snomas"] = new MVarDesc( "snomas", 3, "Total_snow_storage_land", "total_snow_storage_land", "kg m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["qstar"] = "surface_moisture_scale";
      gmao_names["surface_moisture_scale"] = "qstar";
      gmao_vars["qstar"] = new MVarDesc( "qstar", 3, "surface_moisture_scale", "surface_moisture_scale", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["dqvdtdyn"] = "tendency_of_specific_humidity_due_to_dynamics";
      gmao_names["tendency_of_specific_humidity_due_to_dynamics"] = "dqvdtdyn";
      gmao_vars["dqvdtdyn"] = new MVarDesc( "dqvdtdyn", 4, "tendency_of_specific_humidity_due_to_dynamics", "tendency_of_specific_humidity_due_to_dynamics", "kg/kg/s", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Cp,tavg3_3d_qdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90" );

      cf_names["albvisdr"] = "surface_albedo_for_visible_beam";
      gmao_names["surface_albedo_for_visible_beam"] = "albvisdr";
      gmao_vars["albvisdr"] = new MVarDesc( "albvisdr", 3, "surface_albedo_for_visible_beam", "surface_albedo_for_visible_beam", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["nh3cmass"] = new MVarDesc( "nh3cmass", 3, "", "ammonia column mass density __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["ssdp002"] = "Sea Salt Dry Deposition Bin 002";
      gmao_names["Sea Salt Dry Deposition Bin 002"] = "ssdp002";
      gmao_vars["ssdp002"] = new MVarDesc( "ssdp002", 3, "Sea Salt Dry Deposition Bin 002", "sea salt dry deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["albvisdf"] = "surface_albedo_for_visible_diffuse";
      gmao_names["surface_albedo_for_visible_diffuse"] = "albvisdf";
      gmao_vars["albvisdf"] = new MVarDesc( "albvisdf", 3, "surface_albedo_for_visible_diffuse", "surface_albedo_for_visible_diffuse", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      cf_names["bcinc"] = "Black Carbon Mixing Ratio Analysis Increments";
      gmao_names["Black Carbon Mixing Ratio Analysis Increments"] = "bcinc";
      gmao_vars["bcinc"] = new MVarDesc( "bcinc", 4, "Black Carbon Mixing Ratio Analysis Increments", "black carbon mixing ratio analysis increments", "kg/kg", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_gas_Nv,inst3_3d_gas_Nv.%s,0,1,3,0,0" );

      cf_names["dms"] = "Dimethylsulphide";
      gmao_names["Dimethylsulphide"] = "dms";
      gmao_vars["dms"] = new MVarDesc( "dms", 4, "Dimethylsulphide", "dimethylsulphide", "kg kg-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0" );

      gmao_vars["mfxc"] = new MVarDesc( "mfxc", 4, "", "pressure_weighted_accumulated_eastward_mass_flux", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Nv,tavg3_3d_lsf_Nv.%s,0,1,3,3,90" );

      cf_names["emis"] = "surface_emissivity";
      gmao_names["surface_emissivity"] = "emis";
      gmao_vars["emis"] = new MVarDesc( "emis", 3, "surface_emissivity", "surface_emissivity", "1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30" );

      gmao_vars["nipnh3aq"] = new MVarDesc( "nipnh3aq", 3, "", "ammonia change from aqueous chemistry __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["kh"] = "total_scalar_diffusivity";
      gmao_names["total_scalar_diffusivity"] = "kh";
      gmao_vars["kh"] = new MVarDesc( "kh", 4, "total_scalar_diffusivity", "total_scalar_diffusivity", "m+2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Cp,tavg3_3d_trb_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["t2mmax"] = "2-meter_air_temperature";
      gmao_names["2-meter_air_temperature"] = "t2mmax";
      gmao_vars["t2mmax"] = new MVarDesc( "t2mmax", 3, "2-meter_air_temperature", "2-meter_air_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0" );

      cf_names["lwgntwtr"] = "open_water_net_downward_longwave_flux";
      gmao_names["open_water_net_downward_longwave_flux"] = "lwgntwtr";
      gmao_vars["lwgntwtr"] = new MVarDesc( "lwgntwtr", 3, "open_water_net_downward_longwave_flux", "open_water_net_downward_longwave_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

      gmao_vars["nh4smass"] = new MVarDesc( "nh4smass", 3, "", "ammonium surface mass concentration __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["t10m"] = "10-meter_air_temperature";
      gmao_names["10-meter_air_temperature"] = "t10m";
      gmao_vars["t10m"] = new MVarDesc( "t10m", 3, "10-meter_air_temperature", "10-meter_air_temperature", "K", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_smp_Nx,inst3_2d_smp_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["nipno3aq"] = new MVarDesc( "nipno3aq", 3, "", "nitrate production from aqueous chemistry __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dtdtgwd"] = "air_temperature_tendency_due_to_GWD";
      gmao_names["air_temperature_tendency_due_to_GWD"] = "dtdtgwd";
      gmao_vars["dtdtgwd"] = new MVarDesc( "dtdtgwd", 4, "air_temperature_tendency_due_to_GWD", "air_temperature_tendency_due_to_gwd", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      gmao_vars["coscnbas"] = new MVarDesc( "coscnbas", 3, "", "co surface concentration in ppbv (asia non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["spland"] = "rate_of_spurious_land_energy_source";
      gmao_names["rate_of_spurious_land_energy_source"] = "spland";
      gmao_vars["spland"] = new MVarDesc( "spland", 3, "rate_of_spurious_land_energy_source", "rate_of_spurious_land_energy_source", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["coscbbgl"] = new MVarDesc( "coscbbgl", 3, "", "co surface concentration in ppbv (global biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["v500"] = "northward_wind_at_500_hPa";
      gmao_names["northward_wind_at_500_hPa"] = "v500";
      gmao_vars["v500"] = new MVarDesc( "v500", 3, "northward_wind_at_500_hPa", "northward_wind_at_500_hpa", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      gmao_vars["nisv002"] = new MVarDesc( "nisv002", 3, "", "nitrate convective scavenging bin 002 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bcem002"] = "Black Carbon Emission Bin 002";
      gmao_names["Black Carbon Emission Bin 002"] = "bcem002";
      gmao_vars["bcem002"] = new MVarDesc( "bcem002", 3, "Black Carbon Emission Bin 002", "black carbon emission bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["bcem001"] = "Black Carbon Emission Bin 001";
      gmao_names["Black Carbon Emission Bin 001"] = "bcem001";
      gmao_vars["bcem001"] = new MVarDesc( "bcem001", 3, "Black Carbon Emission Bin 001", "black carbon emission bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["ghland"] = "Ground_heating_land";
      gmao_names["Ground_heating_land"] = "ghland";
      gmao_vars["ghland"] = new MVarDesc( "ghland", 3, "Ground_heating_land", "ground_heating_land", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      gmao_vars["colsnbas"] = new MVarDesc( "colsnbas", 3, "", "co chemical loss (asia non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      gmao_vars["copdbbla"] = new MVarDesc( "copdbbla", 3, "", "co chemical production (central and south america biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["supso4aq"] = "SO4 Prod from Aqueous SO2 Oxidation [column] __ENSEMBLE__";
      gmao_names["SO4 Prod from Aqueous SO2 Oxidation [column] __ENSEMBLE__"] = "supso4aq";
      gmao_vars["supso4aq"] = new MVarDesc( "supso4aq", 3, "SO4 Prod from Aqueous SO2 Oxidation [column] __ENSEMBLE__", "so4 prod from aqueous so2 oxidation [column] __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["dtdttot"] = "tendency_of_air_temperature_due_to_physics";
      gmao_names["tendency_of_air_temperature_due_to_physics"] = "dtdttot";
      gmao_vars["dtdttot"] = new MVarDesc( "dtdttot", 4, "tendency_of_air_temperature_due_to_physics", "tendency_of_air_temperature_due_to_physics", "K s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Cp,tavg3_3d_tdt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90" );

      cf_names["rzmc"] = "water_root_zone";
      gmao_names["water_root_zone"] = "rzmc";
      gmao_vars["rzmc"] = new MVarDesc( "rzmc", 3, "water_root_zone", "water_root_zone", "m-3 m-3", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["dvdttrb"] = "tendency_of_northward_wind_due_to_turbulence";
      gmao_names["tendency_of_northward_wind_due_to_turbulence"] = "dvdttrb";
      gmao_vars["dvdttrb"] = new MVarDesc( "dvdttrb", 4, "tendency_of_northward_wind_due_to_turbulence", "tendency_of_northward_wind_due_to_turbulence", "m s-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Cp,tavg3_3d_udt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["copdbbgl"] = new MVarDesc( "copdbbgl", 3, "", "co chemical production (global biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["ocdp001"] = "Organic Carbon Dry Deposition Bin 001 __ENSEMBLE__";
      gmao_names["Organic Carbon Dry Deposition Bin 001 __ENSEMBLE__"] = "ocdp001";
      gmao_vars["ocdp001"] = new MVarDesc( "ocdp001", 3, "Organic Carbon Dry Deposition Bin 001 __ENSEMBLE__", "organic carbon dry deposition bin 001 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["evap"] = "evaporation_from_turbulence";
      gmao_names["evaporation_from_turbulence"] = "evap";
      gmao_vars["evap"] = new MVarDesc( "evap", 3, "evaporation_from_turbulence", "evaporation_from_turbulence", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      cf_names["ocdp002"] = "Organic Carbon Dry Deposition Bin 002 __ENSEMBLE__";
      gmao_names["Organic Carbon Dry Deposition Bin 002 __ENSEMBLE__"] = "ocdp002";
      gmao_vars["ocdp002"] = new MVarDesc( "ocdp002", 3, "Organic Carbon Dry Deposition Bin 002 __ENSEMBLE__", "organic carbon dry deposition bin 002 __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["eflux"] = "total_latent_energy_flux";
      gmao_names["total_latent_energy_flux"] = "eflux";
      gmao_vars["eflux"] = new MVarDesc( "eflux", 3, "total_latent_energy_flux", "total_latent_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30" );

      gmao_vars["nidp003"] = new MVarDesc( "nidp003", 3, "", "nitrate dry deposition bin 003 __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      cf_names["omega"] = "vertical_pressure_velocity";
      gmao_names["vertical_pressure_velocity"] = "omega";
      gmao_vars["omega"] = new MVarDesc( "omega", 4, "vertical_pressure_velocity", "vertical_pressure_velocity", "Pa s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      cf_names["bceman"] = "Black Carbon Anthropogenic Emissions";
      gmao_names["Black Carbon Anthropogenic Emissions"] = "bceman";
      gmao_vars["bceman"] = new MVarDesc( "bceman", 3, "Black Carbon Anthropogenic Emissions", "black carbon anthropogenic emissions __ensemble__", "kg m-2 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90" );

      gmao_vars["coclnbeu"] = new MVarDesc( "coclnbeu", 3, "", "co column burden (european non-biomass burning)", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_tag_Nx,tavg3_2d_tag_Nx.%s,0,0,3,3,90" );

      cf_names["evpintr"] = "interception_loss_energy_flux";
      gmao_names["interception_loss_energy_flux"] = "evpintr";
      gmao_vars["evpintr"] = new MVarDesc( "evpintr", 3, "interception_loss_energy_flux", "interception_loss_energy_flux", "W m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30" );

      cf_names["doxdtdyn"] = "tendency_of_ozone_due_to_dynamics";
      gmao_names["tendency_of_ozone_due_to_dynamics"] = "doxdtdyn";
      gmao_vars["doxdtdyn"] = new MVarDesc( "doxdtdyn", 4, "tendency_of_ozone_due_to_dynamics", "tendency_of_ozone_due_to_dynamics", "mol mol-1 s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Cp,tavg3_3d_odt_Cp.%s,1,2,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90" );

      gmao_vars["taitime"] = new MVarDesc( "taitime", 3, "", "taitime", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_aer_Nv,inst3_3d_aer_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_chm_Nv,inst3_3d_chm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_cld_Nv,tavg3_3d_cld_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Nv,tavg3_3d_lsf_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Nv,tavg3_3d_mst_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_nav_Nv,tavg3_3d_nav_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_odt_Nv,tavg3_3d_odt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_qdt_Nv,tavg3_3d_qdt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_rad_Nv,tavg3_3d_rad_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_tdt_Nv,tavg3_3d_tdt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_lfo_Nx,inst1_2d_lfo_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_smp_Nx,inst1_2d_smp_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_smp_Nx,inst3_2d_smp_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_flx_Nx,tavg1_2d_flx_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lfo_Nx,tavg1_2d_lfo_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_lnd_Nx,tavg1_2d_lnd_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_rad_Nx,tavg1_2d_rad_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_adg_Nx,tavg3_2d_adg_Nx.%s,0,0,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_chm_Nx,tavg3_2d_chm_Nx.%s,0,0,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_smp_Nx,tavg3_2d_smp_Nx.%s,0,0,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_lsf_Ne,tavg3_3d_lsf_Ne.%s,0,3,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_mst_Ne,tavg3_3d_mst_Ne.%s,0,3,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_nav_Ne,tavg3_3d_nav_Ne.%s,0,3,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_trb_Ne,tavg3_3d_trb_Ne.%s,0,3,3,3,90" );

      cf_names["u"] = "eastward_wind";
      gmao_names["eastward_wind"] = "u";
      gmao_vars["u"] = new MVarDesc( "u", 4, "eastward_wind", "eastward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Np,inst3_3d_asm_Np.%s,0,2,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_3d_asm_Nv,inst3_3d_asm_Nv.%s,0,1,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_asm_Nv,tavg3_3d_asm_Nv.%s,0,1,3,3,90|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_3d_udt_Nv,tavg3_3d_udt_Nv.%s,0,1,3,3,90" );

      gmao_vars["nismass"] = new MVarDesc( "nismass", 3, "", "nitrate surface mass concentration __ensemble__", "", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst1_2d_hwl_Nx,inst1_2d_hwl_Nx.%s,0,0,1,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg3_2d_aer_Nx,tavg3_2d_aer_Nx.%s,0,0,3,3,90" );

      cf_names["v50m"] = "northward_wind_at_50_meters";
      gmao_names["northward_wind_at_50_meters"] = "v50m";
      gmao_vars["v50m"] = new MVarDesc( "v50m", 3, "northward_wind_at_50_meters", "50-meter_northward_wind", "m s-1", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,inst3_2d_asm_Nx,inst3_2d_asm_Nx.%s,0,0,3,0,0|https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_slv_Nx,tavg1_2d_slv_Nx.%s,0,0,1,1,30" );

      cf_names["tauywtr"] = "northward_stress_over_water";
      gmao_names["northward_stress_over_water"] = "tauywtr";
      gmao_vars["tauywtr"] = new MVarDesc( "tauywtr", 3, "northward_stress_over_water", "northward_stress_over_water", "N m-2", "https://opendap.nccs.nasa.gov/dods/GEOS-5/fp/0.25_deg/assim,tavg1_2d_ocn_Nx,tavg1_2d_ocn_Nx.%s,0,0,1,1,30" );

