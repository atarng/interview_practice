#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

constexpr int ALLOWABLE_PRINT = 50;

////////////////////// RANDOM SAMPLE GENERATOR ////////////////////////////////
int ModifiedBinarySearch(const vector<int>& collection, int raw_rand) {
  int offset = 0;
  int beg = 0, end = collection.size() - 1;
  // printf("GenerateRandomSample] raw_random: %lu\n", raw_rand);
  bool upper_range = 0;
  while (beg <= end) {
    offset = (beg + end) / 2;

    auto to_search_for = (raw_rand+offset);
    auto left = collection[offset];
    auto right = (offset+1 < collection.size() ?
        collection[offset+1] :
        collection[collection.size() - 1]);

    if ((raw_rand+offset) < left) {
      upper_range = false;
      end = offset - 1;
    } else if ((raw_rand+offset+1) >= right) {
      upper_range = true;
      beg = offset + 1;
    } else {
      upper_range = true;
      break;
    }
  }
  offset = ((beg + end) / 2)  + (upper_range ? 1 : 0);
  return offset;
}
// Scales With K. Seems to always perform better than Robert Floyd...
// although I can't figure out why, it really should not O(nlogn) vs. O(1)...
vector<int> GenerateRandomSample(int range, int samples) {
  vector<int> solution; // Populated in the order that the numbers are generated in.
  vector<int> to_exclude; // Inserted into in sorted order.
  for(int i = 0; i < samples; ++i) {
    auto raw_rand = rand() % (range - to_exclude.size());
    // binary search rather than linear search
    int offset = ModifiedBinarySearch(to_exclude, raw_rand);    
    int to_insert = (raw_rand + offset);
    to_exclude.insert(to_exclude.begin() + offset, to_insert);
    solution.push_back(to_insert);
  }
  return solution;
}
////////////////// STEVE SOLUTION /////////////////////////////////////
// COULD NOT FIGURE OUT HOW TO MAKE THIS CONSTANT TIME!!!
// THIS ENDS UP LEADING TO AN NLOGN SEARCH FOR ALL THE VALUES!
// TODO(ATARNG): POKE STEVE'S BRAIN FOR WHAT HE INTENDED.
int get_value(vector<int>& search_space, int target, int default_end) {
  int left_index = -1;
  int right_index = default_end;
  int end = search_space.size();
  if(search_space.size() > 0) {
    int beg = 0;
    while(beg <= end) {
      int mid = beg + (end - beg) / 2;
      if(target > search_space[mid]) {
        beg = mid + 1;
      } else if(target < search_space[mid]) {
        end = mid - 1;
      } else {
        cout << "What." << endl;
      }
    }
    if(beg >= search_space.size()) {
      beg = search_space.size();
      end = search_space.size() - 1;
    } else if(end < 0) {
      beg = 0;
      end = 0;
    }
    swap(beg, end);
    left_index = search_space[beg] < target ? search_space[beg] : left_index;
    right_index = search_space[end] > target ? search_space[end] : right_index;
  }
  int left  = target - left_index;
  int right = right_index - target;
  int total = (left + right) - 1;
  search_space.insert(search_space.begin() + end, target);
  return total;
}
vector<int> SteveSolution(vector<int>& contiguous_sub_arrays) {
  auto end = std::chrono::system_clock::now();
  auto start = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);  
  vector<pair<int, int>> sorted_value_index_collection(0);
  int length = contiguous_sub_arrays.size();
  for(int i = 0; i < length; ++i) {
    sorted_value_index_collection.push_back({contiguous_sub_arrays[i], i});
  }

  start = std::chrono::system_clock::now();
  sort(sorted_value_index_collection.begin(), sorted_value_index_collection.end(),
      greater<pair<int,int>&>());
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of Steve_A: %lums\n\n", diff.count());

  vector<int> solution(length, 0);
  vector<int> previously_visited_indices(0);
  for(auto p : sorted_value_index_collection) {
    // Couldn't figure out how to make this constant time from Steve's Suggestion
    // TODO(atarng): figure out how to optimize this part.
    solution[p.second] = get_value(previously_visited_indices, p.second, length);
  }
  if(contiguous_sub_arrays.size() <= ALLOWABLE_PRINT) {
    cout << "Steve's Solution: " << endl;
    for(int i = 0; i < solution.size(); ++i) {
      cout << solution[i] << (((i+1) % 10 == 0) ? "\n" : " ");
    } cout << endl;
  } 
  return solution;
}
////////////////////////////// DEREK SOLUTION ///////////////////////
int GetCount(vector<int>& contiguous_sub_arrays, int index) {
  int val = contiguous_sub_arrays[index];
  int count = 1; // self
  for (int right = index+1; right < contiguous_sub_arrays.size(); ++right) {
    if(contiguous_sub_arrays[right] < val) {
      ++count;
      continue;
    } break;
  }
  for (int left = index-1; left >= 0; --left) {
    if(contiguous_sub_arrays[left] < val) {
      ++count;
      continue;
    } break;
  }
  return count;
}
vector<int> DerekGetSubArraysCount(vector<int>& contiguous_sub_arrays) {
  vector<int> count_sub_arrays(contiguous_sub_arrays.size(), 0);
  for(int i = 0; i <count_sub_arrays.size(); ++i) {
    auto result = GetCount(contiguous_sub_arrays, i);
    count_sub_arrays[i] = result;
  }
  if(contiguous_sub_arrays.size() <= ALLOWABLE_PRINT) {
    cout << "Dereks Solution: " << endl;
    for(int i = 0; i < count_sub_arrays.size(); ++i) {
      cout << count_sub_arrays[i] << (((i+1) % 10 == 0) ? "\n" : " ");
    } cout << endl;
  }
  return count_sub_arrays;
}

/////////////////////// MY SOLUTION //////////////////////////
void PopulateLeftSubArrays(vector<int>& contiguous_sub_arrays,
                           vector<int>& left_to_fill) {
  for(int i = 0; i < contiguous_sub_arrays.size(); ++i) {
    int termination_value = -1;
    int sub_array_sum = 0;
    int j = i-1;
    while(j >= 0) {
      if(contiguous_sub_arrays[j] > contiguous_sub_arrays[i]) {
        break;
      }
      if(contiguous_sub_arrays[j] > termination_value) {
        sub_array_sum += left_to_fill[j];
        termination_value = contiguous_sub_arrays[j];
        j -= (left_to_fill[j] );
      } else {
        --j;
      }
    }
    left_to_fill[i] = sub_array_sum + 1;
  }
}
void PopulateRightSubArrays(vector<int>& contiguous_sub_arrays,
                           vector<int>& left_to_fill) {
  for(int i = contiguous_sub_arrays.size() - 1; i >= 0; --i) {
    int termination_value = -1;
    int sub_array_sum = 0;
    int j = i+1;
    while(j < contiguous_sub_arrays.size()) {
      if(contiguous_sub_arrays[j] > contiguous_sub_arrays[i]) {
        break;
      }
      if(contiguous_sub_arrays[j] > termination_value) {
        sub_array_sum += left_to_fill[j];
        termination_value = contiguous_sub_arrays[j];
        j += (left_to_fill[j] );
      } else {
        ++j;
      }
    }
    left_to_fill[i] = sub_array_sum + 1;
  }
}
void ContiguousSubArrays(vector<int>& contiguous_sub_arrays) {    
  vector<int> left_subs(contiguous_sub_arrays.size(), 0);
  PopulateLeftSubArrays(contiguous_sub_arrays, left_subs);
  // for(auto l : left_subs) {
  //   cout << l << " ";
  // } cout << endl;

  vector<int> right_subs(contiguous_sub_arrays.size(), 0);  
  PopulateRightSubArrays(contiguous_sub_arrays, right_subs);
  // for(auto r : right_subs) {
  //   cout << r << " ";
  // } cout << endl;

  for(int i = 0; i < contiguous_sub_arrays.size(); ++i) {
    contiguous_sub_arrays[i] = left_subs[i] + right_subs[i] - 1;
  }
  if(contiguous_sub_arrays.size() <= ALLOWABLE_PRINT) {
    cout << "my_solution" << endl;
    for(int i = 0; i < contiguous_sub_arrays.size(); ++i) {
      cout << contiguous_sub_arrays[i] << (((i+1) % 10 == 0) ? "\n" : " ");
    } cout << endl;
  }
}

// vector<int> test_1 = {9, 7, 0, 6, 2};
// Answer: 1 5 1 3 1
// vector<int> test_2 = {3, 4, 1, 6, 2};
// Answer: [1,3,1,5,1]
int main() {
  srand(time(NULL));
  
  auto end = std::chrono::system_clock::now();
  auto start = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);  
  
  // try testing worst case-ish.
  // constexpr int blah = 100000;
  // vector<int> test_1(blah);
  // start = std::chrono::system_clock::now();
  // for(int i = blah; i > 0; --i){
  //   test_1[i-1] = i;
  // } end = std::chrono::system_clock::now();
  // diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  // printf("Sample Generation Time: %lums\n\n", diff.count());

  // Constraints members of 1 <= N[i] <= 1000000
  // Unfortunately MY random sample generator can't generate samples
  // of 1000000... so we'll just set it to 300000
  start = std::chrono::system_clock::now();
  vector<int> test_1 = GenerateRandomSample(10000000, 300000);
  // vector<int> test_1 = GenerateRandomSample(300, 150);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("Sample Generation Time: %lums\n\n", diff.count());
  
  if(test_1.size() <= ALLOWABLE_PRINT) {
    cout << "testing:\n";
    for(int i = 0; i < test_1.size(); ++i) {
      cout << test_1[i] << (((i+1) % 10 == 0) ? "\n" : " ");
    } cout << endl; 
  }

  start = std::chrono::system_clock::now();
  DerekGetSubArraysCount(test_1);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of Derek's: %lums\n\n", diff.count());

  start = std::chrono::system_clock::now();
  SteveSolution(test_1);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of Steve's: %lums\n\n", diff.count());

  start = std::chrono::system_clock::now();
  ContiguousSubArrays(test_1);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of mine: %lums\n\n", diff.count());

  cout << "Finished Execution!" << endl;
  return 0;
}
