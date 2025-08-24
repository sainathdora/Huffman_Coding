#include "Encode.h"


Encode::Encode(const std::map<char, long long>& mp)
    : mp(mp) {}
void Encode::dfs(Node *root, const std::string& s, std::map<char, std::string>& table) {
    if(!root) return;
    if (!(root->left_node || root->right_node)) {
        table[root->c] = s;
        return;
    }
    dfs(root->left_node, s + "0", table);
    dfs(root->right_node, s + "1", table);
}

void Encode::Start() {
    for (auto i : mp) {
        PQ.push(new Node(i.second, i.first, nullptr, nullptr));
    }
    while (PQ.size() >= 2) {
        Node *left = PQ.top(); PQ.pop();
        Node *right = PQ.top(); PQ.pop();
        Node *merged = new Node(left->frequency + right->frequency, '\0', left, right);
        PQ.push(merged);
    }
    Node *root = PQ.top();
    this->root_node = root;
    dfs(root, "", table);
}

std::map<char, std::string> Encode::find_translation_table() {
    return table;
}
Node* Encode::find_root(){
    return this->root_node;
}