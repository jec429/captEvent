#! /bin/bash

inputFile=200TripTRawEvent.root
inputDump=200TripTRawEvent.dump
outputDump=210TripTRawEvent.dump

if [ ! -f ${inputDump} ]; then
    echo MISSING INPUT
    exit 0
fi

if [ ! -f ${inputFile} ]; then
    echo MISSING INPUT
    exit 0
fi

# Clean up the dump of the midas file (previously generated)
grep -v % ${inputDump} \
    | grep -v ERROR \
    | grep -v SEVERE \
    | head -n -5 \
    > tempFileInput.dump

# Generate a dump of the root file
dump-event.exe -a ${inputFile} \
    | sed 's/(0x[0-9a-f]*)::/()::/' > ${outputDump}

# Clean up the dump of the root file
grep -v % ${outputDump} \
    | grep -v ERROR \
    | grep -v SEVERE \
    > tempFileOutput.dump

# Compare the dump of the midas file (previously generated) with the
# dump of the root file.
diff tempFileInput.dump tempFileOutput.dump

#Check the output dump file size.
wc -l tempFileOutput.dump | grep 105

rm tempFileInput.dump
rm tempFileOutput.dump
