#include "file.h"

// Расшифровка слова (шифр Цезаря)
string decrypt(const string& encrypted, int shift){
    string result;
    for (char c : encrypted)
    {
        if (isalpha(static_cast<unsigned char>(c)))
        {
            char base = isupper(static_cast<unsigned char>(c)) ? 'A' : 'a';
            int offset = (c - base - shift) % 26;
            if (offset < 0) offset += 26;
            result += static_cast<char>(base + offset);
        }
        else
        {
            result += c;
        }
    }
    return result;
}

// Загрузка слов из файла
vector<string> loadWordsFromFile(const string& filename, int shift)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        ofstream createFile(filename);
        if (createFile.is_open())
        {
            createFile << "KHOOR\nZRUOG\nJDPH\nZLQWHU\nEXFNHW\n"; // HELLO, WORLD, GAME, WINTER, BUCKET
            createFile.close();
            cout << " Файл \"words.txt\" не найден. Создан пример." << endl;
            file.open(filename);
        }
        cout << endl;

        if (!file.is_open())
        {
            cerr << "Не удалось открыть или создать файл \"words.txt\"" << endl;
            exit(EXIT_FAILURE);
        }
    }

    vector<string> words;
    string line;
    while (getline(file, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.empty()) continue;

        string word = decrypt(line, shift);
        transform(word.begin(), word.end(), word.begin(),
            [](unsigned char ch) { return toupper(ch); });
        words.push_back(word);
    }
    file.close();

    if (words.empty())
    {
        cerr << "Файл не содержит слов." << endl;
        exit(EXIT_FAILURE);
    }
    return words;
}

// Получение одной корректной буквы
char getPlayerLetter(const vector<char>& used)
{
    string input;
    while (true)
    {
        cout << "\nВведите букву (A–Z): ";
        getline(cin, input);

        if (input.size() != 1 || !isalpha(static_cast<unsigned char>(input[0])))
        {
            cout << "Введите одну латинскую букву." << endl;
            continue;
        }

        char letter = static_cast<char>(toupper(static_cast<unsigned char>(input[0])));
        if (find(used.begin(), used.end(), letter) != used.end())
        {
            cout << "Буква '" << letter << "' уже была. Попробуйте другую." << endl;
            continue;
        }
        return letter;
    }
}

// Вывод статистики (вызывается при любом исходе)
void printStatistics(const string& secret, int totalAttempts, int wrongAttempts,
    const vector<char>& guessedLetters, double timeSec, bool won)
{
    cout << "\n\n" << (won ? "ПОБЕДА! ПОЗДРАВЛЯЕМ, ВЫ ВЫЖИЛИ." : "СОЖАЛЕЕМ. ПОРАЖЕНИЕ...") << endl;
    cout << "\nСТАТИСТИКА ИГРЫ:" << endl;
    cout << "• Искомое слово: " << secret << endl;
    cout << "• Всего попыток: " << totalAttempts << "" << endl;
    cout << "• Ошибок: " << wrongAttempts << " из 6" << endl;
    cout << "• Затрачено времени: " << fixed << setprecision(2) << timeSec << " сек." << endl;
    cout << "• Введённые буквы: ";
    for (size_t i = 0; i < guessedLetters.size(); ++i)
    {
        if (i > 0) cout << ", ";
        cout << guessedLetters[i];
    }
    cout << endl << endl;
}

// Основная игра
void playHangman(const vector<string>& words)
{
    size_t idx = static_cast<size_t>(rand()) % words.size();
    string secret = words[idx];

    vector<bool> revealed(secret.size(), false);
    vector<char> guessedLetters;
    int totalAttempts = 0;
    int wrongAttempts = 0;
    const int MAX_WRONG = 6;
    clock_t start = clock();

    cout << "Добро пожаловать в «Виселицу»!!" << endl;
    cout << "У вас есть " << MAX_WRONG << " возможностей на ошибку, чтобы угадать слово из "
        << secret.size() << " букв." << endl;
    cout << "ЖЕЛАЕМ УДАЧИ ВЫЖИТЬ:)" << endl;
    while (wrongAttempts < MAX_WRONG)
    {
        // Отображение слова
        cout << "\nСлово: ";
        for (size_t i = 0; i < secret.size(); ++i)
        {
            cout << (revealed[i] ? secret[i] : '_') << ' ';
        }

        cout << "\nОшибок: " << wrongAttempts << " / " << MAX_WRONG;
        if (!guessedLetters.empty())
        {
            cout << "\nИспользовано: ";
            for (size_t i = 0; i < guessedLetters.size(); ++i)
            {
                if (i > 0) cout << ", ";
                cout << guessedLetters[i];
            }
        }

        char letter = getPlayerLetter(guessedLetters);
        guessedLetters.push_back(letter);
        totalAttempts++;

        // Проверка есть ли буква в слове
        bool correct = false;
        for (size_t i = 0; i < secret.size(); ++i)
        {
            if (secret[i] == letter)
            {
                revealed[i] = true;
                correct = true;
            }
        }

        if (correct)
        {
            cout << "\nВерно! Буква '" << letter << "' есть в слове." << endl;
        }
        else
        {
            wrongAttempts++;
            cout << "\n Нет буквы '" << letter << "'. Ошибок: " << wrongAttempts << endl;
        }

        // Проверка победы
        if (all_of(revealed.begin(), revealed.end(), [](bool b) { return b; }))
        {
            double timeSec = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
            printStatistics(secret, totalAttempts, wrongAttempts, guessedLetters, timeSec, true);
            return;
        }
    }

    // Поражение
    double timeSec = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    printStatistics(secret, totalAttempts, wrongAttempts, guessedLetters, timeSec, false);
}
