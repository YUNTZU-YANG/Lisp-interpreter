#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <iomanip>
#include <cctype>

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
bool gExit = false;          // 碰到 (exit) 結束
bool gErrorOccurred = false; // 有error用
stack<Token> tokenStack;
Token peekToken;
bool peeked = false ;

char GetChar()
{
    char c = cin.get();
    if (c == '\n')
    {
        gLine++;
        gColumn = 1;
    }

    else
        gColumn++;

    return c;
}

// 退回一個字元
void UngetChar(char c)
{
    cin.putback(c);
    if (c == '\n')
    {
        gLine--;
        gColumn = 1;
    }
    else
    {
        gColumn--;
        if (gColumn < 1)
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
    if ( peeked )
    {
        peeked = false ;
        return peekToken ;
    }

    // 跳過空白
    while (cin.peek() != EOF && IsWhiteSpace(cin.peek()))
        GetChar();

    // 檢查是否已經到檔案結尾
    if (cin.peek() == EOF)
    {
        token.type = END;
        return token;
    }

    char c = cin.peek();

    // 處理註解
    if (c == ';')
    {
        while (cin.peek() != '\n' && cin.peek() != EOF)
            GetChar();
        return GetToken(); // 註解讀完繼續讀下一個token
    }

    // 開始讀token
    c = GetChar();

    token.line = gLine - 1;
    if (token.line == 0)
        token.line++;

    token.column = gColumn - 1;

    if (c == '(')
        token = {"(", LBRACKET, token.line, token.column};
    else if (c == ')')
        token = {")", RBRACKET, token.line, token.column};
    else if (c == '.')
    {

        // 判斷是否是浮點數的小數點
        if (isdigit(cin.peek()))
        {
            token.str = ".";
            token.type = FLOAT;
            while (isdigit(cin.peek()))
                token.str += GetChar();
        }
        else if(IsSeparator(cin.peek()))
            token = {".", DOT, gLine, gColumn - 1};
        else
        {
            token.str+=c;
            while(!IsSeparator(cin.peek()))
                token.str+=GetChar();

            token.type = SYMBOL;
        }
        
    }
    else if (c == '\'')
        token = {"\'", QUOTE, gLine, gColumn};
    else if (c == '"')
    {

        // 處理字串
        token.str = "\"";
        token.type = STRING;
        while (true)
        {
            int tempL = gLine;
            int tempC = gColumn;
            char ch = GetChar();

            if (ch == '\n' || ch == EOF )
            {
                // ErrorNoClosingQuote(tempL - 1, tempC);
                token.type = ERROR;
                return token;
            }

            if (ch == '"')
            {
                token.str += "\"";
                break;
            }

            // 處理反斜線
            if (ch == '\\')
            {
                char esc = GetChar();

                if (esc == 'n')
                    token.str += "\\n";
                else if (esc == 't')
                    token.str += "\\t";
                else if (esc == '"')
                    token.str += "\\\"";
                else if (esc == '\\')
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
        while (!IsSeparator(cin.peek()) && cin.peek() != EOF)
            token.str += GetChar();

        // classify
        if (token.str == "nil" || token.str == "#f")
            token.type = NIL;
        else if (token.str == "t" || token.str == "#t")
            token.type = TRUE;
        else if (regex_match(token.str, regex("^[-+]?[0-9]+$")))
            token.type = INT;
        else if (regex_match(token.str, regex("^[-+]?([0-9]*\\.[0-9]+|[0-9]+\\.)$")))
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
            unexpect_atom,
            unexpect_left
        };

        Error syntax_error ;


    public:
        vector<Token> tokens ;
        bool Is_ATOM( Token token )
        {
            if ( token.type == NIL || token.type == TRUE || token.type == STRING || token.type == INT || 
                 token.type == FLOAT || token.type == SYMBOL )
                return true ;
            return false ;
        }

        bool Is_SEXP() 
        {
            Token token = GetToken();
            tokens.push_back( token ) ;
            if ( Is_ATOM( token ) )
                return true ;
            else if ( token.type == LBRACKET ) 
            {
                int sexp = 0 ;
                while(true)
                {
                    peekToken=GetToken();
                    peeked = true ;
                    if (peekToken.type == RBRACKET)
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
                    else if (peekToken.type==DOT)
                    {
                        
                        tokens.push_back(peekToken);
                        if(sexp==0)
                        {
                            Set_Error(unexpect_atom);
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
                                Set_Error(unexpect_left);
                                return false ;
                            }
                        }
                        else
                        {
                            Set_Error(unexpect_atom);
                            return false ;
                        }

                        break;
                    }
                    else
                    {
                        if(Is_SEXP())
                            sexp++;
                        else 
                        {
                            Set_Error(unexpect_atom);
                            return false ;
                        }
                    }
                }

                return true ;
            } 
            else if ( token.type == QUOTE ) 
            {
                if ( Is_SEXP() )
                {
                    return true ;
                }
                else
                {
                    Set_Error( unexpect_atom );
                    return false;
                }
            }
            else 
            {
                Set_Error( unexpect_atom ) ;
                return false ;
            }

            Set_Error(unexpect_atom);
            return false ;
        } 

        void Error_message() 
        {
            if ( tokens[tokens.size()-1].type == ERROR )
            {
                cout << "ERROR (no closing quote) : END-OF-LINE encountered at Line " << tokens[tokens.size()-1].line << " Column " ;
                cout << tokens[tokens.size()-1].column + tokens[tokens.size()-1].str.length() << endl;
            }
            else if ( tokens[tokens.size()-1].type == END )
            {
                cout << "ERROR (no more input) : END-OF-FILE encountered" << endl ;
            }
            else if ( syntax_error == unexpect_atom )
            {
                cout << "ERROR (unexpected token) : atom or '(' expected when token at Line ";
                cout << tokens[tokens.size()-1].line ;
                cout << " Column " ;
                cout << tokens[tokens.size()-1].column;
                cout << " is >>";
                cout << tokens[tokens.size()-1].str;
                cout << "<<" << endl;
            } 
            else if ( syntax_error == unexpect_left )
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

        void init()
        {
            syntax_error = without_error ;
        }

        void Set_Error( Error error ) 
        {
            if ( syntax_error == without_error )
                syntax_error = error ;
        }

        bool Is_END() 
        {
            if ( tokens[tokens.size()-1].type == END )
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

class TreeBuilder {
    private:
        vector<Token> tokens;

    public:
        Node* BuildTree(int& index) 
        {
            Token token = tokens[index];
            Node* n = new Node();

            if ( token.type == LBRACKET ) 
            {
                index ++ ;
                n->left = BuildTree( index ) ;
                n->right = new Node() ;
                // BuildTree的index 只會計算到結束的token 所以index 要加一
                // 判斷下一個token

                index ++ ;
                Node* current = n->right ;
                Node* newnode = NULL ;
                while( tokens[index].type != RBRACKET && tokens[index].type != DOT ) 
                {
                    newnode = BuildTree( index );
                    current->left = newnode; 
                    current->right = new Node();
                    index ++;
                    if ( tokens[index].type == DOT )
                    {
                        index++ ;
                        current->right = BuildTree( index );
                        index++ ;   // 跳到右括號
                        return n;
                    }
                    else if ( tokens[index].type == RBRACKET )
                    {
                        current->right->token.type = NIL;
                        current->right->token.str = "nil";
                        return n; 
                    }

                    current = current->right;
                }


                if ( tokens[index].type == DOT )
                {
                    index++ ;
                    n->right = BuildTree( index );
                    index++ ;   // 跳到右括號
                }
                else if ( tokens[index].type == RBRACKET ) 
                {
                    current->token.type = NIL;
                    current->token.str = "nil" ;
                }
            }
            else if ( token.type == QUOTE )
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

        void init( vector<Token> list ) 
        {
            tokens = list ;
        }

        bool check_exit( Node* node )
        {
            if ( !node )
                return false ;
            if ( !node->right && !node->left )
                return false ;
            if ( node->left->token.str == "exit" && node->right->token.type == NIL)
                return true ;
            return false ;
        }
};

// 印SExp
void PrintSExp(Node *node, int indent, bool space)
{
    if (!node)
        return;

    // 是 atom
    if (!node->left && !node->right)
    {
        if (space)
            cout << string(indent, ' ');
        if (node->token.type == FLOAT)
            cout << fixed << setprecision(3) << stod(node->token.str) << endl;
        else if (node->token.type == INT)
            cout << stoi(node->token.str) << endl;
        else if (node->token.type == TRUE)
            cout << "#t" << endl;
        else if (node->token.type == NIL)
            cout << "nil" << endl;
        else if (node->token.type == QUOTE)
            cout << "quote"<<endl;
        else if (node->token.type == STRING)
        {
            string s = node->token.str;
            for (size_t i = 0; i < s.length(); ++i)
            {
                if (s[i] == '\\' && i + 1 < s.length())
                {
                    if (s[i + 1] == '\\')
                    {
                        cout << "\\";
                        ++i;
                    }
                    else if (s[i + 1] == '\"')
                    {
                        cout << "\"";
                        ++i;
                    }
                    else if (s[i + 1] == 'n')
                    {
                        cout << "\n";
                        ++i;
                    }
                    else if (s[i + 1] == 't')
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

    // 印 (
    if(space)
        cout << string(indent, ' ') ;
    cout << "( ";
    Node *cur = node;
    bool first = true;
    while (cur)
    {
        if (first)
        {
            PrintSExp(cur->left, indent+2, false);
            first = false;
        }
        else
        {
            PrintSExp(cur->left, indent + 2, true);
        }

        if (!cur->right || cur->right->token.type == NIL)
            break;

        // right 是 atom（不是 cons cell）
        if (!cur->right->left && !cur->right->right )
        {
            cout << string(indent+2, ' ') << ".\n" ;
            PrintSExp(cur->right, indent + 2, true);
            break;
        }

        // 繼續往下走 cons cell
        cur = cur->right;
    }

    cout << string(indent, ' ') << ")" << endl;
}

int main()
{
    string tmp;
    getline(cin, tmp);
    cout << "Welcome to OurScheme!\n\n";
    bool newline = true;
    while (true)
    {
        gLine = 1;
        gColumn = 1;
        cout << "> ";
        Syntax syntax_anaylize;
        syntax_anaylize.init();
        TreeBuilder tree ;
        Node *root = NULL;
        if (syntax_anaylize.Is_SEXP()) {
            tree.init(syntax_anaylize.tokens);
            int index = 0 ;
            root = tree.BuildTree(index);
            if ( tree.check_exit( root ))
            {
                cout << endl;
                break;
            }
            PrintSExp(root, 0, true);
        }
        else 
        {
            syntax_anaylize.Error_message();
            if (syntax_anaylize.Is_END())
                break;
            
            if(!syntax_anaylize.Is_Error())
            {
                while( cin.peek() != '\n' && cin.peek() != EOF )
                    cin.get();

                cin.get();
            }
        }
        
        cout << endl;
    }

    cout << "Thanks for using OurScheme!";
}