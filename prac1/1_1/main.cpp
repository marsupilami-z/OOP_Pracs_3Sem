//===============
//  Вариант 28
//===============

#include <iostream>
#include <list>
#include <cmath>

using std::list;
using std::cout;

// Сама структура
struct Point2D
{
  int x, y;

  Point2D(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}

  double radius() const
  {
    return sqrt(x * x + y * y);
  }

  bool operator<(const Point2D& other) const
  {
    return radius() < other.radius();
  }

  bool operator==(const Point2D& other) const
  {
    return x == other.x && y == other.y;
  }
};

// Функции
template <class T>
void push(list<T> &lst, const T &elem) // Сортировака по возрастанию
{
  typename list<T>::iterator iter{lst.begin()};

  while (iter != lst.end() && *iter < elem)
  {
    ++iter;
  }

  lst.insert(iter, elem);
}

template <class T>
void pop_front(list<T> &lst)
{
  if (lst.empty())
    return;
  lst.pop_front();
}

template <class T>
void pop_back(list<T> &lst)
{
  if (lst.empty())
    return;
  lst.pop_back();
}

template <class T>
void pop(list<T> &lst, const T& value)
{
  typename list<T>::iterator iter{lst.begin()};

  while (iter != lst.end())
  {
    if (*iter == value)
    {
      lst.erase(iter);
      return;
    }
    ++iter;
  }
}

template <class T, class P>
list<T> filter(const list<T> &lst, P pred)
{
  list<T> result;

  for (const auto &i : lst)
  {
    if (pred(i))
    {
      push(result, i);
    }
  }

  return result;
}

// main
std::ostream& operator<<(std::ostream& os, const Point2D& p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

bool predicat_test(const Point2D& p)
{
    return (p.x * p.x + p.y * p.y) < 1.0;
}

template <class T>
void print_list(const list<T>& lst, const std::string& title = "")
{
    if (!title.empty()) cout << title << ":\n  ";
    for (auto it = lst.begin(); it != lst.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << "\n";
}

int main()
{
    list<Point2D> points;
    push(points, Point2D(3, 4));
    push(points, Point2D(0, 0));
    push(points, Point2D(1, 0));
    push(points, Point2D(2, 2));
    print_list(points, "После push");

    list<Point2D> inside = filter(points, predicat_test);
    print_list(inside, "После filter");

    pop_front(points);
    print_list(points, "После pop_front");

    pop_back(points);
    print_list(points, "После pop_back");

    pop(points, Point2D(1, 0));
    print_list(points, "После pop(1,0)");

    return 0;
}