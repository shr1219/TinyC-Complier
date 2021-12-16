#include "lexicalanalysis.h"
#include <ctype.h>
#include<algorithm>
using namespace std;
//初始化静态成员


std::map<Token_Type, std::vector<std::string>> LexicalAnalysis::tokens = {
    {Token_Type::key_word,  std::vector<std::string>{"int","void","if","else","while","return"}},
    {Token_Type::identifier,std::vector<std::string>{}},
    {Token_Type::constant,  std::vector<std::string>{}},
    {Token_Type::sign,      std::vector<std::string>{"=","+","-","*","/","==",">=",">","<=","<",
    "!=",";",",","/*","*/","//","(",")","{","}","#"}}
};


void LexicalAnalysis::init(ifstream &ifs) {
    string text;
    while (getline(ifs,text)) {
        vector<string>temp;
        istringstream line(text);
        string word;
        while (line>>word){
            temp.push_back(std::move(word));
        }
        file.push_back(std::move(temp));
    }

    for (auto i = file.cbegin(); i != file.cend(); ++i)
        for (auto j = file[i - file.cbegin()].cbegin(); j != file[i - file.cbegin()].cend(); ++j) {
            if (!analysis(*j)) {
                My_ECode=1;
                cout << "识别错误!" << endl;
                return;
            }
        }

}

bool LexicalAnalysis::analysis(const string& word) {
    //word中不含空白

    //正确的情况下：
    //以数字开头的只能是costant
    //以字母开头的只能是identifier /  key_word
    //以非字母、非数字开头只能是sign
    if (word.size() == 0)
        return true;
    if (isdigit(word[0])) {
        return has_constant(word);
    }
    else if (isalpha(word[0])) {
        return has_keyword(word)|| has_identifier(word);//优先判定为keyword
    }
    else {
        return has_sign(word);
    }

    return true;//这句不会使用
}

void LexicalAnalysis::add_to_ret(const ret_type& r) {
    /*
    if (r.first == "identifier") {
        int no;
        auto iter = find(identifier_table.cbegin(), identifier_table.cend(), r.second);
        if (iter == identifier_table.cend()) {
            no = identifier_table.size();
            identifier_table.push_back(r.second);
        }
        else
            //no=1;
            no = iter - identifier_table.cbegin();
        char p[20];
        _itoa_s(no, p, 10);
        rets.push_back(ret_type(r.first, p));
    }//构建常量表
    else
    */
    rets.push_back(r);
}

bool LexicalAnalysis::has_sign(const string& word) {
    string temp;
    temp += word[0];
    if (word.size() > 1) {
        temp += word[1];
        auto iter = find(tokens[Token_Type::sign].cbegin(), tokens[Token_Type::sign].cend(), temp);
        if (iter != tokens[Token_Type::sign].cend()) {
            add_to_ret(ret_type(*iter, ""));
            return analysis(word.substr(2, word.size() - 2));
        }
        temp = temp.substr(0, temp.size() - 1);
    }

    auto iter = find(tokens[Token_Type::sign].cbegin(), tokens[Token_Type::sign].cend(), temp);
    if (iter != tokens[Token_Type::sign].cend()) {
        add_to_ret(ret_type(*iter, ""));
        return analysis(word.substr(1, word.size() - 1));
    }


    return false;

}


bool LexicalAnalysis::has_constant(const string& word) {
    size_t pos = 0;
    for (pos = 0; pos < word.size(); pos++) {
        if (isalpha(word[pos]))
            return false;
        else if (!isdigit(word[pos])) {
            break;
        }
    }
    add_to_ret(ret_type("constant", word.substr(0,pos)));

    return analysis(word.substr(pos, word.size() - pos));
}


bool LexicalAnalysis::has_keyword(const string& word) {
    for (auto kw : tokens[Token_Type::key_word]) {
        if ((word.size() >= kw.size()) && (word.substr(0, kw.size()) == kw)) {
            add_to_ret(ret_type(kw, ""));
            if (word.size() == kw.size())//防止kw.size()越界，尽管此时len=0
                return true;
            return analysis(word.substr(kw.size(), word.size() - kw.size()));
        }
    }
    return false;
}

bool LexicalAnalysis::has_identifier(const string& word) {
    size_t pos;
    for (pos = 0; pos < word.size(); ++pos) {
        if (!isalnum(word[pos]) && !isdigit(word[pos]))
            break;
    }
    add_to_ret(ret_type("identifier", word.substr(0,pos)));
    return analysis(word.substr(pos, word.size() - pos));
}

string LexicalAnalysis::show() {
    string str;
    for (auto ret : rets)
        str+="<"+ret.first+","+ret.second+">\n";
    return str;
}

LexicalAnalysis::ret_type LexicalAnalysis::get_next() {
    return pos < int(rets.size()) ? rets[pos++] : ret_type("#","-");
}
