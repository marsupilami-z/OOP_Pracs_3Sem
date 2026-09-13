#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <stdexcept>

using std::cout;
using std::string;
using std::map;
using std::set;
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
    if (producer != other.producer) return producer < other.producer;
    if (model != other.model) return model < other.model;
    if (type != other.type) return type < other.type;
    if (memory_card != other.memory_card) return memory_card < other.memory_card;
    if (megapixels != other.megapixels) return megapixels < other.megapixels;
    if (sensor_size != other.sensor_size) return sensor_size < other.sensor_size;
    if (weight != other.weight) return weight < other.weight;
    return price < other.price;
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

string make_key(const Camera& c)
{
  return c.producer + " " + c.model;     
}

void insert_camera(map<string, Camera>& m, const Camera& c)
{
  string key = make_key(c);

  if (m.find(key) != m.end())
    throw std::runtime_error("Key already exists: " + key);

  m[key] = c;
}

map<string, Camera>::iterator find_by_key(map<string, Camera>& m, const string& key)
{
  return m.find(key);
}

vector<map<string, Camera>::iterator> find_by_value(map<string, Camera>& m, const Camera& value)
{
  vector<map<string, Camera>::iterator> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    if (it->second.producer == value.producer
        && it->second.model == value.model
        && it->second.megapixels == value.megapixels)
    {
      result.push_back(it);
    }
  }

  return result;
}

template <class P>
map<string, Camera> filter(const map<string, Camera>& m, P pred)
{
  map<string, Camera> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    if (pred(it->second))
      result[it->first] = it->second;
  }

  return result;
}

vector<Camera> distinct_values(map<string, Camera>& m)
{
  set<Camera> seen;
  vector<Camera> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    if (seen.find(it->second) == seen.end())
    {
      seen.insert(it->second);
      result.push_back(it->second);
    }
  }

  return result;
}

void print_map(map<string, Camera>& m)
{
  for (auto it = m.begin(); it != m.end(); ++it)
  {
    cout << "Key: " << it->first << ", value: " << it->second << "\n";
  }
  cout << "\n";
}

int main()
{
  map<string, Camera> cameras;

  insert_camera(cameras, Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  insert_camera(cameras, Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  insert_camera(cameras, Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  insert_camera(cameras, Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  insert_camera(cameras, Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  insert_camera(cameras, Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));

  print_map(cameras);

  auto found = find_by_key(cameras, "Sony A7 III");
  if (found != cameras.end())
    cout << "Found by key: " << found->second << "\n\n";

  Camera probe("Canon", "EOS R5", "", 0, 45);
  auto found_v = find_by_value(cameras, probe);
  cout << "Found by value: " << found_v.size() << " element(s)\n\n";

  int threshold = 30;
  auto pred = [threshold](const Camera& c) { return c.megapixels > threshold; };
  map<string, Camera> filtered = filter(cameras, pred);
  cout << "Filtered (megapixels > " << threshold << "):\n";
  print_map(filtered);

  vector<Camera> distinct = distinct_values(cameras);
  cout << "Distinct values: " << distinct.size() << "\n";
  for (auto& c : distinct)
    cout << "  " << c << "\n";
  cout << "\n";

  try
  {
    insert_camera(cameras, Camera("Sony", "A7 III", "Mirrorless", 35.6, 24, 650, "SD", 1800));
  }
  catch (std::runtime_error& e)
  {
    cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}