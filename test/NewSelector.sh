#!/bin/sh

usage () {
    echo "Usage: ./NewSelector.sh prefix [template]"
    echo ""
    echo "Example: ./NewSelector.sh working"
    echo "  will produce  workingSelector.h workingSelector.C and runWorking.C"
}

NEW=$1;
shift;

if [ "x${NEW}" == "x" ]; then
    usage
    echo ""
    echo "ERROR: Must provide a prefix name name for the new selector"
    exit 1
fi

ORIG=template
if [ "x${1}" != "x" ]; then
    ORIG=$1
    shift;
fi
UPCASEORIG=`echo ${ORIG} | sed 's/^\\(.\\)/\\U\\1/'`

UPCASENEW=`echo ${NEW} | sed 's/^\\(.\\)/\\U\\1/'`

echo ${UPCASEORIG}
echo ${UPCASENEW}

cat ${ORIG}Selector.h | \
    sed s/${ORIG}/${NEW}/g | \
    sed s/${UPCASEORIG}/${UPCASENEW}/g \
    > ${NEW}Selector.h
cat ${ORIG}Selector.C | \
    sed s/${ORIG}/${NEW}/g | \
    sed s/${UPCASEORIG}/${UPCASENEW}/g \
    > ${NEW}Selector.C
cat run${UPCASEORIG}.C | \
    sed s/${ORIG}/${NEW}/g | \
    sed s/${UPCASEORIG}/${UPCASENEW}/g \
    > run${UPCASENEW}.C 

