#ifndef GIGATRAJ_CONFIGURATION_H
#define GIGATRAJ_CONFIGURATION_H

#include <string>
#include <vector>
#include <map>

#include "gigatraj/gigatraj.hh"

namespace gigatraj {

#ifdef USE_LONGARGS
#include <getopt.h>
#endif


/*!  \defgroup misc Miscellaneous

    Classes in the Miscellaneous group are those which do
    not fit in any of the other modules categories.  Generally, these provide
    access to some aspect of the computing environment which
    has been abstracted into classes for easy re-implementation
    in the future.

*/


/*! \defgroup configstuff Configuration

    The configuration class make it easy to define and use command-line options in a program.
    In gigatraj, it is used by the various tools.

*/    

/// \brief  type ConfigFlag holds flag values specifying certain characteristics of a confuiguration parameter
/*!  \ingroup configstuff
   A ConfigFlag variable holds one or more bitwise flags that determine how
   a configuration parameter is to be treated.
   
   These flags are set by ORing values together when a 
   configuration parameter is created.
*/
typedef int ConfigFlag;


/*! \defgroup ConfigVals ConfigFlag values
    \brief Configuration parameters can have certain flags set or cleared
    \ingroup configstuff
*/

/*!  ConfigFlag value:  cMustBePresent = Some value for a given parmeter must be specified
     \ingroup ConfigVals
*/
const ConfigFlag cMustBePresent = 0x0001;

/*!  \brief ConfigFlag value:  cMustBeValid = The value specified for a given configuration parameter must be valid. The most useful case is when the parameter is a file name, in which case validity means that the file must exist.
     \ingroup ConfigVals
*/
const ConfigFlag cMustBeValid = 0x0002;

/*!  \brief ConfigFlag value:  cNoReplaceConfig = If set, then the default paramter value cannot be replaced or overridden.
     \ingroup ConfigVals
*/
const ConfigFlag cNoReplace = 0x0004;


/// \brief type ConfigType encodes the type of a configuration parameter
/*!  \ingroup configstuff
   A ConfigType variable holds a code that indicates what type
   a configuration parameter is. The value of a configuration
   parameter is always stored as a string, but it may be
   chacked for validity before being cast by the calling program
   into a variable fo the appropriate type. That check, of course,
   requires that the type be known. 
   
   The ConfigType of a configuration parameter is set when the parameter
   is created.
*/
typedef int ConfigType;


/*! \defgroup ConfigType parameter types
    \ingroup configstuff
    Configuration parameters can be any of a set of types.
*/
/*!   ConfigType value: cNone = no type is defined (not used for defining parameters types!) 
     \ingroup ConfigType
*/
const ConfigType cNone = 0;
/*!  \brief  ConfigType value:  cBoolean = Either true or false. The parameter is true if specified, false otherwise.
     \ingroup ConfigType
*/
const ConfigType cBoolean = 1;
/*! \brief  ConfigType value: cInt = an integer value
     \ingroup ConfigType
*/
const ConfigType cInt = 2;
/*!  ConfigType value: cFloat = a floating-point number
     \ingroup ConfigType
*/
const ConfigType cFloat = 3;
/*! \brief  ConfigType value: cDouble = a double-precision floating-point number
     \ingroup ConfigType
*/
const ConfigType cDouble = 4;
/*!  ConfigType value: cString = a string
     \ingroup ConfigType
*/
const ConfigType cString = 7;
/*! \brief  ConfigType value: cConfig = a configuration file name
     \ingroup ConfigType
*/
const ConfigType cConfig = 10;





/*!
\ingroup configstuff
\brief manages configuration for the gigatraj model.


The Configuration class provides a facility for configuring standalone
programs that use the gigatraj model library.  Programs  register their
parameters with a Configuration object, along with any default values. 
The Configuration object can then override defaults by reading 
one or more configuration files, or by getting arguments from the
command line.

Configuration information takes the form of named parameters, each 
consisting of a string value, a type indicator, and flags.
The names of the parameters, their types, flags, and default values
are all defined by the programmer when setting up the Configuration object.
The Configuration class exists to assist the programmer in managing
parameters, not to dictate those parameters.

It is possible to write a program that relies on a Configuration object
to supply default values only, but there would be little advantage in that.
Likewise, one could write a program that takes all of its parameters from
command line options alone, but this would result in some very long
and unwieldy command lines. The main source of configuration parameters
will in most cases be the contents of configuration files.

Configuration files are plain text files that spefiy parameters as "keyword=value"
pairs, one per line. Blank lines and comments (lines beginning with "#") are
ignored.  Some files are intended to be used for general settings applicable
across a wide variety of programs. Such files contain a few parameters that may be common to
a number of gigatraj-based programs, so the file may be shared by all of them.
Other configuration files may be intended as more program-specific, containing a wider
and more extensive set of specifications.
Either way, configuration files may be specified as parameter defaults, or they may
be specified on the command line. Multiple such files may be defined;
they are applied in the order in which they are specified.

In addition, the Configuration object allows for its parameter settings
to be specified on the command line when the program is run.
These may be
specified with GNU-style long options (e.g., "--verbose"), or with traditional 
single-character options (e.g, "-v").  If long optiona are used, the name
of the option is the parameter name.

The order in which configuration parameters are taken is the reverse of
the order of their priority: the last parameters input override any 
previous settings of those parameters. Thus the default values, hard-coded into
a program as they are creaded, have the lowest priority. 
The configuration files are then read, in order, with their settings
overriding any default values.  Finally, any settings specified on
the command line override all others.

Note that if an unknown (e.g., misspeled) parameter is specified on the
command line, and error is thrown. If an unknown parameter is encountered in
a configuration file, and strict mode is off, then no error is thrown;
this permits several related-but-distict programs to share one configuration
file.

This raises the question, how are configuration files
to be specified, aside from default values? The answer is that if
the program defines a configuration parameter for a configuration file, 
that parameter may be set on the command line.
All such parameters will be read first.
So then the final order in which parameters take effects is: default values,
then command-line-specified configuration file names, then the 
config configuration files, and
finally the non-config file command-line options.

Here is an example of how to use a configuration object:


\code
#include "gigatraj/Configure.hh"

int main(int argc, char * const argv[])
{

    Configuration conf;
    std::string verbose, cdir;
    int first_arg;
    
    // Define a config file whose default location is system-wide.
    // this file must exist, or the program will fail.
    conf.add( "CFG", cConfig, "/etc/myprogram.conf", cMustBeValid );
    
    // Define a configuration file whose default location is $HOME/.gigatrajrc
    // This can be overridden on the command line.
    conf.add( "RCFYLE", cConfig, string(getenv("HOME")) + "/.gigatrajrc" );
    
    // define a config file local to this directory, for this program. 
    // This cannot be overridden by a config file setting or the command line.
    conf.add( "MYCFG", cConfig, "myprogram.conf", cNoReplace );
    
    
    // register these program parameters.  Note that all parameter values are retrievd as strings.
    // It is up to the program to convert the strings to other types
    // such as integers, as appropriate.
    
    // Register the parameter "verbose" with the default value "N" (for "No").
    // Accept "v" as the short version of the option.
    // This is a boolean parameter, so it stands alone (i.e., "--verbose", 
    // not "--verbose=maybe"
    conf.add("verbose", cBoolean, "N", "v");
    
    // Register the option "--cachedir=directorypath".
    // The default path is "./".
    // The short version of the option is "-o".
    conf.add("cachedir", "./", "o");

    // Register the option "--config=AConfigFile".
    // The default value is "my.conf".
    // This does specify configuration file, which will automatically
    // be read in later.
    conf.add("config", "my.conf", "c", 1 );

    // With all options registered, get their values for this program.
    // Note that if the "--config=AConfigFile" option was used, 
    // that file will be read here, and its options loaded.
    // "first_arg" will be set to the index into argv[] of the first
    // command-line option that was not recognized. Typically this
    // will be the first non-option argument, but it might be a user typo.
    // (It is up to the application to check.)
    first_arg = conf.load(argc,argv);
    
    // Retrieve the option values
    vertical = conf.get("vertical");
    cdir = conf.get("cachedir");
    // Note that we do not retrieve the option value for "config",
    // since that is useless to us at this point.

    return 0;

}
\endcode



*/
class Configuration {

   public:
   
      /// An exception for attempting to duplicate a parameter
      class badparamconflict {};
      
      /// An exception for trying to access a missing parameter
      class badparamnotfound {};

      /// An exception for bad command-line options
      class badcmdline {};
      
      /// An exception for an error reading a file
      class badcfgread {};

      /// An exception for a badly formatted parameter value
      class badcfgformat {};

      /// An exception for a memory allocation failure
      class badmemerr {};
      
      /// An exception for asking for a parameter of the wrong type
      class badparamtype {};


      /// the basic constructor
      /*! This is the default constructor.
      */
      Configuration();
   
      /// constructor w/ file name
      /*! This constructor takes the name of a configuration file

          \param cfg the name of the configuration file

      */     
      Configuration( const std::string &cfg);
     
      /// constructor w/ multiple file names
      /*! This constructor takes a vector of configuration file names
      
          \param cfgs a String vetcor of configuration file names
      */     
      Configuration( const std::vector<std::string> &cfgs);
     
      /// Copy-constructor
      /*! This is the copy-constructor.
      */
      Configuration(const Configuration&);
      
      /// The destructor
      /*! This is the destructor.
      */
      ~Configuration();

      /// assignment operator
      /*! This overklays the equals operator to implement Confiruation assignment.
      */
      Configuration& operator=(const Configuration& src);
      
   
      /// registers a parameter.
      /*! registers a parameter with the Configuration object.
         \param name the name of the parameter
         \param type = The type of the parameter: cInt, cFloat, cDouble, cBoolean, cString, or cConfig.
                       Parameters of type cConfig have values which are the names of
                       configuration files full of configuration settings to be read.
         \param deflt a default value for this parameter. This should be NULL if there is no default.
                      If given, this is the value the parameter will have 
         \param shortopt the short name, used for short command-line options. 
         \param flags  a bitwise combination of attributed of the parameter:
                       cMustBePresent, cMustBeValid, or cNoReplace.
         \param help = a short descriptive string used in help/usage text of the application.
      */
      void add( const std::string &name, const ConfigType type, const std::string &deflt, const std::string &shortopt, const ConfigFlag flags=0, const std::string &help ="");

      /// loads all parameter settings
      /*! load loads the configuration from the resource file, the configuration file, and/or the command line
          
          \param argc the number of arguments (passed from main())
          \param argv an array of character arrays (passed from main())
          
          \return the index into argv of the first non-option command-line argument, or
                  -1 if there is none. Thus the calling application can easily access
                  and process any such arguments after the options have been loaded.
      */
      int load(int argc, char * const argv[] );

      /// resets all parameter settings
      /*! clears the Configuration object of all parameters and settings
      
      */
      void clear();
       
      /// retrieves a configuration parameter setting
      /*! get retrieves a parameter setting.  It should be called after the
          load method has been called. 
      
          \param name the name of the desired parameter
          \return the string value of the requested parameter
      */    
      std::string get(const std::string &name);
      
      /// sets a configuration parameter 
      /*! The set method loads a single parameter setting.  It is intended
          for use in situations in which the parameter's default value should
          depend on some internal logic of the application. Remember that
          configuration values are always stored as strings. The version of set()
          that takes a string argument may be used to set any type of parameter.
          Alternate versions of set() exist to set a parameter of the appropriate type.
          
      
          \param name the name of the desired parameter
          \param value a string containing the value of the parameter
      */    
      void set(const std::string &name, const std::string &value);

      /// sets an integer configuration parameter 
      /*! This version of the set method loads a single parameter setting of type cInt.  
          It is intended
          for use in situations in which the parameter's default value should
          depend on some internal logic of the application. Remember that
          configuration values are always stored as strings. 
          
          
          \param name the name of the desired parameter
          \param value the integer value of the parameter
      */    
      void set(const std::string &name, const int &value);

      /// sets a floating-point configuration parameter 
      /*! This version of the set method loads a single parameter setting of type cFloat or cDouble.  
          It is intended
          for use in situations in which the parameter's default value should
          depend on some internal logic of the application. Remember that
          configuration values are always stored as strings, so some precision
          may be lost in converting the real value to a string. 
          
          \param name the name of the desired parameter
          \param value the real value of the parameter
      */    
      void set(const std::string &name, const float &value);

      /// sets a double-precision configuration parameter 
      /*! This version of the set method loads a single parameter setting of type cFloat or cDouble.  
          It is intended
          for use in situations in which the parameter's default value should
          depend on some internal logic of the application. Remember that
          configuration values are always stored as strings, so some precision
          may be lost in converting the real value to a string. 
          
          \param name the name of the desired parameter
          \param value the real value of the parameter
      */    
      void set(const std::string &name, const double &value);

      /// sets an boolean configuration parameter 
      /*! This version of the set method loads a single parameter setting of type cBoolean.  
          It is intended
          for use in situations in which the parameter's default value should
          depend on some internal logic of the application. Remember that
          configuration values are always stored as strings; true is stored
          as "Y", false as "N". 
          
          \param name the name of the desired parameter
          \param value the boolean value of the parameter
      */    
      void set(const std::string &name, const bool &value);
      
      
      /// converts a string to an integer
      /*! str2int is a utility routine to convert a string to an integer
         
         \param value string representation of the integer value
         \return the integer value of the given string
      */
      int str2int(const std::string &value);
      
      /// converts a string to a real
      /*! str2float is a utility routine to convert a string to a floating-point number
         
         \param value string representation of the real value
         \return the float value of he given string
      */
      float str2float(const std::string &value);
      
      /// converts a string to a double
      /*! str2dbl is a utility routine to convert a string to a double-precision floating-point number
         
         \param value string representation of the value
         \return the double-precision value of the given string
      */
      double str2dbl(const std::string &value);
      
      /// converts a string to a boolean
      /*! str2bool is a utility routine to convert a string to a boolean value
         
         \return the boolean value of the given string. 
                 A string that starts with "y", "Y", "t", or "T" is true;
                 a strinbg that starts with "n", "N", "F", or "f" is false.
                 All other string vlaues will trhow a bsdcfgformat exception.
         \param value string representation of the value
      */
      bool str2bool(const std::string &value);


      /// retrieves an integer parameter value 
      /*! fetchParam returns a given parameter as its proper type.
      
          \param name the name of the parameter being requested
          \param value returns the value of the parameter
      */
      void fetchParam(const std::string &name, int &value);

      /// retrieves a floating-point parameter value 
      /*! fetchParam returns a given parameter as its proper type.
      
          \param name the name of the parameter being requested
          \param value returns the value of the parameter
      */
      void fetchParam(const std::string &name, float &value);

      /// retrieves a double-precision floating-point parameter value 
      /*! fetchParam returns a given parameter as its proper type.
      
          \param name the name of the parameter being requested
          \param value returns the value of the parameter
      */
      void fetchParam(const std::string &name, double &value);
      
      /// retrieves a boolean parameter value 
      /*! fetchParam returns a given parameter as its proper type.
      
          \param name the name of the parameter being requested
          \param value returns the value of the parameter
      */
      void fetchParam(const std::string &name, bool &value);
      
      /// retrieves a string parameter value 
      /*! fetchParam returns a given parameter as its proper type.
      
          \param name the name of the parameter being requested
          \param value returns the value of the parameter
      */
      void fetchParam(const std::string &name, std::string &value);

      /// prints out help/usage text about the parameters
      /*! The help method prints out a short description of each parameter to stderr.
          The parameter descriptions will be printed in the order in which
          the parameters were registered with the Configure object.
        
          \param intro a string to be printed out before the parameter descriptions.
          \param postscript a string to be printed out after the parameter descriptions.
          
      */
      void help(const std::string intro="", const std::string postscript="");


   private:

      /// delete these:
      /// the parameter was set from the resource file
      static const int ParamFromRc = 4;   
      

      /*! \name ParamSet flags
          These flags indicate whether and how a parameter was set
      */
      //@{
      /// the parameter has been set
      static const int ParamWasSet      = 1;
      /// the parameter was set from the command line
      static const int ParamFromCmdLine = 2;   
      /// the parameter was set from the configuration file
      static const int ParamFromCfg = 4;   
      //@}
      
      /// strips leading and trailing whitespace from a string
      /*! strwht prepares a string for parsing by removing any
          leading or trailing whitespace characters (i.e., tabs and spaces).
        
        \return the cleansed string
        \param input the string to be cleaned
      */
      std::string strwht( const std::string input );

      char *cstring( std::string s );
      
     /*! class for holding getopts information
     */
     class GoInfo {
     
         public:
            int n;
            char **names;
            char *shortopts;
#ifdef USE_LONGARGS
            struct option *longopts;
#endif
       
          /// constructor
          GoInfo();
          
          // destructor
          ~GoInfo();
          
          void clear();
     
     };

     void setupGetopts( GoInfo &go );

     void takedownGetopts( GoInfo &go );
     

      /// extracts configuration files and resource files from the command line
      /*! cmdLineConfigs goes through the command-line arguments and identifies any
          that set a config file parameter. The value of that parameter
          is then replaced by the value specified on the command line.
      
      */
      void cmdLineConfigs(int argc, char * const argv[] );

      /// loads parameter settings from a file
      /*! loadfile serves as the back-end for the loadResource and loadConfig methods, which
          do essentially the same thing with different files and priorities.
      
          \param filename the name of the file to be read

      */
      void loadfile( const std::string &filename, const std::string cfgname, int strict=0 );


      /// loads parameter settings from the command line.
      /*! loadCmdLine reads configuration settings from the command line
      
          \param argc the number of arguments (passed from main())
          \param argv an array of character arrays (passed from main())
          
          \return the index into argv of the first non-option parameter,
                  or -1 if there is none.
      */
      int loadCmdLine(int argc, char * const argv[] );

      /*! structure for holding parameter information
          - name = the name of the parameter (also the long command-line option)
          - type = the type of parameter (ConfigType)
          - short = the short command-line option
          - flags = flag values (ConfigFlag)
          - value = the value to which the parameter was set
          - description = a short description of what the parameter is for (for help/usage text)
      */
      struct param {
          std::string name;
          char shortopt;
          ConfigType type;
          ConfigFlag flags;
          std::string value;
          std::string description;
          int status;
      };
      
      /// checks the syntax of a parameter value
      /*! check inspects a parameter value to see if it is
          legal for the parameter type.
          
          \return true if the paramater value is OK
          \param p the parameter structure
      */    
      bool check(const struct param *p);
      
      ///  holds a sequence of configuraton files.
      //   The 0th element is the most global ("configuration file")
      //   and the final element is the most specific ("resource file").
      std::vector<std::string> configs;
      
      /// the parameter name for each config file
      // these are the names of parameters (i.e., keys to params)
      std::vector<std::string> cfgnames;

      /// the names of the parameters
      std::vector<std::string> paramNames;
      
      /// set to 1 if the configuration has been loaded.
      int status;
      
      /// map of parameters and their values
      std::map<std::string, struct param> params;
      
      /// finds a parameter
      /*! findParam returns a pointer to a param sturcture, given a name
       
          \param name the name of the parameter
          \return a param structure that describes the parameter
      */    
      struct param findParam(const std::string &name);


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
