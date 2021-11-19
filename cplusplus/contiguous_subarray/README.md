# interview_practice : Contiguous SubArray:

You are given an array a of N integers. For each index i, you are required to
determine the number of contiguous subarrays that fulfills the following conditions:
The value at index i must be the maximum element in the contiguous subarrays, and
These contiguous subarrays must either start from or end with i.

Output:
An array where each index i contains an integer denoting the maximum number of
contiguous subarrays of a[i].

Example:
a = [3, 4, 1, 6, 2]
output = [1, 3, 1, 5, 1]

Explanation:
For index 0 - [3] is the only contiguous subarray that starts (or ends) with 3,
and the maximum value in this subarray is 3.
For index 1 - [4], [3, 4], [4, 1]
For index 2 -[1]
For index 3 - [6], [6, 2], [1, 6], [4, 1, 6], [3, 4, 1, 6]
For index 4 - [2]
So, the answer for the above input is [1, 3, 1, 5, 1]

=======================================================================================

Regarding the above prompt, a former coworker stumbled upon this problem on leetcode.
I got curious because he was asking about dynamic programming and whether he could
optimize his solution.

Long story short, yes. Yes he could.
- In the file, there "Derek" has an implementation that has an approach that is at worst
case O(n^2), although on a randomized case, would terminate relatively early. Still it
doesn't hurt to consider the worst case (all ascending, all descending).

- Another coworker suggested to possibly sort the values with a parallel array of the
associated indices. I may be missing some logic from him, since it seems like he had
an idea to make it constant time to lookup the set at each traversal, however at best
I could figure out an nlogn solution for it.

- Finally, we come down to what I ended up writing when the original prompt was asked
and whether dynamic programming was applicable to it. It effectively has an O(n)
traversal because when it hits a node that has already validated subarrays in that
direcction it will skip over the nodes that it are within that validation range.
This coupled with the early termination should give an effective O(n) traversal.
