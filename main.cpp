#include <iostream>
#include <fstream>
#include <string.h>
#include <cctype>
#include <algorithm>
#include <string>

using namespace std;

int lineCount = 0;
bool kwFlag, spFlag, idFlag;
const string keyWords[] = {"begin", "end", "if", "else", "while", "for","break",
    "continue", "case", "switch", "class", "public", "private", "abstract", "final"};

int countSP_dol, countSP_perc, countSP_at, countID, countKW = 0;

int countWordsInLine(const char *line) {
    int count = 0;

    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {
            count++;
        } if (i == strlen(line) - 1 && line[i] != ' ') {
            count++;
        }
    }
    return count;
}

void checkID(const char *charWord, string word) {
    bool allGood = true;
    for (int i = 1; i < word.length(); i++) {
        if (!isalpha(word[i]) && !isdigit(word[i])) {
            cout << "Invalid Identifier Word at line " << lineCount  << ": " << word << endl;
            i = word.length();
            allGood = false;
        }
    }
    if (allGood) {
        countID++;
    }
}

void checkSP(const char *charWord, const string &word) {
    bool allGood = true;
    for (int i = 1; i < word.length(); i++) {
        if (charWord[i] == '.' || charWord[i] == ',' || charWord[i] == '\'' || charWord[i] == '\"' || charWord[i] == ':' ||
            charWord[i] == ';' || charWord[i] == '(' || charWord[i] == ')' || charWord[i] == '[' || charWord[i] == ']' ||
            charWord[i] == '{' || charWord[i] == '}' || charWord[i] == '#' || charWord[i] == '~' || charWord[i] == '!' || charWord[i] == '?') {
            cout << "Invalid Special Word at line " << lineCount << ": " << word << endl;
            i = word.length();
            allGood = false;
        }
    }
    if (allGood) {
        if (charWord[0] == '$') {
            countSP_dol++;
        } else if (charWord[0] == '%') {
            countSP_perc++;
        } else {
            countSP_at++;
        }
    }
}

void checkKW(string word) {
    transform(word.begin(), word.end(),word.begin(), ::tolower);

    for (int i = 0; i < 15; i++) {
        if (keyWords[i] == word) {
            countKW++;
        }
    }
}

void classifyWords(const char *line, const bool *kwFlag, const bool *spFlag, const bool *idFlag) {
    const int wordCount = countWordsInLine(line);
    string words[wordCount];

    string currWord = "";
    int currCount = 0;
    for (int i = 0; i < strlen(line); i++) {
        if (line[i] == ' ') {

            if (*idFlag && isalpha(currWord[0])) {
                checkID(currWord.c_str(), currWord);
            }
            if (*spFlag && (currWord[0] == '$' || currWord[0] == '@' || currWord[0] == '%')) {
                checkSP(currWord.c_str(), currWord);
            }
            if (*kwFlag) {
                checkKW(currWord);
            }

            words[currCount] = currWord;
            currCount++;
            currWord = "";
        } else if (i == strlen(line) - 1) {
            currWord += line[i];

            if (*idFlag && isalpha(currWord[0])) {
                checkID(currWord.c_str(), currWord);
            }
            if (*spFlag && (currWord[0] == '$' || currWord[0] == '@' || currWord[0] == '%')) {
                checkSP(currWord.c_str(), currWord);
            }
            if (*kwFlag) {
                checkKW(currWord);
            }
            words[currCount] = currWord;
            currCount++;
            currWord = "";
        } else {
            currWord += line[i];
        }
    }
}

int countTotalWords(ifstream *myFile) {
    int count = 0;
    string line;

    while (getline(*myFile, line)) {
        lineCount++;
        const char *words = line.c_str();

        classifyWords(words, &kwFlag, &spFlag, &idFlag);

        for (int i = 0; i < strlen(words); i++) {
            if (words[i] == ' ') {
                count++;
            } if (i == strlen(words) - 1 && words[i] != ' ') {
                count++;
            }
        }
    }
    return count;
}


int main(int const argc, char const *argv[]) {
    ifstream myInfile;

    if (argc < 2) {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        exit(1);
    }

    const string filename = argv[1];
    myInfile.open(filename);
    if (!myInfile) {
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        exit(1);
    }

    if (myInfile.peek() == EOF) {
        cout << "The File is Empty." << endl;
        exit(1);
    }

    if (argc > 2) {
        for (int i = 2 ; i < argc; i++) {
            string flag = argv[i];

            if (flag == "-kw") {
                kwFlag = true;
            } else if (flag == "-sp") {
                spFlag = true;
            } else if (flag == "-id") {
                idFlag = true;
            } else {
                cout << argv[i] << " is not a valid flag." << endl;
            }
        }
    }

    const int totalWords = countTotalWords(&myInfile);
    cout << "Total number of words: " << totalWords << endl;

    if (kwFlag) {
        cout << "Number of Keywords: " << countKW << endl;
    }
    if (idFlag) {
        cout << "Number of Identifiers: " << countID << endl;
    }
    if (spFlag) {
        cout << "Number of Special Words Starting with $: " << countSP_dol << endl;
        cout << "Number of Special Words Starting with @: " << countSP_at << endl;
        cout << "Number of Special Words Starting with %: " << countSP_perc << endl;
    }

    myInfile.close();
    return 0;
}
