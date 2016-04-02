# Photo-recovery

This is a course project of CS50 problem set 4: recover JEPG files from a forensic image.

The C code reads in the data from “card.raw” and find the JPEG files. JPEG files start with magic number of “ff d8 ff”. Search the magic number within the file to locate the start of a JPEG file.

The "card.raw" data contains 50 images of CS50 course staff, and I unlocked them with the code.

Code written and tested by Yiz.  
