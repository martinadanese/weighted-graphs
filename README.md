# weighted-graphs

Weighted graphs are implemented through the `Graph` class templated on the node value. It relies on:

- `bst<const k_type,v_type,OP>` defined in `binary_tree.hpp` header, which implements binary search trees (bst). 

- `ll.hpp`, where linked lists are implemented.

- `node`, a structure defined inside the `Graph` class that represents the node of a graph. All nodes present a different vertex number,
starting from 0 to the number of nodes, and a value, which may differ from the vertex according to the applications.

Graph presents two constructors: the first allows for definitions passing the values of all the nodes through an initializer list,
while the signature of the second has the number of nodes as input and in the body vertices and values are set as equal.

It is possible to add edges with the member function `edge`, passing as input the starting and the anding node and possibly the weight.

Thanks to the overloading of the operator put to, a summary of the graph can be printed.
