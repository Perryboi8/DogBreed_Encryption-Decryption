#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <sstream>
using namespace std;

//Ryan Perry
//CPSC 240
//Project 1
//09/20/25


//Functions

vector<string> dog_Breeds(const string &filename);
void encryptfile(string inputFileName, string outputFileName, int key, const vector<string>& breeds);
void decryptFile(string inputFileName, string outputFileName, int key, const vector<string>& breeds);

//



//Load breeds from CSV (name column only)
vector<string> dog_Breeds(const string &filename) {
    vector<string> breeds;
    ifstream file(filename);
    string line;

    //Skip the header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, rest;

        getline(ss, id, ',');    // id column
        getline(ss, name, ',');  // name column

        if (!name.empty()) {
            breeds.push_back(name);
        }
    }
    return breeds;
}

//Encrypt file map each character -> rotated breed
void encryptfile(string inputFileName, string outputFileName, int key, const vector<string>& breeds) {
    ifstream input_File(inputFileName, ios::binary); // binary keeps all chars
    ofstream output_File(outputFileName);

    //Check for input file
    if (!input_File) {
        cerr << "Error: could not open input file!" << endl;
        return;
    }

    int n = breeds.size();
    if (n == 0) {
        cerr << "Error: no breeds loaded!, please check you have the right dogbreeds.csv file" << endl;
        return;
    }

    char c;
    while (input_File.get(c)) {
        int idx = (unsigned char)c % n;       // maps char -> index
        int encIdx = (idx + key) % n;         // shift by key
        output_File << breeds[encIdx] << "\n"; // write one breed per line
    }
}

//Decryptfile, map each breed -> original character
void decryptFile(string inputFileName, string outputFileName, int key, const vector<string>& breeds) {
    ifstream input_File(inputFileName);
    ofstream output_File(outputFileName, ios::binary);

    //check if input file can be read
    if (!input_File) {
        cerr << "Error: could not open input file!" << endl;
        return;
    }

    int n = breeds.size();
    if (n == 0) {
        cerr << "Error: no breeds loaded!, please check you havbe the right dogbreeds.csv file" << endl;
        return;
    }

    //Build lookup: breed -> index
    unordered_map<string, int> breedIndex;
    for (int i = 0; i < n; i++) {
        breedIndex[breeds[i]] = i;
    }

    string line;
    while (getline(input_File, line)) {
        if (breedIndex.find(line) != breedIndex.end()) {
            int idx = breedIndex[line];
            int decIdx = (idx - key + n) % n;
            char originalChar = (char)decIdx;
            output_File << originalChar;
        }
    }
}

//main
int main() {
    int choice, key;
    string inputFile, outputFile;
    string dogFile = "dogbreeds.csv";

    // Load all breeds
    vector<string> dogBreed = dog_Breeds(dogFile);
    if (dogBreed.empty()) {
        cerr << "Error: Could not load breeds from " << dogFile << endl;
        return 1;
    }

    cout << "1. Encrypt a file\n"
         << "2. Decrypt a file\n";
    cout << "Enter Choice: ";

    //input validation
    while (true) {
        if (cin >> choice && (choice == 1 || choice == 2)) {
            break;
        } else {
            cout << "Invalid Input, please enter 1 or 2: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }

    cout << "Enter the input file name: ";
    cin >> inputFile;
    while (true) {
        ifstream tmpInput(inputFile);
        if (tmpInput) break;
        cout << "Error opening input file, try again: ";
        cin >> inputFile;
    }

    cout << "Enter the output file name: ";
    cin >> outputFile;

    cout << "Enter the encryption/decryption key (integer): ";
        while (true) {
            cin >> key;

            if (cin.fail()) {
                cin.clear();

                cin.ignore(100000, '\n');
                cout << "Invalid input, please enter a integer for the key\n";
            }
            else {
                break;
            }
        }

    if (choice == 1) {
        encryptfile(inputFile, outputFile, key, dogBreed);
        cout << "Encryption complete -> " << outputFile << endl;
    } else {
        decryptFile(inputFile, outputFile, key, dogBreed);
        cout << "Decryption complete -> " << outputFile << endl;
    }

    return 0;
}
