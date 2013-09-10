#! /usr/bin/env python

import re
import math

#-------------------------------------------------------------------------------
def extractTimings(filename):
   """
   Extract timings from vdtPerfBenchmark files into a list of tuples done like:
   name - time - error
   """
   file = open(filename, "r")
   timings=[]
   for line in file:
      splittedLine=line.split()
      timings.append([splittedLine[1],float(splittedLine[3]),float(splittedLine[5])])
   return timings

#-------------------------------------------------------------------------------
def normaliseTimings(timings):
   """
   Normalise the timings according to the Identity funcitons
   """
   newTimings=[]
   offset=-1
   for name, time, err in timings:
      if "Identity" in name:
         offset=time
         offsetErr=err
         continue
      time -= offset
      err = math.sqrt( err*err + offsetErr*offsetErr )
      newTimings.append( [name,time,err] )
   return newTimings
#-------------------------------------------------------------------------------
def extractTitleTimings(filename,normalise):   
   timings=extractTimings(filename)
   finalTimings=[]
   title=filename.split("__")[0]
   if (normalise):
      finalTimings=normaliseTimings(timings)
   else:
      #finalTimings = filter(lambda nameTimeErr: "Identity" not in nameTimeErr[0] ,timings)
      finalTimings=timings
   return [title,finalTimings]

#-------------------------------------------------------------------------------
def createSummaryTable(TitledTimingsList):
   """
   Create a big summary table with name time error time error time error ...
   for the different files.
   timingsList is a list of tuples: name timings
   """
   firstTimings =TitledTimingsList[0][1]
   nlines=len(firstTimings)
   print "   Function Name",
   for title,timings in TitledTimingsList:
      print "%20s" %title,
   print
   for i in xrange(nlines):
      print "%16s" %firstTimings[i][0],
      first=True
      norm=1.
      speedupString=""
      for title,timings in TitledTimingsList:
         time = timings[i][1]
         err = timings[i][2]
         if first:
            norm=time
         else:
            dividend=time
            if dividend==0.: dividend =1.
            factor = norm/dividend
            speedupString = "%.1fX" %factor
         s="%.2f +- %.2f %s" %(time,err,speedupString)
         print "%20s" %s,
         first=False
      print


#-------------------------------------------------------------------------------
def timings2TimingsDict(timings):
   titledTimingsDict={}
   for name,time,err in timings:
      titledTimingsDict[name]=[time,err]
   return titledTimingsDict

#-------------------------------------------------------------------------------
def createVectTable(TitledTimingsList):
   """
   Create a big summary table with name time error of the scalar symbols and in the
   following time error time error for the different files for the vectorised symbols.
   """

   # Duplicate the first titledTimings: the 0th used for the scalar, the 1st for the vect
   TitledTimingsList = [TitledTimingsList[0]]+TitledTimingsList
   
   firstTimings =TitledTimingsList[0][1]
   nlines=len(firstTimings)
   print "   Function Name",
   vectSuffix=""
   for title,timings in TitledTimingsList:      
      print "%20s" %(title+vectSuffix),
      vectSuffix=" vect."
   print
   for i in xrange(nlines):
      funcName=firstTimings[i][0]
      if funcName.endswith('v'):continue
      
      print "%16s" %funcName,
      first=True
      norm=1.
      speedupString=""
      for title,timings in TitledTimingsList:
         if first:
            time = timings[i][1]
            err = timings[i][2]
            norm=time
            s="%.2f +- %.2f" %(time,err)
            print "%20s" %s,
            first=False
         else:
            timingsDict = timings2TimingsDict(timings)            
            time,err = timingsDict[funcName+"v"]
            err = timingsDict[funcName][1]
            dividend=time
            if dividend==0.: dividend =1.
            factor = norm/dividend
            speedupString = "%.1f X" %factor
            s="%.2f +- %.2f %s" %(time,err,speedupString)
            print "%20s" %s,
            first=False
      print

#-------------------------------------------------------------------------------
def createLibmVdtTable(TitledTimings):
   """
   Create a big summary table with name time error of the libm symbols and the corresponding vdt ones.
   """

   nlines=len(TitledTimings)
   title,timings = TitledTimings
   print "--------- %s Scalar Calls ----------" %title
   print "%16s %16s %16s" %("Function Name","Libm","VDT")
   timingsDict=timings2TimingsDict(timings) 
   for name,time,err in timings:
      if name.startswith("Fast_") or name.endswith("v"): continue
      
      libmString="%.2f +- %.2f" %(time,err)

      vdtName = "Fast_%s" %name
      if "Inverse" in vdtName:
         vdtName= vdtName.replace("Inverse","Inv")
      
      vdtTime = timingsDict[vdtName][0]
      vdtErr = timingsDict[vdtName][1]
      speedup = time / vdtTime
      speedupString = "%.1fX"%speedup
      vdtString="%.2f +- %.2f %s" %(vdtTime,vdtErr,speedupString)

      print "%16s %20s %20s" %(name,libmString,vdtString)
      
#-------------------------------------------------------------------------------
if __name__ == "__main__":
   from optparse import OptionParser

   parser = OptionParser(usage="usage: %prog perfFile1 [... perfFileN] options ")
   parser.add_option("-s", dest="summaryTable",
                     help="Create summary table",
                     default=False,
                     action="store_true")
                     
   parser.add_option("-l", dest="libmTable",
                     help="Create libm VS vdt timings table",
                     default=False,
                     action="store_true")

   parser.add_option("-v", dest="vectTable",
                     help="Create vector timings table",
                     default=False,
                     action="store_true")                  
                     
   parser.add_option("--no-normalise", dest="noNormalise",
                     help="Do not normalise to identity",
                     default=False,
                     action="store_true")                 
   
   (options, perffilenames) = parser.parse_args()

   # Create Titled timings
   titledTimingsList=[]
   for perffilename in perffilenames:   
      titledTimingsList.append(extractTitleTimings(perffilename,not options.noNormalise))

   if options.summaryTable:
      createSummaryTable(titledTimingsList)

   if options.vectTable:
      createVectTable(titledTimingsList)

   if options.libmTable:
      map (createLibmVdtTable, titledTimingsList)






      


      
   