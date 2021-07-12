#ifndef	__BST_
#define __BST_

#include <cstdlib> 
#include <functional>
#include <iostream>
#include <utility>
#include <memory>
#include <iterator>
#include <vector>
#include <stdlib.h>
//
//
// ===============================================================
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   node
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ===============================================================

template <typename k_type, typename v_type>
  struct _node{
    using pair_type = std::pair<k_type, v_type>;
    pair_type pair;
    std::unique_ptr<_node> right;
    std::unique_ptr<_node> left;
    _node* parent=nullptr;
    
    _node() noexcept = default; 
    ~_node() noexcept = default;
    
    explicit _node(const pair_type& p) 
      : pair{p} {}
    
    explicit _node (pair_type&& p) noexcept 
      : pair{std::move(p)} {}
    
    _node(const pair_type& p, _node* nptr) noexcept 
      : pair{p} {
      if (nptr->left){
        left.reset(new _node(nptr->left->pair, nptr->left.get()));
	left->parent = this;
	}
      if (nptr->right){
        right.reset(new _node(nptr->right->pair, nptr->right.get()));
	right->parent = this;
        }
      }
  };

// ===============================================================
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   iterator
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ===============================================================

  template <typename k_type, typename v_type, typename O> 
  class _iterator
  {
  using node = _node<k_type,v_type>;
  node *current; 

  public:
    using value_type = O;
    using reference = value_type&;
    using pointer = value_type*;
    using difference_type = std::ptrdiff_t; 
    using iterator_category = std::bidirectional_iterator_tag;   
    // iterator ctr
    explicit _iterator(node *p) noexcept: current{p} {}
  
// ===============================================================
    reference operator*() const noexcept { return current->pair; }// when dereference I shall return pair
    pointer operator->() const noexcept{ return &**this; }
    
// ===============================================================
    // pre-increment
    _iterator &operator++() noexcept{
      
      // check if there are sub-right trees
      if (current->right){
        
	current = current->right.get();
	while(current->left){
	  current = current->left.get();
        }

      } else {// go up and use the parent only if coming from right
        
	auto tmp_p = current->parent;
	
	if (!tmp_p){//if node is null ptr the end is reached and must return 
	  current = tmp_p;
	  return *this;
	  }

	while (tmp_p->parent && (tmp_p->right.get() == current)) {
	  current = tmp_p;
	  tmp_p = tmp_p->parent;
	  }
	if( tmp_p->right.get() == current) 
	  tmp_p = tmp_p->parent;
	
	current = tmp_p;
      }
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
    _iterator &operator--() {
      
      // check if there are sub-right trees
      if (current->left){
        
	current = current->left.get();
	while(current->right){
	  current = current->right.get();
        }

      } else {// go up and use the parent only if coming from right
        
	auto tmp_p = current->parent;
	
	if (!tmp_p){//if node is null ptr the end is reached and must return 
	  current = tmp_p;
	  return *this;
	  }

	while (tmp_p->parent && (tmp_p->left.get() == current)) {
	  current = tmp_p;
	  tmp_p = tmp_p->parent;
	  }
	if( tmp_p->left.get() == current) 
	  tmp_p = tmp_p->parent;
	
	current = tmp_p;
      }
      return *this;
    }

// ===============================================================
    _iterator operator--(int) {
      auto tmp{*this};
      --(*this);
      return tmp;
    }
// ===============================================================
    // fine because comparing the pointers, not the keys
    friend bool operator==(const _iterator &a, const _iterator &b) {
      return (a.current == b.current);
    }
    friend bool operator!=(const _iterator &a, const _iterator &b) { 
      return !(a == b); /// !!! era forward
    }
};

// ===============================================================
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//   bst
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// ===============================================================
template <typename k_type, typename v_type,
          typename OP = std::less<k_type> >
class bst{

  using node = _node<k_type,v_type>;
  using p_type = typename node::pair_type;
  using iterator = _iterator<k_type, v_type, p_type>;
  using const_iterator = _iterator<k_type, v_type, const p_type>;
  OP op = OP{};
  std::unique_ptr<node> head;  
  std::size_t _size{0};

// ===============================================================
//  utilities
// ===============================================================

// ===============================================================
//  place in the middle
// ===============================================================
  std::pair<iterator,bool> place_in_the_middle(node* tmp, node* _node){
    
    std::vector<p_type> vec_tmp;
    vec_tmp = store_branch(vec_tmp, 1, tmp);
    vec_tmp.push_back(tmp->pair);
    vec_tmp = store_branch(vec_tmp, 0, tmp);

    auto par = tmp->parent;
    tmp->parent = nullptr;
    if (par->right.get() == tmp){
      _node->parent = par;
      par->right.reset(_node);
      _balance(vec_tmp, 0, vec_tmp.size());
      return std::pair<iterator,bool>{ iterator(_node) , 1};
      }
    else {
      _node->parent = par;
      par->left.reset(_node);
      _balance(vec_tmp, 0, vec_tmp.size());
      return std::pair<iterator,bool>{ iterator(_node) , 1 };
      }
  }

// ===============================================================
//  balance
// ===============================================================
  void _balance(const std::vector<typename node::pair_type>& tmp, std::size_t sidx, std::size_t eidx) noexcept {
   
    if (eidx == sidx)
      return;

    _insert(tmp[(eidx+sidx)/2]);
    
    if ((eidx+sidx)/2 != sidx)
      _balance(tmp, sidx, (eidx+sidx)/2);
    if ((eidx+sidx)/2+1 != eidx)
      _balance(tmp, (eidx+sidx)/2+1, eidx);
  
  }

// ===============================================================
//  store branch
// ===============================================================
  
  std::vector<p_type>&  store_branch(std::vector<p_type>& tmp, const bool store_left, node* to_remove) noexcept {
    
    bool am_left=0;
    if(to_remove == head.get() || to_remove->parent->left.get() == to_remove)
      am_left=1;
    
    auto it = iterator(to_remove);
    
    // ==================================================
    if (am_left && !store_left){
      ++it;
      //right branch (between to_remove and parent) removed and reinsered after.
      while (it != iterator(to_remove->parent)){
	tmp.push_back(*it);
        ++it;}
    }

    // ==================================================
    else if (!am_left && store_left){
      --it;
      //left branch (between to_remove and parent) removed and reinsered after.
      while (it != iterator(to_remove->parent)){
        tmp.push_back(*it);
        --it;}
    }

    // ==================================================
    else if (!am_left && !store_left){
      ++it;
      auto par = to_remove->parent;
      while(par->parent && par->parent->right.get() == par)
        par = par->parent;
      iterator stop=end();
      if (par->parent) // if not head
        stop = (iterator(par->parent));

      //left branch (between to_remove and parent) removed and reinsered after.
      while (it != stop){
	tmp.push_back(*(it));
	++it;} 
    }

    // ==================================================
    else if (am_left && store_left){
      auto par = to_remove->parent;
      while(par->parent && par->parent->left.get() == par)
        par = par->parent;
      iterator stop=begin();
      if (par->parent) // if head
        stop = ++(iterator(par->parent));

      //left branch (between to_remove and parent) removed and reinsered after.
      while (it != stop)
	tmp.push_back(*(--it));
        
    }
    return tmp;
    
  }


// ===============================================================
//  _erase
// ===============================================================
  
  void _erase(const node* to_remove) noexcept {
    
    bool left=0;
    if(to_remove == head.get() || to_remove->parent->left.get() == to_remove)
      left=1;
    
    if(left) {
      if(to_remove == head.get() ){
        head.reset(head->left.release());
        head->parent=nullptr;}
      else{
        auto par = to_remove->parent;
        par->left.reset(to_remove->left.release());
        if(par->left)
          par->left->parent = par;}
      }
    else {
      auto par = to_remove->parent;
      par->right.reset(to_remove->right.release());
      if(par->right)
        par->right->parent = par;
    }
  }


// ===============================================================
public:
// ===============================================================


  bst() noexcept = default;
  ~bst() noexcept = default;

  std::size_t get_size() const noexcept {return _size;}
  
// ===============================================================
//   begin & end
// ===============================================================


// ===============================================================
  /*
   * _b: to avoid code duplication
   * begin node: the one with the smallest k
   * namely the left-most
   */
  node* _b() const noexcept {
    auto tmp = head.get();
    while (tmp->left)
      tmp = tmp->left.get();
    return tmp;  
  }

  // define begin iterators
  iterator begin() noexcept { return iterator{_b()};}
  const_iterator begin() const noexcept { return const_iterator{_b()};}
  const_iterator cbegin() const noexcept { return const_iterator{_b()};} 

// ===============================================================
  // define end iterators
  iterator end() noexcept { return iterator{nullptr}; }
  const_iterator end() const noexcept { return const_iterator{nullptr}; }
  const_iterator cend() const noexcept { return const_iterator{nullptr}; }

// ===============================================================
  /*
   * _last: to avoid code duplication
   * last node: the one with the highest k, 
   * namely the right-most
   */
  node* _last() const noexcept { 
    auto tmp = head.get();
    while (tmp->right)
      tmp = tmp->right.get();
    return tmp; 
  }

  iterator last() noexcept { return iterator{_last()};}
  const_iterator last() const noexcept { return const_iterator{_last()};}

// ===============================================================
//   insert
// ===============================================================
 
  std::pair<iterator, bool> insert(const p_type& x) {return _insert(x);}
  std::pair<iterator, bool> insert(p_type&& x) {return _insert(std::move(x));}
  

// ===============================================================
  template <typename O>
  std::pair<iterator,bool> _insert(O&& v) {
    
    auto _node = new node{std::forward<O>(v)};
    auto tmp = head.get();
    bool found=1;
    _size++;

    if (!tmp){
      head.reset(_node);
      return std::pair<iterator,bool>{ begin() , found };
    }
 
    while (tmp || tmp == head.get()  ) {
      if (op(v.first, tmp->pair.first)) {
          
	  if (tmp->left)
	    tmp = tmp->left.get();
	  else {
            _node->parent = tmp;
	    tmp->left.reset(_node);
            return std::pair<iterator,bool>{ iterator(_node) , found };
	  }
      }

      else if (op(tmp->pair.first, v.first)) {

	  if (tmp->right)
	    tmp = tmp->right.get();
	  else {
            _node->parent = tmp;
	    tmp->right.reset(_node);
            return std::pair<iterator,bool>{ iterator(_node) , found };
          }
      }
      else {// if found a node with same key, order the value
        while (!op(tmp->pair.first, v.first) && !op(tmp->pair.first, v.first)){
	    if (v.second < tmp->pair.second ){
	    std::cout << tmp->pair.first << std::endl;
	      if (tmp->left){
	        tmp = tmp->left.get();
                if (tmp->pair.first != v.first)
		  return place_in_the_middle(tmp,_node);
		  }
	      else {
                _node->parent = tmp;
	        tmp->left.reset(_node);
                return std::pair<iterator,bool>{ iterator(_node) , found };
	      }
	    }
            else if (tmp->pair.second < v.second) {
              if (tmp->right){
                tmp = tmp->right.get();
                if (tmp->pair.first != v.first)
	          {
	        std::cout << "here" << std::endl;
		  return place_in_the_middle(tmp,_node);
		  }
                }
	      else {
                _node->parent = tmp;
                tmp->right.reset(_node);
                return std::pair<iterator,bool>{ iterator(_node) , found };
              }
            }
            else if (tmp->pair.second == v.second) {
              found = 0;
              _size--;
              delete _node;
              return std::pair<iterator,bool>{ end() , found };  
	    }
	}//esce il primo diverso 
        
	return place_in_the_middle(tmp, _node);
        
      }
    }
    
    found = 0;
    _size--;
    delete _node;
    return std::pair<iterator,bool>{ end() , found };  
  }

// ===============================================================
//   move and copy ctrs and assignments
// ===============================================================

  bst(bst&&) noexcept = default;
  bst& operator=(bst&&) noexcept = default;


// ===============================================================
  bst(const bst& b){
    if (b.head) {
      auto _node = new node{b.head->pair, b.head.get()};
      head.reset(_node);
      _size = b._size;
      }
    }
  
// ===============================================================
  bst operator=(const bst& b){
    head.reset();              
    auto tmp = b;              
    (*this) = std::move(tmp);  
    return *this;
  }



// ===============================================================
//   find
// ===============================================================

  /*
   * loop over all nodes until the desired one is found.
   */
  iterator find(const k_type& x, const v_type& v) noexcept { return iterator{_find(x,v)}; }
  const_iterator find(const k_type& x,  const v_type& v) const noexcept { return const_iterator{_find(x,v)}; }

// ===============================================================
  node* _find(const k_type& x, const v_type& v) const noexcept {   
    
    auto next = head.get();
    while (next) {
      if (op(next->pair.first, x))
        next = next->right.get();
      else if (op(x, next->pair.first))
        next = next->left.get();
      else if (!op(x, next->pair.first) && !op(next->pair.first, x)){
        if (next->pair.second == v)
	  return next;
	else{ // if has same key but different value, look for same value
          if (next->pair.second < v)
            next = next->right.get();
          else if (v < next->pair.second)
            next = next->left.get();
	  
	}
	}
    }

    //if not found
    return nullptr;
  }

// ===============================================================
//   emplace
// ===============================================================

  template< class... Types >
  std::pair<iterator,bool> emplace(Types&&... args){
    return _insert(std::make_pair(std::forward<Types>(args)...));
  }

// ===============================================================
//   clear
// ===============================================================
  
  void clear() noexcept {
    head.reset();
    return;
  }

// ===============================================================
//   balance
// ===============================================================
  
  void balance() noexcept {
    /*
     * to balance the tree all items are stored in a std::vector
     * in ascending order thanks to the iterator, then the head
     * is reset to a nullptr and then stored elements are insered 
     * again recursively splitting into half the vector.
     */

    std::vector<typename node::pair_type> tmp;
    for (const auto &x : *this)
      tmp.push_back(x);
    
    head.reset(nullptr);

    _size =0;
    _balance(tmp, 0, tmp.size());
    return;
  }

    
// ===============================================================
//   erase
// ===============================================================
  
  void erase(const k_type& x) noexcept {
    node* to_remove = _find(x);
    bool left=0;
    auto it = iterator(to_remove);
   
    if (it == end()){
      std::cout << "the value is not present" << std::endl;
      return;}
    
    _size--;
    if(to_remove == head.get() || to_remove->parent->left.get() == to_remove)
      left=1;

    std::vector<p_type> tmp;
    tmp = store_branch(tmp, !left, to_remove);//store opposite branch

    _erase();
    // use _balance to insert values in a balanced way
    _balance(tmp, 0, tmp.size());
  }

// ===============================================================
//   remove minimum
// ===============================================================
  
  p_type remove_min() noexcept {
    p_type it = *(begin());
    std::cout <<it.first << std::endl; 
    erase(it.first);

    return it;
  }
  
// ===============================================================
//   update distance
// ===============================================================
  
  p_type update_dist(const v_type& v, const k_type& d) noexcept {
    
    p_type it = *(begin());
    std::cout <<it.first << std::endl; 
    erase(it.first);

    return it;
  }

// ===============================================================
//   overloading
// ===============================================================
  
  //subscripting operator
  v_type& operator[](const k_type& x) noexcept  {
    auto it = find(x);
    if ( it == end()){
      v_type v = "inserted new value with [] operator";
      emplace(x,v);
      it = find(x);
    }
    return it->second;
  }

// ===============================================================
  
  v_type& operator[](k_type& x) noexcept  {
    auto it = find(x);
    if ( it == end()){
      v_type v = "inserted new value with [] operator";
      emplace(x,v);
      it = find(x);
    }
    return it->second;
  }
  
// ===============================================================
  friend
  // put-to operator
  std::ostream &operator<<(std::ostream &os, const bst& b) noexcept {
    if (!b.head){
      os << " ";
      return os;}
    for (const auto &x : b)
      os << "[" << x.first << ", " << x.second << "]   ";
    return os;
  }
};
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


#endif
