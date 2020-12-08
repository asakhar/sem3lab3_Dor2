#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Signal
{
  char state;
  int time;
  Signal(char _state = 'X', int _time = 0);
  std::istream& input(std::istream& st);
  std::ostream& output(std::ostream& st) const;
};

class Chart
{
public:
  static size_t constexpr __n = 20;

private:
  Signal sections[__n];
  size_t csize;

public:
  inline size_t size() const { return csize; }
  Chart();
  Chart(char _state);
  Chart(char const* ascii_symbs);
  std::istream& input(std::istream& st);
  std::ostream& output(std::ostream& st) const;
  Chart add(Chart const& second) const;
  int get_total_time() const;
  void insertSignalBlock(Signal&& sig);
  Chart& replace(int timestamp, Chart const& second);
  Chart& repeat(size_t n);
  Chart& rshift(int tshift);
  Chart& lshift(int tshift);
  Chart& mergeBlocks();
};