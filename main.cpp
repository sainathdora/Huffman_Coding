#include "Encode.h"
#include "Decode.h"
#include <bitset>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main()
{
    // ======================================== Part 1: Encoding =====================================
    string input_path = "input.txt";
    ifstream infile(input_path, ios::binary);
    if (!infile)
    {
        cout << "Error: Could not open input.txt\n";
        return 1;
    }

    char ch;
    map<char, long long> char_freq;
    string original_content = "";
    while (infile.get(ch))
    {
        original_content += ch;
        char_freq[ch]++;
    }
    infile.close();

    Encode encoder(char_freq);
    encoder.Start();
    map<char, string> translation_table = encoder.find_translation_table();

    string encoded_bit_string = "";
    for (char i : original_content)
    {
        encoded_bit_string += translation_table[i];
    }

    ofstream x("input.huff", ios::binary);

    // 1. Store total bits (8 bytes)
    long long total_bits = encoded_bit_string.size();
    x.write(reinterpret_cast<const char *>(&total_bits), 8);

    // 2. Store number of unique characters (1 byte)
    unsigned char no_of_unique_chars = static_cast<unsigned char>(translation_table.size());
    x.put(no_of_unique_chars);

    // 3. Store Frequency Table (Char + Int(4 byte) Freq)
    for (auto const &[character, freq] : char_freq)
    {
        x.put(character);
        int freq_small = static_cast<int>(freq);
        x.write(reinterpret_cast<const char *>(&freq_small), 4);
    }

    // 4. Write Bits (Packed into bytes)
    string temp = "";
    for (char bit : encoded_bit_string)
    {
        temp += bit;
        if (temp.length() == 8)
        {
            bitset<8> bin(temp);
            x.put(static_cast<unsigned char>(bin.to_ulong()));
            temp.clear();
        }
    }
    // Handle remaining bits (Padding)
    if (!temp.empty())
    {
        while (temp.length() < 8)
            temp += '0';
        bitset<8> bin(temp);
        x.put(static_cast<unsigned char>(bin.to_ulong()));
    }
    x.close();

    // ======================================== Part 2: Decoding =====================================
    ifstream en_in("input.huff", ios::binary);
    if (!en_in)
        return 1;

    long long total_bits_2;
    en_in.read(reinterpret_cast<char *>(&total_bits_2), 8);

    unsigned char table_size_byte;
    en_in.read(reinterpret_cast<char *>(&table_size_byte), 1);
    int table_size = static_cast<int>(table_size_byte);

    map<char, long long> freq_table;
    for (int i = 0; i < table_size; ++i)
    {
        char c_key;
        int f_val;
        en_in.read(&c_key, 1);
        en_in.read(reinterpret_cast<char *>(&f_val), 4);
        freq_table[c_key] = f_val;
    }

    Encode encoded_obj_2(freq_table);
    encoded_obj_2.Start();

    string bit_stream = "";
    unsigned char byte_read;
    long long bits_processed = 0;

    while (bits_processed < total_bits_2 && en_in.read(reinterpret_cast<char *>(&byte_read), 1))
    {
        for (int i = 7; i >= 0; --i)
        {
            if (bits_processed < total_bits_2)
            {
                bit_stream += ((byte_read >> i) & 1) ? '1' : '0';
                bits_processed++;
            }
        }
    }
    en_in.close();

    Decode decoder(encoded_obj_2);
    decoder.Construct_original_msg(bit_stream);

    ofstream dedcoded_file("out.txt");
    dedcoded_file << decoder.decoded_message;
    dedcoded_file.close();

    // ======================================== Benchmarks =====================================
    ifstream original_f(input_path, ios::binary | ios::ate);
    ifstream compressed_f("input.huff", ios::binary | ios::ate);

    streamsize size_orig = original_f.tellg();
    streamsize size_comp = compressed_f.tellg();

    cout << "\n--- Benchmarks ---\n";
    cout << "Original size:   " << size_orig << " bytes\n";
    cout << "Compressed size: " << size_comp << " bytes\n";

    if (size_orig > 0)
    {
        double ratio = (1.0 - (double)size_comp / size_orig) * 100.0;
        cout << "Space Saved:     " << ratio << "%\n";
    }

    return 0;
}