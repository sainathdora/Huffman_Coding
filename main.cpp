#include "Encode.h"
using namespace std;

int main(){
    freopen("input.txt", "r", stdin);
    string s;
    cin>>s;
    cout<<s<<"\n";
    map<char, long long>char_freq;
    for(char i:s)char_freq[i]++;
    Encode encoder(char_freq);
    encoder.Start();
    for(auto i:encoder.find_translation_table()){
        cout<<i.first<<" : "<<i.second<<"\n";
    }
    return 0;
}