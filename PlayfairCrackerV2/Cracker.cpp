#include "Cracker.h"
#include <cmath>

void Cracker::decrypt(std::string& key) {

    for (size_t i = 0; i < m_ciphered.size(); i += 2) {

        auto c1 = m_ciphered[i];
        auto c2 = m_ciphered[i + 1];

        auto find_index = [&key](char c) {
            return std::distance(key.begin(), std::find(key.begin(), key.end(), c));
        };

        auto c1_idx = find_index(c1);
        auto c2_idx = find_index(c2);
        size_t c1_row = c1_idx / 5, c1_col = c1_idx % 5;
        size_t c2_row = c2_idx / 5, c2_col = c2_idx % 5;

        if (c1_row == c2_row) {
            m_text[i] = key[((c1_row * 5) + (c1_col + 4) % 5)];
            m_text[i + 1] = key[((c2_row * 5) + (c2_col + 4) % 5)];
        }
        else if (c1_col == c2_col) {
            m_text[i] = key[(((c1_row + 4) % 5) * 5) + c1_col];
            m_text[i + 1] = key[(((c2_row + 4) % 5) * 5) + c2_col];
        }
        else {
            m_text[i] = key[(c1_row * 5) + c2_col];
            m_text[i + 1] = key[(c2_row * 5) + c1_col];
        }

    }
}

double Cracker::simulated_annealing() {

    m_text.resize(m_ciphered.size());

    auto key_copy = m_key;
    std::string new_key;

    new_key.resize(key_copy.length());
    decrypt(key_copy);


    size_t counter = 0;
    auto max_score = utils::score_text(m_text);
    auto best_score = max_score;
    double score, delta, prob;

    // For each step, find our best key
    for (double step = ANNEALING_TEMP; step >= 40; step -= ANNEALING_STEP_SIZE) {

        for (auto count = 0; count < MAX_ITERATIONS; count++) {

            new_key = key_copy;
            utils::rearrange_key(new_key);
            decrypt(new_key);
            score = utils::score_text(m_text);
            // our difference between our current maxScore and step score
            delta = score - max_score;
            // We did work in the positive direction (hopefully...)
            if (delta >= 0) {
                max_score = score;
                key_copy = new_key;
            }
            else if (step > 0) {
                // the work we did is a side-grade
                prob = exp(delta / step);
                if (prob > 1.0 * rand() / RAND_MAX) {
                    max_score = score;
                    key_copy = new_key;
                }
            }
            // This is our best score so far
            if (max_score > best_score) {
                best_score = max_score;
                m_key = key_copy;
                std::cout << "Attempt n: " << counter << " Key: " << m_key << " Score: " << best_score << std::endl;
            }
            counter++;
        }
    }

    return best_score;
}

double utils::score_quad(const std::string& text)
{
    if (text.length() < 4) {
        std::cout << "Invalid text as parameter in score_quad(): " << text << std::endl;
        return 0.0;
    }

    int index[4];

    // Calculate the individual index for each character in the quadgram
    // Check if characters are within valid range ('A' to 'Z')
    for (int i = 0; i < 4; ++i) {
        if (text[i] < 'A' || text[i] > 'Z') {
            std::cerr << "Invalid character in quadgram: " << text[i] << std::endl;
            return 0.0;
        }
        index[i] = (text[i] - 'A') * std::pow(26, 3 - i);
    }

    return quadgram[index[0] + index[1] + index[2] + index[3]];
}

double utils::score_text(std::string& text)
{
    double out = 0.0;

    for (size_t i = 0; i < text.length() - 3; i++) {
        out += score_quad(text.substr(i, 4));
    }

    return out;
}

int utils::fix_text(std::string& text)
{
    text.erase(std::remove(text.begin(), text.end(), ' '), text.end()); // Remove spaces

    for (char& c : text) {
        if (!std::isalpha(c)) {
            std::cerr << "Invalid character: " << c << std::endl;
            return -1;
        }
        c = std::toupper(c);
    }

    return 0;
}

void utils::rearrange_key(std::string& key)
{
    // Lambda for swapping rows
    auto key_swap_rows = [&key](int r1, int r2) {
        for (int i = 0; i < 5; i++) {
            std::swap(key[r1 * 5 + i], key[r2 * 5 + i]);
        }
    };

    // Lambda for swapping columns
    auto key_swap_colums = [&key](int c1, int c2) {
        for (int i = 0; i < 5; i++) {
            std::swap(key[i * 5 + c1], key[i * 5 + c2]);
        }
    };

    // Lambda for swapping characters
    auto key_swap_chars = [&key](int i1, int i2) {
        std::swap(key[i1], key[i2]);
    };

    // Lambda for shuffling characters
    auto key_shuffle = [&key,&key_swap_chars ](int num) {
        for (int i = 0; i < num; i++) {
            key_swap_chars(rand() % 25, rand() % 25);
        }
    };

    // Decision logic for altering the key
    switch (rand() % 100) {
        case 1:
        case 2:
            key_shuffle(rand() % 26);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            key_swap_rows(rand() % 5, rand() % 5);
            break;
        case 7:
        case 8:
        case 9:
        case 10:
            key_swap_colums(rand() % 5, rand() % 5);
            break;
        default:
            key_swap_chars(rand() % 25, rand() % 25);
            break;
    }
}
