#ifndef GIGATRAJ_MPIGRP_H
#define GIGATRAJ_MPIGRP_H

#include "mpi.h"

#include "gigatraj/gigatraj.hh"
#include "gigatraj/ProcessGrp.hh"

#include <vector>

namespace gigatraj {

// note: this class is defined only for MPI

#ifdef USE_MPI

/*!

\ingroup pgroup
\brief implements the ProcessGrp parallel processing interface using MPI

The MPIGrp class permits gigatraj programmers to do
with parallel processing using MPI (Message Passing Interface), by using a standardized
interface supplied by the ProcessGrp virtual class.
With the MPIGrp class, the processor groups are based on the 
MPI communicator process groups. The group's root processor
is the MPI rank 0 processor.

Note that this MPIGrp class is based on the C-language binding to the OpenMPI library.
The MPI C++ binding is not used, because little documentation was found for it.

*/

class MPIGrp : public ProcessGrp {

   public:
      
      /// the default constructor 
      /*! This is the default constructor for MPIGrp.  It 
          uses the MPI_COMM_WORLD communicator.
      */    
      MPIGrp();
      
      /// a constructor that uses a given MPI communicator
      /*! This is a version of the constructor that makes an MPIGrp object 
          from a given MPI ccommunicator
      
          \param mpicomm the MPI communicator
          \param free bitwise flags that indicate how the object is to be destroyed:
              - 0: do NOT free the communicator 
              - 1: Free the communicator
               
      */    
      MPIGrp( MPI_Comm mpicomm, int free = 0 );

      /// a constructor that initializes MPI.
      /*! This is a version of the constructor that initializes MPI.
          This constructor should be called only once, when
          creating the very first process group used in the program,
          and it needs the main program routine's arguments.
          
          \param argc same as the argc parameter in the main() function of any program
          \param argv same as the argv parameter in the main() function of any program
      */
      MPIGrp(int argc, char* argv[]);

      /// a constructor that uses a given MPI ccommunicator and group
      /*! This version of the constructor makes an MPIGrp object from a 
          given MPI ccommunicator and group
      
          \param mpicomm the MPI communicator
          \param mpigroup the MPI group.  If this processor is a member of this group, then
                         the group must belong to the communicator.
          \param free bitwise flags that indicate how the object is to be destroyed:
              - 0: do NOT free either the communicator or the group
              - 1: Free only the communicator
              - 2: free only the group
              - 3: free both the communicator and the group
               
      */    
      MPIGrp( MPI_Comm mpicomm, MPI_Group mpigroup, int free = 0);
      
      /// Copy-constructor
      /*! This is the copy-constructor for the MPIGrp class
      */ 
      MPIGrp(const MPIGrp&);
      
      /// The destructor
      /*! This is the destructor for the MPIGrp class
      */ 
      ~MPIGrp();

      /// determines whether the current processor is root of its group
      /*! This function answers the question, is this processor the
          coordinating processors, or root, of its group.
          
          \return awlays returns 1, since this ia a serial processing environment.
      */    
      int is_root() const;

      /// returns an ID for this processor
      /*! This function returns an integer that identifies this processor.
      
          \return the processor ID
      */    
      int id() const;

      /// returns the ID of the current group
      /*! This function returns an ID string for the current group.
          Note that this is merely an identifying tag (and may in fact
          be partially derived from the object's tag).
          
          \return the group ID
      */    
      std::string group_id() const;      
     
     
      /// sets the ID of the current group
      /*! This function sets an ID number for the current group.
          Note that this is merely an identifying tag number
          and does not reflect any connection to MPI
          
          \param id the group ID
      */    
      void group_id( int id ) const;      
     
      /// returns the ID of this group's root  processor
      /*! This function returns an integer that identifies the coordinating, or root, processor
          within the current processor's group.  
          
          \return the ID of the root processor
      */
      int root_id() const;

      /// Determines group membership
      /*! This function finds out whether it is a member of a processor group.
        
          \return 1 if this processor is a memeber of a process group, 0 otherwise
      */
      int belongs() const;
      
      /// Sets the type of a processor
      /*! Sets the type or role that a processor is to play within its group.
      
           \param id the integer identifier of the particular processor within
                 this group whose type is to be set. If -1, then every progrssor in 
                 the group is set to this type.
           \param type the role to be assigned to that processor.  This must always be All.
      */     
      void setType(int id, ProcessRole type);
      
      /// copies a process group object
      /*! This function copies a process group.
      
          \return a pointer to the new copy
      */
      MPIGrp* copy() const;

      /// synchronizes with the rest of the group
      /*! This function causes the current processor to pause until
          the other processors in its group are caught up with it.
      */
      void sync() const;

      /// synchronizes with a particular processor
      /*! This function causes the current processor to pause until
          another processor in its group is caught up with it.
          
          \param id the ID of the individual processor with which this
                    one is to synchronize.  
      */    
      void sync(int id) const;

      /// synchronizes with the rest of the group
      /*! This function causes the current processor to pause until
          the other processors in its group are caught up with it.
          
          \param msg a string message to be printed, to aid in debugging
      */
      void sync(const std::string& msg) const;


      /// synchronizes with a particular processor
      /*! This function causes the current processor to pause until
          another processor in its group is caught up with it.
          
          \param id the ID of the individual processor with which this
                    one is to synchronize.  
          \param msg a string message to be printed, to aid in debugging
      */    
      void sync(int id, const std::string& msg) const;

      /// de-synchronizes with other processors, by introducing a random wait
      /*! This function causes the current processor to pause 
          a random amount of time, to reduce synchronization 
          with other processors. This routine has no effect if there
          is no multiprocessing; it does not wait at all.
          
          \param interval an interval, in seconds, that is the maximum 
                  time for this processor to wait before proceeding. 
                  The actual wait time is a uniformaly distributed random
                  number between zero and this time. 
                  
      */    
      void desync(int interval);

      /// shuts down processing
      /*! This function whatever serial or parallel processing system is in use,
          performing any cleanup activities that are necessary.
      */    
      void shutdown();

      /// creates a new group from the current group
      /*! This function creates a new group from the current group.  the processors
          for the new subgroup are selected by specifying the processor ID
          of the first processor, and the number of processors.  
      
          \return a pointer to the new group object.
          \param size the desired number of processors in the new group.  This must be 1.
          \param flags  process group flags (PG_* values) or'd together to specify how the group is to be created.
          \param offset the first processor of the current group that is to be part of the new group.
                        This must be 0.
                        
      */      
      MPIGrp* subgroup(int size=0, int flags=0, int offset=0);

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
      MPIGrp* subgroup( int size, int *list, int flags=0 );          
   
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



      /// returns the rank of the current process within this group
      /*! This function returns the rank of the current processor within 
          its MPI communicator group.
          
          \return the rank of the processor
      */    
      int rank() const;      

      /// returns a the MPI communicator for this group
      /*! This function returns the MPI communicator for the 
          gigatraj process group
          of which this processor is a member.
      
          \return the MPI_Comm communicator.
      */
      MPI_Comm MPIcomm() const;

      /// returns a the MPI group for the MPI communicator for this group
      /*! This function returns the MPI group for the 
          gigatraj process group of which this processor 
          is a member.
      
          \return the MPI_Group to which this processor belongs
      */
      MPI_Group MPIgroup() const;
      
      /// returns a set of flags
      /*!  This function returns a set of flags used internally by this object.
      
          \return the flags
          - 0 = both MPI communicator and MPI group may be freed by the destructor
          - 1 = MPI communicator was supplied externally and must \b not be freed by the destructor
          - 2 = MPI group was supplied externally and must \b not be freed by the destructor
          - 3 = both MPI communicator and MPI group were supplied externally and must \b not be freed by the destructor
      */
      int getflags() const {
          return flags;
      }    


   private:
   
     // The MPI Communicator for this process group
     MPI_Comm comm;
     
     // The MPI Group for this process group
     MPI_Group group;

     /* flags:
          - 1 = comm was supplied externally and must not be freed by destructor
          - 2 = group was supplied externally and must not be freed by destructor
     */
     int flags;     

     // the group this processor belongs to
     // (this is an arbitary tag number)
     int mygroup;
          
};

#endif

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
