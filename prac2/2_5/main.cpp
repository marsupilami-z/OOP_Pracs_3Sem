#include <iostream>
#include <string>

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
      current = current->getNext();

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
    key = k;
    data = d;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    height = 1;
  }

  void setKey(K k) { key = k; }
  K getKey() { return key; }

  void setData(V d) { data = d; }
  V getData() { return data; }

  int getHeight() { return height; }
  void setHeight(int h) { height = h; }

  Node* getLeft() { return left; }
  void setLeft(Node* n) { left = n; }

  Node* getRight() { return right; }
  void setRight(Node* n) { right = n; }

  Node* getParent() { return parent; }
  void setParent(Node* n) { parent = n; }
};

template <class K, class V>
class Tree
{
protected:
  Node<K, V>* root;

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
    Node<K, V>* x = y->getLeft();
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
    Node<K, V>* y = x->getRight();
    Node<K, V>* T2 = y->getLeft();

    y->setLeft(x);
    x->setRight(T2);

    if (T2) T2->setParent(x);

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

    if (bf > 1 && balanceFactor(n->getLeft()) >= 0) return rotateRight(n);

    if (bf > 1 && balanceFactor(n->getLeft()) < 0)
    {
      n->setLeft(rotateLeft(n->getLeft()));
      return rotateRight(n);
    }

    if (bf < -1 && balanceFactor(n->getRight()) <= 0) return rotateLeft(n);

    if (bf < -1 && balanceFactor(n->getRight()) > 0)
    {
      n->setRight(rotateRight(n->getRight()));
      return rotateLeft(n);
    }

    return n;
  }

public:
  Node<K, V>* getRoot() { return root; }

  Tree() { root = nullptr; }

  virtual Node<K, V>* Add_R(Node<K, V>* N) { return Add_R(N, root); }

  virtual Node<K, V>* Add_R(Node<K, V>* N, Node<K, V>* Current)
  {
    if (N == nullptr) return nullptr;

    if (root == nullptr)
    {
      root = N;
      return N;
    }

    if (Current->getKey() > N->getKey())
    {
      if (Current->getLeft() != nullptr)
        Current->setLeft(Add_R(N, Current->getLeft()));
      else
        Current->setLeft(N);

      Current->getLeft()->setParent(Current);
    }
    else if (Current->getKey() < N->getKey())
    {
      if (Current->getRight() != nullptr)
        Current->setRight(Add_R(N, Current->getRight()));
      else
        Current->setRight(N);

      Current->getRight()->setParent(Current);
    }
    else
    {
      return Current;
    }

    return balance(Current);
  }

  virtual void Add(K key, V data)
  {
    Node<K, V>* N = new Node<K, V>(key, data);
    root = Add_R(N, root);

    if (root) root->setParent(nullptr);
  }

  void destroy(Node<K, V>* n)
  {
    if (!n) return;

    destroy(n->getLeft());
    destroy(n->getRight());

    delete n;
  }

  ~Tree() { destroy(root); }
};

template <class K, class V>
class MultiTree : public Tree<K, V>
{
protected:
  Node<K, V>* Add_Multi(Node<K, V>* N, Node<K, V>* Current)
  {
    if (N == nullptr) return nullptr;

    if (this->root == nullptr)
    {
      this->root = N;
      return N;
    }

    if (N->getKey() < Current->getKey())
    {
      if (Current->getLeft() != nullptr)
        Current->setLeft(Add_Multi(N, Current->getLeft()));
      else
        Current->setLeft(N);

      Current->getLeft()->setParent(Current);
    }
    else
    {
      if (Current->getRight() != nullptr)
        Current->setRight(Add_Multi(N, Current->getRight()));
      else
        Current->setRight(N);

      Current->getRight()->setParent(Current);
    }

    return this->balance(Current);
  }

  void collect(Node<K, V>* n, const K& key, SortedStack<V>& result)
  {
    if (!n) return;

    collect(n->getLeft(), key, result);

    if (n->getKey() == key)
      result.push(n->getData());

    collect(n->getRight(), key, result);
  }

public:
  MultiTree() : Tree<K, V>() {}

  Node<K, V>* Add_R(Node<K, V>* N) override
  {
    return Add_Multi(N, this->root);
  }

  void Add(K key, V data) override
  {
    Node<K, V>* N = new Node<K, V>(key, data);
    this->root = Add_Multi(N, this->root);

    if (this->root) this->root->setParent(nullptr);
  }

  SortedStack<V> operator[](const K& key)
  {
    SortedStack<V> result;
    collect(this->root, key, result);

    return result;
  }
};

string make_key(const Camera& c)
{
  return c.producer + " " + c.model;
}

template <class T>
void print_stack(SortedStack<T>& stack)
{
  Element<T>* current = stack.getBegin();

  while (current != nullptr)
  {
    cout << "  " << current->getValue() << "\n";
    current = current->getNext();
  }
}

int main()
{
  MultiTree<string, Camera> tree;

  Camera c1("Canon", "EOS R5",   "Mirrorless", 35.9, 45, 738, "CFexpress", 3900);
  Camera c2("Sony",  "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1800);
  Camera c3("Sony",  "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1500);
  Camera c4("Nikon", "Z6",       "Mirrorless", 35.9, 24, 675, "XQD",       2000);
  Camera c5("Canon", "EOS 90D",  "DSLR",       22.3, 32, 701, "SD",        1200);
  Camera c6("Sony",  "A7 III",   "Mirrorless", 35.6, 24, 650, "SD",        1700);

  tree.Add(make_key(c1), c1);
  tree.Add(make_key(c2), c2);
  tree.Add(make_key(c3), c3);
  tree.Add(make_key(c4), c4);
  tree.Add(make_key(c5), c5);
  tree.Add(make_key(c6), c6);

  cout << "===== Sony A7 III =====\n";
  SortedStack<Camera> sony = tree["Sony A7 III"];
  print_stack(sony);

  cout << "===== Canon EOS R5 =====\n";
  SortedStack<Camera> canon = tree["Canon EOS R5"];
  print_stack(canon);

  cout << "===== NonExistent =====\n";
  SortedStack<Camera> none = tree["Some Unknown Camera"];
  cout << "  count: " << none.Number() << "\n";

  return 0;
}