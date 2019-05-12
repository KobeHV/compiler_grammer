//
// Created by kobe on 2019/4/19.
//
#include "LR1.h"
#include "token.h"

int main() {

    //token
    fstream file1("C:\\Code\\C\\clion\\LR1\\test.txt",ios::in);
    ScanAnalyse(file1);
    print();

    //grammer
    fstream file2("C:\\Code\\C\\clion\\LR1\\token.txt", ios::in);
    LR lr;
    Node *root = lr.run(file2);
    //cout << "************ScanTree***********" << endl;
    return 0;
}
