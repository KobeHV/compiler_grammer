//
// Created by kobe on 2019/4/19.
//珍爱生命，请远离c++14
//

#include "LR1.h"

using namespace std;
string LR::actionStatStr[] = {
        "acc",
        "s",
        "r"
};

//G1
string strVn[] = {"S'", "states", "Type", "E", "T", "B", "rop", "dec", "ass", "cond", "loop", "num"};
string strVt[] = {"#", "id", "boolid", "int", "float", "bool", "=", "+", "*", "(", ")",
                  "if", "then", "else", "{", "}", "do", "while", ";", "!", "&&", "||", "true", "false",
                  "==", "!=", ">=", "<=", ">", "<", "iConst", "fConst"};
string str_all[] = {"S'", "states", "Type", "E", "T", "B", "rop", "dec", "ass", "cond", "loop", "num",
                    "#", "id", "boolid", "int", "float", "bool", "=", "+", "*", "(", ")",
                    "if", "then", "else", "{", "}", "do", "while", ";", "!", "&&", "||", "true", "false",
                    "==", "!=", ">=", "<=", ">", "<", "iConst", "fConst"};
void _split(string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

vector<string> split(string &s, char delim) {
    vector<string> elems;
    _split(s, delim, elems);
    return elems;
}

bool isVt(string &str) {
//    int len = strVt->length();//这样算出来的长度永远为1！！！！
    int len = sizeof(strVt) / sizeof(strVt[0]);
    for (int i = 0; i < len; i++) {
        if (str == strVt[i]) {
            return true;
        }
    }
    return false;
}

bool isVn(string &str) {
    int len = sizeof(strVn) / sizeof(strVn[0]);
    for (int i = 0; i < len; i++) {
        if (str == strVn[i]) {
            return true;
        }
    }
    return false;
}

string Prod::all_str() {
    string str = noTerminal;
    str += " ->";
    for (int i = 0; i < right.size(); i++) {
        str += " " + right[i];
    }
    set<string>::iterator iter;
    for (iter = additionalVt.begin();
         iter != additionalVt.end(); iter++) {
        str += " , " + *iter;
    }
//    str += "   ***attrs: ";
////    map<string, string>::iterator iter2;
////    for (iter2 = attrs.begin(); iter2 != attrs.end(); iter2++) {
////        string string1 = iter2->first;
////        str += "  " + string1;
////    }
    return str;
}

Prod::Prod(string &in) {
    if (in.size() == 0) return;
    string str;
    vector<string> s = split(in, ' ');
    noTerminal = s[0];
    if (s[1] == "->") {
        for (int i = 2; i < s.size(); i++) {
            string str = s[i];
            if (str.at(0) != '{' || (str.at(0) == '{' && str.size() == 1)) {
                right.push_back(str);
            } else if (s[i].at(0) == '{' && s[i].size() > 3) {
                int len = s[i].size();
                string str = s[i].substr(1, len - 2);
                attrs.insert(pair<string, string>(str, " "));
            }
        }
    } else {
        cout << "Error : can't find ->" << endl;
    }
}

Prod::Prod() {}

void Item::add(string &prod) {
    Prod p = Prod(prod);
    if (p.noTerminal == "S'") {
        p.additionalVt.insert("#");
    }
    vector<Prod>::iterator it = find(prods.begin(), prods.end(), p);
    //如果vector中保存的是自定义类型（结构体/类），则需要为该类型重载==操作符。再用find
    if (it == prods.end()) {
        prods.push_back(p);
    }
}

void Item::add(Prod &p) {
    vector<Prod>::iterator it = find(prods.begin(), prods.end(), p);
    //如果vector中保存的是自定义类型（结构体/类），则需要为该类型重载==操作符。再用find
    if (it == prods.end()) {
        prods.push_back(p);
    }
}

void LR::addG() { //加载文法
    fstream file("C:\\Code\\C\\clion\\LR1\\G1.txt", ios::in | ios::out);
//    fstream file("C:\\Code\\C\\clion\\LR1\\G2.txt", ios::in | ios::out);
//    fstream file("C:\\Code\\C\\clion\\LR1\\G_sem.txt", ios::in | ios::out);
    string str;
    file >> noskipws;//not ignore space
    if (!file.is_open()) {
        cout << "Could not find the file\n";
        exit(EXIT_FAILURE);
    }
    while (!file.eof()) {
        getline(file, str);
        G.add(str);
    }
    file.close();

}

void LR::loadStr(string &in) {
//    cout << "loadStr..." << endl;
    vector<std::string> s = split(in, ' ');
    inStr.push_back("#");
    status.push_back(0);
    for (int i = s.size() - 1; i >= 0; --i) {
        inStr.push_back(s[i]);
    }
}

Node *LR::parser(string &str) {
    cout << endl << "************parse..." << str << "*************" << endl;

    bool success = false;
    vector<Node *> node_vec;
    int step = 0;
    while (!success) {
        cout << "step:" << step << endl;
        int sTop = status.size() - 1; // 栈顶
        int iTop = inStr.size() - 1;
        pair<int, string> p = make_pair(status[sTop], inStr[iTop]);
        if (ACTION.find(p) == ACTION.end())  // 出错！
            break;
        pair<actionStat, int> act = ACTION[p];
        if (act.first == SHIFT) { // 移进
            cout << "SHIFT" << endl;
            node_vec.push_back(new Node(inStr[iTop]));

            status.push_back(act.second);
            parse.push_back(inStr[iTop]);
            inStr.pop_back();
        } else if (act.first == REDUCE) {
            Prod p = G.prods[act.second];//要规约的表达式
            string string1 = p.all_str();
            cout << "REDUCE: " + string1 << endl;
            vector<Node *> childs;
            for (int i = 0; i < p.right.size(); i++) {
                childs.push_back(node_vec[i + node_vec.size() - p.right.size()]);
            }


            if (p.right.size() != 0) { // 空串，无需出栈，直接规约
                for (int i = 0; i < p.right.size(); i++) {
                    status.pop_back();
                    parse.pop_back();
                    node_vec.pop_back();
                }
            }
            Node *father = new Node(p.noTerminal);
            father->prod = p;
            node_vec.push_back(father);
            node_vec[node_vec.size() - 1]->childs = childs;


            parse.push_back(p.noTerminal);
            sTop = status.size() - 1; // 栈顶
            iTop = inStr.size() - 1;
            pair<int, string> goto_pair =
                    make_pair(status[status.size() - 1], p.noTerminal);
            status.push_back(GOTO[goto_pair]);
        } else if (act.first == ACCEPT) {
            success = true;
            cout << "Success!" << endl;
        }
        step++;
    }
    if (!success) {
        cout << "parse error!" << endl;
    }
    return node_vec[0];
}

Item LR::closure(Item &I) {
//    cout << "closure..." << endl;
    if (I.prods.size() == 0) return I;
    for (int i = 0; i < I.prods.size(); i++) { //I中每项A->α.Bβ,a
        Prod prod = I.prods[i];
        for (int j = 0; j < prod.right.size() - 1; j++) {
            if (prod.right[j] == ".") {
                string B = prod.right[j + 1];
                string beta;
                int m = 0;
                if (j < prod.right.size() - 2) {
                    for (m = j + 2; m < prod.right.size(); m++) {
                        beta += prod.right[m] + " ";
                    }
                }
                for (int k = 0; k < G.prods.size(); k++) {//B->γ
                    if (G.prods[k].noTerminal == B) {
                        Prod p = G.prods[k];
                        p.right.insert(p.right.begin(), ".");
                        set<string> first;//到外边来声明，最后一起加给p的展望符
                        for (set<string>::iterator it = prod.additionalVt.begin();
                             it != prod.additionalVt.end(); it++) {
                            string str_first = beta + *it;//first(βa)
                            set<string> first1 = LR::first(str_first);
                            first.insert(first1.begin(), first1.end());
                        }
                        p.additionalVt = first;
                        //判断一下有重复的就不要加了
                        auto it = find(I.prods.begin(), I.prods.end(), p);
                        if (it == I.prods.end()) { // 不在I中
                            I.prods.push_back(p);
                        }
                    }
                }
            }
        }
    }
    // ??? 每次循环里push之后,I的size就变了啊,这个怎么处理啊
    return I;
}

Item LR::Goto(Item &I, string X) {
//    cout << "Goto..." << endl;
    Item J;
    if (I.prods.size() == 0) return J; // 项目集为空则返回空项目

    for (int i = 0; i < I.prods.size(); ++i) { // 枚举I的产生式
        Prod prod = I.prods[i];
        for (int j = 0; j < prod.right.size() - 1; j++) {
            if (prod.right[j] == "." && prod.right[j + 1] == X) {
                prod.right[j] = prod.right[j + 1];
                prod.right[j + 1] = ".";
                J.add(prod);
                break;
            }
        }
    }
    return closure(J);
}

void LR::items() {// 加载项目集
    cout << "items..." << endl;
    Item initial;
    string str0 = "S' -> . states";//C[0]
    Prod p = Prod(str0);
    p.additionalVt.insert("#");
    initial.prods.push_back(p);
    Item item = LR::closure(initial);
    C.push_back(item); // 置C初值
    //???随时更新，不知道循环有没有问题
    for (int i = 0; i < C.size(); ++i) { // C的每个项目集
        int len1 = C.size();
        Item I = C[i];
        int len = sizeof(str_all) / sizeof(str_all[0]);
        for (int j = 0; j < len; j++) {
            string X = str_all[j];
            Item next = Goto(I, X);
            if (next.prods.size() != 0) { // 不为空
                auto it = find(C.begin(), C.end(), next);
                if (it == C.end()) { // 不在C中
                    C.push_back(next);
                }
            }
        }
    }
}

void LR::build() { // 构造Action、GOTO表
    cout << "build..." << endl;
    items();
//    cout << "items end" << endl;

    for (int i = 0; i < C.size(); ++i) { // 逐个项目集
        Item item = C[i];
        //GOTO
        int length = sizeof(str_all) / sizeof(str_all[0]);
        for (int j = 0; j < length; j++) {
            Item goto_item = Goto(item, str_all[j]);
            string X = str_all[j];
            for (int m = 0; m < C.size(); m++) {
                if (goto_item == C[m]) {
                    GOTO[make_pair(i, str_all[j])] = m;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < C.size(); ++i) { // 逐个项目集
        Item item = C[i];
        //Action
        for (int k = 0; k < item.prods.size(); k++) { // 逐个项目
            Prod prod = item.prods[k];
            for (int j = 0; j < prod.right.size(); j++) {
                if (prod.right[j] == "." && j < prod.right.size() - 1) {//A->α.aβ,b
                    string a = prod.right[j + 1];
                    if (isVt(a)) { // 终结符
                        int goto_index = GOTO[make_pair(i, a)];
                        ACTION[make_pair(i, a)] = make_pair(SHIFT, goto_index);
                    }
                } else if (prod.right[j] == "." && j == prod.right.size() - 1
                           && prod.noTerminal != "S'") {//A->α.,a
                    prod.right.erase(prod.right.end());//去掉最后的 . ,否则在后边的G文法项目比较的时候找不到相等的
                    vector<Prod> g_prods = G.prods;
                    for (set<string>::iterator it = prod.additionalVt.begin();
                         it != prod.additionalVt.end(); it++) {
                        string a = *it;
                        int m = 0;
                        for (m = 0; m < g_prods.size(); m++) {
                            if (prod.noTerminal == g_prods[m].noTerminal
                                && prod.right == g_prods[m].right) {
                                ACTION[make_pair(i, a)] = make_pair(REDUCE, m);
                            }
                        }
                    }
                } else if (prod.right[j] == "." && j == prod.right.size() - 1
                           && prod.noTerminal == "S'") {//A->α.,a
                    ACTION[make_pair(i, "#")] = make_pair(ACCEPT, -1);
                }
            }
        }

    }
    cout << "build...end" << endl;
}

set<string> LR::first(string &str) { // s不为产生式
    string str1 = str;
    vector<std::string> s = split(str1, ' ');
    set<string> set_first;

    if (s.size() == 0)
        return set_first;
    if (isVt(s[0])) {
        set_first.insert(s[0]);
    } else {
        for (int i = 0; i < G.prods.size(); i++) {//设计文法中无空产生式
            Prod p = G.prods[i];
            if (p.noTerminal == s[0]) {
                string X = p.right[0];
                if (isVt(X)) {
                    set_first.insert(X);
                } else {
                    set<string> f1 = first(X);
                    set<string>::iterator it; //定义前向迭代器
                    for (it = f1.begin(); it != f1.end(); it++) {
                        set_first.insert(*it);
                    }
                }
            }
        }
    }
    return set_first;
}

void LR::printTable() {
    cout << "print..." << endl;
    map<pair<int, string>, pair<actionStat, int> >::iterator iter;
    iter = ACTION.begin();
    cout << "****************************************" << endl;
    cout << "ACTION[ <i,X> , <enum,j> ]" << endl;
    while (iter != ACTION.end()) {
        cout << "[ <" << iter->first.first << "," << iter->first.second << "> , <"
             << iter->second.first << "," << iter->second.second << "> ]" << endl;
        iter++;
    }
    map<pair<int, string>, int>::iterator iter2;
    iter2 = GOTO.begin();
    cout << endl << "****************************************" << endl;
    cout << "GOTO[<i,X> , j]" << endl;
    while (iter2 != GOTO.end()) {
        string str = iter2->first.second;
        if (isVn(str)) {//注意 isVn("xx")就不对，必须先声明一个string再传进去,GOTO表的原意只有针对Vn，所以这里只打印Vn
            cout << "[ <" << iter2->first.first << "," << iter2->first.second << "> , "
                 << iter2->second << " ]" << endl;
        }
        iter2++;
    }
}

void LR::printC() {
    for (int i = 0; i < C.size(); i++) {
        Item item = C[i];
        cout << "Item[" << i << "]:" << endl;
        for (int j = 0; j < item.prods.size(); j++) {
            Prod prod = item.prods[j];
            string string1 = prod.all_str();
            cout << string1;
            cout << endl;
        }
        cout << endl;
    }
}

void scanTree(Node *root) {
    cout << root->V <<endl;
    if(root->childs.size() == 0){
        return;
    }
    for(Node *p : root->childs){
        scanTree(p);
    }
}
Node *LR::run(fstream &file) {
    cout << "run..." << endl;
    vector<string> in;
    string str;
    file >> noskipws;//not ignore space
    if (!file.is_open()) {
        cout << "Could not find the file" << endl;
        exit(EXIT_FAILURE);
    }
    while (!file.eof()) {
        getline(file, str);
//        cout << str << endl;
        in.push_back(str);
    }
    file.close();

    LR::addG();
    LR::build();

    LR::printTable();
    LR::printC();
    LR::loadStr(in[0]);
    Node *root = LR::parser(in[0]);
    //scanTree(root);
    return root;
//    return NULL;
}



