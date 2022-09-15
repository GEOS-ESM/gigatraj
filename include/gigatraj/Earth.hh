#ifndef GIGATRAJ_EARTH_H
#define GIGATRAJ_EARTH_H

#include "config.h"
#include "gigatraj/gigatraj.hh"
#include "gigatraj/PlanetSphereNav.hh"

#include <math.h>


namespace gigatraj {

/*!

\ingroup Navigation

\brief implements navigation on a spherical %Earth

The Earth class is a class for dealing with navigating on the earth:
longitudes, latitudes, distances, and so on.

This version of the class represents a perfectly spherical earth
(i.e, not an ellipsoidal earth).


*/   

class Earth : public PlanetSphereNav {


   public:

      /// The base constructor.
      Earth();
   
      /// The destructor
      ~Earth(); 
  
      /// the type of object this is
      static const std::string id;
 
};
}

#endif



/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/
