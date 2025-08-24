#pragma once
class Node{
public:
    long long frequency;
    char c='\0';
    Node *left_node=nullptr;
    Node *right_node=nullptr;
    Node(long long frequency, char c, Node *left, Node *right);
};