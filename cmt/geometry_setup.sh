#! /bin/sh

# Download the calibration files.
${CAPTEVENTROOT}/scripts/captain-get-geometry

export CAPTAINGEOMETRY
CAPTAINGEOMETRY=`find ${CAPTEVENTROOT}/${CAPTEVENTCONFIG} -name "*baseline*"`

export MINICAPTAINGEOMETRY
MINICAPTAINGEOMETRY=`find ${CAPTEVENTROOT}/${CAPTEVENTCONFIG} -name "*mini-captain*"`

