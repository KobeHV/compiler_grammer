//
// Created by kobe on 2019/4/27.
//
#include "LR1.h"
using namespace std;
void scanTree(Node *root) {
    cout << root->V <<endl;
    if(root->childs.size() == 0){
        return;
    }
    for(Node *p : root->childs){
        scanTree(p);
    }
}
void seman(){

}


