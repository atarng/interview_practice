#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

using namespace std;

std::vector<int> g_population;
std::mt19937 g_gen;
constexpr int TRIALS = 1000;
constexpr int CHECK_DISTRIBUTION_LIMIT = 50;

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
    
    // This part can be optimized as a binary search
    // int offset = 0;
    // while(offset < to_exclude.size() &&
    //  (raw_rand+offset) >= to_exclude[offset]) offset++;
    
    int to_insert = (raw_rand + offset);
    to_exclude.insert(to_exclude.begin() + offset, to_insert);
    solution.push_back(to_insert);
  }

  // printf("GenerateRandomSample] Range: %d, Samples: %d\n", range, samples);
  // // for (auto s : solution) {
  // for (int i = 0; i < solution.size(); ++i) {
  //   auto s = solution[i];
  //   cout << ((s < 10) ? "0" : "") << s <<
  //       (((i + 1) % 10 == 0) ? "\n" : " ");
  // }
  
  return solution;
}

// This is a deterministic way of generating the set.
// Accepted answer to: https://tinyurl.com/yadmswnw
std::unordered_set<int> robertFloydRandomSampleSet(int N, int k) {
  std::unordered_set<int> elems;

  // Init if we care about insertion order.
  // (K, 0): preallocating and filling by index is slower.
  // std::vector<int> solution;

  for (int r = N - k; r < N; ++r) {
    // No significant improvement using rand
    // auto v = rand() % r;
    int v = std::uniform_int_distribution<>(0, r)(g_gen);
    

    // there are two cases.
    // v is not in candidates ==> add it
    // v is in candidates ==> well, r is definitely not, because
    // this is the first iteration in the loop that we could've
    // picked something that big.
    if (!elems.insert(v).second) {
      // printf("duplicate(%d), insert max(%d)\n", v, r);
      elems.insert(r);

    // Init if we care about insertion order.
    //   // Turns out this is actually slower: solution[r-k] = r;
    //   solution.push_back(r);
    // } else {
    //   // Turns out this is actually slower: solution[r-k] = v;
    //   solution.push_back(v);
    }
  }
  
  // printf("otherRandomSample] Range: %d, Samples: %d\n", N, k);
  // for (auto s : solution) cout << s << " ";
  // cout << endl;

  // If we don't actually care about knowing what order the elements
  // are generated in then we can just copy it into the vector here.
  // std::vector<int> solution(elems.begin(), elems.end());
  // std::shuffle(solution.begin(), solution.end(), g_gen);
  // return solution;

  return elems;
}

// preallocating gen and using the same one over initializing it each trial
// is significantly faster. Switched to using global instead of pass by ref.
// std::vector<int> otherRandomSample(int N, int k, std::mt19937& gen) {
std::vector<int> robertFloydRandomSample(int N, int k) {
  // This part could be making it too slow (copy constructor)...
  auto as_set = robertFloydRandomSampleSet(N, k);
  std::vector<int> solution(as_set.begin(), as_set.end());
  std::shuffle(solution.begin(), solution.end(), g_gen);
  return solution;
}

// Prepopulate so we don't do this per trial.
void PrepopulateRange(int N, std::vector<int>& population) {
  population = vector(N, 0);
  for(int i = 0; i < population.size(); ++i) {
    population[i] = i;
  }
}

// Using global population instead of pass by reference to match method signature of
// BenchmarkHelper, This algorithm scales with N.
// std::vector<int> SampleFromPopulatedRange(int K, const std::vector<int>& population) {
std::vector<int> SamplePopulatedRange(int /*dummy_N*/, int K) {
  std::vector<int> samples;
  std::sample(g_population.begin(), g_population.end(),
              std::back_inserter(samples),
              K, std::mt19937{std::random_device{}()});
  return samples;
}

void BenchMarkingHelper(int N, int K,
                        std::function<vector<int>(int, int)> to_execute) {
  if (K > N) {
    printf("K > N this will fail!");
    return;
  }
  int count = 0;
  vector<int> nums(N, 0);
  if(N <= CHECK_DISTRIBUTION_LIMIT) {
    while(count++ < TRIALS) {
      auto sample =
          to_execute(N, K);
      for(auto s : sample) {
        ++nums[s];
      }
    }
  } else {
    while(count++ < TRIALS) {
      to_execute(N, K);
    }
  }

  if(N <= CHECK_DISTRIBUTION_LIMIT) {
    cout << "distribution:" << endl;
    for(int i = 0; i < nums.size(); ++i) {
      printf("[%s%d]: %s%d%c",
          (i<10 ? "0" : ""), i,
          (nums[i]<10 ? "0" : ""), nums[i],
          ((((i+1) % 5) == 0) ? '\n' : ' '));
    }
    cout << endl;
  }
}

// To allow us to use the Robert Floyd method without doing a
// copy constructor into the vector.
void BenchMarkingHelperSet(int N, int K,
    std::function<unordered_set<int>(int, int)> to_execute) {
  if (K > N) {
    printf("K > N this will fail!");
    return;
  }

  int count = 0;
  while(count++ < TRIALS) {
    to_execute(N, K);
  }
}

int main() {
  srand(time(NULL));
  constexpr int N = 50;
  constexpr int K = 10;

  auto end = std::chrono::system_clock::now();
  auto start = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);

  start = std::chrono::system_clock::now();
  BenchMarkingHelper(N, K, GenerateRandomSample);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of mine: %lums\n\n", diff.count());

  // Will be used to obtain a seed for the random number engine
  std::random_device rd;
  // changed to a global variable to not impact benchmarking.
  g_gen = std::mt19937(rd());
 
   start = std::chrono::system_clock::now();
  BenchMarkingHelper(N, K, robertFloydRandomSample);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of robertFloyd: %lums\n", diff.count()); 
 
  start = std::chrono::system_clock::now();
  BenchMarkingHelperSet(N, K, robertFloydRandomSampleSet);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of robertFloydSet: %lums\n\n", diff.count());

  // Prepopulate range collection to not impact the benchmarking
  // std::vector<int> population; changed to a global variable to fit
  // the benchmarking framework
  PrepopulateRange(N, g_population);
  start = std::chrono::system_clock::now();
  BenchMarkingHelper(N, K, SamplePopulatedRange);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
  printf("execution of Standard: %lums\n\n", diff.count());

  return 0;
}
