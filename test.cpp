#include <gtest/gtest.h>
#include <sstream>
#include "TimingChartOperators.hpp"

TEST (TimingChartTest, InputsFromStream){
  std::stringstream ss;
  ss << R"(3
X 4
0 3
1 6
)";
  Chart a;
  ss >> a;
  ASSERT_EQ(a.size(), 3);
  ASSERT_EQ(a[0].state, 'X');
  ASSERT_EQ(a[1].state, '0');
  ASSERT_EQ(a[2].state, '1');
  ASSERT_EQ(a[0].time, 4);
  ASSERT_EQ(a[1].time, 3);
  ASSERT_EQ(a[2].time, 6);
}

TEST (TimingChartTest, OutputsToStream){
  std::stringstream ss;
  Chart a{"1111100XX1"};
  ss << a;
  ASSERT_EQ(a.size(), 4);
  ASSERT_STREQ(ss.str().c_str(), "1111100XX1");
}
TEST (TimingChartTest, ConcatsTwoDiagrams){
  std::stringstream ss;
  Chart a{"1111100XX1"}, b{"111XXX00X"}, c = a + b;
  ss << c;
  ASSERT_EQ(c.size(), 7); // merge check
  ASSERT_STREQ(ss.str().c_str(), "1111100XX1111XXX00X");
}