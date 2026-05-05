# include <iostream>
# include <sstream>
# include <string>
# include <cstring>
# include <vector>

using namespace std;

enum TokenType {
    IDENT,
    CONST,     // ex: 35, 35.67, 'a', "Hi, there", true, false, .35, 35., 0023
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
    NE,        // '!='
    BIT_AND,   // '&'
    BIT_OR,    // '|'
    XOR,       // '^'
    ASSIGN,    // '='
    NOT,       // '!'
    AND,       // '&&'
    OR,        // '||'
    PA,        // '+=' plus assign
    SA,        // '-=' sub assign
    MA,        // '*=' mul assign
    DA,        // '/=' div assign
    OA,        // '%=' 取MOD中的'O' mod assign
    PP,        // '++' 
    SS,        // '--'
    RS,        // '>>'
    LS,        // '<<'
    SEMICOLON, // ';'
    COMMA,     // ','
    CO,        // conditional operator '?'
    COC,       // conditional operator ':' 冒號英文為COLON
    ERROR,     // 表示這個token為unrecgonizeTokenWithFirstChar
    NOTHING
};


enum CommandType {
    DONE,    // Done()
    LAV,     // ListAllVariables()
    LAF,     // ListAllFunctions()
    LV,      // listVariable( char name[] )
    LF,      // ListFunction( char name[] )
    FUN_DEF, // function definition
    ID_DEF,  // ident definition
    STAT,    // statement
    NO
};


struct Token {
    string tokenValue;
    TokenType tokenType;
};


struct Var {
    string varName;
    TokenType varType;
    string arrayNum;
};


struct Function {
    string functionName;
    TokenType functType;
    vector <Var> localVars;
    vector <Var> parameters;
    vector <Token> instrs;
};

// 初始化token
void ClearToken(Token& token) {
    token.tokenValue = "";
    token.tokenType = NOTHING;
} // ClearToken()


void ClearVar(Var& var) {
    var.arrayNum = "";
    var.varName = "";
    var.varType = NOTHING;
} // ClearVar()


void ClearFunction(Function& funct) {
    funct.functionName = "";
    funct.functType = NOTHING;
    funct.instrs.clear();
    funct.localVars.clear();
    funct.parameters.clear();
} // ClearFunction()

// 輸出token的tokenType
void PrintTokenType(Token token) {
    if (token.tokenType == IDENT) cout << "tokenType: IDENT" << endl;
    else if (token.tokenType == CONST) cout << "tokenType: CONST" << endl;
    else if (token.tokenType == INT) cout << "tokenType: INT" << endl;
    else if (token.tokenType == FLOAT) cout << "tokenType: FLOAT" << endl;
    else if (token.tokenType == CHAR) cout << "tokenType: CHAR" << endl;
    else if (token.tokenType == BOOL) cout << "tokenType: BOOL" << endl;
    else if (token.tokenType == STRING) cout << "tokenType: STRING" << endl;
    else if (token.tokenType == VOID) cout << "tokenType: VOID" << endl;
    else if (token.tokenType == IF) cout << "tokenType: IF" << endl;
    else if (token.tokenType == ELSE) cout << "tokenType: ELSE" << endl;
    else if (token.tokenType == WHILE) cout << "tokenType: WHILE" << endl;
    else if (token.tokenType == DO) cout << "tokenType: DO" << endl;
    else if (token.tokenType == RETURN) cout << "tokenType: RETURN" << endl;
    else if (token.tokenType == SLBRACKET) cout << "tokenType: SLBRACKET" << endl;
    else if (token.tokenType == SRBRACKET) cout << "tokenType: SRBRACKET" << endl;
    else if (token.tokenType == MLBRACKET) cout << "tokenType: MLBRACKET" << endl;
    else if (token.tokenType == MRBRACKET) cout << "tokenType: MRBRACKET" << endl;
    else if (token.tokenType == LLBRACKET) cout << "tokenType: LLBRACKET" << endl;
    else if (token.tokenType == LRBRACKET) cout << "tokenType: LRBRACKET" << endl;
    else if (token.tokenType == PLUS) cout << "tokenType: PLUS" << endl;
    else if (token.tokenType == SUB) cout << "tokenType: SUB" << endl;
    else if (token.tokenType == MUL) cout << "tokenType: MUL" << endl;
    else if (token.tokenType == DIV) cout << "tokenType: DIV" << endl;
    else if (token.tokenType == MOD) cout << "tokenType: MOD" << endl;
    else if (token.tokenType == GT) cout << "tokenType: GT" << endl;
    else if (token.tokenType == LT) cout << "tokenType: LT" << endl;
    else if (token.tokenType == GE) cout << "tokenType: GE" << endl;
    else if (token.tokenType == LE) cout << "tokenType: LE" << endl;
    else if (token.tokenType == EQ) cout << "tokenType: EQ" << endl;
    else if (token.tokenType == NE) cout << "tokenType: NE" << endl;
    else if (token.tokenType == BIT_AND) cout << "tokenType: BIT_AND" << endl;
    else if (token.tokenType == BIT_OR) cout << "tokenType: BIT_OR" << endl;
    else if (token.tokenType == XOR) cout << "tokenType: XOR" << endl;
    else if (token.tokenType == ASSIGN) cout << "tokenType: ASSIGN" << endl;
    else if (token.tokenType == NOT) cout << "tokenType: NOT" << endl;
    else if (token.tokenType == AND) cout << "tokenType: AND" << endl;
    else if (token.tokenType == OR) cout << "tokenType: OR" << endl;
    else if (token.tokenType == PA) cout << "tokenType: PA" << endl;
    else if (token.tokenType == SA) cout << "tokenType: SA" << endl;
    else if (token.tokenType == MA) cout << "tokenType: MA" << endl;
    else if (token.tokenType == DA) cout << "tokenType: DA" << endl;
    else if (token.tokenType == OA) cout << "tokenType: OA" << endl;
    else if (token.tokenType == PP) cout << "tokenType: PP" << endl;
    else if (token.tokenType == SS) cout << "tokenType: SS" << endl;
    else if (token.tokenType == RS) cout << "tokenType: RS" << endl;
    else if (token.tokenType == LS) cout << "tokenType: LS" << endl;
    else if (token.tokenType == SEMICOLON) cout << "tokenType: SEMICOLON" << endl;
    else if (token.tokenType == COMMA) cout << "tokenType: COMMA" << endl;
    else if (token.tokenType == CO) cout << "tokenType: CO" << endl;
    else if (token.tokenType == COC) cout << "tokenType: COC" << endl;
    else cout << "tokenType: error" << endl;
} // PrintTokenType()


class Scanner {
public:
    Token mToken;       // scanner讀取到的token
    bool mError;        // 紀錄token是否出錯(unrecgonizeTokenWithFirstChar)
    char mErrorToken;   // 紀錄出錯的token
    int mLine;          // 紀錄scanner讀到一個指令的第幾行
    bool mTokenStarted; // 是否讀到第一個token 若有回傳true 否則false 

    Scanner();

    // 從input中拿出一個token
    void GetToken() {
        Initialize();
        SkipWhiteSpace();
        mTokenStarted = true;
        // 讀取token的第一個字元
        char ch = '\0';
        ch = cin.peek();

        // 依照ch來找token
        // ID constant reservedWord
        if (IsLetter(ch) || IsDigit(ch) || ch == '\'' || ch == '\"' || ch == '.') {
            SearchMatchedToken(ch);
        } // if
        // 括號
        else if (ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}') {
            SearchMatchedToken(ch);
        } // else if
        // 計算operator和以這些開頭的token
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            SearchMatchedToken(ch);
        } // else if
        // 比較operator和以這些開頭的token
        else if (ch == '>' || ch == '<' || ch == '=' || ch == '!' || ch == '&' || ch == '|' || ch == '^') {
            SearchMatchedToken(ch);
        } // else if
        // 其他
        else if (ch == ';' || ch == ',' || ch == '?' || ch == ':') {
            SearchMatchedToken(ch);
        } // else if
        else {
            mError = true;
            mErrorToken = ch;
        } // else
        /*
        if ( mError ) cout << "error token: " << mErrorToken << endl ;
        else if ( mToken.tokenType != NOTHING ) cout << "token: " << mToken.tokenValue << endl ;
        PrintTokenType( mToken ) ;
        */

    } // GetToken()

    // 清除一整行的input
    void ClearInput() {
        char ch = '\0';
        while (cin.peek() != '\n') {
            ch = cin.get();
        } // while

        ch = cin.get();
        if (ch == '\n' && !mTokenStarted) mTokenStarted = true;
        else if (ch == '\n' && mTokenStarted) mLine++;
    } // ClearInput()

    // 去除whiteSpace
    void SkipWhiteSpace() {
        char ch = '\0';
        while (IsWhiteSpace(cin.peek()) || cin.peek() == '/') {
            ch = cin.get();

            // 遇到comment要跳過 
            if (ch == '/' && cin.peek() == '/') {
                ClearInput();
            } // if
            // 如果只是'/' 把字元放回去
            else if (ch == '/' && cin.peek() != '/') {
                cin.putback(ch);
                return;
            } // else if

            if (ch == '\n' && !mTokenStarted) mTokenStarted = true;
            else if (ch == '\n' && mTokenStarted) mLine++;
        } // while
    } // SkipWhiteSpace()

private:
    // 初始化scanner中所有變數
    void Initialize() {
        ClearToken(mToken);
        mError = false;
        mErrorToken = '\0';
    } // Initialize()

    // 根據第一個字元來取得token
    void SearchMatchedToken(char ch) {
        int i = 0;

        i = 0;
        if (IsLetter(ch)) {
            while (IsLetter(cin.peek()) || IsDigit(cin.peek()) || cin.peek() == '_') {
                mToken.tokenValue += cin.get();
                i++;
            } // while

            if (mToken.tokenValue == "int") mToken.tokenType = INT;
            else if (mToken.tokenValue == "float") mToken.tokenType = FLOAT;
            else if (mToken.tokenValue == "char") mToken.tokenType = CHAR;
            else if (mToken.tokenValue == "bool") mToken.tokenType = BOOL;
            else if (mToken.tokenValue == "string") mToken.tokenType = STRING;
            else if (mToken.tokenValue == "void") mToken.tokenType = VOID;
            else if (mToken.tokenValue == "if") mToken.tokenType = IF;
            else if (mToken.tokenValue == "else") mToken.tokenType = ELSE;
            else if (mToken.tokenValue == "while") mToken.tokenType = WHILE;
            else if (mToken.tokenValue == "do") mToken.tokenType = DO;
            else if (mToken.tokenValue == "return") mToken.tokenType = RETURN;
            else if (mToken.tokenValue == "true") mToken.tokenType = CONST;
            else if (mToken.tokenValue == "false") mToken.tokenType = CONST;
            else mToken.tokenType = IDENT;
        } // if
        else if (IsDigit(ch) || ch == '.') {
            bool hasDot = false;
            while (IsDigit(cin.peek()) || (cin.peek() == '.' && !hasDot)) {
                mToken.tokenValue += cin.get();
                if (mToken.tokenValue[i] == '.') hasDot = true;
                i++;
            } // while

            // "."不是token 是error 
            if (mToken.tokenValue == ".") {
                mError = true;
                mErrorToken = '.';
                mToken.tokenType = ERROR;
            } // if
            else mToken.tokenType = CONST;
        } // else if
        else if (ch == '\'') {
            mToken.tokenValue += cin.get();
            i++;
            while (cin.peek() != '\'' && cin.peek() != '\n') {
                mToken.tokenValue += cin.get();
                i++;
            } // while

            int j = 0;
            char charValue[2];
            while (j < i) {
                if (j < 2) charValue[j] = mToken.tokenValue[j + 1];
                j++;
            } // while

            // 只有一個引號的狀況 ( ' ) 
            if (i == 1) {
                mError = true;
                mErrorToken = '\'';
                mToken.tokenType = ERROR;
            } // if
            // 引號之間有2個字元且不是特殊字元的狀況 ( 'ab', 'ff' )
            else if (i == 3 && !IsSpecialChar(charValue)) {
                mError = true;
                mErrorToken = '\'';
                mToken.tokenType = ERROR;
            } // else if
            // 引號之間有三個或以上個字元的狀況 ( 'abc', 'hello world' )
            else if (i > 3) {
                mError = true;
                mErrorToken = '\'';
                mToken.tokenType = ERROR;
            }  // else if
            else {
                if (cin.peek() == '\'') mToken.tokenValue += cin.get();
                // 只有一個引號和字元的情況 ( 'a, '\n )
                if (mToken.tokenValue[i] != '\'') {
                    mError = true;
                    mErrorToken = '\'';
                    mToken.tokenType = ERROR;
                } // if
                else mToken.tokenType = CONST;
            } // else      
        } // else if
        else if (ch == '\"') {
            mToken.tokenValue += cin.get();
            i++;
            while (cin.peek() != '\"' && cin.peek() != '\n') {
                mToken.tokenValue += cin.get();
                i++;
            } // while 

            if (cin.peek() == '\"') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = CONST;
            } // if
            else if (cin.peek() == '\n') {
                mError = true;
                mErrorToken = '\"';
                mToken.tokenType = ERROR;
            } // else if
        } // else if
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
            ch == '>' || ch == '<' || ch == '=' || ch == '!') {
            mToken.tokenValue += cin.get();
            i++;

            if (ch == '+' && cin.peek() == '+') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = PP;
            } // if
            else if (ch == '-' && cin.peek() == '-') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = SS;
            } // else if
            else if (ch == '>' && cin.peek() == '>') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = RS;
            } // else if
            else if (ch == '<' && cin.peek() == '<') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = LS;
            } // else if
            else if (cin.peek() == '=') {
                mToken.tokenValue += cin.get();

                if (ch == '+') mToken.tokenType = PA;
                if (ch == '-') mToken.tokenType = SA;
                if (ch == '*') mToken.tokenType = MA;
                if (ch == '/') mToken.tokenType = DA;
                if (ch == '%') mToken.tokenType = OA;
                if (ch == '>') mToken.tokenType = GE;
                if (ch == '<') mToken.tokenType = LE;
                if (ch == '=') mToken.tokenType = EQ;
                if (ch == '!') mToken.tokenType = NE;
            } // else if
            else {
                if (ch == '+') mToken.tokenType = PLUS;
                if (ch == '-') mToken.tokenType = SUB;
                if (ch == '*') mToken.tokenType = MUL;
                if (ch == '/') mToken.tokenType = DIV;
                if (ch == '%') mToken.tokenType = MOD;
                if (ch == '>') mToken.tokenType = GT;
                if (ch == '<') mToken.tokenType = LT;
                if (ch == '=') mToken.tokenType = ASSIGN;
                if (ch == '!') mToken.tokenType = NOT;
            } // else
        } // else if
        else if (ch == '&') {
            mToken.tokenValue += cin.get();
            i++;
            if (cin.peek() == '&') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = AND;
            } // if
            else mToken.tokenType = BIT_AND;
        } // else if 
        else if (ch == '|') {
            mToken.tokenValue += cin.get();
            i++;
            if (cin.peek() == '|') {
                mToken.tokenValue += cin.get();
                mToken.tokenType = OR;
            } // if
            else mToken.tokenType = BIT_OR;
        } // else if 
        else {
            mToken.tokenValue += cin.get();
            if (ch == '(') mToken.tokenType = SLBRACKET;
            if (ch == ')') mToken.tokenType = SRBRACKET;
            if (ch == '[') mToken.tokenType = MLBRACKET;
            if (ch == ']') mToken.tokenType = MRBRACKET;
            if (ch == '{') mToken.tokenType = LLBRACKET;
            if (ch == '}') mToken.tokenType = LRBRACKET;
            if (ch == '^') mToken.tokenType = XOR;
            if (ch == ';') mToken.tokenType = SEMICOLON;
            if (ch == ',') mToken.tokenType = COMMA;
            if (ch == '?') mToken.tokenType = CO;
            if (ch == ':') mToken.tokenType = COC;
        } // else

    } // SearchMatchedToken()

    // 若ch為空白字元 回傳true 否則回傳false
    bool IsWhiteSpace(char ch) {
        if (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\0') return true;
        else return false;
    } // IsWhiteSpace()

    // 若ch為字母 回傳true 否則回傳false
    bool IsLetter(char ch) {
        if (ch >= 'A' && ch <= 'Z') return true;
        else if (ch >= 'a' && ch <= 'z') return true;
        else return false;
    } // IsLetter()

    // 若ch為數字 回傳true 否則回傳false
    bool IsDigit(char ch) {
        if (ch >= '0' && ch <= '9') return true;
        else return false;
    } // IsDigit()

    // 若charValue為"\n" "\t" "\0" "\'" "\"" "\\" 就回傳true 否則回傳flase 
    bool IsSpecialChar(char charValue[2]) {
        if (charValue[0] == '\\' && charValue[1] == 'n') return true;
        else if (charValue[0] == '\\' && charValue[1] == 't') return true;
        else if (charValue[0] == '\\' && charValue[1] == '0') return true;
        else if (charValue[0] == '\\' && charValue[1] == '\'') return true;
        else if (charValue[0] == '\\' && charValue[1] == '\"') return true;
        else if (charValue[0] == '\\' && charValue[1] == '\\') return true;
        else return false;
    } // IsSpecialChar()

}; // Scanner()


Scanner::Scanner() {
    ClearToken(mToken);
    mError = false;
    mErrorToken = '\0';
    mLine = 1;
    mTokenStarted = true;
} // Scanner::Scanner()


class Parser {
public:
    Scanner mScanner;
    Token mCurrentToken; // 用來暫存一個token
    Token mParsingToken; // parser正要檢查的那個token
    vector <Token> mTokens;
    vector <Var> mVars;
    vector <Var> mTempLocalVars; // 暫存localVariable
    vector < vector<Var> > mVarsInBracket;
    vector <Function> mFunctions;
    Function mTempFunct;
    CommandType mCommand;
    bool mVarsIsInBracket;
    bool mIsFunction;
    bool mSyntaxError; // 文法錯誤
    bool mSemanticError; // 語意錯誤 e.g. 'abc'
    bool mIsFunctVar;
    Token mErrorToken;
    string mListedIdent; // ListVariable()和ListFunction()需要的參數

    Parser();

    // 檢查一道指令的文法是否正確 
    bool IsRightGrammer() {
        Initialize();

        TakeLatestToken();
        if (Is_SystemCall()) return true;
        else if (NoError() && Is_Definition()) return true;
        else if (NoError() && Is_Statement()) return true;
        else if (!mSemanticError) return SetSyntaxError();
        else return SetSemanticError();
    } // IsRightGrammer()

    // 輸出錯誤提示 
    void PrintError() {
        if (mScanner.mError) {
            cout << "> Line " << mScanner.mLine;
            cout << " : unrecognized token with first char : '" << mScanner.mErrorToken << "'" << endl;
        } // if
        // undefine ident要放在unexpect token前 因為會發生undefine ident就必定不發生unexpect token
        else if (mSemanticError) {
            cout << "> Line " << mScanner.mLine;
            cout << " : undefined identifier : '" << mParsingToken.tokenValue << "'" << endl;
        } // else if
        else if (mSyntaxError) {
            cout << "> Line " << mScanner.mLine;
            cout << " : unexpected token : '" << mParsingToken.tokenValue << "'" << endl;
        } // else if
    } // PrintError()

    // 列出所有variables 
    void ListAllVars() {
        int i = 0;
        int j = 1;
        Var temp;
        while (i < mVars.size()) {
            while (j < mVars.size()) {
                if (mVars[i].varName > mVars[j].varName) {
                    temp = mVars[i];
                    mVars[i] = mVars[j];
                    mVars[j] = temp;
                } // if

                j++;
            } // while

            i++;
            j = i + 1;
        } // while

        i = 0;
        while (i < mVars.size()) {
            cout << mVars[i].varName << endl;
            i++;
        } // while
    } // ListAllVars()

    // 列出所有functions 
    void ListAllFuncts() {
        int i = 0;
        int j = 1;
        Function temp;
        while (i < mFunctions.size()) {
            while (j < mFunctions.size()) {
                if (mFunctions[i].functionName > mFunctions[j].functionName) {
                    temp = mFunctions[i];
                    mFunctions[i] = mFunctions[j];
                    mFunctions[j] = temp;
                } // if

                j++;
            } // while

            i++;
            j = i + 1;
        } // while

        i = 0;
        while (i < mFunctions.size()) {
            cout << mFunctions[i].functionName << "()" << endl;
            i++;
        } // while
    } // ListAllFuncts()

    // 列出指定的variable 
    void ListVar() {
        string str = "";
        str.assign(mListedIdent, 1, mListedIdent.size() - 2);

        int i = 0;
        while (i < mVars.size() && mVars[i].varName != str) {
            i++;
        } // while

        if (i == mVars.size()) return;

        if (mVars[i].varType == VOID) cout << "void";
        else if (mVars[i].varType == INT) cout << "int";
        else if (mVars[i].varType == FLOAT) cout << "float";
        else if (mVars[i].varType == CHAR) cout << "char";
        else if (mVars[i].varType == BOOL) cout << "bool";
        else if (mVars[i].varType == STRING) cout << "string";

        cout << " " << mVars[i].varName;
        if (mVars[i].arrayNum[0] != '\0') cout << "[ " << mVars[i].arrayNum << " ]";
        cout << " ;" << endl;
    } // ListVar()

    // 列出指定的function 
    void ListFunct() {
        string str = "";
        str.assign(mListedIdent, 1, mListedIdent.size() - 2);

        int i = 0;
        while (i < mFunctions.size() && mFunctions[i].functionName != str) {
            i++;
        } // while

        if (i == mFunctions.size()) return;

        int numOfTypeSet = 0;
        int j = 0;
        while (j < mFunctions[i].instrs.size()) {
            // '(' '[' '++' '--'前面不加空格
            if (j != 0 &&
                mFunctions[i].instrs[j].tokenType != SLBRACKET &&
                mFunctions[i].instrs[j].tokenType != MLBRACKET &&
                mFunctions[i].instrs[j].tokenType != PP &&
                mFunctions[i].instrs[j].tokenType != SS &&
                mFunctions[i].instrs[j - 1].tokenType != LLBRACKET &&
                mFunctions[i].instrs[j - 1].tokenType != LRBRACKET &&
                mFunctions[i].instrs[j - 1].tokenType != SEMICOLON) cout << " ";

            // 如果前面是 if else while '('前面加空格 
            if (j != 0 &&
                (mFunctions[i].instrs[j - 1].tokenType == IF ||
                    mFunctions[i].instrs[j - 1].tokenType == ELSE ||
                    mFunctions[i].instrs[j - 1].tokenType == WHILE) &&
                mFunctions[i].instrs[j].tokenType == SLBRACKET) cout << " ";

            cout << mFunctions[i].instrs[j].tokenValue;

            // '{' ';' '}'要換行  '{'之後每行加2空格  '}'之後每行減2空格
            if (mFunctions[i].instrs[j].tokenType == LLBRACKET ||
                mFunctions[i].instrs[j].tokenType == SEMICOLON ||
                mFunctions[i].instrs[j].tokenType == LRBRACKET) {
                if (mFunctions[i].instrs[j].tokenType == LLBRACKET) numOfTypeSet = numOfTypeSet + 2;
                if (j + 1 < mFunctions[i].instrs.size() &&
                    mFunctions[i].instrs[j + 1].tokenType == LRBRACKET) numOfTypeSet = numOfTypeSet - 2;

                cout << endl;

                int k = 0;
                while (k < numOfTypeSet) {
                    cout << " ";
                    k++;
                } // while
            } // if

            j++;
        } // while
    } // ListFunct()

private:
    // 所有判斷文法的function都會把屬於他文法的token全部加到mTokens中 並呼叫TakeLatestToken()來取得下一個token
    // 也就是說 只有當判斷式的內容是在判斷一個token 判斷式裡面才會做AddToTokens()和TakeLatestToken()
    // 但是每個function的最後一個判斷式不能加上TakeLatestToken()
    // 每個迴圈的最後一行一定要有TakeLateestToken()

    // 設定systemcall
    void SetSystemCall() {
        if (mTokens[0].tokenValue == "Done") mCommand = DONE;
        else if (mTokens[0].tokenValue == "ListAllVariables") mCommand = LAV;
        else if (mTokens[0].tokenValue == "ListAllFunctions") mCommand = LAF;
        else if (mTokens[0].tokenValue == "ListVariable") mCommand = LV;
        else if (mTokens[0].tokenValue == "ListFunction") mCommand = LF;
    } // SetSystemCall()


    bool NoError() {
        if (!mSyntaxError && !mSemanticError) return true;
        else return false;
    } // NoError()

    // 當確定整個command為syntax error就呼叫這個function
    bool SetSyntaxError() {
        mSyntaxError = true;
        mErrorToken = mParsingToken;
        return false;
    } // SetSyntaxError()

    // 當確定整個command為semantic error就呼叫這個function
    bool SetSemanticError() {
        mSemanticError = true;
        mErrorToken = mParsingToken;
        return false;
    } // SetSemanticError()

    //  初始化parser中的變數
    void Initialize() {
        ClearToken(mErrorToken);
        mTokens.clear();
        mTempLocalVars.clear();
        mIsFunction = false;
        mSyntaxError = false;
        mSemanticError = false;
        mVarsIsInBracket = true;
        mIsFunctVar = false;
        mCommand = NO;

        ClearFunction(mTempFunct);

        mListedIdent = "";
    } // Initialize()

    // 決定要讓parser檢查已經讀取過的token或是去input中拿新token 
    void TakeLatestToken() {
        // 每次進行AddToTokens時都會清除mCurrentToken 所以當mCurrentToken.tokenType為NOTHING時 代表需要拿新的token 
        if (mCurrentToken.tokenType != NOTHING) {
            mParsingToken = mCurrentToken;
        } // if
        else {
            mScanner.GetToken();
            mParsingToken = mScanner.mToken;
            mCurrentToken = mScanner.mToken;
        } // else
    } // TakeLatestToken()

    // 把確認過文法的token放進mTokens中
    void AddToTokens() {
        mTokens.push_back(mParsingToken);
        ClearToken(mCurrentToken);
    } // AddToTokens()

    // 如果mVar中有這個var就回傳true 否則回傳false 
    bool HasVar(string str) {
        mIsFunctVar = false;
        int i = 0;
        // 到全域變數找 
        while (i < mVars.size()) {
            if (mVars[i].varName == str) return true;
            i++;
        } // while

        i = 0;
        // 到區域變數找 
        while (i < mTempLocalVars.size()) {
            if (mTempLocalVars[i].varName == str) return true;
            i++;
        } // while

        i = 0;
        while (mIsFunction && i < mTempFunct.parameters.size()) {
            if (mTempFunct.parameters[i].varName == str) return true;
            i++;
        } // while

        i = 0;
        while (i < mFunctions.size()) {
            if (mFunctions[i].functionName == str) {
                mIsFunctVar = true;
                return true;
            } // if

            i++;
        } // while

        i = 0;
        while (mVarsIsInBracket && i < mVarsInBracket.size()) {
            int j = 0;
            while (j < mVarsInBracket[i].size()) {
                if (mVarsInBracket[i][j].varName == str) return true;
                j++;
            } // while

            i++;
        } // while

        if (mIsFunction && mTempFunct.functionName == str) {
            mIsFunctVar = true;
            return true;
        } // if

        if (str == "cin" || str == "cout") return true;

        return false;
    } // HasVar()


    bool WrongVar(TokenType type) {
        if (mIsFunctVar && type != SLBRACKET) return true;
        else if (!mIsFunctVar && type == SLBRACKET) return true;
        else return false;
    } // WrongVar()

    // 把定義的variable加進mVars中 
    void AddToVariables(vector<Var> vec, TokenType type) {
        int i = 0;
        int j = 0;
        bool reDef = false;

        cout << "> ";
        while (i < vec.size()) {
            vec[i].varType = type;
            // 檢查是否有同名的variable
            while (j < mVars.size()) {
                if (vec[i].varName == mVars[j].varName) {
                    reDef = true;
                    mVars[j] = vec[i];
                } // if

                j++;
            } // while

            if (reDef) cout << "New definition of " << vec[i].varName << " entered ..." << endl;
            else {
                cout << "Definition of " << vec[i].varName << " entered ..." << endl;
                mVars.push_back(vec[i]);
            } // else

            i++;
            j = 0;
            reDef = false;
        } // while
    } // AddToVariables()


    void AddToVarsInBracket(vector<Var> vec, TokenType type) {
        int i = 0;
        int j = 0;
        bool reDef = false;
        while (i < vec.size()) {
            // 檢查是否有同名的variable
            vec[i].varType = type;
            while (j < mVarsInBracket.back().size()) {
                if (vec[i].varName == mVarsInBracket.back()[j].varName) {
                    reDef = true;
                    mVarsInBracket.back()[j] = vec[i];
                } // if

                j++;
            } // while

            if (!reDef) mVarsInBracket.back().push_back(vec[i]);

            i++;
            j = 0;
            reDef = false;
        } // while
    } // AddToVarsInBracket()


    void AddToLocalVars(vector<Var> vec, TokenType type) {
        int i = 0;
        int j = 0;
        bool reDef = false;

        while (i < vec.size()) {
            // 檢查是否有同名的variable
            vec[i].varType = type;
            while (j < mTempLocalVars.size()) {
                if (vec[i].varName == mTempLocalVars[j].varName) {
                    reDef = true;
                    mTempLocalVars[j] = vec[i];
                } // if

                j++;
            } // while

            if (!reDef) mTempLocalVars.push_back(vec[i]);

            i++;
            j = 0;
            reDef = false;
        } // while
    } // AddToLocalVars()

    // 暫時把讀取到的var存在vec中
    void AddToTempIdents(vector<Var>& vec, string tokenName) {
        Var temp;
        ClearVar(temp);

        temp.varName = tokenName;
        temp.arrayNum = "";

        vec.push_back(temp);
    } // AddToTempIdents()


    void AddToFunctions(TokenType type) {
        int i = 0;
        bool reDef = false;

        cout << "> ";
        mTempFunct.functType = type;

        i = 0;
        while (i < mFunctions.size()) {
            // 檢查是否有同名的function
            if (mFunctions[i].functionName == mTempFunct.functionName) {
                reDef = true;
                mFunctions[i] = mTempFunct;
            } // if

            i++;
        } // while

        if (reDef) cout << "New definition of " << mTempFunct.functionName << "() entered ..." << endl;
        else {
            cout << "Definition of " << mTempFunct.functionName << "() entered ..." << endl;
            mFunctions.push_back(mTempFunct);
        } // else

        mCommand = FUN_DEF;
    } // AddToFunctions()


    bool Is_SystemCall() {
        // Done() || ListAllVariables() || ListAllFunctions() || 
        // listVariable( char name[] ) || ListFunction( char name[] )
        TakeLatestToken();
        if (mParsingToken.tokenValue == "Done" ||
            mParsingToken.tokenValue == "ListAllVariables" ||
            mParsingToken.tokenValue == "ListAllFunctions") {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == SLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (mParsingToken.tokenType == SRBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                    if (mParsingToken.tokenType == SEMICOLON) {
                        AddToTokens();
                        SetSystemCall();
                        return true;
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // if
        else if (mParsingToken.tokenValue == "ListVariable" ||
            mParsingToken.tokenValue == "ListFunction") {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == SLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (mParsingToken.tokenType == CONST && mParsingToken.tokenValue[0] == '\"') {
                    mListedIdent = mParsingToken.tokenValue;
                    AddToTokens();
                    TakeLatestToken();
                    if (mParsingToken.tokenType == SRBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                        if (mParsingToken.tokenType == SEMICOLON) {
                            AddToTokens();
                            SetSystemCall();
                            return true;
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else return false;
    } // Is_SystemCall()


    bool Is_Definition() {
        // definition : VOID Identifier function_definition_without_ID |
        // type_specifier Identifier function_definition_or_declarators
        vector<Var> tempVars;
        TokenType tempType;

        tempVars.clear();
        tempType = NOTHING;

        TakeLatestToken();
        if (mParsingToken.tokenType == VOID) {
            tempType = mParsingToken.tokenType;
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == IDENT) {
                mTempFunct.functionName = mParsingToken.tokenValue;
                AddToTokens();
                TakeLatestToken();
                if (Is_FunctionDefinitionWithoutID()) {
                    AddToFunctions(tempType);
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // if
        else if (Is_TypeSpecifier()) {
            tempType = mParsingToken.tokenType;
            TakeLatestToken();
            if (mParsingToken.tokenType == IDENT) {
                mTempFunct.functionName = mParsingToken.tokenValue;
                AddToTempIdents(tempVars, mParsingToken.tokenValue);
                AddToTokens();
                TakeLatestToken();
                if (Is_FunctionDefinitionOrDeclarators(tempVars)) {
                    if (mIsFunction) {
                        AddToFunctions(tempType);
                        mCommand = FUN_DEF;
                    } // if
                    else {
                        AddToVariables(tempVars, tempType);
                        mCommand = ID_DEF;
                    } // else

                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else return false;
    } // Is_Definition()


    bool Is_TypeSpecifier() {
        // type_specifier : INT | CHAR | FLOAT | STRING | BOOL
        TakeLatestToken();
        if (mParsingToken.tokenType == INT || mParsingToken.tokenType == CHAR ||
            mParsingToken.tokenType == FLOAT || mParsingToken.tokenType == STRING ||
            mParsingToken.tokenType == BOOL) {
            AddToTokens();
            return true;
        } // if
        else return false;
    } // Is_TypeSpecifier()


    bool Is_FunctionDefinitionOrDeclarators(vector<Var>& tempVars) {
        // function_definition_or_declarators : function_definition_without_ID | rest_of_declarators
        TakeLatestToken();
        // 為function definition
        if (Is_FunctionDefinitionWithoutID()) {
            mIsFunction = true;
            return true;
        } // if
        // 為ident definition 
        else if (NoError() && Is_RestOfDeclarators(tempVars)) {
            mIsFunction = false;
            return true;
        } // else if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_FunctionDefinitionOrDeclarators()


    bool Is_RestOfDeclarators(vector<Var>& tempVars) {
        // rest_of_declarators : [ '[' Constant ']' ] { ',' Identifier [ '[' Constant ']' ] } ';'
        string arrayNum = "";
        TakeLatestToken();
        // '[' Constant ']' 可不加 
        if (mParsingToken.tokenType == MLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == CONST) {
                arrayNum = mParsingToken.tokenValue;
                AddToTokens();
                TakeLatestToken();
                if (mParsingToken.tokenType == MRBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                    tempVars.back().arrayNum = arrayNum;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // if

        while (mParsingToken.tokenType == COMMA) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == IDENT) {
                AddToTempIdents(tempVars, mParsingToken.tokenValue);
                AddToTokens();
                TakeLatestToken();
                // '[' Constant ']' 可不加 
                if (mParsingToken.tokenType == MLBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                    if (mParsingToken.tokenType == CONST) {
                        arrayNum = mParsingToken.tokenValue;
                        AddToTokens();
                        TakeLatestToken();
                        if (mParsingToken.tokenType == MRBRACKET) {
                            AddToTokens();
                            TakeLatestToken();
                            tempVars.back().arrayNum = arrayNum;
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if

            } // if
            else return SetSyntaxError();

            TakeLatestToken();
        } // while

        if (mParsingToken.tokenType == SEMICOLON) {
            AddToTokens();
            return true;
        } // if
        else return false;
    } // Is_RestOfDeclarators()


    bool Is_FunctionDefinitionWithoutID() {
        // function_definition_without_ID : '(' [ VOID | formal_parameter_list ] ')' compound_statement
        mIsFunction = true;
        mVarsIsInBracket = false;
        TakeLatestToken();
        if (mParsingToken.tokenType == SLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == VOID) {
                AddToTokens();
                TakeLatestToken();
            } // if
            else if (Is_FormalParameterList(mTempFunct.parameters)) {
                TakeLatestToken();
            } // else if

            if (NoError() && mParsingToken.tokenType == SRBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_CompoundStatement()) {
                    int i = 0;
                    while (i < mTempLocalVars.size() || i < mTokens.size()) {
                        if (i < mTempLocalVars.size()) mTempFunct.localVars.push_back(mTempLocalVars[i]);
                        if (i < mTokens.size()) mTempFunct.instrs.push_back(mTokens[i]);

                        i++;
                    } // while

                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();

        } // if
        else return false;
    } // Is_FunctionDefinitionWithoutID()


    bool Is_FormalParameterList(vector <Var>& parameters) {
        // formal_parameter_list : type_specifier [ '&' ] Identifier [ '[' Constant ']' ]
        // { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
        Var tempVar;

        ClearVar(tempVar);

        TakeLatestToken();
        if (Is_TypeSpecifier()) {
            tempVar.varType = mParsingToken.tokenType;
            TakeLatestToken();
            // '&'可不加 
            if (mParsingToken.tokenType == BIT_AND) {
                AddToTokens();
                TakeLatestToken();
            } // if

            if (mParsingToken.tokenType == IDENT) {
                tempVar.varName = mParsingToken.tokenValue;
                AddToTokens();
                TakeLatestToken();
            } // if
            else return SetSyntaxError();
            // '[' Constant ']' 可不加 
            if (mParsingToken.tokenType == MLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (mParsingToken.tokenType == CONST) {
                    tempVar.arrayNum = mParsingToken.tokenValue;
                    AddToTokens();
                    TakeLatestToken();
                    if (mParsingToken.tokenType == MRBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if

            parameters.push_back(tempVar);
            ClearVar(tempVar);

            while (mParsingToken.tokenType == COMMA) {
                AddToTokens();
                TakeLatestToken();
                if (Is_TypeSpecifier()) {
                    tempVar.varType = mParsingToken.tokenType;
                    TakeLatestToken();
                    // '&'可不加 
                    if (mParsingToken.tokenType == BIT_AND) {
                        AddToTokens();
                        TakeLatestToken();
                    } // if

                    if (mParsingToken.tokenType == IDENT) {
                        tempVar.varName = mParsingToken.tokenValue;
                        AddToTokens();
                        TakeLatestToken();
                    } // if
                    else return SetSyntaxError();
                    // '[' Constant ']' 可不加 
                    if (mParsingToken.tokenType == MLBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                        if (mParsingToken.tokenType == CONST) {
                            tempVar.arrayNum = mParsingToken.tokenValue;
                            AddToTokens();
                            TakeLatestToken();
                            if (mParsingToken.tokenType == MRBRACKET) {
                                AddToTokens();
                                TakeLatestToken();
                            } // if
                            else return SetSyntaxError();
                        } // if
                        else return SetSyntaxError();
                    } // if

                    parameters.push_back(tempVar);
                    ClearVar(tempVar);
                } // if ( Is_TypeSpecifier() )
                else return SetSyntaxError();

                TakeLatestToken();
            } // while
        } // if
        else return false;

        return true;
    } // Is_FormalParameterList()


    bool Is_CompoundStatement() {
        // compound_statement : '{' { declaration | statement } '}'
        TakeLatestToken();
        if (mParsingToken.tokenType == LLBRACKET) {
            if (!mVarsIsInBracket) mVarsIsInBracket = true;
            else {
                // 每次進入一層大括號 就建立一個stack用來存vars
                vector <Var> tempVars;
                mVarsInBracket.push_back(tempVars);
            } // else 

            AddToTokens();
            TakeLatestToken();
            while (Is_Declaration() || Is_Statement()) {
                TakeLatestToken();
            } // while

            if (mSyntaxError) return SetSyntaxError();
            if (mSemanticError) return SetSemanticError();

            if (mParsingToken.tokenType == LRBRACKET) {
                AddToTokens();
                if (!mVarsInBracket.empty()) mVarsInBracket.pop_back();
                if (mVarsInBracket.empty()) mVarsIsInBracket = false;
                return true;
            } // if
            else return SetSyntaxError();
        } // if
        else return false;
    } // Is_CompoundStatement()

    // 分成 function中的local variable declaration 和 被大括號括住的declaration 
    bool Is_Declaration() {
        // declaration : type_specifier Identifier rest_of_declarators
        vector <Var> tempVars;
        Var tempVar;

        tempVars.clear();
        ClearVar(tempVar);

        TakeLatestToken();
        if (Is_TypeSpecifier()) {
            tempVar.varType = mParsingToken.tokenType;
            TakeLatestToken();
            if (mParsingToken.tokenType == IDENT) {
                tempVar.varName = mParsingToken.tokenValue;
                tempVars.push_back(tempVar);
                AddToTokens();
                TakeLatestToken();
                if (Is_RestOfDeclarators(tempVars)) {
                    if (mVarsInBracket.empty()) AddToLocalVars(tempVars, tempVar.varType);
                    else AddToVarsInBracket(tempVars, tempVar.varType);
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // if
        else return false;
    } // Is_Declaration()


    bool Is_Statement() {
        // statement : ';' |
        // RETURN [ expression ] ';' |
        // IF '(' expression ')' statement [ ELSE statement ] |
        // WHILE '(' expression ')' statement |
        // DO statement WHILE '(' expression ')' ';'
        // compound_statement |
        // expression ';' |
        //  if ( mSyntaxError ) return SetSyntaxError() ;
        //  if ( mSemanticError ) return SetSemanticError() ;
        if (mSyntaxError) return SetSyntaxError();
        TakeLatestToken();
        if (mParsingToken.tokenType == SEMICOLON) {
            AddToTokens();
            mCommand = STAT;
            return true;
        } // if
        else if (mParsingToken.tokenType == RETURN) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Expression()) {
                TakeLatestToken();
            } // if

            if (NoError() && mParsingToken.tokenType == SEMICOLON) {
                AddToTokens();
                mCommand = STAT;
                return true;
            } // if
            else return SetSyntaxError();
        } // else if
        else if (mParsingToken.tokenType == IF) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == SLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_Expression()) {
                    TakeLatestToken();
                    if (mParsingToken.tokenType == SRBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                        if (Is_Statement()) {
                            TakeLatestToken();
                            if (mParsingToken.tokenType == ELSE) {
                                AddToTokens();
                                TakeLatestToken();
                                if (Is_Statement()) {
                                    // do nothing
                                } // if
                                else return SetSyntaxError();
                            } // if

                            mCommand = STAT;
                            return true;
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else if (mParsingToken.tokenType == WHILE) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == SLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_Expression()) {
                    TakeLatestToken();
                    if (mParsingToken.tokenType == SRBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                        if (Is_Statement()) {
                            mCommand = STAT;
                            return true;
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else if (mParsingToken.tokenType == DO) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Statement()) {
                TakeLatestToken();
                if (mParsingToken.tokenType == WHILE) {
                    AddToTokens();
                    TakeLatestToken();
                    if (mParsingToken.tokenType == SLBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                        if (Is_Expression()) {
                            TakeLatestToken();
                            if (mParsingToken.tokenType == SRBRACKET) {
                                AddToTokens();
                                TakeLatestToken();
                                if (mParsingToken.tokenType == SEMICOLON) {
                                    AddToTokens();
                                    mCommand = STAT;
                                    return true;
                                } // if
                                else return SetSyntaxError();
                            } // if
                            else return SetSyntaxError();
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else if (NoError() && Is_CompoundStatement()) {
            mCommand = STAT;
            return true;
        } // else if
        else if (NoError() && Is_Expression()) {
            TakeLatestToken();
            if (mParsingToken.tokenType == SEMICOLON) {
                AddToTokens();
                mCommand = STAT;
                return true;
            } // if
            else return SetSyntaxError();
        } // else if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_Statement() 


    bool Is_Expression() {
        // expression : basic_expression { ',' basic_expression }
        TakeLatestToken();
        if (Is_BasicExpression()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == COMMA) {
                AddToTokens();
                TakeLatestToken();
                if (Is_BasicExpression()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();

    } // Is_Expression()


    bool Is_BasicExpression() {
        // basic_expression : Identifier rest_of_Identifier_started_basic_exp |
        // ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp |
        // sign { sign } signed_unary_exp romce_and_romloe |
        // ( Constant | '(' expression ')' ) romce_and_romloe
        TakeLatestToken();
        if (mParsingToken.tokenType == IDENT) {
            if (!HasVar(mParsingToken.tokenValue)) return SetSemanticError();
            AddToTokens();
            TakeLatestToken();
            if (WrongVar(mParsingToken.tokenType)) {
                mParsingToken = mTokens.back();
                return SetSemanticError();
            } // if

            if (Is_RestOfIdentStartedBasicExp()) {
                return true;
            } // if
            else return SetSyntaxError();
        } // if
        else if (mParsingToken.tokenType == PP || mParsingToken.tokenType == SS) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == IDENT) {
                if (!HasVar(mParsingToken.tokenValue)) return SetSemanticError();

                AddToTokens();
                TakeLatestToken();
                if (WrongVar(mParsingToken.tokenType)) {
                    mParsingToken = mTokens.back();
                    return SetSemanticError();
                } // if

                if (Is_RestOfPPMMIdentStartedBasicExp()) {
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else if (mParsingToken.tokenType == CONST) {
            AddToTokens();
            TakeLatestToken();
            if (Is_RomceAndRomloe()) {
                return true;
            } // if
            else return SetSyntaxError();
        } // else if
        else if (mParsingToken.tokenType == SLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Expression()) {
                TakeLatestToken();
                if (mParsingToken.tokenType == SRBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                    if (Is_RomceAndRomloe()) {
                        return true;
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        else if (Is_Sign()) {
            TakeLatestToken();
            while (Is_Sign()) {
                TakeLatestToken();
            } // while

            if (Is_SignedUnaryExp()) {
                TakeLatestToken();
                if (Is_RomceAndRomloe()) {
                    return true;
                }  // if
                else return SetSyntaxError();
            }  // if
            else return SetSyntaxError();
        } // else if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_BasicExpression()


    bool Is_RestOfIdentStartedBasicExp() {
        // rest_of_Identifier_started_basic_exp : [ '[' expression ']' ] 下行繼續... 
        // ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe ) |
        // '(' [ actual_parameter_list ] ')' romce_and_romloe
        TakeLatestToken();
        // '[' expression ']' ( ... | ... ) 的那條文法 
        if (mParsingToken.tokenType == MLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Expression()) {
                TakeLatestToken();
                if (mParsingToken.tokenType == MRBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();

            if (mParsingToken.tokenType == PP || mParsingToken.tokenType == SS) {
                AddToTokens();
                TakeLatestToken();
                if (Is_RomceAndRomloe()) {
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else if (Is_AssignmentOperator()) {
                TakeLatestToken();
                if (Is_BasicExpression()) {
                    return true;
                } // if
                else return SetSyntaxError();
            } // else if
            else if (NoError() && Is_RomceAndRomloe()) {
                return true;
            } // else if
            else return SetSyntaxError();
        } // if
        // '(' [ actual_parameter_list ] ')' romce_and_romloe 的那條文法  
        else if (mParsingToken.tokenType == SLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (Is_ActualParameterList()) {
                TakeLatestToken();
            } // if

            if (mSyntaxError) return SetSyntaxError();

            if (mParsingToken.tokenType == SRBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_RomceAndRomloe()) {
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if
        // [ PP | MM ] romce_and_romloe 的那條文法  
        else if (mParsingToken.tokenType == PP || mParsingToken.tokenType == SS) {
            AddToTokens();
            TakeLatestToken();
            if (Is_RomceAndRomloe()) {
                return true;
            } // if
            else return SetSyntaxError();
        } // else if
        // assignment_operator basic_expression 的那條文法
        else if (Is_AssignmentOperator()) {
            TakeLatestToken();
            if (Is_BasicExpression()) {
                return true;
            } // if
            else return SetSyntaxError();
        } // else if
        // [ PP | MM ]被省略，romce_and_romloe 的那條文法
        else if (NoError() && Is_RomceAndRomloe()) {
            return true;
        } // else if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfIdentStartedBasicExp()


    bool Is_RestOfPPMMIdentStartedBasicExp() {
        // rest_of_PPMM_Identifier_started_basic_exp : [ '[' expression ']' ] romce_and_romloe 
        if (mParsingToken.tokenType == MLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Expression()) {
                TakeLatestToken();
                if (mParsingToken.tokenType == MRBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                    if (Is_RomceAndRomloe()) {
                        return true;
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // if
        else if (Is_RomceAndRomloe()) {
            return true;
        } // else if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfPPMMIdentStartedBasicExp()


    bool Is_Sign() {
        // sign : '+' | '-' | '!'
        TakeLatestToken();
        if (mParsingToken.tokenType == PLUS || mParsingToken.tokenType == SUB ||
            mParsingToken.tokenType == NOT) {
            AddToTokens();
            return true;
        } // if
        else return false;
    } // Is_Sign()


    bool Is_ActualParameterList() {
        // actual_parameter_list : basic_expression { ',' basic_expression }
        TakeLatestToken();
        if (Is_BasicExpression()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == COMMA) {
                AddToTokens();
                TakeLatestToken();
                if (Is_BasicExpression()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_ActualParameterList() 


    bool Is_AssignmentOperator() {
        // assignment_operator : '=' | TE | DE | RE | PE | ME
        TakeLatestToken();
        if (mParsingToken.tokenType == PA || mParsingToken.tokenType == SA ||
            mParsingToken.tokenType == MA || mParsingToken.tokenType == DA ||
            mParsingToken.tokenType == OA || mParsingToken.tokenType == ASSIGN) {
            AddToTokens();
            return true;
        } // if
        else return false;
    } // Is_AssignmentOperator()


    bool Is_RomceAndRomloe() {
        // romce_and_romloe : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
        TakeLatestToken();
        if (Is_RestOfMaybeLogicalOrExp()) {
            TakeLatestToken();
            if (mParsingToken.tokenType == CO) {
                AddToTokens();
                TakeLatestToken();
                if (Is_BasicExpression()) {
                    TakeLatestToken();
                    if (mParsingToken.tokenType == COC) {
                        AddToTokens();
                        TakeLatestToken();
                        if (Is_BasicExpression()) {
                            // do nothing
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // if

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RomceAndRomloe()


    bool Is_RestOfMaybeLogicalOrExp() {
        // rest_of_maybe_logical_OR_exp : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeLogicalAndExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == OR) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeLogicalAndExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeLogicalOrExp()


    bool Is_MaybeLogicalAndExp() {
        // maybe_logical_AND_exp : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
        TakeLatestToken();
        if (Is_MaybeBitOrExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == AND) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeBitOrExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeLogicalAndExp()


    bool Is_RestOfMaybeLogicalAndExp() {
        // rest_of_maybe_logical_AND_exp : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeBitOrExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == AND) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeBitOrExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeLogicalAndExp() 


    bool Is_MaybeBitOrExp() {
        // maybe_bit_OR_exp : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
        TakeLatestToken();
        if (Is_MaybeBitXorExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == BIT_OR) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeBitXorExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeBitOrExp()


    bool Is_RestOfMaybeBitOrExp() {
        // rest_of_maybe_bit_OR_exp : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeBitXorExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == BIT_OR) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeBitXorExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeBitOrExp()


    bool Is_MaybeBitXorExp() {
        // maybe_bit_ex_OR_exp : maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
        TakeLatestToken();
        if (Is_MaybeBitAndExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == XOR) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeBitAndExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();

    } // Is_MaybeBitXorExp()


    bool Is_RestOfMaybeBitXorExp() {
        // rest_of_maybe_bit_ex_OR_exp : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeBitAndExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == XOR) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeBitAndExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeBitXorExp()


    bool Is_MaybeBitAndExp() {
        // maybe_bit_AND_exp : maybe_equality_exp { '&' maybe_equality_exp }
        TakeLatestToken();
        if (Is_MaybeEqualityExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == BIT_AND) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeEqualityExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeBitAndExp()


    bool Is_RestOfMaybeBitAndExp() {
        // rest_of_maybe_bit_AND_exp : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeEqualityExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == BIT_AND) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeEqualityExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeBitAndExp()


    bool Is_MaybeEqualityExp() {
        // maybe_equality_exp : maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
        TakeLatestToken();
        if (Is_MaybeRelationalExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == EQ || mParsingToken.tokenType == NE) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeRelationalExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeEqualityExp()


    bool Is_RestOfMaybeEqualityExp() {
        // rest_of_maybe_equality_exp : rest_of_maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeRelationalExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == EQ || mParsingToken.tokenType == NE) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeRelationalExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeEqualityExp()


    bool Is_MaybeRelationalExp() {
        // maybe_relational_exp : maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
        TakeLatestToken();
        if (Is_MaybeShiftExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == LT || mParsingToken.tokenType == GT ||
                mParsingToken.tokenType == LE || mParsingToken.tokenType == GE) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeShiftExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeRelationalExp()


    bool Is_RestOfMaybeRelationalExp() {
        // rest_of_maybe_relational_exp : rest_of_maybe_shift_exp { ( '<' | '>' | LE | GE ) maybe_shift_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeShiftExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == LT || mParsingToken.tokenType == GT ||
                mParsingToken.tokenType == LE || mParsingToken.tokenType == GE) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeShiftExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeRelationalExp()


    bool Is_MaybeShiftExp() {
        // maybe_shift_exp : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
        TakeLatestToken();
        if (Is_MaybeAdditiveExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == LS || mParsingToken.tokenType == RS) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeAdditiveExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeShiftExp()


    bool Is_RestOfMaybeShiftExp() {
        // rest_of_maybe_shift_exp : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeAdditiveExp()) {
            TakeLatestToken();
            while ( mParsingToken.tokenType == LS || mParsingToken.tokenType == RS ) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeAdditiveExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeShiftExp()


    bool Is_MaybeAdditiveExp() {
        // maybe_additive_exp : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
        TakeLatestToken();
        if ( Is_MaybeMultExp() ) {
            TakeLatestToken();
            while ( mParsingToken.tokenType == PLUS || mParsingToken.tokenType == SUB ) {
                AddToTokens();
                TakeLatestToken();
                cout << mParsingToken.tokenValue << endl;
                if (Is_MaybeMultExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeAdditiveExp()


    bool Is_RestOfMaybeAdditiveExp() {
        // rest_of_maybe_additive_exp : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
        TakeLatestToken();
        if (Is_RestOfMaybeMultExp()) {
            TakeLatestToken();
            while (mParsingToken.tokenType == PLUS || mParsingToken.tokenType == SUB) {
                AddToTokens();
                TakeLatestToken();
                if (Is_MaybeMultExp()) {
                    TakeLatestToken();
                } // if
                else return SetSyntaxError();
            } // while

            return true;
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_RestOfMaybeAdditiveExp()


    bool Is_MaybeMultExp() {
        // maybe_mult_exp : unary_exp rest_of_maybe_mult_exp
        TakeLatestToken();
        if (Is_UnaryExp()) {
            TakeLatestToken();
            if (Is_RestOfMaybeMultExp()) {
                return true;
            } // if
            else return SetSyntaxError();
        } // if
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_MaybeMultExp()


    bool Is_RestOfMaybeMultExp() {
        // rest_of_maybe_mult_exp : { ( '*' | '/' | '%' ) unary_exp }
        TakeLatestToken();
        while (mParsingToken.tokenType == MUL || mParsingToken.tokenType == DIV ||
            mParsingToken.tokenType == MOD) {
            AddToTokens();
            TakeLatestToken();
            if (Is_UnaryExp()) {
                TakeLatestToken();
            } // if
            else return SetSyntaxError();
        } // while

        return true;
    } // Is_RestOfMaybeMultExp()


    bool Is_UnaryExp() {
        // unary_exp : sign { sign } signed_unary_exp | unsigned_unary_exp |
        // ( PP | MM ) Identifier [ '[' expression ']' ]
        TakeLatestToken();
        if (mParsingToken.tokenType == PP || mParsingToken.tokenType == SS) {
            AddToTokens();
            TakeLatestToken();
            if (mParsingToken.tokenType == IDENT) {
                if (!HasVar(mParsingToken.tokenValue)) return SetSemanticError();
                AddToTokens();
                TakeLatestToken();
                if (WrongVar(mParsingToken.tokenType)) {
                    mParsingToken = mTokens.back();
                    return SetSemanticError();
                } // if

                // [ '[' expression ']' ]
                if (mParsingToken.tokenType == MLBRACKET) {
                    AddToTokens();
                    TakeLatestToken();
                    if (Is_Expression()) {
                        TakeLatestToken();
                        if (mParsingToken.tokenType == MRBRACKET) {
                            AddToTokens();
                        } // if
                        else return SetSyntaxError();
                    } // if
                    else return SetSyntaxError();
                } // if

                return true;
            } // if
            else return SetSyntaxError();
        } // if
        else if (Is_Sign()) {
            TakeLatestToken();
            while (Is_Sign()) {
                TakeLatestToken();
            } // while

            if (Is_SignedUnaryExp()) {
                return true;
            } // if
            else return SetSyntaxError();
        } // else if
        else if (NoError() && Is_UnsignedUnaryExp()) {
            return true;
        } // else if 
        else if (NoError()) return false;
        else return SetSyntaxError();
    } // Is_UnaryExp()


    bool Is_SignedUnaryExp() {
        // signed_unary_exp : Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']' ] |
        // Constant | '(' expression ')'
        TakeLatestToken();
        if (mParsingToken.tokenType == CONST) {
            AddToTokens();
            return true;
        } // if
        else if (mParsingToken.tokenType == IDENT) {
            if (!HasVar(mParsingToken.tokenValue)) return SetSemanticError();
            AddToTokens();
            TakeLatestToken();
            if (WrongVar(mParsingToken.tokenType)) {
                mParsingToken = mTokens.back();
                return SetSemanticError();
            } // if

            if (mParsingToken.tokenType == SLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_ActualParameterList()) {

                    TakeLatestToken();
                } // if

                if (NoError() && mParsingToken.tokenType == SRBRACKET) {
                    AddToTokens();
                } // if
                else return SetSyntaxError();
            } // if
            else if (mParsingToken.tokenType == MLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_Expression()) {
                    TakeLatestToken();
                    if (mParsingToken.tokenType == MRBRACKET) {
                        AddToTokens();
                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // else if

            return true;
        } // else if
        else if (mParsingToken.tokenType == SLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Expression()) {
                TakeLatestToken();
                if (mParsingToken.tokenType == SRBRACKET) {
                    AddToTokens();
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if 
        else return false;
    } // Is_SignedUnaryExp()


    bool Is_UnsignedUnaryExp() {
        // unsigned_unary_exp : Identifier 下行繼續... 
        // [ '(' [ actual_parameter_list ] ')' | [ '[' expression ']' ] [ ( PP | MM ) ] ] |
        // Constant | '(' expression ')'
        TakeLatestToken();
        if (mParsingToken.tokenType == CONST) {
            AddToTokens();
            return true;
        } // if
        else if (mParsingToken.tokenType == IDENT) {
            if (!HasVar(mParsingToken.tokenValue)) return SetSemanticError();
            AddToTokens();
            TakeLatestToken();
            if (WrongVar(mParsingToken.tokenType)) {
                mParsingToken = mTokens.back();
                return SetSemanticError();
            } // if

            if (mParsingToken.tokenType == SLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_ActualParameterList()) {
                    TakeLatestToken();
                } // if

                if (NoError() && mParsingToken.tokenType == SRBRACKET) {
                    AddToTokens();
                } // if
                else return SetSyntaxError();
            } // if
            else if (mParsingToken.tokenType == MLBRACKET) {
                AddToTokens();
                TakeLatestToken();
                if (Is_Expression()) {
                    TakeLatestToken();
                    if (mParsingToken.tokenType == MRBRACKET) {
                        AddToTokens();
                        TakeLatestToken();
                        if (mParsingToken.tokenType == PP || mParsingToken.tokenType == SS) {
                            AddToTokens();
                        } // if

                    } // if
                    else return SetSyntaxError();
                } // if
                else return SetSyntaxError();
            } // else if
            else if (mParsingToken.tokenType == PP || mParsingToken.tokenType == SS) {
                AddToTokens();
            } // else if

            return true;
        } // else if
        else if (mParsingToken.tokenType == SLBRACKET) {
            AddToTokens();
            TakeLatestToken();
            if (Is_Expression()) {
                TakeLatestToken();
                if (mParsingToken.tokenType == SRBRACKET) {
                    AddToTokens();
                    return true;
                } // if
                else return SetSyntaxError();
            } // if
            else return SetSyntaxError();
        } // else if 
        else return false;
    } // Is_UnsignedUnaryExp()

}; // Parser()


Parser::Parser() {
    ClearToken(mCurrentToken);
    ClearToken(mParsingToken);
    mTokens.clear();
    mVars.clear();
    mTempLocalVars.clear();
    mFunctions.clear();

    ClearFunction(mTempFunct);

    mCommand = NO;
    mIsFunction = false;
    mSyntaxError = false;
    mSemanticError = false;
    ClearToken(mErrorToken);

    mListedIdent = "";

} // Parser::Parser()


int main() {
    int uTestNum = 0;
    //cin >> uTestNum ; // 注意 在外面測試時一定要把這行註解掉!!!!!
    //char uselessCh = cin.get() ; // 把uTestNum後面的'\n'去掉

    if (false) return 0; // 這句是用來卡styleCheck的

    Parser parser;

    cout << "Our-C running ..." << endl;

    do {
        if (parser.IsRightGrammer()) {
            if (parser.mCommand == STAT) cout << "> Statement executed ..." << endl;
            else if (parser.mCommand == LAV) {
                cout << "> ";
                parser.ListAllVars();
                cout << "Statement executed ..." << endl;
            } // else if
            else if (parser.mCommand == LAF) {
                cout << "> ";
                parser.ListAllFuncts();
                cout << "Statement executed ..." << endl;
            } // else if
            else if (parser.mCommand == LV) {
                cout << "> ";
                parser.ListVar();
                cout << "Statement executed ..." << endl;
            } // else if
            else if (parser.mCommand == LF) {
                cout << "> ";
                parser.ListFunct();
                cout << "Statement executed ..." << endl;
            } // else if

            parser.mScanner.mTokenStarted = false;
        } // if
        else {
            parser.PrintError();
            parser.mScanner.ClearInput();
            ClearToken(parser.mCurrentToken);
        } // else

        parser.mScanner.mLine = 1;

        /*
        int i = 0 ;
        while ( i < parser.mTokens.size() ) {
          cout << parser.mTokens[i++].tokenValue << " " ;
        } // while
        cout << endl ;
        */

    } while (parser.mCommand != DONE);

    cout << "> Our-C exited ..." << endl;

    return 0;
} // main()


