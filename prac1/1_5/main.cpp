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

template <class T>
class LinkedListParent
{
protected:
  Element<T>* head;
  Element<T>* tail;
  int num;

public:
  virtual int Number() const { return num; }
  virtual Element<T>* getBegin() const { return head; }
  virtual Element<T>* getEnd() const { return tail; }

  LinkedListParent()
  {
    head = nullptr;
    tail = nullptr;
    num = 0;
  }

  virtual Element<T>* push(T value) = 0;
  virtual T pop() = 0;

  virtual ~LinkedListParent() {}
};

template <typename ValueType>
class ListIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t;
  using pointer = ValueType*;
  using reference = ValueType&;

  ListIterator() { ptr = nullptr; }
  ListIterator(Element<ValueType>* p) { ptr = p; }
  ListIterator(const ListIterator& it) { ptr = it.ptr; }

  bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
  bool operator==(ListIterator const& other) const { return ptr == other.ptr; }

  Element<ValueType>& operator*()
  {
    if (ptr == nullptr)
      throw std::runtime_error("Iterator is not bound to any element");
    return *ptr;
  }

  ListIterator& operator++() { ptr = ptr->getNext(); return *this; }
  ListIterator& operator++(int) { ptr = ptr->getNext(); return *this; }

  ListIterator& operator--() { ptr = ptr->getPrevious(); return *this; }
  ListIterator& operator--(int) { ptr = ptr->getPrevious(); return *this; }

  ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
  ListIterator& operator=(Element<ValueType>* p) { ptr = p; return *this; }

private:
  Element<ValueType>* ptr;
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

  ListIterator<T> begin()
  {
    ListIterator<T> it = this->head;
    return it;
  }

  ListIterator<T> end()
  {
    ListIterator<T> it = nullptr;
    return it;
  }
};

template <class T>
class Stack : public IteratedLinkedList<T>
{
public:
  Stack() : IteratedLinkedList<T>() {}

  Element<T>* push(T value) override
  {
    Element<T>* elem = new Element<T>(value, nullptr, this->tail);

    if (this->tail != nullptr)
      this->tail->setNext(elem);
    else
      this->head = elem;

    this->tail = elem;
    this->num++;
    return elem;
  }

  T pop() override
  {
    if (this->tail == nullptr)
      return T();

    T value = this->tail->getValue();
    Element<T>* prev = this->tail->getPrevious();
    delete this->tail;

    this->tail = prev;

    if (prev != nullptr)
      prev->setNext(nullptr);
    else
      this->head = nullptr;

    this->num--;
    return value;
  }
};

template <class T>
class SortedStack : public Stack<T>
{
public:
  SortedStack() : Stack<T>() {}

  Element<T>* push(T value) override
  {
    Element<T>* elem = new Element<T>(value, nullptr, nullptr);

    if (this->head == nullptr)
    {
      this->head = elem;
      this->tail = elem;
      this->num++;
      return elem;
    }

    ListIterator<T> it = this->begin();
    Element<T>* current = nullptr;

    while (it != this->end() && (*it).getValue() < value)
    {
      ++it;
    }

    if (it == this->end())
    {
      elem->setPrevious(this->tail);
      this->tail->setNext(elem);
      this->tail = elem;
    }
    else
    {
      current = &(*it);
      Element<T>* prev = current->getPrevious();

      elem->setNext(current);
      elem->setPrevious(prev);

      if (prev != nullptr)
        prev->setNext(elem);
      else
        this->head = elem;

      current->setPrevious(elem);
    }

    this->num++;
    return elem;
  }

  T pop() override
  {
    if (this->head == nullptr)
      return T();

    ListIterator<T> it = this->begin();
    T value = (*it).getValue();

    Element<T>* next = this->head->getNext();
    delete this->head;
    this->head = next;

    if (next != nullptr)
      next->setPrevious(nullptr);
    else
      this->tail = nullptr;

    this->num--;
    return value;
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

template <class T>
void print_stack(SortedStack<T>& stack)
{
  for (ListIterator<T> it = stack.begin(); it != stack.end(); ++it)
  {
    cout << *it << "\n";
  }
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
    cout << cameras.pop() << "\n";
  }

  return 0;
}