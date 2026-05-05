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
        while(cin.peek() != '\n' && cin.peek() != EOF )
        {
            GetChar();
        }
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
                cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << tokens[tokens.size()-1].line << " Column " ;
                cout << tokens[tokens.size()-1].column + tokens[tokens.size()-1].str.length() << endl;
            }
            else if(tokens[tokens.size()-1].type == END)
            {
                cout << "ERROR (no more input) : END-OF-FILE encountered" << endl ;
            }
            else if(syntax_error == ATOM)
            {
                cout << "ERROR (unexpected token) : atom or '(' expected when token at Line ";
                cout << tokens[tokens.size()-1].line ;
                cout << " Column " ;
                cout << tokens[tokens.size()-1].column;
                cout << " is >>";
                cout << tokens[tokens.size()-1].str;
                cout << "<<" << endl;
            } 
            else if(syntax_error == LEFT)
            {
                cout << "ERROR (unexpected token) : ')' expected when token at Line ";
                cout << tokens[tokens.size()-1].line ;
                cout << " Column " ;
                cout << tokens[tokens.size()-1].column;
                cout << " is >>";
                cout << tokens[tokens.size()-1].str;
                cout << "<<" << endl ;
            }
        }

        void Init()
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
        while(cin.peek()!='\n'&&cin.peek()!=EOF)
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

        void Init(vector<Token> list) 
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

/**
 *                             _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     佛祖保佑        永无BUG
*/


class Evaluate {
private:
    struct Function {
        vector<string> parameters;  // 參數列表
        vector<Node*> body;         // lambda 的內容
        map<string, Node*> env;
    };

    Function funclambda;
    map<string, Node*> symbolTable;
    map<string, Function> functionTable;
    set<string> primitiveNames;
    bool onelevel = true;

public:
    Node* errornode;
    void InitLevel()
    {
        onelevel = true;
    }

    void InitPrimitives() 
    {
        // 定義所有內建函數名稱
        errornode = nullptr;
        vector<string> primitives = 
        {
            "cons", "list", "quote", "define", "car", "cdr",
            "atom?", "pair?", "list?", "null?", "integer?", "real?", "number?",
            "string?", "boolean?", "symbol?", "+", "-", "*", "/",
            "not", "and", "or", ">", ">=", "<", "<=", "=",
            "string-append", "string>?", "string<?", "string=?",
            "eqv?", "equal?", "begin", "if", "cond", "clean-environment", "exit", "lambda", "let"
        };

        // 將內建函數名稱記錄到 primitiveNames
        for (auto& name : primitives) 
        {
            primitiveNames.insert(name);
        }
    }

    Node* Evalulated(Node* exp) 
    {
        if(!exp) return nullptr;
        bool level = onelevel;
        if(onelevel)
            onelevel = false;
        if(!exp->left && !exp->right) {  // atom
            if(exp->token.type == SYMBOL) {
                return GetValue(exp);
            }

            return exp;
        }

        if(IsDotPair(exp))
        {
            errornode = exp;
            throw runtime_error("ERROR (non-list) : ") ;
        }

        Node* op = nullptr;
        if( exp->left && (exp->left->left || exp->left->right) || ( primitiveNames.count(exp->left->token.str)==0 && functionTable.count(exp->left->token.str)==0 ))
            op = Evalulated(exp->left);
        else
            op = exp->left;
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
            if(!level)
                throw runtime_error("ERROR (level of DEFINE)\n");
            return EvalDefine(args, exp);
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
            return EvalBegin(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "if")
        {
            return EvalIf(args, exp);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "cond")
        {
            return EvalCond(args, exp);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "clean-environment")
        {
            if(!level)
                throw runtime_error("ERROR (level of CLEAN-ENVIRONMENT)\n");
            if(!args || args->token.type != NIL )
                throw runtime_error("ERROR (incorrect number of arguments) : clean-environment\n");
            symbolTable.clear();
            functionTable.clear();
            Node* head = new Node();
            head->token.str = "environment cleaned";
            head->token.type = SYMBOL;
            return head;
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "exit")
        {
            if(!level)
                throw runtime_error("ERROR (level of EXIT)\n");
            if(!args || args->token.type != NIL )
                throw runtime_error("ERROR (incorrect number of arguments) : exit\n");
            
            return exp;
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "lambda" && op->token.type == SYMBOL)
        {
            return EvalLambda(args, exp);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "lambda" && op->token.type == Func)
        {
            return EvalLambdaFunc(args);
        }
        else if(op && (!op->left&&!op->right) && op->token.str == "let")
        {
            return EvalLet(args, exp);
        }
        else if(op && (!op->left&&!op->right) && functionTable.count(op->token.str)!=0)
        {
            return EvalOurFunc(args, exp, op);
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

    Node* EvalLet(Node* args, Node* head)
    {
        // 檢查區域變數的綁定格式
        Node* local = args->left;
        while(local->token.type != NIL)
        {
            if(!local->left || !local->left->left || local->left->left->token.type!=SYMBOL|| !local->left->right ||!local->left->right->left||!local->left->right->right||
                local->left->right->right->token.type!=NIL)
            {
                errornode = head;
                throw runtime_error("ERROR (LET format) : ");
            }

            local = local->right;
        }

        // 檢查body 
        int arguments = 0;
        Node* argument = args->right;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if(arguments==0)
        {
            errornode = head;
            throw runtime_error("ERROR (LET format) : ");
        }

        // 把區域變數做綁定
        map<string, Node*>state = symbolTable;
        map<string, Node*>thenew = symbolTable;
        local = args->left;
        while(local->token.type != NIL)
        {
            thenew[local->left->left->token.str] = Evalulated(local->left->right->left);
            local = local->right;
        }

        symbolTable = thenew;


        // 透過所有的body計算
        Node* root = nullptr;
        argument = args->right;
        while (argument && argument->token.type != NIL) 
        {   
            root = Evalulated(argument->left);
            argument = argument->right;
        }

        // 還原
        symbolTable = state;
        return root;
    }

    Node* EvalLambdaFunc(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {   // 考慮過參數為0的情況
            ++arguments;
            argument = argument->right;
        }

        if ( funclambda.parameters.size() != arguments )
            throw runtime_error("ERROR (incorrect number of arguments) : lambda\n");
        

        argument = args;
        arguments = 0 ;


        // 綁定參數
        map<string, Node*>thenew = symbolTable;
        map<string, Node*>orginal = symbolTable;

        while (argument && argument->token.type != NIL) 
        {
            thenew[funclambda.parameters[arguments]] = Evalulated(argument->left);
            arguments++;
            argument = argument->right;
        }

        symbolTable = thenew;

        // 運算要回傳最後一個
        Node* root = nullptr;
        for(int n=0 ; n<=funclambda.body.size()-1 ; n++)
        {
            root = Evalulated(funclambda.body[n]);
        }

        symbolTable = orginal;
        return root;
    }

    Node* EvalLambda(Node* args, Node*head)
    {
        if(!args||!args->left||!args->right)
        {
            errornode = head;
            throw runtime_error("ERROR (LAMBDA format) : ");
        }


        Function f;
        Node* num = args->left;
        // 檢查lambda參數有多少個，再把參數放入function 內
        while(num&&num->token.type!=NIL)
        {
            if( !num->left || num->left->token.type!=SYMBOL)
            {
                errornode = head;
                throw runtime_error("ERROR (LAMBDA format) : ");
            }
            else
            {
                f.parameters.push_back(num->left->token.str);
            }

            num = num->right;
        }

        // 檢查body是否有一個以上
        Node* body = args->right;
        int number = 0;
        while(body&&body->token.type!=NIL)
        {
            if(!body->left)
            {
                errornode = head;
                throw runtime_error("ERROR (LAMBDA format) : ");
            }

            ++number;
            body = body->right;
        }

        if(number==0)
        {
            errornode = head;
            throw runtime_error("ERROR (LAMBDA format) : ");
        }


        // 再來把所有的body都放進funclambda內
        body = args->right;
        while(body && body->token.type!=NIL)
        {
            f.body.push_back(body->left);
            body = body->right;
        }

        funclambda = f;
        Node* lam = new Node();
        lam->token.str = "lambda";
        lam->token.type = Func;
        lam->left = nullptr;
        lam->right = nullptr;
        return lam;
    }

    Node* EvalIf(Node* args, Node* head)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments != 2 && arguments != 3)
            throw runtime_error("ERROR (incorrect number of arguments) : if\n");
        
        Node* value = Evalulated(args->left);
        if(!value->left && !value->right && value->token.type == NIL)
        {
            if(arguments == 2)
            {
                errornode = head ;
                throw runtime_error("ERROR (no return value) : ");
            }
            
            Node* fals = Evalulated(args->right->right->left);
            return fals;
        }
        
        Node* tru = Evalulated(args->right->left);
        return tru;
    }

    void EvalConditions(Node* args, Node* head)
    {
        Node* argument = args ;
        while(argument && argument->token.type != NIL)
        {
            Node* condition = argument->left;
            int num = 0 ;
            while(condition && condition->token.type!=NIL)
            {
                num++;
                if(!condition->left && !condition->right && condition->token.type!=NIL)
                {
                    errornode = head;
                    throw runtime_error("ERROR (COND format) : ");
                }
                
                condition = condition->right;
            }

            if(num<2)
            {
                errornode = head;
                throw runtime_error("ERROR (COND format) : ");
            }

            argument = argument->right;
        }
    }

    Node* FindResult(Node* args, Node* head)
    {
        if(args->right->token.type==NIL)
        {
            errornode = head;
            throw runtime_error("ERROR (COND format) : ");
        }

        Node* argument = args->right ;
        Node* result = nullptr;
        while(argument && argument->token.type != NIL)
        {
            result = Evalulated(argument->left);
            argument = argument->right;
        }

        return result;
    }

    Node* EvalCond(Node* args, Node* head)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 1)
        {
            errornode = head;
            throw runtime_error("ERROR (COND format) : ");
        }
        
        
        argument = args;
        // check format:  (cond (#t)  )     or    (cond #t) 沒有可以回傳的東西，這種格式會錯
        EvalConditions(argument, head);
        Node* root = nullptr;
        while (argument && argument->token.type != NIL)
        {
            if(argument->right->token.type==NIL && argument->left->left->token.str=="else")
                return FindResult(argument->left, head);
            
            Node* condition = Evalulated(argument->left->left);
            if(condition->token.type == NIL)
            {
                ;
            }
            else 
            {
                return FindResult(argument->left, head);
            }

            argument=argument->right;
        }

        if(root == nullptr)
        {
            errornode = head;
            throw runtime_error("ERROR (no return value) : ");
        }
        
        return root;
    }


    
    Node* EvalBegin(Node* args)
    {
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments < 1)
            throw runtime_error("ERROR (incorrect number of arguments) : begin\n");

        Node* argumnet = args ;
        Node* result = nullptr;
        while(argumnet&&argumnet->token.type != NIL)
        {
            result = Evalulated(argumnet->left);
            argumnet = argumnet->right;
        }
        
        return result;
    }

    Node* EvalEqv(Node* args)
    {
        // 應該是這邊或是equal?有問題
        int arguments = 0;
        Node* argument = args;
        while (argument && argument->token.type != NIL) 
        {
            ++arguments;
            argument = argument->right;
        }

        if (arguments != 2)
            throw runtime_error("ERROR (incorrect number of arguments) : eqv?\n");

        argument = args;
        Node* one = nullptr,* two=nullptr ;
        Node* root = new Node();
        one = Evalulated(argument->left);
        argument = argument->right;
        two = Evalulated(argument->left);

        if((!one->left&&!one->right) && (!two->left&&!two->right) && one->token.type!=STRING && two->token.type!=STRING)
        {
            if((one->token.type==INT || one->token.type==FLOAT)&&(two->token.type==INT || two->token.type==FLOAT))
            {
                int f1, f2;
                f1 = stof(one->token.str);
                f2 = stof(two->token.str);
                if(f1==f2)
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

            if(one->token.str == two->token.str)
            {
                root->token.type=TRUE;
                root->token.str="#t";
            }
            else
            {
                root->token.type=NIL;
                root->token.str="nil";
            }
        }
        else
        {
            if(one == two)
            {
                root->token.type=TRUE;
                root->token.str="#t";
            }
            else
            {
                root->token.type=NIL;
                root->token.str="nil";
            }
        }

        return root;
    }

    bool AllEqual(Node* firstarg, Node* secondarg)
    {
        if(firstarg==nullptr&&secondarg==nullptr)
            return true;
        if (firstarg==nullptr || secondarg==nullptr) 
            return false;
        else if((!firstarg->left&&!firstarg->right) && (!secondarg->left&&!secondarg->right))
        {
            if(firstarg->token.str==secondarg->token.str)
                return true;
            else
                return false;
        }
        else if((firstarg->left&&firstarg->right) && (secondarg->left&&secondarg->right))
        {
            return (AllEqual(firstarg->left, secondarg->left) && AllEqual(firstarg->right, secondarg->right));
        }
        else
        {
            return false ;
        }

        return false;
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

        if (arguments != 2)
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
                if(finalans==nullptr)
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
            ++arguments;
            argument = argument->right;
        }

        if(arguments<2)
            throw runtime_error("ERROR (incorrect number of arguments) : and\n");

        argument = args;
        while (argument&&argument->token.type!=NIL)
        {
            ans = Evalulated(argument->left);
            if(ans->token.type == NIL)
            {
                result = false;
                break;
            }

            argument = argument->right;
        }
        
        
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
        if((!head->left&&!head->right)&&(head->token.type==NIL||head->token.type==TRUE))
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
        if((!head->left&&!head->right)&&(head->token.type==INT||head->token.type==FLOAT))
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
        if(root && root->left==nullptr && root->right == nullptr && root->token.type == NIL)
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
        Node* temp = Evalulated(args->left);
        Node* root = new Node();
        if(IsDotPair(temp))
        {
            root->token.type = NIL;
            root->token.str = "nil";
        }
        else
        {
            root->token.type = TRUE;
            root->token.str = "#t";
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

    Node* EvalOurFunc(Node* args, Node* head, Node* operation)
    {
        // 檢查參數數量是否一致
        Function myFunc = functionTable[operation->token.str];
        int arguments = 0;
        Node* param = args;
        while(param&&param->token.type!=NIL)
        {
            ++arguments;
            param = param->right;
        }

        if(arguments!=myFunc.parameters.size())
        {
            throw runtime_error("ERROR (incorrect number of arguments) : "+ operation->token.str + "\n");
        }

        // 把參數綁定
        map<string, Node*> state = symbolTable;
        map<string, Node*> thenew = symbolTable;
        param = args;
        for(int n = 0 ; n <= myFunc.parameters.size()-1 ;n++)
        {
            thenew[myFunc.parameters[n]] = Evalulated(param->left);
            param = param->right;
        }

        symbolTable = thenew;

        Node* root = nullptr;
        for(int n = 0 ; n <=myFunc.body.size()-1; n++)
        {
            root = Evalulated(myFunc.body[n]);
        }

        // 還原
        symbolTable = state;
        return root;
    }

    Node* EvalDefineFunc(Node* args, Node* head)
    {
        // 檢查名字和參數是不是symbol不是就throw error
        Node* param = args->left;
        while(param&&param->token.type!=NIL)
        {
            if(param->left&& param->left->token.type==SYMBOL)
            {
                ;
            }
            else
            {
                errornode = head;
                throw runtime_error("ERROR (DEFINE format) : ");
            }

            param = param->right;
        }

        // 檢查body大於一個
        Node* body = args->right;
        if(!body || body->token.type == NIL)
        {
            errornode = head;
            throw runtime_error("ERROR (DEFINE format) : ");
        }

        param = args->left;
        string name = param->left->token.str;
        param = param->right;

        // 新增參數到function裡面
        Function f;
        while(param&& param->token.type!=NIL)
        {
            f.parameters.push_back(param->left->token.str);
            param=param->right;
        }

        // 新增body到function
        body = args->right;
        while(body&&body->token.type!=NIL)
        {
            f.body.push_back(body->left);
            body= body->right;
        }


        functionTable[name] = f;

        Node* root = new Node();
        root->token.str = name + " defined";
        root->token.type = SYMBOL;
        return root;
    }

    Node* EvalDefine(Node* args, Node* head) 
    {
        if(!args || !args->left || !args->right || !args->right->right ) 
        { 
            head->left->token.type = SYMBOL;
            errornode = head;
            throw runtime_error("ERROR (DEFINE format) : ");
        }

        Node* symNode = args->left;
        Node* valNode = args->right->left;

        if(symNode->left&&symNode->right)
            return EvalDefineFunc(args, head);
        
        if(args->right->right->token.type!=NIL)
        { 
            head->left->token.type = SYMBOL;
            errornode = head;
            throw runtime_error("ERROR (DEFINE format) : ");
        }


        if(symNode->token.type != SYMBOL) 
        {
            head->left->token.type = SYMBOL;
            errornode = head ;
            throw runtime_error("ERROR (DEFINE format) : ");
        }
        else if(primitiveNames.count(symNode->token.str)!=0)
        {
            head->left->token.type = SYMBOL;
            errornode = head ;
            throw runtime_error("ERROR (DEFINE format) : ");
        }

        Node* value = Evalulated(valNode);
        symbolTable[symNode->token.str] = value; // 把值放進symboltable

        Node* defined = new Node();
        defined->token = symNode->token;
        defined->token.str += " defined";
        return defined;
    }

    Node* EvalAtom(Node* args)
    {
        if(!args||!args->left||!args->right||args->right->token.type!=NIL)
        {
            throw runtime_error("ERROR (incorrect number of arguments) : atom?\n");
        }

        Node* head = new Node();
        Node* root = Evalulated(args->left);
        if(root&&!root->left&&!root->right)
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
        else if(primitiveNames.count(name) || functionTable.count(name) )
        {
            src->token.type=Func;
            return src;
        }

        throw runtime_error("ERROR (unbound symbol) : " + name + "\n");
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

/*                   初音
 *
 * _______________#########_______________________ 
 * ______________############_____________________ 
 * ______________#############____________________ 
 * _____________##__###########___________________ 
 * ____________###__######_#####__________________ 
 * ____________###_#######___####_________________ 
 * ___________###__##########_####________________ 
 * __________####__###########_####_______________ 
 * ________#####___###########__#####_____________ 
 * _______######___###_########___#####___________ 
 * _______#####___###___########___######_________ 
 * ______######___###__###########___######_______ 
 * _____######___####_##############__######______ 
 * ____#######__#####################_#######_____ 
 * ____#######__##############################____ 
 * ___#######__######_#################_#######___ 
 * ___#######__######_######_#########___######___ 
 * ___#######____##__######___######_____######___ 
 * ___#######________######____#####_____#####____ 
 * ____######________#####_____#####_____####_____ 
 * _____#####________####______#####_____###______ 
 * ______#####______;###________###______#________ 
 * ________##_______####________####______________ 
 */

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
        syntax_anaylize.Init();
        TreeBuilder tree ;
        Node *root = NULL;
        if(syntax_anaylize.Is_SEXP()) {
            tree.Init(syntax_anaylize.tokens);
            int index = 0 ;
            root = tree.BuildTree(index);
            if(tree.check_exit(root))
            {
                cout << endl;
                break;
            }

            try
            {            
                evaluate.InitLevel();
                root = evaluate.Evalulated(root);
                PrintSExp(root, 0, true);
            }
            catch(runtime_error& e)
            {
                string errMsg = e.what();
                cout << errMsg ;
                if(evaluate.errornode!= nullptr)
                    PrintSExp(evaluate.errornode, 0, true);
                evaluate.errornode=nullptr;
            }

            gLine=1;
            gColumn=0;
            newline = true ;
            COUNTLINE(newline);
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