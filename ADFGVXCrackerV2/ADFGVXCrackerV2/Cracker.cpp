#include "Cracker.h"

void Cracker::do_work()
{
    mutex.lock();
    std::cout << "[+]thread " << m_tid << " started cracking length " << m_permutation.size() << std::endl;
    mutex.unlock();

	do {

		auto result = rearrange_text();
		auto ic = calculate_ic(result);

		if (ic > 0.05) {
			std::lock_guard<std::mutex> lock(mutex);
            info i;
            i.text = result;
            i._permutation = m_permutation;
			best_ics.emplace_back(ic, i);
		}

	} while (std::next_permutation(m_permutation.begin(), m_permutation.end()));

    std::cout << "[+]thread " << m_tid << " finished execution" << std::endl;
}


double Cracker::calculate_ic(const std::string& text)
{
    std::map<std::string, int> frequency;
    int bigrams = 0;

    for (size_t i = 0; i < text.length() - 1; i += 2) {
        std::string bigram = text.substr(i, 2);
        frequency[bigram]++;
        bigrams++;
    }

    //Calculate Index Of Coincidence
    double index = 0.0;

    for (const auto& elem : frequency) {
        index += (double)elem.second * (elem.second - 1);
    }

    index = (bigrams > 0) ? (index / ((double)bigrams * (double)(bigrams - 1))) : 0;

    return index;
}

std::string Cracker::rearrange_text()
{
    std::string text_cpy(m_ciphered_text);

    std::vector<int> first_colum;
    std::vector<int> last_colum;

    std::vector<std::string> colum_map(m_permutation.size());

    auto extra_chars = text_cpy.length() % m_permutation.size();
    int empty_space = (!extra_chars) ? 0 : (m_permutation.size() - extra_chars);

    for (int i = 0; i < m_permutation.size(); i++) {
        if (i < m_permutation.size() - empty_space)
            first_colum.push_back(m_permutation[i]);
        else
            last_colum.push_back(m_permutation[i]);
    }

    int colum_size = text_cpy.length() / m_permutation.size();

    int i = 0;
    int step = 0;
    int curr_pos = 0;

    while (curr_pos < (int)text_cpy.length()) {
        if (std::find(first_colum.begin(), first_colum.end(), i) != first_colum.end()) {
            step = colum_size + 1;
        }
        else {
            step = colum_size;
            text_cpy.insert((curr_pos + step), 1, ' ');
            curr_pos++;
        }

        curr_pos += step;
        i++;
    }

    short extra_char = (!empty_space) ? 0 : 1;
    for (int j = 0; j < m_permutation.size(); j++) {
        auto start = (m_permutation[j] * (colum_size + extra_char));
        colum_map[j] = text_cpy.substr(start, colum_size + extra_char);
    }

    std::string result;

    for (int i = 0; i < colum_map[0].size(); i++) {
        for (int j = 0; j < m_permutation.size(); j++) {
            result += (colum_map[j][i] != ' ') ? std::string(1, colum_map[j][i]) : "";
        }
    }
    return result;
}

void print_ic_results()
{
    auto best_ics = Cracker::get_ics();
        
    std::sort(best_ics.begin(), best_ics.end(), sort_by_ic);

    for (size_t i = 0; i < std::min(best_ics.size(), static_cast<size_t>(20)); ++i) {

        std::cout << "Key: {";
        for (int j = 0; j < best_ics[i].second._permutation.size(); j++) {
            std::cout << best_ics[i].second._permutation[j] << " ";
        }
        std::cout << "} IC: " << best_ics[i].first << " - Text: " << best_ics[i].second.text << std::endl;
    }
        
}
