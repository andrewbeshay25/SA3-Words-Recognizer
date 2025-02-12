#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

bool kwFlag, spFlag, idFlag;

void classifyWords(const char *line, bool *kwFlag, bool *spFlag, bool *idFlag) {

}

int countWords(ifstream *myFile) {
    int count = 0;
    string line;

    while (getline(*myFile, line)) {
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


    cout << "Total number of words: " << countWords(&myInfile) << endl;

    myInfile.close();
    return 0;
}