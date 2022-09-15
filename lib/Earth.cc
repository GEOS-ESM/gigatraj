
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/Earth.hh"


using namespace gigatraj;


const std::string Earth::id = "Earth";

/*! 
The constructor initializes earth-related constants:

The radius of the earth is set to the average value for 
a spherical earth: 6371.009 km.

See http://en.wikipedia.org/wiki/Earth_radius

*/
Earth :: Earth() : PlanetSphereNav()
{

   r = 6371.009; // km


}


/*!
The destructor does nothing.
*/
Earth :: ~Earth()
{

}


