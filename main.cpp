
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sys/stat.h>
#include <direct.h>
#include "json.hpp"

using namespace std;
using json::JSON;

string configFileName = "config.json";
string statsFileName = "stats.json";
bool statsEnabled = true;
vector<char> dictionary {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
int wordsLength = 5;
string secretWord = "Super";
bool outputEnabled = false;
string outputDirectory = "output/";
int wordsBeforeSplittingFiles = 10000000;
int logLevel = 2;

ofstream output;
int filesCount = 0;
long long int timeBeforeRunning = 0;
long long int timeAfterRunning = 0;
unsigned long long int totalWordsNumber = 0;
unsigned long long int currentWordsNumber = 0;
int lastProgress = 0;
bool secretWordFound = false;
string secretWordResult = "";
string word = "";

int stop() {
    cout << "\n";
    cin.ignore();
	exit(0);
}

long long int getTimestamp() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

inline bool fileExist(const string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

void updateStats() {
    if(statsEnabled) {
        JSON stats;
        if(fileExist(statsFileName)) {
            ifstream infile {statsFileName};
            string statsContent((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
            stats = JSON::Load(statsContent);
        }
        string timestamp = to_string(getTimestamp());
        stats[timestamp]["logLevel"] = logLevel;
        stats[timestamp]["secretWord"] = secretWord;
        stats[timestamp]["secretWordFound"] = secretWordFound;
        for(int i = 0; i < dictionary.size(); i++) { stats[timestamp]["dictionary"].append(string(1, dictionary[i])); }
        stats[timestamp]["wordsLength"] = wordsLength;
        stats[timestamp]["wordsBeforeSplittingFiles"] = wordsBeforeSplittingFiles;
        stats[timestamp]["totalWordsNumber"] = totalWordsNumber;
        stats[timestamp]["time"] = (timeAfterRunning - timeBeforeRunning) / 1000;
        stats[timestamp]["filesCount"] = filesCount;
        if((timeAfterRunning - timeBeforeRunning) / 1000 != 0) { stats[timestamp]["words/s"] = totalWordsNumber / ((timeAfterRunning - timeBeforeRunning) / 1000); }
        else { stats[timestamp]["words/s"] = totalWordsNumber; }
        ofstream statsFile(statsFileName);
        statsFile << stats;
        statsFile.close();
    }
}

void loadConfig() {
    if(fileExist(configFileName)) { // Read config.json and update default settings
        ifstream infile {configFileName};
        string configContent((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        JSON config = JSON::Load(configContent);

        JSON dictionaryJSON = config["dictionary"];
        dictionary.clear();
        dictionary.shrink_to_fit();
        for(int i = 0; i < dictionaryJSON.size(); i++) {
            string character = dictionaryJSON[i].ToString();
            dictionary.push_back(character[0]);
        }
        wordsLength = config["wordsLength"].ToInt();
        secretWord = config["secretWord"].ToString();
        outputEnabled = config["output"]["enable"].ToBool();
        outputDirectory = config["output"]["directory"].ToString();
        wordsBeforeSplittingFiles = config["output"]["wordsBeforeSplittingFiles"].ToInt();
        logLevel = config["logLevel"].ToInt();
        statsEnabled = config["statsEnabled"].ToBool();
    } else { // Create config.json and fill it with default settings
        JSON config;
        for(int i = 0; i < dictionary.size(); i++) { config["dictionary"].append(string(1, dictionary[i])); }
        config["wordsLength"] = wordsLength;
        config["secretWord"] = secretWord;
        config["output"]["enable"] = outputEnabled;
        config["output"]["directory"] = outputDirectory;
        config["output"]["wordsBeforeSplittingFiles"] = wordsBeforeSplittingFiles;
        config["logLevel"] = logLevel;
        config["statsEnabled"] = statsEnabled;
        ofstream configFile(configFileName);
        configFile << config;
        configFile.close();
    }
}

void sendToOutput(string word) {
    if(outputEnabled) {
        if(currentWordsNumber == 0 || currentWordsNumber % wordsBeforeSplittingFiles == 0) {
            filesCount++;
            output.close();
            string path = outputDirectory + to_string(currentWordsNumber / wordsBeforeSplittingFiles) + ".words";
            output.open(path);
        } else {
            output << "\n";
        }
        output << word;
    }
}

int getCurrentProgress() {
    return int(trunc(currentWordsNumber * 100 / totalWordsNumber));
}

string setCharacterAt(string word, unsigned int index, char character) {
    return word.substr(0, index) + character;
}

void iterateOverCharacters(unsigned int wordPosition) {
    for(int i = 0; i < dictionary.size(); i++) {
        if(secretWordFound) { break; }
        if(wordPosition == 0) { word = ""; }

        word = setCharacterAt(word, wordPosition, dictionary[i]);
        sendToOutput(word);
        currentWordsNumber++;

        if(word == secretWord) {
            secretWordFound = true;
            secretWordResult = word;
            break;
        }

        int currentProgress = getCurrentProgress();

        if(currentWordsNumber == 1 && (logLevel == 1 || logLevel == 2)) { cout << "\n"; }

        if(logLevel == 1 && currentProgress != lastProgress) {
            long long int timeToCompleteProcess =
                    ((getTimestamp() - timeBeforeRunning) * (100 - currentProgress)) / currentProgress; //ms for 100%
            long long int estimationEndTime = ((getTimestamp() + timeToCompleteProcess) - getTimestamp()) / 1000;
            printf("\r");
            cout <<
            string(44 + wordsLength + to_string((timeBeforeRunning + timeToCompleteProcess) / 1000).size(), ' ');
            printf("\r");
            cout << "Working  (word: " << word << ",";
            cout << string(wordsLength - word.size(), ' ') << " progress: " << currentProgress << "%)";
        } else if(logLevel == 2 && currentProgress != lastProgress) {
            long long int timeToCompleteProcess =
                    ((getTimestamp() - timeBeforeRunning) * (100 - currentProgress)) / currentProgress; //ms for 100%
            long long int estimationEndTime = ((getTimestamp() + timeToCompleteProcess) - getTimestamp()) / 1000;
            printf("\r");
            cout <<
            string(44 + wordsLength + to_string((timeBeforeRunning + timeToCompleteProcess) / 1000).size(), ' ');
            printf("\r");
            cout << "Working  (word: " << word << ",";
            cout << string(wordsLength - word.size(), ' ') << " progress: " << currentProgress << "%, ";
            cout << "time left: " << estimationEndTime << "s)";
        } else if(logLevel == 3 && currentProgress != lastProgress) {
            cout << "\n";
            long long int timeToCompleteProcess = ((getTimestamp() - timeBeforeRunning) * (100 - currentProgress)) / currentProgress; //ms for 100%
            long long int estimationEndTime = ((getTimestamp() + timeToCompleteProcess) - getTimestamp()) / 1000;
            cout << "Working  (" << wordPosition << ":" << dictionary[i] << "->" << word << ",";
            cout << string(wordsLength - word.size(), ' ') << " progress: " << currentProgress << "%, ";
            cout << "time left: " << estimationEndTime << "s)";
        }

        if(currentProgress != lastProgress) { lastProgress = currentProgress; }

        if((wordPosition + 1) < wordsLength) {
            iterateOverCharacters(wordPosition + 1);
        }
    }
}

int main() {
    try {
        loadConfig();

        if(wordsLength == 0) {
            // Fired when 0 or not int
            throw string("wordsLength can't be equal to 0 and must be an integer.");
        }

        for(int i = 1; i <= wordsLength; i++) {
            totalWordsNumber = totalWordsNumber + ceil(pow(dictionary.size(), i));
        }
        cout << "Starting (" << totalWordsNumber << " words are going to be created)";

        if(outputEnabled) {
            #if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
				#if defined(_MSC_VER)
					_mkdir(outputDirectory.c_str());
				#else
					mkdir(outputDirectory.c_str());
				#endif
            #else
                mkdir(outputDirectory.c_str(), 0777);
            #endif
        }
        timeBeforeRunning = getTimestamp();
        iterateOverCharacters(0);
        timeAfterRunning = getTimestamp();
        output.close();

        cout << "\nDone     (";
        cout << currentWordsNumber << " words in ";
        cout << (timeAfterRunning - timeBeforeRunning) / 1000 << "s -> ";
        if((timeAfterRunning - timeBeforeRunning) / 1000 != 0) { cout << totalWordsNumber / ((timeAfterRunning - timeBeforeRunning) / 1000) << " words/s, "; }
        else { cout << totalWordsNumber << "/+ words/s, "; }
        if (filesCount > 1) { cout << filesCount << " files)"; } else { cout << filesCount << " file)"; }

        if(secretWordFound) {
            cout << "\nRiddle   (I found the secret word, it's '" << secretWordResult <<"' :-O)";
        } else {
            cout << "\nRiddle   (I didn't found the secret word :-/)";
        }

        updateStats();

        stop();
    }
    catch (const string & message) {
        cout << "Error : " << message;
        stop();
    }
}
