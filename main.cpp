#include "file.h"

int main()
{
    setlocale(LC_ALL, "ru");
    srand(static_cast<unsigned int>(time(nullptr)));

    const string FILENAME = "words.txt";
    const int SHIFT = 3;

    vector<string> words = loadWordsFromFile(FILENAME, SHIFT);
    playHangman(words);

    cout << "Спасибо за игру!!" << endl;
    return 0;
}
