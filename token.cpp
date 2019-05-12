#include "token.h"

//Created By Kobe on 2019/4/11
using namespace std;


string KeyWord[] = {"if", "else", "do", "while", "int", "float", "bool", "struct", "then", "true"};
char Operate[] = {'+', '*', '=', '!'};
char Divide[] = {';', '(', ')', '{', '}'};
char Space[] = {' ', '\t', '\r', '\n'};

map<string, string> relatemap = {
        {"+",  "ADD"},
        {"*",  "MUL"},
        {"==", "EQUAL_EQUAL"},
        {"!=", "NOT_EQUAL"},
        {"++", "INC"},
        {"=",  "EQUAL"},
        {";",  "SEMI"},
        {"(",  "SLP"},
        {")",  "SRP"},
        {"{",  "LP"},
        {"}",  "RP"},
};
map<string, int> sortmap = {
        {"if",     2},
        {"else",   3},
        {"do",     4},
        {"while",  5},
        {"int",    6},
        {"double", 7},
        {"bool",   8},
        {"struct", 9},
        {"+",      10},
        {"*",      11},
        {"==",     12},
        {"!=",     13},
        {"++",     14},
        {"=",      15},
        {";",      16},
        {"(",      17},
        {")",      18},
        {"{",      19},
        {"}",      20},
};
//map <int,string> tokenmap;
vector<int> list1;
vector<string> list2;
vector<string> table;
int sortNumber = 0;//attributeVaule
bool IsKeyword(string keyword) {
    //int length = KeyWord->size();  //Wrong!
    int length = sizeof(KeyWord) / sizeof(KeyWord[0]);
    for (int i = 0; i < length; i++) {
        if (keyword == KeyWord[i])
            return true;
    }
    return false;
}

bool IsOperate(char str) {
    int length = sizeof(Operate) / sizeof(Operate[0]);
    for (int i = 0; i < length; i++) {
        if (str == Operate[i])
            return true;
    }
    return false;
}

bool IsDivide(char ch) {
    int length = sizeof(Divide) / sizeof(Divide[0]);
    for (int i = 0; i < length; i++) {
        if (ch == Divide[i])
            return true;
    }
    return false;
}

bool IsSpace(char ch) {
    int length = sizeof(Space) / sizeof(Space[0]);
    for (int i = 0; i < length; i++) {
        if (ch == Space[i])
            return true;
    }
    return false;
}

bool IsLetter(char ch) {
    if ((ch >= 'A' && ch <= 'Z') ||
        (ch >= 'a' && ch <= 'z'))
        return true;
    return false;
}

bool IsDigit(char ch) {
    if (ch >= '0' && ch <= '9') return true;
    return false;
}

string ScanByChar(fstream &file) {
    string str;
    char c;
    file >> noskipws;//not ignore space
    if (!file.is_open()) {
        cout << "Could not find the file\n";
        exit(EXIT_FAILURE);
    }
    while (file >> c) {  //// if while( !file.eof() ) , will repeat read the char at last
        if (file.eof()) break;
        str += c;
    }
    file.close();
    return str;
}

void ScanAnalyse(fstream &file) {
    ofstream resultFile("C:\\Code\\C\\clion\\LR1\\token.txt");
    cout << "into scan..." << endl;
    string str = ScanByChar(file);
    int pos = 0;
    int length = str.length();
    char ch;
    string buffer = "";
    int state = 0;
    while (pos < length) {
        ch = str[pos];
        buffer = "";
        if (IsSpace(ch)) { pos++; }
        else if (IsDivide(ch)) {
            buffer += ch;
            pos++;
            cout << buffer + "  <" + relatemap[buffer] + ", _>" << endl;
            list1.push_back(sortmap[buffer]);
            list2.push_back("NONE");
            resultFile << buffer << " ";
            //tokenmap.insert( pair<int,string>(sortmap[buffer],"NONE") );
        } else if (IsOperate(ch)) {
            buffer += ch;
            pos++;
            if (IsOperate(ch = str[pos]) && pos < length) {
                buffer += ch;
                pos++;
            } else {
            }
            cout << buffer + "  <" + relatemap[buffer] + ", _>" << endl;
            list1.push_back(sortmap[buffer]);
            list2.push_back("NONE");
            resultFile << buffer << " ";
            //tokenmap.insert( pair<int,string>(sortmap[buffer],"NONE") );
        } else if (IsDigit(ch)) {
            state = 1;
            buffer += ch;
            pos++;
            bool flag = true;
            while (!IsSpace(ch = str[pos]) && pos < length && flag) {
                buffer += ch;
                switch (state) {
                    case 1:
                        if (IsDigit(ch)) state = 1;
                        else if (ch == '.') state = 2;
                        else if (ch == 'E' || ch == 'e') state = 4;
                        else {
                            pos--;
                            buffer.erase(buffer.end() - 1);
                            flag = false;
                        }
                        break;
                    case 2:
                        if (IsDigit(ch)) state = 3;
                        else {
                            pos--;
                            buffer.erase(buffer.end() - 1);
                            flag = false;
                        }
                        break;
                    case 3:
                        if (IsDigit(ch)) state = 3;
                        else if (ch == 'E' || ch == 'e') state = 4;
                        else {
                            pos--;
                            buffer.erase(buffer.end() - 1);
                            flag = false;
                        }
                        break;
                    case 4:
                        if (ch == '+' || ch == '-') state = 5;
                        else if (IsDigit(ch)) state = 6;
                        else {
                            pos--;
                            buffer.erase(buffer.end() - 1);
                            flag = false;
                        }
                        break;
                    case 5:
                        if (IsDigit(ch)) state = 6;
                        else {
                            pos--;
                            buffer.erase(buffer.end() - 1);
                            flag = false;
                        }
                        break;
                    case 6:
                        if (IsDigit(ch)) state = 6;
                        else {
                            pos--;
                            buffer.erase(buffer.end() - 1);
                            flag = false;
                        }
                        break;
                    default:
                        pos--;
                        flag = false;
                        buffer.erase(buffer.end() - 1);
                        cout << "error!" << endl;
                        break;
                }
                pos++;
            }
            cout << buffer + " <CONST," + buffer + ">" << endl;
            list1.push_back(9);
            resultFile << "iConst" << " ";
            bool flag2 = false;
            int i = 0;
            for (i = 0; i < table.size(); i++) {
                if (buffer == table[i]) {
                    list2.push_back(to_string(i));
                    flag2 = true;
                }
            }
            if (!flag2) {
                list2.push_back(to_string(sortNumber));
                sortNumber++;
                table.push_back(buffer);
            }
        } else if (IsLetter(ch) || ch == '_') {
            buffer += ch;
            pos++;
            while (!IsSpace(ch = str[pos++]) && pos < length) {
                if (IsLetter(ch) || ch == '_' || IsDigit(ch)) {
                    buffer += ch;
                } else {
                    //fseek(fp,-1L,SEEK_CUR);
                    //fp.seekg(0,ios::cur);
                    pos--;
                    break;
                }
            }
            if (IsKeyword(buffer)) {
                cout << buffer + " <" + buffer + ",_>" << endl;
                list1.push_back(sortmap[buffer]);
                list2.push_back("NONE");
                resultFile << buffer << " ";
            } else {
                cout << buffer + " <IDN," + buffer + ">" << endl;
                list1.push_back(1);
                resultFile << "id" << " ";
                bool flag = false;
                int i = 0;
                for (i = 0; i < table.size(); i++) {
                    if (buffer == table[i]) {
                        list2.push_back(to_string(i));
                        flag = true;
                    }
                }
                if (!flag) {
                    list2.push_back(to_string(sortNumber));
                    sortNumber++;
                    table.push_back(buffer);
                }
            }
        } else if ("/") {
            state = 1;
            buffer += ch;
            pos++;
            ch = str[pos++];
            buffer += ch;
            if (ch == '*') {
                state = 2;
            }
            while (pos < length && state != 4) {
                ch = str[pos++];
                buffer += ch;
                switch (state) {
                    case 2:
                        if (ch == '*') {
                            state = 3;
                        } else {}
                        break;
                    case 3:
                        if (ch == '/') {
                            state = 4;
                        } else if (ch == '*') {
                        } else {
                            state = 2;
                        }
                        break;
                    case 4:
                        break;
                }
            }
            cout << "annotation:" << buffer << endl;
        }
    }
}

void print() {
    int length = list1.size();
    cout << "/*******token:*******/" << endl;
    int i = 0;
    for (i = 0; i < length - 1; i++) {
        cout << "(" << list1[i] << "," + list2[i] + ") , ";
    }
    cout << "(" << list1[i] << "," + list2[i] + ")" << endl;
    int length2 = table.size();
    cout << "/*******table:*******/" << endl;
    for (i = 0; i < length2 - 1; i++) {
        cout << table[i] + " , ";
    }
    cout << table[i] << endl;
}