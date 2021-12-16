#ifndef PARSEANALYSIS_H
#define PARSEANALYSIS_H


#include<set>
#include<string>
#include<map>
#include<stack>
#include<vector>
#include"lexicalanalysis.h"
#include"Semantic.h"
class ParseAnalysis{
public:
    enum class V {vn,vt};
    using V_Type = std::pair<V, int>;
    enum struct Vn_Type {
        Program,
        Type,
        Func_Variable_Sequence,
        Func_Variable_Declare,
        Inner_Declare,
        Inner_Variable_Declare,
        Statement_Block,
        Statement_Sequence,
        Statement,
        If_Statement,
        Else_Statement,
        While_Statement,
        Return_Statement,
        Return_Statement_In,
        Assign_Statement,
        Expression,
        Expression_In,
        Plus_Expression,
        Plus_Expression_In,
        Relop,
        Item,
        Item_In,
        Factor
    };

    enum struct Vt_Type {
        key_int,
        key_void,
        key_if,
        key_else,
        key_while,
        key_return,
        identifier,
        sign_assign,
        sign_plus,
        sign_minus,
        sign_mul,
        sign_div,
        sign_equal,
        sign_bigger_equal,
        sign_bigger,
        sign_smaller_equal,
        sign_smaller,
        sign_not_equal,
        sign_semicolon,
        sign_comma,
        sign_left_small,
        sign_right_small,
        sign_left_big,
        sign_right_big,
        sign_well,
        constant,
        empty
    };
    using Result = std::vector<V_Type>;
    using S_Result = std::set<V_Type>;
private:
    std::vector<std::string>set_of_vn;//非终结符集合
    std::vector<std::string>set_of_vt;//终结符结合 终结符即为语法分析结果
    std::map<V_Type, std::vector<Result>>production;//产生式
    std::string init();//初始化文法信息
    void get_first_set();//计算first集合
    void get_follow_set();//计算follow集合
    void create_table();//创建预测表
    std::map<V_Type, S_Result>first_set;//Fisrt集合
    std::map<V_Type, S_Result>follow_set;//Follow集合
    std::vector<std::vector<int>>table;
    V_Type get_next_word(LexicalAnalysis& la);
    bool copy_and_add(S_Result&, const S_Result&, bool fliter);
    bool copy_sequence_first(S_Result&, const Result& c, int i, bool fliter = false);//将First(c[i]..)复制
    bool contain_empty(const  S_Result&);

    void get_print();
public:

    std::string init_ret;
    std::string first_ret;
    std::string follow_ret;
    std::string table_ret;
    std::string do_analyze(LexicalAnalysis& la);//根据预测表分析
    ParseAnalysis() {
        init_ret=init();
        get_first_set();
        get_follow_set();
        create_table();
        get_print();
        //show_table();
        //show();
    }
    void show_table();
   // void show();
    std::string get_semantic();
    std::vector<Semantic>tbls;//存放符号表
private:
    int ptr;//当前指向的符号表

    void check(std::stack<std::pair<V_Type, int>>&, std::vector<std::string>&);//检查语义栈
    std::string act(V_Type, int, std::vector<std::string>&);//语义动作 决定回填符号
    std::string make_temp();
    std::pair<std::string,std::string>ret,rec_ret;
    std::vector<std::string>formu;//生成的四元式
    void make_formu(const std::string&, const std::string&,
        const std::string&,const std::string&,int=-1);
    bool search(const std::string&);
};




#endif // PARSEANALYSIS_H
