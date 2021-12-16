#ifndef SEMANTIC_H
#define SEMANTIC_H

#include<map>
#include<string>

class Node {
public:
    int type;//0:数值  1:函数
    Node() :type(0) {};
    Node(int t) :type(t) {};
};
class Semantic{
private:
    int parent;//记录一个父节点
    std::map<std::string,Node>local;
public:
    int cnt;
    int if_pos, else_pos,end_pos,while_pos;
    int end_change;//可以改变end_pos的次数
    int level;//if/else/while层数
    //if+2 else+3 while+2
    Semantic(int p) :parent(p),cnt(0),if_pos(0),else_pos(0),
        end_pos(0),while_pos(0),end_change(0),level(0){};
    int get_parent() {
        return parent;
    }

    void set_parent(int n) {
        parent = n;
    }
    bool insert(const std::string& s,Node&&n) {
        if (find(s))
            return false;
        local[s] = n;
        return true;
    }


    bool find(const std::string& s) {
        return local.find(s) != local.end();
    }

};

#endif // SEMANTIC_H
