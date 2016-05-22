#include <iostream>
#include <math.h>
#include <vector>
#include <chrono>

using namespace std;

// Settings
int wordsLength = 2;
string outputFilePath = "dictionary.txt";

// Lists
vector<char> characters {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', ',', '?', '!'};
vector<string> words {};

// Temp
int totalWordsNumber = 0;
string word = "";
int lastProgress = 0;

unsigned long getTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count();
}

int getCurrentProgress() {
    return trunc(words.size() * 100 / totalWordsNumber);
}

string setCharacterAt(string word, int index, char character) {
    if(index + 1 > word.length()) {
        return word.substr(0, index) + character + word.substr(index);
    } else {
        return word.substr(0, index) + character + word.substr(index + 1);
    }
}

int iterateOverCharacters(int wordPosition) {
    for(int i = 0; i < characters.size(); i++) {
        if(wordPosition == 0) { word = ""; }

        word = setCharacterAt(word, wordPosition, characters[i]);
        cout << "\n" << wordPosition << ":" << characters[i] << "->" << word;
        words.push_back(word);

        int currentProgress = getCurrentProgress();
        if(currentProgress != lastProgress) {
            lastProgress = currentProgress;
            cout << " - (" << currentProgress << "%)";
        }

        if((wordPosition + 1) < wordsLength) {
            iterateOverCharacters(wordPosition + 1); //Change to ++ ?
        }
    }
}

int main() {
    //How many possibilities will be generated ?
    for(int i = 1 ; i <= wordsLength ; i++) {
        totalWordsNumber += pow(characters.size(), i);
    }
    cout << "At the end, we'll have " << totalWordsNumber << " words.";
    cout << "\nStarting...\n";

    //Run and keep a track of times
    long timeBeforeRunning = getTimestamp();
    iterateOverCharacters(0);
    long timeAfterRunning = getTimestamp();

    cout << "\n\nDone (" << timeAfterRunning - timeBeforeRunning << " ms, " << words.size() << " words, " << (words.size() * 1000) / timeAfterRunning - timeBeforeRunning << " words per second).";
    cout << "\nHave a nice day.\n";

    system("pause");
    return 0;
}