#define DEBUG 1

class Solution {
public:
    // constraints:
    // 1 <= A.length <= 500
    // 1 <= B.length <= 500
    // (NOTE A.length != B.length)
    // 1 <= A[i], B[i] <= 2000
    // (NOTE A[i], B[i] are not unique; A, B can contain duplicates in their sets)
    int maxUncrossedLines(vector<int>& A, vector<int>& B) {
        return dpMaxUncrossedLines(A,B);
    }
private:
    int dpMaxUncrossedLines(vector<int>& A, vector<int>& B) {
        vector<vector<int>> dp_a_b(A.size() + 1, vector<int>(B.size() + 1, 0));
        for(int r = 1; r <= A.size(); ++r) {
            for(int c = 1; c <= B.size(); ++c) {
                dp_a_b[r][c] = max({dp_a_b[r-1][c],
                                    dp_a_b[r][c-1],
                                    dp_a_b[r-1][c-1] + (A[r-1] == B[c-1])});
            }
        }
        return dp_a_b[A.size()][B.size()];
    }
};
