#include <gtest/gtest.h>
#include <sstream>
#include "hot_queue.h"
#include "priority_queue_stl.h"

template<typename T>
class ScalarPriority {
public:
  static bool less_priority(T n0, T n1) {
    return n0 > n1;
  }

  static T get_priority(T n0) {
    return n0;
  }

  static void set_priority(T &n0, T v) {
    n0 = v;
  }
};

template<typename Q>
const std::string stringify(const Q &q) {

  std::ostringstream ss;
  for (auto n : q.hot_.queue_ ) {
     ss << n <<",";
  }
  ss << "\n";

  for (auto c : q.cold_ ) {
    for (auto e : c) {
      ss << e << ",";
    }
    ss << "|";
  }
  return ss.str();
}

TEST(HotQueue, insert_remove_increase_int) {
  fudge::HotQueue<int, int, ScalarPriority<int>,
                  fudge::PriorityQueueSTL<int,int,ScalarPriority<int>>> q(4);
  q.insert(10);
  ASSERT_EQ("10,\n", stringify(q));

  q.insert(14);
  ASSERT_EQ("10,\n14,|", stringify(q));

  q.insert(2);
  ASSERT_EQ("2,10,\n14,|", stringify(q));

  q.insert(16);
  ASSERT_EQ("2,10,\n14,|16,|", stringify(q));

  q.insert(15);
  ASSERT_EQ("2,10,\n14,15,|16,|", stringify(q));

  // test increase priority from hot to hot.
  q.increase_priority(10, 1);
  ASSERT_EQ("1,2,\n14,15,|16,|", stringify(q));

  ASSERT_EQ(1, q.remove_front());
  ASSERT_EQ("2,\n14,15,|16,|", stringify(q));

  ASSERT_EQ(2, q.remove_front());
  ASSERT_EQ("\n14,15,|16,|", stringify(q));

  ASSERT_EQ(14, q.remove_front());
  ASSERT_EQ("15,\n16,|", stringify(q));

  q.insert(27);
  ASSERT_EQ("15,\n16,||27,|", stringify(q));

  // test increase priority from cold to hot.
  q.increase_priority(27, 3);
  ASSERT_EQ("3,15,\n16,|||", stringify(q));

  ASSERT_EQ(3, q.remove_front());
  ASSERT_EQ("15,\n16,|||", stringify(q));

  ASSERT_EQ(15, q.remove_front());
  ASSERT_EQ("\n16,|||", stringify(q));

  ASSERT_EQ(16, q.remove_front());
  ASSERT_EQ("\n||", stringify(q));

  q.insert(15);
  ASSERT_EQ("15,\n||", stringify(q));

  q.insert(28);
  ASSERT_EQ("15,\n||28,|", stringify(q));

  // test increase priority within cold.
  q.increase_priority(28, 27);
  ASSERT_EQ("15,\n|27,||", stringify(q));
}
