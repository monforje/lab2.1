// ============================================================================
// Данный заголовочный файл содержит реализацию алгоритма Ахо–Корасик (AC)
// для многократного поиска подстрок в строке.
// ============================================================================

#ifndef AC_HPP
#define AC_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>
#include <memory>

// Логика алгоритма:
// 1. Для всех шаблонов (patterns) строится префиксное дерево (trie).
// 2. Для каждого узла trie вычисляется суффиксная ссылка.
// 3. Также для ускорения переходов при обработке символов текста формируются связки go.
// 4. Для быстрого обнаружения всех шаблонов, “оканчивающихся” в данном узле или 
//    в его суффиксном предке, создадим конечную ссылку up.

// Сложность алгоритма:
// Построение автомата: O(l), l - суммарная длина всех паттернов.
// Поиск: O(n + k), где n - длина текста, k - количество найденных вхождений паттернов в тексте.

// ==================================================================
// | AhoSearch (string text, vector<string> patterns, size_t count) |
// ==================================================================

struct Node {
    std::unordered_map<char, std::unique_ptr<Node>> son;
    // go: кэш (мемоизация) переходов по символам, когда мы уже знаем, куда идти.
    std::unordered_map<char, Node*> go;
    Node* parent = nullptr;
    char charToParent = '\0';
    Node* suffLink = nullptr;
    Node* up = nullptr;
    bool isTerminal = false;
    // pattern_indices: индексы шаблонов, которые оканчиваются в данном узле.
    std::vector<int> pattern_indices;

    Node(Node* p = nullptr, char c = '\0') noexcept : parent(p), charToParent(c) {}
};

inline Node* createNode(Node* parent = nullptr, char c = '\0') noexcept {
    return new Node(parent, c);
}

static Node* root = nullptr;

inline void initTrie() noexcept {
    root = createNode();
}

inline void addString(const std::string& word, size_t index) noexcept {
    Node* cur = root;
    for (const char c : word) {
        auto& child = cur->son[c];
        if (!child) {
            child = std::make_unique<Node>(cur, c);
        }
        cur = child.get();
    }
    cur->isTerminal = true;
    cur->pattern_indices.push_back(static_cast<int>(index));
}

static Node* getSuffLink(Node* v) noexcept;
static Node* getLink(Node* v, char c) noexcept;
static Node* getUp(Node* v) noexcept;

inline static Node* getSuffLink(Node* v) noexcept {
    if (!v->suffLink) {
        if (v == root || v->parent == root) {
            v->suffLink = root;
        } else {
            v->suffLink = getLink(getSuffLink(v->parent), v->charToParent);
        }
    }
    return v->suffLink;
}

inline static Node* getLink(Node* v, char c) noexcept {
    auto it = v->go.find(c);
    if (it == v->go.end()) {
        if (v->son.count(c)) {
            v->go[c] = v->son[c].get();
        } else {
            v->go[c] = (v == root) ? root : getLink(getSuffLink(v), c);
        }
    }
    return v->go[c];
}

inline static Node* getUp(Node* v) noexcept {
    if (!v->up) {
        Node* sufflink = getSuffLink(v);
        v->up = (sufflink->isTerminal) 
                    ? sufflink 
                    : ((sufflink == root) ? root : getUp(sufflink));
    }
    return v->up;
}

inline void buildAutomation(const std::vector<std::string>& patterns) noexcept {
    initTrie();

    for (size_t i = 0; i < patterns.size(); ++i) {
        addString(patterns[i], i);
    }

    // Очередь для обхода в ширину (BFS) по trie.
    std::queue<Node*> q;
    root->suffLink = root;

    for (auto& child : root->son) {
        child.second->suffLink = root;
        q.push(child.second.get());
    }

    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();

        for (auto& child : cur->son) {
            char c = child.first;
            Node* node = child.second.get();
            Node* f = cur->suffLink;

            while (f != root && f->son.find(c) == f->son.end()) {
                f = f->suffLink;
            }
            if (f->son.find(c) != f->son.end()) {
                node->suffLink = f->son[c].get();
            } else {
                node->suffLink = root;
            }
            q.push(node);
        }

        cur->up = (cur->suffLink->isTerminal) ? cur->suffLink : cur->suffLink->up;
    }
}

// Функция AhoSearch ищет все паттерны (patterns) в тексте (text). 
// Возвращает true, если найдены все шаблоны, иначе — false.
// Параметр count указывает, сколько шаблонов требуется найти.
// (Если каждый шаблон нужно найти хотя бы один раз.)
[[nodiscard]] bool AhoSearch(const std::string& text, const std::vector<std::string>& patterns, size_t count) noexcept {
    buildAutomation(patterns);

    std::vector<bool> found(count, false);
    size_t found_count = 0;

    Node* cur = root;
    for (const char& c : text) {
        cur = getLink(cur, c);

        Node* temp = cur;
        while (temp != root) {
            if (temp->isTerminal) {
                for (size_t index : temp->pattern_indices) {
                    if (index < count && !found[index]) {
                        found[index] = true;
                        ++found_count;
                        if (found_count == count) {
                            return true;
                        }
                    }
                }
            }
            temp = getUp(temp);
        }
    }
    return found_count == count;
}

inline void deleteTrie(Node* node) noexcept {
    if (node) {
        for (auto& child : node->son) {
            deleteTrie(child.second.release());
        }
        delete node;
    }
}

#endif // AC_HPP