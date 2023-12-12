#include <iostream>
#include <cctype>
#include "Cracker.h"
#include "Cipher.h"

Cipher Cipher::m_instance;

std::vector<std::pair<double, info>> Cracker::best_ics;
std::mutex Cracker::mutex;

std::vector<std::pair<std::string, double>> Cracker::letter_frequencies{
		{"E", 0.1202}, {"T", 0.0910}, {"A", 0.0812}, {"O", 0.0768}, {"I", 0.0731},
		{"N", 0.0695}, {"S", 0.0628}, {"R", 0.0602}, {"H", 0.0592}, {"D", 0.0432},
		{"L", 0.0398}, {"C", 0.0271}, {"U", 0.0288}, {"M", 0.0261}, {"F", 0.0230},
		{"P", 0.0182}, {"G", 0.0203}, {"W", 0.0209}, {"Y", 0.0211}, {"B", 0.0149},
		{"V", 0.0111}, {"K", 0.0069}, {"X", 0.0017}, {"J", 0.0010}, {"Q", 0.0011},
		{"Z", 0.0007}
};

int main()
{
	
	std::string text = "FXGXFXXGGFAXAGAGGGFXGADDFVXXDFGVXGFXAXXAAVAVAVGAGDDXGGADDGGDVAGGAVDVXDAGGAGFGGAFXVXXAFFGFVFFFVFDGFGADAAXFGFFADDXGFGXGFGFGXFFGGXGGVXXAXGVFXFGDFADGVAGAFAVAFAFAFFGFXFXDVXVDXGVDFXAGVGAAAAFAGAVGFGXFGXXAGVFDGAXFGGXFDGAXVAAXVAVDFAVGGAFAFAXDADDFGGGGGFGFGDXGFGVGXAFFXFGAGGVAVGGGGFVFXGXXFGGAGFXFVDGGGGXXGAVVVFVFGFVGXAFDVFGGAFGFDXAAVDAAXXFGAGVVAGAAGDXGFGXXXAGGFADGGXFVGDGFFGGFGFXAGGFGFAFVAGVXGGGGXVVDGAGFGGVFVGGVXFGFVAXGFGFGVGXAFGAGAGXGFGXGXGXVVGVGFDAXAGFGVGDGFXFADFFGFGGFFXXADFFGDFXAGGXAXGVAADGAFAFFVDFGAGGADGDFXDXGFVFFXGXXXGFGFXVFGGGVXGFAXGAGAGXAFDAAVAXGDAFFXDVGFXDAXGVAAGVAFXGFXFDGXFXGFGXAVGXFDDXVGGAFVGFADDVGXGGFDVAGXGVDDAFAFAXFFDFGGGFFFFFVFGXFXGGDFAXFFFDGAGXFXXVAGAFFXFDFGFXVAAXFAFXGXGAGVAAAGGXXVFADVDXFFGVDXGXAFGFDDGGAFFDFGAFXFGGGGGVGFAVFGGXFGAVFAFXGFAVAGDGGFAVFXGXXXFVGFGAGXXDGGFAFGFXGGFGXXGXGGXGFGFAGVAAGXAVGXAGDXAVFXXXFXAADAGXAFAFDVGVGXFXDGDXGVAFAGAXGVGDGXGGFGAVVAGAADGVFXGGGDGVFXADGFDFAFGAAXVFVXGGDDGXFAGVFGXADAGFAVFXDFFXXGFFAAAGGFVXGVAAAGAXFXGGGAGXXFDGAGGFFDXGDAGVAVXXGGXGGXADFGFXFVGFGVXFGAVAGXFGVXFAFVAVGGXGXFAGFADAGGGGFFGGAGFXAXVAADDXAXGVAXGXFXVAGGGGDXGFAXAGAFAFFAGAGAGXFDGGFGAFGXGFAXFFFXFGGFADFGGFGXFAGAXXVAGXFVAXGFGFGGAAAXAVGFDADFXFFADXXADXFGVGGXDFGGGFGVGADXGFFVGAVDAFFAGFFXVVAFGVFGAVXGAXDFDVAFGFDXAXFDFVDXGFGVGXGXAFGXGFAVXGXVGXFVAXFAGXVVDFAVFFADFGAVGXGVAAFFVAGVGVGFAGFVFXGGGDGVGVFAGAGGFGGDADFVFXDVDXDFGVFXGFAXAGDXFFGGFFGAXDDGXFAXXFXVVXGDFGXXFFGFFVFVADAFGFDXGVGDGXGVGGGVXAAGGGXGAAVGGGGVFAGAGAFAGGGDGAFGFGDFFFAGVGXFFFVXGAAFAFDGXGXGGAAGXGDFXGGXXAGFFXVGAAFDDFADAAGXFFVFAXFXFAXFFAFFDGAAVGXGGGXGXXAGGVXXFDAAXDFGAGXGGFXFXXFXVAGGGXXGFAFVFFVDDFFAFAAGGAVDDGVFFXXGFDDXDFXDVGFGXAVFXFAADVGAGDXXADFFXFGFFFAADXXAVGDXAFVAFFGGXAVGFGAAXGXVAXDAFAVGXAVFAAFAAGXAFDVAVGXGVAAFFXGDFAGXAXAAXAGAGDFAXAXAVGFGGFGFXFAGXXDAXDGGAFVFGGXGFGFGGGAFDDVXGXGFFDXXXGFGFAAAVGAGFAGAFGGAAAAGXDXVDGFAVGVGAAFGVVAAAGXFVGXDFFVGAFFGGFVXFAFGDGGFGAGAVFGAXXDADAGDFXVGXGAXVGXGVGFAGGGGFAAGXAXDGVDGVFFFVDFGAFAAXVFGDADGXFVGFAAGAAGAAAXAAGFAGDVGXGVGGFFGXFFGVVVGXFGFGAFVVXDAAADGFFVGAGGGXGXAGFFVAAAAGAAFXGDFFGDFGFAXGAVDXFFGGADFXFAFXDADFFVXAXGGGGVAAGXADGGAFAAXXAVAVGVDXFXGGXAGXADGADAAXGVGGAVAGFFDXGGAAAGFVDGGVFAAVAFFGGDFAAADFAXAVDDDGFXDADXGFXGAFFXFGFXDXGAGAGVXGAFAVDDDFAVXGFVFDAVAGFVGXGAAAXAXXDAAGFFGXFFAVFVGAGXGFAXFFFAAAFXFVXXADAXAGFXGVFDGGGDAVXFGVGDAFGXAXGGAVVDGDAAGDGFGXAFGXFXAFAFAGGDAGGGXGGVVXFXFXFGGVFVAFGAXFGVDDDVFFGDXXXGGAXFGXDXAXGGGVFFGXGXGVFGGGXGFFAXFVXVGVGAVVGVFAVAGAGFXXGVVADXFVFVGFADDFADGXXDGFAVGGXFDAFVAFFXFGAAAXFADXGGFFGVFVGAXAXAAVGXAXFXGAAVADFGFAGXGAXADVGGAXGXAAADAFGDDDGAGDXXGAGXXGGFXGADFAFFGVXGGDXXAVGAGFGFGFAFAXGDXFGAFXAAFVGVFXXFXXXDGADGFXFXGAAVDAGVXFAAFXDVAAAADGXGDGGXFXGVFXFAFXGGAFGGGVFGDXFXGGFAGVAFXXDDGDGFAVGAGADDADAGFGAGGXGFGFGAAFGAGDFGGVADAVAVAFGVGDAGFXFFGXAGDVGXDGFGXFFXGGVFADDFGFXGFGFXGVGXGGGGFXGGAFAAFVAFFVDVAGFXXAFVAADGDFGVGVFGFFDVGAAXXGAAFGGGAXAXFVDDGAFVGXFGAGGFFDAAFGXFGAVAAGXFAAAGFDVGFDVFXGDFGGFADGVGXGGFFFDDGDGFFGGADADFXXAGAFFAXFVGFFDFXAGAFFDFGFAXXFXAGGVXAGVGVAVGVFDXDDGGAGGGXXDFGAFDFVFGFAVFGGVAXGXFGGAAADVGAVFAFAFGGDFGVFGDGGVAGAAVXXXGDGFFADXAGFAFGGGFGGAAVFVGGFXGGFXGDAFDGFXAGDFGXFFAXGFGDVGVVFFGGAGXDGDAXAXGXAXGGGDFGAFFXFGFVFXDFAVAGGXGDFVDVGDFFGXAGDVGXDDGVDAGXGGFXGGDAFGGXGFADGGGVAVFGAVGXFDFXFXFFFAGXGVFGGAAGGXDXFGDFVAGFAXFXAAGVVFFGDGXGDVDXFADXXFDFAAAVAXGADGAXGXXVAXDXXGGADGGVAGXDGFXDGFGFFXVFDGFXGXXXGGGGDAFXFXDVGFAVFXFGFAAAAAGGGXGFGVFFAVAFVAXGFADAGDGGAGGGGDGXFAXGFFAXFDFXAXGGDXGXXFGFGFAVDDDVAFAFAVXFGVGGFXAAGAAAGXGVDDGVGXGGFFDXFFFFXFGGAGFXFVFXAAGFDAFVADDXAFGAAVAGXDGVFAGAAXXFADGDGAGVAVFGFDGAXFFVGGGXAFGVFVFDAGGAFAGVXDFGAVGVAFFGXVFGDXFXFGXVFVAVGVGFDGFXGAXFGVXDFDGVVFFDFFFGAXADAAFFXGGVFAAVVAFAADFXAFFVAVDVGVAGGVAVDFFVAAGFGVDGFXFXGVFAXFFVXXGAAVGXGVGGDVAXGFAAGXAXGGGFAFGVXXGAGAXDXGAGFXXAAFGDAAXFGGADDAGFFGAAGDGGGGXGFFXFAVAAGFGAGAGXDGGVFXDGAVAFFAGXXGGXVXGGAFFGXXAXXXGDGAGVFVXGAXGDFDAVGAGDVVAXDXFGFFAFAXAFAFGDGGXGAFGFAFFXFDXGAFGAFFVDAFXGFXFDGXFFAGFGGAGGFFGAFXAGGXGAGGFXAXDGGVDFAXGXFVXAGXDFGVAVGVAFXVGVXXVFADAGDXFFAFAVAXAVFAFAXFFGGGGFGGAAFAFFAFXVFXFGGXADFAGGFVAAGGAFAVGVAAGDGGXXDVFXGVGGAGXDGVXDGFGGFFDFADADGXAXXDAFGDGAAXGXXADAAFDVGFADFFVAFAGXAFGGGAFGGXFVAADXDFXXGVGDADXGDXGAGFGVFAFDFFFXAVXFFGGFGFXADXFXXADVGAXGFFGXDDGFFGGGFVGVAVXXGGXGXXGFDFFAFGDVVVAVFFFFAVDVFFXFAADFGGXAXGDDGDGDVAAGGVAAGAGXGVGFGVGFGXADDFXGDXGXXXXVAGFXGAFVDXFVFFGFFVGDXFVFFXFXFFAFGXGAAFXXGFXGGFGADADAGFAGDFVAXGGAXGADAGAFXAXXAAVFXFFDDDXDVFVXGGGGGAGDAVXAXVFAFGXADGVAVDGAVGFAAGVFAFXGGGVVAGAAXDVDXFGXGFGXVXGDVGFGFXADXXVGDGXXVFGXGGGAXFFFGFFXXAGGGAAFGDGAGGAAVAFFADXAXAXXVGVGGDFXVGGGXXXXGGAGDDVAVFXXVAFGFGVAXGFFFDFXGFXGFFAFVGFAXGVGGGVDXXAAFFXDGAGGXGFFVGXGFFXXAGVXGFFVAAAAGXFFXAFGGDDAXXDAGGVAGXVGVFGGVADFVGVAFAVDFFDFXFXFXGXXVGVGGFGGADVGAFXAVGDAXGVGVGFFFAFFXAAAFGVFXFAFFAFGFDXAGAVFAFFGVFFGXFFXGGXXFAXFXFXAFAFFVAGGGDVGFAGFXGXFXXVGDDGFVGAFFXDDVGXXGFVGDAGXGGAGVGAGXFVDXGAGVFGGVFGGFVAGGDADVXVFGAADGDXFDGFXVFGGFAFFAXGAAAXGGVFAVXDGXXXAXGFGVGFGXGVDGGFFXDGAXXAFGFVGFADDFADGGGXGAGFDXAFGDAFXXGXGVAFDGFGDDGGFVGAGVDVFGFAGDAGXGDXFXAXGDVDGGXFGGXAXAAFGGVXDGDGFXVGFAAAFDAFGGVXAAGGAXXVAGAXAXG";
	
	std::cout << "ADFGVX CRACKER V2\n\n[-]starting threads..." << std::endl;
	Cracker c1(text, 6);
	Cracker c2(text, 7);
	Cracker c3(text, 8);
	Cracker c4(text, 9);
	Cracker c5(text, 10);

    while (true) {
        std::cout << "Press Enter to continue or 'q' to quit." << std::endl;

        char key = std::cin.get();

        if (key == 'q') {
            break;
        }
        else if (key == '\n') {
            auto ics = Cracker::get_ics();
            for (short i = 0; i < ics.size(); i++) {
                std::cout << "[" << i << "] -> IC: " << ics[i].first << " " << "Permutation Lenght: "<< ics[i].second._permutation.size() << "-> { ";
                for (short j = 0; j < ics[i].second._permutation.size(); j++) {
                    std::cout << ics[i].second._permutation[j] << " ";
                }
                std::cout << "}" << std::endl;
            }

            std::cout << "Select a number: ";
            int selected;
            if (std::cin >> selected) {
                if (selected < 0 || selected >= ics.size()) {
                    std::cout << "Invalid number" << std::endl;
                }
                else {
                    std::cout << "[+] Applying substitution based on frequency" << std::endl;
                    auto ciphered_freq = Cracker::calc_bifreq(ics[selected].second.text);
                    auto words_freq = Cracker::get_words_freq();

                    std::vector<std::pair<std::string, char>> substitution;
                    substitution.reserve(ciphered_freq.size());

                    for (int i = 0; i < ciphered_freq.size(); i++) {
                        std::pair<std::string, char> pair;

                        if (i < words_freq.size()) {
                            pair.first = ciphered_freq[i].first;
                            pair.second = words_freq[i].first[0];
                        }
                        else {
                            pair.first = ciphered_freq[i].first;
                            pair.second = '!';
                        }

                        substitution.push_back(pair);
                    }

                    auto ciphered = ics[selected].second.text;

                    std::cout << "\n\nCiphered text: " << ciphered << std::endl;
                    std::cout << "\nSubstitution:" << Cracker::substitute(ciphered, substitution) << std::endl;

                    std::cout << "Substitution map: \n" << std::endl;
                    for (auto& i : substitution) {
                        std::cout << i.first << ":" << i.second << std::endl;
                    }

                    std::cout << "Do you want to change association? Y/N" << std::endl;
                    std::cin.ignore(); // Ignore newline left in the buffer
                    std::cin.get(key);

                    while (tolower(key) == 'y') {
                        std::string bigram;
                        char new_char;

                        // Get user input for the bigram they want to change
                        std::cout << "Enter the bigram to change: ";
                        std::cin >> bigram;

                        // Get user input for the new character
                        std::cout << "Enter the new character for " << bigram << ": ";
                        std::cin >> new_char;

                        // Update the substitution map
                        for (auto& pair : substitution) {
                            if (pair.first == bigram) {
                                pair.second = new_char;
                                break; // Break if we've found and updated the bigram
                            }
                        }

                        // Optional: Show the updated substitution map
                        std::cout << "Updated substitution map: \n";
                        for (auto& i : substitution) {
                            std::cout << i.first << ":" << i.second << std::endl;
                        }

                        // Ask if the user wants to make more changes
                        std::cout << "Do you want to make another change? Y/N" << std::endl;
                        std::cin.ignore(); // Ignore newline left in the buffer
                        std::cin.get(key);
                    }

                    std::cout << "\nSubstitution:" << Cracker::substitute(ciphered, substitution) << std::endl;
                }
            }
            else {
                std::cout << "Invalid input" << std::endl;
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the rest of the line
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    return 0;
}


