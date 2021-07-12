#ifndef	__GRAPH_
#define __GRAPH_

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
  
  //init_sssp(std::vector<unsigned int>& d, std::vector<unsigned int>& pred) {}

/*
  relax(Q, u, v, w){
    if(d[u]+w < d[v]){
      update_dist(Q, v, d[u]+w);
      pred[v] = u;
    }
  }
  */

  bst<int, node> build_queue(unsigned int s){
    
    bst<int, node> b; // dist and node
    std::cout << V.size() << std::endl;
    for (unsigned int i{0}; i<V.size(); ++i){
    //  if (i ==s)
    //    continue;
      b.insert(std::pair<int,node>(-i,V[i]));
    }
    std::cout << b << std::endl;
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
    std::cout << "ctr" << std::endl;
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
      d.push_back(-i-1);
      pred.push_back(-1);
      }
    
    d[s]=0;
    bst<int, node> q = build_queue(s);
    std::cout << "q size " << q.get_size() << std::endl;
    //std::pair<int, Graph<unsigned int>::node> u;
    /*while (q.get_size() > 0){
    // std::pair<int, Graph<unsigned int>::node> u = std::move(q.remove_min());
     std::pair<int, node> u = q.remove_min();
     std::cout << "q size " << q.get_size() << std::endl;
    }
    */
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
