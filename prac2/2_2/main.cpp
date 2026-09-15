#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

using std::cout;
using std::string;
using std::multimap;
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

  bool operator==(const Camera& other) const
  {
    return producer == other.producer
        && model == other.model
        && type == other.type
        && memory_card == other.memory_card
        && megapixels == other.megapixels
        && sensor_size == other.sensor_size
        && weight == other.weight
        && price == other.price;
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

template <class K, class V>
void insert_element(multimap<K, V>& m,
                    const typename multimap<K, V>::key_type& key,
                    const V& value)
{
  m.insert({key, value});
}

template <class K, class V>
vector<typename multimap<K, V>::iterator>
find_all_by_key(multimap<K, V>& m,
                const typename multimap<K, V>::key_type& key)
{
  vector<typename multimap<K, V>::iterator> result;
  auto range = m.equal_range(key);

  for (auto it = range.first; it != range.second; ++it)
    result.push_back(it);

  return result;
}

template <class K, class V, class Pred>
vector<typename multimap<K, V>::iterator>
find_by_value(multimap<K, V>& m, Pred pred)
{
  vector<typename multimap<K, V>::iterator> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    if (pred(it->second))
      result.push_back(it);
  }

  return result;
}

template <class K, class V, class P>
multimap<K, V> filter(const multimap<K, V>& m, P pred)
{
  multimap<K, V> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    if (pred(it->second))
      result.insert({it->first, it->second});
  }

  return result;
}

template <class K, class V>
vector<V> distinct_values(const multimap<K, V>& m)
{
  set<V> seen;
  vector<V> result;

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

template <class K, class V>
void print_multimap(const multimap<K, V>& m)
{
  for (auto it = m.begin(); it != m.end(); ++it)
  {
    cout << "Key: " << it->first << ", value: " << it->second << "\n";
  }
  cout << "\n";
}

int main()
{
  multimap<string, Camera> cameras;

  insert_element(cameras, "Canon EOS R5",  Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  insert_element(cameras, "Sony A7 III",   Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  insert_element(cameras, "Sony A7 III",   Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1500));
  insert_element(cameras, "Nikon Z6",      Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  insert_element(cameras, "Canon EOS 90D", Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  insert_element(cameras, "Fujifilm X-T4", Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  insert_element(cameras, "Panasonic G9",  Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));

  print_multimap(cameras);

  cout << "find_all_by_key\n";
  auto by_key = find_all_by_key(cameras, "Sony A7 III");
  for (auto it : by_key)
    cout << it->second << "\n";
  cout << "\n";

  auto found_v = find_by_value(cameras, [](const Camera& c) {
    return c.megapixels == 45;
  });
  cout << "Found by value " << found_v.size() << " element(s)\n\n";

  int threshold = 30;
  auto pred = [threshold](const Camera& c) { return c.megapixels > threshold; };
  multimap<string, Camera> filtered = filter(cameras, pred);
  cout << "Filtered (megapixels > " << threshold << "):\n";
  print_multimap(filtered);

  vector<Camera> distinct = distinct_values(cameras);
  cout << "Distinct values: " << distinct.size() << "\n";
  for (auto& c : distinct)
    cout << "  " << c.sensor_size << ", " << c.megapixels << "MP\n";
  cout << "\n";

  return 0;
}