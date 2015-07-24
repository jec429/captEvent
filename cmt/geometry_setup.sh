#! /bin/sh

# Download the calibration files.
${CAPTEVENTROOT}/scripts/captain-get-geometry

export CAPTAINGEOMETRY
CAPTAINGEOMETRY=`find ${CAPTEVENTROOT}/${CAPTEVENTCONFIG} -name "*baseline*" | sort -V | tail -1`

export MINICAPTAINGEOMETRY
MINICAPTAINGEOMETRY=`find ${CAPTEVENTROOT}/${CAPTEVENTCONFIG} -name "*mini-captain*" | sort -V | tail -1`

