#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <utility>
#include <iterator>
#include <initializer_list>
#include <functional>
#include <iostream>

namespace mihalchenko
{
  template <typename Key, typename Value, typename Compare = std::less<Key>>
  class AVLTree
  {
  public:
    class ConstIterator;
    class Iterator;

    AVLTree();

    template <class InputIt>
    AVLTree(InputIt start, InputIt stop);

    using pair_t = std::pair<Key, Value>;
    AVLTree(size_t count, pair_t &pairKeyVal);
    AVLTree(std::initializer_list<pair_t> initAVL);
    AVLTree(const AVLTree &copy);
    AVLTree(AVLTree &&move);
    ~AVLTree();

    void insert(Key key, Value value);
    void insert(pair_t pairKeyVal);
    bool empty() const noexcept;
    size_t getSize() const noexcept;
    size_t erase(const Key &key);
    void clear();
    void swap(AVLTree &) noexcept;

    AVLTree &operator=(AVLTree other);

    Value &at(const Key &);
    const Value &at(const Key &key) const;

    Value &operator[](const Key &key);
    const Value &operator[](const Key &) const;
    Iterator find(const Key &);
    ConstIterator find(const Key &) const;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;
    Iterator begin() noexcept;
    Iterator end() noexcept;

  private:
    class Node
    {
    public:
      friend class AVLTree;
      Node(Key key, Value data, int height = 0, Node *left = nullptr, Node *right = nullptr, Node *previous = nullptr);

    private:
      pair_t pairOfKeyVal_;
      int height_;
      Node *left_;
      Node *right_;
      Node *previous_;
    };

    Node *insertNode(Key key, Value val, Node *node);
    Iterator findNode(const Key &key, Node *node);
    ConstIterator findNode(const Key &key, Node *node) const;

    int calcHeight(Node *node);
    void balancingTree(Node *overweight);
    void leftrightSpin(Node *node);
    void rightleftSpin(Node *node);
    void leftSpin(Node *node);
    void rightSpin(Node *node);

    Node *erase(Node *node, const Key &key);
    void swap(Node &) noexcept;
    Node &operator=(Node &other);

    int getHeight(Node *node);
    Node *getRoot();
    const Node *getRoot() const;

    Node *copyTree(Node *node, Node *previous);
    void clear(Node *node);

    Node *root_;
    size_t size_ = 0;
  };
}

template <typename Key, typename Value, typename Compare>
class mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator :
  public std::iterator<std::bidirectional_iterator_tag, std::pair<const Key, Value>>
{
public:
  friend class AVLTree<Key, Value, Compare>;
  ConstIterator();
  ConstIterator(Node *node, Node *root);
  ConstIterator(const ConstIterator &) = default;
  ~ConstIterator() = default;

  ConstIterator &operator++();
  ConstIterator operator++(int);
  ConstIterator &operator--();
  ConstIterator operator--(int);
  const pair_t &operator*() const;
  const pair_t *operator->() const;
  bool operator!=(const ConstIterator &rhs) const;
  bool operator==(const ConstIterator &rhs) const;
  ConstIterator &operator=(const ConstIterator &) = default;

private:
  Node *node_;
  Node *root_;
};

template <typename Key, typename Value, typename Compare>
class mihalchenko::AVLTree<Key, Value, Compare>::Iterator :
  public std::iterator<std::bidirectional_iterator_tag, Key, Value, Compare>
{
public:
  friend class AVLTree<Key, Value, Compare>;
  Iterator();
  Iterator(ConstIterator iter);
  Iterator(const Iterator &) = default;
  ~Iterator() = default;

  Iterator &operator++();
  Iterator operator++(int);
  Iterator &operator--();
  Iterator operator--(int);
  Iterator &operator=(const Iterator &) = default;
  pair_t &operator*();
  pair_t *operator->();
  const pair_t &operator*() const;
  const pair_t *operator->() const;
  bool operator!=(const Iterator &rhs) const;
  bool operator==(const Iterator &rhs) const;

private:
  ConstIterator constIter_;
};

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::Node::Node(Key key, Value data, int height, Node *left, Node *right, Node *previous) :
  pairOfKeyVal_(std::make_pair(key, data)),
  height_(height),
  left_(left),
  right_(right),
  previous_(previous)
{}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node
  &mihalchenko::AVLTree<Key, Value, Compare>::operator=(Node &other)
{
  swap(other);
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node
  *mihalchenko::AVLTree<Key, Value, Compare>::insertNode(Key key, Value val, Node *node)
{
  Node *newNode = new Node(key, val, 0, nullptr, nullptr, nullptr);
  if (!node)
  {
    root_ = newNode;
    size_++;
    return root_;
  }
  Compare compare;
  if ((node) && !compare(key, node->pairOfKeyVal_.first) && !compare(node->pairOfKeyVal_.first, key))
  {
    return node;
  }
  bool existNode = true;
  while (existNode)
  {
    if (compare(key, node->pairOfKeyVal_.first))
    {
      if (!node->left_)
      {
        node->left_ = newNode;
        newNode->previous_ = node;
        size_++;
        existNode = false;
      }
      node = node->left_;
    }
    else
    {
      if (!node->right_)
      {
        node->right_ = newNode;
        newNode->previous_ = node;
        size_++;
        existNode = false;
      }
      node = node->right_;
    }
  }
  balancingTree(node);
  return newNode;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree()
{
  root_ = nullptr;
  size_ = 0;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(size_t count, pair_t &pairKeyVal)
{
  root_ = nullptr;
  size_ = 0;
  for (size_t i = 0; i < count; ++i)
  {
    insert(pairKeyVal);
    size_++;
  }
}

template <typename Key, typename Value, typename Compare>
template <class InputIt>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(InputIt start, InputIt stop)
{
  root_ = nullptr;
  size_ = 0;
  for (InputIt it = start; it != stop; ++it)
  {
    insert(*it);
    size_++;
  }
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(std::initializer_list<pair_t> initAVL)
{
  root_ = nullptr;
  size_ = 0;
  for (auto it = initAVL.begin(); it != initAVL.end(); ++it)
  {
    insert(*it);
    size_++;
  }
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(const AVLTree &copy)
{
  root_ = copyTree(copy.root_, nullptr);
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::AVLTree(AVLTree &&move)
{
  root_ = copyTree(std::move(move.root_), nullptr);
  move.clear();
  move.root_ = nullptr;
  move.size_ = 0;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::~AVLTree()
{
  clear();
  root_ = nullptr;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::insert(mihalchenko::AVLTree<Key, Value, Compare>::pair_t pair)
{
  insert(pair.first, pair.second);
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::insert(Key key, Value value)
{
  Node *node = insertNode(key, value, root_);
  if (node != nullptr)
  {
    size_t value1 = (node->left_) ? node->left_->height_ : 0;
    size_t value2 = (node->right_) ? node->right_->height_ : 0;
    node->height_ = std::max(value1, value2) + 1;
  }
  return;
}

template <typename Key, typename Value, typename Compare>
size_t mihalchenko::AVLTree<Key, Value, Compare>::erase(const Key &key)
{
  if (find(key) == end())
  {
    return 1;
  }
  else
  {
    erase(root_, key);
    return 0;
  }
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  mihalchenko::AVLTree<Key, Value, Compare>::find(const Key &key)
{
  return findNode(key, root_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  mihalchenko::AVLTree<Key, Value, Compare>::find(const Key &key) const
{
  return findNode(key, root_);
}

template <typename Key, typename Value, typename Compare>
Value &mihalchenko::AVLTree<Key, Value, Compare>::at(const Key &key)
{
  return (*find(key)).second;
}

template <typename Key, typename Value, typename Compare>
const Value &mihalchenko::AVLTree<Key, Value, Compare>::at(const Key &key) const
{
  return (*find(key)).second;
}

template <typename Key, typename Value, typename Compare>
Value &mihalchenko::AVLTree<Key, Value, Compare>::operator[](const Key &key)
{
  try
  {
    find(key);
  }
  catch (const std::out_of_range &)
  {
    insert(key, root_);
    return (*find(key)).second;
  }
}

template <typename Key, typename Value, typename Compare>
const Value &mihalchenko::AVLTree<Key, Value, Compare>::operator[](const Key &key) const
{
  try
  {
    find(key);
  }
  catch (const std::out_of_range &)
  {
    return (*find(key)).second;
  }
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::clear()
{
  clear(root_);
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::swap(AVLTree &other) noexcept
{
  std::swap(root_, other.root_);
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>
  &mihalchenko::AVLTree<Key, Value, Compare>::operator=(AVLTree other)
{
  swap(other);
  return *this;
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::empty() const noexcept
{
  return root_ == nullptr;
}

template <typename Key, typename Value, typename Compare>
size_t mihalchenko::AVLTree<Key, Value, Compare>::getSize() const noexcept
{
  return size_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  mihalchenko::AVLTree<Key, Value, Compare>::begin() noexcept
{
  if (empty())
  {
    return cend();
  }
  Node *temp = root_;
  while (temp->left_ != nullptr)
  {
    temp = temp->left_;
  }
  return Iterator(ConstIterator(temp, root_));
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  mihalchenko::AVLTree<Key, Value, Compare>::end() noexcept
{
  return ConstIterator(nullptr, root_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  mihalchenko::AVLTree<Key, Value, Compare>::cbegin() const noexcept
{
  if (empty())
  {
    return cend();
  }
  Node *temp = root_;
  while (temp->left_ != nullptr)
  {
    temp = temp->left_;
  }
  return ConstIterator(temp, root_);
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  mihalchenko::AVLTree<Key, Value, Compare>::cend() const noexcept
{
  return ConstIterator(nullptr, root_);
}

template <typename Key, typename Value, typename Compare>
int mihalchenko::AVLTree<Key, Value, Compare>::calcHeight(Node *node)
{
  if ((root_ == nullptr) || (node == nullptr))
  {
    return 0;
  }
  int weightLeftNode = (node->left_) ? calcHeight(node->left_) : 0;
  int weightRightNode = (node->right_) ? calcHeight(node->right_) : 0;
  int temp = (weightRightNode > weightLeftNode) ? weightRightNode - weightLeftNode + 1 : weightLeftNode - weightRightNode + 1;
  node->height_ = temp;
  return temp;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::balancingTree(Node *node)
{
  Node *temp = node;
  while (temp)
  {
    int finalWeight = (temp) ? calcHeight(temp->right_) - calcHeight(temp->left_) : 0;
    if (std::abs(finalWeight) == 2)
    {
      if ((temp->right_) && (temp->right_->height_ == 2) && (finalWeight < 0))
      {
        rightleftSpin(temp->right_);
      }
      else if ((temp->left_) && (temp->left_->height_ == 2) && (finalWeight > 0))
      {
        leftrightSpin(temp->left_);
      }
      else if ((temp->right_) && (temp->right_->height_ == 1) && (finalWeight < 0))
      {
        rightSpin(temp);
      }
      else if ((temp->left_) && (temp->left_->height_ == 2) && (finalWeight > 0))
      {
        leftSpin(temp);
      }
    }
    temp = temp->previous_;
  }
  return;
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::leftSpin(Node *nodeP)
{
  Node *nodeQ = nodeP->left_;
  if ((nodeQ->left_) && (nodeQ->right_))
  {
    if (nodeQ->right_->height_ - nodeQ->left_->height_ >= 2)
    {
      nodeP->previous_ = nodeQ;
      nodeP->left_ = nodeQ->right_;
      nodeQ->previous_ = nullptr;
      nodeQ->right_ = nodeP;
    }
  }
  else
  {
    nodeP->previous_ = nodeQ;
    nodeP->left_ = nullptr;
    nodeQ->previous_ = nullptr;
    nodeQ->right_ = nodeP;
  }
  if (nodeP == root_)
  {
    root_ = nodeQ;
  }
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::rightSpin(Node *nodeP)
{
  Node *nodeQ = nodeP->right_;
  if ((nodeQ->left_) && (nodeQ->right_))
  {
    if (nodeQ->left_->height_ - nodeQ->right_->height_ >= 2)
    {
      nodeP->previous_ = nodeQ;
      nodeP->right_ = nodeQ->left_;
      nodeQ->previous_ = nullptr;
      nodeQ->left_ = nodeP;
    }
  }
  else
  {
    nodeP->previous_ = nodeQ;
    nodeP->right_ = nullptr;
    nodeQ->previous_ = nullptr;
    nodeQ->left_ = nodeP;
  }
  if (nodeP == root_)
  {
    root_ = nodeQ;
  }
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::leftrightSpin(Node *nodeP)
{
  leftSpin(nodeP->left_);
  rightSpin(nodeP);
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::rightleftSpin(Node *nodeP)
{
  rightSpin(nodeP->right_);
  leftSpin(nodeP);
}

template <typename Key, typename Value, typename Compare>
int mihalchenko::AVLTree<Key, Value, Compare>::getHeight(Node *node)
{
  return (node) ? node->height_ : 0;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node
  *mihalchenko::AVLTree<Key, Value, Compare>::erase(Node *node, const Key &key)
{
  Compare compare;
  if (node == nullptr)
  {
    return node;
  }
  if (compare(key, node->pairOfKeyVal_.first))
  {
    node->left_ = erase(node->left_, key);
  }
  else if (compare(node->pairOfKeyVal_.first, key))
  {
    node->right_ = erase(node->right_, key);
  }
  else
  {
    Node *temp = nullptr;
    if (node->right_ == nullptr)
    {
      temp = node->left_;
      delete node;
      return temp;
    }
    else if (node->left_ == nullptr)
    {
      temp = node->right_;
      delete node;
      return temp;
    }
    else
    {
      temp = node->right_;
      while (temp->left_ != nullptr)
      {
        temp = temp->left_;
      }
      node->pairOfKeyVal_ = temp->pairOfKeyVal_;
      node->right_ = erase(node->right_, temp->pairOfKeyVal_.first);
    }
  }
  balancingTree(node);
  return node;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node *mihalchenko::AVLTree<Key, Value, Compare>::getRoot()
{
  Node *temp = this;
  while (temp->previous_ != root_)
  {
    temp = temp->previous_;
  }
  return temp->previous_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node
  const *mihalchenko::AVLTree<Key, Value, Compare>::getRoot() const
{
  Node *temp = this;
  while (temp->previous_ != root_)
  {
    temp = temp->previous_;
  }
  return temp->previous_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Node
  *mihalchenko::AVLTree<Key, Value, Compare>::copyTree(Node *node, Node *previous)
{
  if (node == nullptr)
  {
    return nullptr;
  }
  Node *newnode = new Node(node->pairOfKeyVal_.first, node->pairOfKeyVal_.second, node->height_, nullptr, nullptr, previous);
  newnode->left_ = copyTree(node->left_, newnode);
  newnode->right_ = copyTree(node->right_, newnode);
  return newnode;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::ConstIterator()
{
  node_ = nullptr;
  root_ = nullptr;
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::ConstIterator(Node *node, Node *root)
{
  node_ = node;
  root_ = root;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  &mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator++()
{
  if (node_->right_)
  {
    node_ = node_->right_;
    while (node_->left_)
    {
      node_ = node_->left_;
    }
  }
  else
  {
    Node *tempNode = node_->previous_;
    while (tempNode && node_ == tempNode->right_)
    {
      node_ = tempNode;
      tempNode = node_->previous_;
    }
    node_ = tempNode;
  }
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator++(int)
{
  ConstIterator result(*this);
  ++(*this);
  return result;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  &mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator--()
{
  if (node_ == nullptr)
  {
    node_ = root_;
    while (node_->right_)
    {
      node_ = node_->right_;
    }
  }
  else
  {
    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      Node *tempNode = node_->previous_;
      while (tempNode && node_ == tempNode->left_)
      {
        node_ = tempNode;
        tempNode = node_->previous_;
      }
      node_ = tempNode;
    }
  }
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator--(int)
{
  ConstIterator result(*this);
  --(*this);
  return result;
}

template <typename Key, typename Value, typename Compare>
const typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t
  &mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator*() const
{
  return node_->pairOfKeyVal_;
}

template <typename Key, typename Value, typename Compare>
const typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t
  *mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator->() const
{
  return std::addressof(node_->pairOfKeyVal_);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator==(const ConstIterator &rhs) const
{
  return node_ == rhs.node_;
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator::operator!=(const ConstIterator &rhs) const
{
  return !(*this == rhs);
}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::Iterator::Iterator() : constIter_(ConstIterator())
{}

template <typename Key, typename Value, typename Compare>
mihalchenko::AVLTree<Key, Value, Compare>::Iterator::Iterator(ConstIterator iter) : constIter_(iter)
{}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  &mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator++()
{
  ++constIter_;
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator++(int)
{
  Iterator result = constIter_;
  ++constIter_;
  return result;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  &mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator--()
{
  --constIter_;
  return *this;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator--(int)
{
  Iterator result = constIter_;
  --constIter_;
  return result;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t
  &mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator*()
{
  return constIter_.node_->pairOfKeyVal_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::pair_t
  *mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator->()
{
  return std::addressof(constIter_.node_->pairOfKeyVal_);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator!=(const Iterator &rhs) const
{
  return !(rhs == *this);
}

template <typename Key, typename Value, typename Compare>
bool mihalchenko::AVLTree<Key, Value, Compare>::Iterator::operator==(const Iterator &rhs) const
{
  return constIter_ == rhs.constIter_;
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::Iterator
  mihalchenko::AVLTree<Key, Value, Compare>::findNode(const Key &key, Node *node)
{
  Compare compFunc;
  if (node == nullptr)
  {
    return Iterator(ConstIterator(nullptr, root_));
  }
  if (node->pairOfKeyVal_.first == key)
  {
    return Iterator(ConstIterator(node, root_));
  }
  else if (compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->left_);
  }
  else if (!compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->right_);
  }
  else
  {
    throw std::out_of_range("Such element not exist\n");
  }
}

template <typename Key, typename Value, typename Compare>
typename mihalchenko::AVLTree<Key, Value, Compare>::ConstIterator
  mihalchenko::AVLTree<Key, Value, Compare>::findNode(const Key &key, Node *node) const
{
  Compare compFunc;
  if (node == nullptr)
  {
    return ConstIterator(nullptr, root_);
  }
  if (node->pairOfKeyVal_.first == key)
  {
    return ConstIterator(node, root_);
  }
  else if (compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->left_);
  }
  else if (!compFunc(key, node->pairOfKeyVal_.first))
  {
    return findNode(key, node->right_);
  }
  else
  {
    throw std::out_of_range("Such element not exist\n");
  }
}

template <typename Key, typename Value, typename Compare>
void mihalchenko::AVLTree<Key, Value, Compare>::clear(Node *node)
{
  if (node == nullptr)
  {
    return;
  }
  clear(node->left_);
  clear(node->right_);
  delete node;
}

#endif