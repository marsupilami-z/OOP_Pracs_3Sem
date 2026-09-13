#include <iostream>
#include <string>
#include <stack>

using std::cout;
using std::string;
using std::stack;

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

  virtual void setKey(K k) { key = k; }
  virtual K getKey() { return key; }

  virtual void setData(V d) { data = d; }
  virtual V getData() { return data; }

  int getHeight() { return height; }
  void setHeight(int h) { height = h; }

  virtual Node* getLeft() { return left; }
  virtual void setLeft(Node* n) { left = n; }

  virtual Node* getRight() { return right; }
  virtual void setRight(Node* n) { right = n; }

  virtual Node* getParent() { return parent; }
  virtual void setParent(Node* n) { parent = n; }
};

template <class K, class V>
void print(Node<K, V>* N)
{
  cout << "Key: " << N->getKey() << ", value: " << N->getData() << "\n";
}

template <class K, class V>
class Tree
{
protected:
  Node<K, V>* root;

  int height(Node<K, V>* n)
  {
    return n ? n->getHeight() : 0;
  }

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

public:
  virtual Node<K, V>* getRoot() { return root; }

  Tree()
  {
    root = nullptr;
  }

  virtual Node<K, V>* Add_R(Node<K, V>* N)
  {
    return Add_R(N, root);
  }

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
      {
        Current->setLeft(Add_R(N, Current->getLeft()));
        Current->getLeft()->setParent(Current);
      }
      else
      {
        Current->setLeft(N);
        Current->getLeft()->setParent(Current);
      }
    }
    else if (Current->getKey() < N->getKey())
    {
      if (Current->getRight() != nullptr)
      {
        Current->setRight(Add_R(N, Current->getRight()));
        Current->getRight()->setParent(Current);
      }
      else
      {
        Current->setRight(N);
        Current->getRight()->setParent(Current);
      }
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

  virtual Node<K, V>* Min(Node<K, V>* Current = nullptr)
  {
    if (root == nullptr) return nullptr;
    if (Current == nullptr) Current = root;

    while (Current->getLeft() != nullptr)
      Current = Current->getLeft();

    return Current;
  }

  virtual Node<K, V>* Max(Node<K, V>* Current = nullptr)
  {
    if (root == nullptr) return nullptr;
    if (Current == nullptr) Current = root;

    while (Current->getRight() != nullptr)
      Current = Current->getRight();

    return Current;
  }

  virtual Node<K, V>* Find(K key, Node<K, V>* Current)
  {
    if (Current == nullptr) return nullptr;
    if (Current->getKey() == key) return Current;
    if (Current->getKey() > key) return Find(key, Current->getLeft());

    return Find(key, Current->getRight());
  }

  virtual void PreOrder(Node<K, V>* N, void (*f)(Node<K, V>*))
  {
    if (N == nullptr) return;

    f(N);
    PreOrder(N->getLeft(), f);
    PreOrder(N->getRight(), f);
  }

  virtual void InOrder(Node<K, V>* N, void (*f)(Node<K, V>*))
  {
    if (N == nullptr) return;

    InOrder(N->getLeft(), f);
    f(N);
    InOrder(N->getRight(), f);
  }

  virtual void PostOrder(Node<K, V>* N, void (*f)(Node<K, V>*))
  {
    if (N == nullptr) return;

    PostOrder(N->getLeft(), f);
    PostOrder(N->getRight(), f);
    f(N);
  }

  void destroy(Node<K, V>* n)
  {
    if (!n) return;
    
    destroy(n->getLeft());
    destroy(n->getRight());
    delete n;
  }

  ~Tree()
  {
    destroy(root);
  }
};

template <class K, class V>
class TreeIterator
{
private:
  stack<Node<K, V>*> st;
  bool reverse;

  void pushLeft(Node<K, V>* n)
  {
    while(n)
    {
      st.push(n);
      n = reverse ? n->getRight() : n->getLeft();
    }
  }

public:
  TreeIterator(Node<K, V>* root, bool rev = false) : reverse(rev)
  {
    pushLeft(root);
  }

  bool hasNext() { return !st.empty(); }

  Node<K, V>* next()
  {
    Node<K, V>* n = st.top();
    st.pop();
    pushLeft(reverse ? n->getLeft() : n->getRight());
    return n;
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

template <class K, class V, class P>
Node<K, V>* findInOrder(Node<K, V>* n, P pred)
{
  if (!n) return nullptr;
  Node<K, V>* left = findInOrder(n->getLeft(), pred);
  if (left) return left;
  if (pred(n->getData())) return n;
  return findInOrder(n->getRight(), pred);
}

int main()
{
  Tree<string, Camera> tree;

  Camera c1("Canon",    "EOS R5",    "Mirrorless", 35.9, 45, 738, "CFexpress", 3900);
  Camera c2("Nikon",    "Z6",        "Mirrorless", 35.9, 24, 675, "XQD",       2000);
  Camera c3("Sony",     "A7 III",    "Mirrorless", 35.6, 24, 650, "SD",        1800);
  Camera c4("Canon",    "EOS 90D",   "DSLR",       22.3, 32, 701, "SD",        1200);
  Camera c5("Fujifilm", "X-T4",      "Mirrorless", 23.5, 26, 607, "SD",        1700);
  Camera c6("Panasonic","Lumix G9",  "Mirrorless", 17.3, 20, 658, "SD",        1300);

  tree.Add(make_key(c1), c1);
  tree.Add(make_key(c2), c2);
  tree.Add(make_key(c3), c3);
  tree.Add(make_key(c4), c4);
  tree.Add(make_key(c5), c5);
  tree.Add(make_key(c6), c6);

  cout << "===== InOrder =====\n";
  TreeIterator<string, Camera> it(tree.getRoot(), false);
  while (it.hasNext())
  {
    Node<string, Camera>* n = it.next();
    cout << "Key: " << n->getKey() << ", value: " << n->getData() << "\n";
  }
  cout << "\n";

  cout << "===== итератор =====\n";
  TreeIterator<string, Camera> itr(tree.getRoot(), true);
  while (itr.hasNext())
  {
    Node<string, Camera>* n = itr.next();
    cout << "Key: " << n->getKey() << ", value: " << n->getData() << "\n";
  }
  cout << "\n";

  cout << "===== Find =====\n";
  Node<string, Camera>* found = tree.Find("Nikon Z6", tree.getRoot());
  if (found) cout << "Key: " << found->getKey() << ", value: " << found->getData() << "\n\n";

  cout << "===== find_by_value =====\n";
  auto pred = [](const Camera& c) { return c.model == "A7 III"; };
  Node<string, Camera>* fv = findInOrder(tree.getRoot(), pred);
  if (fv) cout << "Key: " << fv->getKey() << ", value: " << fv->getData() << "\n";

  return 0;
}