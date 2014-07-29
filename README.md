mergePeaks_resolve
==================

A C++ parser to convert output of HOMER's mergePeaks into a table of registered peak intensities
Copyright 2014 Shraddha Pai
Please see LICENSE for details on usage restrictions.

Motivation and background
---------------------------
HOMER is a third-party software written by Chris Benner and others, which is routinely used for analysis of data from the ChIP-SEQ assay.
HOMER contains a utility, mergePeaks, which  "registers" peaks from multiple files part of the same experiment; for example, to find overlapping peaks for individual timepoints in a timecourse experiment. For details on mergePeaks see: http://homer.salk.edu/homer/ngs/mergePeaks.html
mergePeaks output prints a comma-separated list of peak identifiers from each file, without providing the average peak intensity for the registered peaks.

This utility, mergePeaks_resolve, takes the output from HOMER mergePeaks and returns a table with mean peak intensities from registered peaks.


INSTALLING mergePeaks_resolve
-----------------------------
This project currently provides a compiled binary for x86-64bit architecture.
To compile from source, you will need a GCC compiler and Boost libraries installed.
The makefile is in the project root directory.
Once you have the prerequsites, simply type

make 

in the project directory. That should be it.

Binary and instructions for compilation on other platforms welcome.

USING mergePeaks_resolve
-------------------------
Either create a symbolic link to the binary "mergePeaks_resolve" in the directory where you will use it,
or put it in a directory on the search path.

Examples
----------
see examples/example.sh


