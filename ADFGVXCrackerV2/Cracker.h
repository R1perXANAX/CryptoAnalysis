#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <algorithm>
#include <map>
#include <mutex>

#define ENGLISH_IC  0.0667
#define MAX_IC 40

static int t_count;

struct info {
    std::vector<int>_permutation;
    std::string text;
};

__inline bool sort_by_ic(const std::pair<double, info>& a, const std::pair<double, info>& b) {
    return std::abs(a.first - ENGLISH_IC) < std::abs(b.first - ENGLISH_IC);
}

void print_ic_results();

class Cracker
{
private:
    std::vector<int>m_permutation;
    std::thread m_running_thread;
    std::string m_ciphered_text;
    int m_tid;
    
    static std::mutex mutex;
    static std::vector<std::pair<double, info>> best_ics;
    static std::vector<std::pair<std::string, double>> letter_frequencies;

private:

    void                                                            do_work();
    double                                                          calculate_ic(const std::string& text);
    std::string                                                     rearrange_text();
    
    
public:
    Cracker(std::string& ciphered_text, int key_lenght) {
        t_count++;
        m_tid = t_count;

        m_ciphered_text = ciphered_text;
        for (int i = 0; i < key_lenght; i++)
            m_permutation.push_back(i);

        m_running_thread = std::thread(&Cracker::do_work, this);
    }

    ~Cracker() {
        if (m_running_thread.joinable())
            m_running_thread.join();
        t_count--;
    }

    __inline static std::vector<std::pair<double, info>>            get_ics() {
        std::lock_guard<std::mutex> lock(mutex);
        return best_ics;
    }

    __inline void wait() {
        if (m_running_thread.joinable())
            m_running_thread.join();
    }

    static std::vector<std::pair<std::string, double>>              calc_bifreq(const std::string& text);
    static std::string                                              substitute(const std::string& ciphered, const std::vector<std::pair<std::string, char>>& bimap);

    __inline static std::vector<std::pair<std::string, double>>& get_words_freq() {
        return letter_frequencies;
    }
};
