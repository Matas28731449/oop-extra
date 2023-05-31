#include "headers/library.h"

void read(map< string, map< size_t, size_t > > &references, map< string, size_t > &word_count, map< string, size_t > &link_list) {
    ifstream in;
    string   row,
             word,
             file;
    bool     invalid; // declaring a boolean variable for validation
    int      row_number = 1; // initializing a row number variable

    system("ls read"); // running a system command to list files in the "read" directory
    cout << "Enter the name of the file (be sure to add *.txt file extension): "; cin >> file;

    try {
        cout << "Reading data...\n";
        ifstream in("read/" + file);
        if(in.fail()) throw 400;
        while(!in.eof()) {
            getline(in, row); // reading a line from the file
            stringstream ss_in(row); // creating a stringstream object from the line
            while(ss_in >> word) { // extracting words from the stringstream
                if(regex_match(word, regex("[(http(s)?):\\/\\/(www\\.)?a-zA-Z0-9@:%._\\+~#=]{2,256}\\.(?!jpg|svg|img|png)[a-z]{2,6}\\b([-a-zA-Z0-9@:%_\\+.~#?&//=]*)"))) {
                    link_list.insert(pair<string, int>(word, row_number)); // inserting the link and row number into the link_list map if it matches the regex
                }
                else {
                    auto it = remove_if(word.begin(), word.end(),
                    [](const char symbol) { // using a lambda function to remove certain symbols from the word
                        return symbol == '.' || symbol == ':' || symbol == ',' || symbol == ';' || symbol == '-' ||
                               symbol == '"' || symbol == '(' || symbol == ')' || symbol == '!' || symbol == '?' ||
                               symbol == '$' || symbol == '%' ;
                    });
                    word.erase(it, word.end()); // erasing the symbols from the word
                }
                word_count[word] ++; // incrementing the word count in the word_count map
                references[word][row_number] ++; // incrementing the count of word occurrences in the references map
            }
            row_number ++; // incrementing the row number
        }
        in.close();
    } catch(int e) {
        cout << "Error " << e << ": file not found\n";
        exit(0);
    }
}

void write(map< string, map< size_t, size_t > > &references, map< string, size_t > &word_count, map< string, size_t > &link_list) {
    cout << "Writing data...\n";
    ofstream out("write/write.txt");

    out << "URL addresses: \n";
    for(auto &i : link_list) {
        out << "'" << i.first << "' " << "in the row: " << i.second << "\n";
    }
    out << "\n";

    for(auto &i : word_count) { // looping through the word_count map
        if(i.second > 1) { // checking if the word appears more than once
            out << "'" << i.first << "' " << "repeated " << i.second << " times:\n";
            for(auto &j : references.at(i.first)) { // looping through the references map for the specific word
                out << "Line " << j.first << ", time(-s) " << j.second << "\n";
            }
            out << "\n";
        }
    }
}

int main() {
    map< string, map< size_t, size_t > > references; // declaring a map to store word references with row numbers and occurrence counts
    map< string, size_t >                word_count, // declaring a map to store word counts
                                         link_list; // declaring a map to store links and row numbers

    read(references, word_count, link_list);
    write(references, word_count, link_list);
    cout << "\nFinished.\nCheck write/write.txt for results.\n";

    return 0;
}
