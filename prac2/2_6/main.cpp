#include <iostream>
#include <string>
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
  { field = value; next = next_ptr; prev = prev_ptr; }

  Element* getNext() const { return next; }
  void setNext(Element* value) { next = value; }

  Element* getPrevious() const { return prev; }
  void setPrevious(Element* value) { prev = value; }

  T getValue() const { return field; }
  void setValue(T value) { field = value; }
};

template <class T>
class LinkedListParent
{
protected:
  Element<T>* head;
  Element<T>* tail;
  int num;

public:
  int Number() const { return num; }

  Element<T>* getBegin() const { return head; }
  Element<T>* getEnd() const { return tail; }

  LinkedListParent() { head = nullptr; tail = nullptr; num = 0; }

  virtual Element<T>* push(T value) = 0;
  virtual T pop() = 0;

  virtual ~LinkedListParent() {}
};

template <class T>
class ListIterator
{
private:
  Element<T>* ptr;

public:
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

  ListIterator<T> begin() { return ListIterator<T>(this->head); }
  ListIterator<T> end()   { return ListIterator<T>(nullptr); }
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
    if (this->tail == nullptr) return T();

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

      if (prev != nullptr) prev->setNext(elem);
      else this->head = elem;

      cur.setPrevious(elem);
    }

    this->num++;
    return elem;
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

  bool operator<(const Camera& o) const
  {
    if (price != o.price) return price < o.price;
    if (weight != o.weight) return weight < o.weight;
    if (sensor_size != o.sensor_size) return sensor_size < o.sensor_size;
    return model > o.model;
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
class Node
{
protected:
  K key;
  V data;

  Node* left;
  Node* right;
  Node* parent;

  int height;

public:
  Node(K k = K(), V d = V())
  {
    key = k; data = d;
    left = right = parent = nullptr;
    height = 1;
  }

  void setKey(K k) { key = k; }
  K    getKey()    { return key; }

  void setData(V d) { data = d; }
  V    getData()    { return data; }

  int  getHeight()      { return height; }
  void setHeight(int h) { height = h; }

  Node* getLeft()  { return left; }
  void  setLeft(Node* n)  { left = n; }

  Node* getRight() { return right; }
  void  setRight(Node* n) { right = n; }

  Node* getParent() { return parent; }
  void  setParent(Node* n) { parent = n; }
};

template <class K, class V>
class TreeIterator
{
private:
  Node<K, V>* current;
public:
  TreeIterator(Node<K, V>* n = nullptr) : current(n) {}

  Node<K, V>& operator*()  { return *current; }
  Node<K, V>* operator->() { return current; }

  bool operator==(const TreeIterator& o) const { return current == o.current; }
  bool operator!=(const TreeIterator& o) const { return current != o.current; }

  TreeIterator& operator++()
  {
    if (current == nullptr) return *this;

    if (current->getRight() != nullptr)
    {
      current = current->getRight();

      while (current->getLeft() != nullptr) 
        current = current->getLeft();
    }
    else
    {
      Node<K, V>* p = current->getParent();

      while (p != nullptr && current == p->getRight())
      { 
        current = p; 
        p = p->getParent(); 
      }

      current = p;
    }

    return *this;
  }

  TreeIterator& operator--()
  {
    if (current == nullptr) return *this;

    if (current->getLeft() != nullptr)
    {
      current = current->getLeft();

      while (current->getRight() != nullptr)
        current = current->getRight();
    }
    else
    {
      Node<K, V>* p = current->getParent();

      while (p != nullptr && current == p->getLeft())
      {
        current = p;
        p = p->getParent();
      }
      current = p;
    }

    return *this;
  }
};

template <class K, class V>
class SearchTree
{
protected:
  Node<K, V>* root;

  virtual Node<K, V>* Add_R(Node<K, V>* N, Node<K, V>* Current)
  {
    if (N == nullptr) return Current;
    if (Current == nullptr) return N;

    if (N->getKey() < Current->getKey())
    {
      Current->setLeft(Add_R(N, Current->getLeft()));

      if (Current->getLeft())
        Current->getLeft()->setParent(Current);
    }
    else if (N->getKey() > Current->getKey())
    {
      Current->setRight(Add_R(N, Current->getRight()));

      if (Current->getRight())
        Current->getRight()->setParent(Current);
    }
    else
      return Current;

    return Current;
  }

  Node<K, V>* Min(Node<K, V>* c)
  {
    if (c == nullptr) return nullptr;

    while (c->getLeft() != nullptr)
      c = c->getLeft();

    return c;
  }

  Node<K, V>* Max(Node<K, V>* c)
  {
    if (c == nullptr) return nullptr;

    while (c->getRight() != nullptr)
      c = c->getRight();

    return c;
  }

  void destroy(Node<K, V>* n)
  {
    if (!n) return;

    destroy(n->getLeft());
    destroy(n->getRight());
    delete n;
  }

public:
  SearchTree() { root = nullptr; }
  virtual ~SearchTree() { destroy(root); }

  TreeIterator<K, V> getRoot() { return TreeIterator<K, V>(root); }

  virtual TreeIterator<K, V> Add(K key, V data)
  {
    if (Find(key) != end()) return end();

    Node<K, V>* N = new Node<K, V>(key, data);
    root = Add_R(N, root);

    if (root)
      root->setParent(nullptr);

    return TreeIterator<K, V>(N);
  }

  TreeIterator<K, V> Find(K key, bool (*eq)(K, K) = nullptr)
  {
    Node<K, V>* n = root;

    while (n != nullptr)
    {
      bool equal;

      if (eq != nullptr)
        equal = eq(key, n->getKey());
      else
        equal = (key == n->getKey());

      if (equal) return TreeIterator<K, V>(n);

      if (key < n->getKey())
        n = n->getLeft();
      else
        n = n->getRight();
    }

    return end();
  }

  TreeIterator<K, V> Min()
  {
    return TreeIterator<K, V>(Min(root));
  }

  TreeIterator<K, V> Max()
  {
    return TreeIterator<K, V>(Max(root));
  }

  TreeIterator<K, V> begin()  { return Min(); }
  TreeIterator<K, V> end()    { return TreeIterator<K, V>(nullptr); }
  TreeIterator<K, V> rbegin() { return Max(); }
  TreeIterator<K, V> rend()   { return TreeIterator<K, V>(nullptr); }
};

template <class K, class V>
class AVLTree : public SearchTree<K, V>
{
protected:
  int height(Node<K, V>* n) { return n ? n->getHeight() : 0; }

  void updateHeight(Node<K, V>* n)
  {
    if (!n) return;

    int hl = height(n->getLeft());
    int hr = height(n->getRight());

    n->setHeight(1 + (hl > hr ? hl : hr));
  }

  int balanceFactor(Node<K, V>* n)
  {
    return n ? height(n->getLeft()) - height(n->getRight()) : 0;
  }

  Node<K, V>* rotateRight(Node<K, V>* y)
  {
    Node<K, V>* x  = y->getLeft();
    Node<K, V>* T2 = x->getRight();

    x->setRight(y);
    y->setLeft(T2);

    if (T2) T2->setParent(y);

    x->setParent(y->getParent());
    y->setParent(x);

    updateHeight(y);
    updateHeight(x);

    return x;
  }

  Node<K, V>* rotateLeft(Node<K, V>* x)
  {
    Node<K, V>* y  = x->getRight();
    Node<K, V>* T2 = y->getLeft();

    y->setLeft(x);
    x->setRight(T2);

    if (T2)
      T2->setParent(x);

    y->setParent(x->getParent());
    x->setParent(y);

    updateHeight(x);
    updateHeight(y);

    return y;
  }

  Node<K, V>* balance(Node<K, V>* n)
  {
    if (!n) return nullptr;

    updateHeight(n);
    int bf = balanceFactor(n);

    if (bf > 1 && balanceFactor(n->getLeft()) >= 0)
      return rotateRight(n);

    if (bf > 1 && balanceFactor(n->getLeft()) < 0)
    {
      n->setLeft(rotateLeft(n->getLeft()));
      return rotateRight(n);
    }

    if (bf < -1 && balanceFactor(n->getRight()) <= 0)
      return rotateLeft(n);

    if (bf < -1 && balanceFactor(n->getRight()) > 0)
    {
      n->setRight(rotateRight(n->getRight()));
      return rotateLeft(n);
    }

    return n;
  }

  Node<K, V>* Add_R(Node<K, V>* N, Node<K, V>* Current) override
  {
    if (N == nullptr) return Current;
    if (Current == nullptr) return N;

    if (N->getKey() < Current->getKey())
    {
      Current->setLeft(Add_R(N, Current->getLeft()));

      if (Current->getLeft())
        Current->getLeft()->setParent(Current);
    }
    else if (N->getKey() > Current->getKey())
    {
      Current->setRight(Add_R(N, Current->getRight()));

      if (Current->getRight())
        Current->getRight()->setParent(Current);
    }
    else
      return Current;

    return balance(Current);
  }

public:
  AVLTree() : SearchTree<K, V>() {}
};

template <class K, class V>
class MultiTree : public AVLTree<K, V>
{
protected:
  Node<K, V>* Add_Multi(Node<K, V>* N, Node<K, V>* Current)
  {
    if (N == nullptr) return Current;
    if (Current == nullptr) return N;

    if (N->getKey() < Current->getKey())
    {
      Current->setLeft(Add_Multi(N, Current->getLeft()));
      if (Current->getLeft()) Current->getLeft()->setParent(Current);
    }
    else
    {
      Current->setRight(Add_Multi(N, Current->getRight()));
      if (Current->getRight()) Current->getRight()->setParent(Current);
    }

    return this->balance(Current);
  }

public:
  MultiTree() : AVLTree<K, V>() {}

  TreeIterator<K, V> Add(K key, V data) override
  {
    Node<K, V>* N = new Node<K, V>(key, data);
    this->root = Add_Multi(N, this->root);

    if (this->root)
      this->root->setParent(nullptr);

    return TreeIterator<K, V>(N);
  }
};

template <class K, class V>
class SplayTree : public MultiTree<K, V>
{
protected:
  void splayToRoot(Node<K, V>* n)
  {
    while (n->getParent() != nullptr)
    {
      Node<K, V>* p = n->getParent();
      Node<K, V>* g = p->getParent();

      Node<K, V>* newSubRoot;

      if (n == p->getLeft())
        newSubRoot = this->rotateRight(p);
      else
        newSubRoot = this->rotateLeft(p);

      if (g == nullptr)
        this->root = newSubRoot;
      else if (g->getLeft() == p)
        g->setLeft(newSubRoot);
      else
        g->setRight(newSubRoot);
    }
  }

  void collectAll(Node<K, V>* n, const K& key, SortedStack<V>& result)
  {
    if (!n) return;

    collectAll(n->getLeft(), key, result);

    if (n->getKey() == key)
      result.push(n->getData());

    collectAll(n->getRight(), key, result);
  }

public:
  SplayTree() : MultiTree<K, V>() {}

  TreeIterator<K, V> Add(K key, V data) override
  {
    Node<K, V>* N = new Node<K, V>(key, data);

    if (this->root == nullptr)
    {
      this->root = N;
      return TreeIterator<K, V>(N);
    }

    Node<K, V>* cur = this->root;
    while (true)
    {
      if (key < cur->getKey())
      {
        if (cur->getLeft() == nullptr)
        {
          cur->setLeft(N);
          N->setParent(cur);
          break;
        }

        cur = cur->getLeft();
      }
      else
      {
        if (cur->getRight() == nullptr)
        {
          cur->setRight(N);
          N->setParent(cur);
          break;
        }

        cur = cur->getRight();
      }
    }

    splayToRoot(N);
    return TreeIterator<K, V>(N);
  }

  TreeIterator<K, V> find(K key, bool (*eq)(K, K) = nullptr)
  {
    Node<K, V>* n = this->root;
    Node<K, V>* found = nullptr;

    while (n != nullptr)
    {
      bool equal;

      if (eq != nullptr)
        equal = eq(key, n->getKey());
      else
        equal = (key == n->getKey());

      if (equal)
      {
        found = n;
        break;
      }

      if (key < n->getKey())
        n = n->getLeft();
      else
        n = n->getRight();
    }

    if (found != nullptr)
    {
      splayToRoot(found);
      return TreeIterator<K, V>(found);
    }
    return TreeIterator<K, V>(nullptr);
  }

  SortedStack<V> operator[](const K& key)
  {
    find(key);
    SortedStack<V> result;
    collectAll(this->root, key, result);

    return result;
  }
};

string make_key(const Camera& c)
{
  return c.producer + " " + c.model;
}

bool eq_keys(string a, string b) { return a == b; }

template <class T>
void print_stack(SortedStack<T>& stack)
{
  for (ListIterator<T> it = stack.begin(); it != stack.end(); ++it)
    cout << "  " << (*it).getValue() << "\n";
}

int main()
{
  SplayTree<string, Camera> tree;

  Camera c1("Canon",    "EOS R5",   "Mirrorless", 35.9, 45, 738, "CFexpress", 3900);
  Camera c2("Sony",     "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1800);
  Camera c3("Sony",     "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1500);
  Camera c4("Nikon",    "Z6",       "Mirrorless", 35.9, 24, 675, "XQD",       2000);
  Camera c5("Canon",    "EOS 90D",  "DSLR",       22.3, 32, 701, "SD",        1200);
  Camera c6("Sony",     "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1700);

  tree.Add(make_key(c1), c1);
  tree.Add(make_key(c2), c2);
  tree.Add(make_key(c3), c3);
  tree.Add(make_key(c4), c4);
  tree.Add(make_key(c5), c5);
  tree.Add(make_key(c6), c6);

  cout << "Root after Add (last inserted): "
       << tree.getRoot()->getKey() << "\n\n";

  cout << "find(\"Nikon Z6\", eq_keys):\n";
  TreeIterator<string, Camera> f = tree.find("Nikon Z6", eq_keys);

  if (f != tree.end())
    cout << "  " << f->getKey() << " -> " << f->getData() << "\n";
  cout << "Root now: " << tree.getRoot()->getKey() << "\n\n";

  cout << "tree[\"Sony A7 III\"]:\n";

  SortedStack<Camera> sony = tree["Sony A7 III"];
  print_stack(sony);
  cout << "Root after []: " << tree.getRoot()->getKey() << "\n\n";

  cout << "In-order (via iterators):\n";

  for (TreeIterator<string, Camera> it = tree.begin(); it != tree.end(); ++it)
    cout << "  " << it->getKey() << " -> " << it->getData() << "\n";

  return 0;
}