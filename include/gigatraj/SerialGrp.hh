#ifndef GIGATRAJ_SERIALGRP_H
#define GIGATRAJ_SERIALGRP_H

#include "gigatraj/gigatraj.hh"
#include "gigatraj/ProcessGrp.hh"


namespace gigatraj {


/*!

\ingroup pgroup
\brief implements the ProcessGrp parallel processing interface on a serial processor

The SerialGrp class uses the ProcessGrp
interface for parallel processing, but without doing any actual
parallel processing.  Thus, this class
permits gigatraj programmers to 
switch between serial and parallel processing by substituting
an object of this class for one of a parallel-processing class
such as MPIGrp.   

*/

class SerialGrp : public ProcessGrp {

   public:
      
      /// the constructor
      /*! This is the constructor for the SerialGrp class
      */ 
      SerialGrp();
      
      
      /// Copy-constructor
      /*! This is the copy-constructor for the SerialGrp class
      */ 
      SerialGrp(const SerialGrp&);
      
      /// The destructor
      /*! This is the destructor for the SerialGrp class
      */ 
      ~SerialGrp();

      /// determines whether the current processor is root of its group
      /*! This function answers the question, is this processor the
          coordinating processors, or root, of its group.
          
          \return always returns 1, since this ia a serial processing environment.
      */    
      int is_root() const;

      /// returns an ID for this processor
      /*! This function returns an integer that identifies this processor.
      
          \return always returns 0, since there is only one processor
      */    
      int id() const;

      /// returns the ID of the current group
      /*! This function returns an ID string for the current group.
          Note that this is merely an identifying tag (and may in fact
          be partially derived from the object's tag).
          
          \return the group ID
      */    
      std::string group_id() const;      
     
      /// returns the ID of this group's root  processor
      /*! This function returns an integer that identifies the coordinating, or root, processor
          within the current processor's group.  
          
          \return always returns 0, since there is only one processor
      */
      int root_id() const;


      /// Determines group membership
      /*! This function finds out whether it is a member of a processor group.
        
          \return always returns 1
      */
      int belongs() const;

      /// Sets the type of a processor
      /*! Sets the type or role that a processor is to play within its group.
      
           \param id the integer identifier of the particular processor within
                 this group whose type is to be set.  This must always be 0 or -1.
                 If -1, then every progrssor in 
                 the group is set to this type.
           \param type the role to be assigned to that processor.  This must always be All.
      */     
      void setType(int id, ProcessRole type);
      
      /// copies a process group object
      /*! This function copies a process group.
      
          \return a pointer to the new copy
      */
      SerialGrp* copy() const;

      /// synchronizes with the rest of the group
      /*! This function would synchronize with all the members of its group.
          But since there is only one processor, this function does nothing.
      */
      void sync() const;

      /// synchronizes with the rest of the group
      /*! This function would synchronize with all the members of its group.
          But since there is only one processor, this function does nothing.

          \param msg a string message to be printed, to aid in debugging
      */
      void sync(const std::string& msg) const;

      /// de-synchronizes with other processors, by introducing a random wait
      /*! This function causes the current processor to pause 
          a random amount of time, to reduce synchronization 
          with other processors. This routine has no effect if there
          is no multiprocessing; it does not wait at all.
          
          \param interval an interval, in seconds, that is the maximum 
                  time for this processor to wait before proceeding. 
                  The actual wait time is always 0.0 for this serial
                  processing group.
      */    
      void desync(int interval);

      /// synchronizes with a particular processor
      /*! This function would cause the current processor to pause until
          another processor is caught up with it.  But since there is
          only one processor, it does nothing.
          
          \param id the ID of the individual processor with which this
                    one is to synchronize.  
      */    
      void sync(int id) const;


      /// shuts down processing
      /*! This function whatever serial or parallel processing system is in use,
          performing any cleanup activities that are necessary.
      */    
      void shutdown();


      /// creates a new group from the current group
      /*! This function creates a new group from the current group.  the processors
          for the new subgroup are selected by specifying the processor ID
          of the first processor, and the number of processors.  
          Note that the ProcessGrp object keeps track of 
          its parent and its children, so 
          be careful when deleting such objects. (See the orphan and the disinherit
          methods.)
      
          \return a pointer to the new group object.
          \param size the desired number of processors in the new group.  This must be 1.
          \param flags  process group flags (PG_* values) or'd together to specify how the group is to be created.
          \param offset the first processor of the current group that is to be part of the new group.
                        This must be 0.
      */      
      SerialGrp* subgroup(int size=0, int flags=0, int offset=0 );

      /// creates a subgroup from the current group
      /*! This function creates a subgroup from the current group, using a list 
          of desired members of the new group
          
          \return a pointer to the new group object.
          \param size the desired number of processors in the first new group
          \param list an array of processor IDs to be used to create the new group. 
                 The first ID will be the new group's root procesor.
                 For SerialGrp, this must be a single-element vector, with the single
                 element being 0.
          \param flags process group flags (PG_* values) or'd together to specify how the group is to be created.
      */
      SerialGrp *subgroup( int size, int *list, int flags=0 );         
   
      /// creates two new groups from the current group
      /*! This function creates two new groups from the current group.
          That processor's ProcessGrp object will receive the reals using its
          \b receive_reals() method.
      
          \param size the desired number of processors in the first new group
          \param a the first new group, which is a subset of the current group.  This will be identical
                   to the current group.
          \param b the second new group, which holds all members of the current group which are not in group "a".
                   This will be a group with no members, null.
          \param flags: process group flags (PG_* values) or'd together to specify how the group is to be created.
      */
      void split( int size, ProcessGrp **a, ProcessGrp **b, int flags=0 ) const;
   
      /// returns the total number of distinct processors running in parallel
      /*! This method returns the number of processors. In serial processing,
          this is 1. In parallel processing, it is the total number of processes.
          
          Note that this is not the number of prcessors in this process group,
          which is obtained with the size() method, but the total number of
          processors, regardless of membership in a process group.
          
          This method is intended to be useful for determining delay times
          and numers-of-retries, to avoid conflicts in cases where a number of different processors
          may attempt to access the same resource.
      */
      int numberOfProcessors() const;   

      /// generates a random number that is shared throughout the whole group
      /*! This methods produces a uniform (pseudo-)random number that, while
          differing for each invocations and differing between Process Groups,
          is the same within a process group.
          
          \return a random floating-point nunber between 0.0 and 1,0
      */
      real random() const;  
   
      /// sends a set of reals to another processor in this group.
      /*! This function sends a set of reals to another processor in this group

           \param id the ID (with respect to this group) of the other processor
           \param n the number of reals to send
           \param vals an array of reals to be sent
           \param tag an arbitrary number used to label the content in vals
      */
      void send_reals( int id, int n, const real *vals, int tag=0) const;

      /// sends a set of doubles to another processor in this group
      /*! This function sends a set of doubles to another processor in this group
          That processor's ProcessGrp object will receive the doubles using its
          \b receive_doubles() method.  For SerialGrp, this function does nothing.

           \param id the ID (with respect to this group) of the other processor
           \param n the number of doubles to send
           \param vals an array of doubles to be sent
           \param tag an arbitrary number used to label the content in vals
      */
      void send_doubles( int id, int n, const double *vals, int tag=0) const;

      /// sends a set of integers to another processor in this group
      /*! This function sends a set of integers to another processor in this group.
          That processor's ProcessGrp object will receive the integers using its
          \b receive_ints() method.  For SerialGrp, this function does nothing.

           \param id the ID (with respect to this group) of the other processor
           \param n the number of integers to send
           \param vals an array of integers to be sent
           \param tag an arbitrary number used to label the content in vals
      */
      void send_ints( int id, int n, const int *vals, int tag=0) const;

      /// sends a string to another processor in this group
      /*! This function sends a string to another processor in this group.
          That processor's ProcessGrp object will receive the string using its
          \b receive_string() method.  For SerialGrp, this function does nothing.
          
           \param id the ID (with respect to this group) of the other processor
           \param str the string to be sent
           \param tag an arbitrary number used to label the content in vals
      */
      void send_string( int id, const std::string &str, int tag=0) const;

   
      /// receives a set of reals from another processor in this group
      /*! This function receives a set of reals sent from another processor in this group
          using the \b send_reals() method.  For SerialGrp, this function does nothing.

           \param id the ID (with respect to this group) of the other processor.
                 If -1, then the values will be received from any processor in this group.
           \param n the number of reals to receive
           \param vals an array of reals to be sent
           \param tag an arbitrary number used to label the content in vals
           \param src returns the ID of the processor which sent the values
      */
      void receive_reals( int id, int n, real *vals, int tag=0, int *src=NULL) const;

      /// receives a set of doubles from another processor in this group
      /*! This function receives a set of doubles sent from another processor in this group
          using the \b send_doubles() method.  For SerialGrp, this function does nothing.

           \param id the ID (with respect to this group) of the other processor.
                 If -1, then the values will be received from any processor in this group.
           \param n the number of doubles to receive
           \param vals an array of doubles to be sent
           \param tag an arbitrary number used to label the content in vals
           \param src returns the ID of the processor which sent the values
      */
      void receive_doubles( int id, int n, double *vals, int tag=0, int *src=NULL) const;

      /// receives a set of integers from another processor in this group
      /*! This function receives a set of integers sent from another processor in this group
          using the \b send_ints() method.  For SerialGrp, this function does nothing.

           \param id the ID (with respect to this group) of the other processor.
                 If -1, then the values will be received from any processor in this group.
           \param n the number of integers to receive
           \param vals an array of integers to be sent
           \param tag an arbitrary number used to label the content in vals
           \param src returns the ID of the processor which sent the values
      */
      void receive_ints( int id, int n, int *vals, int tag=0, int *src=NULL) const;

      /// receives a string from another processor in this group.
      /*! This function receives a string sent from another processor in this group using 
          the \b send_string() method.  For SerialGrp, this function does nothing.

           \param id the ID (with respect to this group) of the other processor
                 If -1, then the values will be received from any processor in this group.
           \param vals the string to receive
           \param tag an arbitrary number used to label the content in vals
           \param src returns the ID of the processor which sent the values
      */
      void receive_string( int id, std::string *vals, int tag=0, int *src=NULL) const;

   private:
   
     
     // parent group that this group split off from
     SerialGrp *my_parent;
     
     // child subgroups created from this one
     std::vector<SerialGrp*> my_children;
     
     
     
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
