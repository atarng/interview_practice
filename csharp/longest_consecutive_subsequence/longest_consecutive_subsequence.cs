///
// Source: https://leetcode.com/problems/longest-consecutive-sequence/submissions/
///
public class Solution
{
    public int LongestConsecutive(int[] nums)
    {
        int longestSequence = 0;
        // put everything into a hashset: insert: (O(n)), Lookup O(1)
        HashSet<int> numsHash = new HashSet<int>(nums);
        
        foreach(var num in numsHash)
        {
            // we only want to check each sequence once, set up condition to only enter at head or tail of the sequence.
            bool isBeginningOfSequeunce = !(numsHash.Contains(num - 1));
            if(isBeginningOfSequeunce)
            {
                int currSequence = 1;
                int numToSearch = num;
                while(numsHash.Contains(++numToSearch))
                {
                    ++currSequence;
                }
                longestSequence = Math.Max(longestSequence, currSequence);
            }
        }
        
        return longestSequence;
    }
}
