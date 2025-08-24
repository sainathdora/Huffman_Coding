#pragma once
#include<bits/stdc++.h>
#include "Node.h"
class Encode{
public:
    std::map<char, long long>mp;
    struct compare{
        bool operator()(Node *a, Node *b){
            if(a->frequency<=b->frequency){
                return false;
            }else return true;
        }
    };
    priority_queue<Node*, vector<Node*>, compare>PQ;
    void Start(){
        for(auto i:this->mp){
            PQ.push(new Node(i.second, i.first, nullptr, nullptr));
        }
        while(PQ.size()>=2){
            Node *left = PQ.top(); 
            PQ.pop();
            Node *right = PQ.top();
            PQ.pop();
            Node *merged = new Node(left->frequency+right->frequency, '\0', left, right);
            PQ.push(merged);
        }
        Node *root = PQ.top();
        

    }

    Encode(std::map<char, long long>mp);
private:

};