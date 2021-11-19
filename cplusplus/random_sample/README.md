# interview_practice
This file benchmarks the performance of my jerry-rigged/homebrew algorithm for
generating a random set of numbers from a range (without replacement, so no duplicates).

This is compared against a method published by Robert Floyd, as well as the standard
method over an interatable collection representing the range.

# Robert Floyd Algorithm:
- generates the random variables in  a deterministic way, with a rolling window of size (N-K), and iterated K times O(k), using an unordered_set to guarantee uniqueness, reallocating it into a vector O(k), shuffle O(k).
Runtime: O(k) (albeit an expensive k, having to reallocate a vector of size K again), Space: O(K)

# My Algorithm:
- Generate the variables in truly random order, decrementing the size of the next random range, binary-inserting into a sorted array (Oklogk)/O(k^2) due to having to shift all elements after insertion point, consider just using a set, adding the appropriate offset value to the random number generated, then adding the adjusted random number into the vector to return O(1).
Runtime: O(klogk) , Space: O(K)

# Standard STL C++ sample method
Runtime: O(N) , Space: O(N)

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

O(klogk)
