
#ifndef GIGATRAJ_FILEPATH_H
#define GIGATRAJ_FILEPATH_H

#include <string>
#include <vector>

#include "gigatraj/gigatraj.hh"

// assume Unix (or Unix-flavored) unless otherwise specified
#ifndef OSTYPE
#define OSTYPE UNIX
#endif

namespace gigatraj {

/// type FilePathFlag holds flag values to indicate things allowed in a legal path
typedef int FilePathFlag;

/*!
\ingroup misc
\brief FilePath provides a way of manipulating file paths in an operating system. 

The FilePath class provides a standard interface for dealing with file path names
by hiding operating-system-dependent conventions.  It does not implement
any actual file functions; it merely lets you manipulate 
the file names, isolating the user from having to deal with path separator
characters, character case issues, and path component order.

We assume that a file path is made up of a series of components, each of which is
a string that consists of legal characters in that operating system's file naming
conventions.  The components are separated by a separator character.  The components
are arranged in order, from uppermost to lowermost.  For example, in a Unix
system, a full path name might be "/alpha/beta/gamma/delta".  The separator would
be "/", the uppermost component would be "", and the lowermost would be "delta".

*/

class FilePath {


   public:
   
    /// Error: illegal file name component
    class badFileName {};
    
    /// Error: could not make directory
    class badMkdirFailed {};
    
    
    /// basic constructor
    /*! This is the basic constructor for the FilePath class.
    */
    FilePath();
    
    /// constructor that specifies the top-level part of the path
    /*!  This version of the constructor specifies the top-level ocmponent
         of the file path.  
         The top-level component of a file path is handled a little 
         differently from the other path components:
         in Unix, the top-level components determine whether
         the path is relative or absolute.  
         We can use this form of the constructor to specify this, for 
         improved readability; the default for Unix is the "."
         current directory (i.e., a relative file path).
         
         \param top the top-level component
    */     
    FilePath( const std::string top );
    
    /// destructor
    /*! This is the destructor for the FilePath class.
    */
    ~FilePath();
    
    /// copy constructor
    /*! This is the copy constructor for FilePath, used to
        generate a duplicate object. 
    */     
    FilePath(const FilePath& src);
    
    /// assignment operator
    /*! This operator overlay permits one FilePath object to be assigned
        the contents of another.
    */    
    FilePath& operator=(const FilePath& src);
    
    /// equality operator
    /*! This operator overlay tests for equality of two FilePath objects
        by comparing their contents.
    */
    bool operator==(const FilePath& two) const;

    /// inequality operator
    /*! This operator overlay tests for inequality of two FilePath objects
        by comparing their contents.
    */
    bool operator!=(const FilePath& two) const;

    /// returns a full file path name
    /*! This function returns the full path name of the file, suitable for ]
        passing to an \c open() function.
        
        \return a string containing the full path name
    */    
    std::string fullpath() const;
    
    /// returns the directory portion of the file path
    /*! This function extracts the directory portion of a file path.
    
    
         \return a string containing the directory path name
         \param n the number of directory components to extract from the 
                  top of the file system hierarchy.  n=0 causes
                  all directory components (except for the base file name)
                  to be returned.  n=5 returns the uppermost five components
                  from the hierarchy, including the topmost component.  
                  n=-5 returns the lowermost five components, not including
                  the base file name.
    */
    std::string directory( int n = 0) const;
    
    // returns the base name portion of a file path
    /*! This function extracts the base file name (lowermost component 
        of the filesystem hierarchy)
        
        \return a string containing the base path name 
    */
    std::string basename() const;
    
    /// set the top-level component of the file path
    /*! This function sets the top-level, uppermost component of the hierarchy.
    
        \param top the name of the top-level component
    */
    void setTop( const std::string top );    


    /// appends a new component to the file path
    /*! This function appends a new component to the file path.
    
        \param item a string containing a single component to be
                    added to the lowermost part of the hierarchy.
                    If the string contains separator characters, then it
                    is split into components, each of which is appended to the path.
    */
    void append( const std::string item );

    /// append a file path to this file path
    /*! This function appends a new (partial) hierarchical file path to this one.  
        (This typiocally makes sense only if the second path is relative.)
         
         \param item the hierarchy to be appended.  Any top-level component
                of \p item is ignored.
    */
    void append( const FilePath& item );
    
    /// prepend a new component to the hierarchy
    /*! This function prepends a new component to the file path
    
        \param item a string containing a single component to be
                    added to the uppermost part of the hierarchy.
                    Whether this replaces the current object's top-level
                    component is operating-system dependent.
                    
    */
    void prepend( const std::string item );

    /// prepend a file path to this file path
    /*! This function prepends a new (partial) hierarchical file path to this one.  
    
         \param item the hierarchy to be prepended.  Its top-level component
                becomes the top-level components of the current object.
                If the string contains separator characters, then it
                is split into components, each of which is prepended to the path.
    */
    void prepend( const FilePath& item );

   
    /// clears the path's contents
    /*! This function emptys the FilkePath object of all file path components.
    */
    void clear();
    
    /// returns the number of file path components
    /*! This function returns the number of components in the path, not counting the
        top-level component ("/" or "./" in Unix)
    */
    int count() const;

    /// searches for a file path component that matches a given string
    /* This function returns the index of the first component of this FilePath
       that matches a given string.
        
        \return the index of the matching file path component. Returns -1 if the
                search string is not found.
        \param srch the string which is sought
        \param start the component to start with (0 = uppermost)
               If \p start is larger than the number of components, -1 is returned.    
    */
    int find( std::string srch, int start=0 );
    
    /// searches for a file path component that matches a given FilePath oject
    /* This function returns the first component of a sequence in this FilePath
       that matches a given FilePath. top-level components are not compared.

        \return the index of the matching file path component. Returns -1 if the
                search FilePath is not found.
        \param srch the FilePath which is sought
        \param start the component to start with (0 = uppermost)
               If \p start is larger than the number of components, -1 is returned.    
    */
    int find( FilePath& srch, int start=0 );

    /// makes a directory
    /* This function creates a directory in the file system from the contents 
       of this FilePath.  
       If parent directories need to be created as well, they are.
       If the directory already exists, it is treated as a success.
       If the creation fails for any other reason, this method
       throws the badMkdirFailed exception.
    */
    void makedir();

    /// displays the contents of this FilePath
    /*! This function prints the FilePAth contents to stderr.
    */
    void dump(const std::string& msg ) const;
    
   protected:
   
      /// the file path separator character (which may in principle be multiple characters)
      static const std::string sep;

      ///  SepOK = Separators are OK in the path
      static const FilePathFlag SepOK = 1 << 0;

      /// The uppermost component in the file path hierarchy
      std::string topmost;
      
      /// the lowermost components in the file path hierarchy
      std::vector<std::string> parts;
      
      /// checks if a FilePath's contents are well-formed.  
      void checkLegal( const std::string str, FilePathFlag flags=0 ) const;
    
};

};

#endif




/******************************************************************************* 
***  Written by: 
***     L. R. Lait (NASA Ames Research Center, Code SG) 
***     Code 614 
***     NASA Goddard Space Flight Center 
***     Greenbelt, MD 20771 
*** 
***  Copyright (c) 2023 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.  All Rights Reserved. 
*** 
*** Disclaimer:
*** No Warranty: THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE. THIS AGREEMENT DOES NOT, IN ANY MANNER, CONSTITUTE AN ENDORSEMENT BY GOVERNMENT AGENCY OR ANY PRIOR RECIPIENT OF ANY RESULTS, RESULTING DESIGNS, HARDWARE, SOFTWARE PRODUCTS OR ANY OTHER APPLICATIONS RESULTING FROM USE OF THE SUBJECT SOFTWARE.  FURTHER, GOVERNMENT AGENCY DISCLAIMS ALL WARRANTIES AND LIABILITIES REGARDING THIRD-PARTY SOFTWARE, IF PRESENT IN THE ORIGINAL SOFTWARE, AND DISTRIBUTES IT "AS IS." 
*** Waiver and Indemnity:  RECIPIENT AGREES TO WAIVE ANY AND ALL CLAIMS AGAINST THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT.  IF RECIPIENT'S USE OF THE SUBJECT SOFTWARE RESULTS IN ANY LIABILITIES, DEMANDS, DAMAGES, EXPENSES OR LOSSES ARISING FROM SUCH USE, INCLUDING ANY DAMAGES FROM PRODUCTS BASED ON, OR RESULTING FROM, RECIPIENT'S USE OF THE SUBJECT SOFTWARE, RECIPIENT SHALL INDEMNIFY AND HOLD HARMLESS THE UNITED STATES GOVERNMENT, ITS CONTRACTORS AND SUBCONTRACTORS, AS WELL AS ANY PRIOR RECIPIENT, TO THE EXTENT PERMITTED BY LAW.  RECIPIENT'S SOLE REMEDY FOR ANY SUCH MATTER SHALL BE THE IMMEDIATE, UNILATERAL TERMINATION OF THIS AGREEMENT. 
***  (Please see the NOSA_19110.pdf file for more information.) 
*** 
********************************************************************************/
