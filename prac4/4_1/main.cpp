#include <iostream>
#include <string>

using std::cout;
using std::string;

template <class K, class V>
class Node
{
public:
  K key;
  V data;

  int priority;

  Node* left;
  Node* right;
  Node* parent;

  Node(K k = K(), V d = V(), int p = 0)
  {
    key = k;
    data = d;
    priority = p;

    left = nullptr;
    right = nullptr;
    parent = nullptr;
  }
};

template <class K, class V>
class TreapIterator
{
private:
  Node<K, V>* current;

public:
  TreapIterator(Node<K, V>* n = nullptr) : current(n) {}

  V& operator*() { return current->data; }
  Node<K, V>* operator->() { return current; }

  bool operator==(const TreapIterator& other) const { return current == other.current; }
  bool operator!=(const TreapIterator& other) const { return current != other.current; }

  TreapIterator& operator++()
  {
    if (current == nullptr) return *this;

    if (current->right != nullptr)
    {
      current = current->right;
      while (current->left != nullptr)
        current = current->left;
    }
    else
    {
      Node<K, V>* p = current->parent;
      while (p != nullptr && current == p->right)
      {
        current = p;
        p = p->parent;
      }
      current = p;
    }
    return *this;
  }
};

template <class K, class V>
class Treap
{
protected:
  Node<K, V>* root;

  void setParent(Node<K, V>* n, Node<K, V>* p)
  {
    if (n) n->parent = p;
  }

  Node<K, V>* merge(Node<K, V>* L, Node<K, V>* R)
  {
    if (L == nullptr) return R;
    if (R == nullptr) return L;

    if (L->priority > R->priority)
    {
      L->right = merge(L->right, R);
      setParent(L->right, L);
      setParent(L, nullptr);
      return L;
    }
    else
    {
      R->left = merge(L, R->left);
      setParent(R->left, R);
      setParent(R, nullptr);
      return R;
    }
  }

  void split(Node<K, V>* n, K key, Node<K, V>*& L, Node<K, V>*& R)
  {
    if (n == nullptr)
    {
      L = nullptr;
      R = nullptr;
      return;
    }

    if (n->key < key)
    {
      split(n->right, key, n->right, R);
      setParent(n->right, n);
      L = n;
      setParent(L, nullptr);
    }
    else
    {
      split(n->left, key, L, n->left);
      setParent(n->left, n);
      R = n;
      setParent(R, nullptr);
    }
  }

  Node<K, V>* minNode(Node<K, V>* n)
  {
    if (n == nullptr) return nullptr;
    while (n->left != nullptr) n = n->left;
    return n;
  }

  Node<K, V>* maxNode(Node<K, V>* n)
  {
    if (n == nullptr) return nullptr;
    while (n->right != nullptr) n = n->right;
    return n;
  }

  void destroy(Node<K, V>* n)
  {
    if (!n) return;

    destroy(n->left);
    destroy(n->right);
    delete n;
  }

public:
  Treap()
  {
    root = nullptr;
  }

  ~Treap()
  {
    destroy(root);
  }

  Node<K, V>* getRoot() { return root; }

  void Merge(Treap<K, V>& other)
  {
    root = merge(root, other.root);
    if (root) root->parent = nullptr;
    other.root = nullptr;
  }

  void Split(K key, Treap<K, V>& L, Treap<K, V>& R)
  {
    Node<K, V>* l = nullptr;
    Node<K, V>* r = nullptr;

    split(root, key, l, r);

    L.root = l;
    R.root = r;

    if (L.root) L.root->parent = nullptr;
    if (R.root) R.root->parent = nullptr;

    root = nullptr;
  }

  void insert(K key, V data, int priority)
  {
    Node<K, V>* n = new Node<K, V>(key, data, priority);

    Node<K, V>* l = nullptr;
    Node<K, V>* r = nullptr;

    split(root, key, l, r);

    Node<K, V>* lr = merge(l, n);
    root = merge(lr, r);

    if (root) root->parent = nullptr;
  }

  Node<K, V>* find(K key)
  {
    Node<K, V>* n = root;
    while (n != nullptr)
    {
      if (key < n->key) n = n->left;
      else if (key > n->key) n = n->right;
      else return n;
    }
    return nullptr;
  }

  Node<K, V>* Min() { return minNode(root); }
  Node<K, V>* Max() { return maxNode(root); }

  Node<K, V>* Min(Node<K, V>* sub) { return minNode(sub); }
  Node<K, V>* Max(Node<K, V>* sub) { return maxNode(sub); }

  Node<K, V>* successor(Node<K, V>* n)
  {
    if (n == nullptr) return nullptr;

    if (n->right != nullptr) return minNode(n->right);

    Node<K, V>* p = n->parent;
    while (p != nullptr && n == p->right)
    {
      n = p;
      p = p->parent;
    }
    return p;
  }

  TreapIterator<K, V> begin() { return TreapIterator<K, V>(Min()); }
  TreapIterator<K, V> end()   { return TreapIterator<K, V>(nullptr); }

  void InOrder(Node<K, V>* n, void (*f)(Node<K, V>*))
  {
    if (n == nullptr) return;
    InOrder(n->left, f);
    f(n);
    InOrder(n->right, f);
  }
};

template <class K, class V>
void print(Node<K, V>* n)
{
  cout << "Key: " << n->key << ", value: " << n->data << ", priority: " << n->priority << "\n";
}

int main()
{
  Treap<int, string> t;

  t.insert(50, "fifty",   10);
  t.insert(30, "thirty",  20);
  t.insert(70, "seventy",  5);
  t.insert(20, "twenty",  25);
  t.insert(40, "forty",   15);
  t.insert(60, "sixty",   12);
  t.insert(80, "eighty",   3);

  cout << "InOrder (via function):\n";
  t.InOrder(t.getRoot(), print);

  cout << "\nInOrder (via iterator):\n";
  for (TreapIterator<int, string> it = t.begin(); it != t.end(); ++it)
    cout << "Key: " << it->key
         << ", value: " << *it
         << ", priority: " << it->priority << "\n";

  cout << "\n";
  Node<int, string>* mn = t.Min();
  Node<int, string>* mx = t.Max();
  if (mn) cout << "Min: " << mn->key << "\n";
  if (mx) cout << "Max: " << mx->key << "\n";

  cout << "\n";
  Node<int, string>* n40 = t.find(40);
  Node<int, string>* succ = t.successor(n40);
  if (succ) cout << "Successor of 40: " << succ->key << "\n";

  cout << "\nSplit at 50:\n";
  Treap<int, string> L, R;
  t.Split(50, L, R);

  cout << "L (min -> split):\n";
  for (TreapIterator<int, string> it = L.begin(); it != L.end(); ++it)
    cout << "Key: " << it->key << ", value: " << *it << "\n";

  cout << "R (split -> max):\n";
  for (TreapIterator<int, string> it = R.begin(); it != R.end(); ++it)
    cout << "Key: " << it->key << ", value: " << *it << "\n";

  cout << "\nL.Merge(R):\n";
  L.Merge(R);
  for (TreapIterator<int, string> it = L.begin(); it != L.end(); ++it)
    cout << "Key: " << it->key << ", value: " << *it << "\n";

  return 0;
}