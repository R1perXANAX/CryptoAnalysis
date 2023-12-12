#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

class Cipher
{
private:
    const char m_matrix[6][6] = {
    {'F', 'L', '2', 'W', 'H', '0'},
    {'M', 'E', '6', 'Y', 'B', 'J'},
    {'G', '3', 'R', 'Q', 'V', 'O'},
    {'Z', '7', 'I', 'S', 'U', 'P'},
    {'4', 'T', '9', 'K', 'D', '5'},
    {'N', 'A', '8', '1', 'X', 'C'}
    };

    const std::string m_symbols = "ADFGVX";
    std::string m_ciphered_text;
    std::string m_clear_text;

    static Cipher m_instance;
private:
    Cipher() = default;
    Cipher(auto text) : m_clear_text(text) {};

    __inline std::pair<int, int> get_position(char c) {
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                if (m_matrix[i][j] == c) {
                    return { i, j };
                }
            }
        }
        return { -1,-1 };
    }

public:
    
    void operator=(const Cipher&) = delete;

    __inline static Cipher& get_istance() {
        return m_instance;
    }
    
    __inline void set_text(std::string&& text) {
        m_clear_text = text;
    }

    std::string encrypt(const std::string& key);
};

