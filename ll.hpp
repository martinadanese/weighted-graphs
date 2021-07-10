#ifndef	__LL_
#define __LL_

#include <iostream>
#include <memory>
#include <utility>

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


  // -------------------------------
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

#endif
