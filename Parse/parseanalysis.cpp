#include "parseanalysis.h"
#include<stack>
#include<iostream>
#include<iomanip>
#include<algorithm>
using namespace std;

string myitos(int x) {
    string ans;
    while (x) {
        char c = x % 10 + '0';
        x = x / 10;
        ans = c+ans;
    }
    return ans;
}

bool myisdigit(const string&s){
    for(auto c:s)
        if(c<'0'||c>'9')
            return false;
    return true;
}

//给出文法初始化
string ParseAnalysis::init() {
    //初始化符号表
    ptr = 0;//
    tbls.emplace_back(-1);//
    string ret;
    set_of_vn.emplace_back("Program");
    set_of_vn.emplace_back("Type");
    set_of_vn.emplace_back("Func_Variable_Sequence");
    set_of_vn.emplace_back("Func_Variable_Declare");

    set_of_vn.emplace_back("Inner_Declare");
    set_of_vn.emplace_back("Inner_Variable_Declare");

    set_of_vn.emplace_back("Statement_Block");
    set_of_vn.emplace_back("Statement_Sequence");
    set_of_vn.emplace_back("Statement");
    set_of_vn.emplace_back("If_Statement");
    set_of_vn.emplace_back("Else_Statement");
    set_of_vn.emplace_back("While_Statement");
    set_of_vn.emplace_back("Return_Statement");
    set_of_vn.emplace_back("Return_Statement_In");
    set_of_vn.emplace_back("Assign_Statement");

    set_of_vn.emplace_back("Expression");
    set_of_vn.emplace_back("Expression_In");
    set_of_vn.emplace_back("Plus_Expression");
    set_of_vn.emplace_back("Plus_Expression_In");
    set_of_vn.emplace_back("Relop");
    set_of_vn.emplace_back("Item");
    set_of_vn.emplace_back("Item_In");
    set_of_vn.emplace_back("Factor");


    //和语法分析器的stirng结果一致
    set_of_vt.emplace_back("int");
    set_of_vt.emplace_back("void");
    set_of_vt.emplace_back("if");
    set_of_vt.emplace_back("else");
    set_of_vt.emplace_back("while");
    set_of_vt.emplace_back("return");
    set_of_vt.emplace_back("identifier");
    set_of_vt.emplace_back("=");
    set_of_vt.emplace_back("+");
    set_of_vt.emplace_back("-");
    set_of_vt.emplace_back("*");
    set_of_vt.emplace_back("/");
    set_of_vt.emplace_back("==");
    set_of_vt.emplace_back(">=");
    set_of_vt.emplace_back(">");
    set_of_vt.emplace_back("<=");
    set_of_vt.emplace_back("<");
    set_of_vt.emplace_back("!=");
    set_of_vt.emplace_back(";");
    set_of_vt.emplace_back(",");
    set_of_vt.emplace_back("(");
    set_of_vt.emplace_back(")");
    set_of_vt.emplace_back("{");
    set_of_vt.emplace_back("}");
    set_of_vt.emplace_back("#");
    set_of_vt.emplace_back("constant");
    set_of_vt.emplace_back("empty");

    //Program -> <Type>Id'('<Func_Variable_Sequence>')'<Statement_Block>
    V_Type index;
    Result r;
    index = { V::vn,int(Vn_Type::Program)};
    r={
        {V::vn,int(Vn_Type::Type)},
        {V::vt,int(Vt_Type::identifier)},
        {V::vt,int(Vt_Type::sign_left_small)},
        {V::vn,int(Vn_Type::Func_Variable_Sequence)},
        {V::vt,int(Vt_Type::sign_right_small)},
        {V::vn,int(Vn_Type::Statement_Block)}
    };
    production[index].push_back(r);

    //<Type> -> int|void
    index= { V::vn,int(Vn_Type::Type) };
    r = { {V::vt,int(Vt_Type::key_int)} };
    production[index].push_back(r);
    r = { {V::vt,int(Vt_Type::key_void)} };
    production[index].push_back(r);

    //<Func_Variable_Sequence>  -> empty|<Inner_Variable_Declare><Func_Variable_Declare>
    index = { V::vn,int(Vn_Type::Func_Variable_Sequence)};
    r = { {V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
        {V::vn,int(Vn_Type::Inner_Variable_Declare)},
        {V::vn,int(Vn_Type::Func_Variable_Declare)}
    };
    production[index].push_back(r);

    //<Func_Variable_Declare>  -> empty|,<Inner_Variable_Declare><Func_Variable_Declare>
    index = { V::vn,int(Vn_Type::Func_Variable_Declare) };
    r = { {V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
        {V::vt,int(Vt_Type::sign_comma)},
        {V::vn,int(Vn_Type::Inner_Variable_Declare)},
        {V::vn,int(Vn_Type::Func_Variable_Declare)}
    };
    production[index].push_back(r);

    //<Statement_Block>  -> '{' <Inner_Declare><Statement_Sequence>'}'
    index = { V::vn,int(Vn_Type::Statement_Block) };
    r = {
        {V::vt,int(Vt_Type::sign_left_big)},
        {V::vn,int(Vn_Type::Inner_Declare)},
        {V::vn,int(Vn_Type::Statement_Sequence)},
        {V::vt,int(Vt_Type::sign_right_big)},
    };
    production[index].push_back(r);

    //<Inner_Declare>  -> empty| <Inner_Variable_Declare>;<Inner_Declare>
    index = { V::vn,int(Vn_Type::Inner_Declare) };
    r = { {V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
        {V::vn,int(Vn_Type::Inner_Variable_Declare)},
        {V::vt,int(Vt_Type::sign_semicolon)},
        {V::vn,int(Vn_Type::Inner_Declare)}
    };
    production[index].push_back(r);

    //<Inner_Variable_Declare>  -> int Id
    index = { V::vn,int(Vn_Type::Inner_Variable_Declare) };

    r = {
        {V::vt,int(Vt_Type::key_int)},
        {V::vt,int(Vt_Type::identifier)}
    };
    production[index].push_back(r);

    //<Statement_Sequence>  -> <Statement><Statement_Sequence>|empty
    index = { V::vn,int(Vn_Type::Statement_Sequence) };
    r = { {V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
        { V::vn,int(Vn_Type::Statement) },
        { V::vn,int(Vn_Type::Statement_Sequence) }
    };
    production[index].push_back(r);

    //<Statement>  -> <If_Statement>|<Assign_Statement>|<While_Statement>|<Return_Statement>
    index = { V::vn,int(Vn_Type::Statement) };
    r = {
        { V::vn,int(Vn_Type::If_Statement) },
    };
    production[index].push_back(r);

    r = {
        { V::vn,int(Vn_Type::Assign_Statement) }
    };
    production[index].push_back(r);

    r = {
        {V::vn,int(Vn_Type::While_Statement) }
    };
    production[index].push_back(r);

    r = {
        { V::vn,int(Vn_Type::Return_Statement) }
    };
    production[index].push_back(r);
    //<Assign_Statement>  -> id = <Expression>;
    index = { V::vn,int(Vn_Type::Assign_Statement) };
    r = {

        { V::vt,int(Vt_Type::identifier) },
        { V::vt,int(Vt_Type::sign_assign) },
        { V::vn,int(Vn_Type::Expression) },
        { V::vt,int(Vt_Type::sign_semicolon)}
    };
    production[index].push_back(r);

    //<Return_Statement>  -> return<Return_Statement_In>;
    index = { V::vn,int(Vn_Type::Return_Statement) };
    r = {
        { V::vt,int(Vt_Type::key_return) },
        { V::vn,int(Vn_Type::Return_Statement_In) },
        { V::vt,int(Vt_Type::sign_semicolon)}
    };
    production[index].push_back(r);

    //<Return_Statement_In>  -> empty|<Expression>;
    index = { V::vn,int(Vn_Type::Return_Statement_In) };
    r = { {V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
    { V::vn,int(Vn_Type::Expression)}
    };
    production[index].push_back(r);

    //<While_Statement>  -> while '(' <Expression> ')' <Statement_Block>
    index = { V::vn,int(Vn_Type::While_Statement) };
    r = {
        { V::vt,int(Vt_Type::key_while) },
        { V::vt,int(Vt_Type::sign_left_small) },
        { V::vn,int(Vn_Type::Expression) },
        { V::vt,int(Vt_Type::sign_right_small)},
        { V::vn,int(Vn_Type::Statement_Block) },
    };
    production[index].push_back(r);

    //<If_Statement>  -> if '(' <Expression> ')' <Statement_Block><Else_Statement>
    index = { V::vn,int(Vn_Type::If_Statement) };
    r = {
        { V::vt,int(Vt_Type::key_if) },
        { V::vt,int(Vt_Type::sign_left_small) },
        { V::vn,int(Vn_Type::Expression) },
        { V::vt,int(Vt_Type::sign_right_small)},
        { V::vn,int(Vn_Type::Statement_Block) },
        { V::vn,int(Vn_Type::Else_Statement) }
    };
    production[index].push_back(r);

    //<Else_Statement>  -> Emptpy|else <Statement_Block>
    index = { V::vn,int(Vn_Type::Else_Statement) };
    r = {{ V::vt,int(Vt_Type::empty)}};
    production[index].push_back(r);
    r = {
        { V::vt,int(Vt_Type::key_else)},
        { V::vn,int(Vn_Type::Statement_Block)}
    };
    production[index].push_back(r);

    //<Expression>  -> <Plus_Expression><Expression_In>

    index = { V::vn,int(Vn_Type::Expression) };
    r = {
        { V::vn,int(Vn_Type::Plus_Expression)},
        { V::vn,int(Vn_Type::Expression_In)}
    };
    production[index].push_back(r);

    //<Expression_In>  -> empty|<Relop><Expression>
    index = { V::vn,int(Vn_Type::Expression_In) };
    r = { { V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
        { V::vn,int(Vn_Type::Relop)},
        { V::vn,int(Vn_Type::Expression)}
    };
    production[index].push_back(r);


    //<Relop>  -> <|<=|>|>=|==|!=
    index = { V::vn,int(Vn_Type::Relop) };
    r = { { V::vt,int(Vt_Type::sign_smaller)} };
    production[index].push_back(r);
    r = { { V::vt,int(Vt_Type::sign_smaller_equal)} };
    production[index].push_back(r);
    r = { { V::vt,int(Vt_Type::sign_bigger)} };
    production[index].push_back(r);
    r = { { V::vt,int(Vt_Type::sign_bigger_equal)} };
    production[index].push_back(r);
    r = { { V::vt,int(Vt_Type::sign_equal)} };
    production[index].push_back(r);
    r = { { V::vt,int(Vt_Type::sign_not_equal)} };
    production[index].push_back(r);

    //<Plus_Expression>  -> <Item><Plus_Expression_In>
    index = { V::vn,int(Vn_Type::Plus_Expression) };
    r = {
        { V::vn,int(Vn_Type::Item)},
        { V::vn,int(Vn_Type::Plus_Expression_In)}
    };
    production[index].push_back(r);

    //<Plus_Expression_In>  -> empty|+<Plus_Expression>|-<Plus_Expression>
    index = { V::vn,int(Vn_Type::Plus_Expression_In) };
    r = { { V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
    { V::vt,int(Vt_Type::sign_plus)},
    { V::vn,int(Vn_Type::Plus_Expression)}
    };
    production[index].push_back(r);
    r = {
    { V::vt,int(Vt_Type::sign_minus)},
    { V::vn,int(Vn_Type::Plus_Expression)}
    };
    production[index].push_back(r);

    //<Item> -> <Factor><Item_In>
    //| <Factor>*<Item>|<Factor>/<Item>
    index = { V::vn,int(Vn_Type::Item) };

    r = {
        { V::vn,int(Vn_Type::Factor)},
        { V::vn,int(Vn_Type::Item_In)}
    };
    production[index].push_back(r);

    //<Item_In> -> empty|*<Item>|/<Item>
    index = { V::vn,int(Vn_Type::Item_In) };
    r = { { V::vt,int(Vt_Type::empty)} };
    production[index].push_back(r);
    r = {
    { V::vt,int(Vt_Type::sign_mul)},
    { V::vn,int(Vn_Type::Item)}
    };
    production[index].push_back(r);
    r = {
    { V::vt,int(Vt_Type::sign_div)},
    { V::vn,int(Vn_Type::Item)}
    };
    production[index].push_back(r);

    //<Factor> -> Id|num|'('<Expression>')'
    index = { V::vn,int(Vn_Type::Factor) };
    r = { { V::vt,int(Vt_Type::identifier)} };
    production[index].push_back(r);
    r = { { V::vt,int(Vt_Type::constant)} };
    production[index].push_back(r);
    r = {
        { V::vt,int(Vt_Type::sign_left_small)},
        { V::vn,int(Vn_Type::Expression)},
        { V::vt,int(Vt_Type::sign_right_small)},
    };
    production[index].push_back(r);



    //输出语法到string中

    for(const auto &ele:production){
        ret+=set_of_vn[ele.first.second];
        ret+="->";
        for(int i=0;i<int(ele.second.size());++i){
            for(auto c:ele.second[i]){
                if(c.first==V::vn)
                    ret+=set_of_vn[c.second];
                if(c.first==V::vt)
                    ret+=set_of_vt[c.second];
                ret+=" ";
            }
            ret=ret.substr(0,ret.size()-1);
            ret+="|";
        }
        ret=ret.substr(0,ret.size()-1);
        ret+="\n";
    }
    return ret;


}
bool ParseAnalysis::contain_empty(const  S_Result& s) {
    V_Type temp = { V::vt,int(Vt_Type::empty) };
    if (s.find(temp) != s.cend())
        return true;
    else
        return false;
}
bool  ParseAnalysis::copy_and_add(S_Result& des, const S_Result& src, bool fliter) {
    int len = des.size();
    if (!fliter)
        des.insert(src.begin(), src.end());
    else {
        auto iter = src.find({ V::vt,int(Vt_Type::empty) });
        des.insert(src.begin(), iter);
        des.insert(++iter, src.end());
    }
    return len != int(des.size());

}
bool ParseAnalysis::copy_sequence_first(S_Result& des, const Result& c,int i,bool fliter) {
    if (i >= int(c.size()))
        return false;//为fliter的情况设置
    S_Result& src = first_set[c[i]];
    if (!contain_empty(src)||(!fliter&&i==int(c.size())-1)) {
        return copy_and_add(des, src, false);
    }
    else {
        return copy_and_add(des, src, true)| copy_sequence_first(des, c, ++i);
    }
}
void ParseAnalysis::get_first_set() {
    for (int i = 0; i < int(set_of_vt.size()); ++i) {
        V_Type index = { V::vt,i };
        first_set[index].insert(index);//终结符的First为自身
    }//Rule 1
    bool flag = true;
    int count = 0;
    //循环以下过程直到无新增
    while (flag){
        flag = false;
        ++count;
        for (int i = 0; i < int(set_of_vn.size()); ++i) {
            V_Type index = { V::vn,i };
            //int len = production[index].size();
            for (const auto& c : production[index]) {
                if (c[0].first == V::vt)//以终结符开头，包括->empty的情况
                    flag|=(first_set[index].insert(c[0])).second;//Rule 2
                else {
                    //非终结符开头
                    flag|=copy_sequence_first(first_set[index], c, 0);//Rule 3
                }
            }
        }
    }
    cout << count << endl;
}

void ParseAnalysis::get_follow_set() {
    V_Type start = { V::vn,int(Vn_Type::Program) };
    V_Type well = { V::vt,int(Vt_Type::sign_well) };
    follow_set[start].insert(well);//Rule 1

    int count = 0;
    bool flag = true;
    while (flag){
        ++count;
        flag = false;
        for (int i = 0; i < int(set_of_vn.size()); ++i) {
            V_Type index = { V::vn,i };
            for (const auto& c : production[index]) {
                //正向遍历
                bool last_is_vn = false;
                V_Type last;
                for (int j = 0; j < int(c.size()); ++j) {
                    if (last_is_vn) {
                        //rule 2: 有A->..Bb..，将First[b..]添加到Follow[B]
                        flag|=copy_sequence_first(follow_set[last], c, j, true);
                    }
                    if (c[j].first == V::vn) {
                        last = c[j];
                        last_is_vn = true;
                    }
                    else {

                        last_is_vn = false;
                    }
                }

                //反向遍历 Rule 3
                for (int j = c.size() - 1; j >= 0; --j) {
                    if (c[j].first == V::vn) {
                        flag|=copy_and_add(follow_set[c[j]], follow_set[index], false);
                    }
                    if (!contain_empty(first_set[c[j]]))
                        break;
                }
            }
        }
    }
    cout << count << endl;

}
void  ParseAnalysis::create_table() {

    int len_vn = set_of_vn.size();
    int len_vt = set_of_vt.size() - 1;//不为空符号创建
    table.resize(len_vn);
    for (auto& c : table)
        c.resize(len_vt, -1);

    for (int i = 0; i < int(set_of_vn.size()); ++i) {
        V_Type index = { V::vn,i };
        for (int j = 0; j < int(production[index].size()); ++j) {
            S_Result temp;
            copy_sequence_first(temp, production[index][j], 0);
            for (const auto& ele : temp) {
                if (ele.first == V::vt) {
                    if (Vt_Type(ele.second) == Vt_Type::empty) {
                        //rule3
                        for (const auto& c : follow_set[index]) {
                            if (table[index.second][c.second] != -1)
                                cout << "Warning!"<<index.second<<" "<<c.second << endl;
                            table[index.second][c.second] = j;
                        }
                    }
                    else {//rule2
                        if (table[index.second][ele.second] != -1)
                            cout << "Warning!" << index.second << " " << ele.second << endl;
                        table[index.second][ele.second] = j;
                    }
                }
            }
        }
    }
}

void ParseAnalysis::show_table() {
    int row = table.size();
    int col = table[0].size();

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j)
            cout << setw(4) << table[i][j] << "  ";
        cout << endl;
    }

}

bool ParseAnalysis::search(const std::string&s){
    int p=ptr;
    while(p>=0){
        if(tbls[p].find(s))
            return true;
        p=tbls[p].get_parent();
    }
    return false;
}
ParseAnalysis::V_Type ParseAnalysis::get_next_word(LexicalAnalysis& la) {
    //	auto ret=la.get_next();
    rec_ret = ret;
    ret = la.get_next();
    auto iter=find(set_of_vt.cbegin(), set_of_vt.cend(), ret.first);
    //cout << "next_word:" << ret.first << "  "<<endl;
    return { V::vt,iter - set_of_vt.cbegin() };
}

void ParseAnalysis::make_formu(const string& s1, const string& s2, const string& s3, const string& s4,int pos) {
    string s = "(";
    s += s1;
    s += ",";
    s += s2;
    s += ",";
    s += s3;
    s += ",";
    s += s4;
    s += ")";
    if (pos >= 0) {
        //cout << "pos="<<pos << endl;
        formu.insert(formu.begin() + pos, s);
    }
    else
        formu.emplace_back(s);
}
string ParseAnalysis::act(V_Type index, int no, vector<string>& temp) {
    //cout << "Actin" <<temp.size()<< endl;
    string pb = "fill";
    if (index.second == int(Vn_Type::Program)) {
        if (temp[0] == "int"){
            bool result=tbls[ptr].insert(temp[1], 0);
            if(!result){
                My_ECode=3;
                My_EMess+="redefination of variable ";
                My_EMess+=temp[1];
                My_EMess+="\n";
            }
        }
        else{
            bool result=tbls[ptr].insert(temp[1], 1);
            if(!result){
                My_ECode=3;
                My_EMess+="redefination of variable ";
                My_EMess+=temp[1];
                My_EMess+="\n";
            }
        }
    }
    if (index.second == int(Vn_Type::Program))
        pb = temp[0];
    if (index.second == int(Vn_Type::Inner_Variable_Declare)){
        bool result=tbls[ptr].insert(temp[1], 0);
        if(!result){
            My_ECode=3;
            My_EMess+="redefination of variable ";
            My_EMess+=temp[1];
            My_EMess+="\n";
        }
    }
    if (index.second == int(Vn_Type::Assign_Statement)){
        make_formu(":=", temp[2], "_", temp[0]);
        if(!myisdigit(temp[0])&&!search(temp[0])){
            My_ECode=3;
            My_EMess+="undefination of variable ";
            My_EMess+=temp[0];
            My_EMess+="\n";
        }
        if(!myisdigit(temp[2])&&!search(temp[2])){
            My_ECode=3;
            My_EMess+="undefination of variable ";
            My_EMess+=temp[2];
            My_EMess+="\n";
        }
    }


    if (index.second == int(Vn_Type::Plus_Expression_In)) {
        make_formu(temp[0], "", temp[1], make_temp());
        if(!myisdigit(temp[1])&&!search(temp[1])){
            My_ECode=3;
            My_EMess+="undefination of variable ";
            My_EMess+=temp[1];
            My_EMess+="\n";
        }
        pb = "need";//仍缺少一参数，作为记号传给上层
    }
    if (index.second == int(Vn_Type::Plus_Expression)) {
        if (temp[1] == "empty")
            pb = temp[0];
        if (temp[1] == "need") {
            auto iter=formu.back().find(",,");
            formu.back().insert(iter + 1, temp[0]);
            if(!myisdigit(temp[0])&&!search(temp[0])){
                My_ECode=3;
                My_EMess+="undefination of variable ";
                My_EMess+=temp[0];
                My_EMess+="\n";
            }
            auto iter1 = formu.back().find_last_of(",");
            auto iter2 = formu.back().find(")");
            pb = formu.back().substr(iter1 + 1, iter2 - iter1 - 1);
        }
    }
    if (index.second == int(Vn_Type::Factor)) {
        if (no ==0||no==1)
            pb = temp[0];
        else
            pb = temp[1];

    }
    if (index.second == int(Vn_Type::Item_In)) {
        make_formu(temp[0],"", temp[1], make_temp());
        if(!myisdigit(temp[1])&&!search(temp[1])){
            My_ECode=3;
            My_EMess+="undefination of variable ";
            My_EMess+=temp[1];
            My_EMess+="\n";
        }
        pb = "need";//仍缺少一参数，作为记号传给上层
    }
    if (index.second == int(Vn_Type::Item)) {
        if (temp[1] == "empty")
            pb = temp[0];
        if (temp[1] == "need") {
            auto iter = formu.back().find(",,");
            formu.back().insert(iter + 1, temp[0]);
            if(!myisdigit(temp[0])&&!search(temp[0])){
                My_ECode=3;
                My_EMess+="undefination of variable ";
                My_EMess+=temp[0];
                My_EMess+="\n";
            }
            auto iter1 = formu.back().find_last_of(",");
            auto iter2 = formu.back().find(")");
            pb = formu.back().substr(iter1 + 1, iter2 - iter1 - 1);
        }
    }
    if (index.second == int(Vn_Type::Relop))
        pb=temp[0];
    if (index.second == int(Vn_Type::Expression_In)) {
        make_formu(temp[0], "", temp[1], make_temp());
        if(!myisdigit(temp[1])&&!search(temp[1])){
            My_ECode=3;
            My_EMess+="undefination of variable ";
            My_EMess+=temp[1];
            My_EMess+="\n";
        }
        pb = "need";//仍缺少一参数，作为记号传给上层
    }
    if (index.second == int(Vn_Type::Expression)) {
        if (temp[1] == "empty")
            pb = temp[0];
        if (temp[1] == "need") {
            auto iter = formu.back().find(",,");
            formu.back().insert(iter + 1, temp[0]);
            if(!myisdigit(temp[0])&&!search(temp[0])){
                My_ECode=3;
                My_EMess+="undefination of variable ";
                My_EMess+=temp[0];
                My_EMess+="\n";
            }
            auto iter1 = formu.back().find_last_of(",");
            auto iter2 = formu.back().find(")");
            pb = formu.back().substr(iter1 + 1, iter2 - iter1 - 1);
        }
    }
    if (index.second == int(Vn_Type::If_Statement)) {
        //if语句的第一步是()括号内的求值运算
        //这句插入在if求值运算结束后，跳转地址为该句地址+2
        make_formu("jnz", temp[2], "_", myitos(102+ tbls[ptr].end_pos+tbls[ptr].level),tbls[ptr].end_pos);

        //这句紧挨上一句插入,跳转地址为100+else_pos，由于会插入三句j/jnz指令，再+3
        make_formu("j", "_", "_", myitos(103+ tbls[ptr].else_pos + tbls[ptr].level), tbls[ptr].end_pos+1);

        //插在else语句块前面
        make_formu("j", "_", "_", myitos(101+formu.size() + tbls[ptr].level), tbls[ptr].else_pos+2);

    }

    if (index.second == int(Vn_Type::While_Statement)) {
        //while语句的第一步是()括号内的求值运算
        //这句插入在while求值运算结束后，跳转地址为该句地址+2
        make_formu("jnz", temp[2], "_", myitos(102 + tbls[ptr].end_pos), tbls[ptr].end_pos);

        //100为基础，一次+1为下一条，再+1是考虑到下一行又填入了一条指令
        make_formu("j", "_", "_", myitos(100 + formu.size()+2), tbls[ptr].end_pos +1);

        //100为基础,一次-1是因为对while条件的表达式计算在while的上一条
        make_formu("j", "_", "_", myitos(100+ tbls[ptr].while_pos-1));
    }
    //cout << "Actout" << endl;
    return pb;
}

void ParseAnalysis::check(stack<pair<V_Type, int>>&rec_cnt,vector<string>&rec_s) {

    while (!rec_cnt.empty()) {
        auto index = rec_cnt.top().first;
        auto no = rec_cnt.top().second;
        int snum = production[index][no].size();
        int cnt_s = rec_s.size();
        int i = 1;
        if (cnt_s >= snum) {
            for (i = 1; i <= snum; ++i)
                if (rec_s[cnt_s - i] == "_")
                    break;
        }

        if (snum + 1 == i) {

            rec_cnt.pop();
            vector<string>temp;
            for (int j = 0; j < snum; ++j) {
                temp.push_back(rec_s.back());
                rec_s.pop_back();
            }
            reverse(temp.begin(), temp.end());
            string pb = act(index, no, temp);
            rec_s.pop_back();
            rec_s.push_back(pb);
        }
        else
            break;
    }


}

string ParseAnalysis::make_temp(){
    string ans;
    char c;
    if (ptr >= 10) {
        c = ptr / 10;
        ans += (c + '0');
    }
    c = ptr % 10;
    ans += (c + '0');
    ans+="_T";
    if (++tbls[ptr].cnt >= 10) {
        c = tbls[ptr].cnt / 10;
        ans += (c + '0');
    }
    c= tbls[ptr].cnt % 10;
    ans += (c + '0');
    tbls[ptr].insert(ans, 0);
    return ans;
}


std::string ParseAnalysis::do_analyze(LexicalAnalysis& la) {
    if(My_ECode)
        return "";//有语法错误
    string ans;
    int count = 0;
    stack<V_Type>s;
    stack<pair<V_Type,int>>rec_cnt;//使用过的表达式入栈
    vector<string>rec_s;//记录历史 ，其中_符号用于区分递归层数
    s.push({ V::vt,int(Vt_Type::sign_well) });//栈底#号
    s.push({ V::vn,int(Vn_Type::Program) });//开始符号
    V_Type ele = get_next_word(la);
    while (1) {
        char count_s[10];
        itoa(++count,count_s,10);
        ans+=(string("Step")+count_s+":");
        //cout << "Step" << ++count << ":";
        V_Type index = s.top();
        if (ele.first == index.first && ele.second == index.second) {
            //匹配成功
            if (ele.first == V::vt && Vt_Type(ele.second) == Vt_Type::sign_well){
                ans+="Success!\n";
                return ans;
            }
            else {
                ans+="match\n";
                if (ele.second == int(Vt_Type::sign_left_big)) {
                    if (tbls[ptr].end_change > 0) {
                        tbls[ptr].end_pos = formu.size();
                        --tbls[ptr].end_change;
                    }

                    tbls.emplace_back(-1);//开新符号表
                    tbls.back().set_parent(ptr);
                    if(ptr>0)
                        tbls.back().level = tbls[ptr].level + 2 + (rec_ret.first == "else");
                    ptr = tbls.size() - 1;
                }
                if (ele.second == int(Vt_Type::sign_right_big)) {
                   ptr = tbls[ptr].get_parent();
                   //tbls.pop_back();//离开作用域，销毁符号表
                }
                if (ele.second == int(Vt_Type::key_if)) {
                   tbls[ptr].if_pos = formu.size();
                   ++tbls[ptr].end_change;
                }
                if (ele.second == int(Vt_Type::key_else)) {
                   tbls[ptr].else_pos = formu.size();
                }
                if (ele.second == int(Vt_Type::key_while)) {
                   tbls[ptr].while_pos = formu.size() + 1;
                   ++tbls[ptr].end_change;
                }

                if (Vt_Type(ele.second) == Vt_Type::identifier|| Vt_Type(ele.second) == Vt_Type::constant)
                   rec_s.push_back(ret.second);
                else
                   rec_s.push_back(set_of_vt[ele.second]);
                s.pop();
                ele = get_next_word(la);
            }
        }
        else {
            if (index.first == V::vt) {
                My_ECode=2;
                ans+=string("No production for Vt:")+set_of_vt[index.second]+"\n";
                return ans;
            }

            //查表使用产生式
            int no = table[index.second][ele.second];
            if (no == -1) {
                My_ECode=2;
                ans+=string("No production for Vn:")+set_of_vn[index.second]+"\n";
                return ans;
            }
            else {
                //使用no号产生式
                 ans+=set_of_vn[index.second]+"->";
                s.pop();
                for (auto iter = production[index][no].crbegin();
                    iter != production[index][no].crend(); ++iter) {
                    if ((*iter).first == V::vt && Vt_Type((*iter).second) == Vt_Type::empty)
                        continue;
                    s.push(*iter);
                }

                //输出产生式和压入顺序相反
                for (auto iter = production[index][no].cbegin();
                    iter != production[index][no].cend(); ++iter) {
                    if ((*iter).first == V::vn)
                        ans+=set_of_vn[(*iter).second]+" ";
                    else
                        ans+=set_of_vt[(*iter).second]+" ";
                }
                ans+="\n";
                if (production[index][no].size() == 1 && production[index][no][0].first == V::vt &&
                    Vt_Type(production[index][no][0].second) == Vt_Type::empty) {
                    rec_s.push_back("empty");
                }//处理只推空的情况
                else {
                    rec_cnt.push({ index,no });
                    rec_s.push_back("_");
                }
            }
        }
        check(rec_cnt, rec_s);
    }

}

string ParseAnalysis::get_semantic(){
    string ans;
    for(int i=0;i<int(formu.size());++i){
        ans+=myitos(100+i);
        ans+=formu[i];
        ans+="\n";
    }
    return ans;
}


void ParseAnalysis::get_print() {
    //first
    for(const auto &ele:first_set){
        first_ret+="FIRST(";
        if(ele.first.first==V::vn)
            first_ret+=set_of_vn[ele.first.second];
        if(ele.first.first==V::vt)
            first_ret+=set_of_vt[ele.first.second];
        first_ret+="):";

        for(const auto& c:ele.second){
            if(c.first==V::vn)
                first_ret+=set_of_vn[c.second];
            if(c.first==V::vt)
                first_ret+=set_of_vt[c.second];
            first_ret+=" ";
        }
        first_ret+="\n";
    }


    //follow
    for(const auto &ele:follow_set){
        follow_ret+="FOLLOW(";
        if(ele.first.first==V::vn)
            follow_ret+=set_of_vn[ele.first.second];
        if(ele.first.first==V::vt)
            follow_ret+=set_of_vt[ele.first.second];
        follow_ret+="):";

        for(const auto& c:ele.second){
            if(c.first==V::vn)
                follow_ret+=set_of_vn[c.second];
            if(c.first==V::vt)
                follow_ret+=set_of_vt[c.second];
            follow_ret+=" ";
        }
        follow_ret+="\n";
    }


   table_ret+="   ";
   char num[10];
   for(int i=0;i<int(table[0].size());++i){
       if(i<10)
            table_ret+=" ";
       itoa(i,num,10);
       table_ret+=num;
       table_ret+=" ";
   }
   table_ret+="\n";

   int count=0;
   for(const auto&ele:table){
       if(count<10)
            table_ret+=" ";
       itoa(count,num,10);
       table_ret+=num;
       table_ret+=" ";
       for(const auto&c:ele){
           itoa(c,num,10);
           if(c>=0)
               table_ret+=" ";
           table_ret+=num;
           table_ret+=" ";
       }
       table_ret+="\n";
       ++count;
    }

}
