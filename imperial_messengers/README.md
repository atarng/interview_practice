# Imperial Messengers

Prompt:
The empire has a number of cities.  For communicating important messages from
the capitol to other cities, a network of messengers is going to be set up.
Some number of messengers will be stationed in the capitol city. Each
messenger will ride to one other city, where the message will be posted in the
town square, and handed off to some number of new messengers, each of whom
will ride of to a different city, repeating the process.  The empire’s
unemployment rate is such that there are no limits placed upon the number of
messengers in each city, the only goal is to get the message communicated
throughout the empire as quickly as possible.

Problem Breakdown:
Starting from a source node, what is the minimum time to reach every possible
node in the graph.
This is essentially a "minimum-spanning-tree" problem.
Candidate Algorithms include Djistra's Algorithm, and Prim's algorithms.

My first implementation involves creating a "custom min heap", where the
elements are sorted by travel time, and mapped to the identity value of the
city. After visiting each city, it decrements all of the existing messengers
in the heaps remaining travel time, it will then add all of its messengers to
the heap.

After benchmarking, I have confirmed that this method is slower than traditional
Djistra's algorithm, as linearly searching to find the minimum node to use
is about the same cost/quicker than having to maintain a heap that updates based
on the visited nodes.

