#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

template <class T, class K
        , class Container = vector<pair<T,K>>
        , class Compare = less<typename Container::value_type>>
class Priority_Queue_Wrapper {
public:
  Priority_Queue_Wrapper(){}
  Priority_Queue_Wrapper(const Compare& c) : priority_queue_(c) {}

  void push(K key_mapped_to, T sort_queue_val) {
    last_populated_entry_[key_mapped_to] = sort_queue_val;
    priority_queue_.push({sort_queue_val, key_mapped_to});
  }
  
  bool empty() {
    return last_populated_entry_.empty();
  }
  
  pair<T, K> top() {
    try {
      while(true) {
        auto top_pq_pair = priority_queue_.top();
        // Remove "Expired" values from Priority Queue
        if(last_populated_entry_.find(top_pq_pair.second) == last_populated_entry_.end() ||
          top_pq_pair.first != last_populated_entry_[top_pq_pair.second]) {
          // printf("top] top value does not match last entry: %d\n", top_pq_pair.first);
          priority_queue_.pop();        
        } else {
          return top_pq_pair;
        }
      }
    } catch (...) {
      cout << "top] Likely tried to top empty priority queue" << endl;
    }
    return pair<T,K>();
  }
  
  void pop() {
    auto get_valid_top = top();
    priority_queue_.pop();

    // printf("pop] look in map for: %c\n", get_valid_top.second);
    if(auto found = last_populated_entry_.find(get_valid_top.second);
        found != last_populated_entry_.end()) {
      last_populated_entry_.erase(found);
    } else {
      cout << "pop] Map unexpectedly does not have valid_top_entry" << endl;
    }
  }

private:
  priority_queue<pair<T, K>, Container, Compare> priority_queue_;
  unordered_map<K, T> last_populated_entry_;
};

void TestPQWrapper() {
  // Max Priority Queue:
  // Priority_Queue_Wrapper<int, char> count_of_char;
  
  // Min Priority Queue:
  // Priority_Queue_Wrapper<int, char, vector<pair<int,char>>,
  //     std::greater<pair<int,char>>> count_of_char;

  // Attempt at custom compare:
  auto custom_compare = [](pair<int, char> left, pair<int,char> right) {
    return left.first > right.first;
  };
  Priority_Queue_Wrapper<int, char
      , vector<pair<int,char>>, decltype(custom_compare)
      > count_of_char(custom_compare);

  count_of_char.push('a', 3);
  count_of_char.push('b', 8);
  count_of_char.push('c', 5);
  count_of_char.push('a', 10);
  count_of_char.push('b', 20);
  count_of_char.push('a', 2);
  count_of_char.push('c', 7);
  
  // Print Results
  while(!count_of_char.empty()){
    auto pair = count_of_char.top();
    count_of_char.pop();
    printf("%c: %d\n", pair.second, pair.first);
  }

}

int main() {
  TestPQWrapper();
  cout << "Finished Execution!" << endl;
  return 0;
}
