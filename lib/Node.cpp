#include "Node.h"
Node::Node(long long freq, char c, Node *left, Node *right)
    :frequency(freq), c(c), left_node(left), right_node(right){}
    