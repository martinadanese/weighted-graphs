#ifndef	__MAIN_
#define __MAIN_

#include <iostream>
#include "graph.hpp"
#include "ll.hpp"
#include <memory>

int main(){
  
  Graph<unsigned int> g{10,20,30,40};
  g.edge(0,1);
  g.edge(1,1);
  g.edge(0,3);
  g.edge(1,3);
  g.edge(3,1);
  std::cout << g <<std::endl;
}

#endif
