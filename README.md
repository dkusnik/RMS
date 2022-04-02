# Robust Mean-Shift implementation

This software implements the Robust Mean-Shift algoritm. Code is written in CUDA.

# Requirements
- CUDA SDK
- libpng

# Installation
Run:

```
make lib
make main
```

# Usage
Sample usage:
`./main_ms_rlsf <reference image { rgb }> <noisy image {rgb}> <block_radius> <alpha> <sigma> <iter>`
where:

Reference image - original, not noisy image
Noisy image - image which will be filtered
block_radius - radius of the processing block B
alpha - numer of first pixels being choosen from patch
sigma - smoothing parameter
iter - number of iteration for limiting of fliter execution

# Acknowledgment

This code uses parts of the Fourier 0.8 library by Emre Celebi licensed on GPL avalaible here:
http://sourceforge.net/projects/fourier-ipal
http://www.lsus.edu/faculty/~ecelebi/fourier.htm

