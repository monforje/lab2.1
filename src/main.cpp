#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include "ac.hpp"
#include "kmp.hpp"
#include "file.hpp"

const std::string KMP_RESULT_FILE = "../data/kmp_result.txt";
const std::string AC_RESULT_FILE = "../data/ac_result.txt";

void writeHeader(std::ofstream& out, const std::string& header) {
    out << "===========================================\n";
    out << header << "\n";
    out << "-------------------------------------------\n";
    out << std::left << std::setw(10) << "Line" 
        << std::setw(40) << "Data" 
        << std::setw(20) << "Match Index\n";
    out << "-------------------------------------------\n";
}

void writeFooter(std::ofstream& out, double time_ms) {
    out << "-------------------------------------------\n";
    out << "Execution time: " << time_ms << " ms\n";
    out << "===========================================\n\n";
}

int main() {
    std::string filename = "../data/data.txt";

    // Открываем файлы для записи результатов
    std::ofstream kmpFile(KMP_RESULT_FILE);
    std::ofstream acFile(AC_RESULT_FILE);

    if (!kmpFile.is_open() || !acFile.is_open()) {
        throw std::runtime_error("Failed to open output files");
    }

    // Загружаем данные из файла
    std::vector<std::vector<std::string>> words = LinesWithWords(filename);

    // === ПОИСК С ИСПОЛЬЗОВАНИЕМ КМП ===
    auto startKMP = std::chrono::high_resolution_clock::now();

    std::vector<std::string> kmp_patterns = {"2720", "628", "4", "Щ", "Я"}; // Пример паттернов
    bool kmp_has_matches = false;

    for (size_t i = 0; i < words.size(); ++i) {
        const auto& v = words[i];
        if (v.size() != 3) continue;

        for (size_t j = 0; j < 3; ++j) {
            for (const auto& pattern : kmp_patterns) {
                size_t match_count = 0;
                size_t* matches = kmp_search(v[j], pattern, &match_count);

                if (match_count > 0) {
                    if (!kmp_has_matches) {
                        writeHeader(kmpFile, "KMP Search Results");
                        kmp_has_matches = true;
                    }

                    for (size_t k = 0; k < match_count; ++k) {
                        kmpFile << std::left << std::setw(10) << i + 1
                                << std::setw(40) << v[j]
                                << std::setw(20) << matches[k] << "\n";
                    }
                }

                free(matches);
            }
        }
    }

    auto endKMP = std::chrono::high_resolution_clock::now();
    double timeKMP = std::chrono::duration<double, std::milli>(endKMP - startKMP).count();

    // Добавляем время выполнения в файл, только если были найдены совпадения
    if (kmp_has_matches) {
        writeFooter(kmpFile, timeKMP);
    }

    kmpFile.close();

    // === ПОИСК С ИСПОЛЬЗОВАНИЕМ АХО-КОРАСИКА ===
    auto startAC = std::chrono::high_resolution_clock::now();

    std::vector<std::string> aho_patterns = {"6", "2", "8", "7"}; // Пример паттернов
    bool ac_has_matches = false;

    for (size_t i = 0; i < words.size(); ++i) {
        const auto& v = words[i];
        if (v.size() != 3) continue;

        for (size_t j = 0; j < 3; ++j) {
            bool all_found = AhoSearch(v[j], aho_patterns, aho_patterns.size());

            if (all_found) {
                if (!ac_has_matches) {
                    writeHeader(acFile, "Aho-Corasick Search Results");
                    ac_has_matches = true;
                }

                acFile << std::left << std::setw(10) << i + 1
                        << std::setw(40) << v[j]
                        << std::setw(20) << "All patterns found\n";
            }
        }
    }

    auto endAC = std::chrono::high_resolution_clock::now();
    double timeAC = std::chrono::duration<double, std::milli>(endAC - startAC).count();

    // Добавляем время выполнения в файл, только если были найдены совпадения
    if (ac_has_matches) {
        writeFooter(acFile, timeAC);
    }

    acFile.close();

    std::cout << "\nResults written to:\n";
    if (kmp_has_matches) {
        std::cout << " - KMP results: " << KMP_RESULT_FILE << "\n";
    } else {
        std::cout << " - No KMP matches found.\n";
    }

    if (ac_has_matches) {
        std::cout << " - Aho-Corasick results: " << AC_RESULT_FILE << "\n";
    } else {
        std::cout << " - No Aho-Corasick matches found.\n";
    }

    return 0;
}