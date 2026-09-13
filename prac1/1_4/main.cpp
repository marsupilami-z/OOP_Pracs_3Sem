#include <iostream>
#include <iterator>
#include <stdexcept>

using std::cout;

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

    Element<T>* current = this->head;
    while (current != nullptr && current->getValue() < value)
    {
      current = current->getNext();
    }

    if (current == nullptr)
    {
      elem->setPrevious(this->tail);
      this->tail->setNext(elem);
      this->tail = elem;
    }
    else if (current == this->head)
    {
      elem->setNext(this->head);
      this->head->setPrevious(elem);
      this->head = elem;
    }
    else
    {
      Element<T>* prev = current->getPrevious();
      elem->setNext(current);
      elem->setPrevious(prev);
      prev->setNext(elem);
      current->setPrevious(elem);
    }

    this->num++;
    return elem;
  }
};

template <class T, class P>
Stack<T> filter(const Stack<T>& lst, P pred)
{
  Stack<T> result;
  Element<T>* current = lst.getBegin();
  while (current != nullptr)
  {
    if (pred(current->getValue()))
    {
      result.push(current->getValue());
    }
    current = current->getNext();
  }
  return result;
}

template <class T>
void print_stack(Stack<T>& stack)
{
  for (ListIterator<T> it = stack.begin(); it != stack.end(); ++it)
  {
    cout << *it << " ";
  }
  cout << "\n";
}

template <class T>
void print_stack_reverse(Stack<T>& stack)
{
  if (stack.getEnd() == nullptr)
    return;

  ListIterator<T> it = stack.getEnd();
  while (true)
  {
    cout << *it << " ";
    Element<T>* prev = (*it).getPrevious();
    if (prev == nullptr)
      break;
    it = prev;
  }
  cout << "\n";
}

bool is_even(int x)
{
  return x % 2 == 0;
}

int main()
{
  Stack<int> s;
  s.push(3);
  s.push(1);
  s.push(4);
  s.push(1);
  s.push(5);
  print_stack(s);
  print_stack_reverse(s);

  SortedStack<int> ss;
  ss.push(3);
  ss.push(1);
  ss.push(4);
  ss.push(1);
  ss.push(5);
  print_stack(ss);
  print_stack_reverse(ss);

  Stack<int> evens = filter(ss, is_even);
  print_stack(evens);

  try
  {
    ListIterator<int> bad;
    cout << *bad;
  }
  catch (std::runtime_error& e)
  {
    cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}