#ifndef	__LL_
#define __LL_

#include <iostream>
#include <memory>
#include <utility>
#include <iterator>

enum class insert_method { push_back, push_front };



template <typename T>
class LL{

  // -------------------------------
  //  node
  // -------------------------------
  struct node {
    T value;
    std::unique_ptr<node> next;

    //node() = default;
    explicit node(T& v) :  value{v}{}
    explicit node(T&& v) :  value{std::move(v)}{}
    node(T& v, node* n) : value{v}, next{n} {}
    node(T&& v, node* n) : value{std::move(v)}, next{n} {}
    
    ~node() = default;
  };
  




  std::unique_ptr<node> head;
  std::size_t _size=0;
  

  // -------------------------------
  //  insertions
  // -------------------------------
  
  template <typename O>
  void insert_back(O&& v){
    _size += 1;
    
    auto tmp = head.get();
    if(!tmp) {
      head.reset(new node{std::forward<O>(v)});
      return;
      }
      
    while (tmp->next)
      tmp = tmp->next.get();
    
    tmp->next.reset(new node{std::forward<O>(v)});
  }

  template <typename O>
  void insert_front(O&& v){
    _size += 1;
    head.reset(new node{std::forward<O>(v), head.release()});
  }
  

  template <typename O>
  void _insert(O&& v, insert_method m){
    switch(m){
    case insert_method::push_back:
      insert_back(std::forward<O>(v));
      break;
    case insert_method::push_front:
      insert_front(std::forward<O>(v));
      break;
    default:
      std::cout << "no correct method identified, use push back" << std::endl;
      insert_back(std::forward<O>(v));
      break;
    }
  }

 public:
  // -------------------------------
  //  ctrs
  // -------------------------------
  
  LL() = default;
  ~LL() = default;

  void insert(T& v, insert_method m){_insert(v,m);}
  void insert(T&& v, insert_method m){_insert(std::move(v),m);}

  // no row ptr -> default move semantics
  LL(LL&& l) = default;
  LL& operator=(LL&& l) = default;

  std::size_t size() {return _size;} 

  // -------------------------------
  //  copy and move semantics
  // -------------------------------
  
  // copy ctr
  LL(LL& l) {
    //head must point to a copy of the I node (pointed by l.head()
    
    head.reset(new node(l.head.get()->value));
    auto ltmp = l.head.get();
    auto tmp = head.get();
   
    while(ltmp->next){
      ltmp = ltmp->next.get();
      tmp->next.reset(new node{ltmp->value});
      tmp = tmp->next.get();
    } 
    _size = l._size;
  }

  LL operator=(LL& l) {
    head.reset();
    auto tmp = l;
    *this = std::move(tmp);
    return *this;
  }

  T& operator[](std::size_t n){
    auto tmp = head.get();
    for(std::size_t i{0}; i<n; i++)
      tmp = tmp->next.get();
    return tmp->value;
  }
  
  template <typename O>
  class _iterator;
  // -------------------------------
  using iterator = _iterator<T>;
  using const_iterator = _iterator<const T>;
  
  iterator begin() noexcept {return iterator{head.get()};}
  const_iterator begin() const noexcept {return const_iterator{head.get()};}
  
  iterator end() noexcept {return iterator{nullptr};}
  const_iterator end() const noexcept {return const_iterator{nullptr};}

  friend
  std::ostream& operator<<(std::ostream& os, LL& l){
    auto tmp = l.head.get();
    while (tmp){
      os << tmp->value <<" ";
      tmp = tmp->next.get();
      }
    return os;
  }

};








// -------------------------------
//  iterator
// -------------------------------
template <typename T>
template <typename O>
class LL<T>::_iterator
{
using node = typename LL<T>::node;
node *current; 

public:
  using value_type = O;
  using reference = value_type&;
  using pointer = value_type*;
  using difference_type = std::ptrdiff_t; 
  using iterator_category = std::forward_iterator_tag;   
  // iterator ctr
  explicit _iterator(node *p) noexcept: current{p} {}



// ===============================================================
  reference operator*() const noexcept { return current->value; }// when dereference I shall return pair
  pointer operator->() const noexcept{ return &**this; }
    


// ===============================================================
  // pre-increment
  _iterator &operator++() noexcept{
    //if(current->next)
    current = current->next.get();
    return *this;
  }
  


// ===============================================================
  // post-increment
  _iterator operator++(int) {
    auto tmp{*this};
    ++(*this);
    return tmp;
  }

// ===============================================================
  friend bool operator==(const _iterator &a, const _iterator &b) {
    return (a.current == b.current);
  }
  friend bool operator!=(const _iterator &a, const _iterator &b) { 
    return !(a == b); 
  }
};

#endif
