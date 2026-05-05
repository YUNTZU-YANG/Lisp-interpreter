// 中原資工11027241 楊昀祖 -> OurC interpreter Project 2
# include <iostream> 
# include <stdio.h>
# include <vector>
# include <string> 
# include <stack>
# include <stdlib.h>

using namespace std;

enum Type {
  CIN, 
  COUT,
  IDENTIFIER, // Identifier
  CONSTANT,  // ex: 35, 35.67, 'a', "Hi, there", true, false, .35, 35., 0023
  INT,       // int
  FLOAT,     // float
  CHAR,      // char
  BOOL,      // bool
  STRING,    // string <----------- 注意全小寫！
  VOID,      // void
  IF,        // if
  ELSE,      // else
  WHILE,     // while
  DO,        // do
  RETURN,    // return
  SLBRACKET, // 小左括號 '('
  SRBRACKET, // 小右括號 ')'
  MLBRACKET, // 中左括號 '['
  MRBRACKET, // 中右括號 ']'
  LLBRACKET, // 大左括號 '{'
  LRBRACKET, // 大右括號 '}'
  PLUS,      // '+'
  SUB,       // '-'
  MUL,       // '*'
  DIV,       // '/'
  MOD,       // '%'
  GT,        // '>'
  LT,        // '<'
  GE,        // '>='
  LE,        // '<='
  EQ,        // '=='
  NEQ,       // '!='
  SAND,      // '&'
  SOR,       // '|'
  XOR,       // '^'
  ASSIGN,    // '='
  NOT,       // '!'
  AND,       // '&&'
  OR,        // '||'
  PE,        // '+=' plus assign
  ME,        // '-=' sub assign
  TE,        // '*=' mul assign
  DE,        // '/=' div assign
  RE,        // '%=' 取MOD中的'O' mod assign
  PP,        // '++' 
  MM,        // '--'
  RS,        // '>>'
  LS,        // '<<'
  SEMICOLON, // ';'
  COMMA,     // ','
  QUESION,   // conditional operator '?'
  COLON,     // conditional operator ':' 冒號英文為COLON
  ERROR,     // unrecgonize
  LISTALLVARIABLES, // system call
  LISTVARIABLE,    // system call
  LISTALLFUNCTIONS, // system call
  LISTFUNCTION,     // system call
  DONE,             // system call
  NOTHING           // init temp_token
};

struct Token{
  string token;
  int line;
  Type type;
};

struct Value{
  Type type;
  string name;
  string array;
};

struct Func{  
  Type type;
  string name;
  vector<Token> values; // parameter參數
  vector<Token> tokens;
};


struct Result {
  int number ;
  char character;
  string str;
  bool boolean;
} ;

int g_token_line = 1;
int g_int = 0;
bool g_lexical_error = false;      // 非法字元 - unrecognized token with first char
bool g_syntactical_error = false;  // 文法錯誤 - unexpected token
bool g_semantic_error = false;     // 宣告錯誤 - undefined identifier
bool g_definition_func = false;
bool g_definition_value = false;
bool g_store_token = false;        // store token in function 
bool g_executed = true;
Token g_tempToken ;                // 暫存的token
Token g_check_token ;              // 正在檢查的token
vector<Token> g_temptokenlist ;    // 存function tokens
vector<Value> g_tempValue ;
vector<Value> g_Value_All_List ;
Func g_tempFunc ;
vector<Func> g_Func_All_List ;

bool Is_type_specifier() ; 
bool Is_Bracket_add_constant() ;
bool Is_rest_of_declarators() ;
bool Is_formal_parameter_list() ;
bool Is_compound_statement() ;
bool Is_function_definition_without_ID() ;
bool Is_declaration() ;
bool Is_function_definition_or_declarators() ;
bool Is_Definition() ;
bool Is_expression() ;
bool Is_Statement() ;
bool Is_basic_expression() ;
bool Is_rest_of_Identifier_started_basic_exp() ;
bool Is_rest_of_PPMM_Identifier_started_basic_exp() ;
bool Is_signed_unary_exp() ;
bool Is_romce_and_romloe() ;
bool Is_maybe_logical_AND_exp() ;
bool Is_maybe_bit_OR_exp() ;
bool Is_maybe_bit_ex_OR_exp() ;
bool Is_maybe_bit_AND_exp() ;
bool Is_maybe_equality_exp() ;
bool Is_maybe_relational_exp() ;
bool Is_maybe_shift_exp() ;
bool Is_maybe_additive_exp() ;
bool Is_maybe_mult_exp() ;
bool Is_rest_of_maybe_mult_exp() ;
bool Is_unary_exp() ;
bool Is_actual_parameter_list() ;
bool Is_rest_of_maybe_logical_AND_exp() ;
bool Is_rest_of_maybe_bit_OR_exp() ;
bool Is_rest_of_maybe_bit_ex_OR_exp() ;
bool Is_rest_of_maybe_bit_AND_exp() ;
bool Is_rest_of_maybe_equality_exp() ;
bool Is_rest_of_maybe_relational_exp() ;
bool Is_rest_of_maybe_shift_exp() ;
bool Is_rest_of_maybe_additive_exp() ;
bool Is_Special_Grammar() ;
void Clear_a_line() ;
bool Is_SystemCall() ;
bool Set_SyntacticalError() ;
void Executed_SystemCall() ;
int Find_func( vector<Func> list, string str ) ;
int Find_value( vector<Value> list, string str ) ;
void Print_compound( int level, int i, int &index ) ;
// level sign spaces 
// i in list of functions index
// index is index of tokens

void Print_statement( int level, int i, int &index ) ;
// level sign spaces 
// i in list of functions index
// index is index of tokens

void Insert_Func( Func func ) ;
void Insert_value( Value value ) ;
void Print_ListAllFunctions() ;
void Print_ListAllVariables() ;
void InitToken( Token &token ) ;

/*
 * judge char is which type
 * change_Line  : '\n'
 * is_Space : ' ' || '\t'
 * is_Letter : 'a'. 'b'. 'c' ....
 * is_Digit '1'. '2'
*/


// only ' ' and '\t'
bool Is_Space( char c ) { 
  // judge char is space ( without '\n' )
  if ( c == ' ' || c == '\t' )
    return true;
  return false;
} // Is_Space()



bool Change_Line( char c ) {
  if ( c == '\n' )
    return true;
  return false;
} // Change_Line()


// only letters
bool Is_Letter( char c ) {
  if ( ( c -0 < 'a' -0 || c -0 > 'z' -0 ) && ( c -0 < 'A' -0 || c -0 > 'Z' -0 ) )
    return false;
  return true;
} // Is_Letter()


// only numbers
bool Is_Digit( char c ) {
  if ( ( ( c - 0 ) < '0' -0 || ( c - 0 ) > '9' -0 ) )
    return false;
  return true;
} // Is_Digit()


// + - * / % ^ > < & | ! = 
bool Is_Sign( char c ) {
  if ( c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || 
       c == '>' || c == '<' || c == '&' || c == '|' || c == '!' || c == '=' )
    return true;
  return false;
}   // Is_Sign() 


// 括號加';' ',' '?' ':'
bool Is_Other( char c ) {
  if ( c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || 
       c == '}' || c == ';' || c == ',' || c == '?' || c == ':'  )
    return true;
  return false;
} // Is_Other()


// 沒有用到
bool Is_Special( Token atoken ) {
  if (  atoken.token.length() == 2 && atoken.token[1] == '=' ) {
    if ( atoken.token[0] == '+' )
      return true;
    if ( atoken.token[0] == '-' )
      return true;
    if ( atoken.token[0] == '*' )
      return true;
    if ( atoken.token[0] == '/' )
      return true;
    if ( atoken.token[0] == '%' )
      return true;
    if ( atoken.token[0] == '>' )
      return true;
    if ( atoken.token[0] == '<' )
      return true;
    if ( atoken.token[0] == '=' )
      return true;
    if ( atoken.token[0] == '!' )
      return true;
  } // if ( s[0] == '=' )  e.g. ==, +=, -= ...
  else if ( atoken.token.length() >= 2 ) {
    if ( atoken.token[0] == '>' ) {
      if ( atoken.token[1] == '>' )
        return true;
    } // if ( atoken.token[0] == '>' )
    else if ( atoken.token[0] == '<' ) {
      if ( atoken.token[1] == '<' )
        return true;
    } // else if ( atoken.token[0] == '<' )
    else if ( atoken.token[0] == '+' ) {
      if ( atoken.token[1] == '+' )
        return true;
    } // else if ( atoken.token[0] == '+ ) 
    else if ( atoken.token[0] == '-' ) {
      if ( atoken.token[1] == '-' )
        return true;
    } // else if ( atoken.token[0] == '-' )
    else if ( atoken.token[0] == '&' ) {
      if ( atoken.token[1] == '&' )
        return true;
    } // else if ( atoken.token[0] == '&' )
    else if ( atoken.token[0] == '|' ) {
      if ( atoken.token[1] == '|' )
        return true;
    } // else if ( atoken.token[0] == '|' )
  } // else if

  if ( atoken.token.length() == 1 ) {
    if ( atoken.token[0] == '+' )
      return true;
    else if ( atoken.token[0] == '-' )
      return true;
    else if ( atoken.token[0] == '*' )
      return true;
    else if ( atoken.token[0] == '/' )
      return true;
    else if ( atoken.token[0] == '%' ) 
      return true;
    else if ( atoken.token[0] == '>' ) 
      return true;
    else if ( atoken.token[0] == '<' )
      return true;
    else if ( atoken.token[0] == '&' )
      return true;
    else if ( atoken.token[0] == '|' )
      return true;
    else if ( atoken.token[0] == '^' )
      return true;
    else if ( atoken.token[0] == '=' )
      return true;
    else if ( atoken.token[0] == '!' )
      return true;
    else if ( atoken.token[0] == '(' )
      return true;
    else if ( atoken.token[0] == ')' )
      return true;
    else if ( atoken.token[0] == '[' )
      return true;
    else if ( atoken.token[0] == ']' )
      return true;
    else if ( atoken.token[0] == '{' )
      return true;
    else if ( atoken.token[0] == '}' )
      return true;
    
  } // if 

  return false;
} // Is_Special()



// 用來判斷token 數字開頭或是dot開頭的字串是否正確
// 三種錯誤
// 字串只有一個dot 這裡解決
// 字串中有兩個dots - GetDigits fix
// 字串中有不屬於Digit or dot 的字元 這裡解決
bool Is_Number( string s ) {
  if ( s == "." ) {
    return false;
  } // if


  for ( int i = 0 ; i < s.length() ; i++ ) {
    if ( s[i] != '.' && !Is_Digit( s[i] ) )
      return false;
  } // for

  return true;
} // Is_Number()

// 用來判斷"開頭的字串是否正確
bool Is_String( string s ) {
  if ( s[0] == s[s.length() -1] )
    return true;
  return false;
} // Is_String()

bool Is_Char( Token atoken ) {
  if ( atoken.token.length() == 3 ) {
    if ( atoken.token[0] == '\'' && atoken.token[2] == '\'' )
      return true;
  } // if
  else if ( atoken.token.length() == 4 ) {
    if ( atoken.token[0] == '\'' && atoken.token[2] == '\'' ) {
      if ( atoken.token[1] == '\\' ) {
        if ( atoken.token[2] == '0'  || atoken.token[2] == 'n'  || atoken.token[2] == 't' || 
             atoken.token[2] == '\"' || atoken.token[2] == '\'' || atoken.token[2] == '\\' )
          return true;
      } // if
    } // if
  } // else if

  return false;
} // Is_Char()


// 分辨 token 的 type 並且回傳完整的Token回到Identifier
Token Type_Letter( Token atoken ) {
  if ( atoken.token == "cin" )    atoken.type = CIN;
  else if ( atoken.token == "cout" )    atoken.type = COUT;
  else if ( atoken.token == "int" )     atoken.type =  INT;
  else if ( atoken.token == "float" )    atoken.type = FLOAT;
  else if ( atoken.token == "char" )    atoken.type = CHAR;
  else if ( atoken.token == "bool" )    atoken.type = BOOL;
  else if ( atoken.token == "string" )    atoken.type = STRING;
  else if ( atoken.token == "void" )     atoken.type = VOID;
  else if ( atoken.token == "if" )     atoken.type = IF;
  else if ( atoken.token == "else" )    atoken.type = ELSE;
  else if ( atoken.token == "while" )     atoken.type = WHILE;
  else if ( atoken.token == "do" )    atoken.type = DO;
  else if ( atoken.token == "return" )    atoken.type = RETURN;
  else if ( atoken.token == "ListAllVariables" )    atoken.type = LISTALLVARIABLES ;
  else if ( atoken.token == "ListVariable" )    atoken.type = LISTVARIABLE;
  else if ( atoken.token == "ListAllFunctions" )     atoken.type = LISTALLFUNCTIONS;
  else if ( atoken.token == "ListFunction" )    atoken.type = LISTFUNCTION ;
  else if ( atoken.token == "Done" )     atoken.type = DONE;
  else if ( atoken.token == "true" )    atoken.type = CONSTANT ;
  else if ( atoken.token == "false" )    atoken.type = CONSTANT;
  else 
    atoken.type = IDENTIFIER;

  return atoken;
} // Type_Letter()


// 分辨 token 的 type 並且回傳完整的Token回到Identifier
Token Type_Sign( Token atoken ) {
  if (  atoken.token.length() == 2 && atoken.token[1] == '=' ) {
    if ( atoken.token[0] == '+' )
      atoken.type = PE;
    if ( atoken.token[0] == '-' )
      atoken.type = ME;
    if ( atoken.token[0] == '*' )
      atoken.type = TE;
    if ( atoken.token[0] == '/' )
      atoken.type = DE;
    if ( atoken.token[0] == '%' )
      atoken.type = RE;
    if ( atoken.token[0] == '>' )
      atoken.type = GE;
    if ( atoken.token[0] == '<' )
      atoken.type = LE;
    if ( atoken.token[0] == '=' )
      atoken.type = EQ;
    if ( atoken.token[0] == '!' )
      atoken.type = NEQ;
  } // if ( s[0] == '=' )  e.g. 
  else if ( atoken.token.length() >= 2 ) {
    if ( atoken.token[0] == '>' ) {
      if ( atoken.token[1] == '>' )
        atoken.type = RS;
    } // if ( atoken.token[0] == '>' )
    else if ( atoken.token[0] == '<' ) {
      if ( atoken.token[1] == '<' )
        atoken.type = LS;
    } // else if ( atoken.token[0] == '<' )
    else if ( atoken.token[0] == '+' ) {
      if ( atoken.token[1] == '+' )
        atoken.type = PP;
    } // else if ( atoken.token[0] == '+ ) 
    else if ( atoken.token[0] == '-' ) {
      if ( atoken.token[1] == '-' )
        atoken.type = MM;
    } // else if ( atoken.token[0] == '-' )
    else if ( atoken.token[0] == '&' ) {
      if ( atoken.token[1] == '&' )
        atoken.type = AND;
    } // else if ( atoken.token[0] == '&' )
    else if ( atoken.token[0] == '|' ) {
      if ( atoken.token[1] == '|' )
        atoken.type = OR;
    } // else if ( atoken.token[0] == '|' )
  } // if ( atoken.token.length() >= 2 )
  else if ( atoken.token.length() == 1 ) {
    if ( atoken.token[0] == '+' )
      atoken.type = PLUS;
    else if ( atoken.token[0] == '-' )
      atoken.type = SUB;
    else if ( atoken.token[0] == '*' )
      atoken.type = MUL;
    else if ( atoken.token[0] == '/' )
      atoken.type = DIV;
    else if ( atoken.token[0] == '%' ) 
      atoken.type = MOD;
    else if ( atoken.token[0] == '>' ) 
      atoken.type = GT;
    else if ( atoken.token[0] == '<' )
      atoken.type = LT;
    else if ( atoken.token[0] == '&' )
      atoken.type = SAND;
    else if ( atoken.token[0] == '|' )
      atoken.type = SOR;
    else if ( atoken.token[0] == '^' )
      atoken.type = XOR;
    else if ( atoken.token[0] == '=' )
      atoken.type = ASSIGN;
    else if ( atoken.token[0] == '!' )
      atoken.type = NOT;
    else if ( atoken.token[0] == '(' )
      atoken.type = SLBRACKET;
    else if ( atoken.token[0] == ')' )
      atoken.type = SRBRACKET;
    else if ( atoken.token[0] == '[' )
      atoken.type = MLBRACKET;
    else if ( atoken.token[0] == ']' )
      atoken.type = MRBRACKET;
    else if ( atoken.token[0] == '{' )
      atoken.type = LLBRACKET;
    else if ( atoken.token[0] == '}' )
      atoken.type = LRBRACKET;
    else if ( atoken.token[0] == ':' )
      atoken.type = COLON;
    else if ( atoken.token[0] == '?' )
      atoken.type = QUESION;
    else if ( atoken.token[0] == ';' )
      atoken.type = SEMICOLON;
    else if ( atoken.token[0] == ',' ) 
      atoken.type = COMMA;
  } // if 
  else {
    atoken.type = ERROR;
    g_lexical_error = true;
  } // else

  return atoken;
} // Type_Sign()

// 一直讀到不合乎Digit的字元或是peek到第二個dot
string GetDigits() {
  string str = "";
  char c = '\0' ;
  c = cin.peek();
  int dot = 0;
  while ( Is_Digit( c ) || c == '.' ) {
    if ( c == '.' ) 
      dot++;
    if ( dot >= 2 ) {
      return str;
    } // if

    c = cin.get();
    str += c;
    c = cin.peek();
  } // while
  
  return str;
} // GetDigits()

// 一直讀到不合乎Letter的字元
string GetLetter() {
  string str = "";
  char c = '\0' ;
  c = cin.peek();
  while ( Is_Letter( c ) || Is_Digit( c ) || c == '_' ) {
    c = cin.get() ;
    str += c;
    c = cin.peek() ;
  } // while
  
  return str;
} // GetLetter()


// 拿連在一起的Sign
// 在這邊解決註解問題
// 如果回傳 空字串就是遇到註解
string GetSign() {
  string str;
  char c = '\0' ;
  char nxt = '\0';

  c = cin.get();
  str += c ;
  nxt = cin.peek();
  
  if ( c == '/' ) {
    if ( nxt == '/' ) {
      Clear_a_line();
      return "";
    } // if
  } // if
  
  if ( c == '+' || c == '-' || c == '/' || c == '*' || 
       c == '%' || c == '!' || c == '=' ) {
    if ( nxt == '=' ) {
      c = cin.get();
      str += c ;
    } // if
    else if ( ( c == '+' && nxt == '+' ) || ( c == '-' && nxt == '-' ) ) {
      c = cin.get();
      str += c ;
    } // if
  } // if
  else if ( c == '>' || c == '<' ) {
    if ( nxt == '=' ) {
      c = cin.get();
      str += c ;
    } // if
    else if ( ( c == '>' && nxt == '>' ) || ( c == '<' && nxt == '<' ) ) {
      c = cin.get();
      str += c;
    } // else if
  } // else if
  else if ( c == '|' || c == '&' ) {
    if ( nxt == c ) {
      c = cin.get();
      str += c;
    } // if
  } // else if

  
  return str;
} // GetSign()


// 讀到有兩個'\"'或是換行符號就停止
string GetString() { 
  string str ;
  char c = '\0';
  c = cin.get();    // "
  str += c;
  c = cin.peek();
  while ( c != '\n' && c != '\"' ) {
    c = cin.get();
    str += c;
    c = cin.peek();
  } // while
  
  c = cin.get(); // 把'\n' or '"'讀掉
  str += c;

  return str;
} // GetString()

// 讀到有兩個'\''或是換行符號就停止
string GetChar() {
  string str = "";
  char c = '\0';
  c = cin.get(); 
  str += c; // 把'加進字串
  c = cin.peek();
  while ( c != '\n' && c != '\'' ) {
    c = cin.get();
    str += c;
    c = cin.peek();
  } // while

  c = cin.get(); // 把'\n' or '"'讀掉
  str += c;
  return str;
} // GetChar()

// 讀到非空白
void GetSpace() {
  char c = '\0';
  while ( Is_Space( cin.peek() ) ) 
    c = cin.get();
} // GetSpace()



// 拿一個token 
// 在文法分析時，如果需要拿下一個token 就再call 這個function 
// 然後拿去 identifier 製作成一個Token
Token GetToken() {
  Token atoken;
  char c = '\0';
  char nxt = '\0';
  c = cin.peek();
  if ( Is_Digit( c ) || c == '.' )
    atoken.token =  GetDigits();
  else if ( Is_Letter( c ) ) {
    atoken.token = GetLetter();  
  } // else if
  else if ( Is_Sign( c ) ) {
    atoken.token = GetSign();
    if ( atoken.token == "" ) { // 遇到註解
      return GetToken();
    } // if
  } // else if
  else if ( c == '"' )
    atoken.token = GetString();
  else if ( c == '\'' ) 
    atoken.token = GetChar();
  else if ( Is_Space( c ) ) {
    GetSpace();
    return GetToken();
  } // else if ( Is_Space( c ) )
  else if ( Is_Other( c ) ) {
    atoken.token = c ;
    c = cin.get();
  } // else if
  else if ( c == '\n' ) {
    c = cin.get(); // 把換行符號讀掉
    g_token_line ++;
    return GetToken();
  } // else if ( c == '\n' )
  else { // unrecognized token
    c = cin.get();
    atoken.token += c;
    g_lexical_error = true;
  } // else

  atoken.line = g_token_line ;
  return atoken;
} // GetToken()



// 把讀進來的token 分好類 組裝成一個token
Token Identifier() {
  // Grammer 會call這個function 然後拿到一個新的token
  // 依照文法數就會知道是否有文法問題
  Token atoken = GetToken() ;
  int line = 1;
  int i = 0;
  if ( Is_Digit( atoken.token[0] ) ) {
    if ( Is_Number( atoken.token ) ) {
      atoken.type = CONSTANT;
    } // if
    else {
      atoken.type = ERROR;
      g_lexical_error = true ;
    } // else
  } // if ( Is_Digit() )
  else if ( Is_Letter( atoken.token[0] ) ) {
    atoken = Type_Letter( atoken );
  } // else if
  else if ( Is_Sign( atoken.token[0] ) ) {
    atoken = Type_Sign( atoken );
  } // else if
  else if ( atoken.token[0] == '\"' ) {
    if ( Is_String( atoken.token ) )
      atoken.type = CONSTANT;
    else { 
      atoken.type = ERROR;
      g_lexical_error = true;
    } // else
  } // else if
  else if ( atoken.token[0] == '\'' ) {
    if ( Is_Char( atoken ) ) 
      atoken.type = CONSTANT;
    else {
      atoken.type = ERROR;
      g_lexical_error = true;
    } // else
  } // else if
  else if ( Is_Other( atoken.token[0] ) ) {
    atoken = Type_Sign( atoken );
  } // else if
  else {
    g_lexical_error = true ;
    atoken.type = ERROR;
  } // else
  
  if ( g_store_token )
    g_temptokenlist.push_back( atoken );
  
  return atoken;
} // Identifier()


void Update_CheckToken() {
  if ( g_tempToken.type != NOTHING ) {
    g_check_token = g_tempToken;
    InitToken( g_tempToken );
  } // if
  else {
    g_check_token = Identifier();
  } // else
  
} // Update_CheckToken()



// ============ Grammar ===============


// INT CHAR ......
bool Is_type_specifier() {
  if ( g_check_token.type == INT   || g_check_token.type == CHAR   ||
       g_check_token.type == FLOAT || g_check_token.type == STRING ||
       g_check_token.type == BOOL )
    return true;
  return false;
} // Is_type_specifier()

// judge '[' constant ']'
bool Is_Bracket_add_constant() {
  if ( g_check_token.type == MLBRACKET ) {
    Update_CheckToken();
    if ( g_check_token.type == CONSTANT ) {
      Update_CheckToken();
      if ( g_check_token.type == MRBRACKET ) {
        Update_CheckToken(); 
        return true;
      } // if
      else {
        return Set_SyntacticalError() ;
      } // else
    } // if 
    else {
      return Set_SyntacticalError();
    } // else 
  } // if

  return false;
} // Is_Bracket_add_constant()

bool Is_rest_of_declarators() { // 宣告
/*
  e.g. : [ '[' Constant ']' ]  { ',' Identifier [ '[' Constant ']' ] }  ';'
*/
  if ( g_check_token.type == MLBRACKET ) {
    Update_CheckToken();
    if ( g_check_token.type == CONSTANT ) {
      g_tempValue[0].array = g_check_token.token;
      Update_CheckToken();
      if ( g_check_token.type == MRBRACKET ) {
        Update_CheckToken();
      } // if
      else {
        return Set_SyntacticalError() ;
      } // else
    } // if
    else {
      return Set_SyntacticalError() ; // Grammar error
    } // else
  } // if

  // 外面已經有push_back過一次了

  Value temp ;
  if ( g_check_token.type == COMMA ) {
    while ( g_check_token.type == COMMA ) {
      temp.array = "";
      temp.type = g_tempValue[0].type;
      Update_CheckToken();
      if ( g_check_token.type == IDENTIFIER ) {
        temp.name = g_check_token.token;
        g_tempValue.push_back( temp ) ;
        Update_CheckToken();
        if ( g_check_token.type == MLBRACKET ) {
          Update_CheckToken();
          if ( g_check_token.type == CONSTANT ) {
            g_tempValue[ g_tempValue.size() -1 ].array = g_check_token.token;
            Update_CheckToken();
            if ( g_check_token.type == MRBRACKET ) {
              Update_CheckToken();
            } // if
            else {
              return Set_SyntacticalError() ; // Grammar error
            } // else
          } // if
          else {
            return Set_SyntacticalError() ; // Grammar error
          } // else
        } // if
        else {
          ;
        } // else
      } // if
      else { 
        return Set_SyntacticalError() ; // Grammar error
      } // else
    } // while
  } // if

  if ( g_check_token.type == SEMICOLON ) {
    g_definition_value = true;
    return true;
  } // if

  return Set_SyntacticalError();
} // Is_rest_of_declarators()



bool Is_formal_parameter_list() { // function parameter
  //  type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
  // { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
  Value value;
  value.name = "";
  value.type = NOTHING;
  value.array = "" ;


  if ( Is_type_specifier() ) {
    g_tempFunc.values.push_back( g_check_token );
    value.type = g_check_token.type;
    Update_CheckToken();
    if ( g_check_token.type == SAND ) { // '&'
      g_tempFunc.values.push_back( g_check_token );
      Update_CheckToken();
    } // if

    if ( g_check_token.type == IDENTIFIER ) {
      g_tempFunc.values.push_back( g_check_token );
      value.name = g_check_token.token;
      Update_CheckToken();
      if ( g_check_token.type == MLBRACKET ) { // '['
        g_tempFunc.values.push_back( g_check_token );
        Update_CheckToken();
        if ( g_check_token.type == CONSTANT ) {
          g_tempFunc.values.push_back( g_check_token );
          value.array = g_check_token.token;
          Update_CheckToken();
          if ( g_check_token.type == MRBRACKET ) { // ']'
            g_tempFunc.values.push_back( g_check_token );
            Update_CheckToken();
          } // if
          else 
            return Set_SyntacticalError() ; // Grammar error
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      }  // if '['


      g_tempValue.push_back( value );
      while ( g_check_token.type == COMMA ) { // ','
        value.array = "";
        value.type = NOTHING;
        value.name = "";

        g_tempFunc.values.push_back( g_check_token ) ;
        Update_CheckToken(); 
        if ( Is_type_specifier() ) {
          value.type = g_check_token.type;
          g_tempFunc.values.push_back( g_check_token ) ;
          Update_CheckToken();

          if ( g_check_token.type == SAND ) {   // int / char / bool &
            g_tempFunc.values.push_back( g_check_token );
            Update_CheckToken();
          } // if

          if ( g_check_token.type == IDENTIFIER ) {
            value.name = g_check_token.token;
            g_tempFunc.values.push_back( g_check_token );
            Update_CheckToken();
            if ( g_check_token.type == MLBRACKET ) { // '['
              g_tempFunc.values.push_back( g_check_token ) ;
              Update_CheckToken();
              if ( g_check_token.type == CONSTANT ) { 
                value.array = g_check_token.token;
                g_tempFunc.values.push_back( g_check_token ) ;
                Update_CheckToken();
                if ( g_check_token.type == MRBRACKET ) { // ']'  
                  g_tempFunc.values.push_back( g_check_token );
                  Update_CheckToken();
                } // if
                else 
                  return Set_SyntacticalError() ;
              } // if
              else 
                return Set_SyntacticalError() ;
            }  // if '['
          } // if
          else 
            return Set_SyntacticalError() ;
        } // if
        else 
          return Set_SyntacticalError() ;

        g_tempValue.push_back( value );
      } // while // ','
    } // if

    return true;
  } // if 


  return false;
} // Is_formal_parameter_list()



bool Is_compound_statement() {
  // e.g. : '{' { declaration | statement } '}'
  vector<Value> values = g_tempValue; // 記全域變數和前面有宣告過的區域變數
  if ( g_check_token.type == LLBRACKET ) { // '{'
    Update_CheckToken();
    while ( Is_declaration() || Is_Statement() ) {
      if ( g_syntactical_error )
        return Set_SyntacticalError();

      if ( g_tempToken.type == NOTHING ) {
        Update_CheckToken();
      } // if
      else {
        g_check_token = g_tempToken;
        InitToken( g_tempToken );
      } // else 
    } // while

    g_tempValue = values; // 還原成原本的全域變數和前面有宣告過的區域變數
    if ( g_check_token.type == LRBRACKET ) {
      return true;
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if

  return false;
} // Is_compound_statement()



bool Is_function_definition_without_ID() {   // 宣告function 
  // e.g. : '(' [ VOID | formal_parameter_list ] ')' compound_statement
  if ( g_check_token.type == SLBRACKET ) { // '('
    Update_CheckToken();
    if ( g_check_token.type == VOID  ) {
      g_tempFunc.values.push_back( g_check_token );
      Update_CheckToken();
    } // if 可有可無
    else if ( Is_formal_parameter_list() ) {
      ;
    } // else if

    if ( g_check_token.type == SRBRACKET ) { // ')'
      g_store_token = true ; // 把 '{' 、 '}' 也存進去
      Update_CheckToken();
      if ( Is_compound_statement() ) { // 宣告function 
        g_tempFunc.tokens = g_temptokenlist ;
        g_temptokenlist.clear();
        g_definition_func = true ;
        g_store_token = false ; // 停止存token
        return true;
      } // if
      else {
        g_store_token = false; // 停止存token
        g_temptokenlist.clear();
        return Set_SyntacticalError() ; // Grammar error
      } // else
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if
  
  return false;
} // Is_function_definition_without_ID()


bool Is_declaration() { // 宣告變數
  // e.g. : type_specifier Identifier rest_of_declarators
  Value temp ;
  if ( Is_type_specifier() ) {
    temp.type = g_check_token.type;
    Update_CheckToken();
    if ( g_check_token.type == IDENTIFIER ) {
      temp.name = g_check_token.token;
      g_tempValue.push_back( temp );
      Update_CheckToken();
      if ( Is_rest_of_declarators() ) {
        return true;
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if

  return false;
} // Is_declaration()




bool Is_function_definition_or_declarators() { // 這邊有問題
  // e.g. : function_definition_without_ID | rest_of_declarators
  if ( Is_function_definition_without_ID() ||  Is_rest_of_declarators() ) {
    return true;
  } // if

  return false;
} // Is_function_definition_or_declarators()


bool Is_Definition() {
  // function要存token 用 golbe bool 讓UpdateToken再讀的時候一起存到裡面
  // 結束記得要把bool 設為false
  // 讓UpdateToken 停止存token

  // e.g. : VOID Identifier function_definition_without_ID | 
  //        type_specifier Identifier function_definition_or_declarators

  // reset tempFunc
  g_tempFunc.name = "";
  g_tempFunc.type = NOTHING;
  g_tempFunc.tokens.clear();
  g_tempFunc.values.clear();

  // reset tempValue
  g_tempValue.clear();
  g_definition_func = false;
  g_definition_value = false;

  Value temp_value;
  if ( g_check_token.type == VOID ) {
    g_tempFunc.type = VOID;
    Update_CheckToken();
    if ( g_check_token.type == IDENTIFIER ) {
      g_tempFunc.name = g_check_token.token;
      Update_CheckToken();
      if ( Is_function_definition_without_ID() ) {
        g_definition_func = true;
        return true; // 宣告函數
      } // if
      else {
        return Set_SyntacticalError() ; // Grammar error
      } // else 
    } // if ( tokens[tokens_index].type == "Identifier" )
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if ( tokens[tokens_index].type == "VOID" ) 
  else if ( Is_type_specifier() ) {
    temp_value.type = g_check_token.type;
    g_tempFunc.type = g_check_token.type;
    Update_CheckToken();
    if ( g_check_token.type == IDENTIFIER ) {
      g_tempFunc.name = g_check_token.token;
      temp_value.name = g_check_token.token;
      g_tempValue.push_back( temp_value );
      Update_CheckToken();
      if ( Is_function_definition_or_declarators() ) {
        return true; 
      } // if
      else {
        return Set_SyntacticalError() ; // Grammar error
      } // else
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if ( Is_type_specifier( tokens[tokens_index ) )
  

  return false;
} // Is_Definition()



bool Is_assignment_operator() {
  // e.g. : '=' | TE | DE | RE | PE | ME
  if ( g_check_token.type == ASSIGN || g_check_token.type == TE || 
       g_check_token.type == DE || g_check_token.type == RE || 
       g_check_token.type == PE || g_check_token.type == ME ) {
    Update_CheckToken();
    return true;
  } // if

  return false;
} // Is_assignment_operator()



bool Is_rest_of_Identifier_started_basic_exp() {
  /*
  e.g. : [ '[' expression ']' ] ( assignment_operator basic_expression  |  [ PP | MM ] romce_and_romloe ) | 
           '(' [ actual_parameter_list ] ')' romce_and_romloe


           要先檢查'(' [ actual_parameter_list ] ')' romce_and_romloe不然會被 [ PP | MM ] romce_and_romloe 吃掉
  */
  
  if ( g_check_token.type == MLBRACKET ) {
    Update_CheckToken();
    if ( Is_expression() ) {
      if ( g_check_token.type == MRBRACKET ) {
        Update_CheckToken();
        if ( Is_assignment_operator() ) {
          if ( Is_basic_expression() ) 
            return true;
          else 
            return Set_SyntacticalError() ; // Grammar error
        } // if
        else if ( g_check_token.type == PP || g_check_token.type == MM ) {
          Update_CheckToken();
          if ( Is_romce_and_romloe() ) 
            return true;
          else 
            return Set_SyntacticalError() ; // Grammar error
        } // else if
        else if ( Is_romce_and_romloe() ) {
          return true;
        } // else if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if
  else if ( Is_assignment_operator() ) {
    if ( Is_basic_expression() ) 
      return true;
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else if ( g_check_token.type == PP || g_check_token.type == MM ) {
    Update_CheckToken();
    if ( Is_romce_and_romloe() ) 
      return true;
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else if ( g_check_token.type == SLBRACKET ) {
    Update_CheckToken();
    if ( Is_actual_parameter_list() ) {
      if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        if ( Is_romce_and_romloe() )
          return true;
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else if ( g_check_token.type == SRBRACKET ) {
      Update_CheckToken();
      if ( Is_romce_and_romloe() ) 
        return true;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // else if
    else  
      return false;
  } // else if 
  else if ( Is_romce_and_romloe() ) {
    return true;
  } // else if
  
  
  return false;
} // Is_rest_of_Identifier_started_basic_exp()



bool Is_rest_of_PPMM_Identifier_started_basic_exp() {
  // e.g. : [ '[' expression ']' ] romce_and_romloe
  if ( g_check_token.type == MLBRACKET ) {
    Update_CheckToken();
    if ( Is_expression() ) {
      if ( g_check_token.type == MRBRACKET ) {
        Update_CheckToken();
        if ( Is_romce_and_romloe() )
          return true;
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if
  else if ( Is_romce_and_romloe() ) 
    return true;
  return false;
} // Is_rest_of_PPMM_Identifier_started_basic_exp()


bool Is_sign_for_Grammar() {
  if ( g_check_token.type == PLUS || g_check_token.type == SUB ||
       g_check_token.type == NOT )
    return true;
  return false;
} // Is_sign_for_Grammar()


bool Is_signed_unary_exp() { 
  // Identifier ['(' [ actual_parameter_list ] ')' | '[' expression ']' ] | 
  // Constant  | 
  // '(' expression ')'
  
  if ( g_check_token.type == IDENTIFIER || g_check_token.type == CIN ||  
       g_check_token.type == COUT ) {
    g_tempToken = Identifier();
    if ( g_tempToken.type == SLBRACKET ) {
      if ( Find_func( g_Func_All_List, g_check_token.token ) == -1 && 
           g_tempFunc.name != g_check_token.token ) {
        g_semantic_error = true;
        return false;
      } // if
    } // if
    else {
      if ( Find_value( g_tempValue, g_check_token.token ) == -1 && 
           Find_value( g_Value_All_List, g_check_token.token ) == -1 ) {
        g_semantic_error = true;
        return false;
      } // if
    } // else

    Update_CheckToken();
    if ( g_check_token.type == SLBRACKET ) {
      Update_CheckToken();
      if ( Is_actual_parameter_list() ) {
        if ( g_check_token.type == SRBRACKET ) {
          Update_CheckToken();
          return true;
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if 
      else if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        return true; 
      } // else if
    } // if
    else if ( g_check_token.type == MLBRACKET ) {
      Update_CheckToken();
      if ( Is_expression() ) {
        if ( g_check_token.type == MRBRACKET ) {
          Update_CheckToken();
          return true;
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // else if
    else 
      return true;
  } // if
  else if ( g_check_token.type == CONSTANT ) {
    Update_CheckToken();
    return true;
  } // else if
  else if ( g_check_token.type == SLBRACKET ) {
    Update_CheckToken();
    if ( Is_expression() ) {
      if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        return true;
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else
    return false;

  return false;
} // Is_signed_unary_exp()


bool Is_rest_of_maybe_additive_exp() {
  // e.g. : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  if ( Is_rest_of_maybe_mult_exp() ) {
    while ( g_check_token.type == PLUS || g_check_token.type == SUB ) {
      Update_CheckToken();
      if ( Is_maybe_mult_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_rest_of_maybe_additive_exp()


bool Is_rest_of_maybe_shift_exp() {
  // e.g. : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }"
  if ( Is_rest_of_maybe_additive_exp() ) {
    while ( g_check_token.type == LS || g_check_token.type == RS ) {
      Update_CheckToken();
      if ( Is_maybe_additive_exp() )
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_rest_of_maybe_shift_exp()



bool Is_rest_of_maybe_relational_exp() {
  // e.g. : rest_of_maybe_shift_exp  { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  if ( Is_rest_of_maybe_shift_exp() ) {
    while ( g_check_token.type == LT || g_check_token.type == GT ||
            g_check_token.type == LE || g_check_token.type == GE ) {
      Update_CheckToken();
      if ( Is_maybe_shift_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_rest_of_maybe_relational_exp()



bool Is_rest_of_maybe_equality_exp() {
  // : rest_of_maybe_relational_exp  { ( EQ | NEQ ) maybe_relational_exp }
  if ( Is_rest_of_maybe_relational_exp() ) {
    while ( g_check_token.type == EQ || g_check_token.type == NEQ ) {
      Update_CheckToken();
      if ( Is_maybe_relational_exp() )
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_rest_of_maybe_equality_exp() 






bool Is_rest_of_maybe_bit_AND_exp() {
  // e.g. : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
  if ( Is_rest_of_maybe_equality_exp() ) {
    while ( g_check_token.type == SAND ) {
      Update_CheckToken();
      if ( Is_maybe_equality_exp() )
        Update_CheckToken();
      else 
        return false;
    } // while
    
    return true;
  } // if

  return false;
} // Is_rest_of_maybe_bit_AND_exp()


bool Is_rest_of_maybe_bit_ex_OR_exp() {
  // e.g. : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  if ( Is_rest_of_maybe_bit_AND_exp() ) {
    while ( g_check_token.type == XOR ) {
      Update_CheckToken();
      if ( Is_maybe_bit_AND_exp() ) 
        Update_CheckToken();
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_rest_of_maybe_bit_ex_OR_exp()



bool Is_rest_of_maybe_bit_OR_exp() {
  // e.g. : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  if ( Is_rest_of_maybe_bit_ex_OR_exp() ) {
    while ( g_check_token.type == SOR ) {
      Update_CheckToken();
      if ( Is_maybe_bit_ex_OR_exp() )
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_rest_of_maybe_bit_OR_exp()



bool Is_rest_of_maybe_logical_AND_exp() {
  // e.g. : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  if ( Is_rest_of_maybe_bit_OR_exp() ) {
    while ( g_check_token.type == AND ) {
      Update_CheckToken();
      if ( Is_maybe_bit_OR_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if
  
  return false;
} // Is_rest_of_maybe_logical_AND_exp()


bool Is_actual_parameter_list() {
  // e.g. : basic_expression { ',' basic_expression }
  if ( Is_basic_expression() ) {
    while ( g_check_token.type == COMMA ) {
      Update_CheckToken();
      if ( Is_basic_expression() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while
    
    return true;
  } // if

  return false;
} // Is_actual_parameter_list()


bool Is_unsigned_unary_exp() { // 使用function
/*
    e.g. :Identifier [ '(' [ actual_parameter_list ] ')' |  [ '[' expression ']' ] [ ( PP | MM ) ]] | 
          Constant  | 
          '(' expression ')'
*/
  if ( g_check_token.type == IDENTIFIER || g_check_token.type == CIN ||  
       g_check_token.type == COUT ) {
    g_tempToken = Identifier(); // peek token
    if ( g_tempToken.type == SLBRACKET ) {
      if ( Find_func( g_Func_All_List, g_check_token.token ) == -1 &&
           g_tempFunc.name != g_check_token.token ) {
        g_semantic_error = true;
        return false;
      } // if
    } // if
    else {
      if ( Find_value( g_tempValue, g_check_token.token ) == -1 && 
           Find_value( g_Value_All_List, g_check_token.token ) == -1 ) {
        g_semantic_error = true;
        return false;
      } // if
    } // else
    
    Update_CheckToken(); // check_token is '('
    if ( g_check_token.type == SLBRACKET ) { 
       //  Identifier '(' [ actual_parameter_list ] ')' 
      Update_CheckToken();
      if ( Is_actual_parameter_list() ) {
        if ( g_check_token.type == SRBRACKET ) {
          Update_CheckToken(); 
          return true;
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        return true;
      } // else if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else if ( g_check_token.type == MLBRACKET ) { 
      // Identifier [ '[' expression ']' ] [ ( PP | MM ) ]
      Update_CheckToken();
      if ( Is_expression() ) {
        if ( g_check_token.type == MRBRACKET ) {
          Update_CheckToken();
          if ( g_check_token.type == PP || g_check_token.type == MM ) {
            Update_CheckToken();
            return true;
          } // if

          return true;
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // else if
    else if ( g_check_token.type == PP || g_check_token.type == MM ) {
      // Identifier [ ( PP | MM ) ]
      Update_CheckToken();
      return true;
    } // else if

    return true;
  } // if
  else if ( g_check_token.type == CONSTANT ) {
    Update_CheckToken();
    return true;
  } // else if 
  else if ( g_check_token.type == SLBRACKET ) {
    // '(' expression ')'
    Update_CheckToken();
    if ( Is_expression() ) {
      if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        return true;
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else 
    return false;

  return false;
} // Is_unsigned_unary_exp()


bool Is_unary_exp() {
/*
     e.g. : sign { sign } signed_unary_exp  |
            unsigned_unary_exp | 
          ( PP | MM ) Identifier [ '['expression ']'  ]
*/
  if ( Is_sign_for_Grammar() ) {
    Update_CheckToken();
    while ( Is_sign_for_Grammar() ) 
      Update_CheckToken(); 
    if ( Is_signed_unary_exp() ) {
      return true; // sign { sign } signed_unary_exp
    } // if
    else 
      return Set_SyntacticalError() ;
  } // if
  else if ( Is_unsigned_unary_exp() ) {
    return true; // unsigned_unary_exp
  } // else if 
  else if ( g_check_token.type == PP || g_check_token.type == MM ) {
    Update_CheckToken();
    if ( g_check_token.type == IDENTIFIER || g_check_token.type == CIN ||  
         g_check_token.type == COUT ) {
      g_tempToken = Identifier();
      if ( g_tempToken.type == SLBRACKET ) {
        if ( Find_func( g_Func_All_List, g_check_token.token ) == -1 && 
             g_tempFunc.name != g_check_token.token ) {
          g_semantic_error = true;
          return false;
        } // if
      } // if
      else {
        if ( Find_value( g_tempValue, g_check_token.token ) == -1 && 
             Find_value( g_Value_All_List, g_check_token.token ) == -1 ) {
          g_semantic_error = true;
          return false;
        } // if
      } // else

      Update_CheckToken();
      if ( g_check_token.type == MLBRACKET ) {
        Update_CheckToken();
        if ( Is_expression() ) {
          if ( g_check_token.type == MRBRACKET ) {
            Update_CheckToken();
            return true; // ( PP | MM ) Identifier '['expression ']'
          } // if
          else  
            return Set_SyntacticalError() ; 
        } // if
        else 
          return Set_SyntacticalError() ; 
      } // if

      return true; // ( PP | MM ) Identifier
    } // if
    else 
      return Set_SyntacticalError() ;
  } // else if

  return false;
} // Is_unary_exp()


bool Is_rest_of_maybe_mult_exp() {
  // e.g. : { ( '*' | '/' | '%' ) unary_exp } 
  while ( g_check_token.type == MUL || g_check_token.type == DIV || g_check_token.type == MOD ) {
    Update_CheckToken();
    if ( Is_unary_exp() ) {
      ;
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // while
  
  return true;
} // Is_rest_of_maybe_mult_exp()



bool Is_maybe_mult_exp() {
  // : unary_exp  rest_of_maybe_mult_exp
  if ( Is_unary_exp() ) { 
    if ( Is_rest_of_maybe_mult_exp() ) {
      return true;
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar
  } // if

  return false;
} // Is_maybe_mult_exp()




bool Is_maybe_additive_exp() {
  // : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
  if ( Is_maybe_mult_exp() ) {
    while ( g_check_token.type == PLUS || g_check_token.type == SUB ) {
      Update_CheckToken();
      if ( Is_maybe_mult_exp() ) {
        ;
      } // if
      else {
        return Set_SyntacticalError() ; // Grammar error
      } // else
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_additive_exp()


bool Is_maybe_shift_exp() {
  // : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
  if ( Is_maybe_additive_exp() ) {
    while ( g_check_token.type == LS || g_check_token.type == RS ) {
      Update_CheckToken();
      if ( Is_maybe_additive_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_shift_exp()

bool Is_maybe_relational_exp() {
  // e.g. : maybe_shift_exp  { ( '<' | '>' | LE | GE ) maybe_shift_exp }
  if ( Is_maybe_shift_exp() ) {
    while ( g_check_token.type == LT || g_check_token.type == GT ||
            g_check_token.type == LE || g_check_token.type == GE ) {
      Update_CheckToken();
      if ( Is_maybe_shift_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_relational_exp()


bool Is_maybe_equality_exp() { 
  // : maybe_relational_exp  { ( EQ | NEQ ) maybe_relational_exp}
  if ( Is_maybe_relational_exp() ) {
    while ( g_check_token.type == EQ || g_check_token.type == NEQ ) {
      Update_CheckToken();
      if ( Is_maybe_relational_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_equality_exp()


bool Is_maybe_bit_AND_exp() {
  // e.g. : maybe_equality_exp { '&' maybe_equality_exp }
  if ( Is_maybe_equality_exp() ) {
    while ( g_check_token.type == SAND ) {
      Update_CheckToken();
      if ( Is_maybe_equality_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_bit_AND_exp()


bool Is_maybe_bit_ex_OR_exp() {
  // e.g. : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
  if ( Is_maybe_bit_AND_exp() ) {
    while ( g_check_token.type == XOR ) {
      Update_CheckToken();
      if ( Is_maybe_bit_AND_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while


    return true; 
  } // if 

  return false;
} // Is_maybe_bit_ex_OR_exp()


bool Is_maybe_bit_OR_exp() {
  // e.g. : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
  if ( Is_maybe_bit_ex_OR_exp() ) {
    while ( g_check_token.type == SOR ) {
      Update_CheckToken();
      if ( Is_maybe_bit_ex_OR_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_bit_OR_exp()


bool Is_maybe_logical_AND_exp() {
  // e.g. : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
  if ( Is_maybe_bit_OR_exp() ) {
    while ( g_check_token.type == AND ) {
      Update_CheckToken();
      if ( Is_maybe_bit_OR_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if

  return false;
} // Is_maybe_logical_AND_exp()


bool Is_rest_of_maybe_logical_OR_exp() {
  // e.g. : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
  if ( Is_rest_of_maybe_logical_AND_exp() ) {
    while ( g_check_token.type == OR ) {
      Update_CheckToken();
      if ( Is_maybe_logical_AND_exp() ) 
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while

    return true;
  } // if
  
  return false;
} // Is_rest_of_maybe_logical_OR_exp()



  
bool Is_romce_and_romloe() {
  // e.g. : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
  if ( Is_rest_of_maybe_logical_OR_exp() ) {
    if ( g_check_token.type == QUESION ) {
      Update_CheckToken();
      if ( Is_basic_expression() ) {
        if ( g_check_token.type == COLON ) {
          Update_CheckToken();
          if ( Is_basic_expression() )
            return true;
          else 
            return Set_SyntacticalError() ; // Grammar error
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return true;

    return true;
  } // if

  return false;
} // Is_romce_and_romloe()


bool Is_basic_expression() {
  // e.g. : Identifier rest_of_Identifier_started_basic_exp
  // | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  // | sign { sign } signed_unary_exp romce_and_romloe
  // | ( Constant | '(' expression ')' ) romce_and_romloe

  if ( g_check_token.type == IDENTIFIER || g_check_token.type == CIN ||
       g_check_token.type == COUT ) {
    g_tempToken = Identifier();
    if ( g_tempToken.type == SLBRACKET ) {
      if ( Find_func( g_Func_All_List, g_check_token.token ) == -1 && 
           g_tempFunc.name != g_check_token.token ) {
        g_semantic_error = true;
        return false;
      } // if
    } // if
    else {
      if ( Find_value( g_tempValue, g_check_token.token ) == -1 && 
           Find_value( g_Value_All_List, g_check_token.token ) == -1 ) {
        g_semantic_error = true;
        return false;
      } // if
    } // else

    Update_CheckToken();
    if ( Is_rest_of_Identifier_started_basic_exp() ) {
      return true;
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // if
  else if ( g_check_token.type == PP || g_check_token.type == MM ) {
    Update_CheckToken();
    if ( g_check_token.type == IDENTIFIER || g_check_token.type == CIN || 
         g_check_token.type == COUT ) {
      g_tempToken = Identifier();
      if ( g_tempToken.type == SLBRACKET ) {
        if ( Find_func( g_Func_All_List, g_check_token.token ) == -1 && 
             g_tempFunc.name != g_check_token.token ) {
          g_semantic_error = true;
          return false;
        } // if
      } // if
      else {
        if ( Find_value( g_tempValue, g_check_token.token ) == -1 && 
             Find_value( g_Value_All_List, g_check_token.token ) == -1 ) {
          g_semantic_error = true;
          return false;
        } // if
      } // else

      Update_CheckToken();
      if ( Is_rest_of_PPMM_Identifier_started_basic_exp() )
        return true;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if 
  else if ( Is_sign_for_Grammar() ) {
    Update_CheckToken();
    while ( Is_sign_for_Grammar() ) 
      Update_CheckToken();
    if ( Is_signed_unary_exp() ) {
      if ( Is_romce_and_romloe() )
        return true;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if 
  else if ( g_check_token.type == CONSTANT ) {
    Update_CheckToken();
    if ( Is_romce_and_romloe() ) {
      return true;
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else if ( g_check_token.type == SLBRACKET ) {
    Update_CheckToken();
    if ( Is_expression() ) {
      if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        if ( Is_romce_and_romloe() ) {
          return true;
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if 
      else 
        return Set_SyntacticalError() ; // Grammar error 
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if

  return false;
} // Is_basic_expression()

bool Is_expression() {
  // e.g. : basic_expression { ',' basic_expression }
  if ( Is_basic_expression() ) {
    while ( g_check_token.type == COMMA ) {
      Update_CheckToken();
      if ( Is_basic_expression() )
        ;
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // while
    
    return true;
  } // if
  
  return false;
} // Is_expression()


bool Is_Statement() {
/*  
    e.g. : ';' // the null statement
          | expression ';'
          | compound_statement
          | RETURN [ expression ] ';'
          | IF '(' expression ')' statement [ ELSE statement ]
          | WHILE '(' expression ')' statement
          | DO statement WHILE '(' expression ')' ';'
*/
  if ( g_check_token.type == SEMICOLON ) { // ;
    return true;
  } // if
  else if ( Is_expression() ) {
    if ( g_check_token.type == SEMICOLON ) { // ;
      return true;
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if 
  else if ( Is_compound_statement() ) {
    return true;
  } // else if
  else if ( g_check_token.type == RETURN ) { 
    Update_CheckToken();
    if ( Is_expression() ) {
      if ( g_check_token.type == SEMICOLON ) {
        return true; // RETURN expression  ';'
      } // if
      else {
        return Set_SyntacticalError() ; // Grammar error
      } // else
    } // if
    else if ( g_check_token.type == SEMICOLON ) 
      return true; // RETURN ';'
    else 
      return Set_SyntacticalError() ; // Grammar error  
  } // else if "return"
  else if ( g_check_token.type == IF ) {
    Update_CheckToken();
    if ( g_check_token.type == SLBRACKET ) {
      Update_CheckToken();
      if ( Is_expression() ) {
        if ( g_check_token.type == SRBRACKET ) {
          Update_CheckToken();
          if ( Is_Statement() ) {
            g_tempToken = Identifier(); // peek token
            
            if ( g_tempToken.type == ELSE ) { 
              Update_CheckToken() ; // 從g_temp_token 吃掉 else 
              Update_CheckToken() ;
              if ( Is_Statement() ) {
                return true; // IF '(' expression ')' statement [ ELSE statement ]
              } // if
              else {
                return Set_SyntacticalError() ; // Grammar error
              } // else
            } // if  可有可無
            else {
              return true; // IF '(' expression ')' statement
            } // else
          } // if

          return Set_SyntacticalError() ; // Grammar error
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  }  // else if
  else if ( g_check_token.type == WHILE ) {
    Update_CheckToken();
    if ( g_check_token.type == SLBRACKET ) {
      Update_CheckToken();
      if ( Is_expression() ) {
        if ( g_check_token.type == SRBRACKET ) {
          Update_CheckToken(); 
          if ( Is_Statement() ) {
            return true;  // WHILE '(' expression ')' statement
          } // if
          else {
            return Set_SyntacticalError() ; // Grammar error
          } // else
        } // if
        else
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else if ( g_check_token.type == DO ) {
    Update_CheckToken();
    if ( Is_Statement() ) {
      if ( g_check_token.type == LRBRACKET ) {
        Update_CheckToken();
        if ( g_check_token.type == WHILE ) {
          Update_CheckToken();
          if ( g_check_token.type == SLBRACKET ) {
            Update_CheckToken();
            if ( Is_expression() ) {
              if ( g_check_token.type == SRBRACKET ) {
                Update_CheckToken();
                if ( g_check_token.type == SEMICOLON )
                  return true; // DO statement WHILE '(' expression ')' ';'
                else 
                  return Set_SyntacticalError() ; // Grammar error
              } // if 
              else 
                return Set_SyntacticalError() ; // Grammar error
            } // if
            else 
              return Set_SyntacticalError() ; // Grammar error
          } // if
          else 
            return Set_SyntacticalError() ; // Grammar error
        } // if
        else 
          return Set_SyntacticalError() ; // Grammar error
      } // if
      else {
        return Set_SyntacticalError();
      } // else
    } // if
    else
      return Set_SyntacticalError() ; // Grammar error
  } // else if
  else if ( Is_SystemCall() ) {
    InitToken( g_tempToken );
    return true;
  } // else if 
  
  return false;
} // Is_Statement()

bool Is_Special_Grammar() {
/*
          | CIN  { RS  expression } ';'
          | COUT { LS ( CONSTANT | identifier [ '[' ( CONSTANT | identifier ) ']' ]  }  ';'
*/
  bool notfind = false; 
  if ( g_check_token.type == CIN ) {
    Update_CheckToken();
    if ( g_check_token.type == RS ) {
      while ( g_check_token.type == RS ) {
        Update_CheckToken();
        if ( Is_expression() ) {
          ;
        } // if
        else {
          return Set_SyntacticalError();
        } // else
      } // while

      if ( g_check_token.type == SEMICOLON ) {
        return true;
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return false ; // Grammar error
  } // if 
  else if ( g_check_token.type == COUT ) {
    Update_CheckToken();
    if ( g_check_token.type == LS ) { // <<
      while ( g_check_token.type == LS ) {
        Update_CheckToken();
        if ( Is_expression() ) {
          ;
        } // if
        else {
          return Set_SyntacticalError();
        } // else
      } // while
      
      if ( g_check_token.type == SEMICOLON ) {
        return true;
      } // if
      else 
        return Set_SyntacticalError() ; // Grammar error
    } // if
    else 
      return false ; // Grammar error
  } // else if 


  return false;
} // Is_Special_Grammar()


bool Errors() {
  if ( g_lexical_error || g_syntactical_error || g_semantic_error )
    return true;
  return false;
} // Errors()

bool Grammar_Is_Ok() {
  Update_CheckToken();
  g_tempValue.clear();
  if ( !Errors() && Is_SystemCall() ) {
    Executed_SystemCall();
    InitToken( g_tempToken );
    return true;
  } // if
  else if ( !Errors() && Is_Definition() ) {
    cout << "> ";
    if ( g_definition_func ) { // 要先檢查是否為定義函數，否則在function裡面定義的函數判斷
                               // 會被先判斷
      int index = Find_func( g_Func_All_List, g_tempFunc.name );
      if ( index != -1 ) { // update function
        g_Func_All_List[index] = g_tempFunc;
        cout << "New definition of " << g_Func_All_List[index].name;
        cout << "() entered ..." << endl;
      } // if
      else { // 新增
        Insert_Func( g_tempFunc );
        cout << "Definition of " << g_tempFunc.name << "() entered ..." << endl;
      } // else
      
      return true;
    } // if
    else if ( g_definition_value ) { // add in list of value
      for ( int i = 0 ; i < g_tempValue.size() ; i++ ) {
        int index = Find_value( g_Value_All_List, g_tempValue[i].name );
        if ( index != -1 ) {
          // update value
          g_Value_All_List[index] = g_tempValue[i];
          cout << "New definition of " << g_Value_All_List[index].name << " entered ..." << endl;
        } // if
        else { // 新增
          Insert_value( g_tempValue[ i ] );
          cout << "Definition of " << g_tempValue[ i ].name << " entered ..." << endl;
        } // else
      } // for

      return true;
    } // else if
  } // else if
  else if ( !Errors() && Is_Statement() ) {
    // 在statement時檢查identifier是否有被存取過
    cout << "> Statement executed ..." << endl;
    return true;
  } // else if
  else { 
    return Set_SyntacticalError();
  } // else

  return false;
} // Grammar_Is_Ok()
// ========^^^ Grammar ^^^===============



// 找是否有相同token的Func找不到就回傳-1
int Find_func( vector<Func> list, string str ) {
  if ( list.size() == 0 )
    return -1;
  
  for ( int i = 0 ; i < list.size() ; i++ ) {
    if ( list[i].name == str ) {
      return i;
    } // if
  } // for

  return -1;
} // Find_func()



// 找是否有相同token的value找不到就回傳NULL
int Find_value( vector<Value> list, string str ) {
  if ( list.size() == 0 )
    return -1;
  
  for ( int i = 0 ; i < list.size() ; i++ ) {
    if ( list[i].name == str ) {
      return i;
    } // if
  } // for

  return -1;
} // Find_value()


void Insert_Func( Func func ) {
  if ( g_Func_All_List.size() == 0 ) {
    g_Func_All_List.push_back( func );
    return ;
  } // if

  bool end = false;
  for ( int i = 0 ; i < g_Func_All_List.size() && !end ; i++ ) {
    if ( g_Func_All_List[i].name > func.name ) {
      g_Func_All_List.insert( g_Func_All_List.begin() + i, func ) ;
      end = true;
    } // if
  } // for

  if ( !end ) 
    g_Func_All_List.push_back( func );
} // Insert_Func()


void Insert_value( Value value ) {
  if ( g_Value_All_List.size() == 0 ) {
    g_Value_All_List.push_back( value );
    return ;
  } // if

  bool end = false;
  for ( int i = 0 ; i < g_Value_All_List.size() && !end ; i++ ) {
    if ( g_Value_All_List[i].name > value.name ) {
      g_Value_All_List.insert( g_Value_All_List.begin() + i, value ) ;
      end = true;
    } // if
  } // for

  if ( !end ) 
    g_Value_All_List.push_back( value );
} // Insert_value()


void Print_ListAllFunctions() {
  for ( int i = 0 ; i < g_Func_All_List.size() ; i++ ) {
    cout << g_Func_All_List[i].name << "()" << endl;
  } // for


} // Print_ListAllFunctions()


void Print_ListAllVariables() {
  for ( int i = 0 ; i < g_Value_All_List.size() ; i++ ) {
    if ( g_Value_All_List[i].type != COUT && g_Value_All_List[i].type != CIN )
      cout << g_Value_All_List[i].name << endl;
  } // for
} // Print_ListAllVariables()




void Print_parameter( int index ) {
  for ( int i = 0 ; i < g_Func_All_List[index].values.size() ; i++ ) { 
    if ( g_Func_All_List[index].values[i].type == IDENTIFIER && 
         i + 1 < g_Func_All_List[index].values.size() && 
         ( g_Func_All_List[index].values[i +1].type == MLBRACKET || 
           g_Func_All_List[index].values[i +1].type == PP ||
           g_Func_All_List[index].values[i +1].type == MM ||
           g_Func_All_List[index].values[i +1].type == COMMA ) ) {
      cout << g_Func_All_List[index].values[i].token;
    } // if
    else {
      cout << g_Func_All_List[index].values[i].token << " ";
    } // else
  } // for
} // Print_parameter()


// special case for 
// 如果identifier 後面接著 '[' 
// 不用印空白
// do while 要另外寫
void Print_compound( int level, int i, int &index ) {
  cout << g_Func_All_List[i].tokens[index].token << endl;
  index += 1;
  while ( g_Func_All_List[i].tokens[index].type != LRBRACKET ) {
    Print_statement( level +2, i, index ) ;
  } // while

  for ( int j = 0 ; j < level ; j++ ) {
    cout << " " ;
  } // for

  cout << "}\n" ;
  index += 1;
} // Print_compound()


void Print_statement( int level, int i, int &index ) {
  for ( int i = 0 ; i < level ; i++ ) {
    cout << " ";
  } // for

  if ( g_Func_All_List[i].tokens[index].type == IF ) {
    stack<string> bracket;
    cout << g_Func_All_List[i].tokens[index].token << " " ; // IF
    index = index + 1;
    cout << g_Func_All_List[i].tokens[index].token << " " ; // '( '
    if ( g_Func_All_List[i].tokens[index].type == SLBRACKET ) {
      bracket.push( g_Func_All_List[i].tokens[index].token ) ;
    } // if

    index = index + 1 ;
    while ( ! bracket.empty() ) {
      if ( g_Func_All_List[i].tokens[index].type == SLBRACKET ) {
        bracket.push( g_Func_All_List[i].tokens[index].token ) ;
      } // if 
      else if ( g_Func_All_List[i].tokens[index].type == SRBRACKET ) {
        bracket.pop();
      } // else if

      if ( g_Func_All_List[i].tokens[index].type == IDENTIFIER && 
           ( g_Func_All_List[i].tokens[index +1].type == MLBRACKET || 
             g_Func_All_List[i].tokens[index +1].type == PP ||
             g_Func_All_List[i].tokens[index +1].type == MM ||
             g_Func_All_List[i].tokens[index +1].type == SAND || 
             g_Func_All_List[i].tokens[index +1].type == SLBRACKET ) ) {
        cout << g_Func_All_List[i].tokens[index].token ;
      } // if
      else if ( g_Func_All_List[i].tokens[index].type == LISTALLFUNCTIONS || 
                g_Func_All_List[i].tokens[index].type == LISTALLVARIABLES || 
                g_Func_All_List[i].tokens[index].type == LISTFUNCTION || 
                g_Func_All_List[i].tokens[index].type == LISTVARIABLE ) {
        cout << g_Func_All_List[i].tokens[index].token ;
      } // else if 
      else {
        if ( bracket.size() == 0 ) {
          cout << g_Func_All_List[i].tokens[index].token ;
        } // if
        else {
          cout << g_Func_All_List[i].tokens[index].token  << " " ;
        } // else
      } // else

      index = index + 1 ;
    } // while

    if ( g_Func_All_List[i].tokens[index].type == LLBRACKET ) {
      cout << " " ;
      Print_compound( level, i, index );
    } // if
    else {
      cout << endl;
      Print_statement( level + 2, i, index );
    } // else
    
    if ( g_Func_All_List[i].tokens[index].type == ELSE ) {
      for ( int i = 0 ; i < level ; i++ ) {
        cout << " " ;
      } // for

      cout << g_Func_All_List[i].tokens[index].token ;
      index = index +1 ;
      if ( g_Func_All_List[i].tokens[index].type == LLBRACKET ) {
        cout << " " ;
        Print_compound( level, i, index );
      } // if
      else {
        Print_statement( level + 2, i, index );
      } // else
    } // if    ELSE
  } // if  IF
  else if ( g_Func_All_List[i].tokens[index].type == DO ) {
    cout << g_Func_All_List[i].tokens[index].token << " " ;
    index = index + 1 ;
    if ( g_Func_All_List[i].tokens[index].type == LLBRACKET ) {
      Print_compound( level, i, index );
      // printcompound 會印出換行
      // 要重新印多個空格
      for ( int i = 0 ; i < level ; i++ ) {
        cout << " " ;
      } // for

      // 印出 while ();
      while ( g_Func_All_List[i].tokens[index].type != SEMICOLON ) {
        if ( g_Func_All_List[i].tokens[index].type == IDENTIFIER && 
             ( g_Func_All_List[i].tokens[index +1].type == MLBRACKET || 
               g_Func_All_List[i].tokens[index +1].type == PP ||
               g_Func_All_List[i].tokens[index +1].type == MM ||
               g_Func_All_List[i].tokens[index +1].type == SAND ||
               g_Func_All_List[i].tokens[index +1].type == SLBRACKET ) ) {
          cout << g_Func_All_List[i].tokens[index].token ;
        } // if
        else if ( g_Func_All_List[i].tokens[index].type == LISTALLFUNCTIONS || 
                  g_Func_All_List[i].tokens[index].type == LISTALLVARIABLES || 
                  g_Func_All_List[i].tokens[index].type == LISTFUNCTION || 
                  g_Func_All_List[i].tokens[index].type == LISTVARIABLE ) {
          cout << g_Func_All_List[i].tokens[index].token ;
        } // else if 
        else {
          cout << g_Func_All_List[i].tokens[index].token  << " " ;
        } // else

        index = index + 1;
      } // while

      cout << g_Func_All_List[i].tokens[index].token << "\n" ;
      index = index + 1;
    } // if
  } // else if   DO WHILE
  else if ( g_Func_All_List[i].tokens[index].type == WHILE ) {
    stack<string> bracket;
    cout << g_Func_All_List[i].tokens[index].token << " " ; // WHILE
    index = index + 1;
    cout << g_Func_All_List[i].tokens[index].token << " " ; // '( '
    if ( g_Func_All_List[i].tokens[index].type == SLBRACKET ) {
      bracket.push( g_Func_All_List[i].tokens[index].token ) ;
    } // if

    index = index + 1 ;
    while ( ! bracket.empty() ) {
      if ( g_Func_All_List[i].tokens[index].type == SLBRACKET ) {
        bracket.push( g_Func_All_List[i].tokens[index].token ) ;
      } // if 
      else if ( g_Func_All_List[i].tokens[index].type == SRBRACKET ) {
        bracket.pop();
      } // else if

      if ( g_Func_All_List[i].tokens[index].type == IDENTIFIER && 
           ( g_Func_All_List[i].tokens[index +1].type == MLBRACKET || 
             g_Func_All_List[i].tokens[index +1].type == PP ||
             g_Func_All_List[i].tokens[index +1].type == MM ||
             g_Func_All_List[i].tokens[index +1].type == SAND || 
             g_Func_All_List[i].tokens[index +1].type == SLBRACKET ) ) {
        cout << g_Func_All_List[i].tokens[index].token ;
      } // if
      else if ( g_Func_All_List[i].tokens[index].type == LISTALLFUNCTIONS || 
                g_Func_All_List[i].tokens[index].type == LISTALLVARIABLES || 
                g_Func_All_List[i].tokens[index].type == LISTFUNCTION || 
                g_Func_All_List[i].tokens[index].type == LISTVARIABLE ) {
        cout << g_Func_All_List[i].tokens[index].token ;
      } // else if 
      else {
        if ( bracket.size() == 0 ) {
          cout << g_Func_All_List[i].tokens[index].token ;
        } // if
        else {
          cout << g_Func_All_List[i].tokens[index].token  << " " ;
        } // else
      } // else

      index = index + 1 ;
    } // while

    if ( g_Func_All_List[i].tokens[index].type == LLBRACKET ) {
      cout << " ";
      Print_compound( level, i, index );
    } // if
    else {
      cout << endl;
      Print_statement( level + 2, i, index );
    } // else
  } // else if  WHILE
  else if ( g_Func_All_List[i].tokens[index].type == LLBRACKET ) {
    Print_compound( level, i, index );
  } // else if '{' 
  else {
    while ( g_Func_All_List[i].tokens[index].type != SEMICOLON ) {
      if ( g_Func_All_List[i].tokens[index].type == IDENTIFIER && 
           ( g_Func_All_List[i].tokens[index +1].type == MLBRACKET || 
             g_Func_All_List[i].tokens[index +1].type == PP ||
             g_Func_All_List[i].tokens[index +1].type == MM ||
             g_Func_All_List[i].tokens[index +1].type == SLBRACKET || 
             g_Func_All_List[i].tokens[index +1].type == COMMA ) ) {
        cout << g_Func_All_List[i].tokens[index].token ;
      } // if
      else if ( g_Func_All_List[i].tokens[index].type == LISTALLFUNCTIONS || 
                g_Func_All_List[i].tokens[index].type == LISTALLVARIABLES || 
                g_Func_All_List[i].tokens[index].type == LISTFUNCTION || 
                g_Func_All_List[i].tokens[index].type == LISTVARIABLE ) {
        cout << g_Func_All_List[i].tokens[index].token ;
      } // else if 
      else {
        cout << g_Func_All_List[i].tokens[index].token  << " " ;
      } // else

      index = index +1 ;
    } // while
    
    // ';'
    cout << g_Func_All_List[i].tokens[index].token ;
    cout << "\n";
    index = index + 1 ;
  } // else  statement
} // Print_statement()

void Print_Type( Type type ) {
  if ( type == CIN )    cout << "cin";
  else if ( type == COUT )    cout << "cout" ;
  else if ( type == INT )     cout << "int";
  else if ( type == FLOAT )    cout << "float";
  else if ( type == CHAR )    cout << "char";
  else if ( type == BOOL )    cout << "bool";
  else if ( type == STRING )    cout << "string";
  else if ( type == VOID )     cout << "void";
  else if ( type == IF )     cout << "if";
  else if ( type == ELSE )    cout << "else";
  else if ( type == WHILE )     cout << "while";
  else if ( type == DO )    cout << "do";
  else if ( type == RETURN )    cout << "return";
  else if ( type == LISTALLVARIABLES )    cout << "listallvariables" ;
  else if ( type == LISTVARIABLE )    cout << "listvariable";
  else if ( type == LISTALLFUNCTIONS )     cout << "listallfunction";
  else if ( type == LISTFUNCTION )    cout << "listfunction" ;
  else if ( type == DONE )     cout << "done";
  else if ( type == CONSTANT )    cout << "constant" ;
  else if ( type == IDENTIFIER ) cout << "identifier";
  return ;
} // Print_Type()



// 先檢查是哪一種error
// 在去print error message
void PrintErrorMessage() {
  if ( g_lexical_error && g_check_token.type == ERROR ) {
    cout << "> Line " ;
    if ( g_check_token.line == 0 ) 
      cout << g_check_token.line +1;
    else 
      cout << g_check_token.line;

    cout << " unrecognized token with first char : '";
    cout << g_check_token.token[0];
    cout << "'" << endl;
  } // if
  else if ( g_semantic_error ) {
    cout << "> Line " ;
    if ( g_check_token.line == 0 ) 
      cout << g_check_token.line +1;
    else 
      cout << g_check_token.line;
    cout << " : undefined identifier : '" << g_check_token.token << "'" << endl;
  } // else if
  else if ( g_syntactical_error ) {
    cout << "> Line " ;
    if ( g_check_token.line == 0 ) 
      cout << g_check_token.line +1;
    else 
      cout << g_check_token.line;
    cout << " : unexpected token : '" << g_check_token.token << "'" << endl;
  } // else if
  
} // PrintErrorMessage()


void Executed_SystemCall() {
  if ( g_tempToken.type == LISTALLVARIABLES ) {
    // 印出所有變數
    cout << "> " ;
    if ( g_tempToken.token == "" )
      Print_ListAllVariables();
    cout << "Statement executed ..." << endl;
  } // if
  else if ( g_tempToken.type == LISTVARIABLE ) {
    // 印出指定變數
    cout << "> ";
    string str = g_tempToken.token;

    if ( str.length() < 2 ) {
      cout << "Statement executed ..." << endl;
      return ;
    } // if
    
    str.erase( str.begin() );
    str.erase( str.end() -1 );
    int index = Find_value( g_Value_All_List, str );
    if ( index != -1 && ( str != "cin" && str != "cout" ) ) {
      Print_Type( g_Value_All_List[index].type );
      cout << " " << g_Value_All_List[index].name ;

      // 如果有array就要印出
      if ( g_Value_All_List[index].array != "" ) {
        cout << "[ " << g_Value_All_List[index].array << " ]";
      } // if

      cout << " ;" << endl;
    } // if

    cout << "Statement executed ..." << endl;
  } // else if
  else if ( g_tempToken.type == LISTALLFUNCTIONS ) {
    // 印出所以Function name
    cout << "> " ;
    if ( g_tempToken.token == "" )
      Print_ListAllFunctions();
    cout << "Statement executed ..." << endl;
  } // else if
  else if ( g_tempToken.type == LISTFUNCTION ) {
    cout << "> " ;
    string str = g_tempToken.token;
    int i = 0;
    str.erase( str.begin() );
    str.erase( str.end() -1 );
    int index = Find_func( g_Func_All_List, str ) ;
    if ( index != -1 ) {
      // 印type
      Print_Type( g_Func_All_List[index].type );
      cout << " ";
      // 印名字
      cout << g_Func_All_List[index].name ;
      cout << "(" ;

      if ( g_Func_All_List[index].values.size() != 0 )
        cout << " ";
      // 印參數會幫忙把後面的空格印出來
      Print_parameter( index ) ;
      cout << ") " ;
      // 印內容
      Print_compound( 0, index, i ); 
    } // if
    
    cout << "Statement executed ..." << endl;
  } // else if
  else if ( g_tempToken.type == DONE ) {
    g_executed = false;
  } // else if

  InitToken( g_tempToken );
} // Executed_SystemCall()

bool Is_SystemCall() {
  // system call '(' [ actual_parameter_list ] ')' ;
  // 把 actual_parameter_list 加上去
  
  Token atoken ;
  atoken.type = NOTHING ;
  atoken.token = "";
  atoken.line = 0 ; 
  if ( g_check_token.type == LISTALLVARIABLES || 
       g_check_token.type == LISTALLFUNCTIONS || 
       g_check_token.type == DONE ) {
    atoken.type = g_check_token.type ;
    Update_CheckToken();
    if ( g_check_token.type == SLBRACKET ) {
      Update_CheckToken();
      if ( g_check_token.type == SRBRACKET ) {
        Update_CheckToken();
        if ( g_check_token.type == SEMICOLON ) {
          g_tempToken = atoken ;
          return true;
        } // if
        else {
          return Set_SyntacticalError();
        } // else
      } // if
      else {
        return Set_SyntacticalError();
      } // else
    } // if
    else {
      return Set_SyntacticalError();
    } // else
  } // if
  else if ( g_check_token.type == LISTFUNCTION || 
            g_check_token.type == LISTVARIABLE ) {          
    atoken.type = g_check_token.type ;
    Update_CheckToken();
    if ( g_check_token.type == SLBRACKET ) {
      Update_CheckToken();
      if ( g_check_token.type == CONSTANT ) {    
        atoken.token = g_check_token.token ;
        Update_CheckToken() ;
        if ( g_check_token.type == SRBRACKET ) {
          Update_CheckToken();
          if ( g_check_token.type == SEMICOLON ) {
            g_tempToken = atoken ;
            return true ;
          } // if
          else {
            return Set_SyntacticalError();
          } // else
        } // if
        else {
          return Set_SyntacticalError();
        } // else
      } // if
      else {
        return Set_SyntacticalError();
      } // else
    } // if
    else {
      return Set_SyntacticalError();
    } // else
  } // else if
  else {
    return false;
  } // else
} // Is_SystemCall()


// 當發生錯誤時 有需要就把後面的tokens 全部讀掉
// 讀到換行符號，但沒有把換行也讀掉
void Clear_a_line() {
  char c = '\0';
  c = cin.peek();
  while ( c != '\n' ) {
    c = cin.get();
    c = cin.peek();
  } // while
} // Clear_a_line()

void Init_Error_bool() {
  g_lexical_error = false;
  g_syntactical_error = false;
  g_semantic_error = false;
} // Init_Error_bool();

void InitToken( Token &token ) {
  token.token = "";
  token.type = NOTHING;
  token.line = 1;
} // InitToken()  


void SkipWhiteSpace() {
  char c = cin.peek();
  while ( c == '\n' || c == ' ' || c == '\t' ) {
    c = cin.get();
    c = cin.peek();
  } // while
} // SkipWhiteSpace()


bool Set_SyntacticalError() {
  g_syntactical_error = true;
  return false;
} // Set_SyntacticalError()


// 在最後一行的註解
// 會被算進去next Grammar 的行數
int main() {
  int i = 0 ;
  char c = '\0';
  scanf( "%d", &i ) ;
  cout << "Our-C running ..." << endl;
  InitToken( g_tempToken );
  InitToken( g_check_token );
  SkipWhiteSpace();
  Value v_cin, v_cout ;
  v_cin.name = "cin";
  v_cin.type = CIN;
  v_cout.name = "cout";
  v_cout.type = COUT;
  g_Value_All_List.push_back( v_cin );
  g_Value_All_List.push_back( v_cout );

  g_int = i;
  while ( g_executed ) {
    if ( Grammar_Is_Ok() ) {
      ;
    } // if
    else {
      if ( g_tempToken.line == g_check_token.line ) {
        InitToken( g_tempToken );
        Clear_a_line();
      } // if
      else {
        g_tempToken.line -= g_check_token.line;
      } // else 
      
      if ( g_tempToken.type == NOTHING ) {
        Clear_a_line();
      } // if

      PrintErrorMessage();
      Init_Error_bool();
    } // else
    
    g_token_line = 0;
  } // while
  
  cout << "> Our-C exited ...";
} // main()