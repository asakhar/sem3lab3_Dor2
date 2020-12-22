#include "TimingChartDynamic.hpp"

void exit(Chart& sel) { exit(0); }

void print_chart(Chart& chart) { std::cout << chart; }
void input_chart(Chart& chart)
{
  std::cout << "Input signal sections for chart:\n\
  number of section - first line;\n\
  sections - then (format <state|0, 1 or X> <time|initeger>): \n";
  std::cin >> chart;
}
void add_chart(Chart& chart)
{
  std::cout << "Input second chart to add (e.g. 00001110101XXX): ";
  std::string inp;
  std::cin >> inp;
  Chart other;
  try
  {
    other = Chart(inp.c_str());
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
  try
  {
    std::cout << "Resolved chart: \n" << chart + other;
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
}

void replace_chart_part(Chart& chart)
{
  std::cout << "Input timestamp to replace: ";
  int ts;
  std::cin >> ts;
  std::cout << "Input second chart to replace with (e.g. 00001110101XXX): ";
  std::string inp;
  std::cin >> inp;
  Chart other;
  try
  {
    other = Chart(inp.c_str());
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
  try
  {
    std::cout << "Resolved chart: \n" << chart(ts, other);
  }
  catch (std::out_of_range& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
}

void repeat_chart(Chart& chart)
{
  std::cout << "Input n (repetition number): ";
  size_t n;
  std::cin >> n;
  try
  {
    std::cout << "Resolved chart: \n" << (chart *= n);
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
}

void lshift_chart(Chart& chart)
{
  std::cout << "Input shifting time: ";
  int ts;
  std::cin >> ts;
  try
  {
    std::cout << "Resolved chart: \n" << (chart <<= ts);
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
}
void rshift_chart(Chart& chart)
{
  std::cout << "Input shifting time: ";
  long ts;
  std::cin >> ts;
  try
  {
    std::cout << "Resolved chart: \n" << (chart >>= ts);
  }
  catch (std::runtime_error& e)
  {
    std::cerr << e.what() << std::endl;
    return;
  }
}

void get_block(Chart& chart)
{
  std::cout << "Input block number (1-" << chart.size() << "): ";
  long num;
  std::cin >> num;
  try
  {
    std::cout << "Got signal block: " << chart.at(num-1);
  }
  catch (std::out_of_range& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

void (*options[])(Chart&) = {exit,         print_chart,  input_chart,  add_chart, replace_chart_part,
                             repeat_chart, lshift_chart, rshift_chart, get_block};

int main()
{
  Chart chart;
  while (1)
  {
    std::cout << "Ask... \n\
    [0]Exit\n\
    [1]Print chart\n\
    [2]Input chart\n\
    [3]Add chart to other\n\
    [4]Replace spec timestamp of chart with other chart\n\
    [5]Repeat chart n times\n\
    [6]Shift chart to the left\n\
    [7]Shift chart to the right\n\
    [8]Get block\n"
                 ">>";
    int choice;
    std::cin >> choice;
    if (choice > sizeof(options) / sizeof(void*) - 1 || choice < 0)
    {
      std::cout << "Try again!\n";
      continue;
    }
    std::cin.get();
    options[choice](chart);
    std::cout << std::endl;
  }
}