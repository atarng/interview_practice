// #define AT_DEBUG

#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <iostream>
#include <chrono>

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

      // stop bubbling down if our value is less than our children
      // Or if we hit the end.
      if(smaller_index > heap_filled_ ||
          heap_head_[bubble_down_index].travel_time_ <=
          heap_head_[smaller_index].travel_time_ ) {
        break;
      }

// #if defined(AT_DEBUG)
//       printf("swap: (%d: %d, %d) <-> (%d: %d,%d)\n",
//           bubble_down_index,
//           heap_head_[bubble_down_index].city_index_,
//           heap_head_[bubble_down_index].travel_time_,
//           smaller_index,
//           heap_head_[smaller_index].city_index_,
//           heap_head_[smaller_index].travel_time_);
// #endif

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
      // Stop Bubbling Up if our travel time is greater than or equal to parent
      if(heap_head_[bubble_up_index].travel_time_ >=
          heap_head_[parent_index].travel_time_) {
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
  // void ElapseTime(int time_elapsed) {
  //   for(int i = 0; i <= heap_filled_; ++i) {
  //     heap_head_[i].travel_time_ -= time_elapsed;
  //   }
  // }
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
    if(travel_array[i][0] < 0) {
      continue;
    }
    TravelIndexPair to_insert(travel_array[i][0], i);
    travel_heap.push(to_insert);
  } // travel_heap.PrintHeap();

  int result = 0;
  int iterations = TERMINATING_SAFETY_CHECK;
  // int time_since_start = 0;
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
    visited_array[visited] = 0;
    ++visited_count;
    int time_elapsed = max(0, top.travel_time_ - result);
#if defined(AT_DEBUG)
    printf("visited: %d took %d time\n", visited, time_elapsed);
#endif
    result += time_elapsed;
    /// Decrement time among all remaining messengers.
    // travel_heap.ElapseTime(time_elapsed);
    // if(visited == 19) {
    //   travel_heap.PrintHeap();
    // }

    // Send new messengers out from the visited city
    for (int i = 1; i < number_of_cities; ++i) {
      // If we have not visited the city already we can broadcast
      // our messengers again.
      int time_to_send_msg = -1;
      if (visited_array[i] &&
          (time_to_send_msg = travel_array[visited][i]) > 0) {
        int time_plus_offset_from_start = result + time_to_send_msg;
// #if defined(AT_DEBUG)
//         printf("send(from: %d) messenger: %d (t: %d)\n",
//             visited, i, time_plus_offset_from_start);
// #endif
        // city_to_send_messenger_to
        TravelIndexPair to_insert(time_plus_offset_from_start, i);
        travel_heap.push(to_insert);
      }
    }
    // travel_heap.PrintHeap();
  }
  printf("result: %d\n", result);
  return result;
}

void Djistras_Algo(int number_of_cities, int* travel_array[]) {
  //2*
  unsigned short two_int_array_size = (/* 2* */number_of_cities)*sizeof(int);
  int* travel_parent_array = (int*)(malloc(two_int_array_size));
  travel_parent_array[0] = 0;
  /// Fill with "INFINITY"
  for(int i = 1; i < number_of_cities; ++i) {
    travel_parent_array[/* 2* */i] = MAX_INT;
  }

  unsigned short city_array_size = (number_of_cities)*sizeof(bool);
  bool* visited_array = (bool*)(malloc(city_array_size));
  memset(visited_array, 0, city_array_size);
  // Loop until we confirm visiting all nodes
  int visited_count = 0;
  int visited_time_total = -1;
  while(visited_count < number_of_cities) {
    // Get unvisited node with the smallest travel_time.
    int minimum_time_index = -1;
    // O(n)
    for(int i = 0; i < number_of_cities; ++i) {
      if(visited_array[i]) { continue; }
      if(minimum_time_index < 0 ||
          // travel_parent_array[/* 2* */i] < travel_parent_array[/* 2* */minimum_time_index]) {
          travel_parent_array[i] < travel_parent_array[minimum_time_index]) {
        minimum_time_index = i;
      }
    }
    // With this node, we then mark it as visited, and update its edges
    // if it is closer than all the other existing edges that have been set.
    visited_time_total = travel_parent_array[/* 2* */minimum_time_index];
    visited_array[minimum_time_index] = 1;
    ++visited_count;
    for(int i = 0; i < number_of_cities; ++i) {
      // Already Visited this node OR
      // No edge between these two nodes.
      if(visited_array[i] || travel_array[i][minimum_time_index] < 0) {
        continue;
      }
      // PersonalTravelTime + current edge
      int personal_travel_time = travel_parent_array[/* 2* */minimum_time_index];
      int edge_travel_time = travel_array[i][minimum_time_index];
      int& existing_travel_time = travel_parent_array[/* 2* */i];
      if(personal_travel_time + edge_travel_time < existing_travel_time) {
        existing_travel_time = personal_travel_time + edge_travel_time;
        // Is this even needed?
        // travel_parent_array[/* 2* */i+1] = minimum_time_index;
      }
    }
  }
  printf("result: %d\n", visited_time_total);
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
      if(travel_path[0] != 'x' &&
         travel_path[0] != 'X'){
        to_set = atoi(travel_path);
      }
      travel_array[i][j] = to_set;
      travel_array[j][i] = to_set;
      ++j;
      travel_path = strtok (NULL, TO_PARSE);
    }
    free(copy_line_value);
  }

// #if defined(AT_DEBUG)
//   printf("PopulateTravelArray\n");
//   for(int i = 0; i < argc; ++i) {
//     for(int j = 0; j < i; ++j) {
//     printf("%d ", travel_array[i][j]);
//     } printf("\n");
//   } printf("\n");
// #endif
  
  return travel_array;
}

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
#if defined(AT_DEBUG)
    printf( "Travel times for City_%d: ", i+1);
#endif
    // (i * (DIGITS_IN_MAX_INT + 1) + 2), including spaces and newline/null
    char* travel_path =
        (char*)malloc((i * (DIGITS_IN_MAX_INT + 1) + 2) * sizeof(char));
    fgets(travel_path, (i * (DIGITS_IN_MAX_INT + 1) + 2), stdin);
#if defined(AT_DEBUG)
    printf("%s", travel_path);
#endif
    stdin_test[i] = travel_path;
  } printf("\n");

  auto end = std::chrono::system_clock::now();
  auto start = std::chrono::system_clock::now();
  auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
  
  as_travel_grid = PopulateTravelArray(num_lines_to_read, stdin_test);
  
  printf("My Homebrew Heap:\n");
  start = std::chrono::system_clock::now();
  MinimumTimeRequiredForMessageToBeKnown(num_lines_to_read, as_travel_grid);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
  printf("execution of mine: %lums\n\n", diff.count());

  printf("Djistra's Algo:\n");
  start = std::chrono::system_clock::now();
  Djistras_Algo(num_lines_to_read, as_travel_grid);
  end = std::chrono::system_clock::now();
  diff = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
  printf("execution of djistra's: %lums\n\n", diff.count());

  free(stdin_test);

	return 0;
}