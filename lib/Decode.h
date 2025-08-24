#pragma once
#include "Encode.h"
#include "Node.h"
#include <map>
#include <string>

class Decode{
public:
    Encode obj;
    Node* root_node;
    std::string decoded_message;
    std::map<std::string, char>translation_table;
    Decode(Encode obj);
    void Construct_original_msg(std::string);
    void print_decoded_msg();
};