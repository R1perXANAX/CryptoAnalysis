#include "Cipher.h"

std::string Cipher::encrypt(const std::string& key) {
    std::string intermediate_text;

    // Phase 1: Substitution
    for (char c : m_clear_text) {
        auto [row, col] = get_position(toupper(c));
        if (row == -1)
            continue;
        intermediate_text += m_symbols[row];
        intermediate_text += m_symbols[col];
    }

    std::cout << "Substitution result: " << intermediate_text << std::endl;
    // Phase 2: Transposition
    int cols = key.size();
    int rows = (intermediate_text.size() + cols - 1) / cols; // -1 to remove any remainder from the division
    std::vector<std::vector<char>> table(rows, std::vector<char>(cols, ' '));

    int idx = 0;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (idx < intermediate_text.size()) {
                table[row][col] = intermediate_text[idx++];
            }
            else {
                // If idx is out of range, break out of the inner loop.
                break;
            }
        }
    }


    std::cout << "Intermediate Table:" << std::endl << std::endl;
    for (int col = 0; col < cols; ++col) {
        std::cout << key[col] << ' ';
    }

    std::cout << std::endl;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            std::cout << table[row][col] << ' ';
        }
        std::cout << std::endl;
    }


    std::map<char, std::string> colum_map;
    for (int col = 0; col < cols; ++col) {
        for (int row = 0; row < rows; ++row) {
            colum_map[key[col]] += table[row][col];
        }
    }

    for (auto& pair : colum_map) {

        m_ciphered_text += pair.second;
    }

    return m_ciphered_text;
}