#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        std::vector<unsigned long long> convertedValues(5, 0);  // Store up to 5 base-36 converted numbers
        std::string cleanedStr;
        
        // Normalize the string (convert to lowercase, remove non-alphanumeric)
        for (char ch : k) {
            if (std::isalnum(ch)) {
                cleanedStr += std::tolower(ch);
            }
        }

        int strLen = cleanedStr.length();
        int numGroups = (strLen + 5) / 6;  // num of 6-character groups

        // Process each 6-character group starting from the end
        for (int i = 0; i < numGroups; i++) {
            int startPos = std::max(0, strLen - 6 * (i + 1));
            int endPos = strLen - 6 * i;
            std::string group = cleanedStr.substr(startPos, endPos - startPos);

            // Convert this part to a base-36 number
            unsigned long long val = 0;
            unsigned long long power = 1;

            // Work from right to left (lowest power to highest)
            for (int j = group.size() - 1; j >= 0; --j) {
                val += letterDigitToNumber(group[j]) * power;
                power *= 36;
            }

            convertedValues[4 - i] = val;  // Store from the right (convertedValues[4] is last group)
        }

        // compute final hash value using the r-values
        HASH_INDEX_T resultHash = 0;
        for (int i = 0; i < 5; ++i) {
            resultHash += convertedValues[i] * rValues[i];
        }

        return resultHash;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        if (std::isdigit(letter)) {
            return static_cast<HASH_INDEX_T>(letter - '0' + 26);
        }
        return static_cast<HASH_INDEX_T>(letter - 'a');
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
