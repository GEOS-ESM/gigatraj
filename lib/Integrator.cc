
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/Integrator.hh"

using namespace gigatraj;

void Integrator :: conformal( int mode )
{
    confml = mode;
} 

int Integrator :: conformal()
{
    return confml;
}

