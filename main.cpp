#include "Encode.h"
#include "Decode.h"
#include <fstream>
using namespace std;

int main(){
    freopen("input.txt", "r", stdin);
    string s;
    cin>>s;
    map<char, long long>char_freq;
    for(char i:s)char_freq[i]++;
    Encode encoder(char_freq);
    encoder.Start();
    map<char, string>translation_table = encoder.find_translation_table();
    ofstream encodedmsg("EncodedMsg.txt");
    if(!encodedmsg){
        cout<<"Couldn't open the file\n";
        return 0;
    }
    string encoded_message = "";
    for(char i:s){
        encodedmsg<<translation_table[i];
        encoded_message += translation_table[i];
    }
    cout<<"Encoding Message has been saved in: build\\EncodedMsg.txt\n";


    cout<<"Decoding the: "<<encoded_message<<"\n";
    
    Decode decoder(encoder);
    decoder.Construct_original_msg(encoded_message);
    decoder.print_decoded_msg();
    return 0;
}