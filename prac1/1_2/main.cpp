#include <iostream>
#include <list>
#include <string>

using std::list;
using std::string;
using std::cout;

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
    if (price != other.price)
      return price < other.price;

    if (weight != other.weight)
      return weight < other.weight;

    if (sensor_size != other.sensor_size)
      return sensor_size < other.sensor_size;

    return model > other.model;
  }

  bool operator==(const Camera& other) const
  {
    return producer == other.producer && model == other.model;
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
void push(list<T> &lst, const T &elem)
{
  typename list<T>::iterator iter{lst.begin()};

  while (iter != lst.end() && *iter < elem)
  {
    ++iter;
  }

  lst.insert(iter, elem);
}

template <class T>
bool pop(list<T> &lst, T &result)
{
  if (lst.empty())
    return false;

  result = lst.back();
  lst.pop_back();
  return true;
}

template <class T>
void print_list(const list<T>& lst)
{
  for (auto it = lst.begin(); it != lst.end(); ++it)
  {
    cout << "  " << *it << "\n";
  }
  cout << "\n";
}

int main()
{
  list<Camera> cameras;

  push(cameras, Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  push(cameras, Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  push(cameras, Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  push(cameras, Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  push(cameras, Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  push(cameras, Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));
  push(cameras, Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1500));

  print_list(cameras);

  Camera top;
  int n = 1;
  while (pop(cameras, top))
  {
    cout << n++ << ") " << top << "\n";
  }

  return 0;
}