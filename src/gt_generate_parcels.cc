/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


/*!

\page  gt_generate_parcels gt_generate_parcels: Output a list of parcels suitable for input by gtmodel_s01

The gt_generate_parcels program uses a set of specifications to 
crate a distributions of Parcels, which is then output for use as
the initialization of a trajectory model.
For example, it can be used with gtmodel_s01 in this way:

\code
    gt_generate_parcels -n 100 \
        --clat 34.5 --clon -102.5 --radius 55 \
        --zlow 300 --zhigh 350 \
    | gtmodel_s01 --source MetGridSBRot \
        --begdate 2007-05-12T15:00 --enddate 2007-05-13T17:00 \
        --vertical Theta \
        --parcels -
\endcode    

The calling sequence is:
\code 
gt_generatge_parcels [ [--clon center_longitude ] [--clat center_latitude ] [--radius rad ] ] \
                     | [ [--LLlon lower_left_longitude] [--LLlat lower_left_latitude ] \
                         [--URlon upper_right_longitude] [--URlat upper_right_latitude ] ] \
                     [--zlow lower_vertical ] [--zhigh higer_vertical] \
                     [-n|--number n_parcels ] \
                     [ --random | --grid ] \
                     [ --format output_format ] \
                     [ --verbose ] \\
                     [ -h|--help ]  
\endcode

The command-line options are:

  \li \c help : prints out a description of options and then stops
  \li \c verbose : prints out messages to let the user know what progress is being made
  \li \c number: the number of parcels to be generated and output
  \li \c random: specified that the parcels are to be distributed randomly across the specified region
  \li \c grid: specifies that the parcles are to be distributed in a regular grid-like pattern across the specified region
  \li \c zlow: the lower vertical lmit of the specified region
  \li \c zhigh: the upper verticla limit of the specified region
  \li \c clon: for a cyclindrical region, the center longitude
  \li \c clat: for a cylindrical region, the center latitude
  \li \c radius: for a cylindrical region, the cylinder radius
  \li \c LLlon: for a rectangular region, the logoitude of the lower-left corner
  \li \c LLat: for a rectangular region, the latitude of the lower-left corner
  \li \c URlon: for a rectangular region, the longitude of the upper-right corner
  \li \c URlat: for a rectangular region, the latitude of the upper-right corner 
  \li \c format: the output format of the parcel information. 
     
 It is an error to specify options for a rectangular region with 
 options for a cylindrical region.

*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "gigatraj/gigatraj.hh"
#include "gigatraj/FilePath.hh"
#include "gigatraj/Configuration.hh"
 
#include "gigatraj/Parcel.hh"

#include "gigatraj/PGenGrid.hh"
#include "gigatraj/PGenDisc.hh"
#include "gigatraj/PGenRnd.hh"
#include "gigatraj/PGenRndDisc.hh"
 
#include "gigatraj/StreamPrint.hh"
  
using namespace gigatraj;
using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;
  

/*------------------------------------------------------------------------------------------*/
/* This function sets up configuration parameters and gathers settings
   from configuration files and command line options
*/
int getconfig(int argc, char * const argv[], Configuration& conf ) 
{

    // distribution mode: 0=regular grid-like; 1=uniform random
    int distmode;    

    // flag that indicates whether anything went wrong
    int status;
    // argv index, pointing to a command-line option
    int aidx;
    // print help text and quit?
    bool doHelp;
    // usage help string
    std::string usage;
    
    
    usage = "gt_generate_parcels ";
    
    // start pout assuming all is well
    status = 0;

    //---------- define the settings
    /*
       This serves as an example of how to use the Configuration class.
       The following lines each define a configuration parameter.
       The calling sequence is: 
         conf.add( name, type, defaultValue, shortOpt, flags, description )
         
         If your system has long-style command-line options, then you
         can set a parameter by putting two hyphens in front of the 
         parameter name ("--verbose"). You can also use the short form
         ("-v") if you like or if your system does not have long-style command line
         options.

    */

    usage +=  "[ --help|-h ]";
    conf.add("help"      , cBoolean, "N"                , "h", 0, "print help" );

    usage +=  " [--verbose] ";
    conf.add("verbose"   , cBoolean, "N"                , "" , 0, "print detailed progress messages" );

    usage +=  " [-n | --nunber N] ";
    conf.add("number"    , cInt,    "50"                , "n", 0, "the number of Parcels to generate" );
    
    usage += "  [--random | -r ] ";
    conf.add("random"    , cBoolean, "N"                , "r", 0, "distribute the parcels randomly" );   

    usage += "  [--grid | -g ] ";
    conf.add("grid"      , cBoolean, "Y"                , "g", 0, "distribute the parcels in a regular grid-like pattern" );   

    usage +=  " [--zlow vertical_low ]";
    conf.add("zlow"     , cFloat  , "400.0"             , "", 0, "lower vertical limit" );

    usage +=  " [--zhigh vertical_high ]";
    conf.add("zhigh"     , cFloat  , "420.0"             , "", 0, "upper vertical limit" );

    usage +=  " [--clat center_lat ]";
    conf.add("clat"     , cFloat  , ""             , "", 0, "latitude of cylinder center" );

    usage +=  " [--clon center_lon ]";
    conf.add("clon"     , cFloat  , ""             , "", 0, "longitude of cylinder center" );

    usage +=  " [--radius rad ]";
    conf.add("radius"     , cFloat  , ""             , "", 0, "radius of cylinder, in km" );

    usage +=  " [--LLlat lower_left_lat ]";
    conf.add("LLlat"     , cFloat  , ""             , "", 0, "latitude of lower-left corner of rectangle" );

    usage +=  " [--LLlon lower_left_lon ]";
    conf.add("LLlon"     , cFloat  , ""             , "", 0, "longitude of lower-left corner of rectangle" );

    usage +=  " [--URlat upper_right_lat ]";
    conf.add("URlat"     , cFloat  , ""             , "", 0, "latitude of upper-right corner of rectangle" );

    usage +=  " [--URlon upper_right_lon ]";
    conf.add("URlon"     , cFloat  , ""             , "", 0, "longitude of upper-right corner of rectangle" );

    usage +=  " [--format fmt]";
    conf.add("format", cString,"%o %a %v"                 , "", 0, "StreamPrint/StreamRead parcel format string" );


    // Load the config setting values from the command line
    // The defaults are loaded first
    // Then the command-line options
    aidx = conf.load(argc,argv);
    
    
    // take care of the "help" case here
    // (This is also an example of how to retrieve a parameter's value.)
    conf.fetchParam("help", doHelp);
    if ( conf.get("help") == "Y" ) {
       conf.help(usage,"");
       exit(0);
    }
    
    //---------- do some simple checks before returning to the main program
    
    // User can specify cylinder or rectangle options, but not both
    string cyl = conf.get("clat") + conf.get("clon") + conf.get("radius");
    string rect = conf.get("LLlat") + conf.get("LLlon") + conf.get("URlat") + conf.get("URlon");
    if ( cyl != "" && rect != "" ) {
       cerr << "Cannot specify both cylindrical and rectangular regions on the command line" << endl;
       status = 1;
    }
    
        
    // Any errors?  Print out a usage message.
    if ( status != 0 ) {   
       cerr << "Bad configuration." << endl;
       conf.help(usage,"");
    }
    
    return status;
}

  
int main( int argc, char * argv[] ) 
{
    // return status. 0 = all went well.
    int status;
    // configuration object
    Configuration config;
    // flag for printing information about what the program is doing
    bool verbose;                      
    // region-shape mdoe: -1=not set, 0=cylindr, 1=rectangle
    int shape = -1;
    // parcel distribution: 0 = regular grid-like; 1=rrandom uniform
    int distrib = 0;
    // cylinder longitude
    real clon = 0.0;
    // cylinder latitude 
    real clat = 0.0;
    // cylinder radius
    real radius = 50.0;
    // rectangle lower-left longitude
    real LLlon = -10.0;
    // rectangle lower-left latitude
    real LLlat = -10.0;
    // rectangle upper-right longitude
    real URlon = 10.0;
    // rectangle upper-right latitude
    real URlat = 10.0;
    // number of parcels to generate
    int n;
    // lower vertical boundary
    real zlow;
    // upper vertical boundary
    real zhigh;
    // output format
    string outform;
    // Parcel Generator objects
    PGenDisc      *p_cyl_grd;
    PGenRndDisc   *p_cyl_rnd;
    PGenGrid      *p_rct_grd;
    PGenRnd       *p_rct_rnd;
    // Sampl eParcel
    Parcel p;
    // the output vector of Parcels
    std::vector<Parcel> *result;
    // To print parcel info to stdout
    StreamPrint Out;
    // cube root of the number of parcels
    real n3;
    // delta-longitude for a regular rectangular grid
    real lonDelta;
    // delta-latitude for a regular rectangular grid
    real latDelta;
    // delta-vertical for a regular rectanglular grid
    real zDelta;
    // number of vertical levels for grid-like distirbutions
    real nz;
    // loop counter
    int i;

    // we assume all will go well (until it doesn't)    
    status = 0;

    // configure the model
    status = status | getconfig( argc, argv, config );

    // Proceed only if there have been no errors so far
    if ( status == 0 ) {

       if ( config.get("clat") != "" ) {
          config.fetchParam("clat", clat);
          shape = 0;
       }
       if ( config.get("clon") != "" ) {
          config.fetchParam("clon", clon);
          shape = 0;
       }
       if ( config.get("radius") != "" ) {
          config.fetchParam("radius", radius);
          shape = 0;
       }
       if ( config.get("LLlon") != "" ) {
          config.fetchParam("LLlon", LLlon);
          shape = 1;
       }
       if ( config.get("LLlat") != "" ) {
          config.fetchParam("LLlat", LLlat);
          shape = 1;
       }
       if ( config.get("URlon") != "" ) {
          config.fetchParam("URlon", URlon);
          shape = 1;
       }
       if ( config.get("URlat") != "" ) {
          config.fetchParam("URlat", URlat);
          shape = 1;
       }
       config.fetchParam("zlow", zlow);
       config.fetchParam("zhigh", zhigh);
       config.fetchParam("number", n);

       if ( config.get("grid") == "Y" ) {
          distrib = 0;
       }
       if ( config.get("random") == "Y" ) {
          distrib = 1;
       }
       outform = config.get("format");

       verbose = ( config.get("verbose") == "Y" );
       
       
       if ( shape < 0 ) {
          shape = 0;
       }

       if (verbose) {
          cerr << "verbose = " << verbose << endl;
          cerr << "shape = " << shape << endl;
          cerr << "  clon = " << clon << endl;
          cerr << "  clat = " << clat << endl;
          cerr << "  radius = " << radius << endl;
          cerr << "  LLlon = " << LLlon << endl;
          cerr << "  LLlat = " << LLlat << endl;
          cerr << "  URlon = " << URlon << endl;
          cerr << "  URlat = " << URlat << endl;
          cerr << "zlow = " << zlow << endl;
          cerr << "zhigh = " << zhigh << endl;
          cerr << "number = " << n << endl;
          cerr << "distrib = " << distrib << endl;
          cerr << "format = '" << outform << "'" << endl;
       }
       switch (shape) {
       case 0: 
              if ( verbose ) {
                 cerr << "Cylinder ";
              }
              switch (distrib) {
              case 0: 
                      if ( verbose ) {
                         cerr << "Grid " << endl;
                      }
                      zDelta = ABS(zhigh - zlow);
                      p_cyl_grd = new PGenDisc( clon, clat, (zlow + zhigh)/2.0, radius, zDelta ); 
                      result = p_cyl_grd->create_vector( p, n );
                      delete p_cyl_grd;
                      break;
              case 1:
                      if ( verbose ) {
                         cerr << "Random " << endl;
                      }
                      p_cyl_rnd = new PGenRndDisc( clon, clat, (zlow + zhigh)/2.0, radius, ABS(zhigh - zlow) ); 
                      result = p_cyl_rnd->create_vector( p, n );
                      delete p_cyl_rnd;
                      break;
              }   
              break;
       case 1: 
              if ( verbose ) {
                 cerr << "Rectangle ";
              }
              if ( URlon < LLlon ) {
                 URlon = URlon + 360.0;
              }
              switch (distrib) {
              case 0: 
                      if ( verbose ) {
                         cerr << "Grid " << endl;
                      }
                      // first, guess at how many levels
                      // by taking ( N^(1/3) )/ 4
                      n3 = POW( n*1.0, 1./3. );
                      nz = TRUNC(n3/4.0 + 0.5);
                      if ( nz <= 0 ) {
                          nz = 1;
                      }
                      // Now try to find a factor that divided into N evenly
                      n3 = (n*1.0)/nz;
                      if ( TRUNC(n3 + 0.5)*nz != n ) {
                         for (i=(nz*3)/2; i > 1; i-- ) {
                             n3 = (n*1.0)/i;
                             if ( TRUNC(n3 + 0.5)*i != n ) {
                                nz = i;
                                break;
                             }
                         }
                      }
                      n3 = TRUNC(SQRT(n/nz) + 0.5);
                      
                      lonDelta = (URlon - LLlon)/(n3 - 1.0);
                      latDelta = (URlat - LLlat)/(n3 - 1.0);
                      zDelta = (zhigh - zlow)/(nz - 1.0);
                      p_rct_grd = new PGenGrid();
                      //std::cerr << "LL=( " << LLlon << ", " << LLlat << " )" << endl;
                      //std::cerr << "UR=( " << URlon << ", " << URlat << " )" << endl;
                      //std::cerr << "z range = [" << zlow << ", " << zhigh << " ]" << endl;
                      //std::cerr << "lonDelta " << lonDelta << ", nlon = " << (URlon - LLlon)/lonDelta << endl;
                      //std::cerr << "latDelta " << latDelta << ", nlat = " << (URlat - LLlat)/latDelta << endl;
                      //std::cerr << "zDelta " << zDelta << ", num z " << (zhigh - zlow)/zDelta << endl;
                      //std::cerr << " nz = " << nz << ", n3 = " << n3 << endl;
                      result = p_rct_grd->create_vector( p 
                                                       , LLlon, URlon, lonDelta
                                                        , LLlat, URlat, latDelta
                                                        , zlow, zhigh, zDelta );
                      delete p_rct_grd;
                      break;
              case 1:
                      if ( verbose ) {
                         cerr << "Random " << endl;
                      }
                      p_rct_rnd = new PGenRnd( LLlat, LLlon
                                             , URlat, URlon );
                      p_rct_rnd->setZ( zlow, zhigh );
                      result = p_rct_rnd->create_vector( p, n );
                      delete p_rct_rnd;
                      break;
              }   
              break;
       }                
      
      
       Out.format(outform + "\n");
        
       // do printout here
       Out.apply( *result );
      
  
    }

    
    
    
    // Leave.
    exit(status);

}