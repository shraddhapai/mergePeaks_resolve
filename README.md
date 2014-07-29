mergePeaks_resolve
==================

A multi-threaded C++ parser to convert output of HOMER's mergePeaks into a table of registered peak intensities
Copyright 2014 Shraddha Pai
Please see LICENSE for details on usage restrictions.

Motivation and background
---------------------------
HOMER is a third-party software written by Chris Benner and others, which is routinely used for analysis of data from the ChIP-SEQ assay.
HOMER contains a utility, mergePeaks, which  "registers" peaks from multiple files part of the same experiment; for example, to find overlapping peaks for individual timepoints in a timecourse experiment. For details on mergePeaks see: http://homer.salk.edu/homer/ngs/mergePeaks.html
mergePeaks output prints a comma-separated list of peak identifiers from each file, without providing the average peak intensity for the registered peaks. Note the peak identifiers in the last three columns of this sample output:

< begin snippet of mergePeaks output >
#name (cmd = mergePeaks -d 120 peak1.bed peak2.bed peak3.bed -prefix test/test -matrix test/test -venn test/test_venn.txt -code)        chr     start   end     strand  Stat   
 Parent files    Total subpeaks  peak1.bed       peak2.bed       peak3.bed
Merged-chr1-3114752-4   chr1    3114586 3114893 -       0.000000        111     4       1634_1724_913,810_1010_922      1116_272_2024   140_551_92
Merged-chr1-3202645-16  chr1    3202344 3202964 -       0.000000        111     16      25_1175_360,1815_963_1560,1612_1975_1163,467_799_1909   283_1251_395,1889_390_1013,677_2042_101,2274_1204_1382,1419_1238_1589,2206_289_1078     1638_250_252,66_1726_852,2005_809_537,1638_742_979,481_929_935,1820_1929_1765
Merged-chr1-3255617-8   chr1    3255462 3255793 -       0.000000        111     8       127_278_720,634_872_1961,65_143_562     1655_429_485,2338_72_910        509_424_366,393_914_211,1265_1446_1087 
Merged-chr1-3242006-13  chr1    3241809 3242278 +       0.000000        111     13      1270_698_1031,1912_1981_39,2119_1586_399,641_328_1403,1916_545_1624,1724_1902_434,183_1355_1416 2306_1654_21,1468_33_1608,800_1118_1668,143_1785_1893,2053_1425_894     432_1375_945
Merged-chr1-3076578-6   chr1    3076463 3076720 +       0.000000        111     6       1511_361_1551   857_1180_997,189_1795_1384,2260_956_342 462_2026_139,1177_235_888
Merged-chr1-3167519-33  chr1    3167073 3168007 -       0.000000        111     33      350_1252_534,2068_1182_748,2084_1324_1160,1920_1354_523,1779_442_995,1653_380_408,1102_1616_240,535_323_460,2243_248_673,572_1606_988   1515_725_1918,1798_1831_886,1690_697_1865,182_72_2009,327_674_1399,1154_864_1932,1881_1066_1491,521_200_1489,1125_1648_717,1551_1665_1236,1685_943_1106,1957_2028_1082,229_1809_1592    1758_613_276,927_1193_1492,420_1696_1053,1603_166_1782,1175_1879_1295,815_295_1527,147_1034_529,236_1102_1222,2147_1151_1450,1622_1152_466   
Merged-chr1-3093741-8   chr1    3093556 3093940 +       0.000000        111     8       2204_818_1206,721_1086_509      1323_261_882,2087_81_1673       1020_830_1468,841_329_1875,973_265_108,2114_1844_1328  
Merged-chr1-3119632-3   chr1    3119557 3119723 +       0.000000        111     3       1156_567_92     329_295_991     1842_917_767
< end snippet >

The current utility, mergePeaks_resolve, takes the output from HOMER mergePeaks and returns a table with mean peak intensities from registered peaks.

< begin snippet of mergePeaks_resolve output >
chrom   start   end     peak1.bed       peak2.bed       peak3.bed
chr1    3114586 3114893 42      50      58  
chr1    3202344 3202964 58      50.6667 56.1667
chr1    3255462 3255793 49.3333 47      58  
chr1    3241809 3242278 43.8571 43.4    43  
chr1    3076463 3076720 58      54.3333 58
chr1    3167073 3168007 51.4    56.3077 51.8
chr1    3093556 3093940 56      58      52.5
chr1    3119557 3119723 32      58      58  
< end snippet >

The resulting table can be used as input for higher-level analysis - e.g. peak-wise analysis to identify unique temporal epigenomic patterns.

Comment on running time:
Using boost threads to process resolve input peak files in parallel, mergePeaks_resolve is fast.
On a test case of 6 histone peak files containing ~16M peaks each, and where the mergePeak output file contained 35K records, the script took 5m21s on a machine with the following specs:
Intel Xeon E5540 (2.53GHz), 8 cores with hyperthreading, 96G RAM.
 

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


