#ifndef LEXICALANALYSIS_H
#define LEXICALANALYSIS_H
#include"para.h"
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include<map>
enum class Token_Type {key_word,identifier,constant,sign};
class LexicalAnalysis
{
public:
    using ret_type = std::pair<std::string, std::string>;
    LexicalAnalysis() = delete;
    LexicalAnalysis(std::ifstream& ifs):pos(0){
        init(ifs);
    }//输入;
    ret_type get_next();
    std::string show();//测试用

private:
    int pos;
    static std::map<Token_Type,std::vector<std::string>>tokens;
    std::vector<std::vector<std::string>> file; //待分析文本
    std::vector<ret_type>rets;//分析结果
    std::vector<std::string>identifier_table;
    bool note_valid;//处于注释段中

    bool analysis(const std::string&);//分析
    void init(std::ifstream&);
    void add_to_ret(const ret_type&);
    bool has_sign(const std::string&);
    bool has_keyword(const std::string&);
    bool has_identifier(const std::string&);
    bool has_constant(const std::string&);
};



#endif // LEXICALANALYSIS_H
