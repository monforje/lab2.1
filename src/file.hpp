// ============================================================================
// Данный заголовочный файл содержит несколько функций для чтения файлов и 
// обработки их содержимого.
// ============================================================================

#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <regex>
#include <algorithm>
#include <iostream>
#include <iterator>

// =====================================
//            Работа с файлом
// =====================================
//
// Каждая функция предназначена для чтения содержимого файла и его 
// преобразования в различные форматы (строку, вектор строк, вектор слов).
// -------------------------------------------------------------
// | FileToString | FileToWords | FileToLines | LinesWithWords |
// -------------------------------------------------------------

[[maybe_unused]] inline std::vector<std::string> SpliteByEnter(const std::string& content) {
    // Резервируем приблизительный размер, 
    // чтобы избежать дополнительных аллокаций.
    // Аллокация - это выделение памяти под переменную, массив, объект 
    // или другой тип данных. Аллокация может быть статической (выделяется 
    // память на протяжении всего периода жизни программы) или динамической 
    // (выделяется память при необходимости, например, при создании объекта 
    // или массива).
    std::vector<std::string> lines;
    lines.reserve(content.size() / 30);
    
    // Оборачиваем исходную строку во view для удобного доступа 
    // без дополнительного копирования.
    // string_view - это класс, который позволяет безопасно и эффективно 
    // просматривать содержимое строки, не создавая ее копию. Он хранит 
    // ссылку на начало и конец строки, а также ее размер. 
    // string_view позволяет использовать строку как если бы она была 
    // const-qualified, но при этом не делая копию строки.
    std::string_view view{content};

    // Переменные start и end указывают на начало и конец текущего отрезка строки.
    size_t start = 0;
    size_t end   = 0;

    // Внешний цикл проходит по всей длине строки.
    while (end < view.size()) {
        // Двигаем end, пока не встретим перевод строки или конец строки.
        while (end < view.size() && view[end] != '\n' && view[end] != '\r') {
            ++end;
        }

        // Если между start и end есть хоть один символ, добавляем его в результат.
        if (start < end) {
            lines.emplace_back(view.substr(start, end - start));
        }

        // Пропускаем все подряд идущие переводы строки, чтобы начать 
        // следующую строку после них.
        while (end < view.size() && (view[end] == '\n' || view[end] == '\r')) {
            ++end;
        }

        start = end;
    }

    return lines;
}

[[maybe_unused]] inline std::vector<std::string> SplitByWords(const std::string& content) {
    std::vector<std::string> words;
    words.reserve(content.size() / 5);

    std::string_view view{content};

    size_t start = 0, end = 0;

    while (end < view.size()) {
        while (end < view.size() && (view[end] == ' ' || view[end] == '\t' || view[end] == '\n' || view[end] == '\r')) {
            ++end;
        }

        start = end;

        while (end < view.size() && view[end] != ' ' && view[end] != '\t' && view[end] != '\n' && view[end] != '\r') {
            ++end;
        }

        if (start < end) {
            words.emplace_back(view.substr(start, end - start));
        }
    }

    return words;
}

[[nodiscard]] inline std::vector<std::string> FileToLines(const std::string& filename) {
    // Открываем файл в бинарном режиме и сразу переходим в конец, 
    // чтобы узнать его размер (через tellg).
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    // Получаем текущую позицию (размер файла).
    std::streamsize fsize = file.tellg();
    file.seekg(0); // Возвращаемся в начало файла

    // Создаем строку нужного размера и считываем данные из файла в эту строку.
    std::string content(fsize, '\0');
    file.read(&content[0], fsize);

    // Проверяем, все ли считалось; если нет и мы не достигли конца файла, 
    // сообщаем об ошибке.
    if (!file && !file.eof()) {
        throw std::runtime_error("Failed to read entire file: " + filename);
    }

    file.close();
    
    return SpliteByEnter(content);
}

[[nodiscard]] inline std::string FileToString(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    file.seekg(0, std::ios::end);
    const size_t fsize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Создаем строку нужного размера и считываем всё содержимое файла.
    std::string data(static_cast<size_t>(fsize), '\0');
    if (!file.read(data.data(), fsize)) {
        throw std::runtime_error("Error reading file: " + filename);
    }

    // Лямбда-функция, определяющая, является ли символ ASCII-пробелом.
    auto isAsciiWhitespace = [](unsigned char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
    };

    // Удаляем (erase) из   строки все пробельные символы на основе условия (remove_if).
    // std::remove_if "сдвигает" все нужные элементы в начало, 
    // а затем erase "отрезает" лишнее в конце.
    data.erase(std::remove_if(data.begin(), data.end(), isAsciiWhitespace), data.end());

    return data;
}

[[nodiscard]] inline std::vector<std::string> FileToWords(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::streamsize fsize = file.tellg();
    file.seekg(0);

    std::string content(fsize, '\0');
    file.read(&content[0], fsize);

    if (!file && !file.eof()) {
        throw std::runtime_error("Failed to read entire file: " + filename);
    }

    file.close();

    return SplitByWords(content);
}

[[maybe_unused]] inline std::vector<std::string> SplitByGroups(const std::string& line) 
{
    std::vector<std::string> result;
    std::istringstream stream(line);
    std::string part;

    std::vector<std::string> temp;
    while (stream >> part) {
        temp.push_back(part);
    }

    if (temp.size() >= 6) {
        result.push_back(temp[0] + " " + temp[1]); // Первая группа — два первых элемента
        result.push_back(temp[2] + " " + temp[3] + " " + temp[4]); // Вторая группа — три элемента
        result.push_back(temp[5] + (temp.size() > 6 ? " " + temp[6] : "")); // Третья группа — оставшиеся элементы
    } else {
        throw std::invalid_argument("Invalid input format");
    }

    return result;
}

[[nodiscard]] inline std::vector<std::vector<std::string>> LinesWithWords(const std::string& filename) 
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::streamsize fsize = file.tellg(); 
    file.seekg(0); 

    std::string content(fsize, '\0');
    file.read(&content[0], fsize);

    if (!file && !file.eof()) {
        throw std::runtime_error("Failed to read entire file: " + filename);
    }   

    file.close();

    std::vector<std::vector<std::string>> result;
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line)) {
        try {
            result.push_back(SplitByGroups(line));
        } catch (const std::exception& e) {
            std::cerr << "Skipping line due to error: " << e.what() << '\n';
        }
    }

    return result;
}

#endif // FILE_HPP