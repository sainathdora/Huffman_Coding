#include "Encode.h"
Encode::Encode(std::map<char, long long>mp) 
    :mp(mp){
        for(auto i:mp)std::cout<<i.first<<", "<<i.second<<'\n';
        }

