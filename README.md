0) Copy the tarball to a CSE machine (e.g. e5-cse-135-??.cse.psu.edu for ?? in 01-38) and extract it into a local directory of your choosing. This project simulates putting the memory references and values generated through naive ( O(N3) ) matrix multiplication of square matrices of size N through a parameterized cache hierarchy. Most of the functionality for this program has already been provided. However, a) three helper functions for field sizing for cache access currently return a dummy value or do nothing, and b) two key functions needed to properly perform caching (namely, moving data into and out of a given layer of cache, FILL and WRITEBACK, respectively) are currently implemented as stub functions that do nothing, causing the program to crash if they are relied upon. Your job will be to implement these missing functionalities (defined in "YOURCODEHERE.h") within the functions defined in "YOURCODEHERE.c". You may define additional helper functions in YOURCODEHERE.c (a function to compute the lg2 of a power of 2 is useful - don't use a floating-point based library function...).

   **Each of the five functions has a single missing region of code; with the exception of the initialization function which sets several variables, these should all be < 10 lines of additional code/function (more often 1-4, depending on formatting).** You will need to read through the provided framework to figure out how to properly use the "performaccess" function to set local contents based on another level's data (fill) and to write data from the local contents into the next level of the memory hierarchy (writeback).

You will need to familiarize yourself with the existing functions defined in csim.c, specifically "performaccess", and the cache structure defined in csim.h, **although you are not allowed to modify them.**

Your project, once complete, will be able to correctly execute all tests invoked by "make test" as well as other cache and matrix configurations not present in the test list. Only cache hierarchies with monotonically nondecreasing block sizes (in integer multiples of 8-bytes) throughout the cache hierarchy will be tested. Similarly, only cache hierarchies with monotonically nondecreasing capacity from upper to lower caches will be tested.

1) Ensure that your environment is correctly configured (e.g. with default gcc, etc.) by running "make test"

   You can verify correct initial state of your environment/files by noting the following:

1) the code should compile without any errors or warnings.
2) the first test case (no cache instantiated) should run to completion and match the output in the included copy of the output from running make test on a completed version of the program
2) the second test case should quickly generate a segfault due to the unimplemented stub functions
2) Modify YOURCODEHERE.c -- this is the **only** file you will be modifying and turning in. Your project **MUST** compile without modification to the makefile, or any other source files. Your code will be recompiled against the other files in their original state, **on CSE servers**. Any reliance on additional modifications will likely result in non-compiling status under test and a **zero** for the project. Please ensure that any code you develop on a non-CSE platform works on the CSE servers, as the code is **NOT GENERALLY PORTABLE.**

   There is missing code in each of the functions in YOURCODEHERE.c. Descriptions of the functionality of each function are in YOURCODEHERE.h.

3) Continue to test your project. All tests in "make test" should run to completion (expected total run time 1-2 minutes, mostly in last test). Statistics for matrix sizes <= N=8 should match the provided output statistics exactly. Statistics for larger matrix sizes should be very similar but output may or may not be identical.
3) You will turn in only the "YOURCODEHERE.c" file online, via CANVAS (i.e. here)

   In addition to your code fixes, provide, as a multi-line comment in YOURCODEHERE.c, a description of your testing approach to check whether or not your output is correct, given that the contents of invalid memory locations within the cache will contain arbitrary data that may not match between different runs.
