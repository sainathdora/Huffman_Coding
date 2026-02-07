#include "Encode.h"
#include "Decode.h"
#include <bitset>
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
    string temp = "";

    for (char bit : encoded_message)
    {
        temp += bit;

        if (temp.length() == 8)
        {
            bitset<8> bin(temp);
            char c = static_cast<char>(bin.to_ulong());
            cout << "Processing " << temp << " -> " << c << "\n";
            x.put(c);
            temp.clear();
        }
    }
    x.close();
    encodedmsg.close();
    infile.close();
    cout << "Encoding Message has been saved in: build\\EncodedMsg.txt\n";
    // ======================================== Part - 2 Decoding =====================================
    // Build the char and frequency table
    ifstream en_in("input.huff", ios::binary);
    if (!en_in)
        return 1;

    // 1. Read the first 8 bytes (Total Bits)
    long long total_bits_2;
    en_in.read(reinterpret_cast<char *>(&total_bits_2), 8);

    // 2. Read the 9th byte (Table Size)
    unsigned char table_size_byte;
    en_in.read(reinterpret_cast<char *>(&table_size_byte), 1);
    int table_size = static_cast<int>(table_size_byte);

    // 3. Build the Map (Character and Frequency)
    map<char, long long> freq_table;

    for (int i = 0; i < table_size; ++i)
    {
        char ch;
        int freq;

        // Read 1 byte for the character
        en_in.read(&ch, 1);

        // Read 4 bytes for the integer frequency
        en_in.read(reinterpret_cast<char *>(&freq), 4);

        freq_table[ch] = freq;
    }

    // Verification: Print the table
    cout << "Table Size: " << table_size << endl;
    for (auto const &[character, frequency] : freq_table)
    {
        cout << "'" << character << "': " << frequency << endl;
    }
    // First create the encoded object
    Encode encoded_obj_2(freq_table);
    // Now find the translation table
    encoded_obj_2.Start();
    map<char, string> translation_table_2 = encoded_obj_2.find_translation_table();
    // Now we have the translation table
    cout
        << "Decoding the: " << encoded_message << "\n";

    Decode decoder(encoder);
    decoder.Construct_original_msg(encoded_message);
    decoder.print_decoded_msg();
    return 0;
}