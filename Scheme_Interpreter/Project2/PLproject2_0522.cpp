#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <regex>
#include <iomanip>
#include <cctype>
#include <set>
using namespace std;

enum Type
{
    LBRACKET,
    RBRACKET,
    DOT,
    INT,
    FLOAT,
    STRING,
    SYMBOL,
    QUOTE,
    NIL,
    TRUE,
    END,
    ERROR,
    Func
};

// AST
struct Token
{
    string str;
    Type type;
    int line, column;
};

struct Node
{
    Token token;
    Node *left = nullptr;
    Node *right = nullptr;
};

int gLine = 1, gColumn = 0;
bool gExit = false;          // 碰到(exit) 結束
bool gErrorOccurred = false; // 有error用
stack<Token> tokenStack;
Token peekToken;
bool peeked = false ;

char GetChar()
{
    char c = cin.get();
    if(c == '\n')
    {
        gLine++;
        gColumn = 0;
    }

    else
        gColumn++;

    return c;
}

// 退回一個字元
void UngetChar(char c)
{
    cin.putback(c);
    if(c == '\n')
    {
        gLine--;
        gColumn = 1;
    }
    else
    {
        gColumn--;
        if(gColumn < 1)
            gColumn = 1;
    }
}

bool IsWhiteSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool IsSeparator(char c)
{
    return c == '(' || c == ')' || c == '\'' || c == '"' || c == ';' || IsWhiteSpace(c);
}

Token GetToken()
{
    Token token;
    if(peeked)
    {
        peeked = false ;
        return peekToken ;
    }

    // 跳過空白
    while(cin.peek() != EOF && IsWhiteSpace(cin.peek()))
        GetChar();

    // 檢查是否已經到檔案結尾
    if(cin.peek() == EOF)
    {
        token.type = END;
        return token;
    }

    char c = cin.peek();

    // 處理註解
    if(c == ';')
    {
        while(cin.peek() != '\n' && cin.peek() != EOF)
            GetChar();
        return GetToken(); // 註解讀完繼續讀下一個token
    }

    // 開始讀token
    c = GetChar();
    token.line = gLine;
    token.column = gColumn;

    if(c == '(')
        token = {"(", LBRACKET, token.line, token.column};
    else if(c == ')')
        token = {")", RBRACKET, token.line, token.column};
    else if(c == '.')
    {
        while(!IsSeparator(cin.peek()))
            token.str+=GetChar();
        if(regex_match(token.str, regex("^[-+]?[0-9]+$")))
            token.type=FLOAT;
        else if(token.str=="")
            token.type=DOT;
        else
            token.type=SYMBOL;
        token.str = c+token.str;
    }
    else if(c == '\'')
        token = {"\'", QUOTE, gLine, gColumn};
    else if(c == '"')
    {

        // 處理字串
        token.str = "\"";
        token.type = STRING;
        while(true)
        {
            int tempL = gLine;
            int tempC = gColumn;
            char ch = GetChar();

            if(ch == '\n' || ch == EOF)
            {
                // ErrorNoClosingQuote(tempL - 1, tempC);
                token.type = ERROR;
                return token;
            }

            if(ch == '"')
            {
                token.str += "\"";
                break;
            }

            // 處理反斜線
            if(ch == '\\')
            {
                char esc = GetChar();

                if(esc == 'n')
                    token.str += "\\n";
                else if(esc == 't')
                    token.str += "\\t";
                else if(esc == '"')
                    token.str += "\\\"";
                else if(esc == '\\')
                    token.str += "\\\\";
                else
                    token.str += string("\\") + esc;
            }
            else
                token.str += ch;
        }
    }
    else
    {
        // 處理一般符號或數字
        token.str += c;
        while(!IsSeparator(cin.peek()) && cin.peek() != EOF)
            token.str += GetChar();

        // classify
        if(token.str == "nil" || token.str == "#f")
            token.type = NIL;
        else if(token.str == "t" || token.str == "#t")
            token.type = TRUE;
        else if(regex_match(token.str, regex("^[-+]?[0-9]+$")))
            token.type = INT;
        else if(regex_match(token.str, regex("^[-+]?([0-9]*\\.[0-9]+|[0-9]+\\.)$")))
            token.type = FLOAT;
        else
            token.type = SYMBOL;
    }
    return token;
}
class Syntax{
    private:
        enum Error
        {
            without_error,
            ATOM,
            LEFT
        };

        Error syntax_error ;

    public:
        vector<Token> tokens ;
        bool Is_ATOM(Token token)
        {
            if(token.type == NIL || token.type == TRUE || token.type == STRING || token.type == INT || 
                 token.type == FLOAT || token.type == SYMBOL)
                return true ;
            return false ;
        }

        bool Is_SEXP() 
        {
            Token token = GetToken();
            tokens.push_back(token);
            if(Is_ATOM(token))
                return true;
            else if(token.type == LBRACKET) 
            {
                int sexp = 0;
                while(true)
                {
                    peekToken=GetToken();
                    peeked = true ;
                    if(peekToken.type == RBRACKET)
                    {
                        tokens.push_back(peekToken);
                        GetToken();
                        if(sexp==0)
                        {
                            Token temp;
                            temp.str="nil";
                            temp.type=NIL;
                            tokens.pop_back();
                            tokens.pop_back();
                            tokens.push_back(temp);
                            return true ;
                        }
                        
                        return true;
                    }
                    else if(peekToken.type==DOT)
                    {
                        
                        tokens.push_back(peekToken);
                        if(sexp==0)
                        {
                            SetError(ATOM);
                            return false ;
                        }

                        GetToken();
                        if(Is_SEXP())
                        {
                            Token temp;
                            temp = GetToken();
                            tokens.push_back(temp);
                            if(temp.type==RBRACKET)
                            {
                                return true ;
                            }
                            else
                            {
                                SetError(LEFT);
                                return false;
                            }
                        }
                        else
                        {
                            SetError(ATOM);
                            return false;
                        }

                        break;
                    }
                    else
                    {
                        if(Is_SEXP())
                            sexp++;
                        else 
                        {
                            SetError(ATOM);
                            return false ;
                        }
                    }
                }

                return true ;
            } 
            else if(token.type == QUOTE) 
            {
                if(Is_SEXP())
                {
                    return true ;
                }
                else
                {
                    SetError(ATOM);
                    return false;
                }
            }
            else 
            {
                SetError(ATOM) ;
                return false ;
            }

            SetError(ATOM);
            return false ;
        } 

        void Error_message() 
        {
            if(tokens[tokens.size()-1].type == ERROR)
            {
                cout << "ERROR(no closing quote) : END-OF-LINE encountered at Line " << tokens[tokens.size()-1].line << " Column " ;
                cout << tokens[tokens.size()-1].column + tokens[tokens.size()-1].str.length() << endl;
            }
            else if(tokens[tokens.size()-1].type == END)
            {
                cout << "ERROR(no more input) : END-OF-FILE encountered" << endl ;
            }
            else if(syntax_error == ATOM)
            {
                cout << "ERROR(unexpected token) : atom or '(' expected when token at Line ";
                cout << tokens[tokens.size()-1].line ;
                cout << " Column " ;
                cout << tokens[tokens.size()-1].column;
                cout << " is >>";
                cout << tokens[tokens.size()-1].str;
                cout << "<<" << endl;
            } 
            else if(syntax_error == LEFT)
            {
                cout << "ERROR(unexpected token) : ')' expected when token at Line ";
                cout << tokens[tokens.size()-1].line ;
                cout << " Column " ;
                cout << tokens[tokens.size()-1].column;
                cout << " is >>";
                cout << tokens[tokens.size()-1].str;
                cout << "<<" << endl ;
            }
        }

        void init()
        {
            syntax_error = without_error ;
        }

        void SetError(Error error) 
        {
            if(syntax_error == without_error)
                syntax_error = error ;
        }

        bool Is_END() 
        {
            if(tokens[tokens.size()-1].type == END)
                return true ;
            return false ;
        }

        bool Is_Error()
        {
            if(tokens[tokens.size()-1].type == ERROR)
                return true ;
            return false ;
        }
};


// 印SExp
void PrintSExp(Node *node, int indent, bool space)
{
    if(!node)
        return;

    // 是 atom
    if(!node->left && !node->right)
    {
        if(space)
            cout << string(indent, ' ');
        if(node->token.type == FLOAT)
            cout << fixed << setprecision(3) << stod(node->token.str) << endl;
        else if(node->token.type == INT)
            cout << stoi(node->token.str) << endl;
        else if(node->token.type == TRUE)
            cout << "#t" << endl;
        else if(node->token.type == NIL)
            cout << "nil" << endl;
        else if(node->token.type == QUOTE)
            cout << "quote"<<endl;
        else if(node->token.type == Func)
            cout << "#<procedure "+node->token.str+ ">\n";
        else if(node->token.type == STRING)
        {
            string s = node->token.str;
            for(size_t i = 0; i < s.length(); ++i)
            {
                if(s[i] == '\\' && i + 1 < s.length())
                {
                    if(s[i + 1] == '\\')
                    {
                        cout << "\\";
                        ++i;
                    }
                    else if(s[i + 1] == '\"')
                    {
                        cout << "\"";
                        ++i;
                    }
                    else if(s[i + 1] == 'n')
                    {
                        cout << "\n";
                        ++i;
                    }
                    else if(s[i + 1] == 't')
                    {
                        cout << "\t";
                        ++i;
                    }
                    else
                    {
                        cout << "\\" << s[i + 1];
                        ++i;
                    }
                }
                else
                    cout << s[i];
            }
            cout << endl;
        }
        else
            cout << node->token.str << endl;
        return;
    }

    // 印(
    if(space)
        cout << string(indent, ' ') ;
    cout << "( ";
    Node *cur = node;
    bool first = true;
    while(cur)
    {
        if(first)
        {
            PrintSExp(cur->left, indent+2, false);
            first = false;
        }
        else
        {
            PrintSExp(cur->left, indent+2, true);
        }

        if(!cur->right || cur->right->token.type == NIL)
            break;

        // right 是 atom（不是 cons cell）
        if(!cur->right->left && !cur->right->right)
        {
            cout << string(indent+2, ' ') << ".\n" ;
            PrintSExp(cur->right, indent+2, true);
            break;
        }

        // 繼續往下走 cons cell
        cur = cur->right;
    }

    cout << string(indent, ' ') << ")" << endl;
}

void COUNTLINE(bool &newline)
{
    while(cin.peek()==' ')
        GetChar();
        if(cin.peek()==';')
        {
            while(cin.peek()!='\n')
                GetChar();
            GetChar();
        }
        else if(cin.peek()!='\n')
            newline= false;
        else
            GetChar();
}


class TreeBuilder {
    private:
        vector<Token> tokens;
        set<string> primitivFunc;
    public:
        Node* BuildTree(int& index) 
        {
            Token token = tokens[index];
            Node* n = new Node();

            if(token.type == LBRACKET) 
            {
                index ++ ;
                n->left = BuildTree(index) ;
                n->right = new Node() ;
                // BuildTree的index 只會計算到結束的token 所以index 要加一
                // 判斷下一個token

                index ++ ;
                Node* current = n->right ;
                Node* newnode = NULL ;
                while(tokens[index].type != RBRACKET && tokens[index].type != DOT) 
                {
                    newnode = BuildTree(index);
                    current->left = newnode; 
                    current->right = new Node();
                    index ++;
                    if(tokens[index].type == DOT)
                    {
                        index++ ;
                        current->right = BuildTree(index);
                        index++ ;   // 跳到右括號
                        return n;
                    }
                    else if(tokens[index].type == RBRACKET)
                    {
                        current->right->token.type = NIL;
                        current->right->token.str = "nil";
                        return n; 
                    }

                    current = current->right;
                }


                if(tokens[index].type == DOT)
                {
                    index++ ;
                    n->right = BuildTree(index);
                    index++ ;   // 跳到右括號
                }
                else if(tokens[index].type == RBRACKET) 
                {
                    current->token.type = NIL;
                    current->token.str = "nil" ;
                }
            }
            else if(token.type == QUOTE)
            {
                n->left = new Node();
                n->right = new Node();
                n->left->token = token; 
                index++;
                n->right->left = BuildTree(index);
                n->right->right = new Node();
                n->right->right->token.type = NIL;
                n->right->right->token.str = "nil";
            }
            else    // atom 
            {
                n->token = token ;
            }

            return n ;
        }

        void init(vector<Token> list) 
        {
            tokens = list ;
        }

        bool check_exit(Node* node)
        {
            if(!node)
                return false ;
            if(!node->right && !node->left)
                return false ;
            if(node->left->token.str == "exit" && node->right->token.type == NIL)
                return true ;
            return false ;
        }
};


class Evaluate {
private:
    map<string, Node*> symbolTable;
    set<string> primitiveNames;
public:
    // 有些有加if 之後感覺沒必要，就不加了，反正每個錯誤後都會初始化
    Node* errornode;
    void InitPrimitives() {
        // 定義所有內建函數名稱
        errornode = nullptr;
        vector<string> primitives = {
            "cons", "list", "quote", "define", "car", "cdr",
            "atom?", "pair?", "list?", "null?", "integer?", "real?", "number?",
            "string?", "boolean?", "symbol?", "+", "-", "*", "/",
            "not", "and", "or", ">", ">=", "<", "<=", "=",
            "string-append", "string>?", "string<?", "string=?",
            "eqv?", "equal?", "begin", "if", "cond", "clean-environment"
        };

        // 將內建函數名稱記錄到 primitiveNames
        for (auto& name : primitives) {
            primitiveNames.insert(name);
        }
    }

    Node* Evalulated(Node* exp) {
        if(!exp) return nullptr;

        if(!exp->left && !exp->right) {  // atom
            if(exp->token.type == SYMBOL) {
                return GetValue(exp);
            }

            return exp;
        }

        if(IsDotPair(exp))
        {
            throw runtime_error("ERROR(non-list) : ") ;
        }

        Node* op = Evalulated(exp->left);
        Node* args = exp->right;

        if(op && (!op->left&&!op->right) && op->token.str=="cons")
        {
            return EvalCons(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str=="list")
        {
            return EvalList(args);
        }
        else if(op && (!op->left&&!op->right) && (op->token.str=="'" || op->token.str=="quote"))
        {
            return args->left;
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "define") 
        {
            return EvalDefine(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "car")
        {
            return EvalCar(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "cdr")
        {
            return EvalCdr(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "atom?")
        {
            return EvalAtom(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "pair?")
        {
            return EvalPair(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "list?")
        {
            return EvalListQ(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "null?")
        {
            return EvalNull(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "integer?")
        {
            return EvalIntger(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "real?")
        {
            return EvalReal(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "number?")
        {
            return EvalReal(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "string?")
        {
            return EvalString(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "boolean?")
        {
            return EvalBoolean(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "symbol?")
        {
            return EvalSymbol(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "not")
        {
            return EvalNot(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "and")
        {
            return EvalAnd(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "or")
        {
            return EvalOr(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "+")
        {
            return EvalPlus(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "-")
        {
            return EvalMinus(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "*")
        {
            return EvalMult(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "/")
        {
            return EvalDivis(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == ">")
        {
            return EvalBiggerThan(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == ">=")
        {
            return EvalBigger(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "<")
        {
            return EvalLessThan(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "<=")
        {
            return EvalLess(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "=")
        {
            return EvalValueEqual(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "string-append")
        {
            return EvalStringAppend(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "string>?")
        {
            return EvalStringBigger(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "string<?")
        {
            return EvalStringLess(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "string=?")
        {
            return EvalStringEqual(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "eqv?")
        {
            return EvalEqv(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "equal?")
        {
            return EvalEqual(args);
        }
        
        else if(op && (!op->left&&!op->right) && op->token.str == "begin")
        {
            if(!args||!args->right||args->right->token.type!=NIL)
            {
                throw runtime_error("ERROR (incorrect number of arguments) : begin\n");
            }

            return Evalulated(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "if")
        {
            return EvalStringLess(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "cond")
        {
            // if else is not at the least condition, then it is a normal symbol
            // when the condition is not nil then return the result 
            // return the least result, and all the results are need evaluated
            return EvalStringLess(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "clean-environment")
        {
            if(!args || args->token.type != NIL )
                throw runtime_error("ERROR (incorrect number of arguments) : clean-environment\n");
            symbolTable.clear();
            Node* head = new Node();
            head->token.str = "environment cleaned";
            head->token.type = SYMBOL;
            return head;
        }
        else 
        {
            errornode=op;
            throw runtime_error("ERROR (attempt to apply non-function) : ");
        }

        return exp;
    }

    Node* EvalCons(Node* args)
    {
        if(!args||!args->left||!args->right||!args->right->left||!args->right->right||args->right->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : cons\n");
        Node* first = Evalulated(args->left);
        Node* second = Evalulated(args->right->left);
        Node* head = new Node();
        head->left=first;
        head->right=second;
        return head;
    }

    Node* EvalCar(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : car\n");
        Node* root = Evalulated(args->left);
        if(!root||!root->left||!root->right)
        {   
            if(!errornode)
                errornode=root;
            throw runtime_error("ERROR (car with incorrect argument type) : ");
        }

        return root->left;
    }

    Node* EvalCdr(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : cdr\n");
        Node* root = Evalulated(args->left);
        if(!root||!root->left||!root->right)
        {   
            errornode=root;
            throw runtime_error("ERROR (cdr with incorrect argument type) : ");
        }

        return root->right;
    }

    Node* EvalEqv(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : eqv?\n");

        argument = args;
        Node* one = nullptr,* two=nullptr ;
        Node* root = new Node();
        one = Evalulated(argument->left);
        argument = argument->right;
        two = Evalulated(argument->left);

        if ( one && two && (one->left==nullptr && one->right == nullptr) && (two->left == nullptr&& two->right==nullptr) )
        {
            if (one->token.type == STRING && two->token.type == STRING)
            {
                if(one==two)
                {
                    root->token.type = TRUE;
                    root->token.str = "#t";
                }
                else
                {
                    root->token.type = NIL;
                    root->token.str = "nil";
                }
            }
            else if((one->token.type==INT||one->token.type==FLOAT) && (two->token.type==INT||two->token.type==FLOAT))
            {
                float o, t;
                o = stof(one->token.str);
                t = stof(two->token.str);
                if(o == t)
                {
                    root->token.type = TRUE;
                    root->token.str = "#t";
                }
                else
                {
                    root->token.type = NIL;
                    root->token.str = "nil";
                }
            }
            else
            {
                root->token.type = NIL;
                root->token.str = "nil";
            }
        }
        else
        {
            if (one == two)
            {
                root->token.type = TRUE;
                root->token.str = "#t";
            }
            else
            {
                root->token.type = NIL;
                root->token.str = "nil";
            }
        }

        return root ;
    }

    bool AllEqual(Node* firstarg, Node* secondarg)
    {
        if(firstarg && secondarg && firstarg->right == nullptr && firstarg->left==nullptr && secondarg->left==nullptr && secondarg->right==nullptr)
        {
            if( (firstarg->token.type==INT || firstarg->token.type == FLOAT) && (secondarg->token.type==INT || secondarg->token.type == FLOAT))
            {
                float firstnum, secondnum;
                firstnum = stof(firstarg->token.str);
                secondnum = stof(secondarg->token.str);
                if(firstnum == secondnum)
                    return true ;
                else 
                    return false;
            }
            else 
            {
                if(firstarg->token.str == secondarg->token.str)
                    return true ;
                else 
                    return false;
            }
        }
        else if(firstarg && secondarg && firstarg->right != nullptr && firstarg->left!=nullptr && secondarg->left!=nullptr && secondarg->right!=nullptr)
        {
            return (AllEqual(firstarg->right, secondarg->right) && AllEqual(firstarg->left, secondarg->left));
        }
        else
        {
            return false ;
        }

        return false ;
    }

    Node* EvalEqual(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : equal?\n");
        
        argument = args;
        Node* one = nullptr,* two=nullptr ;
        Node* root = new Node();
        one = Evalulated(argument->left);
        argument = argument->right;
        two = Evalulated(argument->left);
        if (AllEqual(one, two))
        {
            root->token.type=TRUE;
            root->token.str = "#t";
        }
        else
        {
            root->token.type = NIL;
            root->token.str = "nil";
        }

        return root;
    }

    Node* EvalStringEqual(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : string=?\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        string lastvalue = "";

        Node* num = Evalulated(argument->left);
        if (num->token.type == STRING) 
        {
            lastvalue = num->token.str;
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (string=? with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == STRING) 
            {
                if(lastvalue == num->token.str)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (string=? with incorrect argument type) : ");
            }

            lastvalue = num->token.str;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalStringLess(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : string<?\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        string lastvalue = "";

        Node* num = Evalulated(argument->left);
        if (num->token.type == STRING) 
        {
            lastvalue = num->token.str;
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (string<? with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == STRING) 
            {
                if(lastvalue<num->token.str)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (string<? with incorrect argument type) : ");
            }

            lastvalue = num->token.str;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalStringBigger(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : string>?\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        string lastvalue = "";

        Node* num = Evalulated(argument->left);
        if (num->token.type == STRING) 
        {
            lastvalue = num->token.str;
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (string>? with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == STRING) 
            {
                if(lastvalue>num->token.str)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (string>? with incorrect argument type) : ");
            }

            lastvalue = num->token.str;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalStringAppend(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : string-append\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "";
        root->token.type = STRING;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == STRING) 
            {
                num->token.str.erase(0,1);
                num->token.str.pop_back();

                root->token.str+=num->token.str;
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (string-append with incorrect argument type) : ");
            }

            argument = argument->right;
        }

        root->token.str = "\"" + root->token.str + "\"";

        return root;
    }

    Node* EvalValueEqual(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : =\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        float lastvalue = 0;
        float value = 0;

        Node* num = Evalulated(argument->left);
        if (num->token.type == INT||num->token.type == FLOAT) 
        {
            lastvalue = stof(num->token.str);
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (= with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT||num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                if(lastvalue == value)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (= with incorrect argument type) : ");
            }

            lastvalue = value;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalLess(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : <=\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        float lastvalue = 0;
        float value = 0;

        Node* num = Evalulated(argument->left);
        if (num->token.type == INT||num->token.type == FLOAT) 
        {
            lastvalue = stof(num->token.str);
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (<= with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT||num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                if(lastvalue<=value)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (<= with incorrect argument type) : ");
            }

            lastvalue = value;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalLessThan(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : <\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        float lastvalue = 0;
        float value = 0;

        Node* num = Evalulated(argument->left);
        if (num->token.type == INT||num->token.type == FLOAT) 
        {
            lastvalue = stof(num->token.str);
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (< with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT||num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                if(lastvalue<value)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (< with incorrect argument type) : ");
            }

            lastvalue = value;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalBigger(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : >=\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        float lastvalue = 0;
        float value = 0;

        Node* num = Evalulated(argument->left);
        if (num->token.type == INT||num->token.type == FLOAT) 
        {
            lastvalue = stof(num->token.str);
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (>= with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT||num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                if(lastvalue>=value)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (>= with incorrect argument type) : ");
            }

            lastvalue = value;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalBiggerThan(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : >\n");

        argument = args;
        Node* root = new Node();
        root->token.str = "#t";
        root->token.type = TRUE;
        float lastvalue = 0;
        float value = 0;

        Node* num = Evalulated(argument->left);
        if (num->token.type == INT||num->token.type == FLOAT) 
        {
            lastvalue = stof(num->token.str);
        } 
        else 
        {
            errornode = num;
            throw runtime_error("ERROR (> with incorrect argument type) : ");
        }

        argument=argument->right ;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT||num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                if(lastvalue>value)
                {
                    ;
                }
                else
                {
                    root->token.str = "nil";
                    root->token.type = NIL;
                }
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (> with incorrect argument type) : ");
            }

            lastvalue = value;
            argument = argument->right;
        }

        return root;
    }

    Node* EvalPlus(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : +\n");

        argument = args;
        Node* root = new Node();
        float ans = 0;
        bool isInt = true;
        float value = 0.0;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT) 
            {
                value = stoi(num->token.str);
            } 
            else if (num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                isInt = false;
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (+ with incorrect argument type) : ");
            }

            ans = ans+ value;
            argument = argument->right;
        }
        
        if (isInt) 
        {
            root->token.type = INT;
            root->token.str = to_string(ans);
        } 
        else 
        {
            root->token.type = FLOAT;
            root->token.str = to_string(ans);
        }

        return root;
    }

    Node* EvalMinus(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : -\n");

        argument = args;
        Node* root = new Node();
        float ans = 0;
        bool isInt = true;
        float value = 0.0;

        Node* begin = Evalulated(args->left);
        if (begin->token.type == INT) 
        {
            ans = stoi(begin->token.str);
        } 
        else if (begin->token.type == FLOAT) 
        {
            ans = stof(begin->token.str);
            isInt = false;
        } 
        else 
        {
            errornode = begin;
            throw runtime_error("ERROR (- with incorrect argument type) : ");
        }

        argument = argument->right;

        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT) 
            {
                value = stoi(num->token.str);
            } 
            else if (num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                isInt = false;
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (- with incorrect argument type) : ");
            }

            ans = ans- value;
            argument = argument->right;
        }
        
        if (isInt) 
        {
            root->token.type = INT;
            root->token.str = to_string(ans);
        } 
        else 
        {
            root->token.type = FLOAT;
            root->token.str = to_string(ans);
        }

        return root;
    }

    Node* EvalMult(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 2)
            throw runtime_error("ERROR (incorrect number of arguments) : *\n");

        argument = args;
        Node* root = new Node();
        float ans = 1;
        bool isInt = true;
        float value = 0.0;
        while(argument && argument->token.type != NIL)
        {
            Node* num = Evalulated(argument->left);
            if (num->token.type == INT) 
            {
                value = stoi(num->token.str);
            } 
            else if (num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                isInt = false;
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (* with incorrect argument type) : ");
            }

            ans = ans* value;
            argument = argument->right;
        }
        
        if (isInt) 
        {
            root->token.type = INT;
            root->token.str = to_string(ans);
        } 
        else 
        {
            root->token.type = FLOAT;
            root->token.str = to_string(ans);
        }

        return root;
    }

    Node* EvalDivis(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            arguments++;
            argument = argument->right;
        }

        if (arguments < 2) {
            throw runtime_error("ERROR (incorrect number of arguments) : /\n");
        }

        float ans = 0.0;
        bool isInt = true;

        Node* begin = Evalulated(args->left);
        if (begin->token.type == INT) 
        {
            ans = stoi(begin->token.str);
        } 
        else if (begin->token.type == FLOAT) 
        {
            ans = stof(begin->token.str);
            isInt = false;
        } 
        else 
        {
            errornode = begin;
            throw runtime_error("ERROR (/ with incorrect argument type) : ");
        }

        argument = args->right;
        while (argument && argument->token.type != NIL) {
            Node* num = Evalulated(argument->left);
            float value = 0.0;
            if (num->token.type == INT) 
            {
                value = stoi(num->token.str);
            } 
            else if (num->token.type == FLOAT) 
            {
                value = stof(num->token.str);
                isInt = false;
            } 
            else 
            {
                errornode = num;
                throw runtime_error("ERROR (/ with incorrect argument type) : ");
            }

            if (value == 0) {
                throw runtime_error("ERROR (division by zero) : /\n");
            }

            ans /= value;
            argument = argument->right;
        }


        Node* root = new Node();
        if (isInt) 
        {
            root->token.type = INT;
            root->token.str = to_string(ans);
        } 
        else 
        {
            root->token.type = FLOAT;
            root->token.str = to_string(ans);
        }

        return root;
    }


    Node* EvalOr(Node* args)
    {
        int arguments=0;
        Node* argument = args;
        Node* ans = nullptr;
        Node* finalans = nullptr;
        bool result = false;
        while(argument&&argument->token.type!=NIL)
        {
            ans = Evalulated(argument->left);
            if(ans&&!ans->left&&!ans->right&&ans->token.type!=NIL)
            {
                finalans = ans;
                result = true;
            }
            
            ++arguments;
            argument = argument->right;
        }

        if(arguments<2)
            throw runtime_error("ERROR (incorrect number of arguments) : or\n");

        Node* root = new Node();
        if(result)
        {
            return finalans;
        }
        else
        {
            root->token.str="nil";
            root->token.type=NIL;
        }

        return root;
    }

    Node* EvalAnd(Node* args)
    {
        // check if nil atom in the arguments
        int arguments=0;
        Node* argument = args;
        Node* ans = nullptr;
        Node* finalans = nullptr;
        bool result = true;
        while(argument&&argument->token.type!=NIL)
        {
            ans = Evalulated(argument->left);
            if(ans&&!ans->left&&!ans->right&&ans->token.type==NIL)
            {
                result = false;
            }
            
            ++arguments;
            argument = argument->right;
        }

        if(arguments<2)
            throw runtime_error("ERROR (incorrect number of arguments) : and\n");

        Node* root = new Node();
        if(result)
        {
            return ans;
        }
        else
        {
            root->token.str="nil";
            root->token.type=NIL;
        }

        return root;

    }

    Node* EvalNot(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : not\n");

        Node* head = Evalulated(args->left);
        Node* root = new Node();
        if((!head->left&&!head->right)&&head->token.type==NIL)
        {
            root->token.type=TRUE;
            root->token.str="#t";
        }
        else
        {
            root->token.type=NIL;
            root->token.str="nil";
        }

        return root;
    }

    Node* EvalSymbol(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : symbol?\n");
        
        Node* head = Evalulated(args->left);
        Node* root = new Node();
        if((!head->left&&!head->right)&&head->token.type==SYMBOL)
        {
            root->token.type=TRUE;
            root->token.str="#t";
        }
        else
        {
            root->token.type=NIL;
            root->token.str="nil";
        }

        return root;
    }

    Node* EvalString(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : string?\n");
        
        Node* head = Evalulated(args->left);
        Node* root = new Node();
        if((!head->left&&!head->right)&&head->token.type==STRING)
        {
            root->token.type=TRUE;
            root->token.str="#t";
        }
        else
        {
            root->token.type=NIL;
            root->token.str="nil";
        }

        return root;
    }

    Node* EvalBoolean(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : boolean?\n");
        
        Node* head = Evalulated(args->left);
        Node* root = new Node();
        if((!head->left&&!head->right)&&head->token.type==NIL||head->token.type==TRUE)
        {
            root->token.type=TRUE;
            root->token.str="#t";
        }
        else
        {
            root->token.type=NIL;
            root->token.str="nil";
        }

        return root;
    }

    Node* EvalReal(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : real?\n");
        Node* head = Evalulated(args->left);
        Node* root = new Node();
        if((!head->left&&!head->right)&&head->token.type==INT||head->token.type==FLOAT)
        {
            root->token.type=TRUE;
            root->token.str="#t";
        }
        else
        {
            root->token.type=NIL;
            root->token.str="nil";
        }

        return root;
    }

    Node* EvalIntger(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : integer?\n");
        Node* head = Evalulated(args->left);
        Node* root = new Node();
        if((!head->left&&!head->right)&&head->token.type==INT)
        {
            root->token.type=TRUE;
            root->token.str="#t";
        }
        else
        {
            root->token.type=NIL;
            root->token.str="nil";
        }

        return root;
    }


    Node* EvalPair(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : pair?\n");
        
        Node* root = Evalulated(args->left);
        Node* head = new Node();
        if(root&&root->right&&root->left)
        {
            head->token.type=TRUE;
            head->token.str="#t";
        }
        else
        {
            head->token.type=NIL;
            head->token.str="nil";
        }

        return head;
    }

    Node* EvalNull(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
            throw runtime_error("ERROR (incorrect number of arguments) : null?\n");

        Node* root = Evalulated(args->left);
        Node* head = new Node();
        if(root&& root->left==nullptr && root->right == nullptr && root->token.type == NIL)
        {
            head->token.type=TRUE;
            head->token.str="#t";
        }
        else
        {
            head->token.type=NIL;
            head->token.str="nil";
        }

        return head;
    }

    Node* EvalListQ(Node* args)
    {
        Node* temp = args;
        Node* root = new Node();
        while(temp->token.type!=NIL||temp)
        {
            temp->left = Evalulated(temp->left);
            temp=temp->right;
        }

        if(!temp)
        {
            root->token.str="nil";
            root->token.type=NIL;
        }
        else
        {
            root->token.str="#t";
            root->token.type=TRUE;
        }

        return root;
    }

    Node* EvalList(Node* args)
    {
        Node* temp = args;
        while(temp)
        {
            temp->left = Evalulated(temp->left);
            temp=temp->right;
        }

        return args;
    }

    Node* EvalDefine(Node* args) 
    {
        if(!args || !args->left || !args->right || !args->right->right || args->right->right->token.type!=NIL) {
            throw runtime_error("ERROR(DEFINE format) : ");
        }

        Node* symNode = args->left;
        Node* valNode = args->right->left;

        if(symNode->token.type != SYMBOL) 
        { // and token.str != primetivefunction
            throw runtime_error("ERROR(DEFINE format) : ");
        }

        Node* value = Evalulated(valNode);
        symbolTable[symNode->token.str] = value;

        Node* head = new Node();
        head->token = symNode->token;
        head->token.str += " defined";
        return head;
    }

    Node* EvalAtom(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
        {
            throw runtime_error("ERROR (incorrect number of arguments) : atom?\n");
        }
        Node* head = new Node();
        if(args&&!args->left&&!args->right)
        {
            head->token.str = "#t";
            head->token.type=TRUE;
        }
        else
        {
            head->token.str = "nil";
            head->token.type=NIL;
        }
        return head;
    }

    Node* GetValue(Node* src) 
    {
        string name = src->token.str;
        Node* head = new Node();
        if(symbolTable.count(name)) {
            return symbolTable[name];
        }
        else if(primitiveNames.count(name))
        {
            src->token.type=Func;
            return src;
        }

        
        throw runtime_error("ERROR(unbound symbol) : " + name + "\n");
    }

    bool IsDotPair(Node* node) 
    {
        if(!node || !node->left || !node->right)
        {
            errornode = node;
            throw runtime_error("ERROR (non-list) : ");
        }

        Node* cur = node;
        while(cur->right) {
            if(!cur->right->left && !cur->right->right) {
                return cur->right->token.type != NIL;
            }

            cur = cur->right;
        }

        errornode = node ;
        throw runtime_error("ERROR (non-list) : ");
    }

};


int main()
{
    string tmp;
    getline(cin, tmp);
    cout << "Welcome to OurScheme!\n\n";
    bool newline = true;
    Evaluate evaluate;
    evaluate.InitPrimitives();

    while(true)
    {
        if(newline)
        {
            gLine = 1;
            gColumn = 0;
        }

        cout << "> ";
        Syntax syntax_anaylize;
        syntax_anaylize.init();
        TreeBuilder tree ;
        Node *root = NULL;
        if(syntax_anaylize.Is_SEXP()) {
            tree.init(syntax_anaylize.tokens);
            int index = 0 ;
            root = tree.BuildTree(index);
            if(tree.check_exit(root))
            {
                cout << endl;
                break;
            }

            try
            {
                root = evaluate.Evalulated(root);
                PrintSExp(root, 0, true);
                gLine=1;
                gColumn=0;
                newline = true ;
                COUNTLINE(newline);
            }
            catch(runtime_error& e)
            {

                string errMsg = e.what();
                cout << errMsg ;
                if(evaluate.errornode!= nullptr)
                    PrintSExp(evaluate.errornode, 0, true);
                evaluate.errornode=nullptr;
            }
        }
        else 
        {
            syntax_anaylize.Error_message();
            if(syntax_anaylize.Is_END())
                break;
            
            if(!syntax_anaylize.Is_Error())
            {
                while(cin.peek() != '\n' && cin.peek() != EOF)
                    GetChar();

                GetChar();
            }
            
            peeked=false;
            newline=true;
        }
        
        cout << endl;
    }

    cout << "Thanks for using OurScheme!";
}