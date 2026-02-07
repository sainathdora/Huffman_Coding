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
    long long total_bits_2;
    en_in.read(reinterpret_cast<char *>(&total_bits_2), 8);

    unsigned char table_size_byte;
    en_in.read(reinterpret_cast<char *>(&table_size_byte), 1);
    int table_size = static_cast<int>(table_size_byte);

    // Build the Map(Character and Frequency)
    map<char, long long>
        freq_table;

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
    // Now we have the translation table, but dont need it tho
    // Find the encoded string
    string bit_stream = "";
    unsigned char byte;
    long long bits_read = 0;

    // Read byte by byte until the end of the file
    while (en_in.read(reinterpret_cast<char *>(&byte), 1))
    {
        // Process each of the 8 bits in the byte
        for (int i = 7; i >= 0; --i)
        {
            if (bits_read < total_bits)
            {
                // Check if the i-th bit is set
                if ((byte >> i) & 1)
                {
                    bit_stream += '1';
                }
                else
                {
                    bit_stream += '0';
                }
                bits_read++;
            }
            else
            {
                // We have reached the total_bits, ignore any remaining padding in this byte
                break;
            }
        }
    }

    cout << "Successfully read " << bit_stream.length() << " bits into the string." << endl;

    Decode decoder(encoded_obj_2);

    decoder.Construct_original_msg(bit_stream);
    string ded = decoder.decoded_message;
    en_in.close();
    // Open a new file
    ofstream dedcoded_file("out.txt");
    dedcoded_file << ded;
    dedcoded_file.close();
    cout << "=======================================Bench Marks===================\n";
    ifstream in("input.txt", ios::binary | ios::ate);
    streamsize size = in.tellg();
    cout << "Orignal file size = " << size << " Bytes\n";
    ifstream comp("input.huff", ios::binary | ios::ate);
    streamsize size_comp = comp.tellg();
    cout << "Compressed File Size = " << size_comp << " Bytes\n";
    double change = ((double)size - (double)size_comp) / (size);
    cout << "Change(%) = " << change * 100 << "\n";
    in.close();
    comp.close();
    return 0;
}