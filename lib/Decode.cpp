#include "Decode.h"
Decode::Decode(Encode obj)
    :obj(obj){
        std::map<char, std::string>chartoString = obj.find_translation_table();
        // convert it from value to pair
        for(auto i:chartoString){
            this->translation_table.insert({i.second, i.first});
        }
        this->root_node = obj.find_root();
    }
void Decode::Construct_original_msg(std::string s){
    // s is a binary string
    char c;
    Node* current_node = this->root_node;
    std::string letter = "";
    std::string decoded_msg="";
    for(char i: s){
        if(i=='0'){
            current_node = current_node->left_node;
            letter += '0';
            if(!(current_node->left_node || current_node->right_node)){
             decoded_msg += this->translation_table[letter]; 
             current_node = this->root_node;
             letter = "";
            }
        }else{
            current_node = current_node->right_node;
            letter += '1';
            if(!(current_node->left_node || current_node->right_node)){
             decoded_msg += this->translation_table[letter]; 
             current_node = this->root_node;
             letter="";
            }
        }
    }
    this->decoded_message = decoded_msg;
    return;
}
void Decode::print_decoded_msg(){
    std::cout<<(this->decoded_message)<<"\n";
}
