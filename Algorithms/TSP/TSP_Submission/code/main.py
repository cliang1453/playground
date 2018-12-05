#!/usr/bin/env python3

from ctypes import *
tsp = cdll.LoadLibrary('./tsp.so')
tsp.f.argtypes = [c_char_p]*5
tsp.f.restype = c_int
# argtype: tsp, sol, trace, timeout-in-seconds, seed (set seed=0 for random seed)
tsp.f(b"./tsp-data/Berlin.tsp", b"Berlin.sol",b"Berlin.trace", b"2", b"0")


