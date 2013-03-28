#!/usr/bin/env python
import re
import optparse
import os
import sys

parser = optparse.OptionParser("usage: %prog [options] <files>");
parser.add_option("--names",action="store",
                  dest="names",type="string",default="oaEvent.names",
                  help="Set the list of namespace substitutions.")
(options,args) = parser.parse_args();

def ReplaceClassName(oldName, newName, source):
    front = r'(^|[^a-zA-Z0-9~:_])'
    back = r'([^a-zA-Z0-9_])'
    pattern = front + "(" + oldName + "|" + newName+ ")" + back
    result = re.sub(pattern,r'\1'+newName+r'\3',source)
    return result

def FixTHandle(source):
    pattern = r'(THandle<.*)\*>'
    result = re.sub(pattern,r'\1'+">",source)
    return result

#########################
# START THE REAL SCRIPT #
#########################

subs = []

if len(options.names) < 2:
    parser.print_help()
    print "Must provide file with list of name substitutions"
    sys.exit(1)

if len(args) < 1:
    parser.print_help()
    print "Must provide list of file names"
    sys.exit(1)
    
namesFile = open(options.names,"r")

for line in namesFile:
    if re.match("^#",line) != None: continue
    s = line.split()
    if len(s) == 2: subs.append(s)

for sourceName in args:
    sourceFile = open(sourceName,'r')
    resultName = sourceName + ".new"
    resultFile = open(resultName,'w')
    for line in sourceFile:
        # Skip preprocessor lines
        if re.match("^#",line) != None:
            resultFile.write(line)
            continue
        # Add the namespace based on the names file.
        for name in subs:
            line = ReplaceClassName(name[0],name[1],line)
        # Fix the syntax of the THandle lines that are replacing
        # TBorrowed lines.
        line = FixTHandle(line)
        resultFile.write(line)
    resultFile.close()
    sourceFile.close()
    status = os.system("diff -q " + sourceName + " " + resultName)
    if status == 0: os.unlink(resultName)
    
