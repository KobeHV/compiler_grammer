//
// Created by kobe on 2019/4/19.
//
#include "LR1.h"
#include "semantic.cpp"

int main() {

    fstream file("C:\\Code\\C\\clion\\LR1\\test1.txt", ios::in);
    LR lr;
    Node *root = lr.run(file);
    cout << "************ScanTree***********" << endl;
    scanTree(root);
    return 0;
}
