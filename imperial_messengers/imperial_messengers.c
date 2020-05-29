/// CONSTRAINTS
// The first line of the input will be N, the number of cities, such that 1 <= n <= 100.
//
// atarng(note): there is no bound on the value of A(i,j),
// assume 0-UNSIGNED_MAX? 0-INT_MAX?
//
/// TIME TRACKING
//
// 4min:40seconds to understand what the problem is asking.
// 40 minutes to get an environment that can take stdin as input.
// Install Cygwin/GCC
//
// Begin considering how to solve problem:
// Naiive solution: go through all the permutations of legal paths:
// Sum them all up, and then return the lowest travel time.
// Greedy approach: Sort the possible paths by travel time.
//
// WOOPS: Realized Mistake in my understanding of the problem after not being able
//        to reach the example output given the example input.
//        go over the prompt further
// Realize what the actual constraints of the problem is.
//  +45 minutes. (Unlimited messengers can be sent, not just a single messenger)
//
// + 1.5 Hour write custom "City Traversal Heap"
// + 1 Hour implementing solution with the Custom heap, and debugging my bugs in the heap.
//
// Write Test Cases +30 minutes
//
// Total 4 hours 30 minutes
//

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>

// TODO(atarng): Are these ANSI C?
#include <math.h>

#define MAX_INT 0x7FFFFFFF
#define MAX_N 100
#define DIGITS_IN_MAX_INT 10
#define TERMINATING_SAFETY_CHECK 1000
#define ACTUAL_HEAP_HEAD 1

#define MIN(X,Y) ((X) < (Y) ? : (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? : (X) : (Y))

using namespace std;

struct TravelIndexPair {
  int travel_time_;
  int city_index_;  
  TravelIndexPair() { travel_time_ = city_index_ = 0; }
  TravelIndexPair(int t, int c){
    travel_time_ = t;
    city_index_ = c;
  }
  TravelIndexPair(const TravelIndexPair& copy){
    travel_time_ = copy.travel_time_;
    city_index_ = copy.city_index_;
  }
};

// Use Min Heap Logic
struct TravelIndexUpdateHeap {
  int heap_capacity_;
  int heap_filled_;
  TravelIndexPair* heap_head_;
  
  TravelIndexUpdateHeap() {
    heap_capacity_ = 0;
    heap_filled_ = 0;
    heap_head_ = 0;
  } 
  bool empty() { return heap_filled_ == 0; }
  TravelIndexPair* top() { return &heap_head_[ACTUAL_HEAP_HEAD]; }
  void pop() {
    // assign value at heap head to last index in heap
    heap_head_[1] = heap_head_[heap_filled_--];
    
    // printf("remove top element: bubble down: (%d: %d) heap_size: %d\n",
    //     heap_head_[1].city_index_, heap_head_[1].travel_time_, heap_filled_);

    // bubble down.
    int bubble_down_index = ACTUAL_HEAP_HEAD;
    int iterations = TERMINATING_SAFETY_CHECK;
    while(--iterations &&
      bubble_down_index < heap_filled_) {
      // Left: 2 * index, Right: 2 * index + 1
      int left_index = 2 * bubble_down_index;
      int right_index = 2 * bubble_down_index + 1;
      // swap with smaller child to maintain min_heap_property
      int smaller_index = (heap_head_[left_index].travel_time_ <
          heap_head_[right_index].travel_time_) ?
          left_index : right_index;
          
      if(smaller_index > heap_filled_) {
        break;
      }

#if defined(AT_DEBUG)
      printf("swap: (%d: %d, %d) <-> (%d: %d,%d)\n",
          bubble_down_index,
          heap_head_[bubble_down_index].city_index_,
          heap_head_[bubble_down_index].travel_time_,
          smaller_index,
          heap_head_[smaller_index].city_index_,
          heap_head_[smaller_index].travel_time_);
#endif
    
      TravelIndexPair tmp = heap_head_[smaller_index];
      heap_head_[smaller_index] = heap_head_[bubble_down_index];
      heap_head_[bubble_down_index] = tmp;

      bubble_down_index = smaller_index;
    }
  }
  void push(TravelIndexPair to_push) {
    // printf("push: (%d: %d)\n", to_push.city_index_, to_push.travel_time_);
    if(heap_filled_ == heap_capacity_) {
      // Make sure to account for the empty/starting case.
      heap_capacity_ = (heap_capacity_ > 0) ? heap_capacity_ * 2 : 2;
      TravelIndexPair* temp = (TravelIndexPair*)malloc(heap_capacity_ *
          sizeof(TravelIndexPair));
      if(heap_filled_ != 0) {
        memcpy(temp, heap_head_, (heap_filled_+1) * sizeof(TravelIndexPair));
      }
      heap_head_ = temp;
    }
    ++heap_filled_;
    heap_head_[heap_filled_] = to_push;

    // Bubble Up
    int bubble_up_index = heap_filled_;
    // Parent: floor(index/2)
    int iterations = TERMINATING_SAFETY_CHECK;
    while(--iterations &&
      bubble_up_index > ACTUAL_HEAP_HEAD) {
      int parent_index = floor(bubble_up_index / 2);
      if(heap_head_[parent_index].travel_time_ <
          heap_head_[bubble_up_index].travel_time_) {
        break;
      }
      // swap
      TravelIndexPair tmp = heap_head_[parent_index];
      heap_head_[parent_index] = heap_head_[bubble_up_index];
      heap_head_[bubble_up_index] = tmp;
      bubble_up_index = parent_index;
    }
  }
  
  void PrintHeap() {
#if defined(AT_DEBUG)
    printf("PrintHeap:\n");
    for(int i = 1; i <= heap_filled_; ++i) {
      printf("(%d: %d)%s",
          heap_head_[i].city_index_,
          heap_head_[i].travel_time_,
          i % heap_filled_ ? " " : "\n");
    } printf("\n");
#endif

  }
  
  // All elements in priority queue reduce by a fixed amount
  // Order is maintained:
  void ElapseTime(int time_elapsed) {
    for(int i = 0; i < heap_filled_; ++i) {
      heap_head_[i].travel_time_ -= time_elapsed;
    }
  }
};

// return: When the last city has received the message.
int MinimumTimeRequiredForMessageToBeKnown(int number_of_cities,
                                           int* travel_array[]) {
  /// Not needed can just exit when the heap is empty.
  unsigned short city_array_size = (number_of_cities)*sizeof(bool);
  bool* visited_array = (bool*)(malloc(city_array_size));
  /// Fill out all the cities that still need to be visited.
  memset(visited_array, 1, city_array_size);
  
  // Visited Capital by default:
  *visited_array = 0;
  int visited_count = 1;

  TravelIndexUpdateHeap travel_heap;
  // starting from city 0, send out all messengers.
  for(int i = 1; i < number_of_cities; ++i) {
    auto time_to_insert = travel_array[i][0];
    if(time_to_insert > 0) {
      TravelIndexPair to_insert(time_to_insert, i);
      travel_heap.push(to_insert);
    }
  }
  
  travel_heap.PrintHeap();

  int result = 0;
  int iterations = TERMINATING_SAFETY_CHECK;
  // Exit if we've visited all the cities, or there are no more cities to visit.
  while(--iterations &&
    visited_count < number_of_cities && !travel_heap.empty()) {
      // We actually want to copy it.
    TravelIndexPair top(*travel_heap.top());
    int visited = top.city_index_;
    // printf("messenger arrived: %d(tr: %d)\n", visited, top.travel_time_);
    travel_heap.pop();

    // If we've already visited this city, then don't process it.
    if(!visited_array[visited]) {
      continue;
    }
    
    printf("visited: %d took %d time\n", visited, top.travel_time_);
    
    visited_array[visited] = 0;
    ++visited_count;
    int time_elapsed = top.travel_time_;

    // Decrement time among all remaining messengers.
    result += time_elapsed;
    travel_heap.ElapseTime(time_elapsed);
    travel_heap.PrintHeap();

    // Send new messengers out from the visited city
    for (int i = 1; i < number_of_cities; ++i) {
      // If we have not visited the city already we can broadcast
      // our messengers again.
      int time_to_send_msg = -1;
      if (visited_array[i] &&
          (time_to_send_msg = travel_array[visited][i]) > 0) {

        // printf("send(from: %d) messenger: %d (t: %d)\n",
        //     visited, i, time_to_send_msg);

        // city_to_send_messenger_to
        TravelIndexPair to_insert(time_to_send_msg, i);
        travel_heap.push(to_insert);
      }
    }
    travel_heap.PrintHeap();
  }
  printf("END/MinimumTimeRequiredForMessageToBeKnown] result: %d\n", result);
  return result;
}

// Data Processing for the NxN array.
// TECHNICALLY unnceccesary since the data is mirrored,
// could technically always just access it through a mix of Transposing
// and direct access. HOWEVER figured I'd populate it, so that my solution
// could potentially be generic, for non symmetrical travel between cities.
int** PopulateTravelArray(int argc, const char* argv[]) {
  int byte_length = (argc) * sizeof(int*);
  int** travel_array = (int**)(malloc(byte_length));
  for(int i = 0; i < argc; ++i) {
    travel_array[i] = (int*)(malloc(byte_length));
    // zero fill
    memset(travel_array[i], 0, byte_length);
  }
  
  const char* TO_PARSE = " ";
  for(int i = 1; i < argc; ++i) {
    const char* line_value_ro = argv[i];
    char* copy_line_value = strdup(line_value_ro);
    char* travel_path = strtok(copy_line_value, TO_PARSE);
    int j = 0;
    while (travel_path != NULL) {
      int to_set = -1;
      if(travel_path[0] != 'x'){
        to_set = atoi(travel_path);
      }
      travel_array[i][j] = to_set;
      travel_array[j][i] = to_set;
      ++j;
      travel_path = strtok (NULL, TO_PARSE);
    }
    free(copy_line_value);
  }

#if defined(AT_DEBUG)
  printf("PopulateTravelArray\n");
  for(int i = 0; i < argc; ++i) {
    for(int j = 0; j < i; ++j) {
    printf("%d ", travel_array[i][j]);
    } printf("\n");
  } printf("\n");
#endif
  
  return travel_array;
}

//////////////
// 21
// 20 19
// 18 17 16
// 15 14 13 12
// 11 10 9 8 7
// 6 5 4 3 2 1
// OUTPUT: 11
int main(int argc, char* argv[]) {
  int** as_travel_grid = 0;
  /// Preprocess string, so that we end up with an NxN Grid.
  /// Begin with dummy sample data.
  // printf("*** DEFAULT TEST ***\n");
  // int test_length = 5;
  // const char* test_v[5] = {"5",
  //                          "50",
  //                          "30 5",
  //                          "100 20 50",
  //                          "10 x x 10"};
  // as_travel_grid = PopulateTravelArray(test_length, test_v);
  // MinimumTimeRequiredForMessageToBeKnown(test_length, as_travel_grid);

  // Swap this in when we have an environment that receives argc argv
  // PopulateTravelArray(argc, argv);
  
  /// Or possibly just use stdin
  printf("\n*** TEST STDIN ***\n");
  printf( "Enter Cities: ");
  char cities[4];
  fgets( cities, 4, stdin);
  printf("%s\n", cities);
  int num_lines_to_read = atoi(cities);
  const char** stdin_test = (const char**)malloc(num_lines_to_read * sizeof(char*));
  stdin_test[0] = cities;
  for(int i = 1; i < num_lines_to_read; ++i) {
    printf( "Travel times for City_%d: ", i+1);
    // (i * (DIGITS_IN_MAX_INT + 1) + 2), including spaces and newline/null
    char* travel_path =
        (char*)malloc((i * (DIGITS_IN_MAX_INT + 1) + 2) * sizeof(char));
    fgets(travel_path, (i * (DIGITS_IN_MAX_INT + 1) + 2), stdin);
    printf("%s", travel_path);
    stdin_test[i] = travel_path;
  } printf("\n");
  as_travel_grid = PopulateTravelArray(num_lines_to_read, stdin_test);
  MinimumTimeRequiredForMessageToBeKnown(num_lines_to_read, as_travel_grid);
  free(stdin_test);

	return 0;
}

