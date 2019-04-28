//
// Created by kobe on 2019/4/19.
//

#ifndef LR1_LR1_H
#define LR1_LR1_H

#include <iostream>
#include <fstream>
#include <ctype.h>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <sstream>

using namespace std;

struct Node {
    Node(string v_) {
        V = v_;
    }

    ~Node() {
        for (Node *p:childs)
            free(p);
    }

    string V;
    vector<Node *> childs;
    map<string, string> attrs;
};

class Prod { // 这里是存放形如X->abc的形式，不存在多个候选式
public:
    string noTerminal; // 产生式左部非终结符名字
    vector<string> right; // 产生式右部
    set<string> additionalVt; // 附加终结符
    map<string, string> attrs;//语义分析的属性

    //重载 “==” 操作符，函数最后的 const 别忘了，否则会报错
    //如果vector中保存的是自定义类型（结构体/类），则需要为该类型重载==操作符。再用find
    bool operator==(const Prod &p) const {

        if (!(this->noTerminal == p.noTerminal && this->right == p.right)) {
            return false;
        }
        // additionalVt别忘了比较展望符
        set<string>::iterator iter1;
        set<string>::iterator iter2;
        for (iter1 = this->additionalVt.begin(), iter2 = p.additionalVt.begin();
             iter1 != this->additionalVt.end() && iter2 != p.additionalVt.end();
             iter1++, iter2++) {
            if (*iter1 != *iter2) {
                return false;
            }
        }
        return true;
    }

    Prod(string &in);

    string all_str();
};

class Item { // 项目集,代表某一个状态
public:
    vector<Prod> prods; // 项目集
    bool operator==(const Item &item) const {
        if (this->prods.size() != item.prods.size()) return false;
        if (this->prods == item.prods) return true;
        return false;
    }

    void add(string &prod);

    void add(Prod &prod);
};

class LR {
public:
    Item G; // 文法G
    enum actionStat {
        ACCEPT = 0,
        SHIFT,
        REDUCE,
    };
    static string actionStatStr[];

    vector<Item> C; // 项目集规范族
    map<pair<int, string>, int> GOTO; // goto数组，项目集<int, int>=char
    map<pair<int, string>, pair<actionStat, int> > ACTION; // Action数组，Action[(i, a)]=(s|r)j
    map<string, set<string> > FIRST; // first集
    set<string> first(string &str); // 求first集
    vector<string> inStr; // 输入串/栈
    vector<int> status; // 状态栈
    vector<string> parse; // 符号栈
    Item closure(Item &I); // 求该项目的闭包
    Item Goto(Item &I, string X); // 求I经过X到达的项目集
    void items(); // 求项目集状态机DFA！!


    void addG(); // 添加产生式
    void build(); // 构造Action、GOTO表

    void loadStr(string &in); // 读取输入串
    Node *parser(string &string); // LR(1)分析
    Node *run(fstream &file);

    void printTable();

    void printC();
};


#endif //LR1_LR1_H
