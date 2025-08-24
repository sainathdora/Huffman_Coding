#pragma once
#include <map>
#include <queue>
#include <string>
#include <iostream>
#include "Node.h"
class Encode {
public:
    std::map<char, long long> mp;
    struct compare{
        bool operator()(Node *a, Node *b){ 
            if(a->frequency<=b->frequency){ 
                return false;
            }
            return true; 
        }
    };
    std::priority_queue<Node*, std::vector<Node*>, compare> PQ;
    Node* root_node;
    Node* find_root();
    Encode(const std::map<char, long long>& mp);
    void Start();
    std::map<char, std::string> find_translation_table();

private:
    std::map<char, std::string> table;
    void dfs(Node *root, const std::string& s, std::map<char, std::string>& table);
};
