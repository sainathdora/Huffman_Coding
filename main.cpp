#include "Encode.h"
#include "Decode.h"
#include <fstream>
using namespace std;

int main()
{
    ifstream infile("input.txt", ios::binary);
    char ch;
    map<char, long long> char_freq;
    string s;
    while (infile.get(ch))
    {
        s += ch;
        char_freq[ch]++;
    }
    Encode encoder(char_freq);
    encoder.Start();
    map<char, string> translation_table = encoder.find_translation_table();
    ofstream encodedmsg("EncodedMsg.txt");
    if (!encodedmsg)
    {
        cout << "Couldn't open the file\n";
        return 0;
    }
    string encoded_message = "";
    for (char i : s)
    {
        encodedmsg << translation_table[i];
        encoded_message += translation_table[i];
    }
    ofstream x("input.huff");
    // store the first 8 bytes to store encoded message size or no of bits
    long long total_bits = encoded_message.size();
    x.write(reinterpret_cast<char *>(&total_bits), 8);
    cout << "bits = " << total_bits << "\n";
    // Now store the table size/no of unique characters;
    unsigned char no_of_unique_chars = translation_table.size();
    x.put(no_of_unique_chars);
    // Now store chuncks(char + Int)
    for (auto i : char_freq)
    {
        // encode the char
        x.put(i.first);
        // now encode the integer
        x.write(reinterpret_cast<char *>(&i.second), 4);
    }
    // Now write the bits into the file

    cout << "Encoding Message has been saved in: build\\EncodedMsg.txt\n";

    cout << "Decoding the: " << encoded_message << "\n";

    Decode decoder(encoder);
    decoder.Construct_original_msg(encoded_message);
    decoder.print_decoded_msg();
    return 0;
}