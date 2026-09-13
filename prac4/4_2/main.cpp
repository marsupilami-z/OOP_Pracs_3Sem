#include <iostream>
#include <vector>
#include <cstdlib>

using std::cout;
using std::vector;

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

  int size;
  V sum;
  bool rev;

  Node(K k = K(), V d = V(), int p = 0)
  {
    key = k;
    data = d;
    priority = p;

    left = nullptr;
    right = nullptr;
    parent = nullptr;

    size = 1;
    sum = d;
    rev = false;
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
  Treap() { root = nullptr; }
  virtual ~Treap() { destroy(root); }

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
      if (key < n->key) 
        n = n->left;

      else if (key > n->key) 
        n = n->right;

      else 
        return n;
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

  void InOrder(Node<K, V>* n, void (*f)(Node<K, V>*))
  {
    if (n == nullptr) return;
    InOrder(n->left, f);
    f(n);
    InOrder(n->right, f);
  }
};

template <class T>
class ImplicitTreap : protected Treap<int, T>
{
protected:
  int sz(Node<int, T>* n) { return n ? n->size : 0; }
  T sm(Node<int, T>* n) { return n ? n->sum : T(); }

  void update(Node<int, T>* n)
  {
    if (!n) return;

    n->size = 1 + sz(n->left) + sz(n->right);
    n->sum = n->data + sm(n->left) + sm(n->right);
  }

  void push(Node<int, T>* n)
  {
    if (n && n->rev)
    {
      Node<int, T>* t = n->left;
      n->left = n->right;
      n->right = t;

      if (n->left) n->left->rev = !n->left->rev;
      if (n->right) n->right->rev = !n->right->rev;

      n->rev = false;
    }
  }

  Node<int, T>* merge(Node<int, T>* L, Node<int, T>* R)
  {
    push(L);
    push(R);

    if (!L) return R;
    if (!R) return L;

    if (L->priority > R->priority)
    {
      L->right = merge(L->right, R);
      update(L);
      return L;
    }
    else
    {
      R->left = merge(L, R->left);
      update(R);
      return R;
    }
  }

  void split(Node<int, T>* n, int k, Node<int, T>*& L, Node<int, T>*& R)
  {
    push(n);

    if (!n)
    {
      L = nullptr;
      R = nullptr;
      return;
    }

    int ls = sz(n->left);

    if (ls >= k)
    {
      split(n->left, k, L, n->left);
      update(n);
      R = n;
    }
    else
    {
      split(n->right, k - ls - 1, n->right, R);
      update(n);
      L = n;
    }
  }

  void printRec(Node<int, T>* n)
  {
    if (!n) return;
    push(n);
    printRec(n->left);
    cout << n->data << " ";
    printRec(n->right);
  }

public:
  ImplicitTreap() : Treap<int, T>() {}

  void Build(const vector<T>& arr)
  {
    this->destroy(this->root);
    this->root = nullptr;

    for (int i = 0; i < (int)arr.size(); i++)
    {
      Node<int, T>* n = new Node<int, T>(0, arr[i], rand());
      this->root = merge(this->root, n);
    }
  }

  void InsertAt(int pos, T value)
  {
    Node<int, T>* n = new Node<int, T>(0, value, rand());

    Node<int, T>* L = nullptr;
    Node<int, T>* M = nullptr;

    split(this->root, pos, L, M);
    this->root = merge(merge(L, n), M);
  }

  bool RemoveAt(int pos)
  {
    if (pos < 0 || pos >= sz(this->root)) return false;

    Node<int, T>* L = nullptr;
    Node<int, T>* M = nullptr;
    Node<int, T>* R = nullptr;

    split(this->root, pos, L, M);
    split(M, 1, M, R);

    delete M;
    this->root = merge(L, R);
    return true;
  }

  T QuerySum(int l, int r)
  {
    Node<int, T>* L = nullptr;
    Node<int, T>* M = nullptr;
    Node<int, T>* R = nullptr;

    split(this->root, l, L, M);
    split(M, r - l + 1, M, R);

    T result = sm(M);

    this->root = merge(merge(L, M), R);
    return result;
  }

  void Reverse(int l, int r)
  {
    Node<int, T>* L = nullptr;
    Node<int, T>* M = nullptr;
    Node<int, T>* R = nullptr;

    split(this->root, l, L, M);
    split(M, r - l + 1, M, R);

    if (M) M->rev = !M->rev;

    this->root = merge(merge(L, M), R);
  }

  void Print()
  {
    printRec(this->root);
    cout << "\n";
  }

  int Size() { return sz(this->root); }
};

int main()
{
  srand(42);

  ImplicitTreap<double> tree;

  vector<double> arr = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5};
  tree.Build(arr);

  cout << "\n";
  tree.Print();

  cout << "\n\n";
  cout << tree.QuerySum(0, 6) << "\n";

  cout << "\n";
  cout << tree.QuerySum(2, 4) << "\n";

  cout << "\n\n";
  tree.InsertAt(3, 99.9);
  tree.Print();

  cout << "\n\n";
  tree.RemoveAt(3);
  tree.Print();

  cout << "\n\n";
  tree.Reverse(1, 5);
  tree.Print();

  cout << "\n\n";
  cout << tree.QuerySum(1, 5) << "\n";

  cout << "\n\n";
  tree.Reverse(1, 5);
  tree.Print();

  return 0;
}