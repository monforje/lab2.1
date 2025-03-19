// ============================================================================
// Данный заголовочный файл содержит реализацию алгоритма Кнута-Морриса-Пратта (KMP)
// для поиска вхождений подстроки в строку.
// ============================================================================

#ifndef KMP_HPP
#define KMP_HPP

#include <string>
#include <iostream>
#include <cstdlib>

// Логика алгоритма:
// 1. Считаем префикс-функцию (pie - массив) для шаблона: для каждого символа записываем
//    длину максимального суффикса, совпадающего с его префиксом.
// 2. Используя эту префикс-функцию, при несовпадении символов шаблона и текста не начинаем
//    сравнение заново, а “перепрыгиваем” в подходящую позицию в шаблоне.
// 3. Проходим по тексту, сопоставляя символы с шаблоном. Если символы совпадают – увеличиваем
//    счётчик совпадений; если нет – используем префикс-функцию, чтобы не терять уже найденные совпадения.
// 4. Когда счётчик достигает длины шаблона, фиксируем найденную позицию (cur - size_ + 1).

// Сложность алгоритма: O (n + m)

// ========================================================================
// |      kmp_search(string text, string pattern, size_t match_count)     |
// ========================================================================

// lpfun long prefix function - это функция, которая для каждого символа в шаблоне 
// рассчитывает длину максимального суффикса подстроки, совпадающего с её префиксом.
inline size_t* lpfun(const std::string& pattern) noexcept {
    size_t size_ = pattern.size();
    // Выделяем на куче память под массив размером size_ элементов типа size_t.
    size_t* pie = (size_t*)malloc(size_ * sizeof(size_t));
    if (!pie) {
        std::cerr << "Memory allocation failed\n";
        exit(EXIT_FAILURE);
    }

    pie[0] = 0;
    // k — счетчик совпавших символов на текущем этапе.
    size_t k = 0;

    for (size_t i = 1; i < size_; ++i) {
        while (k > 0 && pattern[k] != pattern[i]) {
            k = pie[k - 1];
        }
        if (pattern[k] == pattern[i]) {
            ++k;
        }
        pie[i] = k;
    }

    return pie;
}

// Функция kmp_search выполняет поиск шаблона (pattern) в тексте (text), 
// возвращая массив позиций, где шаблон найден. В *match_count возвращается 
// общее число совпадений.
inline size_t* kmp_search(const std::string& text, const std::string& pattern, size_t* match_count) noexcept {
    size_t size_ = pattern.size();

    if (size_ == 0 || text.size() < size_) {
        *match_count = 0;
        return nullptr;
    }

    size_t* pie = lpfun(pattern);
    size_t matched_pos = 0;

    size_t* matches = (size_t*)malloc(text.size() * sizeof(size_t));
    if (!matches) {
        std::cerr << "Memory allocation failed\n";
        free(pie);
        exit(EXIT_FAILURE);
    }

    *match_count = 0;

    for (size_t cur = 0; cur < text.size(); ++cur) {
        // Пока имеем несовпадение и уже есть какое-то количество совпавших символов,
        // откатываемся по префикс-функции.
        while (matched_pos > 0 && pattern[matched_pos] != text[cur]) {
            matched_pos = pie[matched_pos - 1];
        }

        if (pattern[matched_pos] == text[cur]) {
            ++matched_pos;
        }

        if (matched_pos == size_) {
            matches[(*match_count)++] = cur - size_ + 1;
            matched_pos = pie[matched_pos - 1];
        }
    }

    free(pie);
    return matches;
}

#endif // KMP_HPP