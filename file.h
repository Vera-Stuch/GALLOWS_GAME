#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

string decrypt(const string& encrypted, int shift);

vector<string> loadWordsFromFile(const string& filename, int shift);

char getPlayerLetter(const vector<char>& used);
   

void printStatistics(const string& secret, int totalAttempts, int wrongAttempts,
    const vector<char>& guessedLetters, double timeSec, bool won);

void playHangman(const vector<string>& words);
