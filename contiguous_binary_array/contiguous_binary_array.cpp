class Solution {
public:
    int findMaxLength(vector<int>& nums) {
        return DP_FindMaxLength(nums);
    }
private:
    // []
    // [0]
    // [0,1]
    // [0,0,0,1,1,1,0]
    // [0,1,0,0,0,0,0,0,0,1,0]
    // [0,0,1,0,0,0,1,1]
    // [1,0,0,0,1,0,0,0,1,1]
    // [1,0,0,1,1,1,0]
    //////////
    int DP_FindMaxLength(vector<int>& nums) {
        if(nums.size() == 0) return 0;

        int counter_1_0 = 0;
        int max_length = 0;
        
        vector<pair<int, int>> negative_peaks;
        vector<pair<int, int>> positive_peaks;

        for(int i = 0;  i < nums.size(); ++i) {
            // cout << "i: " << i << endl;
            counter_1_0 += (nums[i] == 1) ? 1 : -1;
            nums[i] = counter_1_0;

            if(counter_1_0 == 0) {
                max_length = max(max_length, i+1);
            } if(i < 2) continue;
            // Found a "peak", skip the ones that are already 0.
            auto temp = nums[i - 1];
            if(i > 1 && temp != 0 &&
               abs(temp) < abs(nums[i-2]) &&
               abs(temp) < abs(nums[i]) ) {
                // indices_val_0.push_back(i-1);
                // indices_val_0.push_back({nums[i-1], i-1});
                auto p = make_pair(temp, i-1);
                if(temp < 0) {
                    negative_peaks.push_back(p);
                } else {
                    positive_peaks.push_back(p);
                }
            }
        }
        /// Check if the last node is a "peak"
        int last_index = nums.size() - 1;
        int last_value = nums[nums.size() - 1];
        if(nums.size() > 1 && abs(last_value) <
                              abs(nums[nums.size() - 2])) {
            // indices_val_0.push_back(nums.size() - 1);
            auto p = make_pair(last_value, last_index);
            if(last_value < 0) {
                negative_peaks.push_back(p);
            } else {
                positive_peaks.push_back(p);
            }
        }

        sort(negative_peaks.begin(), negative_peaks.end(), greater<pair<int,int>>());
        sort(positive_peaks.begin(), positive_peaks.end());

        /// From now on, treat this variable as an offset.
        counter_1_0 = nums[0] * -1;
        for(int i = 1;  i < nums.size(); ++i) {
            if(i > 1) {
                counter_1_0 += (nums[i-2] > nums[i - 1]) ? 1 : -1;
            }
            int offset = 0;
            if (counter_1_0 > 0) {
                auto iter = negative_peaks.begin();
                // clean up duplicate peaks
                while (iter != negative_peaks.end() &&
                       iter->first + counter_1_0 == 0) {
                    // update max
                    max_length = max(max_length, (iter->second - i) + 1);
                    iter++;
                    offset++;
                }
                if(offset > 0) {
                    auto prev_size = negative_peaks.size();
                    negative_peaks.erase(negative_peaks.begin(), iter);
                }
            } else {
                auto iter = positive_peaks.begin();
                // clean up duplicate peaks
                while (iter != positive_peaks.end() &&
                       iter->first + counter_1_0 == 0) {
                    // update max
                    max_length = max(max_length, (iter->second - i) + 1);
                    iter++;
                    offset++;
                }
                if(offset > 0) {
                    auto prev_size = positive_peaks.size();
                    positive_peaks.erase(positive_peaks.begin(), iter);
                }
            }
            if(max_length >= (nums.size() - i)) {
                // impossible to find anything larger.
                return max_length;
            }
        }
        return max_length;
    }
};
