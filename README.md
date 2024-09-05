# GigaTraj


Gigatraj is a software library, plus some sample programs, for
calculating the trajectories of air parcels in the lower stratosphere.
Its purpose is purely for scientific research.
It allows for several different sources of meteorological data,
and it can calculate either kinematic (in pressure coordinates using omega)
or quasi-isentropic (in potential temperature coordinates using heating rates)
trajectories.

Please note that gigatraj is *not* a general Lagrangian transport model.
It does not attempt to simulate the effects of turbulence or 
small-scale diffusion. This makes it unsuitable for use in regions
where those effects are important--for example, at low altitudes.

Only source code is available here; we do not offer pre-compiled binaries.
Gigatraj is written in C++, and it is known to compile with
the GNU g++ compiler on several different Linux distributions.

To run gigatraj usefully, you will also need several external libraries, which
are not included here. You should have the netcdf v4 libraries installed, with
support  for accessing OPeNDAP servers. And if you want to use parallel
processing, then you should have the openMPI library installed as well. Although
gigatraj is written in C++, it uses only the C language bindings of those two
libraries.

Gigatraj is written by Leslie Robert Lait, NASA ARC-SG, working in 
the Atmospheric Chemistry and Dynamics Laboratory of NASA's Goddard
Space Flight Center.

For questions, contact Leslie.R.Lait@nasa.gov.

Copyright (c) 2023 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.  All Rights Reserved.
 
Disclaimer:
No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS."
Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT.
 
