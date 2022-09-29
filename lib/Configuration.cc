
/******************************************************************************* 
***  Written by: 
***     L. R. Lait (SSAI) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
***  (Please see the COPYING file for more information.) 
********************************************************************************/


#include "gigatraj/Configuration.hh"
#include "gigatraj/FilePath.hh"

#include "config.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <iosfwd>
#include <sstream>


extern int optind;

using namespace gigatraj;


Configuration::Configuration()
{

    // nothing read in yet
    status = 0;
    
}

Configuration::Configuration(const std::string &cfg)
{

    configs.clear();
    configs.push_back(cfg);
    
    status = 0;
    
}

Configuration::Configuration(const std::vector<std::string> &cfgs)
{
    std::vector<std::string>::const_iterator cfg;
    
    configs.clear();
    
    for ( cfg = cfgs.begin(); cfg != cfgs.end(); cfg++ ) {
        configs.push_back(*cfg);
    }
    
    status = 0;
    
}

Configuration::~Configuration()
{
}

Configuration::Configuration(const Configuration& src)
{
    std::vector<std::string>::const_iterator si;
    std::vector<std::string>::const_iterator ni;

    configs.clear();
    cfgnames.clear();
    for ( si=src.configs.begin(), ni=src.cfgnames.begin() ; si != src.configs.end(); si++, ni++ ) {
        configs.push_back( *si );
        cfgnames.push_back( *ni );
    }
 
    status = status;
    params = src.params;

}

Configuration& Configuration::operator=(const Configuration& src)
{
    std::vector<std::string>::const_iterator si;
    std::vector<std::string>::const_iterator ni;

    // handle assignment to self
    if ( this == &src ) {
       return *this;
    }

 
    configs.clear();
    cfgnames.clear();
    for ( si=src.configs.begin(), ni=src.cfgnames.begin(); si != src.configs.end(); si++, ni++ ) {
        configs.push_back( *si );
        cfgnames.push_back( *ni );
    }
 
    status = status;
    params = src.params;

    return *this;
}


struct Configuration::param Configuration::findParam(const std::string &name) 
{
     struct param p;
     
     // iterator for the map of registered parameters
     std::map<std::string, struct param>::iterator piter;
     
     // find the registered parameter that has the desired name
     piter = params.find(name);
     // if the parameter exists, get its value 
     if ( piter != params.end() ) {
         std::string keyword = piter->first;
         p = piter->second;
         return p;
     } else {
         std::cerr << "There is no parameter with name '" << name << "'" << std::endl;
         throw(badparamnotfound());
     }

}

bool Configuration::check(const struct param *p)
{
     bool result;
     // test variables
     bool junkb;
     int junki;
     real junkr;
     
     result = true;
     if ( p->flags & cMustBeValid ) {
        try {
           switch (p->type) {
           cBoolean: junkb = str2bool( p->value );
                     break;
           cInt:     junki = str2int( p->value );
                     break;
           cFloat:    junkr = str2float( p->value );
                     break;
           default:
                   // do nothing
                   break;
           }
        } catch (...) {
           result = false;
        }
     }

     return result;
}
 
void Configuration::set(const std::string &name, const std::string &value)
{

     struct param p;
     
     p = this->findParam(name);
     
     p.value = value;

     p.status = p.status | ParamWasSet;  
     
     // depending on the parameter type and the cMustBeValid flag, check the value syntax here
     if ( ! this->check( &p ) ) {
        throw(Configuration::badcfgformat());
     }
     
     // all is ok at this point, so replace the value in the map
     params[name] = p;
      
     return;

}

void Configuration::set(const std::string &name, const int &value)
{

     struct param p;
     std::ostringstream *numstr; 
     
     p = this->findParam(name);
     
     if ( p.type == cInt ) {
        numstr = new std::ostringstream;
        *numstr << value;
        p.value = (*numstr).str();
        delete numstr;
     
        p.status = p.status | ParamWasSet;  
     } else {
        throw(Configuration::badcfgformat());     
     }
     
     // all is ok at this point, so replace the value in the map
     params[name] = p;
     
     return;

}

void Configuration::set(const std::string &name, const float &value)
{

     struct param p;
     std::ostringstream *numstr; 
     
     p = this->findParam(name);
     
     if ( p.type == cFloat || p.type == cDouble ) {
        numstr = new std::ostringstream;
        *numstr << value;
        p.value = (*numstr).str();
        delete numstr;
     
        p.status = p.status | ParamWasSet;  
     } else {
        throw(Configuration::badcfgformat());     
     }
     
     // all is ok at this point, so replace the value in the map
     params[name] = p;
     
     return;

}
void Configuration::set(const std::string &name, const double &value)
{

     struct param p;
     std::ostringstream *numstr; 
     
     p = this->findParam(name);
     
     if ( p.type == cFloat || p.type == cDouble ) {
        numstr = new std::ostringstream;
        *numstr << value;
        p.value = (*numstr).str();
        delete numstr;
     
        p.status = p.status | ParamWasSet;  
     } else {
        throw(Configuration::badcfgformat());     
     }
     
     // all is ok at this point, so replace the value in the map
     params[name] = p;
     
     return;

}

void Configuration::set(const std::string &name, const bool &value)
{

     struct param p;
     
     p = this->findParam(name);
     
     if ( p.type == cBoolean ) {
        if ( value ) {
           p.value = "Y";
        } else {
           p.value = "N";
        }         
     
        p.status = p.status | ParamWasSet;  
     } else {
        throw(Configuration::badcfgformat());     
     }
     
     // all is ok at this point, so replace the value in the map
     params[name] = p;
     
     return;

}


void Configuration::add( const std::string &name, const ConfigType type, const std::string &deflt, const std::string &shortopt, const ConfigFlag flags, const std::string &help )
{
    // holds the new parameter specs
    struct param p;
    
    // copy the specs into place
    p.name = name;
    p.type = type;
    p.value = deflt;
    p.shortopt = shortopt[0];  // single character
    p.flags = flags;
    p.description = help;
    p.status = 0;

    // does this parameter already exist?
    if ( params.count(name) <= 0 ) {
       // No, it's new.
       
       // std::cerr << "adding " << name << std::endl;
       
       // save it
       params[name] = p;
       
       // and if it is a config file, remember that, too
       if ( type == cConfig ) {
          configs.push_back( deflt );
          cfgnames.push_back( name );
       }
       
       paramNames.push_back( name );
       
    } else {
       // Already exists.
       // But we cannot register the same parameter twice
       
       // std::cerr << "found " << name << "?" << std::endl;
       throw (badparamconflict());    
    }
    
}

std::string Configuration::strwht( const std::string input )
{
     // local working copy of the input string
     std::string value;
    
     value = input;
    
     // strip off leading whitespace
     while ( value[0] == ' ' || value[0] == '\t' ) {
          value.erase( value.begin() );
     }
     // strip off the end
     while ( value[value.size()-1] == ' ' || value[value.size()-1] == '\t' ) {
          value.erase( value.end() );
     }
     
     return value;

}


char *Configuration::cstring( std::string s ) {

     char *result;
     
     // make space for the string
     try {
        result = new char[s.size() + 1];
     } catch (...) {
        throw(Configuration::badmemerr());               
     }
     
     strncpy( result, s.c_str(), s.size()+1 ); // +1 to copy the null char
     
     return result;

}

void Configuration::setupGetopts( struct gostuff &go ) {

    // short options string
    std::string shortopts;
    // iterates over the parameter set
    std::map<std::string, struct param>::iterator piter;  
    // counter
    int i;  
    // parameter name
    std::string pname;
    // parameter
    struct param p;
    // jholding string
    char *cstr;
#ifdef USE_LONGARGS
    struct option *longopts;
#endif
    
    // how many parameters are defined in this object?
    go.n = params.size();

   
    // first, we set up the option strings that we want
    // to recognize. These will include ALL parameters, so
    // that getopts/getopts_long will not return an error
    // if it sees a non-config option specified.


#ifdef USE_LONGARGS
    // allocate memory for the long-format options
    try {
       longopts = new struct option[go.n + 1];
    } catch (...) {
       throw(badmemerr());
    }
#endif
    // set up the options string
    shortopts = "";
    
    // allocate memory for the names strings
    // (we will want to free these afterwards, and the struct option.name
    // is a const char*, so we start these here.)
    try {
       go.names = new char*[go.n];
    } catch(...) {
       throw(badmemerr());
    }
    
    
    // for each registered program parameter....
    for ( i=0, piter = params.begin() ;
          piter != params.end();
          i++, piter++ ) {
     
         // get the name of the parameter
         pname = piter->first;

         
         // get the parameter itself
         p = piter->second;
         
#ifdef USE_LONGARGS
         // copy the name into place
         go.names[i] = cstring(pname);
         // and copy the pointer to where we need it 
         longopts[i].name = go.names[i];
         
         // the parameter takes a value (required_argument is defined in getopt.h)
         if ( p.type == cBoolean ) {
            // it's a boolean parameter
            longopts[i].has_arg = no_argument;
         } else {
            // the parameter takes a value
            longopts[i].has_arg = required_argument;
         }
         // we want a flag returned from getopts_long below, not the value itself
         longopts[i].flag = NULL;
         longopts[i].val = 0;
      
#endif
         // add a short option for this parameter?
         if ( p.shortopt != 0 ) {
            if ( p.type == cBoolean ) {
               shortopts = shortopts + std::string(1,p.shortopt);
            } else {
               shortopts = shortopts + std::string(1,p.shortopt) + ":";
            }
         }

    }

#ifdef USE_LONGARGS
    // terminate the list of long options
    // with a null option
    longopts[go.n].name = NULL;
    longopts[go.n].has_arg = 0;
    longopts[go.n].flag = NULL;
    longopts[go.n].val = 0;
    
    go.longopts = longopts;

#endif

     // set up the short options string in the form in which we will need it
     go.shortopts = cstring( shortopts );


    // reset the getops option index counter
    // (extern declared at the top of this file)
    optind = 1;


}

void Configuration::takedownGetopts( struct gostuff &go ) {
    
    int i;
    
#ifdef USE_LONGARGS
    for (i=0; i<go.n; i++ ) {
        delete go.names[i];
    }
    delete go.names;
    delete go.longopts;
#endif
    delete go.shortopts;

}



void Configuration::cmdLineConfigs(int argc, char * const argv[] )
{

    // 0 = all is well.
    int status = 0;
    // option character index 
    int idx;
    int opt_index;
    // number of parameters
    int n;
    // temporary string
    string pname;
    // holds specs for long options
    struct option *longopts;
    // short options string
    char *shortopts;
    // iterates over the parameter set
    std::map<std::string, struct param>::iterator piter;  
    // counter
    int i;  
    // hold a parameter value
    std::string cval;
    // 
    char *cstr;
    // getopts stuff
    struct gostuff go;
    
    
    // how many parameters are defined in this object?
    n = params.size();
    
    setupGetopts( go );
    shortopts = go.shortopts;
    longopts = static_cast< struct option *>(go.longopts);

    
    // loop through the options from the command line
    // (We'll break out of this loop when we are done.)
    while (1) {
    
#ifdef USE_LONGARGS
       // find which long or short option this corresponds to
       idx = getopt_long(argc, argv, shortopts, longopts, &opt_index );
#else
       // (no long options) find the short option this corresponds to
       idx = getopt(argc, argv, shortopts );
#endif
       
       if ( idx == -1 ) {
          // does not match any registered option.
          // if we are using long options, we are done.
          break;
       }
       
       // which registered parameter was specified as an option?
       switch (idx) {
#ifdef USE_LONGARGS
       case 0: // long option
       
             // which option?
             pname = std::string(longopts[opt_index].name);
             
             // in this routine, we deal only w/ config files
             if ( params[pname].type == cConfig ) {
                // set the parameter to the option value
                params[pname].value = std::string(optarg);
                
                if ( params[pname].flags && cNoReplace ) {
                   std::cerr << "cannot set parameter " << pname << " from cmd line " <<  std::endl;
                   throw(Configuration::badcmdline());               
                }
             
                // find which config file we need to replace
                for ( i=0; i < configs.size(); i++ ) {
                    if ( cfgnames[i] == pname ) {
                        configs[i] = params[pname].value;
                        break;
                    }
                }
             
                params[pname].status = params[pname].status | ParamFromCmdLine;
             
             }
             
             break;
#endif
       default: // short options. idx is the option character
            // for each registered program parameter...
            for ( std::map<std::string, struct param>::iterator piter= params.begin() ;
                piter != params.end();
                piter++ ) {
                   // get the parameter name 
                   std::string pname = piter->first;
                   // get the parameter itself
                   struct param p = piter->second;
                   
                   // do the short options match?
                   if ( p.shortopt == idx ) {
                      // in this routine, we deal only w/ config files
                      if ( p.type == cConfig ) {
                          // replace the config file name
                          p.value = std::string(optarg);

                          p.status = p.status | ParamFromCmdLine;  
 
                          // store the altered parameter
                          params[pname] = p;     
                
                          // find which config file we need to replace
                          for ( i=0; i < configs.size(); i++ ) {
                              if ( cfgnames[i] == pname ) {
                                  configs[i] = params[pname].value;
                                  break;
                              }
                          }
                          
             
                      }
                    
                      // no need to go through the remaining parameters
                      break;  
                   }
            }
            break;     
       }
    }


    takedownGetopts( go );
    
    
    status = 1;

}



void Configuration::loadfile( const std::string &filename, const std::string cfgname, int strict )
{
     // a line read from the file
     std::string line;
     // the keyword part of a keyword=value pair
     std::string keyword;
     // the value part of a keyword=value pair
     std::string value;
     // string position that separates the keyword part from the value part
     size_t pos;
     // line count from the file
     int lineno;
     // iterators over the parameter map
     std::map<std::string, struct param>::iterator pidx;
     // file name string (for opening)
     const char *fname;
     // input file stream
     std::ifstream *input;
     // temporary parameter 
     struct param p;
     // config file parameter
     struct param cfgparam;
     
     cfgparam = params[cfgname];
     
     // open the file
     fname = filename.c_str();
     input = new std::ifstream(fname);
     if ( ! input->good() ) {
        // oops; could not open it
        delete input;
        if (cfgparam.flags & cMustBeValid) {
           // it was mandatory, so throw an error
           throw (badcfgread());
        } else {
           // not mandatory, so who cares?
           return;
        }   
     }
     
     // file is open; try getting its parameters
     try {

        // catch I/O errors
        input->exceptions(std::ios::badbit);
        
        lineno = 0;
        // read each line
        while ( (! input->eof()) && (! input->fail()) && input->good() ) {
        
              lineno++;
        
        
              std::getline(*input, line);
              
              // strip off leading and trailing whitespace
              line = strwht(line);
              
              // ignore empty lines and comments
              if ( line == "" | line[0] == '#' ) {
                 continue;
              }
              
              // split the line into keyword and value
              pos = line.find('=');
              if ( pos != std::string::npos ) {
              
                 keyword = strwht( line.substr(0,pos) );
                 value = strwht( line.substr(pos+1) );
                 
                 // has a parameter(keyword) with this name been registered?
                 pidx = params.find(keyword);
                 if ( pidx != params.end() ) {
                    // has been defined.
                    
                    // grab the parameter itself
                    p = pidx->second;
                    
                    // but we cannot set config file names from within config files!
                    if ( p.type == cConfig ) {
                       std::cerr << "cannot set config file name " << keyword << " from within config " << filename << " on line " << lineno <<  std::endl;
                       throw(Configuration::badcfgread());               
                    }
                    
                    // also, are we even allowed to set this parameter?
                    if ( p.flags && cNoReplace ) {
                       std::cerr << "cannot set parameter " << keyword << " in " << filename << " on line " << lineno <<  std::endl;
                       throw(Configuration::badcfgread());               
                    }
                    
                    // set the parameter value
                    p.value = value;
                    // set its status
                    p.status = p.status | ParamFromCfg;
                    
                    // depending on the parameter type and the cMustBeValid flag, check the value syntax here
                    if ( ! this->check( &p ) ) {
                       throw(Configuration::badcfgformat());
                    }

                    // replace the current parameter with our newly-read version
                    params[ pidx->first ] = p;

                 } else {
                    // parameter found in source but not registered with us
                    if ( strict ) {
                       std::cerr << "unknown keyword " << keyword << " in " << filename << " line " << lineno <<  std::endl;
                       throw(Configuration::badcfgread());               
                    }
                 }
              } else {
                  // syntax error
                  std::cerr << "Syntax error in " << filename << " line " << lineno << ": " << line << std::endl;
                  throw(Configuration::badcfgread());
              } 
                 
        }
     } catch (std::ios::failure) {
        throw(Configuration::badcfgread());
     }   
     
     // close the file
     input->close();
     delete input;
   
}


int Configuration::loadCmdLine(int argc, char * const argv[] )
{

    // 0 = all is well.
    int status = 0;
    // option character index 
    int idx;
    int opt_index;
    // number of parameters
    int n;
    // parameter name
    string pname;
    // holds specs for long options
    struct option *longopts;
    // short options string
    char *shortopts;
    // interates over the parameter set
    std::map<std::string, struct param>::iterator piter;  
    // counter
    int i;  
    // getopts stuff
    struct gostuff go;
    // argv index of first non-option argument 
    int pcount = 0;
    
    
    // how many parameters are defined in this object?
    n = params.size();

    setupGetopts( go );
    shortopts = go.shortopts;
    longopts = static_cast< struct option *>(go.longopts);

    // loop through the options from the command line
    while (1) {
    
#ifdef USE_LONGARGS
       // find which long or short option this corresponds to
       idx= getopt_long(argc, argv, shortopts, longopts, &opt_index );
#else
       // (no long options) find the short option this corresponds to
       idx = getopt(argc, argv, shortopts );
#endif
       if ( idx == -1 ) {
          // does not match any registered option.
          // if we are using long options, we are done.
          break;
       }
       
       // which registered parameter was specified as an option?
       switch (idx) {
#ifdef USE_LONGARGS
       case 0: // long option
             // which option?
             pname = std::string(longopts[opt_index].name);

 
             if ( params[pname].flags && cNoReplace ) {
                std::cerr << "cannot set parameter " << pname << " from cmd line " <<  std::endl;
                throw(Configuration::badcmdline());               
             }
             
             switch (params[pname].type) {
             case cBoolean:
                // boolean set to true 
                params[pname].value = "Y";
                params[pname].status = params[pname].status & ParamFromCmdLine;
                break;
             case cConfig:
                //  no action taken for Config type (already dealt with those in cmdLineConfigs() )
                break;
             default:
                // set the value, but only if not a Confoig
                if ( params[pname].type != cConfig ) {
                   params[pname].value = std::string(optarg);
                   params[pname].status = params[pname].status & ParamFromCmdLine;
                }
                break;
             }    
             break;
       case 63: // '?', which indicates an error--an unknown parameter
             throw(Configuration::badcmdline());                      
             break;
#endif
       default: // short options
             
            // for each registered parameter...
            for ( std::map<std::string, struct param>::iterator piter= params.begin() ;
                piter != params.end();
                piter++ ) {
                   // get the parameter name (i.e., the keyword)
                   std::string pname = piter->first;
                   
                   // get the parameter itself
                   struct param p = piter->second;
                   
                   // set the parameter value from the option value
                   if ( p.shortopt == idx ) {

                      if ( p.flags && cNoReplace ) {
                         std::cerr << "cannot set parameter " << pname << " from cmd line " <<  std::endl;
                         throw(Configuration::badcmdline());               
                      }
                 
                      switch (params[pname].type) {
                      case cBoolean:
                         // boolean set to true 
                         p.value = "Y";
                         p.status = p.status & ParamFromCmdLine;
                         break;
                      case cConfig:
                         //  no action taken for Config type (already dealt with those in cmdLineConfigs() )
                         break;
                      default:
                         // set the value
                         p.value = std::string(optarg);

                         if ( p.flags && cNoReplace ) {
                            std::cerr << "cannot set parameter " << pname << " from cmd line " <<  std::endl;
                            throw(Configuration::badcmdline());               
                         }
                         p.status = p.status & ParamFromCmdLine;
                         break;
                      }    
                   
                      // store the altered parameter
                      params[pname] = p;   
                      
                      // no need to go through remaining parameters
                      break;  
                 }
            }
            break;     
       }
    }
    
    pcount = optind;
    if ( pcount > argc ) {
       pcount = -1;
    }
    
    takedownGetopts( go );
    
    status = 1;
    
    
    return pcount;

}

int Configuration::load(int argc, char * const argv[] )
{
     // iterator for the map of registered parameters
     std::map<std::string, struct param>::iterator piter;
     // index of first non-option argument
     int first;

     // First, get any config file redefinitions from the command line
     cmdLineConfigs( argc, argv );

     // now go through the config files, in order
     for ( int i=0; i<configs.size(); i++ ) {
         loadfile( configs[i], cfgnames[i], 0);
         status = 1;
     
     }

     // load options from the command lines
     first = loadCmdLine( argc, argv );     
     
     // now check all parameters to see if any requires ones are missing
     for ( piter = params.begin() ;
          piter != params.end();
          piter++ ) {
     
          // get the parameter name (i.e., the keyword)
          std::string pname = piter->first;
          
          // get the parameter itself
          struct param p = piter->second;
          
          // is this parameter mandatory? then it must be defined
          if ( ( p.flags & cMustBePresent ) && ( p.value == "" ) ) {
             std::cerr << "parameter " << pname << " must be set somewhere, but is missing " <<  std::endl;
             throw(Configuration::badcfgread());
          }
     
     }
     
     return first;
}


std::string Configuration::get(const std::string &name)
{
     struct param p;
     
     p = this->findParam(name);
     return p.value;
     
}


int Configuration::str2int(const std::string &value)
{
    std::istringstream *numstr; 
    int result;
    
    numstr = new std::istringstream(value.c_str());
    try {
       *numstr >> result;
    } catch (...) {
       throw (Configuration::badcfgformat());
    }
    delete numstr;
    
    return result;
}

float Configuration::str2float(const std::string &value)
{
    std::istringstream *numstr; 
    float result;
    
    numstr = new std::istringstream(value);
    try {
       *numstr >> result;
    } catch (...) {
       throw (Configuration::badcfgformat());
    }
    delete numstr;
    
    return result;
}

double Configuration::str2dbl(const std::string &value)
{
    std::istringstream *numstr; 
    double result;
    
    numstr = new std::istringstream(value);
    try {
       *numstr >> result;
    } catch (...) {
       throw (Configuration::badcfgformat());
    }
    delete numstr;
    
    return result;
}

bool Configuration::str2bool(const std::string &value)
{
    bool result;
    std::string cc;
    
    // extract the first character
    cc = value.substr(0,1);

    // and test it
    if ( cc == "Y" || cc == "y" || cc == "T" || cc == "t" ) {
       result = true;
    } else {
       if ( cc == "N" || cc == "n" || cc == "F" || cc == "f" ) {
          result = false;
       } else {
          throw(Configuration::badcfgformat());               
       }
    }
    return result;
}

void Configuration::fetchParam(const std::string &name, int &value)
{
     struct param p;

     p = this->findParam(name);

     if ( p.type == cInt ) {
        value = this->str2int( p.value );
     } else {
        std::cerr << "Expected integer type for parameter " << name << std::endl;
        throw(badparamtype());
     }
   
}


void Configuration::fetchParam(const std::string &name, float &value)
{
     struct param p;

     p = this->findParam(name);

     if ( p.type == cFloat || p.type == cDouble ) {
        value = static_cast<float>( this->str2dbl( p.value ) );
     } else {
        std::cerr << "Expected real type for parameter " << name << std::endl;
        throw(badparamtype());
     }
   
}
void Configuration::fetchParam(const std::string &name, double &value)
{
     struct param p;

     p = this->findParam(name);

     if ( p.type == cFloat || p.type == cDouble ) {
        value = static_cast<double>( this->str2dbl( p.value ) );
     } else {
        std::cerr << "Expected real type for parameter " << name << std::endl;
        throw(badparamtype());
     }
   
}

void Configuration::fetchParam(const std::string &name, bool &value)
{
     struct param p;

     p = this->findParam(name);

     if ( p.type == cBoolean ) {
        value = this->str2bool( p.value );
     } else {
        throw(badparamtype());
     }
   
}


void Configuration::fetchParam(const std::string &name, std::string &value)
{
    value = this->get(name);
   
}

void Configuration::help(const std::string intro, const std::string postscript) 
{
     // a parameter
     struct param *p; 
     struct param pm;    
     // iterator for the map of registered parameters
     //std::map<std::string, struct param>::iterator piter;
     
     std::cerr << intro << std::endl;

     std::cerr << "Parameters:" << std::endl;

     // for each parameter (in no particular order)
     //for ( piter = params.begin(); piter != params.end(); piter++ ) {
     //    // get the name
     //    std::string keyword = piter->first;
     //    // get the parameter
     //    p = &(piter->second);
     for ( int i=0; i < paramNames.size(); i++ ) {
         
         std::string keyword = paramNames[i];
         pm = params[keyword];
         p = &pm;
         
         std::cerr << "   --" << keyword;
         if ( p->shortopt != 0 ) {
            std::cerr << " , -" << p->shortopt;
         }
         std::cerr << " ";
         
         switch ( p->type ) {
         case cInt: std::cerr << "(Integer) ";
                    break;
         case cFloat: std::cerr << "(Float) ";
                    break;
         case cDouble: std::cerr << "(Double) ";
                    break;
         case cBoolean: std::cerr << "(Boolean) ";
                    break;
         case cString: std::cerr << "(String) ";
                    break;
         case cConfig: std::cerr << "(Configuration file)";
                    break;
         }

         std::cerr << ": ";

         if ( p->description != "" ) {
            std::cerr << p->description;
         }
         if ( p->value != "" ) {
            std::cerr << " = " << p->value;
         }
         
         std::cerr << std::endl;

     }  

     std::cerr << postscript << std::endl;

}

