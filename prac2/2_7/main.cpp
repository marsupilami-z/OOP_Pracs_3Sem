#include <iostream>
#include <string>
#include <stdexcept>

using std::cout;
using std::string;

template <class K, class V>
class HashTable
{
public:
  struct Entry
  {
    bool used;
    bool deleted;

    K key;
    V data;

    Entry() : used(false), deleted(false), key(), data() {}
  };

protected:
  Entry* table;

  int capacity;
  int count;

  size_t hashFunction(const string& s) const
  {
    const size_t p = 61;
    const size_t m = 100000009ULL; // ull - unsigned long long

    size_t h = 0;

    for (size_t i = 0; i < s.size(); i++)
      h = (h * p + (unsigned char)s[i]) % m;

    return h;
  }

  size_t hashFunction(int n) const
  {
    unsigned int u = (unsigned int)n;
    size_t h = 0;

    for (int i = 0; i < (int)sizeof(int); i++)
    {
      h += (u & 0xFF);
      u >>= 8;
    }

    return h;
  }

  int handleCollision(size_t h, const K& key, bool forInsert, bool& exists, bool (*eq)(K, K) = nullptr) const
  {
    int idx = (int)(h % capacity);
    int start = idx;
    int firstDeleted = -1;

    do
    {
      if (table[idx].used)
      {
        bool equal;

        if (eq != nullptr) 
          equal = eq(table[idx].key, key);
        else               
          equal = (table[idx].key == key);

        if (equal)
        {
          exists = true;
          return idx;
        }
      }

      if (!table[idx].used && !table[idx].deleted)
      {
        exists = false;

        if (!forInsert) return -1;

        return (firstDeleted != -1) ? firstDeleted : idx;
      }

      if (forInsert && !table[idx].used && table[idx].deleted && firstDeleted == -1)
        firstDeleted = idx;

      idx = (idx + 1) % capacity;
    } while (idx != start);

    exists = false;
    return forInsert ? firstDeleted : -1;
  }

  int findIndex(const K& key, bool (*eq)(K, K) = nullptr) const
  {
    bool exists;

    return handleCollision(hashFunction(key), key, false, exists, eq);
  }

  int insertIndex(const K& key, bool& exists) const
  {
    return handleCollision(hashFunction(key), key, true, exists);
  }

public:
  HashTable(int cap = 10007) : capacity(cap), count(0)
  {
    table = new Entry[capacity];
  }

  ~HashTable()
  {
    delete[] table;
  }

  int Number() const { return count; }
  int Capacity() const { return capacity; }

  class Iterator
  {
  private:
    HashTable* t;

    int idx;
    int dir;

  public:
    Iterator(HashTable* tp, int i, int d) : t(tp), idx(i), dir(d) {}
    Iterator() : t(nullptr), idx(0), dir(1) {}

    bool operator==(const Iterator& o) const { return idx == o.idx; }
    bool operator!=(const Iterator& o) const { return idx != o.idx; }

    Entry& operator*()  { return t->table[idx]; }
    Entry* operator->() { return &t->table[idx]; }

    Iterator& operator++()
    {
      int i = idx + dir;

      while (i >= 0 && i < t->capacity && !t->table[i].used)
        i += dir;

      idx = i;
      return *this;
    }
  };

  Iterator begin()
  {
    int i = 0;

    while (i < capacity && !table[i].used) 
      i++;

    return Iterator(this, i, +1);
  }

  Iterator end() { return Iterator(this, capacity, +1); }

  Iterator rbegin()
  {
    int i = capacity - 1;

    while (i >= 0 && !table[i].used) 
      i--;

    return Iterator(this, i, -1);
  }

  Iterator rend() { return Iterator(this, -1, -1); }

  Iterator getRoot() { return begin(); }

  Iterator Add(K key, V data)
  {
    bool exists;
    int idx = insertIndex(key, exists);

    if (idx == -1)
      throw std::runtime_error("Hash table is full");

    if (exists)
      return Iterator(this, idx, +1);

    table[idx].used = true;
    table[idx].deleted = false;

    table[idx].key = key;
    table[idx].data = data;

    count++;

    return Iterator(this, idx, +1);
  }

  Iterator Find(K key, bool (*eq)(K, K) = nullptr)
  {
    int idx = findIndex(key, eq);

    if (idx == -1) return end();

    return Iterator(this, idx, +1);
  }

  Iterator remove(K key)
  {
    int idx = findIndex(key);

    if (idx == -1) return end();

    table[idx].used = false;
    table[idx].deleted = true;

    count--;

    Iterator it(this, idx, +1);
    ++it;

    return it;
  }

  V& operator[](K key)
  {
    bool exists;
    int idx = insertIndex(key, exists);

    if (idx == -1)
      throw std::runtime_error("Hash table is full");

    if (exists)
      return table[idx].data;

    table[idx].used = true;
    table[idx].deleted = false;

    table[idx].key = key;
    count++;

    return table[idx].data;
  }

  Iterator Min()
  {
    int best = -1;

    for (int i = 0; i < capacity; i++)
      if (table[i].used && (best == -1 || table[i].key < table[best].key))
        best = i;

    if (best == -1) return end();

    return Iterator(this, best, +1);
  }

  Iterator Max()
  {
    int best = -1;

    for (int i = 0; i < capacity; i++)
      if (table[i].used && (best == -1 || table[best].key < table[i].key))
        best = i;

    if (best == -1) return end();

    return Iterator(this, best, +1);
  }
};

struct Camera
{
  string producer, model, type, memory_card;

  int megapixels;
  double sensor_size, weight, price;

  Camera(string p = "", string m = "", string t = "",
         double s = 0, int mp = 0, double w = 0,
         string mc = "", double pr = 0)
    : producer(p), model(m), type(t), sensor_size(s),
      megapixels(mp), weight(w), memory_card(mc), price(pr) {}
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

int main()
{
  HashTable<string, Camera> ht;

  ht.Add("Canon EOS R5",   Camera("Canon",    "EOS R5",   "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  ht.Add("Nikon Z6",       Camera("Nikon",    "Z6",       "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  ht.Add("Sony A7 III",    Camera("Sony",     "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1800));
  ht.Add("Canon EOS 90D",  Camera("Canon",    "EOS 90D",  "DSLR",       22.3, 32, 701, "SD",        1200));
  ht.Add("Fujifilm X-T4",  Camera("Fujifilm", "X-T4",     "Mirrorless", 23.5, 26, 607, "SD",        1700));
  ht.Add("Panasonic G9",   Camera("Panasonic","Lumix G9", "Mirrorless", 17.3, 20, 658, "SD",        1300));

  cout << "Size = " << ht.Number() << "\n\n";

  cout << "Iteration (forward):\n";
  for (HashTable<string, Camera>::Iterator it = ht.begin(); it != ht.end(); ++it)
    cout << "  Key: " << it->key << ", value: " << it->data << "\n";

  cout << "\nIteration (reverse):\n";
  for (HashTable<string, Camera>::Iterator it = ht.rbegin(); it != ht.rend(); ++it)
    cout << "  Key: " << it->key << ", value: " << it->data << "\n";

  cout << "\nFind(\"Nikon Z6\"):\n";
  HashTable<string, Camera>::Iterator f = ht.Find("Nikon Z6");
  if (f != ht.end())
    cout << "  " << f->key << " -> " << f->data << "\n";

  cout << "\nAdd returns iterator: ";
  HashTable<string, Camera>::Iterator added =
    ht.Add("Leica Q2", Camera("Leica", "Q2", "Compact", 36.0, 47, 718, "SD", 5000));
  if (added != ht.end())
    cout << added->key << " -> " << added->data << "\n";

  cout << "\nMin key: " << ht.Min()->key << "\n";
  cout << "Max key: " << ht.Max()->key << "\n";

  cout << "\nremove(\"Nikon Z6\"):\n";
  HashTable<string, Camera>::Iterator next = ht.remove("Nikon Z6");
  cout << "  size = " << ht.Number() << "\n";
  if (next != ht.end())
    cout << "  next after removed: " << next->key << "\n";

  cout << "\nFind(\"Sony A7 III\") after removal:\n";
  HashTable<string, Camera>::Iterator f2 = ht.Find("Sony A7 III");
  if (f2 != ht.end())
    cout << "  " << f2->key << " -> " << f2->data << "\n";

  cout << "\nAfter remove:\n";
  for (HashTable<string, Camera>::Iterator it = ht.begin(); it != ht.end(); ++it)
    cout << "  " << it->key << " -> " << it->data << "\n";

  cout << "\noperator[] test:\n";
  ht["Sony A7 III"] = Camera("Sony", "A7 III", "Mirrorless", 35.6, 24, 650, "SD", 1500);
  cout << "  " << ht.Find("Sony A7 III")->data << "\n";

  cout << "\nHashTable<int, string> test:\n";
  HashTable<int, string> hti;
  hti.Add(10, "ten");
  hti.Add(20, "twenty");
  hti.Add(30, "thirty");

  for (HashTable<int, string>::Iterator it = hti.begin(); it != hti.end(); ++it)
    cout << "  " << it->key << " -> " << it->data << "\n";

  return 0;
}