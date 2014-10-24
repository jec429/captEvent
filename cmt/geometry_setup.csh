#! /bin/csh

# Download the calibration files.
${CAPTEVENTROOT}/scripts/captain-get-geometry

setenv CAPTAINGEOMETRY `find ${CAPTEVENTROOT}/${CAPTEVENTCONFIG} -name "*baseline*"`

setenv MINICAPTAINGEOMETRY `find ${CAPTEVENTROOT}/${CAPTEVENTCONFIG} -name "*mini-captain*"`

