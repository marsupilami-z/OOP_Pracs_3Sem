#include <iostream>
#include <string>

using std::cout;
using std::string;

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
class Node
{
private:
  T value;

public:
  T getValue() { return value; }
  void setValue(T v) { value = v; }

  bool operator<(Node N) { return value < N.getValue(); }
  bool operator>(Node N) { return value > N.getValue(); }
};

template <class T>
void print(Node<T>* N)
{
  cout << N->getValue() << "\n";
}

template <class T>
class Heap
{
private:
  Node<T>* arr;
  int len;
  int size;

public:
  int getCapacity() { return size; }
  int getCount() { return len; }

  Node<T>& operator[](int index) { return arr[index]; }

  Heap(int MemorySize = 100)
  {
    arr = new Node<T>[MemorySize];
    len = 0;
    size = MemorySize;
  }

  ~Heap()
  {
    delete[] arr;
  }

  void Swap(int index1, int index2)
  {
    Node<T> temp = arr[index1];
    arr[index1] = arr[index2];
    arr[index2] = temp;
  }

  void Copy(Node<T>* dest, Node<T>* source)
  {
    dest->setValue(source->getValue());
  }

  int GetLeftChildIndex(int index) { return index * 2 + 1; }
  int GetRightChildIndex(int index) { return index * 2 + 2; }

  int GetParentIndex(int index)
  {
    if (index % 2 == 0) return index / 2 - 1;

    return index / 2;
  }

  void SiftUp(int index = -1)
  {
    if (index == -1) index = len - 1;
    if (index <= 0) return;

    int parent = GetParentIndex(index);

    if (arr[index] > arr[parent])
    {
      Swap(index, parent);
      SiftUp(parent);
    }
  }

  void SiftDown(int index = 0)
  {
    int left = GetLeftChildIndex(index);
    int right = GetRightChildIndex(index);
    int largest = index;

    if (left < len && arr[left] > arr[largest]) largest = left;
    if (right < len && arr[right] > arr[largest]) largest = right;

    if (largest != index)
    {
      Swap(index, largest);
      SiftDown(largest);
    }
  }

  void push(Node<T>* N)
  {
    if (len >= size) return;

    Copy(&arr[len], N);
    len++;
    SiftUp();
  }

  void push(T v)
  {
    Node<T>* N = new Node<T>;

    N->setValue(v);
    push(N);
    delete N;
  }

  T ExtractMax()
  {
    if (len == 0) return T();

    T max_value = arr[0].getValue();

    Swap(0, len - 1);
    len--;

    if (len > 0) SiftDown(0);

    return max_value;
  }

  bool RemoveAt(int index)
  {
    if (index < 0 || index >= len) return false;

    if (index == len - 1)
    {
      len--;
      return true;
    }

    Swap(index, len - 1);
    len--;

    if (index > 0 && arr[index] > arr[GetParentIndex(index)])
      SiftUp(index);
    else
      SiftDown(index);

    return true;
  }

  bool RemoveValue(T value)
  {
    for (int i = 0; i < len; i++)
    {
      if (arr[i].getValue() == value) return RemoveAt(i);
    }

    return false;
  }

  void Straight(void (*f)(Node<T>*))
  {
    for (int i = 0; i < len; i++)
      f(&arr[i]);
  }
};

int main()
{
  Heap<Camera> heap;

  heap.push(Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  heap.push(Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  heap.push(Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  heap.push(Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  heap.push(Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  heap.push(Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));
  heap.push(Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1500));

  cout << " Max n";
  while (heap.getCount() > 0)
  {
    cout << heap.ExtractMax() << "\n";
  }

  return 0;
}