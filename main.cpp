//
// Created by kobe on 2019/4/19.
//
#include "LR1.h"

int main() {

    fstream file("C:\\Code\\C\\clion\\LR1\\test.txt",ios::in);
    LR lr;
    lr.run(file);

    return 0;
}
