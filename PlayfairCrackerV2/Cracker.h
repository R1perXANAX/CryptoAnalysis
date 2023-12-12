//
// Created by Irene Erba on 12/12/23.
//

#ifndef NUOVOPLAYFAIR_CRACKER_H
#define NUOVOPLAYFAIR_CRACKER_H
#include <iostream>
#include <string>
#include <algorithm>
#include "quadgram.h"


#define MAX_ITERATIONS 10000
#define ANNEALING_STEP_SIZE 0.5
#define ANNEALING_TEMP 50

namespace utils {
    double		score_quad(const std::string& text);
    double		score_text(std::string& text);
    int			fix_text(std::string& text);
    void		rearrange_key(std::string& key);
}

class Cracker
{
private:
    std::string m_key;
    std::string m_ciphered;
    std::string m_text;


private:
    Cracker(std::string key, std::string ciphered) : m_key(key), m_ciphered(ciphered) { std::cout << "Creating istance with key: " << key << " and ciphertext: " << ciphered; };

    // Delete copy constructor and assignment operator
    Cracker(const Cracker&) = delete;
    Cracker& operator=(const Cracker&) = delete;

private:


    void		decrypt(std::string& key);
    double		simulated_annealing();

public:

    inline static Cracker& get_istance(std::string &&key, std::string &&ciphered) {
        utils::fix_text(ciphered);
        static Cracker instance(key, ciphered);
        return instance;
    }

    __inline void crack() {

        srand(time(NULL));
        simulated_annealing();
        decrypt(m_key);
    }

    __inline std::string& get_text() { return m_text; }
};


#endif //NUOVOPLAYFAIR_CRACKER_H
