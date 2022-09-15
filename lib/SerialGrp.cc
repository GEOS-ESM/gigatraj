
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include "gigatraj/SerialGrp.hh"

using namespace gigatraj;

SerialGrp::SerialGrp() : ProcessGrp()
{
   /* by definition, there is only one processor, and
      it performs all roles 
   */
   num_procs = 1;
   my_id = 0;
   role = PGrpRole_All;

};


SerialGrp::SerialGrp(const SerialGrp& src) : ProcessGrp(src)
{
   
}


SerialGrp::~SerialGrp()
{

};

int SerialGrp::is_root() const
{
   // the single serial processor is always the root processor
   return 1;
};

int SerialGrp::id() const
{
   // the single serial processor always has an ID of 0
   return 0;
};

std::string SerialGrp::group_id() const
{
   // the single serial processorgroup  always has an ID of 0
   return "single";
};

int SerialGrp::root_id() const
{
   // the single serial processor's root processor is always itself,
   // with an ID of 0
   return 0;
};

int SerialGrp::belongs() const
{
   // the single serial processor always belongs to whatever
   // processor group is under connsideration
   return 1;
};

void SerialGrp::setType(int id, ProcessRole type)
{
   if ( id == 0 || id == -1 ) {
      // the single serial processor has its role
      role = type;
   } else {
      // and no other ID is valid
      throw (badprocessor());
   }
};

SerialGrp* SerialGrp::copy() const
{
   SerialGrp* newgrp;
   
   newgrp = new SerialGrp();
   newgrp->setType( 0, role );

   if ( mytag != "" ) {
      newgrp->tag( mytag + "-copy" );
   }
   
   return newgrp;
};

void SerialGrp::sync() const
{
    // synchronization is meaningless for the single serial processor
}

void SerialGrp::sync(const std::string& msg) const
{
    // synchronization is meaningless for the single serial processor
    std::cerr << "SerialGrp sync: " << msg << std::endl;
}

void SerialGrp::sync(int id) const
{
    // synchronization is meaningless for the single serial processor
}

void SerialGrp::desync(int interval)
{
    // de-synchronization is meaningless for the single serial processor
}

void SerialGrp::shutdown() 
{
}

SerialGrp* SerialGrp::subgroup(int size, int flags, int offset)
{
   
   // Only the case of the subgroup being the same as the (single-serial-processor) group
   // will succeed.
   
   SerialGrp* new_pgroup;
   int newsize;
   
   // figure out the size (do sanity-checking)
   newsize = size;
   if ( newsize < 0 || offset != 0 ) {
      throw(ProcessGrp::badgroupsize());
   }
   if ( newsize == 0 ) {
      // default to the same number of processors as the current group
      newsize = num_procs;
   }   
   if ( newsize > num_procs ) {
      if ( flags && PG_STRICT ) {
         throw(ProcessGrp::badgroupsize());
      } else {
         newsize = num_procs;
      }
   }
   
   new_pgroup =  new SerialGrp;
   
   return new_pgroup;
};

SerialGrp* SerialGrp::subgroup( int size, int *list, int flags )
{
   SerialGrp* new_pgroup;

   if ( size != 1 ) {
      throw(ProcessGrp::badgroupsize());
   }
      
   if ( list[0] != 0 ) {
      throw(ProcessGrp::badprocessor());
   }

   new_pgroup =  new SerialGrp;
   
   return new_pgroup;
}         

void SerialGrp::split( int size, ProcessGrp **a, ProcessGrp **b, int flags ) const
{
   // only the case of the first group being the full (single-processor) group,
   // and the second being null, will succeed
   
   int size_a, size_b;
   
   // figure out the size (do sanity-checking)
   size_a = size;
   if ( size_a <= 0 ) {
      throw(ProcessGrp::badgroupsize());
   }
   if ( size_a > 1 ) {
      if ( flags && PG_STRICT ) {
         throw(ProcessGrp::badgroupsize());
      } else {
         size_a = 1;
      }
   }
   
   *a = new SerialGrp();
   
   // how many processors are left over for the second group?
   size_b = num_procs - size_a;
   if ( flags && PG_KEEPROOT ) {
      // we need to keep the root processor here, too
      size_b++;
   }

   if ( size_b > 0 ) {
      *b = new SerialGrp();
   } else {
      *b = NULLPTR;
   }


};

int SerialGrp::numberOfProcessors() const
{
    return 1;
} 

/* In the following send/receive functions, we merely check that we
   are sending to ourselves, as the single serial processor.
   And in that case, we need do no actual sending or receiving.
*/

void SerialGrp::send_reals( int id, int n, const real *vals, int tag) const
{
   if ( id != 0 ) {
      throw (badprocessor());
   }
   
};

void SerialGrp::send_doubles( int id, int n, const double *vals, int tag) const
{
   if ( id != 0 ) {
      throw (badprocessor());
   }
};

void SerialGrp::send_ints( int id, int n, const int *vals, int tag) const
{
   if ( id != 0 ) {
      throw (badprocessor());
   }
};

void SerialGrp::send_string( int id, const std::string &str, int tag) const
{
   if ( id != 0 ) {
      throw (badprocessor());
   }
};


void SerialGrp::receive_reals( int id, int n, real *vals, int tag, int *src) const
{
   if ( id > 1 ) {
      throw (badprocessor());
   }
   
   if ( src != NULLPTR ) {
      // source is always processor 0
      *src = 0;
   }   
};

void SerialGrp::receive_doubles( int id, int n, double *vals, int tag, int *src) const
{
   if ( id > 1 ) {
      throw (badprocessor());
   }
   
   if ( src != NULLPTR ) {
      // source is always processor 0
      *src = 0;
   }   
};

void SerialGrp::receive_ints( int id, int n, int *vals, int tag, int *src) const
{
   if ( id > 1 ) {
      throw (badprocessor());
   }
   
   if ( src != NULLPTR ) {
      // source is always processor 0
      *src = 0;
   }   
};

void SerialGrp::receive_string( int id, std::string *str, int tag, int *src) const
{
   if ( id > 1 ) {
      throw (badprocessor());
   }
   
   if ( src != NULLPTR ) {
      // source is always processor 0
      *src = 0;
   }   

};


real SerialGrp::random() const
{
    real result;
    
    result = rnd->uniform(0.0, 1.0 ); 

    return result;
}
