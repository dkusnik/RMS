# Robust Mean-Shift implementation

This software implements the Robust Mean-Shift algoritm. Code is written in CUDA.

# Requirements
- CUDA SDK (optional, recommended)
- OpenMP (if CUDA not provided, recommended)
- libpng

# Installation
You can define in Makefile if you will use the CUDA framework or not, by default yes, when not remove the CUDA variable in Makefile

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

