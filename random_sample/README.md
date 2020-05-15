# interview_practice
This file benchmarks the performance of my jerry-rigged/homebrew algorithm for
generating a random set of numbers from a range (without replacement, so no duplicates).

This is compared against a method published by Robert Floyd, as well as the standard
method over an interatable collection representing the range.

For Range N, and set K, where K < N, these are the general results:
- My algorithm scales against K's proximity to N, similarly to Robert Floyd's method.
-- However my algorithm apparently performs better than Robert Floyd's (and I can't
   figure out why. In theory his algorithm should be faster (O(1) vs. O(N))
- The standard method, which requires an existing collection, scales better with K,
  but does scale against N, causing performance to deteriate over large N.

Sample Values:
N = 1000 K = 999 T(rials): 1000
mine: 355ms
robertFloyd: 665ms
Standard: 86ms

N = 1000 K = 500 T: 1000
mine: 167ms
robertFloyd: 268ms
Standard: 81ms

N = 1000 K = 100 T: 1000
mine: 37ms
robertFloyd: 58ms
Standard: 72ms


N = 100 K = 50 T: 1000
mine: 17ms
robertFloyd: 29ms
Standard: 26ms


N = 10000 K = 2000 T: 1000
mine: 754ms
robertFloyd: 1035ms
Standard: 561ms

N = 10000 K = 1000 T: 1000
mine: 361ms
robertFloyd: 519ms
Standard: 527ms
