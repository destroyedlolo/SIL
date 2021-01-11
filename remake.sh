#!/bin/bash

# rebuild the makefile
# needs LFMakeMaker (https://github.com/destroyedlolo/LFMakeMaker)

LFMakeMaker -v +f=Makefile --opts="-Wall -O2" *.c -t=sil > Makefile

