#!/usr/bin/env python3
#
#  Catalogs the MERRA data fields that are available from the GDISC OPeNDAP server
#  and distills the information into a text catalog 
# 
#  Usage:
#     catalog_merra.pt > MERRA_Directory_Data.ctg
#

import sys
import http.client
#httplib.HTTPConnection.debuglevel = 1                            
import urllib.request, urllib.error, urllib.parse
import io
import gzip
import html.parser

import re



class merra_loc(object):
   "hold a location in the MERRA DAAC where a data variable may be found"
   def __init__(self, baseurl, collect, tag):
       # the start of the URL (this gets you into the MERRA 
       # section of the DAAC)
       self.baseurl = baseurl
       # the subdirectory under the URL
       # This string is coded according to what kinds of variables
       # may be found there
       self.collect = collect
       # (After the base url and the collection, there are additional
       #  directories for the year and the month, which the reader
       #  will need to add on the fly.)
       #  The "tag" is the part of the file name to be accessed,
       #  that omits the date info and the file extension. 
       self.tag = tag
       # The next members reflect information that is extracted
       # from the collection
       # Configuration: "A" = assimilation
       #                "F" = forecast
       #                "S" = simulation
       self.config = self.collect[1:2]
       # time average:  0 = instantaneous snapshot
       #                1 = 1-hour averages
       #                3 = 3-hour averages
       #                6 = 6-hour averages
       #               -1 = longer-term averaged data, not to be used for trajectories
       self.tavg = self.get_tavg()
       # base time:  0 = 00:00 UTC
       #            90 = 01:30 UTC
       self.tbase = self.get_tbase()
       # time spacing:  1 = 1-hour interval between snapshots
       #                3 = 3-hour interval between snapshots
       #                6 = 6-hour interval between snapshots
       #                0 = not applicable
       #               -1 = other
       self.tspace = self.get_tspace()
       # horizontal grid:  0 = native horizontal resolution (2/3 x 1/2)
       #                   1 = reduced resolution (1.25 x 1.25)
       #                   2 = reduced FV resolution (1.25 x 1)
       #                  -1 = unrecognized
       self.hgrid = self.get_hgrid()
       # vertical grid coordinate: 0 = 2D data (i.e., no vertical coord)
       #                           1 = native model levels
       #                           2 = Pressure levels
       #                           3 = model layer edges
       #                  -1 = unrecognized
       self.vcoord = self.get_vcoord()
   def get_tspace(self):
       timedesc = self.collect[2:3]
       freq     = self.collect[3:4]
       if timedesc == 'T' or timedesc == 'I':
          if freq == '1':
             tspc = 1
          elif freq == '3':
             tspc = 3
          elif freq == '6':
             tspc = 6
          else:
             tspc = -1
       else:
          # constant ("C") 
          tspc = 0
       return tspc       
   def get_tavg(self):
       timedesc = self.collect[2:3]
       freq     = self.collect[3:4]
       if timedesc == 'T':
          if freq == '1':
             tavg = 1
          elif freq == '3':
             tavg = 3
          elif freq == '6':
             tavg = 6
          else:
             tavg = -1
       else:
          # constant ("C") and instantanteous ("I")
          tavg = 0
       return tavg       
   def get_tbase(self):
       timedesc = self.collect[2:3]
       freq     = self.collect[3:4]
       if timedesc == 'T':
          # averaged
          if freq == '1':
             tbase = 30
          elif freq == '3':
             tbase = 90
          elif freq == '6':
             tbase = 180
          else:
             tbase = 0
       else:
          # constant ("C") and instantanteous ("I")
          tbase = 0
       return tbase       
   def get_hgrid(self):
       shres     = self.collect[4:5]
       if shres == 'N':
          xhres = 0
       elif shres == 'C':
          xhres = 1
       elif shres == 'F':
          xhres = 2
       else:
          xhres = -1
       return xhres   
   def get_vcoord(self):
       svloc     = self.collect[5:6]
       if svloc == 'X':
          vcoord = 0
       elif svloc == 'V':
          vcoord = 1    
       elif svloc == 'P':
          vcoord = 2    
       elif svloc == 'E':
          vcoord = 3
       else:
          vcoord = -1 
       return vcoord
   # Note: this method serializes the location information for inclusion
   # in an argument to a C++ constructor call.
   def output(self):
       result = self.baseurl + ',' + self.collect + ',' + self.tag
       result = result + ',' + str(self.hgrid) + ',' + str(self.vcoord) + ',' + str(self.tspace) + ',' + str(self.tavg) + ',' + str(self.tbase)
       return result

def lockey( loc1 ):
    # vert coord priority order: P, V, X, E, unk:
    # so remap [ 2, 1, 0, 3, -1 ] 
    #       to [ 4, 3, 2, 1,  0 ] 
    v1 = loc1.vcoord
    if v1 == 2:
       v1 = 4
    elif v1 == 1:
       v1 = 3
    elif v1 == 0:
       v1 = 2
    elif v1 == 3:
       v1 = 1
    else:
       v1 = 0         

    # horiz grid priority order: N, C, F, unk :
    #                 so remap [ 0, 1, 2, -1 ]
    #                     to   [ 3, 2, 1,  0 ]
    h1 = loc1.hgrid
    if h1 == 0:
       h1 = 3
    elif h1 == 1:
       h1 = 2
    elif h1 == 2:
       h1 = 1
    else:
       h1 = 0          

    # time-avg priority order: inst, 1-r, 3-hr, 6-hr, other ]
    #                so remap [ 0, 1, 3, 6, -1 ]
    #                     to  [ 4, 3, 2, 1,  0 ]
    a1 = loc1.tavg
    if a1 == 0:
       a1 = 4
    elif a1 == 1:
       a1 = 3
    elif a1 == 3:
       a1 = 2
    elif a1 == 6:
       a1 = 1
    else:
       a1 = 0            

    result = (v1*100 + h1)*100 + a1     
    return result      

class merra_var(object):
   "hold information about a MERRA variable"
   def __init__(self):
      # The MERRA-native name of the physical quantity, without the "_EOS_Data_Fields" string
      self.name=''
      # The descriptive text that is associated with this physical quantity
      self.longname='(No longname)'
      # The (quasi-) CF-conforming name of this physical quantity
      self.stdname='(No stdname)'
      # The units associated with this physical quantity
      self.units = ''
      # the number of dimensions of this physical quantity
      self.ndims = 0
      # A list of the dimensions of this physical quantity
      self.dims = []
      # A list of the locations in which variations of this physical quantity may be found
      self.locs = []
   # This method serializes the variable object in such a way
   # that it can be used to construct a C++ object, properly initialized with
   # the object's attributes.
   def output(self):
      loclist = ''
      self.locs.sort( key=lockey, reverse=True )
      for loc in self.locs:
         loclist = loclist + '|' + loc.output()
      loclist = loclist.strip('|') 
      qname = self.name # .replace("_EOSGRID_Data_Fields","") 
      print("@".join( [ qname , str(self.ndims) , self.stdname , self.longname , self.units, loclist ]))

class merra_dim(object):
   "hold information about a dimension of a MERRA variable"
   def __init__(self):
      # The MERRA-native name of the physical quantity
      self.name=''
      # The descriptive text that is associated with this physical quantity
      self.longname='(no longname)'
      # The (quasi-) CF-conforming name of this physical quantity
      self.stdname='(no stdname)'
      # The units associated with this physical quantity
      self.units = ''
   def dump(self):
      print("DIM: name=",self.name)
      print("     long name=",self.longname)
      print("     std name=",self.stdname)
      print("     units = ",self.units)


class VarCompatException(Exception):
   "exception for incompatible variable definitions"
   def __init__(self, *args):
      Exception.__init(self,*args)
      self.wrapped_exc = sys.exc_info()

class merra_inventory(object):
   "an inventory of the set of MERRA variables that are found in a single location"
   def __init__(self,baseurl,collect,url):
      # extract the tag from the full url
      tagname = ''
      for part in url.split('/')[-1].split('.')[0:-3]:
         # print "[[[",part,"]]]"
         tagname = tagname + "." + part
      # This is the location being inventoried
      self.loc = merra_loc(baseurl, collect, tagname.strip('.'))
      # This is the list of MERRA variables
      self.varlist = dict()
      # This is hte list of MERRA variable dimensions
      self.dimlist = dict()
   # This method parses the *DAS page for this location on the OpenDAP server
   def scan_das(self, das):
      #print "<<<<<<<<<<<<<<<<<  DAS <<<<<<<<<<<<<<<<<<<<<<<"
      metadata_names = [ "StructMetadata", "ArchiveMetadata", "CoreMetadata", "HDF_GLOBAL" ]
      state = 0
      curname = ''
      curlong = ''
      curstd = ''
      curunits = ''
      curtype = 1
      nestlevel = 0;
      for line in das:
         #print state,nestlevel," line=",line
         if state == 0:
            # State 0: nothing recognizavble encountered yet
            if re.match(r'^ *Attributes *{', line):
               # get started
               state=1
         elif state == 1:
            # Looking for a major section header
            namestuff = re.match(r'^ *([A-Z_a-z0-9]+) *{', line)
            if namestuff != None:
               # we start a variable definition (or one of several metadata sections)
               nestlevel = nestlevel + 1
               myname = namestuff.group(1).strip()
               if metadata_names.count(myname) == 0 :  
                  # We have a major section header, and it is NOT
                  # one of the names in metadata_names.  So
                  # It must be a variable or dimension 
                  curname = myname
                  #print "//=========== opening ",curname             
                  curtype = 1                           
                  curlong = ''  
                  curstd = ''                        
                  curunits = ''
                  state = 2                             
               else:        
                  # We got a major section headerl, but it
                  # is one of the names in the metadata_names list,                            
                  # so it is a metadata description that we skip past    
                  state = 3  
            # close braces on same line?                                
            if re.match(r'^ *} *$', line):
               state = 0 
               nestlevel = nestlevel - 1
         elif state == 2:    
            # data variable description
            if re.match(r'^ *} *$', line):
               # got some kind of closed brace
               if nestlevel == 1:            
                  # we are at the end of the variable description
                  #print "closing ",curname, "(",curtype,") "+curlong   
                  if curtype == 0:  
                     # This is a variable, not a dimension 
                                                       
                     if curname in self.varlist: 
                        # The variable already exists. Grab it.                 
                        v = self.varlist[curname]                       
                        if v.longname != '' and curlong != '' and v.longname != curlong:
                           raise VarCompatException("Quantity '"+curname+"': long name mismatch: <<"+v.longname+'>> vs <<'+curlong+'>>')
                        if v.stdname != '' and curstd != '' and v.stdname != curstd:
                           raise VarCompatException("Quantity '"+curname+"': std name mismatch: <<"+v.stdname+'>> vs <<'+curstd+'>>')
                        if v.units != '' and curunits != '' and v.units != curunits:
                           raise VarCompatException("Quantity '"+curname+"': units mismatch: <<"+v.units+'>> vs <<'+curunits+'>>')
                        if v.longname == "":
                           v.lonname = curlong
                        if v.stdname == "":
                           v.stdname = curstd
                        if v.units == "":
                            v.units = curunits
                     else: 
                        # New variable                                             
                        v = merra_var()                                 
                        v.name = curname 
                        v.longname = curlong 
                        v.stdname = curstd                           
                        v.units = curunits                          
                     #print "::: closing var ",curname , " (",v.longname,")"                                 
                     self.varlist[curname] = v                          
                  elif curtype == 1:  
                     # this is a dimension, not a variable                                  
                     if curname in self.dimlist:                  
                        d = self.dimlist[curname]                       
                        if d.longname != '' and curlong != '' and d.longname != curlong:
                           raise VarCompatException("Quantity '"+curname+"': long name mismatch: <<"+d.longname+'>> vs <<'+curlong+'>>')
                        if d.stdname != '' and curstd != '' and d.stdname != curstd:
                           raise VarCompatException("Quantity '"+curname+"': std name mismatch: <<"+d.stdname+'>> vs <<'+curstd+'>>')
                        if d.units != '' and curunits != '' and d.units != curunits:
                           raise VarCompatException("Quantity '"+curname+"': units mismatch: <<"+d.units+'>> vs <<'+curunits+'>>')
                        if d.longname == "" :
                           d.lonname = curlong
                        if d.stdname == "":
                            d.stdname = curstd
                        if d.units == "":
                            d.units = curunits
                     else:                                              
                        d = merra_dim()                                 
                        d.name = curname                                
                        d.longname = curlong                            
                        d.stdname = curstd 
                        d.units = curunits                          
                     #print "::: closing dim ",curname," (",d.longname,")"                                
                     self.dimlist[curname] = d                          
                  # on to the next variable
                  state = 1
               # we popped up one level of nesting
               nestlevel = nestlevel - 1  
            elif re.match(r'{" *;', line):
               # we nested down one more level
               nestlevel = nestlevel + 1;
            elif re.match(r'^ *String *long_name *"(.*)" *;', line):
               # encountered a long name
               curlong = re.match(r'^ *String *long_name *"(.*)" *;', line).group(1)
               #print "//=========== long name ",curlong             
            elif re.match(r'^ *String *standard_name *"(.*)" *;', line):
               # encountered a standard name
               curstd = re.match(r'^ *String *standard_name *"(.*)" *;', line).group(1)
               #print "//=========== std name ",curstd             
            elif re.match(r'^ *String *units *"(.*)" *;', line):
               # encountered units
               curunits = re.match(r'^ *String *units *"(.*)" *;', line).group(1)
            elif re.match(r'^ *String *coordinates .*$', line):
               # Encountering this string means that the
               # current thing being scanned is a variable, not a dimension
               curtype = 0
               #print "thing ",curname," is a VAR, not DIM!"
         elif state == 3:    
            # metadata description to skip past
            if re.match(r'^ *} *$', line):
               # popping up one level
               nestlevel = nestlevel - 1 
               if nestlevel == 0:
                  # popped up to the top
                  state = 1
            elif re.match(r'^.*{ *$', line):
               # nesting down another level
               nestlevel = nestlevel + 1
   # This method parses the *DDS page for this location on the OpenDAP server
   def scan_dds(self, dds):
      #print ">>>>>>>>>>>>>>>>> DDS >>>>>>>>>>>>>>>>>>."
      for line in dds:
         #print state," line=",line
         if re.match(r'^ *Float.. [A-Za-z][A-Za-z0-9_]*\[.*\]; *$', line):
            # OK, we seem to have found a variable description
            namestuff = re.match(r'^ *(Float32|Float64)  *([A-Za-z][A-Z_a-z0-9]*)(.*); *$', line)
            if namestuff != None:
               # print "matched ",namestuff.group(0)
               
               # extract the variable name
               curname = namestuff.group(2).strip()
               # now extract the dimensions of this variable from the rest of the line
               curdims = []
               dimx = namestuff.group(3).strip()
               while re.match(r'^\[ *([A-Za-z0-9_]+) *= *\d+ *\](.*)$',dimx):
                   mtch = re.match(r'^\[ *([A-Za-z0-9_]+) *= *\d+ *\](.*)$',dimx)
                   if mtch != None:
                      dm = mtch.group(1)
                      # print "    found dim ",dm," for ",curname
                      curdims.append(dm)
                      dimx = mtch.group(2)
               # If this variable has one dimension whose name is the same
               # as this variable, then this variable is a dimension
               isdim = 0
               if len(curdims) == 1 and dm == curname:
                  isdim = 1

               #print "got ",curname, ' with ',curdims
               if isdim:
                  # It's a dimension
                  if curname in self.dimlist:
                     # already defined elsewhere (either here in the DDS or  in the DAS
                     d = self.dimlist[curname]
                  else:   
                     # not yet defined.  Create a new one
                     d = merra_dim()
                     d.name = curname
                     self.dimlist[curname] = d
               else:
                  # It's a variable
                  if curname in self.varlist:
                     # We already have this defined
                     v = self.varlist[curname]
                  else:   
                     # New one. create it
                     v = merra_var()
                     v.name = curname
                  
                  curndims = len(curdims)
                  if v.ndims > 0 and curndims > 0 and v.ndims != curndims:
                     raise VarCompatException("Quantity '"+curname+"': ndims mismatch: <<"+str(v.ndims)+'>> vs <<'+curndims+'>>')
                  v.dims = curdims
                  v.ndims = curndims
                  self.varlist[curname] = v
   # for debugging
   def dump(self):
      name = self.loc.baseurl + ' / ' + self.loc.collect + ' / ' + self.loc.tag
      print("Name: ",name)
      print("Dimensions:",str(len(list(self.dimlist.keys()))))
      for key in list(self.dimlist.keys()):
          d = self.dimlist[key]
          print("    "+d.name+" ("+d.longname+")")
      print("Variables:",str(len(list(self.varlist.keys()))))
      for key in list(self.varlist.keys()):
          v = self.varlist[key]
          print("    "+v.name+" ("+v.longname+")",v.dims)
      


               

def grabweb(url):
   "Grabs a web page for later screen-scraping"
   request = urllib.request.Request(url)
   request.add_header('User-Agent','catalog_merra.py/0.1 Contact Leslie.R.Lait@nasa.gov')
   request.add_header('Accept-encoding', 'gzip')        
   opener = urllib.request.build_opener()                                  
   webhandle = opener.open(request)
   if hasattr(webhandle, 'headers'):
      if webhandle.headers.get('content-encoding', '') == 'gzip':                                 
         compresseddata = webhandle.read()    
         compressedstream = io.StringIO(compresseddata)   
         gzipper = gzip.GzipFile(fileobj=compressedstream)
         #print "gzipped text"
         data = gzipper.read()
      else:
         data = webhandle.read()
      #print data
   webhandle.close()

   data = str( data, 'utf-8' )

   return data


class merra_cat_extract(html.parser.HTMLParser):
   "extracts MERRA collections from a web page" 

   def __init__(self):
      html.parser.HTMLParser.__init__(self)
      self.datarefs = []

   def handle_starttag(self, tag, attributes):
      if tag != 'a': return
      for name, value in attributes:
          if name == 'href':
             href = value
             if re.match(r'MA.*/contents.html$', href):
                #print "href=",href
                tag = href.replace('/contents.html','')
                if tag not in self.datarefs:
                   self.datarefs.append(tag)
   

class merra_fdate_extract(html.parser.HTMLParser):
   "extracts the DAS file reference from a MERRA file listing" 

   def __init__(self):
      html.parser.HTMLParser.__init__(self)
      self.datarefs = []

   def handle_starttag(self, tag, attributes):
      if tag != 'a': return
      for name, value in attributes:
          if name == 'href':
             href = value
             if re.match(r'^MERRA.*\.19900715.hdf.das$', href):
                #print "href=",href
                if href not in self.datarefs:
                   self.datarefs.append(href)
   


##########################################################################################################33


# MERRA data may be ofund at these basic URLs
#urlset = [ "http://goldsmr1.sci.gsfc.nasa.gov/opendap/MERRA/", "http://goldsmr2.sci.gsfc.nasa.gov/opendap/MERRA/",  "http://goldsmr3.sci.gsfc.nasa.gov/opendap/MERRA/" ]
#urlset = [ "https://goldsmr1.gesdisc.eosdis.nasa.gov/opendap/MERRA/", "https://goldsmr2.gesdisc.eosdis.nasa.gov/opendap/MERRA/",  "https://goldsmr3.gesdisc.eosdis.nasa.gov/opendap/MERRA/" ]
urlset = [ "https://goldsmr3.gesdisc.eosdis.nasa.gov/opendap/MERRA/", "https://goldsmr2.gesdisc.eosdis.nasa.gov/opendap/MERRA/",  "https://goldsmr1.gesdisc.eosdis.nasa.gov/opendap/MERRA/" ]

# put everything that we find here
stuff = []

# examine what lies beneath each base url
for baseurl in urlset:
    #print baseurl
    
    # read the front page
    catpage = grabweb(baseurl)
    # extract the collections from that page
    catparser = merra_cat_extract()
    catparser.feed(catpage)
    catparser.close()

    # now that we have a list of collections
    # Examine each one
    for collect in catparser.datarefs:
        #print collect
        
        # Open up the web page for this collection
        # for July 1990.
        files_url = baseurl + collect + '/1990/07/'
        filespage = grabweb(files_url)
        # extract the DAS file reference from the
        # file listing web page, suing just
        # the entry for July 15, 1990.
        filelistparser = merra_fdate_extract()
        filelistparser.feed(filespage)
        filelistparser.close()
        
        # for each DAS file reference
        for href in filelistparser.datarefs:
            #print url
            daturl = files_url + href
            #print "**** ", daturl
            # Read the web page
            data = grabweb(daturl).split("\n")
            # Inventory the variable and dimensions in that file
            invitem = merra_inventory(baseurl,collect,daturl)
            invitem.scan_das(data)
            # switchover to the DDS file reference
            daturl2 = re.sub('\.das', '.dds', daturl)
            data = grabweb(daturl2).split("\n")
            # and scan that to get dimensional information
            invitem.scan_dds(data)
            
            # Add the inventory to the collection of all information
            stuff.append(invitem)

# At this point, "stuff" is a list of all the inventories
# covering all the MERRA hdf files we found for 1990-07-15.
# Each inventory has the list of variables and dimensions 
# contained i that file.


# Make the set of all MERRA variables that 
# we might be interested in
vlist = dict()
# Go through each file's inventory
for item in stuff:
   #item.output()
   # for each variable in the inventory
   for varname in list(item.varlist.keys()):
   
       # get the variable
       v = item.varlist[varname]
       
       # the variable's location must have a legal and proper
       # time average, horiz grid, and vert coord
       if (item.loc.hgrid != -1) and (item.loc.vcoord != -1) and (item.loc.tavg != -1): 
   
          # if we do not already have this variable
          # then add it to our list
          if varname not in vlist:
             vlist[varname] = v
       
          # add this file's location to this variable   
          vlist[varname].locs.append( item.loc )


# Now output all of the variable information
varlist = list(vlist.keys())
varlist.sort()
for varname in varlist:
    v = vlist[varname]
   
    # strip off the "_EOSGRID_Data_Fields" part
    quantname = varname.replace("_EOSGRID_Data_Fields","")
    # output the variable definition
    v.output()
