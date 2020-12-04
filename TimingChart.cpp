#include "TimingChart.hpp"

Signal::Signal(char _state, int _time) : state{_state}, time{_time} {};

std::istream& Signal::input(std::istream& st)
{
  state = '\0';
  while (state != '0' && state != '1' && state != 'X')
  {
    st >> state;
  }
  st >> time;
  return st;
}

std::ostream& Signal::output(std::ostream& st) const
{
  for (int i = 0; i < time; i++)
    st << state;
  return st;
}

Chart::Chart() : csize{0} {}

Chart::Chart(char _state)
{
  for (csize = 0; csize < __n; csize++)
    sections[csize] = Signal(_state, INT32_MAX);
}

Chart::Chart(char const* ascii_symbs) : csize{0}
{
  size_t len  = std::strlen(ascii_symbs);
  int counter = 1;
  char last   = '\0';
  for (size_t i = 0; i < len; i++)
  {
    if (ascii_symbs[i] != '0' && ascii_symbs[i] != '1' && ascii_symbs[i] != 'X')
      throw std::runtime_error("Unexpected symbol.");
    if (last == ascii_symbs[i])
      counter++;
    else
    {
      if (counter && last != '\0')
      {
        if (csize == __n)
          throw std::runtime_error("Too many signals.");
        sections[csize++] = Signal(last, counter);
        counter           = 1;
      }
      last = ascii_symbs[i];
    }
  }
  if (counter)
  {
    if (csize == __n)
      throw std::runtime_error("Too many signals.");
    sections[csize++] = Signal(last, counter);
  }
}

std::istream& Chart::input(std::istream& st)
{
  for (size_t i = 0; i < csize; i++)
    sections[i].input(st);
  return st;
}

std::ostream& Chart::output(std::ostream& st) const
{
  for (size_t i = 0; i < csize; i++)
    sections[i].output(st);
  return st;
}

Chart Chart::add(Chart const& second) const
{
  if (size() + second.size() > __n)
    throw std::runtime_error("Charts were too long.");
  Chart result;

  for (size_t i = 0; i < size(); i++)
    result.sections[result.csize++] = sections[i];

  for (size_t i = 0; i < second.size(); i++)
    result.sections[result.csize++] = sections[i];

  result.mergeBlocks();

  return result;
}

Chart& Chart::replace(int timestamp, Chart const& second)
{
  Chart result;
  int time = 0;
  size_t i = 0;
  for (; i < size(); i++)
  {
    time += sections[i].time;
    if (time >= timestamp)
      break;
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time));
  }
  if (i == size())
    throw std::runtime_error("Timestamp arg was out of avaliable signal time range.");
  result.insertSignalBlock(Signal(sections[i].state, sections[i].time - time + timestamp - 1));

  for (size_t j = 0; j < second.size(); j++)
    result.insertSignalBlock(Signal(second.sections[j].state, second.sections[j].time));
  result.insertSignalBlock(Signal(sections[i].state, time - timestamp));
  i++;
  for (; i < size(); i++)
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time));
  result.mergeBlocks();
  return *this = std::move(result);
}

Chart& Chart::repeat(size_t n)
{
  if (size() * n > __n)
    throw std::runtime_error("Chart was too long to copy it n times.");
  for (size_t i = 1; i < n; i++)
    for (size_t j = 0; j < size(); j++)
      sections[i * size() + j] = sections[j];
  csize = size() * n;
  mergeBlocks();
  return *this;
}

Chart& Chart::rshift(int tshift)
{
  if (tshift == 0)
    return *this;
  if (tshift < 0)
    return lshift(-tshift);
  Chart result;
  int time = 0;
  tshift %= get_total_time();
  auto i = size() - 1;
  for (; i >= 0; i--)
  {
    time += sections[i].time;
    if (time >= tshift)
      break;
  }
  auto pos = i;
  if (sections[i].time - time + tshift)
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time - time + tshift));
  i++;
  for (; i < size(); i++)
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time));
  for (i = 0; i < pos; i++)
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time));
  if (time - tshift)
    result.insertSignalBlock(Signal(sections[i].state, time - tshift));
  result.mergeBlocks();
  return *this = std::move(result);
}
Chart& Chart::lshift(int tshift)
{
  if (tshift == 0)
    return *this;
  if (tshift < 0)
    return rshift(-tshift);
  Chart result;
  int time = 0;
  tshift %= get_total_time();
  auto i = 0;
  for (; i < size(); i++)
  {
    time += sections[i].time;
    if (time >= tshift)
      break;
  }
  auto pos = i;
  if (time - tshift)
    result.insertSignalBlock(Signal(sections[i].state, time - tshift));
  i++;
  for (; i < size(); i++)
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time));
  for (i = 0; i < pos; i++)
    result.insertSignalBlock(Signal(sections[i].state, sections[i].time));
  if (tshift - time + sections[i].time)
    result.insertSignalBlock(Signal(sections[i].state, tshift - time + sections[i].time));
  result.mergeBlocks();
  return *this = std::move(result);
}

int Chart::get_total_time() const
{
  int result = 0;
  for (size_t i = 0; i < size(); i++)
    result += sections[i].time;
  return result;
}

void Chart::insertSignalBlock(Signal&& sig)
{
  if (csize == __n)
    throw std::runtime_error("Cannot insert block.");
  sections[csize++] = sig;
}
void Chart::insertSignalBlock(Signal const& sig)
{
  if (csize == __n)
    throw std::runtime_error("Cannot insert block.");
  sections[csize++] = sig;
}

Chart& Chart::mergeBlocks()
{
  char last             = '\0';
  int block_time        = 1;
  size_t resulting_size = 0;
  for (size_t i = 0; i < size(); i++)
  {
    if (last == sections[i].state)
      block_time += sections[i].time;
    else
    {
      if (last != '\0')
      {
        sections[resulting_size++] = Signal(last, block_time);
        block_time                 = 1;
      }
      last = sections[i].state;
    }
  }
  sections[resulting_size++] = Signal(last, block_time);
  csize                      = resulting_size;
  return *this;
}