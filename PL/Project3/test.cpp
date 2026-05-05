#include <iostream>

using namespace std;

string To_string( float f ) {
  char temp[100];
  sprintf( temp, "%f", f );
  return string( temp ) ;
} // To_string()

int main() {
    
  // preparation

  int a1, a2, a3 ;
  float f1, f2, f3 ;
  bool b1, b2, b3 ;
  string s1, s2, s3 ;
  char c1, c2, c3 ;
  int a[10], b[10] ;
  string s[10] ;
  int i, j, k ;

  // Problem 3 and 4 : test data 1/3 (viewable)

  a1 = 10 ;
  a2 = 20 ;
  a3 = -30 ;
  f1 = 1.5 ;
  f2 = 0.25 ;
  f3 = -0.125 ;
  s1 = "Hi" ;
  s2 = "Hello" ;
  s3 = "What" ;
  c1 = 'a' ;
  c2 = 'b' ;
  c3 = 'c' ;
  s1 = "-1000";
  cout << atoi(s1.c_str()) << endl ;
  cout << To_string(f3);
}