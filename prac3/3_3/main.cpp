#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <cmath>

using std::cout;
using std::string;
using std::list;
using std::vector;

struct Camera
{
  string producer;
  string model;
  string type;
  string memory_card;

  int megapixels;

  double sensor_size;
  double weight;
  double price;

  Camera(string producer_ = "", string model_ = "", string type_ = "",
         double sensor_size_ = 0, int megapixels_ = 0, double weight_ = 0,
         string memory_card_ = "", double price_ = 0)
    : producer(producer_), model(model_), type(type_), sensor_size(sensor_size_),
      megapixels(megapixels_), weight(weight_), memory_card(memory_card_), price(price_) {}

  bool operator<(const Camera& other) const
  {
    if (price != other.price) return price < other.price;
    if (weight != other.weight) return weight < other.weight;
    if (sensor_size != other.sensor_size) return sensor_size < other.sensor_size;

    return model > other.model;
  }

  bool operator>(const Camera& other) const
  {
    return other < *this;
  }
};

std::ostream& operator<<(std::ostream& os, const Camera& c)
{
  os << "[" << c.producer << " " << c.model
     << ", " << c.type
     << ", sensor=" << c.sensor_size
     << ", " << c.megapixels << "MP"
     << ", " << c.weight << "g"
     << ", " << c.memory_card
     << ", " << c.price << "$]";

  return os;
}

template <class T>
class FibNode
{
public:
  T value;
  int degree;

  FibNode* parent;
  list<FibNode*> children;

  FibNode(T v = T())
  {
    value = v;
    degree = 0;
    parent = nullptr;
  }
};

template <class T>
class FibonacciHeap
{
private:
  list<FibNode<T>*> roots;
  FibNode<T>* maxNode;
  int count;

  void link(FibNode<T>* child, FibNode<T>* parent)
  {
    child->parent = parent;
    parent->children.push_back(child);
    parent->degree++;
  }

  void Consolidate()
  {
    if (roots.empty()) return;

    int size = 8;
    int est = (int)(std::log2(roots.size() + 1) * 2) + 2;
    if (est > size) size = est;

    vector<FibNode<T>*> table(size, nullptr);

    list<FibNode<T>*> oldRoots = roots;
    roots.clear();

    for (auto n : oldRoots)
    {
      FibNode<T>* x = n;
      int d = x->degree;

      while (d < size && table[d] != nullptr)
      {
        FibNode<T>* y = table[d];

        if (y->value > x->value)
        {
          FibNode<T>* temp = x;
          x = y;
          y = temp;
        }

        link(y, x);
        table[d] = nullptr;
        d++;
      }

      if (d < size)
        table[d] = x;
    }

    maxNode = nullptr;

    for (int i = 0; i < size; i++)
    {
      if (table[i] != nullptr)
      {
        table[i]->parent = nullptr;
        roots.push_back(table[i]);

        if (maxNode == nullptr || table[i]->value > maxNode->value)
          maxNode = table[i];
      }
    }
  }

public:
  FibonacciHeap()
  {
    maxNode = nullptr;
    count = 0;
  }

  ~FibonacciHeap()
  {
    for (auto n : roots)
      destroy(n);
  }

  void destroy(FibNode<T>* n)
  {
    for (auto c : n->children)
      destroy(c);

    delete n;
  }

  bool isEmpty() { return roots.empty(); }

  int getCount() { return count; }

  void insert(T value)
  {
    FibNode<T>* n = new FibNode<T>(value);
    roots.push_back(n);

    if (maxNode == nullptr || n->value > maxNode->value)
      maxNode = n;

    count++;
  }

  T ExtractMax()
  {
    if (roots.empty()) return T();

    FibNode<T>* m = maxNode;
    T result = m->value;

    for (auto it = roots.begin(); it != roots.end(); ++it)
    {
      if (*it == m)
      {
        roots.erase(it);
        break;
      }
    }

    for (auto child : m->children)
    {
      child->parent = nullptr;
      roots.push_back(child);
    }

    m->children.clear();
    delete m;
    count--;

    if (!roots.empty())
      Consolidate();
    else
      maxNode = nullptr;

    return result;
  }
};

int main()
{
  FibonacciHeap<Camera> heap;

  heap.insert(Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  heap.insert(Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  heap.insert(Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  heap.insert(Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  heap.insert(Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  heap.insert(Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));
  heap.insert(Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1500));

  cout << "Max \n";
  while (!heap.isEmpty())
  {
    cout << heap.ExtractMax() << "\n";
  }

  return 0;
}