#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

// Структура для хранения данных о блоке сигнала
struct Signal
{
  // состояние сигнала на протяжении всего блока
  char state;

  // длина блока
  int time;

  // конструктор 
  // a_state - состояние сигнала (0, 1 или X)
  // a_time - длительность сигнала
  Signal(char a_state = 'X', int a_time = 0);
  
  // ввод сигнала из входного потока. Например std::cin
  std::istream& input(std::istream& st);
  
  // вывод сигнала в выводной поток. Например std::cout
  std::ostream& output(std::ostream& st) const;
};

// временная диаграмма
class Chart
{
public:
  // размер массива под структуры
  static size_t constexpr __n = 20;

private:
  // массив структур блоков сигнала
  Signal sections[__n];

  // текущее количество блоков сигнала в диаграмме
  size_t csize;

public:
  // получить размер диаграммы
  inline size_t size() const { return csize; }

  // конструктор по умолчанию
  Chart();

  // конструктор от состояния сигнала на макс время
  Chart(char _state);

  // конструктор из строки символов 01Х
  Chart(char const* ascii_symbs);

  // ввод диаграммы из входного потока
  std::istream& input(std::istream& st);

  // вывод диаграммы в выходной поток
  std::ostream& output(std::ostream& st) const;

  // добавить вторую диаграмму в конец текущей 
  // (не сохраняет результат в текущей диаграмме, а просто возвращает временный объект)
  Chart add(Chart const& second) const;

  // получить общее время сигнала
  int get_total_time() const;
  
  // вставить блок сигнала в конец
  void insertSignalBlock(Signal const& sig);
  
  // заменить единичный отрезок времени сигнала на блоки из другой диаграммы
  Chart& replace(int timestamp, Chart const& second);
  
  // повторить диаграмму n раз
  Chart& repeat(size_t n);
  
  // циклический сдвиг вправо
  Chart& rshift(int tshift);
  
  // циклический сдвиг влево
  Chart& lshift(int tshift);
  
  // объединить смежные блоки с одинаковым состоянием сигнала
  Chart& mergeBlocks();

  // получить блок сигнала по индексу
  Signal const& getBlock(size_t n) const;
};