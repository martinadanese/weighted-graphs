#ifndef	__GRAPH_
#define __GRAPH_

#include <limits>
#include <iostream>
#include "ll.hpp"
#include "binary_tree.hpp"
#include <memory>
#include <vector>

template <typename T>
class Graph{
  
  struct node {
    const unsigned int vertex=0;
    const T value=0;
    
    node() noexcept = default;
    ~node() noexcept = default;
    
    explicit node(const unsigned int& x) : vertex{x}, value{x} {}
    explicit node(unsigned int&& x) noexcept : vertex{std::move(x)}, value{vertex} {}

    node(const unsigned int& x, const T& t) : vertex{x}, value{t} {}
    node(unsigned int&& x, T& t) noexcept : vertex{std::move(x)}, value{std::move(t)} {}
    
  // -------------------------------------------------
  // move + copy
  // -------------------------------------------------
    node(node&& n) noexcept = default;
    node& operator=(node&& n) noexcept = default;
    
    node(const node& n) : value{n.value}, vertex{n.vertex} {}

  // -------------------------------------------------
  // op overloading
  // -------------------------------------------------
    friend
    bool operator==(const node& n1, const node& n2) {
      return n1.vertex==n2.vertex;
    }
  
    friend
    bool operator<(const node& n1, const node& n2){
      return n1.vertex<n2.vertex;
    }
    
    friend
    bool operator>(const node& n1, const node& n2){
      return n1.vertex>n2.vertex;
    }

    friend
    std::ostream& operator<<(std::ostream& os, const node& n) {
      os << "(" << n.vertex << "," << n.value << ")" ;
      return os;
    }
  };
  /*
  adj is a vector of linked lists. The vector is owned by a pointer.
  The linked lists are ll of a pair of integers: 1) for node idx 2) for weight 
  */
  using pair_type = std::pair<unsigned int,unsigned int>;
  
  const unsigned int n_vertices;
  std::vector<node> V;
  std::unique_ptr<LL<pair_type>[]> adj;

  // -------------------------------------------------
  // dijkstra - private
  // -------------------------------------------------
  
  void relax(bst<int, unsigned int>& q, const std::pair<int,unsigned int>& u, const int v, const unsigned int w, std::vector<int>& d, std::vector<int>& pred){
    if(d[u.second] + w < d[v] ){
      //std::cout <<"updating: [" << d[v] << ", " << v << "] in [" << d[u.second] + w << ", " << v << "]" <<std::endl;
      q.update_dist( d[v], v, d[u.second] + w);
      pred[v] = u.second;
      d[v] = d[u.second] + w;
    }
  }
  

  bst<int, unsigned int> build_queue(const unsigned int s){
    
    bst<int, unsigned int> b; // dist and node
    
    int val;
    for (unsigned int i{0}; i<V.size(); ++i){
      if (i ==s)
        val = 0;
      else
        val = std::numeric_limits<int>::max();
        
      std::pair<int,unsigned int> p{val,V[i].vertex};
      b.insert(p);
    }
    
    return b;
  }

public:

  // -------------------------------------------------
  // ctr
  // -------------------------------------------------
  
  ~Graph() noexcept= default;
  
  explicit Graph(const unsigned int n) 
  : n_vertices{n}, adj{new LL<pair_type>[n]} 
    {
    for(unsigned int i{0}; i<n; i++)
      V.push_back(node{i});
  }
  
  //pass node value
  Graph(std::initializer_list<T> l) 
  : n_vertices{static_cast<unsigned int>(l.size())},
    adj{new LL<pair_type>[l.size()]} 
    
  {
    std::unique_ptr<T[]> elem{new T[l.size()]};
    std::copy(l.begin(),l.end(), elem.get());
    
    for(unsigned int i{0}; i<l.size(); i++){
      V.push_back(node{i,elem[i]});
    }
  }

  // -------------------------------------------------
  // inserting edge
  // -------------------------------------------------
  
  void edge(std::size_t start, const unsigned int end,const unsigned int weight=1) {
    adj[start].insert(pair_type(end, weight),insert_method::push_front);
  }
 
  // -------------------------------------------------
  // dijkstra - public
  // -------------------------------------------------
  
  void dijkstra(unsigned int s){
    std::vector<int> d;
    std::vector<int> pred;

    for(unsigned int i{0}; i<V.size(); i++){
      d.push_back(std::numeric_limits<int>::max());
      pred.push_back(-1);
      }
    
    d[s]=0;
    bst<int, unsigned int> q = build_queue(s);
    
    std::pair<int, unsigned int> u;
    while (q.get_size() > 0){
    //  std::cout << "-->  re-start while " << std::endl;
    //  std::cout << "q is " << q << std::endl;
    //  std::cout << "q size is " << q.get_size() << std::endl;
      u = q.remove_min();
    //  std::cout << "extracting min: " << u.first << " " << u.second <<  std::endl;
      for (auto& x : adj[u.second]){
	relax(q, u, x.first, x.second, d, pred);
	}
      //std::cout << "\n" << std::endl;
    }
    std::cout << "Dijkstra's resulting distances are"  << std::endl;
    for (auto& i : d)
      std::cout << i << "  ";
    std::cout << std:: endl;
    
  }    

  // -------------------------------------------------
  // operator overloading
  // -------------------------------------------------
  
  friend
  std::ostream& operator<<(std::ostream& os, Graph& g){
    for(unsigned int i{0}; i<g.V.size(); i++){
      os << "Vertex " << g.V[i].vertex << ", value " << g.V[i].value << "\n";
      for(unsigned int j{0}; j<g.adj[i].size(); j++)
        os << "  -> [" << (g.adj[i])[j].first << ", " << (g.adj[i])[j].second << "]";
      os << "\n\n";
    }
    return os;
  }

};

#endif
