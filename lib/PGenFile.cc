
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/

#include "config.h"

#include <sstream>

#include "gigatraj/PGenFile.hh"

using namespace gigatraj;

PGenFile :: PGenFile( string fmt )
{
    string fs;
    
    interpretor = new StreamRead(instring);

    if ( fmt == "" ) {
       fs = "%o %a %v";
    } else {
       fs = fmt;
    }
    format( fs );    

}

     
PGenFile :: ~PGenFile()
{
    delete interpretor;
}



int PGenFile :: readparcel( std::istream* input, Parcel *p )
{
   // init to comment status
   int status = 1;
   real lon, lat, z;
   
   
   try {
      // read the line from the input   
      std::string line;
      std::getline(*input, line);

      //std::cerr << "readline string <<" << line << ">>" << std::endl;

      if ( ! input->eof() ) {
 
         // note: we do a little preprocessing first, to discard
         // comments and blank lines.
         // (This is why we cannot simply let the StreamRead object
         // process the input tream directly.)

         // remove any comment text
         size_t pos = line.find("#");
         if ( pos != std::string::npos ) {
            line.erase(pos);
         }

         // remove any trailing whitespace
         int len = line.size();
   
         // does the line now consist of only whitespace or is it empty?
         if ( line.find_first_not_of( " \t\n\r" ) == std::string::npos ) {
            // empty line--we ignore it
            len = 0;
         }
   
         if ( len > 0 ) {

            // switch to input string stream
            instring.str( line );
            //instring.clear();
            //instring.seekg(0);
            //std::cerr << "    instring = <<" << instring.str() << ">>" << std::endl;
            instring.clear();
            instring.seekg(0);
            
            // init these outputs, too
            try {
            
                interpretor->apply(*p);
            
                // No error was thrown, so
                // we set the status to "OK"
                status = 0;

            } catch (...) {
               std::cerr << "Badly formatted line in input file" << std::endl;
               status = -2;
            }             
         }
      } else {
         // return EOF status
         std::cerr << "End of input file" << std::endl;
         status = -1;
      }   
      
   } catch (std::ios::failure) {
      // return error status
      std::cerr << "Error reading input file" << std::endl;
      status = -2;
   }   
   
   
   return status;
   
};

template< template<class U, class = std::allocator<U> > class Seq>
void PGenFile :: initbunch( Seq<Parcel>* seq, const Parcel& p, int *np, std::istream* input
              )
{

     real lon,lat,z;
     int status = 0;
     
     while ( status >= 0 ) {
        try {

           // read a parcel location from the file
           Parcel *it = new Parcel(p); // copy the input parcel's settings

           status = readparcel( input, it );
        
           if ( status == 0 ) {
              // add it to the container of parcels
              seq->push_back(*it);
           }

       } catch (...) {
          throw (ParcelGenerator :: badgeneration());
       }
     }
     
};


Parcel * PGenFile :: create_array(const Parcel& p, int *np
                     , const std::string &file
                   )                        
{
     Parcel* pa;
     Parcel* pb;
     real lon, lat, z;

     *np = -1;
     
     // open the file
     std::ifstream input(file.c_str());
     if ( ! input.good() ) {
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {

        // catch I/O errors
        input.exceptions(std::ios::badbit);
        
        // make a copy of the sample parcel
        pb = p.copy();
        
        // read through each line, interpreting it and counting the parcels
        int n = 0;
        int status = 0;
        while ( status >= 0 ) {
           status = PGenFile::readparcel(&input, pb);
           if ( status == 0 ) {
               //std::cerr << " 1st-pass parcel = " << (*pb) << std::endl;   
               n++;
           }    
        }
     
        // sanity check
        if ( n <= 0 ) {
            throw (ParcelGenerator :: badparcelcount());
        }
     
        // close the file
        input.close();
     
        // Now that we know how many parcels there need to be,
        // create the array
        try {
           pa = new Parcel[n];

           // re-open the file
           input.open(file);
           
           // read in the contents
           int status = 0;
           int i = 0;
           while ( status >= 0 ) {
              // copy the oldparcel into the new
              *pb = p;
              status = PGenFile::readparcel(&input, pb );
              if ( status == 0 ) {
                  pa[i] = *pb;  // copy the input parcel's settings
                  //std::cerr << " array parcel[" << i << "] = " << pa[i] << std::endl;   
                  
                  i++;  // next parcel
              }    
           }
           // close the file
           input.close();
           
           *np = n;
     
        } catch (...) {
           throw (ParcelGenerator :: badgeneration());
        }
        
        delete pb;
        
     } catch (...) {
           throw (ParcelGenerator :: badgeneration());
     }
     
     return pa;

};

Parcel * PGenFile :: create_array(const Parcel& p, int *np
                   , std::istream* file
                   )                        
{
     Parcel* pa;
     real lon, lat, z;
     std::vector<Parcel>* bunch;

     *np = -1;

     bunch = create_vector( p, file );
     *np = bunch->size();

     pa = NULLPTR;
     if ( *np > 0 ) {
        pa = new Parcel[*np];
     
        for ( int i=(*np-1); i>=0; i-- ) {
            pa[i] = (*bunch)[i];
            bunch->pop_back();
        }
     }
        
     return pa;

};

std::vector<Parcel>* PGenFile :: create_vector(const Parcel& p
                    , const std::string &file
                   )                        
{
     // the parcel container
     std::vector<Parcel> *bunch;
     // the number of parcels
     int np = 0;
     
     // create the parcel container
     bunch = new std::vector<Parcel>;
        
     // open the file
     std::ifstream input(file.c_str());
     if ( ! input.good() ) {
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {
     
        // catch I/O errors
        input.exceptions(std::ios::badbit);

        // read the file
        PGenFile::initbunch( bunch, p, &np, &input );
        
        // close the file
        input.close();

    } catch (...) {
        throw (ParcelGenerator :: badgeneration());
    }    

    return bunch;
};


std::vector<Parcel>* PGenFile :: create_vector(const Parcel& p
                   , std::istream* file
                   )                        
{
     // the parcel container
     std::vector<Parcel> *bunch;
     // the number of parcels
     int np = 0;

     // create the parcel container
     bunch = new std::vector<Parcel>;
        
     if ( ! file->good() ) {
        std::cerr << "input file is no good" << std::endl;
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {
     
        // catch I/O errors
        file->exceptions(std::ios::badbit);

        // read the file
        PGenFile::initbunch( bunch, p, &np, file );
        
    } catch (...) {
        std::cerr << "error reading input file" << std::endl;
        throw (ParcelGenerator :: badgeneration());
    }    

    return bunch;
};


std::list<Parcel>* PGenFile :: create_list(const Parcel& p
                    , const std::string &file
                   )                        
{
     // the parcel container
     std::list<Parcel> *bunch;
     // the number of parcels
     int np = 0;
     
     // create the parcel container
     bunch = new std::list<Parcel>;
        
     // open the file
     std::ifstream input(file.c_str());
     if ( ! input.good() ) {
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {
     
        // catch I/O errors
        input.exceptions(std::ios::badbit);

        // read the file
        PGenFile::initbunch( bunch, p, &np, &input );
        
        // close the file
        input.close();

    } catch (...) {
        throw (ParcelGenerator :: badgeneration());
    }    


    return bunch;
};


std::list<Parcel>* PGenFile :: create_list(const Parcel& p
                   , std::istream* file
                   )                        
{
     // the parcel container
     std::list<Parcel> *bunch;
     // the number of parcels
     int np = 0;
     
     // create the parcel container
     bunch = new std::list<Parcel>;
        
     // open the file
     if ( ! file->good() ) {
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {
     
        // catch I/O errors
        file->exceptions(std::ios::badbit);

        // read the file
        PGenFile::initbunch( bunch, p, &np, file );
        
    } catch (...) {
        throw (ParcelGenerator :: badgeneration());
    }    


    return bunch;
};



std::deque<Parcel>* PGenFile :: create_deque(const Parcel& p
                    , const std::string &file
                   )                        
{
     // the parcel container
     std::deque<Parcel> *bunch;
     // the number of parcels
     int np = 0;
     
     // create the parcel container
     bunch = new std::deque<Parcel>;
        
     // open the file
     std::ifstream input(file.c_str());
     if ( ! input.good() ) {
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {
     
        // catch I/O errors
        input.exceptions(std::ios::badbit);

        // read the file
        PGenFile::initbunch( bunch, p, &np, &input );
        
        // close the file
        input.close();

    } catch (...) {
        throw (ParcelGenerator :: badgeneration());
    }    


    return bunch;

};



std::deque<Parcel>* PGenFile :: create_deque(const Parcel& p
                   , std::istream* file
                   )                        
{
     // the parcel container
     std::deque<Parcel> *bunch;
     // the number of parcels
     int np = 0;
     
     // create the parcel container
     bunch = new std::deque<Parcel>;
        
     // open the file
     if ( ! file->good() ) {
        throw (ParcelGenerator :: badgeneration());
     }
     
     try {
     
        // catch I/O errors
        file->exceptions(std::ios::badbit);

        // read the file
        PGenFile::initbunch( bunch, p, &np, file );
        
    } catch (...) {
        throw (ParcelGenerator :: badgeneration());
    }    


    return bunch;

};


Flock* PGenFile :: create_Flock(const Parcel& p
                   , const std::string &file, ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Flock *flock;
     Flock::iterator ip;         
     real lon,lat,z;
     int status = 0;
     Parcel* pa;

     // note: since this is a file, every processor can
     // open it for reading. So they all do.
     
     // open the file
     std::ifstream input(file.c_str());
     if ( ! input.good() ) {
        std::cerr << "Cannot read input file" << std::endl;
        throw (ParcelGenerator :: badgeneration());
     }
     
     int n = 0;
     flock = NULLPTR;
     
     try {
     
        // catch I/O errors
        input.exceptions(std::ios::badbit);

        pa = p.copy();
        
        // read through each line, interpreting it and counting the parcels
        int status = 0;
        while ( status >= 0 ) {
           status = PGenFile::readparcel(&input, pa);
           if ( status == 0 ) {
               n++;
           }    
        }
     
        // sanity check
        if ( n <= 0 ) {
            std::cerr << "number of requeste dparcels is <= 0" << std::endl;
            throw (ParcelGenerator :: badparcelcount());
        }
     
        // close the file
        input.close();
     
        // now create a Flock of that many parels
        flock = new Flock( p, pgrp, n, r);

        // re-open the file
        input.open(file);
           

        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }
   
        for ( ip=flock->begin(); ip != flock->end(); ip++ ) {
            try {
               // read a parcel location from the file
               // (every processor reads the parcel)
               status = readparcel( &input, pa );

               // Only the processor that owns this parcel
               // actually stores it. 
               // But the syntax is the same either way.
               // This is the magic of Flocks.
               *ip = *pa;
            } catch (std::ios::failure) {
               std::cerr << "error reading parcel from input file" << std::endl;
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
        
        // close the file
        input.close();
        
        delete pa;

    } catch (...) {
        std::cerr << "error on reading parcels from input file" << std::endl;
        throw (ParcelGenerator :: badgeneration());
    }    

    if ( flock == NULLPTR ) {
       std::cerr << "no parcels read from input file" << std::endl;
       throw (ParcelGenerator :: badgeneration());
    }

    return flock;

};


Flock* PGenFile :: create_Flock(const Parcel& p
                   , std::istream* file, ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Flock *flock;
     Flock::iterator ip;         
     real lon,lat,z;
     int status = 0;
     std::vector<Parcel>* stuff;
     int np;
     Parcel *pcl;
     int i;
     bool i_am_root;

     np = -1;
     flock = NULLPTR;

     // one processor reads the file
     if ( pgrp == NULLPTR || pgrp->is_root()  ) {
        stuff = create_vector( p, file );
        np = stuff->size();
     } else {
        // this will be obtained from the Flock below
        np = 0;
        stuff = NULLPTR;
     }

     if ( pgrp != NULLPTR ) {
        pgrp->sync();
     }
     
     // now create a Flock of that many parcels
     // Note that only the root process has a valid value for np,
     // but that is all that is required here. 
     // The root processor will communicate the size of the
     // flock to the other processors when the Flock is constructed.
     flock = new Flock( p, pgrp, np, r);
     
     np = flock->size();
     
     
     if ( np > 0 ) {   
        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }

        // the input parcel may be some special subclass of Parcel,
        // so we make a copy of it rather than trying to
        // use our own
        pcl = p.copy();
     
        i_am_root = flock->is_root();
     
        for ( i=0; i<np; i++ ) {

            flock->sync();
            
            try {
            
               if ( i_am_root ) {
                  // get the parcel
                  *pcl = (*stuff)[i];
               }
               
               // If we are the root processor, the parcel pcl
               // is valid, and it is sent to the processor
               // to which it belongs.
               // If we are not the root processor, then
               // then pcl has no valid value, but the
               // processor will ignore it and receive its
               // parcel value from the root processor.
               flock->set( i, *pcl, 0 );

            } catch (std::ios::failure) {
               std::cerr << "Error reading parcel" << std::endl;
               throw (ParcelGenerator :: badgeneration());
            }   
        }
     
        delete pcl;   

     }
        
     if ( stuff != NULLPTR ) {
        delete stuff;
     }
     
     if ( flock == NULLPTR ) {
        std::cerr << "null Flock pointer" << std::endl;
        throw (ParcelGenerator :: badgeneration());
     }

     return flock;

};


Swarm* PGenFile :: create_Swarm(const Parcel& p
                   , const std::string &file, ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Swarm *swarm;
     Swarm::iterator ip;         
     real lon,lat,z;
     int status = 0;
     Parcel* pa;

     // note: since this is a file, every processor can
     // open it for reading. So they all do.
     
     // open the file
     std::ifstream input(file.c_str());
     if ( ! input.good() ) {
        std::cerr << "Cannot read input file" << std::endl;
        throw (ParcelGenerator :: badgeneration());
     }
     
     int n = 0;
     swarm = NULLPTR;
     
     try {
     
        // catch I/O errors
        input.exceptions(std::ios::badbit);

        pa = p.copy();
        
        // read through each line, interpreting it and counting the parcels
        int status = 0;
        while ( status >= 0 ) {
           status = PGenFile::readparcel(&input, pa);
           if ( status == 0 ) {
               n++;
           }    
        }
     
        // sanity check
        if ( n <= 0 ) {
            std::cerr << "number of requested parcels is <= 0" << std::endl;
            throw (ParcelGenerator :: badparcelcount());
        }
     
        // close the file
        input.close();
     
        // now create a Swarm of that many parels
        swarm = new Swarm( p, pgrp, n, r);

        // re-open the file
        input.open(file);
           

        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }
   
        for ( ip=swarm->begin(); ip != swarm->end(); ip++ ) {
            try {
               // read a parcel location from the file
               // (every processor reads the parcel)
               status = readparcel( &input, pa );

               // Only the processor that owns this parcel
               // actually stores it. 
               // But the syntax is the same either way.
               // This is the magic of Swarms.
               *ip = *pa;
            } catch (std::ios::failure) {
               std::cerr << "error reading parcel from input file" << std::endl;
               throw (ParcelGenerator :: badgeneration());
            }   
        }   
        
        // close the file
        input.close();
        
        delete pa;

    } catch (...) {
        std::cerr << "error on reading parcels from input file" << std::endl;
        throw (ParcelGenerator :: badgeneration());
    }    

    if ( swarm == NULLPTR ) {
       std::cerr << "no parcels read from input file" << std::endl;
       throw (ParcelGenerator :: badgeneration());
    }

    return swarm;

};


Swarm* PGenFile :: create_Swarm(const Parcel& p
                   , std::istream* file, ProcessGrp* pgrp, int r
                   )                        
{
     // the parcel container
     Swarm *swarm;
     Swarm::iterator ip;         
     real lon,lat,z;
     int status = 0;
     std::vector<Parcel>* stuff;
     int np;
     Parcel *pcl;
     int i;
     bool i_am_root;

     np = -1;
     swarm = NULLPTR;

     // one processor reads the file
     if ( pgrp == NULLPTR || pgrp->is_root()  ) {
        stuff = create_vector( p, file );
        np = stuff->size();
     } else {
        // this will be obtained from the Swarm below
        np = 0;
        stuff = NULLPTR;
     }

     if ( pgrp != NULLPTR ) {
        pgrp->sync();
     }
     
     // now create a Swarm of that many parcels
     // Note that only the root process has a valid value for np,
     // but that is all that is required here. 
     // The root processor will communicate the size of the
     // swarm to the other processors when the Swarm is constructed.
     swarm = new Swarm( p, pgrp, np, r);
     
     np = swarm->size();
     
     
     if ( np > 0 ) {   
        // sync all the processors before we start loading
        if ( pgrp != NULLPTR ) {
           pgrp->sync();
        }

        // the input parcel may be some special subclass of Parcel,
        // so we make a copy of it rather than trying to
        // use our own
        pcl = p.copy();
     
        i_am_root = swarm->is_root();
     
        for ( i=0; i<np; i++ ) {

            swarm->sync();
            
            try {
            
               if ( i_am_root ) {
                  // get the parcel
                  *pcl = (*stuff)[i];
               }
               
               // If we are the root processor, the parcel pcl
               // is valid, and it is sent to the processor
               // to which it belongs.
               // If we are not the root processor, then
               // then pcl has no valid value, but the
               // processor will ignore it and receive its
               // parcel value from the root processor.
               swarm->set( i, *pcl, 0 );

            } catch (std::ios::failure) {
               std::cerr << "Error reading parcel" << std::endl;
               throw (ParcelGenerator :: badgeneration());
            }   
        }
     
        delete pcl;   

     }
        
     if ( stuff != NULLPTR ) {
        delete stuff;
     }
     
     if ( swarm == NULLPTR ) {
        std::cerr << "null Swarm pointer" << std::endl;
        throw (ParcelGenerator :: badgeneration());
     }

     return swarm;

};


void PGenFile :: format( const string fmt )
{
    string fs;
    
    if ( fmt == "" ) {
       fs = "%o %a %v";
    } else {
       fs = fmt;
    }      
    
    interpretor->format( fmt );

}

std::string PGenFile :: format() const
{
    return interpretor->format();
}

