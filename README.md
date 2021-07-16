# weighted-graphs

Weighted graphs are implemented through the `Graph` class templated on the node value type. It relies on:

- `binary_tree.hpp` header which defines `bst<k_type,v_type,OP>`, a class that implements binary search trees (bst) hierarchical data structure with template comparison operator `OP`, and with nodes (`_node` structure) storing pairs of key (typename `k_type`) and value (typename `v_type`). To traverse the tree properly, an iterator (`_iterator` class) is used.

- `ll.hpp`, where linked lists are implemented. In the template class `LL` each value is stored in a private structure `node` whose memebers variables are the `value` of the element stored in the node and a `std::unique_ptr<node>` to the next node. Analogously `LL` uses ab unique pointer to identify the first node. 

- `node`, a structure private to the `Graph` class that represents the node of a graph. All nodes present a different `vertex` number,
starting from 0 to the number of nodes, and a `value`, which may differ from the vertex according to the applications.

Graph presents two constructors: the first allows for definitions passing the values of all the nodes through an initializer list,
while the signature of the second has the number of nodes as input and in the body vertices and values are set as equal.

It is possible to add edges with the member function `edge`, passing as input the starting and the anding node and possibly the weight.

Thanks to the overloading of the operator put to, a summary of the graph can be printed.
