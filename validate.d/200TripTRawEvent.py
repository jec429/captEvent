#! /usr/bin/env python
#
#  Test the data dump of a midas file.

import sys
import os
import os.path
import commands
import re

midasFile = "tript-banks-080801.mid.gz"
rootFile = '200TripTRawEvent.root'
dumpFile = '200TripTRawEvent.dump'

if not os.path.exists(midasFile): 
    print "MISSING INPUT"
    sys.exit(0)

if os.path.exists(rootFile): os.remove(rootFile)
    
# Get a dump of the file
os.system("dump-event.exe -a -m " + midasFile +
           " -o " + rootFile +
          "  | sed 's/(0x[0-9a-f]*)::/()::/' > " + dumpFile)
eventDump = commands.getoutput('cat ' + dumpFile)

# Make a simple way to check for a regular expression on a single line.
def Check(pattern):
    # Check if a pattern is found in the output.
    result = re.search(pattern,eventDump,re.M)
    if result == None:
        print "Pattern not found: ", pattern
        return False
    return True

# Look for re that should be in the file.
valid = True
if not Check('^ND::TND280Event.+event.00251'): valid = False
if not Check('^  ND::TND280RawEvent.+rawevent.00251'): valid = False
if not Check('^   ND::TMidasBank'): valid = False
if not Check('^Total Events Read: 9$'): valid = False

if not valid:
    print eventDump
    print "" 
    print "FAIL"
