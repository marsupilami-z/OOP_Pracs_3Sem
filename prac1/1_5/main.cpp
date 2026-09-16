#include <iostream>
#include <string>
#include <iterator>
#include <stdexcept>

using std::cout;
using std::string;

template <class T>
class Element
{
private:
  Element* next;
  Element* prev;
  T field;

public:
  Element(T value = T(), Element* next_ptr = nullptr, Element* prev_ptr = nullptr)
  {
    field = value;
    next = next_ptr;
    prev = prev_ptr;
  }

  virtual Element* getNext() const { return next; }
  virtual void setNext(Element* value) { next = value; }

  virtual Element* getPrevious() const { return prev; }
  virtual void setPrevious(Element* value) { prev = value; }

  virtual T getValue() const { return field; }
  virtual void setValue(T value) { field = value; }

  template <class U>
  friend std::ostream& operator<<(std::ostream& ustream, Element<U>& obj);
};

template <class T>
std::ostream& operator<<(std::ostream& ustream, Element<T>& obj)
{
  ustream << obj.field;
  return ustream;
}

template <typename T>
class ListIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;

  ListIterator() { ptr = nullptr; }
  ListIterator(Element<T>* p) { ptr = p; }
  ListIterator(const ListIterator& it) { ptr = it.ptr; }

  bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
  bool operator==(ListIterator const& other) const { return ptr == other.ptr; }

  Element<T>& operator*()
  {
    if (ptr == nullptr)
      throw std::runtime_error("Iterator is not bound to any element");
    return *ptr;
  }

  ListIterator& operator++() { ptr = ptr->getNext(); return *this; }
  ListIterator& operator--() { ptr = ptr->getPrevious(); return *this; }

  ListIterator operator++(int)
  {
    ListIterator tmp(*this);
    ptr = ptr->getNext();
    return tmp;
  }

  ListIterator operator--(int)
  {
    ListIterator tmp(*this);
    ptr = ptr->getPrevious();
    return tmp;
  }

  ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
  ListIterator& operator=(Element<T>* p) { ptr = p; return *this; }

  Element<T>* getPtr() const { return ptr; }

private:
  Element<T>* ptr;
};

template <class T>
class LinkedListParent
{
protected:
  Element<T>* head;
  Element<T>* tail;
  int num;

public:
  LinkedListParent()
  {
    head = nullptr;
    tail = nullptr;
    num = 0;
  }

  virtual int Number() const { return num; }

  virtual ListIterator<T> begin() = 0;
  virtual ListIterator<T> end() = 0;

  virtual ListIterator<T> insert(ListIterator<T> pos, const T& value) = 0;
  virtual ListIterator<T> erase(ListIterator<T> pos) = 0;

  virtual ListIterator<T> push(const T& value) = 0;
  virtual T pop() = 0;

  virtual ~LinkedListParent() {}
};

template <class T>
class IteratedLinkedList : public LinkedListParent<T>
{
public:
  IteratedLinkedList() : LinkedListParent<T>() {}

  virtual ~IteratedLinkedList()
  {
    Element<T>* current = this->head;
    while (current != nullptr)
    {
      Element<T>* next = current->getNext();
      delete current;
      current = next;
    }
  }

  ListIterator<T> begin() override { return ListIterator<T>(this->head); }
  ListIterator<T> end()   override { return ListIterator<T>(nullptr); }

  ListIterator<T> rbegin() { return ListIterator<T>(this->tail); }
  ListIterator<T> rend()   { return ListIterator<T>(nullptr); }

  ListIterator<T> insert(ListIterator<T> pos, const T& value) override
  {
    Element<T>* cur  = pos.getPtr();
    Element<T>* prev = (cur != nullptr) ? cur->getPrevious() : this->tail;

    Element<T>* elem = new Element<T>(value, cur, prev);

    if (prev != nullptr)
      prev->setNext(elem);
    else
      this->head = elem;

    if (cur != nullptr)
      cur->setPrevious(elem);
    else
      this->tail = elem;

    this->num++;
    return ListIterator<T>(elem);
  }

  ListIterator<T> erase(ListIterator<T> pos) override
  {
    Element<T>* cur = pos.getPtr();
    if (cur == nullptr)
      return this->end();

    Element<T>* prev = cur->getPrevious();
    Element<T>* next = cur->getNext();

    if (prev != nullptr) prev->setNext(next);
    else                 this->head = next;

    if (next != nullptr) next->setPrevious(prev);
    else                 this->tail = prev;

    delete cur;
    this->num--;
    return ListIterator<T>(next);
  }
};

template <class T>
class Stack : public IteratedLinkedList<T>
{
public:
  Stack() : IteratedLinkedList<T>() {}

  ListIterator<T> push(const T& value) override
  {
    return this->insert(this->end(), value);
  }

  T pop() override
  {
      if (this->begin() == this->end())
          return T();
          
      ListIterator<T> it = this->rbegin();
      T value = (*it).getValue();
      this->erase(it);
      return value;
  }
};

template <class T>
class SortedStack : protected Stack<T>
{
public:
  SortedStack() : Stack<T>() {}

  ListIterator<T> begin()  { return Stack<T>::begin(); }
  ListIterator<T> end()    { return Stack<T>::end(); }
  ListIterator<T> rbegin() { return Stack<T>::rbegin(); }
  ListIterator<T> rend()   { return Stack<T>::rend(); }
  int Number() const       { return Stack<T>::Number(); }

  ListIterator<T> push(const T& value) override
  {
    ListIterator<T> it = this->begin();
    while (it != this->end() && (*it).getValue() < value)
      ++it;

    return this->insert(it, value);
  }

  T pop() override { return Stack<T>::pop(); }

  T pop_front()
  {
    if (this->begin() == this->end())
      return T();

    ListIterator<T> it = this->begin();
    T value = (*it).getValue();
    this->erase(it);

    return value;
  }

  T pop_back()
  {
    if (this->begin() == this->end())
      return T();

    ListIterator<T> it = this->rbegin();
    T value = (*it).getValue();
    this->erase(it);
    
    return value;
  }

  bool pop(const T& value)
  {
    for (ListIterator<T> it = this->begin(); it != this->end(); ++it)
    {
      if ((*it).getValue() == value)
      {
        this->erase(it);
        return true;
      }
    }
    return false;
  }
};

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

template <class Container>
void print_stack(Container& stack)
{
  for (auto it = stack.begin(); it != stack.end(); ++it)
    cout << (*it).getValue() << "\n";
  cout << "\n";
}

int main()
{
  SortedStack<Camera> cameras;

  cameras.push(Camera("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900));
  cameras.push(Camera("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000));
  cameras.push(Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800));
  cameras.push(Camera("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200));
  cameras.push(Camera("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700));
  cameras.push(Camera("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300));
  cameras.push(Camera("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1500));

  print_stack(cameras);

  cout << " pop \n";
  while (cameras.Number() > 0)
  {
    cout << cameras.pop_back() << "\n";
  }

  return 0;
}