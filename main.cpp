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
  
  Graph<unsigned int> g{10,20,30,40,50,60};
  g.edge(0,1,1);
  g.edge(0,2,5);
  g.edge(2,3,2);
  g.edge(3,4,1);
  g.edge(4,5,3);
  g.edge(1,5,15);
  std::cout << g <<std::endl;
  g.dijkstra(0);
}

#endif
