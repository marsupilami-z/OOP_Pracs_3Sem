#include <iostream>
#include <string>
#include <map>
#include <vector>

using std::cout;
using std::string;
using std::multimap;
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

void insert_camera(multimap<string, Camera>& m, const Camera& c)
{
  m.insert({make_key(c), c});
}

vector<multimap<string, Camera>::iterator> find_all_by_key(multimap<string, Camera>& m, const string& key)
{
  vector<multimap<string, Camera>::iterator> result;
  auto range = m.equal_range(key);

  for (auto it = range.first; it != range.second; ++it)
    result.push_back(it);

  return result;
}

vector<multimap<string, Camera>::iterator> find_by_value(multimap<string, Camera>& m, const Camera& value)
{
  vector<multimap<string, Camera>::iterator> result;

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
multimap<string, Camera> filter(const multimap<string, Camera>& m, P pred)
{
  multimap<string, Camera> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    if (pred(it->second))
      result.insert({it->first, it->second});
  }

  return result;
}

vector<Camera> distinct_values(multimap<string, Camera>& m)
{
  vector<Camera> result;

  for (auto it = m.begin(); it != m.end(); ++it)
  {
    bool found = false;
    for (auto& c : result)
    {
      if (c.sensor_size == it->second.sensor_size
          && c.megapixels == it->second.megapixels)
      {
        found = true;
        break;
      }
    }
    if (!found)
      result.push_back(it->second);
  }

  return result;
}

void print_multimap(multimap<string, Camera>& m)
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

  insert_camera(cameras, Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  insert_camera(cameras, Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  insert_camera(cameras, Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1500));
  insert_camera(cameras, Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  insert_camera(cameras, Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  insert_camera(cameras, Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  insert_camera(cameras, Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));

  print_multimap(cameras);

  cout << "===== find_all_by_key(Sony A7 III) =====\n";
  auto by_key = find_all_by_key(cameras, "Sony A7 III");
  for (auto it : by_key)
    cout << it->second << "\n";
  cout << "\n";

  Camera probe("Canon", "EOS R5", "", 0, 45);
  auto found_v = find_by_value(cameras, probe);
  cout << "Found by value: " << found_v.size() << " element(s)\n\n";

  int threshold = 30;
  auto pred = [threshold](const Camera& c) { return c.megapixels > threshold; };
  multimap<string, Camera> filtered = filter(cameras, pred);
  cout << "Filtered (megapixels > " << threshold << "):\n";
  print_multimap(filtered);

  vector<Camera> distinct = distinct_values(cameras);
  cout << "Distinct values (sensor_size, megapixels): " << distinct.size() << "\n";
  for (auto& c : distinct)
    cout << "  " << c.sensor_size << "\", " << c.megapixels << "MP\n";
  cout << "\n";

  return 0;
}