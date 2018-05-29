# simple-lsb-steganography
Simple implementation for LSB steganography in C++

Dependencies:
  OpenCV 2.4.13.6+ (and its dependencies: hdf5 and gtkglext-x11-1.0 - see makefile)

Usage:
  make
  "./stegano -d|-e -i input.png [-o output.png] [-h]"

Parameters:
  -h            help
  -d            decrypt (extract message)
  -e            encrypt (imprint message)
  -i  input     specify input image
  -o  output    specify output image  (must be lossless, further testing pending)
