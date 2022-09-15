
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/ProcessGrp.hh"

using namespace gigatraj;


ProcessGrp::ProcessGrp()
{
   
   mytag = "";

   // create a source for random numbers, in case we need to desynchronize
   // processors to avoid 
   // Note that it does not have to be a strong sorce of
   // randomness, so urandom should suffice.
   rnd = new RandomSrc("/dev/urandom");
   
};

// copy constructor
ProcessGrp::ProcessGrp(const ProcessGrp& src)
{
   
   // the number of processors
   num_procs = src.num_procs;

   // every processor gets its role duplicated
   role = src.role;

   mytag = src.mytag;

}

ProcessGrp::~ProcessGrp()
{
   delete rnd;
}


int ProcessGrp::size() const
{
    return num_procs;
};

ProcessGrp::ProcessRole ProcessGrp::type() const
{
   return role;
};

std::string ProcessGrp::tag() const
{
    return mytag;
};   

void ProcessGrp::tag( const std::string& tagstr )
{
    mytag = tagstr;
};   


