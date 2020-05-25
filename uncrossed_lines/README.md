# From Leetcode May Challenge problem:

https://leetcode.com/explore/challenge/card/may-leetcoding-challenge/537/week-4-may-22nd-may-28th/3340/

We write the integers of A and B (in the order they are given) on two separate horizontal lines.
Now, we may draw connecting lines: a straight line connecting two numbers A[i] and B[j] such that:
A[i] == B[j];
The line we draw does not intersect any other connecting (non-horizontal) line.
Note that a connecting lines cannot intersect even at the endpoints: each number can only belong to one connecting line.
Return the maximum number of connecting lines we can draw in this way.

#Example
Input: A = [1,3,7,1,7,5], B = [1,9,2,5,1]
Output: 2

Constraints
1 <= A.length <= 500
1 <= B.length <= 500
1 <= A[i], B[i] <= 2000
