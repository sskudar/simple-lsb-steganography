# simple-lsb-steganography
## Simple implementation for LSB steganography in C++

### Dependencies:

  OpenCV 2.4.13.6+ (and its dependencies: hdf5 and gtkglext-x11-1.0 - see makefile)

  Setup using the yaourt AUR helper on Arch linux and its derivatives:
  
  ```
  yaourt opencv   #select opencv and opencv-samples
  yaourt hdf5
  yaourt gtkglext
  ```
  
  Then set up the makefile accordingly. You can use pkg to see if everything installed correctly:
  
  ```
  pkg-config --libs OpenCV
  ```
  
  An error will be indicated with the first line saying:
  > Package OpenCV was not found in the pkg-config search path
  
  In that case, consult the official documentation or try linking the libraries yourself (e.g. "-L/usr/lib -lhdf5" in makefile with gcc/g++)

### Usage:

  ```
  make
  
  ./stegano -d|-e -i input.png [-o output.png] [-h]
  ```
  
### Parameters:

  ```

  -h            help
  
  -d            decrypt (extract message)
  
  -e            encrypt (imprint message)
  
  -i  input     specify input image
  
  -o  output    specify output image  (must be lossless, further testing pending)
  ```
