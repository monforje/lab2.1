
# Лабораторная работа 2.1

## 📌 Описание
Проект реализует два алгоритма поиска подстрок в строке:
- **Ахо-Корасик** – множественный поиск с использованием префиксного дерева (trie) и суффиксных ссылок.
- **Кнут-Моррис-Пратт (КМП)** – быстрый поиск с использованием префикс-функции.

Оба алгоритма реализованы на языке C++.

---

## 🚀 Возможности
### 🔹 **Ахо-Корасик** (`ac.hpp`)
✅ Построение trie из шаблонов  
✅ Создание суффиксных ссылок для ускорения переходов  
✅ Поиск всех шаблонов за один проход  
**Сложность:**  
- Построение: `O(l)`, где `l` — суммарная длина всех шаблонов  
- Поиск: `O(n + k)`, где `n` — длина текста, `k` — количество совпадений  

**Пример:**
```cpp
std::vector<std::string> patterns = {"abc", "bc", "c"};
std::string text = "abcabc";
bool allFound = AhoSearch(text, patterns, patterns.size());
```

---

### 🔹 **Кнут-Моррис-Пратт** (`kmp.hpp`)
✅ Построение префикс-функции  
✅ Поиск с минимальным количеством откатов  
**Сложность:**  
- Построение: `O(m)`, где `m` — длина шаблона  
- Поиск: `O(n)`, где `n` — длина текста  

**Пример:**
```cpp
std::string pattern = "abc";
std::string text = "abcabc";
size_t match_count = 0;
size_t* matches = kmp_search(text, pattern, &match_count);
```

---

### 🔹 **Обработка файлов** (`file.hpp`)
✅ **FileToString** – Чтение файла в строку  
✅ **FileToLines** – Чтение файла построчно  
✅ **FileToWords** – Чтение файла по словам  
✅ **SpliteByEnter** – Разделение строки по символам `
` и `
`  
✅ **SplitByWords** – Разделение строки по пробелам  
✅ **SplitByGroups** – Разделение строки на группы слов  
✅ **LinesWithWords** – Преобразование строк в массив слов  

**Пример:**
```cpp
auto lines = FileToLines("data.txt");
auto words = FileToWords("data.txt");
```

---

## 🏗️ **Структура проекта**
```
├── data/         # Входные и выходные файлы
├── src/          # Исходный код
├── ac.hpp        # Реализация алгоритма Ахо-Корасик
├── kmp.hpp       # Реализация алгоритма Кнут-Морриса-Пратта
├── file.hpp      # Утилиты для работы с файлами
├── main.cpp      # Основной файл программы
```

---

## 💾 **Сборка и запуск**
1. Склонируйте репозиторий:
```bash
git clone https://github.com/monforje/lab2.1.git
```
2. Перейдите в директорию проекта:
```bash
cd lab2.1
```
3. Соберите проект:
```bash
g++ -o lab2.1 src/*.cpp
```
4. Запустите:
```bash
./lab2.1
```

---

## 📋 **Описание работы программы**
1. Считывается текст из файла `data/data.txt`.  
2. Выполняется поиск с помощью алгоритма **КМП**.  
   ➡️ Результаты → `data/kmp_result.txt`  
3. Выполняется поиск с помощью алгоритма **Ахо-Корасик**.  
   ➡️ Результаты → `data/ac_result.txt`  
4. Время выполнения сохраняется в файлы результатов.  

---

## 📂 **Результаты**
- **KMP результаты** → `data/kmp_result.txt`  
- **Ахо-Корасик результаты** → `data/ac_result.txt`  

---

## 📜 **Лицензия**
Проект распространяется по лицензии [MIT](LICENSE).

---

✅ Если у вас возникнут вопросы или предложения — открывайте issue! 😎