#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Signal
{
  char state;
  int time;
  Signal(char _state = 'X', int _time = 0);
  friend std::istream& operator>>(std::istream& st, Signal& sig);
  friend std::ostream& operator<<(std::ostream& st, Signal const& sig);
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
  friend std::istream& operator>>(std::istream& st, Chart& chart);
  friend std::ostream& operator<<(std::ostream& st, Chart const& chart);
  Chart operator+(Chart const& second) const;
  int get_total_time() const;
  void insertSignalBlock(Signal&& sig);
  Chart& operator()(int timestamp, Chart const& second);
  Chart& operator*=(size_t n);
  Chart& operator>>=(int tshift);
  Chart& operator<<=(int tshift);
  Chart& mergeBlocks();

  // получить блок сигнала по индексу (с проверкой границ)
  Signal const& at(size_t n) const;
  // получить блок сигнала по индексу (без проверки границ)
  Signal const& operator[](size_t n) const;
};