# include <iostream>
# include <string>
# include <cstring>
# include <vector>
# include <cstdlib>
# include <stdio.h>
# include <stdexcept>

using namespace std;

class Token {
  public :
  int m_row ;
  int m_column ;
  string m_type ;
  string m_const_type ;
  string m_value ;
} ;

class Content {
  public :
  string m_type ;
  int m_int ;
  char m_char ;
  float m_float ;
  bool m_bool ;
  string m_string ;
  string m_funct ;

  public :
  Content() {
    m_type = "" ;
    m_int = 0 ;
    m_char = '\0' ;
    m_float = 0 ;
    m_bool = 0 ;
    m_string = "" ;
    m_funct = "" ;
  } // Content()

  void Calculation( Token operate, Content content ) ;
  void Print() ;
} ;

class Ident {
  public :
  string m_type ;
  string m_name ;
  string m_array ;
  Content m_content ;
  vector<Content>m_content_array ;
  string m_var_or_funct ;

  public :
  Ident() {
    m_type = "" ;
    m_name = "" ;
    m_array = "-1" ;
    m_var_or_funct = "" ;
  } // Ident()

  void Print() ;
  void Redefine_Funct() ;
  void Redefine_Var( string type_, string name_, string array_, string var_or_funct_ ) ;
} ;

int g_cout = 0, g_para = 0 ;
int g_definition_or_statement = 0, g_variable_or_function = 0 ;
int g_row = 0, g_column = 0 ;
vector<Token>g_TokenBuffer ;
vector<Token>g_TokenList ;
vector<vector<Ident> >g_IdentTable ;

void User_input( Token &token ) ;
void Definition( Token &token ) ;
void Type_specifier( Token &token ) ;
void Function_definition_or_declarators( Token &token ) ;
void Rest_of_declarators( Token &token, bool local ) ;
void Function_definition_without_ID( Token &token ) ;
void Formal_parameter_list( Token &token ) ;
void Compound_statement( Token &token ) ;
void Declaration( Token &token ) ;
void Statement( Token &token ) ;
void Expression( Token &token ) ;
void Basic_expression( Token &token ) ;
void Rest_of_Identifier_started_basic_exp( Token &token ) ;
void Rest_of_PPMM_identifier_started_basic_exp( Token &token ) ;
void Sign( Token &token ) ;
void Actual_parameter_list( Token &token ) ;
void Assignment_operator( Token &token ) ;
void Romce_and_romloe( Token &token ) ;
void Rest_of_maybe_logical_OR_exp( Token &token ) ;
void Maybe_logical_AND_exp( Token &token ) ;
void Rest_of_maybe_logical_AND_exp( Token &token ) ;
void Maybe_bit_OR_exp( Token &token ) ;
void Rest_of_maybe_bit_OR_exp( Token &token ) ;
void Maybe_bit_ex_OR_exp( Token &token ) ;
void Rest_of_maybe_bit_ex_OR_exp( Token &token ) ;
void Maybe_bit_AND_exp( Token &token ) ;
void Rest_of_maybe_bit_AND_exp( Token &token ) ;
void Maybe_equality_exp( Token &token ) ;
void Rest_of_maybe_equality_exp( Token &token ) ;
void Maybe_relational_exp( Token &token ) ;
void Rest_of_maybe_relational_exp( Token &token ) ;
void Maybe_shift_exp( Token &token ) ;
void Rest_of_maybe_shift_exp( Token &token ) ;
void Maybe_additive_exp( Token &token ) ;
void Rest_of_maybe_additive_exp( Token &token ) ;
void Maybe_mult_exp( Token &token ) ;
void Rest_of_maybe_mult_exp( Token &token ) ;
void Unary_exp( Token &token ) ;
void Signed_unary_exp( Token &token ) ;
void Unsigned_unary_exp( Token &token ) ;

void Execute_User_input() ;
void Execute_Definition() ;
void Execute_Compound_statement() ;
void Execute_Declaration() ;
void Execute_Statement() ;
Content Execute_Expression() ;
void Execute_Basic_expression( Content &content ) ;
void Execute_Rest_of_Identifier_started_basic_exp( Token ident, Content &content ) ;
void Execute_Rest_of_PPMM_identifier_started_basic_exp( Token ppmm, Token ident, Content &content ) ;
void Execute_Actual_parameter_list() ;
void Execute_Romce_and_romloe( Content &content ) ;
void Execute_Rest_of_maybe_logical_OR_exp( Content &content ) ;
void Execute_Maybe_logical_AND_exp( Content &content ) ;
void Execute_Rest_of_maybe_logical_AND_exp( Content &content ) ;
void Execute_Maybe_bit_OR_exp( Content &content ) ;
void Execute_Rest_of_maybe_bit_OR_exp( Content &content ) ;
void Execute_Maybe_bit_ex_OR_exp( Content &content ) ;
void Execute_Rest_of_maybe_bit_ex_OR_exp( Content &content ) ;
void Execute_Maybe_bit_AND_exp( Content &content ) ;
void Execute_Rest_of_maybe_bit_AND_exp( Content &content ) ;
void Execute_Maybe_equality_exp( Content &content ) ;
void Execute_Rest_of_maybe_equality_exp( Content &content ) ;
void Execute_Maybe_relational_exp( Content &content ) ;
void Execute_Rest_of_maybe_relational_exp( Content &content ) ;
void Execute_Maybe_shift_exp( Content &content ) ;
void Execute_Rest_of_maybe_shift_exp( Content &content ) ;
void Execute_Maybe_additive_exp( Content &content ) ;
void Execute_Rest_of_maybe_additive_exp( Content &content ) ;
void Execute_Maybe_mult_exp( Content &content ) ;
void Execute_Rest_of_maybe_mult_exp( Content &content ) ;
void Execute_Unary_exp( Content &content ) ;
void Execute_Signed_unary_exp( Content &content ) ;
void Execute_Unsigned_unary_exp( Content &content ) ;

/*
Assign :
    =
    TE
    DE
    ME
    DE
    RE
    PE
    PP
    MM
    !
Compare :
    ==
    !=
    >=
    <=
    >
    <
Logical :
    &&
    ||
Shift :
    <<
    >>
Arithmetic :
    +
    -
    *
    /
    %
*/

// CONTENT_FUNCT_START------------------------------------------------------------------
void Content::Calculation( Token operate, Content content ) {
  if ( m_type == "int" ) {
    if ( operate.m_type == "!" )  m_int = !m_int ;
    else if ( operate.m_type == "PP" )  m_int++ ;
    else if ( operate.m_type == "MM" )  m_int-- ;
    else if ( content.m_type == "int" ) {
      // Assign
      if ( operate.m_type == "=" )  m_int = content.m_int ;
      else if ( operate.m_type == "TE" )  m_int *= content.m_int ;
      else if ( operate.m_type == "DE" )  m_int /= content.m_int ;
      else if ( operate.m_type == "RE" )  m_int %= content.m_int ;
      else if ( operate.m_type == "PE" )  m_int += content.m_int ;
      else if ( operate.m_type == "ME" )  m_int -= content.m_int ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_int == content.m_int, m_int = 0 ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_int != content.m_int, m_int = 0 ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_int > content.m_int, m_int = 0 ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_int < content.m_int, m_int = 0 ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_int >= content.m_int, m_int = 0 ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_int <= content.m_int, m_int = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )  m_type = "bool", m_bool = m_int && content.m_int, m_int = 0 ;
      else if ( operate.m_type == "OR" )  m_type = "bool", m_bool = m_int || content.m_int, m_int = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  m_int = m_int << content.m_int ;
      else if ( operate.m_type == "RS" )  m_int = m_int >> content.m_int ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_int += content.m_int ;
      else if ( operate.m_type == "-" )  m_int -= content.m_int ;
      else if ( operate.m_type == "*" )  m_int *= content.m_int ;
      else if ( operate.m_type == "/" )  m_int /= content.m_int ;
      else if ( operate.m_type == "%" )  m_int %= content.m_int ;
    } // else if
    else if ( content.m_type == "char" )  throw "char is incompatible with int." ;
    else if ( content.m_type == "float" ) {
      // Assign
      if ( operate.m_type == "=" )  m_int = content.m_float ;
      else if ( operate.m_type == "TE" )  m_int *= content.m_float ;
      else if ( operate.m_type == "DE" )  m_int /= content.m_float ;
      else if ( operate.m_type == "RE" )  throw "cannot RE float to int." ;
      else if ( operate.m_type == "PE" )  m_int += content.m_float ;
      else if ( operate.m_type == "ME" )  m_int -= content.m_float ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_int == content.m_float, m_int = 0 ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_int != content.m_float, m_int = 0 ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_int > content.m_float, m_int = 0 ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_int < content.m_float, m_int = 0 ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_int >= content.m_float, m_int = 0 ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_int <= content.m_float, m_int = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )  m_type = "bool", m_bool = m_int && content.m_float, m_int = 0 ;
      else if ( operate.m_type == "OR" )  m_type = "bool", m_bool = m_int || content.m_float, m_int = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS float to int." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS float to int." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_int += content.m_float ;
      else if ( operate.m_type == "-" )  m_int -= content.m_float ;
      else if ( operate.m_type == "*" )  m_int *= content.m_float ;
      else if ( operate.m_type == "/" )  m_int /= content.m_float ;
      else if ( operate.m_type == "%" )  throw "cannot % float to int." ;
    } // else if
    else if ( content.m_type == "bool" ) {
      // Assign
      if ( operate.m_type == "=" )  m_int = content.m_bool ;
      else if ( operate.m_type == "TE" )  m_int *= content.m_bool ;
      else if ( operate.m_type == "DE" )  m_int /= content.m_bool ;
      else if ( operate.m_type == "RE" )  m_int %= content.m_bool ;
      else if ( operate.m_type == "PE" )  m_int += content.m_bool ;
      else if ( operate.m_type == "ME" )  m_int -= content.m_bool ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_int == content.m_bool, m_int = 0 ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_int != content.m_bool, m_int = 0 ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_int > content.m_bool, m_int = 0 ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_int < content.m_bool, m_int = 0 ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_int >= content.m_bool, m_int = 0 ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_int <= content.m_bool, m_int = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )  m_type = "bool", m_bool = m_int && content.m_bool, m_int = 0 ;
      else if ( operate.m_type == "OR" )  m_type = "bool", m_bool = m_int || content.m_bool, m_int = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  m_int = m_int << content.m_bool ;
      else if ( operate.m_type == "RS" )  m_int = m_int >> content.m_bool ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_int += content.m_bool ;
      else if ( operate.m_type == "-" )  m_int -= content.m_bool ;
      else if ( operate.m_type == "*" )  m_int *= content.m_bool ;
      else if ( operate.m_type == "/" )  m_int /= content.m_bool ;
      else if ( operate.m_type == "%" )  m_int %= content.m_bool ;
    } // else if
    else if ( content.m_type == "string" )  throw "string is incompatible with int." ;
  } // if
  else if ( m_type == "char" ) {
    if ( operate.m_type == "!" )  m_char = !m_char ;
    else if ( operate.m_type == "PP" )  m_char++ ;
    else if ( operate.m_type == "MM" )  m_char-- ;
    else if ( content.m_type == "int" ) {
      // Assign
      if ( operate.m_type == "=" )  m_char = content.m_int ;
      else if ( operate.m_type == "TE" )  m_char *= content.m_int ;
      else if ( operate.m_type == "DE" )  m_char /= content.m_int ;
      else if ( operate.m_type == "RE" )  m_char %= content.m_int ;
      else if ( operate.m_type == "PE" )  m_char += content.m_int ;
      else if ( operate.m_type == "ME" )  m_char -= content.m_int ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_char == content.m_int, m_char = '\0' ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_char != content.m_int, m_char = '\0' ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_char > content.m_int, m_char = '\0' ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_char < content.m_int, m_char = '\0' ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_char >= content.m_int, m_char = '\0' ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_char <= content.m_int, m_char = '\0' ;
      // Logical
      else if ( operate.m_type == "AND" )  m_type = "bool", m_bool = m_char && content.m_int, m_char = '\0' ;
      else if ( operate.m_type == "OR" )  m_type = "bool", m_bool = m_char || content.m_int, m_char = '\0' ;
      // Shift
      else if ( operate.m_type == "LS" )  m_char = m_char << content.m_int ;
      else if ( operate.m_type == "RS" )  m_char = m_char >> content.m_int ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_char += content.m_int ;
      else if ( operate.m_type == "-" )  m_char -= content.m_int ;
      else if ( operate.m_type == "*" )  m_char *= content.m_int ;
      else if ( operate.m_type == "/" )  m_char /= content.m_int ;
      else if ( operate.m_type == "%" )  m_char %= content.m_int ;
    } // else if
    else if ( content.m_type == "char" ) {
      // Assign
      if ( operate.m_type == "=" )  m_char = content.m_char ;
      else if ( operate.m_type == "TE" )  m_char *= content.m_char ;
      else if ( operate.m_type == "DE" )  m_char /= content.m_char ;
      else if ( operate.m_type == "RE" )  m_char %= content.m_char ;
      else if ( operate.m_type == "PE" )  m_char += content.m_char ;
      else if ( operate.m_type == "ME" )  m_char -= content.m_char ;
      // Compare
      else if ( operate.m_type == "EQ" )
        m_type = "bool", m_bool = m_char == content.m_char, m_char = '\0' ;
      else if ( operate.m_type == "NEQ" )
        m_type = "bool", m_bool = m_char != content.m_char, m_char = '\0' ;
      else if ( operate.m_type == ">" )
        m_type = "bool", m_bool = m_char > content.m_char, m_char = '\0' ;
      else if ( operate.m_type == "<" )
        m_type = "bool", m_bool = m_char < content.m_char, m_char = '\0' ;
      else if ( operate.m_type == "GE" )
        m_type = "bool", m_bool = m_char >= content.m_char, m_char = '\0' ;
      else if ( operate.m_type == "LE" )
        m_type = "bool", m_bool = m_char <= content.m_char, m_char = '\0' ;
      // Logical
      else if ( operate.m_type == "AND" )
        m_type = "bool", m_bool = m_char && content.m_char, m_char = '\0' ;
      else if ( operate.m_type == "OR" )
        m_type = "bool", m_bool = m_char || content.m_char, m_char = '\0' ;
      // Shift
      else if ( operate.m_type == "LS" )  m_char = m_char << content.m_char ;
      else if ( operate.m_type == "RS" )  m_char = m_char >> content.m_char ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_char += content.m_char ;
      else if ( operate.m_type == "-" )  m_char -= content.m_char ;
      else if ( operate.m_type == "*" )  m_char *= content.m_char ;
      else if ( operate.m_type == "/" )  m_char /= content.m_char ;
      else if ( operate.m_type == "%" )  m_char %= content.m_char ;
    } // else if
    else if ( content.m_type == "float" ) {
      // Assign
      if ( operate.m_type == "=" )  m_char = content.m_float ;
      else if ( operate.m_type == "TE" )  m_char *= content.m_float ;
      else if ( operate.m_type == "DE" )  m_char /= content.m_float ;
      else if ( operate.m_type == "RE" )  throw "cannot RE float to char." ;
      else if ( operate.m_type == "PE" )  m_char += content.m_float ;
      else if ( operate.m_type == "ME" )  m_char -= content.m_float ;
      // Compare
      else if ( operate.m_type == "EQ" )
        m_type = "bool", m_bool = m_char == content.m_float, m_char = '\0' ;
      else if ( operate.m_type == "NEQ" )
        m_type = "bool", m_bool = m_char != content.m_float, m_char = '\0' ;
      else if ( operate.m_type == ">" )
        m_type = "bool", m_bool = m_char > content.m_float, m_char = '\0' ;
      else if ( operate.m_type == "<" )
        m_type = "bool", m_bool = m_char < content.m_float, m_char = '\0' ;
      else if ( operate.m_type == "GE" )
        m_type = "bool", m_bool = m_char >= content.m_float, m_char = '\0' ;
      else if ( operate.m_type == "LE" )
        m_type = "bool", m_bool = m_char <= content.m_float, m_char = '\0' ;
      // Logical
      else if ( operate.m_type == "AND" )
        m_type = "bool", m_bool = m_char && content.m_float, m_char = '\0' ;
      else if ( operate.m_type == "OR" )
        m_type = "bool", m_bool = m_char || content.m_float, m_char = '\0' ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS float to char." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS float to char." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_char += content.m_float ;
      else if ( operate.m_type == "-" )  m_char -= content.m_float ;
      else if ( operate.m_type == "*" )  m_char *= content.m_float ;
      else if ( operate.m_type == "/" )  m_char /= content.m_float ;
      else if ( operate.m_type == "%" )  throw "cannot % float to char." ;
    } // else if
    else if ( content.m_type == "bool" ) {
      // Assign
      if ( operate.m_type == "=" )  m_char = content.m_bool ;
      else if ( operate.m_type == "TE" )  m_char *= content.m_bool ;
      else if ( operate.m_type == "DE" )  m_char /= content.m_bool ;
      else if ( operate.m_type == "RE" )  m_char %= content.m_bool ;
      else if ( operate.m_type == "PE" )  m_char += content.m_bool ;
      else if ( operate.m_type == "ME" )  m_char -= content.m_bool ;
      // Compare
      else if ( operate.m_type == "EQ" )
        m_type = "bool", m_bool = m_char == content.m_bool, m_char = '\0' ;
      else if ( operate.m_type == "NEQ" )
        m_type = "bool", m_bool = m_char != content.m_bool, m_char = '\0' ;
      else if ( operate.m_type == ">" )
        m_type = "bool", m_bool = m_char > content.m_bool, m_char = '\0' ;
      else if ( operate.m_type == "<" )
        m_type = "bool", m_bool = m_char < content.m_bool, m_char = '\0' ;
      else if ( operate.m_type == "GE" )
        m_type = "bool", m_bool = m_char >= content.m_bool, m_char = '\0' ;
      else if ( operate.m_type == "LE" )
        m_type = "bool", m_bool = m_char <= content.m_bool, m_char = '\0' ;
      // Logical
      else if ( operate.m_type == "AND" )
        m_type = "bool", m_bool = m_char && content.m_bool, m_char = '\0' ;
      else if ( operate.m_type == "OR" )
        m_type = "bool", m_bool = m_char || content.m_bool, m_char = '\0' ;
      // Shift
      else if ( operate.m_type == "LS" )  m_char = m_char << content.m_bool ;
      else if ( operate.m_type == "RS" )  m_char = m_char >> content.m_bool ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_char += content.m_bool ;
      else if ( operate.m_type == "-" )  m_char -= content.m_bool ;
      else if ( operate.m_type == "*" )  m_char *= content.m_bool ;
      else if ( operate.m_type == "/" )  m_char /= content.m_bool ;
      else if ( operate.m_type == "%" )  m_char %= content.m_bool ;
    } // else if
    else if ( content.m_type == "string" )  throw "string is incompatible with char." ;
  } // else if
  else if ( m_type == "float" ) {
    if ( operate.m_type == "!" )  m_float = !m_float ;
    else if ( operate.m_type == "PP" )  m_float++ ;
    else if ( operate.m_type == "MM" )  m_float-- ;
    else if ( content.m_type == "int" ) {
      // Assign
      if ( operate.m_type == "=" )  m_float = content.m_int ;
      else if ( operate.m_type == "TE" )  m_float *= content.m_int ;
      else if ( operate.m_type == "DE" )  m_float /= content.m_int ;
      else if ( operate.m_type == "RE" )  throw "cannot RE int to float." ;
      else if ( operate.m_type == "PE" )  m_float += content.m_int ;
      else if ( operate.m_type == "ME" )  m_float -= content.m_int ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_float == content.m_int, m_float = 0 ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_float != content.m_int, m_float = 0 ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_float > content.m_int, m_float = 0 ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_float < content.m_int, m_float = 0 ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_float >= content.m_int, m_float = 0 ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_float <= content.m_int, m_float = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )  m_type = "bool", m_bool = m_float && content.m_int, m_float = 0 ;
      else if ( operate.m_type == "OR" )  m_type = "bool", m_bool = m_float || content.m_int, m_float = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS int to float." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS int to float." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_float += content.m_int ;
      else if ( operate.m_type == "-" )  m_float -= content.m_int ;
      else if ( operate.m_type == "*" )  m_float *= content.m_int ;
      else if ( operate.m_type == "/" )  m_float /= content.m_int ;
      else if ( operate.m_type == "%" )  throw "cannot % int to float" ;
    } // else if
    else if ( content.m_type == "char" ) {
      // Assign
      if ( operate.m_type == "=" )  m_float = content.m_char ;
      else if ( operate.m_type == "TE" )  m_float *= content.m_char ;
      else if ( operate.m_type == "DE" )  m_float /= content.m_char ;
      else if ( operate.m_type == "RE" )  throw "cannot RE char to float." ;
      else if ( operate.m_type == "PE" )  m_float += content.m_char ;
      else if ( operate.m_type == "ME" )  m_float -= content.m_char ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_float == content.m_char, m_float = 0 ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_float != content.m_char, m_float = 0 ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_float > content.m_char, m_float = 0 ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_float < content.m_char, m_float = 0 ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_float >= content.m_char, m_float = 0 ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_float <= content.m_char, m_float = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )
        m_type = "bool", m_bool = m_float && content.m_char, m_float = 0 ;
      else if ( operate.m_type == "OR" )
        m_type = "bool", m_bool = m_float || content.m_char, m_float = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS char to float." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS char to float." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_float += content.m_char ;
      else if ( operate.m_type == "-" )  m_float -= content.m_char ;
      else if ( operate.m_type == "*" )  m_float *= content.m_char ;
      else if ( operate.m_type == "/" )  m_float /= content.m_char ;
      else if ( operate.m_type == "%" )  throw "cannot % char to float" ;
    } // else if
    else if ( content.m_type == "float" ) {
      // Assign
      if ( operate.m_type == "=" )  m_float = content.m_float ;
      else if ( operate.m_type == "TE" )  m_float *= content.m_float ;
      else if ( operate.m_type == "DE" )  m_float /= content.m_float ;
      else if ( operate.m_type == "RE" )  throw "cannot RE float to float." ;
      else if ( operate.m_type == "PE" )  m_float += content.m_float ;
      else if ( operate.m_type == "ME" )  m_float -= content.m_float ;
      // Compare
      else if ( operate.m_type == "EQ" )
        m_type = "bool", m_bool = m_float == content.m_float, m_float = 0 ;
      else if ( operate.m_type == "NEQ" )
        m_type = "bool", m_bool = m_float != content.m_float, m_float = 0 ;
      else if ( operate.m_type == ">" )
        m_type = "bool", m_bool = m_float > content.m_float, m_float = 0 ;
      else if ( operate.m_type == "<" )
        m_type = "bool", m_bool = m_float < content.m_float, m_float = 0 ;
      else if ( operate.m_type == "GE" )
        m_type = "bool", m_bool = m_float >= content.m_float, m_float = 0 ;
      else if ( operate.m_type == "LE" )
        m_type = "bool", m_bool = m_float <= content.m_float, m_float = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )
        m_type = "bool", m_bool = m_float && content.m_float, m_float = 0 ;
      else if ( operate.m_type == "OR" )
        m_type = "bool", m_bool = m_float || content.m_float, m_float = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS float to float." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS float to float." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_float += content.m_float ;
      else if ( operate.m_type == "-" )  m_float -= content.m_float ;
      else if ( operate.m_type == "*" )  m_float *= content.m_float ;
      else if ( operate.m_type == "/" )  m_float /= content.m_float ;
      else if ( operate.m_type == "%" )  throw "cannot % float to float" ;
    } // else if
    else if ( content.m_type == "bool" ) {
      // Assign
      if ( operate.m_type == "=" )  m_float = content.m_bool ;
      else if ( operate.m_type == "TE" )  m_float *= content.m_bool ;
      else if ( operate.m_type == "DE" )  m_float /= content.m_bool ;
      else if ( operate.m_type == "RE" )  throw "cannot RE bool to float." ;
      else if ( operate.m_type == "PE" )  m_float += content.m_bool ;
      else if ( operate.m_type == "ME" )  m_float -= content.m_bool ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_type = "bool", m_bool = m_float == content.m_bool, m_float = 0 ;
      else if ( operate.m_type == "NEQ" )  m_type = "bool", m_bool = m_float != content.m_bool, m_float = 0 ;
      else if ( operate.m_type == ">" )  m_type = "bool", m_bool = m_float > content.m_bool, m_float = 0 ;
      else if ( operate.m_type == "<" )  m_type = "bool", m_bool = m_float < content.m_bool, m_float = 0 ;
      else if ( operate.m_type == "GE" )  m_type = "bool", m_bool = m_float >= content.m_bool, m_float = 0 ;
      else if ( operate.m_type == "LE" )  m_type = "bool", m_bool = m_float <= content.m_bool, m_float = 0 ;
      // Logical
      else if ( operate.m_type == "AND" )  m_type = "bool", m_bool = m_float && content.m_bool, m_float = 0 ;
      else if ( operate.m_type == "OR" )  m_type = "bool", m_bool = m_float || content.m_bool, m_float = 0 ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS bool to float." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS bool to float." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_float += content.m_bool ;
      else if ( operate.m_type == "-" )  m_float -= content.m_bool ;
      else if ( operate.m_type == "*" )  m_float *= content.m_bool ;
      else if ( operate.m_type == "/" )  m_float /= content.m_bool ;
      else if ( operate.m_type == "%" )  throw "cannot % bool to float" ;
    } // else if
    else if ( content.m_type == "string" )  throw "string is incompatible with float." ;
  } // else if
  else if ( m_type == "bool" ) {
    if ( operate.m_type == "!" )  m_bool = !m_bool ;
    else if ( operate.m_type == "PP" )  m_bool++ ;
    else if ( operate.m_type == "MM" )  throw "cannot MM bool." ;
    else if ( content.m_type == "int" ) {
      // Assign
      if ( operate.m_type == "=" )  m_bool = content.m_int ;
      else if ( operate.m_type == "TE" )  m_bool *= content.m_int ;
      else if ( operate.m_type == "DE" )  m_bool /= content.m_int ;
      else if ( operate.m_type == "RE" )  m_bool %= content.m_int ;
      else if ( operate.m_type == "PE" )  m_bool += content.m_int ;
      else if ( operate.m_type == "ME" )  m_bool -= content.m_int ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_bool = m_bool == content.m_int ;
      else if ( operate.m_type == "NEQ" )  m_bool = m_bool != content.m_int ;
      else if ( operate.m_type == ">" )  m_bool = m_bool > content.m_int ;
      else if ( operate.m_type == "<" )  m_bool = m_bool < content.m_int ;
      else if ( operate.m_type == "GE" )  m_bool = m_bool >= content.m_int ;
      else if ( operate.m_type == "LE" )  m_bool = m_bool <= content.m_int ;
      // Logical
      else if ( operate.m_type == "AND" )  m_bool = m_bool && content.m_int ;
      else if ( operate.m_type == "OR" )  m_bool = m_bool || content.m_int ;
      // Shift
      else if ( operate.m_type == "LS" )  m_bool = m_bool << content.m_int ;
      else if ( operate.m_type == "RS" )  m_bool = m_bool >> content.m_int ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_bool += content.m_int ;
      else if ( operate.m_type == "-" )  m_bool -= content.m_int ;
      else if ( operate.m_type == "*" )  m_bool *= content.m_int ;
      else if ( operate.m_type == "/" )  m_bool /= content.m_int ;
      else if ( operate.m_type == "%" )  m_bool %= content.m_int ;
    } // else if
    else if ( content.m_type == "char" ) {
      // Assign
      if ( operate.m_type == "=" )  m_bool = content.m_char ;
      else if ( operate.m_type == "TE" )  m_bool *= content.m_char ;
      else if ( operate.m_type == "DE" )  m_bool /= content.m_char ;
      else if ( operate.m_type == "RE" )  m_bool %= content.m_char ;
      else if ( operate.m_type == "PE" )  m_bool += content.m_char ;
      else if ( operate.m_type == "ME" )  m_bool -= content.m_char ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_bool = m_bool == content.m_char ;
      else if ( operate.m_type == "NEQ" )  m_bool = m_bool != content.m_char ;
      else if ( operate.m_type == ">" )  m_bool = m_bool > content.m_char ;
      else if ( operate.m_type == "<" )  m_bool = m_bool < content.m_char ;
      else if ( operate.m_type == "GE" )  m_bool = m_bool >= content.m_char ;
      else if ( operate.m_type == "LE" )  m_bool = m_bool <= content.m_char ;
      // Logical
      else if ( operate.m_type == "AND" )  m_bool = m_bool && content.m_char ;
      else if ( operate.m_type == "OR" )  m_bool = m_bool || content.m_char ;
      // Shift
      else if ( operate.m_type == "LS" )  m_bool = m_bool << content.m_char ;
      else if ( operate.m_type == "RS" )  m_bool = m_bool >> content.m_char ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_bool += content.m_char ;
      else if ( operate.m_type == "-" )  m_bool -= content.m_char ;
      else if ( operate.m_type == "*" )  m_bool *= content.m_char ;
      else if ( operate.m_type == "/" )  m_bool /= content.m_char ;
      else if ( operate.m_type == "%" )  m_bool %= content.m_char ;
    } // else if
    else if ( content.m_type == "float" ) {
      // Assign
      if ( operate.m_type == "=" )  m_bool = content.m_float ;
      else if ( operate.m_type == "TE" )  m_bool *= content.m_float ;
      else if ( operate.m_type == "DE" )  m_bool /= content.m_float ;
      else if ( operate.m_type == "RE" )  throw "cannot RE float to bool." ;
      else if ( operate.m_type == "PE" )  m_bool += content.m_float ;
      else if ( operate.m_type == "ME" )  m_bool -= content.m_float ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_bool = m_bool == content.m_float ;
      else if ( operate.m_type == "NEQ" )  m_bool = m_bool != content.m_float ;
      else if ( operate.m_type == ">" )  m_bool = m_bool > content.m_float ;
      else if ( operate.m_type == "<" )  m_bool = m_bool < content.m_float ;
      else if ( operate.m_type == "GE" )  m_bool = m_bool >= content.m_float ;
      else if ( operate.m_type == "LE" )  m_bool = m_bool <= content.m_float ;
      // Logical
      else if ( operate.m_type == "AND" )  m_bool = m_bool && content.m_float ;
      else if ( operate.m_type == "OR" )  m_bool = m_bool || content.m_float ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS float to bool." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS float to bool." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_bool += content.m_float ;
      else if ( operate.m_type == "-" )  m_bool -= content.m_float ;
      else if ( operate.m_type == "*" )  m_bool *= content.m_float ;
      else if ( operate.m_type == "/" )  m_bool /= content.m_float ;
      else if ( operate.m_type == "%" )  throw "cannot % float to bool." ;
    } // else if
    else if ( content.m_type == "bool" ) {
      // Assign
      if ( operate.m_type == "=" )  m_bool = content.m_bool ;
      else if ( operate.m_type == "TE" )  m_bool *= content.m_bool ;
      else if ( operate.m_type == "DE" )  m_bool /= content.m_bool ;
      else if ( operate.m_type == "RE" )  m_bool %= content.m_bool ;
      else if ( operate.m_type == "PE" )  m_bool += content.m_bool ;
      else if ( operate.m_type == "ME" )  m_bool -= content.m_bool ;
      // Compare
      else if ( operate.m_type == "EQ" )  m_bool = m_bool == content.m_bool ;
      else if ( operate.m_type == "NEQ" )  m_bool = m_bool != content.m_bool ;
      else if ( operate.m_type == ">" )  m_bool = m_bool > content.m_bool ;
      else if ( operate.m_type == "<" )  m_bool = m_bool < content.m_bool ;
      else if ( operate.m_type == "GE" )  m_bool = m_bool >= content.m_bool ;
      else if ( operate.m_type == "LE" )  m_bool = m_bool <= content.m_bool ;
      // Logical
      else if ( operate.m_type == "AND" )  m_bool = ( m_bool && content.m_bool ) ;
      else if ( operate.m_type == "OR" )  m_bool = m_bool || content.m_bool ;
      // Shift
      else if ( operate.m_type == "LS" )  m_bool = m_bool << content.m_bool ;
      else if ( operate.m_type == "RS" )  m_bool = m_bool >> content.m_bool ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_bool += content.m_bool ;
      else if ( operate.m_type == "-" )  m_bool -= content.m_bool ;
      else if ( operate.m_type == "*" )  m_bool *= content.m_bool ;
      else if ( operate.m_type == "/" )  m_bool /= content.m_bool ;
      else if ( operate.m_type == "%" )  m_bool %= content.m_bool ;
    } // else if
    else if ( content.m_type == "string" )  throw "string is incompatible with bool." ;
  } // else if
  else if ( m_type == "string" ) {
    if ( operate.m_type == "!" )  throw "cannot ! string." ;
    else if ( operate.m_type == "PP" )  throw "cannot PP string." ;
    else if ( operate.m_type == "MM" )  throw "cannot MM string." ;
    else if ( content.m_type == "int" ) {
      // Assign
      if ( operate.m_type == "=" )  m_string = content.m_int ;
      else if ( operate.m_type == "TE" )  throw "cannot TE int to string." ;
      else if ( operate.m_type == "DE" )  throw "cannot DE int to string." ;
      else if ( operate.m_type == "RE" )  throw "cannot RE int to string." ;
      else if ( operate.m_type == "PE" )  m_string += content.m_int ;
      else if ( operate.m_type == "ME" )  throw "cannot ME int to string." ;
      // Compare
      else if ( operate.m_type == "EQ" )  throw "cannot EQ int to string." ;
      else if ( operate.m_type == "NEQ" )  throw "cannot NEQ int to string." ;
      else if ( operate.m_type == ">" )  throw "cannot > int to string." ;
      else if ( operate.m_type == "<" )  throw "cannot < int to string." ;
      else if ( operate.m_type == "GE" )  throw "cannot GE int to string." ;
      else if ( operate.m_type == "LE" )  throw "cannot LE int to string." ;
      // Logical
      else if ( operate.m_type == "AND" )  throw "cannot && int to string." ;
      else if ( operate.m_type == "OR" )  throw "cannot || int to string." ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS int to string." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS int to string." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_string += content.m_int ;
      else if ( operate.m_type == "-" )  throw "cannot - int to string" ;
      else if ( operate.m_type == "*" )  throw "cannot * int to string" ;
      else if ( operate.m_type == "/" )  throw "cannot / int to string" ;
      else if ( operate.m_type == "%" )  throw "cannot % int to string" ;
    } // else if
    else if ( content.m_type == "char" ) {
      // Assign
      if ( operate.m_type == "=" )  m_string = content.m_char ;
      else if ( operate.m_type == "TE" )  throw "cannot TE char to string." ;
      else if ( operate.m_type == "DE" )  throw "cannot DE char to string." ;
      else if ( operate.m_type == "RE" )  throw "cannot RE char to string." ;
      else if ( operate.m_type == "PE" )  m_string += content.m_char ;
      else if ( operate.m_type == "ME" )  throw "cannot ME char to string." ;
      // Compare
      else if ( operate.m_type == "EQ" )  throw "cannot EQ char to string" ;
      else if ( operate.m_type == "NEQ" )  throw "cannot NEQ char to string." ;
      else if ( operate.m_type == ">" )  throw "cannot > char to string." ;
      else if ( operate.m_type == "<" )  throw "cannot < char to string." ;
      else if ( operate.m_type == "GE" )  throw "cannot GE char to string." ;
      else if ( operate.m_type == "LE" )  throw "cannot LE char to string." ;
      // Logical
      else if ( operate.m_type == "AND" )  throw "cannot && char to string." ;
      else if ( operate.m_type == "OR" )  throw "cannot || char to string." ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS char to string" ;
      else if ( operate.m_type == "RS" )  throw "cannot RS char to string." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_string += content.m_char ;
      else if ( operate.m_type == "-" )  throw "cannot - char to string" ;
      else if ( operate.m_type == "*" )  throw "cannot * char to string" ;
      else if ( operate.m_type == "/" )  throw "cannot / char to string" ;
      else if ( operate.m_type == "%" )  throw "cannot % char to string" ;
    } // else if
    else if ( content.m_type == "float" ) {
      // Assign
      if ( operate.m_type == "=" )  m_string = content.m_float ;
      else if ( operate.m_type == "TE" )  throw "cannot TE float to string." ;
      else if ( operate.m_type == "DE" )  throw "cannot DE float to string." ;
      else if ( operate.m_type == "RE" )  throw "cannot RE float to string." ;
      else if ( operate.m_type == "PE" )  m_string += content.m_float ;
      else if ( operate.m_type == "ME" )  throw "cannot ME float to string." ;
      // Compare
      else if ( operate.m_type == "EQ" )  throw "cannot EQ float to string." ;
      else if ( operate.m_type == "NEQ" )  throw "cannot NEQ float to string." ;
      else if ( operate.m_type == ">" )  throw "cannot > float to string." ;
      else if ( operate.m_type == "<" )  throw "cannot < float to string." ;
      else if ( operate.m_type == "GE" )  throw "cannot GE float to string." ;
      else if ( operate.m_type == "LE" )  throw "cannot LE float to string." ;
      // Logical
      else if ( operate.m_type == "AND" )  throw "cannot && float to string." ;
      else if ( operate.m_type == "OR" )  throw "cannot || float to string." ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS float to string." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS float to string." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_string += content.m_float ;
      else if ( operate.m_type == "-" )  throw "cannot - float to string" ;
      else if ( operate.m_type == "*" )  throw "cannot * float to string" ;
      else if ( operate.m_type == "/" )  throw "cannot / float to string" ;
      else if ( operate.m_type == "%" )  throw "cannot % float to string" ;
    } // else if
    else if ( content.m_type == "bool" ) {
      // Assign
      if ( operate.m_type == "=" )  m_string = content.m_bool ;
      else if ( operate.m_type == "TE" )  throw "cannot TE bool to string." ;
      else if ( operate.m_type == "DE" )  throw "cannot DE bool to string." ;
      else if ( operate.m_type == "RE" )  throw "cannot RE bool to string." ;
      else if ( operate.m_type == "PE" )  m_string += content.m_bool ;
      else if ( operate.m_type == "ME" )  throw "cannot ME bool to string." ;
      // Compare
      else if ( operate.m_type == "EQ" )  throw "cannot EQ bool to string." ;
      else if ( operate.m_type == "NEQ" )  throw "cannot NEQ bool to string." ;
      else if ( operate.m_type == ">" )  throw "cannot > bool to string." ;
      else if ( operate.m_type == "<" )  throw "cannot < bool to string." ;
      else if ( operate.m_type == "GE" )  throw "cannot GE bool to string." ;
      else if ( operate.m_type == "LE" )  throw "cannot LE bool to string." ;
      // Logical
      else if ( operate.m_type == "AND" )  throw "cannot && bool to string." ;
      else if ( operate.m_type == "OR" )  throw "cannot || bool to string." ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS bool to string." ;
      else if ( operate.m_type == "RS" )  throw "cannot RS bool to string." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_string += content.m_bool ;
      else if ( operate.m_type == "-" )  throw "cannot - bool to string" ;
      else if ( operate.m_type == "*" )  throw "cannot * bool to string" ;
      else if ( operate.m_type == "/" )  throw "cannot / bool to string" ;
      else if ( operate.m_type == "%" )  throw "cannot % bool to string" ;
    } // else if
    else if ( content.m_type == "string" ) {
      // Assign
      if ( operate.m_type == "=" )  m_string = content.m_string ;
      else if ( operate.m_type == "TE" )  throw "cannot TE string to string." ;
      else if ( operate.m_type == "DE" )  throw "cannot DE string to string." ;
      else if ( operate.m_type == "RE" )  throw "cannot RE string to string." ;
      else if ( operate.m_type == "PE" )  m_string += content.m_string ;
      else if ( operate.m_type == "ME" )  throw "cannot ME string to string." ;
      // Compare
      else if ( operate.m_type == "EQ" )
        m_type = "bool", m_bool = m_string == content.m_string, m_string = "" ;
      else if ( operate.m_type == "NEQ" )
        m_type = "bool", m_bool = m_string != content.m_string, m_string = "" ;
      else if ( operate.m_type == ">" )
        m_type = "bool", m_bool = m_string > content.m_string, m_string = "" ;
      else if ( operate.m_type == "<" )
        m_type = "bool", m_bool = m_string < content.m_string, m_string = "" ;
      else if ( operate.m_type == "GE" )
        m_type = "bool", m_bool = m_string >= content.m_string, m_string = "" ;
      else if ( operate.m_type == "LE" )
        m_type = "bool", m_bool = m_string <= content.m_string, m_string = "" ;
      // Logical
      else if ( operate.m_type == "AND" )  throw "cannot && string to string." ;
      else if ( operate.m_type == "OR" )  throw "cannot || string to string." ;
      // Shift
      else if ( operate.m_type == "LS" )  throw "cannot LS string to string" ;
      else if ( operate.m_type == "RS" )  throw "cannot RS string to string." ;
      // Arithmetic
      else if ( operate.m_type == "+" )  m_string += content.m_string ;
      else if ( operate.m_type == "-" )  throw "cannot - string to string" ;
      else if ( operate.m_type == "*" )  throw "cannot * string to string" ;
      else if ( operate.m_type == "/" )  throw "cannot / string to string" ;
      else if ( operate.m_type == "%" )  throw "cannot % string to string" ;
    } // else if
  } // else if
} // Content::Calculation()

void Content::Print() {
  if ( m_type == "int" )
    cout << m_int ;
  else if ( m_type == "char" ) {
    cout << m_char ;
  } // else if
  else if ( m_type == "float" )
    cout << m_float ;
  else if ( m_type == "bool" ) {
    if ( m_bool )
      cout << "true" ;
    else
      cout << "false" ;
  } // else if
  else if ( m_type == "string" ) {
    for ( int i = 0 ; i < m_string.size() ; i++ ) {
      if ( m_string[i] == '\\' ) {
        if ( i+1 < m_string.size() ) {
          if ( m_string[i+1] == 'n' )
            cout << '\n' ;
          else if ( m_string[i+1] == 't' )
            cout << '\t' ;
          else if ( m_string[i+1] == '\\' )
            cout << '\\' ;
          else if ( m_string[i+1] == '0' )
            cout << '\0' ;
          i++ ;
        } // if
        else
          cout << m_string[i] ;
      } // if
      else
        cout << m_string[i] ;
    } // for
  } // else if
} // Content::Print()
// CONTENT_FUNCT_END--------------------------------------------------------------------

// IDENT_FUNCT_START--------------------------------------------------------------------
void Ident::Print() {
  if ( m_var_or_funct == "var" ) {
    m_content.Print() ;
  } // if
  else if ( m_var_or_funct == "funct" ) {
    cout << m_content.m_funct << endl ;
  } // else if
} // Ident::Print()

void Ident::Redefine_Funct() {
  int layer = 0 ;
  Content content ;

  for ( int i = 0 ; i < g_TokenList.size() ; i++ ) {
    content.m_funct += g_TokenList[i].m_value ;
    if ( g_TokenList[i].m_value == "{" ) {
      layer++ ;
      content.m_funct += "\n" ;
      if ( i+1 < g_TokenList.size() && g_TokenList[i+1].m_value == "}" )
        layer-- ;
      for ( int j = 0 ; j < 2*layer ; j++ )
        content.m_funct += " " ;
    } // if
    else if ( g_TokenList[i].m_value == ";" ) {
      content.m_funct += "\n" ;
      if ( i+1 < g_TokenList.size() && g_TokenList[i+1].m_value == "}" )
        layer-- ;
      for ( int j = 0 ; j < 2*layer ; j++ )
        content.m_funct += " " ;
    } // else if
    else if ( g_TokenList[i].m_value == "}" ) {
      if ( layer > 0 )
        content.m_funct += "\n" ;
      if ( i+1 < g_TokenList.size() && g_TokenList[i+1].m_value == "}" )
        layer-- ;
      for ( int j = 0 ; j < 2*layer ; j++ )
        content.m_funct += " " ;
    } // else if
    else if ( i+1 < g_TokenList.size() && g_TokenList[i].m_type == "Identifier" &&
              ( g_TokenList[i+1].m_type == "(" || g_TokenList[i+1].m_type == "[" ||
                g_TokenList[i+1].m_type == "," || g_TokenList[i+1].m_type == "PP" ||
                g_TokenList[i+1].m_type == "MM" ) ) ;
    else if ( g_TokenList[i].m_type == "(" && i+1 < g_TokenList.size() && g_TokenList[i+1].m_value == ")" ) ;
    else
      content.m_funct += " " ;
  } // for

  m_type = g_TokenList[0].m_value ;
  m_name = g_TokenList[1].m_value ;
  m_array = "-1" ;
  m_content = content ;
  m_var_or_funct = "funct" ;
} // Ident::Redefine_Funct()

void Ident::Redefine_Var( string type_, string name_, string array_, string var_or_funct_ ) {
  Content content_ ;
  m_type = type_ ;
  m_name = name_ ;
  m_array = array_ ;
  m_content = content_ ;
  m_content.m_type = m_type ;
  m_var_or_funct = var_or_funct_ ;
} // Ident::Redefine_Var()
// IDENT_FUNCT_END----------------------------------------------------------------------

// GLOBAL_FUNCT_START-------------------------------------------------------------------
void Remove_if_statement() {
  Token token = g_TokenList[0] ;
  if ( token.m_type == "IF" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    int s_para = 1 ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    while ( s_para > 0 ) {
      token = g_TokenList[0] ;
      if ( token.m_type == "(" ) s_para++ ;
      else if ( token.m_type == ")" ) s_para-- ;
      g_TokenList.erase( g_TokenList.begin() ) ;
    } // while

    token = g_TokenList[0] ;
    if ( token.m_type == "{" ) {
      int para = 1 ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      while ( para > 0 ) {
        token = g_TokenList[0] ;
        if ( token.m_type == "{" ) para++ ;
        else if ( token.m_type == "}" ) para-- ;
        g_TokenList.erase( g_TokenList.begin() ) ;
      } // while
    } // if
    else if ( token.m_type == "IF" )
      Remove_if_statement() ;
    else {
      while ( token.m_type != ";" ) {
        g_TokenList.erase( g_TokenList.begin() ) ;
        token = g_TokenList[0] ;
      } // while

      g_TokenList.erase( g_TokenList.begin() ) ;
    } // else

    if ( !g_TokenList.empty() ) {
      token = g_TokenList[0] ;
      if ( token.m_type == "ELSE" ) {
        g_TokenList.erase( g_TokenList.begin() ) ;
        token = g_TokenList[0] ;
        if ( token.m_type == "{" ) {
          int para = 1 ;
          g_TokenList.erase( g_TokenList.begin() ) ;
          while ( para > 0 ) {
            token = g_TokenList[0] ;
            if ( token.m_type == "{" ) para++ ;
            else if ( token.m_type == "}" ) para-- ;
            g_TokenList.erase( g_TokenList.begin() ) ;
          } // while
        } // if
        else if ( token.m_type == "IF" )
          Remove_if_statement() ;
        else {
          while ( token.m_type != ";" ) {
            g_TokenList.erase( g_TokenList.begin() ) ;
            token = g_TokenList[0] ;
          } // while

          g_TokenList.erase( g_TokenList.begin() ) ;
        } // else
      } // if
    } // if
  } // if
} // Remove_if_statement()

void Sort() {
  for ( int i = 0 ; i < g_IdentTable[0].size() ; i++ ) {
    for ( int j = i ; j < g_IdentTable[0].size() ; j++ ) {
      if ( strcmp( g_IdentTable[0][i].m_name.c_str(), g_IdentTable[0][j].m_name.c_str() ) > 0 ) {
        Ident temp = g_IdentTable[0][i] ;
        g_IdentTable[0][i] = g_IdentTable[0][j] ;
        g_IdentTable[0][j] = temp ;
      } // if
    } // for
  } // for
} // Sort()

bool Ident_Exist( Token token ) {
  for ( int i = g_IdentTable.size() - 1 ; i >= 0 ; i-- ) {
    for ( int j = 0 ; j < g_IdentTable[i].size() ; j++ ) {
      if ( token.m_value == g_IdentTable[i][j].m_name )
        return true ;
    } // for
  } // for

  if ( token.m_value == "cout" || token.m_value == "cin" || token.m_value == "ListAllVariables" ||
       token.m_value == "ListAllFunctions" || token.m_value == "ListVariable" ||
       token.m_value == "ListFunction" || token.m_value == "Done" )
    return true ;

  return false ;
} // Ident_Exist()

bool Isdigit( char character ) {
  if ( character >= '0' && character <= '9' )
    return true ;
  else
    return false ;
} // Isdigit()

bool IsAlphabet( char character ) {
  if ( ( character >= 'a' && character <= 'z' ) || ( character >= 'A' && character <= 'Z' ) )
    return true ;
  else
    return false ;
} // IsAlphabet()
// GLOBAL_FUNCT_END---------------------------------------------------------------------

// RETREIVE_TOKEN_START-----------------------------------------------------------------
string Get_const_type( string token_value ) {
  string const_type = "" ;
  if ( Isdigit( token_value[0] ) || ( token_value[0] == '.'
                                      && token_value.size() > 1 ) ) {
    const_type = "int" ;
    for ( int i = 0 ; i < token_value.size() ; i++  ) {
      if ( token_value[i] == '.' )
        const_type = "float" ;
    } // for
  } // if
  else if ( token_value[0] == '\"' && token_value[token_value.size()-1] == '\"' )  const_type = "string" ;
  else if ( token_value[0] == '\'' && token_value[token_value.size()-1] == '\'' )  const_type = "char" ;
  else if ( token_value == "true" || token_value == "false" )  const_type = "bool" ;
  return const_type ;
} // Get_const_type()

string Get_token_type( string token_value ) {
  string type = "" ;
  if ( token_value == "int" )  type = "INT" ;
  else if ( token_value == "float" )  type = "FLOAT" ;
  else if ( token_value == "char" )  type = "CHAR" ;
  else if ( token_value == "bool" )  type = "BOOL" ;
  else if ( token_value == "string" )  type = "STRING" ;
  else if ( token_value == "void" )  type = "VOID" ;
  else if ( token_value == "if" )  type = "IF" ;
  else if ( token_value == "else" )  type = "ELSE" ;
  else if ( token_value == "while" )  type = "WHILE" ;
  else if ( token_value == "do" )  type = "DO" ;
  else if ( token_value == "return" )  type = "RETURN" ;
  else if ( token_value == "(" )  type = "(" ;
  else if ( token_value == ")" )  type = ")" ;
  else if ( token_value == "[" )  type = "[" ;
  else if ( token_value == "]" )  type = "]" ;
  else if ( token_value == "{" )  type = "{" ;
  else if ( token_value == "}" )  type = "}" ;
  else if ( token_value == "+" )  type = "+" ;
  else if ( token_value == "-" )  type = "-" ;
  else if ( token_value == "*" )  type = "*" ;
  else if ( token_value == "/" )  type = "/" ;
  else if ( token_value == "%" )  type = "%" ;
  else if ( token_value == "^" )  type = "^" ;
  else if ( token_value == ">" )  type = ">" ;
  else if ( token_value == "<" )  type = "<" ;
  else if ( token_value == ">=" )  type = "GE" ;
  else if ( token_value == "<=" )  type = "LE" ;
  else if ( token_value == "==" )  type = "EQ" ;
  else if ( token_value == "!=" )  type = "NEQ" ;
  else if ( token_value == "&" )  type = "&" ;
  else if ( token_value == "|" )  type = "|" ;
  else if ( token_value == "=" )  type = "=" ;
  else if ( token_value == "!" )  type = "!" ;
  else if ( token_value == "&&" )  type = "AND" ;
  else if ( token_value == "||" )  type = "OR" ;
  else if ( token_value == "+=" )  type = "PE" ;
  else if ( token_value == "-=" )  type = "ME" ;
  else if ( token_value == "*=" )  type = "TE" ;
  else if ( token_value == "/=" )  type = "DE" ;
  else if ( token_value == "%=" )  type = "RE" ;
  else if ( token_value == "++" )  type = "PP" ;
  else if ( token_value == "--" )  type = "MM" ;
  else if ( token_value == ">>" )  type = "RS" ;
  else if ( token_value == "<<" )  type = "LS" ;
  else if ( token_value == ";" )  type = ";" ;
  else if ( token_value == "," )  type = "," ;
  else if ( token_value == "?" )  type = "?" ;
  else if ( token_value == ":" )  type = ":" ;
  else if ( token_value == "." )  type = "." ;
  else if ( Isdigit( token_value[0] ) || ( token_value[0] == '.'
                                           && token_value.size() > 1 ) )  type = "Constant" ;
  else if ( token_value.size() > 1 && token_value[0] == '\"' &&
            token_value[token_value.size()-1] == '\"' )  type = "Constant" ;
  else if ( token_value.size() > 1 && token_value[0] == '\'' &&
            token_value[token_value.size()-1] == '\'' )  type = "Constant" ;
  else if ( token_value == "true" || token_value == "false" )  type = "Constant" ;
  else if ( IsAlphabet( token_value[0] ) )  type = "Identifier" ;
  else  type = "UNKNOWN" ;
  return type ;
} // Get_token_type()

char Get_next_char() {
  char one_char ;
  one_char = getchar() ;
  if ( one_char == '\n' ) {
    g_row++ ;
    g_column = 0 ;
  } // if
  else {
    g_column++ ;
  } // else

  return one_char ;
} // Get_next_char()

char Get_next_non_ws_char() {
  char one_char = Get_next_char() ;
  while ( one_char == ' ' || one_char == '\t' || one_char == '\n' ) {
    one_char = Get_next_char() ;
    if ( one_char != ' ' && one_char != '\t' && one_char != '\n' )
      return one_char ;
  } // while

  return one_char ;
} // Get_next_non_ws_char()

Token Get_next_token() {
  bool tokenfound = false, commentfound = false ;
  char head = Get_next_non_ws_char() ;
  Token token ;

  if ( head == EOF )
    return token ;
  token.m_row = g_row ;
  token.m_column = g_column ;
  token.m_value = token.m_value + head ;

  char next_char = cin.peek() ;
  while ( head != '\'' && head != '\"' && next_char != ' ' && next_char != '\t' && next_char != '\n'
          && next_char != '\0' && tokenfound == false ) {
    commentfound = false ;
    if ( head == '&' || head == '|'  ) {
      if ( next_char == head ) {
        next_char = Get_next_char() ;
        token.m_value = token.m_value + next_char ;
      } // if

      tokenfound = true ;
    } // if
    else if ( head == '+' || head == '-' || head == '/' || head == '>' || head == '<' ) {
      if ( next_char == head || next_char == '=' ) {
        next_char = Get_next_char() ;
        token.m_value = token.m_value + next_char ;
      } // if

      if ( token.m_value == "//" ) { // read in whole line of comment
        next_char = cin.peek() ;
        while ( next_char != '\n' ) {
          next_char = Get_next_char() ;
          token.m_value = token.m_value + next_char ;
          next_char = cin.peek() ;
        } // while

        head = Get_next_non_ws_char() ;
        token.m_value = head ;
        token.m_row = g_row ;
        token.m_column = g_column ;
        next_char = cin.peek() ;
        commentfound = true ;
      } // if
      else
        tokenfound = true ;
    } // else if
    else if ( head == '=' || head == '!' || head == '*' || head == '%' ) {
      if ( next_char == '=' ) {
        next_char = Get_next_char() ;
        token.m_value = token.m_value + next_char ;
      } // if

      tokenfound = true ;
    } // else if
    else if ( Isdigit( head ) ) {
      if ( !Isdigit( next_char ) && next_char != '.' )
        tokenfound = true ;
      else if ( next_char == '.' ) {
        for ( int i = 0 ; i < token.m_value.size() ; i++ ) {
          if ( token.m_value[i] == '.' )
            tokenfound = true ;
        } // for
      } // else if
    } // else if
    else if ( head == '.' ) {
      if ( !Isdigit( next_char ) )
        tokenfound = true ;
    } // else if
    else if ( head == '(' || head == ')' || head == '[' || head == ']'
              || head == '{' || head == '}' || head == '+' || head == '-'
              || head == '*'  || head == '/' || head == '%' || head == '^'
              || head == '>' || head == '<' || head == '&' || head == '|'
              || head == '=' || head == '!' || head == ';' || head == ','
              || head == '?' || head == ':' )
      tokenfound = true ;
    else if ( IsAlphabet( head ) ) {
      if ( next_char != '_' && !Isdigit( next_char ) && !IsAlphabet( next_char ) )
        tokenfound = true ;
    } // else if
    else
      tokenfound = true ;

    if ( tokenfound == false && commentfound == false ) {
      next_char = Get_next_char() ;
      token.m_value = token.m_value + next_char ;
      next_char = cin.peek() ;
    } // if
  } // while

  while ( ( head == '\'' || head == '\"' ) && tokenfound == false ) {
    if ( next_char == '\n' )
      tokenfound = true ;
    else if ( next_char == head && token.m_value[token.m_value.size()-1] != '\\' ) {
      next_char = Get_next_char() ;
      token.m_value = token.m_value + next_char ;
      tokenfound = true ;
    } // else if

    if ( tokenfound == false ) {
      next_char = Get_next_char() ;
      token.m_value = token.m_value + next_char ;
      next_char = cin.peek() ;
    } // if
  } // while

  token.m_type = Get_token_type( token.m_value ) ;
  if ( token.m_type == "Constant" )
    token.m_const_type = Get_const_type( token.m_value ) ;
  return token ;
} // Get_next_token()

Token Retrieve_Token() {
  Token token ;
  if ( g_TokenBuffer.size() > 0 ) {
    token = g_TokenBuffer[0] ;
    g_TokenBuffer.erase( g_TokenBuffer.begin() ) ;
  } // if
  else
    token = Get_next_token() ;

  return token ;
} // Retrieve_Token()
// RETREIVE_TOKEN_END-----------------------------------------------------------------

// PARSER_GRAMMER_START---------------------------------------------------------------
void  Unsigned_unary_exp( Token &token ) {
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "Identifier" && token.m_type != "Constant" &&
            token.m_type != "(" )
    throw "unexpected token" ;

  if ( token.m_type == "Identifier" ) {
    if ( !Ident_Exist( token ) )
      throw "undefined identifier" ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "(" ) {
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      if ( token.m_type == "Identifier" || token.m_type == "PP" ||
           token.m_type == "MM" || token.m_type == "+" ||
           token.m_type == "-" || token.m_type == "!" ||
           token.m_type == "Constant" || token.m_type == "(" ) {
        Actual_parameter_list( token ) ;
        token = Retrieve_Token() ;
      } // if

      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type != ")" )
        throw "unexpected token" ;
      else {
        g_TokenList.push_back( token ) ;
      } // else
    } // if
    else if ( token.m_type == "[" || token.m_type == "PP" || token.m_type == "MM" ) {
      if ( token.m_type == "[" ) {
        g_TokenList.push_back( token ) ;
        token = Retrieve_Token() ;
        Expression( token ) ;
        token = Retrieve_Token() ;
        if ( token.m_type == "UNKNOWN" )
          throw "unrecognized token with first char" ;
        else if ( token.m_type != "]" )
          throw "unexpected token" ;
        else
          g_TokenList.push_back( token ) ;

        token = Retrieve_Token() ;
      } // if

      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type == "PP" || token.m_type == "MM" )
        g_TokenList.push_back( token ) ;
      else
        g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
    } // else if
    else
      g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // if
  else if ( token.m_type == "Constant" ) {
    g_TokenList.push_back( token ) ;
  } // else if
  else if ( token.m_type == "(" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ")" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else if
} // Unsigned_unary_exp()

void Signed_unary_exp( Token &token ) {
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "Identifier" && token.m_type != "Constant" &&
            token.m_type != "(" )
    throw "unexpected token" ;

  if ( token.m_type == "Identifier" ) {
    if ( !Ident_Exist( token ) )
      throw "undefined identifier" ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "(" ) {
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      if ( token.m_type == "Identifier" ) {
        Actual_parameter_list( token ) ;
        token = Retrieve_Token() ;
      } // if

      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type != ")" )
        throw "unexpected token" ;
      else
        g_TokenList.push_back( token ) ;
    } // if
    else if ( token.m_type == "[" ) {
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      Expression( token ) ;
      token = Retrieve_Token() ;
      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type != "]" )
        throw "unexpected token" ;
      else
        g_TokenList.push_back( token ) ;
    } // else if
    else
      g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // if
  else if ( token.m_type == "Constant" ) {
    g_TokenList.push_back( token ) ;
  } // else if
  else if ( token.m_type == "(" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ")" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else if
} // Signed_unary_exp()

void Unary_exp( Token &token ) {
  if ( token.m_type == "+" || token.m_type == "-" || token.m_type == "!" ) {
    Sign( token ) ;
    token = Retrieve_Token() ;
    while ( token.m_type == "+" || token.m_type == "-" || token.m_type == "!" ) {
      Sign( token ) ;
      token = Retrieve_Token() ;
    } // while

    Signed_unary_exp( token ) ;
  } // if
  else if ( token.m_type == "PP" || token.m_type == "MM" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Identifier" )
      throw "unexpected token" ;
    else {
      g_TokenList.push_back( token ) ;
    } // else

    if ( !Ident_Exist( token ) )
      throw "undefined identifier" ;

    token = Retrieve_Token() ;
    if ( token.m_type == "[" ) {
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      Expression( token ) ;
      token = Retrieve_Token() ;
      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type != "]" )
        throw "unexpected token" ;
      else {
        g_TokenList.push_back( token ) ;
      } // else
    } // if
    else
      g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // else if
  else {
    Unsigned_unary_exp( token ) ;
  } // else
} // Unary_exp()

void Rest_of_maybe_mult_exp( Token &token ) {
  while ( token.m_type == "*" || token.m_type == "/" || token.m_type == "%" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Unary_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_mult_exp()

void Maybe_mult_exp( Token &token ) {
  Unary_exp( token ) ;
  token = Retrieve_Token() ;
  Rest_of_maybe_mult_exp( token ) ;
} // Maybe_mult_exp()

void Rest_of_maybe_additive_exp( Token &token ) {
  Rest_of_maybe_mult_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "+" || token.m_type == "-" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_mult_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_additive_exp()

void Maybe_additive_exp( Token &token ) {
  Maybe_mult_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "+" || token.m_type == "-" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_mult_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_additive_exp()

void Rest_of_maybe_shift_exp( Token &token ) {
  Rest_of_maybe_additive_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "LS" || token.m_type == "RS" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_additive_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_shift_exp()

void Maybe_shift_exp( Token &token ) {
  Maybe_additive_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "LS" || token.m_type == "RS" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_additive_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_shift_exp()

void Rest_of_maybe_relational_exp( Token &token ) {
  Rest_of_maybe_shift_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "<" || token.m_type == ">" || token.m_type == "LE" ||
          token.m_type == "GE" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_shift_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_relational_exp()

void Maybe_relational_exp( Token &token ) {
  Maybe_shift_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "<" || token.m_type == ">" || token.m_type == "LE" ||
          token.m_type == "GE" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_shift_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_relational_exp()

void Rest_of_maybe_equality_exp( Token &token ) {
  Rest_of_maybe_relational_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "EQ" || token.m_type == "NEQ" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_relational_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_equality_exp()

void Maybe_equality_exp( Token &token ) {
  Maybe_relational_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "EQ" || token.m_type == "NEQ" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_relational_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_equality_exp()

void Rest_of_maybe_bit_AND_exp( Token &token ) {
  Rest_of_maybe_equality_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "&" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_equality_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_bit_AND_exp()

void Maybe_bit_AND_exp( Token &token ) {
  Maybe_equality_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "&" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_equality_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_bit_AND_exp()

void Rest_of_maybe_bit_ex_OR_exp( Token &token ) {
  Rest_of_maybe_bit_AND_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "^" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_bit_AND_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_bit_ex_OR_exp()

void Maybe_bit_ex_OR_exp( Token &token ) {
  Maybe_bit_AND_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "^" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_bit_AND_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_bit_ex_OR_exp()

void Rest_of_maybe_bit_OR_exp( Token &token ) {
  Rest_of_maybe_bit_ex_OR_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "|" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_bit_ex_OR_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_bit_OR_exp()

void Maybe_bit_OR_exp( Token &token ) {
  Maybe_bit_ex_OR_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "|" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_bit_ex_OR_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_bit_OR_exp()

void Rest_of_maybe_logical_AND_exp( Token &token ) {
  Rest_of_maybe_bit_OR_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "AND" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_bit_OR_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_logical_AND_exp()

void Maybe_logical_AND_exp( Token &token ) {
  Maybe_bit_OR_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "AND" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_bit_OR_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Maybe_logical_AND_exp()

void Rest_of_maybe_logical_OR_exp( Token &token ) {
  Rest_of_maybe_logical_AND_exp( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "OR" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Maybe_logical_AND_exp( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Rest_of_maybe_logical_OR_exp()

void Romce_and_romloe( Token &token ) {
  Rest_of_maybe_logical_OR_exp( token ) ;
  token = Retrieve_Token() ;
  if ( token.m_type == "?" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Basic_expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ":" )
      throw "unexpected token" ;
    else {
      g_TokenList.push_back( token ) ;
    } // else

    token = Retrieve_Token() ;
    Basic_expression( token ) ;
  } // if
  else
    g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Romce_and_romloe()

void Assignment_operator( Token &token ) {
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "=" && token.m_type != "TE" && token.m_type != "DE" &&
            token.m_type != "RE" && token.m_type != "PE" && token.m_type != "ME" )
    throw "unexpected token" ;
  else {
    g_TokenList.push_back( token ) ;
  } // else
} // Assignment_operator()

void Actual_parameter_list( Token &token ) {
  Basic_expression( token ) ;
  token = Retrieve_Token() ;
  if ( token.m_type == "," ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Actual_parameter_list( token ) ;
  } // if
  else {
    g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // else
} // Actual_parameter_list()

void Sign( Token &token ) {
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "+" && token.m_type != "-" && token.m_type != "!" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;
} // Sign()

void Rest_of_PPMM_identifier_started_basic_exp( Token &token ) {
  if ( token.m_type == "[" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "]" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
  } // if

  Romce_and_romloe( token ) ;
} // Rest_of_PPMM_identifier_started_basic_exp()

void Rest_of_Identifier_started_basic_exp( Token &token ) {
  if ( token.m_type == "(" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type != ")" ) {
      Actual_parameter_list( token ) ;
      token = Retrieve_Token() ;
    } // if

    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ")" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Romce_and_romloe( token ) ;
  } // if
  else {
    if ( token.m_type == "[" ) {
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      Expression( token ) ;
      token = Retrieve_Token() ;
      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type != "]" )
        throw "unexpected token" ;
      else
        g_TokenList.push_back( token ) ;

      token = Retrieve_Token() ;
    } // if

    if ( token.m_type == "=" || token.m_type == "TE" || token.m_type == "DE" ||
         token.m_type == "RE" || token.m_type == "PE" || token.m_type == "ME" ) {
      Assignment_operator( token ) ;
      token = Retrieve_Token() ;
      Basic_expression( token ) ;
    } // if
    else {
      if ( token.m_type == "PP" || token.m_type == "MM" ) {
        g_TokenList.push_back( token ) ;
        token = Retrieve_Token() ;
      } // if

      Romce_and_romloe( token ) ;
    } // else
  } // else
} // Rest_of_Identifier_started_basic_exp()

void Basic_expression( Token &token ) {
  if ( token.m_type == "Identifier" ) {
    if ( !Ident_Exist( token ) )
      throw "undefined identifier" ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Rest_of_Identifier_started_basic_exp( token ) ;
  } // if
  else if ( token.m_type == "PP" || token.m_type == "MM" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Identifier" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    if ( !Ident_Exist( token ) )
      throw "undefined identifier" ;
    token = Retrieve_Token() ;
    Rest_of_PPMM_identifier_started_basic_exp( token ) ;
  } // else if
  else if ( token.m_type == "Constant" || token.m_type == "(" ) {
    if ( token.m_type == "Constant" ) {
      g_TokenList.push_back( token ) ;
    } // if
    else {
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      Expression( token ) ;
      token = Retrieve_Token() ;
      if ( token.m_type == "UNKNOWN" )
        throw "unrecognized token with first char" ;
      else if ( token.m_type != ")" )
        throw "unexpected token" ;
      else
        g_TokenList.push_back( token ) ;
    } // else

    token = Retrieve_Token() ;
    Romce_and_romloe( token ) ;
  } // else if
  else {
    Sign( token ) ;
    token = Retrieve_Token() ;
    while ( token.m_type == "+" || token.m_type == "-" || token.m_type == "!" ) {
      Sign( token ) ;
      token = Retrieve_Token() ;
    } // while

    Signed_unary_exp( token ) ;
    token = Retrieve_Token() ;
    Romce_and_romloe( token ) ;
  } // else
} // Basic_expression()

void Expression( Token &token ) {
  Basic_expression( token ) ;
  token = Retrieve_Token() ;
  while ( token.m_type == "," ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Basic_expression( token ) ;
    token = Retrieve_Token() ;
  } // while

  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
} // Expression()

void Statement( Token &token ) {
  if ( token.m_type == ";" )
    g_TokenList.push_back( token ) ;
  else if ( token.m_type == "RETURN" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type != ";" ) {
      Expression( token ) ;
      token = Retrieve_Token() ;
    } // if

    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ";" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else if
  else if ( token.m_type == "IF" ) {
    vector<Ident>local_IdentTable ;
    g_IdentTable.push_back( local_IdentTable ) ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "(" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ")" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Statement( token ) ;
    g_IdentTable.erase( g_IdentTable.end() ) ;

    token = Retrieve_Token() ;
    if ( token.m_type == "ELSE" ) {
      g_IdentTable.push_back( local_IdentTable ) ;
      g_TokenList.push_back( token ) ;
      token = Retrieve_Token() ;
      Statement( token ) ;
      g_IdentTable.erase( g_IdentTable.end() ) ;
    } // if
    else
      g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // else if
  else if ( token.m_type == "WHILE" ) {
    vector<Ident>local_IdentTable ;
    g_IdentTable.push_back( local_IdentTable ) ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "(" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ")" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Statement( token ) ;
    g_IdentTable.erase( g_IdentTable.end() ) ;
  } // else if
  else if ( token.m_type == "DO" ) {
    vector<Ident>local_IdentTable ;
    g_IdentTable.push_back( local_IdentTable ) ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Statement( token ) ;
    g_IdentTable.erase( g_IdentTable.end() ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "WHILE" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "(" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ")" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ";" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else if
  else if ( token.m_type == "{" ) {
    Compound_statement( token ) ;
  } // else if
  else {
    Expression( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ";" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else
} // Statement()

void Declaration( Token &token ) {
  Type_specifier( token ) ;
  token = Retrieve_Token() ;
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "Identifier" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;

  Ident ident ;
  ident.m_name = token.m_value ;
  g_IdentTable[g_IdentTable.size()-1].push_back( ident ) ;

  token = Retrieve_Token() ;
  Rest_of_declarators( token, true ) ;
} // Declaration()

void Compound_statement( Token &token ) {
  bool continue_compound = true ;
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "{" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;

  token = Retrieve_Token() ;
  if ( token.m_type == "}" )
    continue_compound = false ;
  g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;

  while ( continue_compound ) {
    token = Retrieve_Token() ;
    if ( token.m_type == "INT" || token.m_type == "CHAR" || token.m_type == "FLOAT" ||
         token.m_type == "STRING" || token.m_type == "BOOL" )
      Declaration( token ) ;
    else
      Statement( token ) ;

    token = Retrieve_Token() ;
    if ( token.m_type == "}" )
      continue_compound = false ;
    g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // while

  token = Retrieve_Token() ;
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "}" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;
} // Compound_statement()

void Formal_parameter_list( Token &token ) {
  Type_specifier( token ) ;
  token = Retrieve_Token() ;
  if ( token.m_type == "&" ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
  } // if

  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "Identifier" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;

  Ident ident ;
  ident.m_name = token.m_value ;
  g_IdentTable[g_IdentTable.size()-1].push_back( ident ) ;

  token = Retrieve_Token() ;
  if ( token.m_type != "[" )
    g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  else {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Constant" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "]" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else

  token = Retrieve_Token() ;
  if ( token.m_type == "," ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    Formal_parameter_list( token ) ;
  } // if
  else {
    g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // else
} // Formal_parameter_list()

void Function_definition_without_ID( Token &token ) {
  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != "(" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;

  token = Retrieve_Token() ;
  if ( token.m_type != ")" ) {
    if ( token.m_type == "VOID" )
      g_TokenList.push_back( token ) ;
    else
      Formal_parameter_list( token ) ;
    token = Retrieve_Token() ;
  } // if

  if ( token.m_type == "UNKNOWN" )
    throw "unrecognized token with first char" ;
  else if ( token.m_type != ")" )
    throw "unexpected token" ;
  else
    g_TokenList.push_back( token ) ;

  token = Retrieve_Token() ;
  Compound_statement( token ) ;
} // Function_definition_without_ID()

void Rest_of_declarators( Token &token, bool local ) {
  if ( token.m_type != "[" ) {
    g_TokenBuffer.insert( g_TokenBuffer.begin(), token ) ;
  } // if
  else {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Constant" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "]" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else

  token = Retrieve_Token() ;
  if ( token.m_type == "," ) {
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Identifier" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    if ( local ) {
      Ident ident ;
      ident.m_name = token.m_value ;
      g_IdentTable[g_IdentTable.size()-1].push_back( ident ) ;
    } // if

    token = Retrieve_Token() ;
    Rest_of_declarators( token, local ) ;
  } // if
  else {
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != ";" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;
  } // else
} // Rest_of_declarators()

void Function_definition_or_declarators( Token &token ) {
  if ( token.m_type == "(" ) {
    g_variable_or_function = 2 ;
    Function_definition_without_ID( token ) ;
  } // if
  else {
    g_variable_or_function = 1 ;
    Rest_of_declarators( token, false ) ;
  } // else
} // Function_definition_or_declarators()

void Type_specifier( Token &token ) {
  if ( token.m_type != "INT" && token.m_type != "CHAR" && token.m_type != "FLOAT" &&
       token.m_type != "STRING" && token.m_type != "BOOL" ) {
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else
      throw "unexpected token" ;
  } // if

  g_TokenList.push_back( token ) ;
} // Type_specifier()

void Definition( Token &token ) {
  if ( token.m_type == "VOID" ) {
    g_variable_or_function = 2 ;
    g_TokenList.push_back( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Identifier" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Function_definition_without_ID( token ) ;
  } // if
  else {
    Type_specifier( token ) ;
    token = Retrieve_Token() ;
    if ( token.m_type == "UNKNOWN" )
      throw "unrecognized token with first char" ;
    else if ( token.m_type != "Identifier" )
      throw "unexpected token" ;
    else
      g_TokenList.push_back( token ) ;

    token = Retrieve_Token() ;
    Function_definition_or_declarators( token ) ;
  } // else
} // Definition()

void User_input( Token &token ) {
  if ( token.m_type == "VOID" || token.m_type == "INT" || token.m_type == "CHAR" ||
       token.m_type == "FLOAT" || token.m_type == "STRING" || token.m_type == "BOOL" ) {
    g_definition_or_statement = 1 ;
    Definition( token ) ;
  } // if
  else {
    g_definition_or_statement = 2 ;
    Statement( token ) ;
  } // else

} // User_input()

void Parser( Token &token ) {
  try {
    User_input( token ) ;
  }  catch ( const char * msg ) {
    char peek ;
    peek = cin.peek() ;
    while ( peek != '\n' && peek != EOF ) {
      peek = getchar() ;
      peek = cin.peek() ;
    } // while

    g_row = 0 ;
    g_TokenList.clear() ;
    throw msg ;
  } // catch
} // Parser()

// PARSER_GRAMMER_END-----------------------------------------------------------------

// EXECUTE_START----------------------------------------------------------------------
void Execute_Unsigned_unary_exp( Content &content ) {
  Token token = g_TokenList[0] ;
  if ( token.m_type == "Identifier" ) {
    int ident_array_size = -1 ;
    int index = -1 ;
    Token ident = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    bool ident_not_found = true ;
    for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
      for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
        if ( g_IdentTable[i][j].m_name == ident.m_value ) {
          ident_array_size = atoi( g_IdentTable[i][j].m_array.c_str() ) ;
          ident_not_found = false ;
        } // if
      } // for
    } // for

    token = g_TokenList[0] ;
    if ( token.m_type == "(" )
      cout << "Function Execution" << endl ;
    else if ( token.m_type == "[" ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content array_index_content = Execute_Expression() ;
      index = array_index_content.m_int ;
      if ( ident_array_size <= index || index < 0 )
        throw "Out of range." ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      token = g_TokenList[0] ;
    } // else if

    if ( token.m_type == "PP" || token.m_type == "MM" ) {
      Token ppmm = token ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      bool ident_not_found = true ;
      for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
        for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
          if ( g_IdentTable[i][j].m_name == ident.m_value ) {
            if ( ident_array_size == -1 ) {
              content = g_IdentTable[i][j].m_content ;
              Content blank ;
              g_IdentTable[i][j].m_content.Calculation( ppmm, blank ) ;
            } // if
            else {
              content = g_IdentTable[i][j].m_content_array[index] ;
              Content blank ;
              g_IdentTable[i][j].m_content_array[index].Calculation( ppmm, blank ) ;
            } // else

            ident_not_found = false ;
          } // if
        } // for
      }  // for
    } // if
    else {
      bool ident_not_found = true ;
      for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
        for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
          if ( g_IdentTable[i][j].m_name == ident.m_value ) {
            if ( ident_array_size == -1 )
              content = g_IdentTable[i][j].m_content ;
            else
              content = g_IdentTable[i][j].m_content_array[index] ;
            ident_not_found = false ;
          } // if
        } // for
      }  // for
    } // else
  } // if
  else if ( token.m_type == "Constant" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    if ( token.m_const_type == "int" ) {
      content.m_type = "int" ;
      content.m_int = atoi( token.m_value.c_str() ) ;
    } // if
    else if ( token.m_const_type == "char" ) {
      content.m_type = "char" ;
      if ( token.m_value[1] == '\\' ) {
        if ( token.m_value[2] == 'n' )
          content.m_char = '\n' ;
        else if ( token.m_value[2] == 't' )
          content.m_char = '\t' ;
        else if ( token.m_value[2] == '\\' )
          content.m_char = '\\' ;
        else if ( token.m_value[2] == '0' )
          content.m_char = '\0' ;
      } // if
      else
        content.m_char = token.m_value[1] ;
    } // else if
    else if ( token.m_const_type == "float" ) {
      content.m_type = "float" ;
      content.m_float = atof( token.m_value.c_str() ) ;
    } // else if
    else if ( token.m_const_type == "bool" ) {
      content.m_type = "bool" ;
      if ( token.m_value == "true" )
        content.m_bool = true ;
      else
        content.m_bool = false ;
    } // else if
    else if ( token.m_const_type == "string" ) {
      content.m_type = "string" ;
      content.m_string = token.m_value ;
      if ( content.m_string.size() >= 2 ) {
        content.m_string.erase( content.m_string.begin() ) ;
        content.m_string.erase( content.m_string.end() - 1 ) ;
      } // if
    } // else if
  } // else if
  else if ( token.m_type == "(" ) {
    g_para++ ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    content = Execute_Expression() ;
    g_para-- ;
    g_TokenList.erase( g_TokenList.begin() ) ;
  } // else if
} // Execute_Unsigned_unary_exp()

void Execute_Signed_unary_exp( Content &content ) {
  Token token = g_TokenList[0] ;
  if ( token.m_type == "Identifier" ) {
    int ident_array_size = -1 ;
    int index = -1 ;
    Token ident = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    bool ident_not_found = true ;
    for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
      for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
        if ( g_IdentTable[i][j].m_name == ident.m_value ) {
          ident_array_size = atoi( g_IdentTable[i][j].m_array.c_str() ) ;
          ident_not_found = false ;
        } // if
      } // for
    } // for

    token = g_TokenList[0] ;
    if ( token.m_type == "(" || token.m_type == "[" ) {
      if ( token.m_type == "(" )
        cout << "Function Execution" << endl ;
      else {
        g_TokenList.erase( g_TokenList.begin() ) ;
        Content array_index_content = Execute_Expression() ;
        index = array_index_content.m_int ;
        if ( ident_array_size <= index || index < 0 )
          throw "Out of range." ;
        g_TokenList.erase( g_TokenList.begin() ) ;
        token = g_TokenList[0] ;
      } // else
    } // if

    ident_not_found = true ;
    for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
      for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
        if ( g_IdentTable[i][j].m_name == ident.m_value ) {
          Content blank ;
          if ( ident_array_size == -1 )
            content = g_IdentTable[i][j].m_content ;
          else
            content = g_IdentTable[i][j].m_content_array[index] ;
          ident_not_found = false ;
        } // if
      } // for
    } // for
  } // if
  else if ( token.m_type == "Constant" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    if ( token.m_const_type == "int" ) {
      content.m_type = "int" ;
      content.m_int = atoi( token.m_value.c_str() ) ;
    } // if
    else if ( token.m_const_type == "char" ) {
      content.m_type = "char" ;
      if ( token.m_value[1] == '\\' ) {
        if ( token.m_value[2] == 'n' )
          content.m_char = '\n' ;
        else if ( token.m_value[2] == 't' )
          content.m_char = '\t' ;
        else if ( token.m_value[2] == '\\' )
          content.m_char = '\\' ;
        else if ( token.m_value[2] == '0' )
          content.m_char = '\0' ;
      } // if
      else
        content.m_char = token.m_value[1] ;
    } // else if
    else if ( token.m_const_type == "float" ) {
      content.m_type = "float" ;
      content.m_float = atof( token.m_value.c_str() ) ;
    } // else if
    else if ( token.m_const_type == "bool" ) {
      content.m_type = "bool" ;
      if ( token.m_value == "true" )
        content.m_bool = true ;
      else
        content.m_bool = false ;
    } // else if
    else if ( token.m_const_type == "string" ) {
      content.m_type = "string" ;
      content.m_string = token.m_value ;
      if ( content.m_string.size() >= 2 ) {
        content.m_string.erase( content.m_string.begin() ) ;
        content.m_string.erase( content.m_string.end() - 1 ) ;
      } // if
    } // else if
  } // else if
  else if ( token.m_type == "(" ) {
    g_para++ ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    content = Execute_Expression() ;
    g_para-- ;
    g_TokenList.erase( g_TokenList.begin() ) ;
  } // else if
} // Execute_Signed_unary_exp()

void Execute_Unary_exp( Content &content ) {
  Token token = g_TokenList[0] ;
  if ( token.m_type == "+" || token.m_type == "-" || token.m_type == "!" ) {
    int negatives = 0 ;
    int nots = 0 ;
    while ( token.m_type == "+" || token.m_type == "-" || token.m_type == "!" ) {
      if ( token.m_type == "-" )
        negatives++ ;
      else if ( token.m_type == "!" )
        nots++ ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      token = g_TokenList[0] ;
    } // while

    Execute_Signed_unary_exp( content ) ;
    if ( negatives % 2 != 0 ) {
      Token operate ;
      operate.m_type = "*" ;
      Content content1 ;
      content1.m_type = "int" ;
      content1.m_int = -1 ;
      content.Calculation( operate, content1 ) ;
    } // if

    if ( nots % 2 != 0 ) {
      Token operate ;
      operate.m_type = "!" ;
      Content blank ;
      content.Calculation( operate, blank ) ;
    } // if
  } // if
  else if ( token.m_type == "PP" || token.m_type == "MM" ) {
    int ident_array_size = -1 ;
    int index = -1 ;
    Token ppmm = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    Token ident = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    bool ident_not_found = true ;
    for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
      for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
        if ( g_IdentTable[i][j].m_name == ident.m_value ) {
          ident_array_size = atoi( g_IdentTable[i][j].m_array.c_str() ) ;
          ident_not_found = false ;
        } // if
      } // for
    } // for

    token = g_TokenList[0] ;
    if ( token.m_type == "[" ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content array_index_content = Execute_Expression() ;
      index = array_index_content.m_int ;
      if ( ident_array_size <= index || index < 0 )
        throw "Out of range." ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      token = g_TokenList[0] ;
    } // if

    ident_not_found = true ;
    for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
      for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
        if ( g_IdentTable[i][j].m_name == ident.m_value ) {
          Content blank ;
          if ( ident_array_size == -1 ) {
            g_IdentTable[i][j].m_content.Calculation( ppmm, blank ) ;
            content = g_IdentTable[i][j].m_content ;
          } // if
          else {
            g_IdentTable[i][j].m_content_array[index].Calculation( ppmm, blank ) ;
            content = g_IdentTable[i][j].m_content_array[index] ;
          } // else

          ident_not_found = false ;
        } // if
      } // for
    } // for
  } // else if
  else {
    Execute_Unsigned_unary_exp( content ) ;
  } // else

} // Execute_Unary_exp()

void Execute_Rest_of_maybe_mult_exp( Content &content ) {
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "*" || token.m_type == "/" || token.m_type == "%" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Unary_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_mult_exp()

void Execute_Maybe_mult_exp( Content &content ) {
  Execute_Unary_exp( content ) ;
  Execute_Rest_of_maybe_mult_exp( content ) ;
} // Execute_Maybe_mult_exp()

void Execute_Rest_of_maybe_additive_exp( Content &content ) {
  Execute_Rest_of_maybe_mult_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "+" || token.m_type == "-" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_mult_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_additive_exp()

void Execute_Maybe_additive_exp( Content &content ) {
  Execute_Maybe_mult_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "+" || token.m_type == "-" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_mult_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_additive_exp()

void Execute_Rest_of_maybe_shift_exp( Content &content ) {
  Execute_Rest_of_maybe_additive_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "LS" || token.m_type == "RS" ) ) {
    if ( g_cout && !g_para )
      return ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_additive_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_shift_exp()

void Execute_Maybe_shift_exp( Content &content ) {
  Execute_Maybe_additive_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "LS" || token.m_type == "RS" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_additive_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_shift_exp()

void Execute_Rest_of_maybe_relational_exp( Content &content ) {
  Execute_Rest_of_maybe_shift_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "<" || token.m_type == ">" ||
                                      token.m_type == "LE" || token.m_type == "GE" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_shift_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_relational_exp()

void Execute_Maybe_relational_exp( Content &content ) {
  Execute_Maybe_shift_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "<" || token.m_type == ">" ||
                                      token.m_type == "LE" || token.m_type == "GE" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_shift_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_relational_exp()

void Execute_Rest_of_maybe_equality_exp( Content &content ) {
  Execute_Rest_of_maybe_relational_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "EQ" || token.m_type == "NEQ" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_relational_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_equality_exp()

void Execute_Maybe_equality_exp( Content &content ) {
  Execute_Maybe_relational_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && ( token.m_type == "EQ" || token.m_type == "NEQ" ) ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_relational_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_equality_exp()

void Execute_Rest_of_maybe_bit_AND_exp( Content &content ) {
  Execute_Rest_of_maybe_equality_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "&" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_equality_exp( content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_bit_AND_exp()

void Execute_Maybe_bit_AND_exp( Content &content ) {
  Execute_Maybe_equality_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "&" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_equality_exp( content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_bit_AND_exp()

void Execute_Rest_of_maybe_bit_ex_OR_exp( Content &content ) {
  Execute_Rest_of_maybe_bit_AND_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "^" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_bit_AND_exp( content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_bit_ex_OR_exp()

void Execute_Maybe_bit_ex_OR_exp( Content &content ) {
  Execute_Maybe_bit_AND_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "^" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_bit_AND_exp( content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_bit_ex_OR_exp()

void Execute_Rest_of_maybe_bit_OR_exp( Content &content ) {
  Execute_Rest_of_maybe_bit_ex_OR_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "|" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_bit_ex_OR_exp( content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_bit_OR_exp()

void Execute_Maybe_bit_OR_exp( Content &content ) {
  Execute_Maybe_bit_ex_OR_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "|" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_bit_ex_OR_exp( content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_bit_OR_exp()

void Execute_Rest_of_maybe_logical_AND_exp( Content &content ) {
  Execute_Rest_of_maybe_bit_OR_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "AND" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_bit_OR_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_logical_AND_exp()

void Execute_Maybe_logical_AND_exp( Content &content ) {
  Execute_Maybe_bit_OR_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "AND" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_bit_OR_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Maybe_logical_AND_exp()

void Execute_Rest_of_maybe_logical_OR_exp( Content &content ) {
  Execute_Rest_of_maybe_logical_AND_exp( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "OR" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content content1 ;
    Execute_Maybe_logical_AND_exp( content1 ) ;
    content.Calculation( token, content1 ) ;
    token = g_TokenList[0] ;
  } // while
} // Execute_Rest_of_maybe_logical_OR_exp()

void Execute_Romce_and_romloe( Content &content ) {
  Execute_Rest_of_maybe_logical_OR_exp( content ) ;
  if ( g_TokenList.size() > 0 && g_TokenList[0].m_value == "?" ) {
    if ( content.m_int >= 1 || content.m_float >= 1 || content.m_bool == true ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content content1 ;
      Execute_Basic_expression( content1 ) ;
      content = content1 ;
      vector<vector<Ident> >bk_Ident_table = g_IdentTable ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content content2 ;
      Execute_Basic_expression( content2 ) ;
      g_IdentTable = bk_Ident_table ;
    } // if
    else {
      vector<vector<Ident> >bk_Ident_table = g_IdentTable ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content content1 ;
      Execute_Basic_expression( content1 ) ;
      g_IdentTable = bk_Ident_table ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content content2 ;
      Execute_Basic_expression( content2 ) ;
      content = content2 ;
    } // else
  } // if
} // Execute_Romce_and_romloe()

void Execute_Rest_of_PPMM_identifier_started_basic_exp( Token ppmm, Token ident, Content &content ) {
  Token token = g_TokenList[0] ;
  int ident_array_size = -1 ;
  int index = -1 ;
  bool ident_not_found = true ;
  for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
    for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
      if ( g_IdentTable[i][j].m_name == ident.m_value ) {
        ident_array_size = atoi( g_IdentTable[i][j].m_array.c_str() ) ;
        ident_not_found = false ;
      } // if
    } // for
  } // for

  if ( token.m_type == "[" ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Content array_index_content = Execute_Expression() ;
    index = array_index_content.m_int ;
    if ( ident_array_size <= index || index < 0 )
      throw "Out of range." ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    token = g_TokenList[0] ;
  } // if

  ident_not_found = true ;
  for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
    for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
      if ( g_IdentTable[i][j].m_name == ident.m_value ) {
        Content blank ;
        if ( ident_array_size == -1 ) {
          g_IdentTable[i][j].m_content.Calculation( ppmm, blank ) ;
          content = g_IdentTable[i][j].m_content ;
        } // if
        else {
          g_IdentTable[i][j].m_content_array[index].Calculation( ppmm, blank ) ;
          content = g_IdentTable[i][j].m_content_array[index] ;
        } // else

        ident_not_found = false ;
      } // if
    } // for
  } // for

  Execute_Romce_and_romloe( content ) ;
} // Execute_Rest_of_PPMM_identifier_started_basic_exp()

void Execute_Rest_of_Identifier_started_basic_exp( Token ident, Content &content ) {
  Token token = g_TokenList[0] ;
  if ( token.m_type == "(" )
    cout << "Function Execution" << endl ;
  else {
    int ident_array_size = -1 ;
    int index = -1 ;
    bool ident_not_found = true ;
    for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
      for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
        if ( g_IdentTable[i][j].m_name == ident.m_value ) {
          ident_array_size = atoi( g_IdentTable[i][j].m_array.c_str() ) ;
          ident_not_found = false ;
        } // if
      } // for
    } // for

    if ( token.m_type == "[" ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content array_index_content = Execute_Expression() ;
      index = array_index_content.m_int ;
      if ( ident_array_size <= index || index < 0 )
        throw "Out of range." ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      token = g_TokenList[0] ;
    } // if

    if ( token.m_type == "=" || token.m_type == "TE" ||
         token.m_type == "DE" || token.m_type == "RE" ||
         token.m_type == "PE" || token.m_type == "ME" ) {
      Token assignment_operator = token ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Execute_Basic_expression( content ) ;
      bool ident_not_found = true ;
      for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
        for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
          if ( g_IdentTable[i][j].m_name == ident.m_value ) {
            if ( index == -1 && ident_array_size == -1 ) {
              g_IdentTable[i][j].m_content.Calculation( assignment_operator, content ) ;
              content = g_IdentTable[i][j].m_content ;
            } // if
            else if ( index > -1 && ident_array_size > -1 ) {
              g_IdentTable[i][j].m_content_array[index].Calculation( assignment_operator, content ) ;
              content = g_IdentTable[i][j].m_content_array[index] ;
            } // else if
            else if ( index == -1 && ident_array_size > -1 )
              throw "Cant assign value to array type." ;
            ident_not_found = false ;
          } // if
        } // for
      } // for
    } // if
    else {
      Token ppmm ;
      bool have_ppmm = false ;
      if ( token.m_type == "PP" || token.m_type == "MM" ) {
        have_ppmm = true ;
        ppmm = g_TokenList[0] ;
        g_TokenList.erase( g_TokenList.begin() ) ;
      } // if

      bool ident_not_found = true ;
      for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
        for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
          if ( g_IdentTable[i][j].m_name == ident.m_value ) {
            if ( ident_array_size == -1 )
              content = g_IdentTable[i][j].m_content ;
            else
              content = g_IdentTable[i][j].m_content_array[index] ;
            ident_not_found = false ;
          } // if
        } // for
      } // for

      Execute_Romce_and_romloe( content ) ;
      if ( have_ppmm ) {
        bool ident_not_found = true ;
        for ( int i = g_IdentTable.size() - 1 ; i >= 0 && ident_not_found ; i-- ) {
          for ( int j = 0 ; j < g_IdentTable[i].size() && ident_not_found ; j++ ) {
            if ( g_IdentTable[i][j].m_name == ident.m_value ) {
              Content blank ;
              if ( ident_array_size == -1 )
                g_IdentTable[i][j].m_content.Calculation( ppmm, blank ) ;
              else
                g_IdentTable[i][j].m_content_array[index].Calculation( ppmm, blank ) ;
              ident_not_found = false ;
            } // if
          } // for
        } // for
      } // if
    } // else
  } // else
} // Execute_Rest_of_Identifier_started_basic_exp()

void Execute_Basic_expression( Content &content ) {
  Token token = g_TokenList[0] ;
  if ( token.m_type == "Identifier" ) {
    Token ident = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    Execute_Rest_of_Identifier_started_basic_exp( ident, content ) ;
  } // if
  else if ( token.m_type == "PP" || token.m_type == "MM" ) {
    Token ppmm = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    token = g_TokenList[0] ;
    Token ident = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    Execute_Rest_of_PPMM_identifier_started_basic_exp( ppmm, ident, content ) ;
  } // else if
  else if ( token.m_type == "Constant" || token.m_type == "(" ) {
    if ( token.m_type == "Constant" ) {
      if ( token.m_const_type == "int" ) {
        content.m_type = "int" ;
        content.m_int = atoi( token.m_value.c_str() ) ;
      } // if
      else if ( token.m_const_type == "char" ) {
        content.m_type = "char" ;
        if ( token.m_value[1] == '\\' ) {
          if ( token.m_value[2] == 'n' )
            content.m_char = '\n' ;
          else if ( token.m_value[2] == 't' )
            content.m_char = '\t' ;
          else if ( token.m_value[2] == '\\' )
            content.m_char = '\\' ;
          else if ( token.m_value[2] == '0' )
            content.m_char = '\0' ;
        } // if
        else
          content.m_char = token.m_value[1] ;
      } // else if
      else if ( token.m_const_type == "float" ) {
        content.m_type = "float" ;
        content.m_float = atof( token.m_value.c_str() ) ;
      } // else if
      else if ( token.m_const_type == "bool" ) {
        content.m_type = "bool" ;
        if ( token.m_value == "true" )
          content.m_bool = true ;
        else
          content.m_bool = false ;
      } // else if
      else if ( token.m_const_type == "string" ) {
        content.m_type = "string" ;
        content.m_string = token.m_value ;
        if ( content.m_string.size() >= 2 ) {
          content.m_string.erase( content.m_string.begin() ) ;
          content.m_string.erase( content.m_string.end() - 1 ) ;
        } // if
      } // else if

      g_TokenList.erase( g_TokenList.begin() ) ;
    } // if
    else {
      g_para++ ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      content = Execute_Expression() ;
      g_para-- ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Execute_Romce_and_romloe( content ) ;
    } // else

    Execute_Romce_and_romloe( content ) ;
  } // else if
  else {
    int negatives = 0 ;
    int nots = 0 ;
    while ( token.m_type == "+" || token.m_type == "-" || token.m_type == "!" ) {
      if ( token.m_type == "-" )
        negatives++ ;
      else if ( token.m_type == "!" )
        nots++ ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      token = g_TokenList[0] ;
    } // while

    Execute_Signed_unary_exp( content ) ;
    if ( negatives % 2 != 0 ) {
      Token operate ;
      operate.m_type = "*" ;
      Content content1 ;
      content1.m_type = "int" ;
      content1.m_int = -1 ;
      content.Calculation( operate, content1 ) ;
    } // if

    if ( nots % 2 != 0 ) {
      Token operate ;
      operate.m_type = "!" ;
      Content blank ;
      content.Calculation( operate, blank ) ;
    } // if

    Execute_Romce_and_romloe( content ) ;
  } // else
} // Execute_Basic_expression()

Content Execute_Expression() {
  Content content ;
  Execute_Basic_expression( content ) ;
  Token token = g_TokenList[0] ;
  while ( g_TokenList.size() > 0 && token.m_type == "," ) {
    g_TokenList.erase( g_TokenList.begin() ) ;
    Execute_Basic_expression( content ) ;
    token = g_TokenList[0] ;
  } // while

  return content ;
} // Execute_Expression()

void Execute_Statement() {
  if ( g_TokenList.size() >= 4 && g_TokenList[0].m_value == "Done" &&
       g_TokenList[1].m_value == "(" && g_TokenList[2].m_value == ")" &&
       g_TokenList[3].m_value == ";" ) {
    cout << "Our-C exited ..." ;
    exit( 0 ) ;
  } // if
  else if ( g_TokenList.size() >= 4 && g_TokenList[0].m_value == "ListAllVariables" &&
            g_TokenList[1].m_value == "(" && g_TokenList[2].m_value == ")" &&
            g_TokenList[3].m_value == ";" ) {
    Sort() ;
    for ( int i = 0 ; i < g_IdentTable[0].size() ; i++ ) {
      if ( g_IdentTable[0][i].m_var_or_funct == "var" )
        cout << g_IdentTable[0][i].m_name << endl ;
    } // for
  } // else if
  else if ( g_TokenList.size() >= 4 && g_TokenList[0].m_value == "ListAllFunctions" &&
            g_TokenList[1].m_value == "(" && g_TokenList[2].m_value == ")" &&
            g_TokenList[3].m_value == ";" ) {
    Sort() ;
    for ( int i = 0 ; i < g_IdentTable[0].size() ; i++ ) {
      if ( g_IdentTable[0][i].m_var_or_funct == "funct" )
        cout << g_IdentTable[0][i].m_name << "()" << endl ;
    } // for
  } // else if
  else if ( g_TokenList.size() >= 5 && g_TokenList[0].m_value == "ListVariable" &&
            g_TokenList[1].m_value == "(" && g_TokenList[2].m_type == "Constant" &&
            g_TokenList[3].m_value == ")" && g_TokenList[4].m_value == ";" ) {
    string target = g_TokenList[2].m_value ;
    if ( target.size() >= 2 ) {
      target.erase( target.begin() ) ;
      target.erase( target.end() - 1 ) ;
    } // if

    for ( int i = 0 ; i < g_IdentTable[0].size() ; i++ ) {
      if ( target == g_IdentTable[0][i].m_name && g_IdentTable[0][i].m_var_or_funct == "var" ) {
        cout << g_IdentTable[0][i].m_type << " " << g_IdentTable[0][i].m_name ;
        if ( g_IdentTable[0][i].m_array != "0" )
          cout << "[ " << g_IdentTable[0][i].m_array << " ]" ;
        cout << " ;" << endl ;
      } // if
    } // for
  } // else if
  else if ( g_TokenList.size() >= 5 && g_TokenList[0].m_value == "ListFunction" &&
            g_TokenList[1].m_value == "(" && g_TokenList[2].m_type == "Constant" &&
            g_TokenList[3].m_value == ")" && g_TokenList[4].m_value == ";" ) {
    string target = g_TokenList[2].m_value ;
    if ( target.size() >= 2 ) {
      target.erase( target.begin() ) ;
      target.erase( target.end() - 1 ) ;
    } // if

    for ( int i = 0 ; i < g_IdentTable[0].size() ; i++ ) {
      if ( target == g_IdentTable[0][i].m_name && g_IdentTable[0][i].m_var_or_funct == "funct" )
        g_IdentTable[0][i].Print() ;
    } // for
  } // else if
  else if ( g_TokenList[0].m_value == "cout" ) {
    Token token ;
    g_cout = 1 ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    token = g_TokenList[0] ;
    while ( token.m_type == "LS" ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      Execute_Expression().Print() ;
      token = g_TokenList[0] ;
    } // while

    g_TokenList.erase( g_TokenList.begin() ) ;
  } // else if
  else {
    Token token = g_TokenList[0] ;
    if ( token.m_type == ";" )  ;
    else if ( token.m_type == "RETURN" )
      cout << "Execute Return." << endl ;
    else if ( token.m_type == "IF" ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content condition = Execute_Expression() ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      if ( condition.m_int > 0 || condition.m_float > 0 || condition.m_bool > 0 ) {
        vector<Ident>local_IdentTable ;
        g_IdentTable.push_back( local_IdentTable ) ;
        Execute_Statement() ;
        g_IdentTable.erase( g_IdentTable.end() ) ;
        if ( !g_TokenList.empty() ) {
          token = g_TokenList[0] ;
          if ( token.m_type == "ELSE" ) {
            g_TokenList.erase( g_TokenList.begin() ) ;
            token = g_TokenList[0] ;
            if ( token.m_type == "{" ) {
              int para = 1 ;
              g_TokenList.erase( g_TokenList.begin() ) ;
              while ( para > 0 ) {
                token = g_TokenList[0] ;
                if ( token.m_type == "{" ) para++ ;
                else if ( token.m_type == "}" ) para-- ;
                g_TokenList.erase( g_TokenList.begin() ) ;
              } // while
            } // if
            else if ( token.m_type == "IF" )
              Remove_if_statement() ;
            else {
              while ( token.m_type != ";" ) {
                g_TokenList.erase( g_TokenList.begin() ) ;
                token = g_TokenList[0] ;
              } // while

              g_TokenList.erase( g_TokenList.begin() ) ;
            } // else
          } // if
        } // if
      } // if
      else {
        token = g_TokenList[0] ;
        if ( token.m_type == "{" ) {
          int para = 1 ;
          g_TokenList.erase( g_TokenList.begin() ) ;
          while ( para > 0 ) {
            token = g_TokenList[0] ;
            if ( token.m_type == "{" ) para++ ;
            else if ( token.m_type == "}" ) para-- ;
            g_TokenList.erase( g_TokenList.begin() ) ;
          } // while
        } // if
        else if ( token.m_type == "IF" )
          Remove_if_statement() ;
        else {
          while ( token.m_type != ";" ) {
            g_TokenList.erase( g_TokenList.begin() ) ;
            token = g_TokenList[0] ;
          } // while

          g_TokenList.erase( g_TokenList.begin() ) ;
        } // else

        token = g_TokenList[0] ;
        if ( token.m_type == "ELSE" ) {
          g_TokenList.erase( g_TokenList.begin() ) ;
          vector<Ident>local_IdentTable ;
          g_IdentTable.push_back( local_IdentTable ) ;
          Execute_Statement() ;
          g_IdentTable.erase( g_IdentTable.end() ) ;
        } // if
      } // else
    } // else if
    else if ( token.m_type == "WHILE" ) {
      g_TokenList.erase( g_TokenList.begin() ) ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      vector<Token>bk_TokenList = g_TokenList ;
      Content condition = Execute_Expression() ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      while ( condition.m_int > 0 || condition.m_float > 0 || condition.m_bool > 0 ) {
        vector<Ident>local_IdentTable ;
        g_IdentTable.push_back( local_IdentTable ) ;
        Execute_Statement() ;
        g_IdentTable.erase( g_IdentTable.end() ) ;
        g_TokenList = bk_TokenList ;
        condition = Execute_Expression() ;
        g_TokenList.erase( g_TokenList.begin() ) ;
      } // while

      token = g_TokenList[0] ;
      if ( token.m_type == "{" ) {
        int para = 1 ;
        g_TokenList.erase( g_TokenList.begin() ) ;
        while ( para > 0 ) {
          token = g_TokenList[0] ;
          if ( token.m_type == "{" ) para++ ;
          else if ( token.m_type == "}" ) para-- ;
          g_TokenList.erase( g_TokenList.begin() ) ;
        } // while
      } // if
      else if ( token.m_type == "IF" )
        Remove_if_statement() ;
      else {
        while ( token.m_type != ";" ) {
          g_TokenList.erase( g_TokenList.begin() ) ;
          token = g_TokenList[0] ;
        } // while

        g_TokenList.erase( g_TokenList.begin() ) ;
      } // else
    } // else if
    else if ( token.m_type == "DO" ) {
      vector<Token>bk_TokenList = g_TokenList ;
      vector<Ident>local_IdentTable ;
      g_IdentTable.push_back( local_IdentTable ) ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Execute_Statement() ;
      g_IdentTable.erase( g_IdentTable.end() ) ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      Content condition = Execute_Expression() ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      while ( condition.m_int > 0 || condition.m_float > 0 || condition.m_bool > 0 ) {
        g_TokenList = bk_TokenList ;
        vector<Ident>local_IdentTable ;
        g_IdentTable.push_back( local_IdentTable ) ;
        g_TokenList.erase( g_TokenList.begin() ) ;
        Execute_Statement() ;
        g_IdentTable.erase( g_IdentTable.end() ) ;
        g_TokenList.erase( g_TokenList.begin() ) ;
        g_TokenList.erase( g_TokenList.begin() ) ;
        condition = Execute_Expression() ;
        g_TokenList.erase( g_TokenList.begin() ) ;
      } // while

      g_TokenList.erase( g_TokenList.begin() ) ;
    } // else if
    else if ( token.m_type == "{" )
      Execute_Compound_statement() ;
    else {
      Execute_Expression() ;
      g_TokenList.erase( g_TokenList.begin() ) ;
    } // else
  } // else
} // Execute_Statement()

void Execute_Declaration() {
  bool is_array_type = false ;
  Token token ;
  Token type, name, array_size ;
  type = g_TokenList[0] ;
  g_TokenList.erase( g_TokenList.begin() ) ;
  do {
    name = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    token = g_TokenList[0] ;
    if ( token.m_type == "[" ) {
      is_array_type = true ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      array_size = g_TokenList[0] ;
      if ( array_size.m_const_type != "int" ) throw "Array size is non int." ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      g_TokenList.erase( g_TokenList.begin() ) ;
      token = g_TokenList[0] ;
    } // if

    Ident ident ;
    if ( is_array_type ) {
      ident.Redefine_Var( type.m_value, name.m_value, array_size.m_value, "var" ) ;
      for ( int i = 0 ; i < atoi( array_size.m_value.c_str() ) ; i++ ) {
        Content content ;
        content.m_type = type.m_value ;
        ident.m_content_array.push_back( content ) ;
      } // for
    } // if
    else
      ident.Redefine_Var( type.m_value, name.m_value, "-1", "var" ) ;

    bool exist = false ;
    for ( int i = 0 ; i < g_IdentTable[g_IdentTable.size()-1].size() ; i++ ) {
      if ( g_IdentTable[g_IdentTable.size()-1][i].m_name == name.m_value ) {
        exist = true ;
        g_IdentTable[g_IdentTable.size()-1][i] = ident ;
      } // if
    } // for

    if ( !exist )
      g_IdentTable[g_IdentTable.size()-1].push_back( ident ) ;

    token = g_TokenList[0] ;
    g_TokenList.erase( g_TokenList.begin() ) ;
    is_array_type = false ;
  }  while ( token.m_type == "," ) ;
} // Execute_Declaration()

void Execute_Compound_statement() {
  Token token ;
  g_TokenList.erase( g_TokenList.begin() ) ;
  token = g_TokenList[0] ;
  while ( token.m_type != "}" ) {
    if ( token.m_type == "INT" || token.m_type == "CHAR" || token.m_type == "FLOAT" ||
         token.m_type == "BOOL" || token.m_type == "STRING" )
      Execute_Declaration() ;
    else
      Execute_Statement() ;
    token = g_TokenList[0] ;
  } // while

  g_TokenList.erase( g_TokenList.begin() ) ;
} // Execute_Compound_statement()

void Execute_Definition() {
  bool exist_var = 0, exist_funct = 0 ;
  // Variable
  if ( g_variable_or_function == 1 ) {
    for ( int i = 0 ; i < g_TokenList.size() ; i++ ) {
      if ( g_TokenList[i].m_type == "Identifier" ) {
        for ( int j = 0 ; j < g_IdentTable[0].size() ; j++ ) {
          if ( g_TokenList[i].m_value == g_IdentTable[0][j].m_name ) {
            exist_var = 1 ;
            if ( i+2 < g_TokenList.size() && g_TokenList[i+2].m_type == "Constant" ) {
              if ( g_TokenList[i+2].m_const_type != "int" ) throw "Array size is non int." ;
              int ident_array_size = atoi( g_TokenList[i+2].m_value.c_str() ) ;
              g_IdentTable[0][j].Redefine_Var( g_TokenList[0].m_value, g_TokenList[i].m_value,
                                               g_TokenList[i+2].m_value, "var" ) ;
              for ( int k = 0 ; k < ident_array_size ; k++ ) {
                Content content ;
                content.m_type = g_IdentTable[0][j].m_type ;
                g_IdentTable[0][j].m_content_array.push_back( content ) ;
              } // for
            } // if
            else
              g_IdentTable[0][j].Redefine_Var( g_TokenList[0].m_value,
                                               g_TokenList[i].m_value, "-1", "var" ) ;
            cout << "New definition of " << g_TokenList[i].m_value << " entered ..." << endl ;
          } // if
        } // for

        if ( exist_var == 0 ) {
          Ident ident ;
          if ( i+2 < g_TokenList.size() && g_TokenList[i+2].m_type == "Constant" ) {
            if ( g_TokenList[i+2].m_const_type != "int" ) throw "Array size is non int." ;
            int ident_array_size = atoi( g_TokenList[i+2].m_value.c_str() ) ;
            ident.Redefine_Var( g_TokenList[0].m_value, g_TokenList[i].m_value,
                                g_TokenList[i+2].m_value, "var" ) ;
            for ( int k = 0 ; k < ident_array_size ; k++ ) {
              Content content ;
              content.m_type = ident.m_type ;
              ident.m_content_array.push_back( content ) ;
            } // for
          } // if
          else
            ident.Redefine_Var( g_TokenList[0].m_value, g_TokenList[i].m_value, "-1", "var" ) ;
          g_IdentTable[0].push_back( ident ) ;
          cout << "Definition of " << g_TokenList[i].m_value << " entered ..." << endl ;
        } // if

        exist_var = 0 ;
      } // if
    } // for
  } // if
  // Function
  else {
    for ( int i = 0 ; i < g_IdentTable[0].size() ; i++ ) {
      if ( g_TokenList[1].m_value == g_IdentTable[0][i].m_name ) {
        exist_funct = 1 ;
        g_IdentTable[0][i].Redefine_Funct() ;
        cout << "New definition of " << g_TokenList[1].m_value << "() entered ..." << endl ;
      } // if
    } // for

    if ( exist_funct == 0 ) {
      Ident ident ;
      ident.Redefine_Funct() ;
      g_IdentTable[0].push_back( ident ) ;
      cout << "Definition of " << g_TokenList[1].m_value << "() entered ..." << endl ;
    } // if

    exist_funct = 0 ;
  } // else
} // Execute_Definition()

void Execute_User_input() {
  if ( g_definition_or_statement == 1 )
    Execute_Definition() ;
  else {
    Execute_Statement() ;
    cout << "Statement executed ..." << endl ;
  } // else
} // Execute_User_input()

void Execute_Parser() {
  // Next token correct line
  try {
    Execute_User_input() ;
  }  catch( const char * msg ) {
    cout << msg << endl ;
  } // catch

  g_cout = 0, g_para = 0 ;
  g_TokenList.clear() ;
} // Execute_Parser()

// EXECUTE_END------------------------------------------------------------------------

int main() {
  string uTestNum ;
  cin >> uTestNum ;
  vector<Ident>global_IdentTable ;
  g_IdentTable.push_back( global_IdentTable ) ;

  cout << "Our-C running ..." << endl ;
  cout << "> " ;
  Token token = Retrieve_Token() ;
  while ( token.m_value != "" ) {
    try {
      Parser( token ) ;
      if ( g_IdentTable.size() > 1 )
        g_IdentTable.erase( g_IdentTable.begin() + 1, g_IdentTable.end() ) ;
      Execute_Parser() ;
    }  catch( const char * msg ) {
      cout << "> Line " << token.m_row << " : " << msg << " \'" << token.m_value << "\'" << endl ;
    } // catch

    cout << "> " ;
    token = Retrieve_Token() ;
  } // while
} // main()
