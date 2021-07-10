#ifndef	__MAIN_
#define __MAIN_

#include <iostream>
#include "graph.hpp"
#include "ll.hpp"
#include <memory>

int main(){
/*
  LL<int> l;
  l.insert(2,insert_method::push_back);
  l.insert(1,insert_method::push_front);
  l.insert(0,insert_method::push_front);
  int x=3;
  l.insert(x,insert_method::push_back);
  std::cout << l << std::endl;
*/
  
  Graph<unsigned int> g{10,20,30,40};
  g.edge(0,1);
  g.edge(1,1);
  g.edge(0,3);
  g.edge(1,3);
  g.edge(3,1);
  std::cout << g <<std::endl;
  g.dijkstra(0);
}

#endif
