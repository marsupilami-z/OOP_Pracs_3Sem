#include <iostream>
#include <iterator>

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

template <typename T>
class ListIterator
{
public:
  ListIterator() { ptr = nullptr; }
  ListIterator(Element<T>* p) { ptr = p; }
  ListIterator(const ListIterator& it) { ptr = it.ptr; }

  bool operator!=(ListIterator const& other) const { return ptr != other.ptr; }
  bool operator==(ListIterator const& other) const { return ptr == other.ptr; }

  Element<T>& operator*() { return *ptr; }

  ListIterator& operator++() { ptr = ptr->getNext(); return *this; }
  ListIterator& operator--() { ptr = ptr->getPrevious(); return *this; }

  ListIterator& operator=(const ListIterator& it) { ptr = it.ptr; return *this; }
  ListIterator& operator=(Element<T>* p) { ptr = p; return *this; }

private:
  Element<T>* ptr;
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
    return ListIterator<T>(nullptr);
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
class SortedStack : protected Stack<T>
{
public:
  SortedStack() : Stack<T>() {}

  ListIterator<T> begin() { return Stack<T>::begin(); }
  ListIterator<T> end()   { return Stack<T>::end(); }
  int Number() const      { return Stack<T>::Number(); }
  T pop()                 { return Stack<T>::pop(); }

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
    while (it != this->end() && (*it).getValue() < value)
      ++it;

    if (it == this->end())
    {
      elem->setPrevious(this->tail);
      this->tail->setNext(elem);
      this->tail = elem;
    }
    else
    {
      Element<T>& cur  = *it;
      Element<T>* prev = cur.getPrevious();

      elem->setNext(&cur);
      elem->setPrevious(prev);

      if (prev != nullptr)
        prev->setNext(elem);
      else
        this->head = elem; 
        
      cur.setPrevious(elem);
    }

    this->num++;
    return elem;
  }
};

template <class Result, class In, class P>
Result filter(In& lst, P pred)
{
  Result result;
  for (auto it = lst.begin(); it != lst.end(); ++it)
    if (pred((*it).getValue()))
      result.push((*it).getValue());
  return result;
}

template <class T>
void print_stack(T& stack)
{
  for (auto it = stack.begin(); it != stack.end(); ++it)
    cout << (*it).getValue() << " ";
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

  cout << "pop: " << s.pop() << "\n";
  print_stack(s);

  SortedStack<int> ss;
  ss.push(3);
  ss.push(1);
  ss.push(4);
  ss.push(1);
  ss.push(5);
  print_stack(ss);

  Stack<int> evens = filter<Stack<int>>(ss, is_even);
  print_stack(evens);

  return 0;
}