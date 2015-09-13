
// CS251 Data Structures
// String Implementation
// IMPORTANT: Do not use any of the functions in the string C runtime library
// Example. Do not use strcpy, strcmp, etc. Implement your own

// IMPORTANT: See the MyString.h file for a description of
// what each method needs to do.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MyString.h"

// My own implementation of strlen
int MyString::slength(const char *s) const {
  // Add implementation here
  int length = 0;
  while (*s++) {
    length++;
  }
  return length;
}

// Initialize _s. Allocate memory for _s and copy s into _s
void MyString::initialize(const char * s) {
  // Add implementation here
  // Allocate memory for _s.

  _s = new char[slength(s) + 1];

  // Copy s into _s
  int i = 0;
  while (s[i] != '\0') {
    _s[i] = s[i];
    i++;
    //s++;
  }
  _s[i] = '\0';
  //strcpy(_s, s);
}

// Create a MyString from a C string
MyString::MyString(const char * s) {
  initialize(s);
}

// Create a MyString from a copy of another string
MyString::MyString(const MyString &s) {
  initialize(s._s);
}

// Create a MyString with an empty string
MyString::MyString() {
  _s = new char[1];
  *_s = 0;
}

// Assignment operator. Without this operator the assignment is
// just a shallow copy that will copy the pointer _s. If the original _s
// goes away then the assigned _s will be a dangling reference.
MyString & MyString::operator = (const MyString & other) {
  if (this != &other) // protect against invalid self-assignment
  {
    // deallocate old memory
    delete [] _s;

    // Initialize _s with the "other" object.
    initialize(other._s);

    // by convention, always return *this
    return *this;
  }
}

// Obtain a substring of at most n chars starting at location i
// if i is larger than the length of the string return an empty string.
/*MyString MyString::substring(int i, int n) {
  // Add implementation here

  // Make sure that i is not beyond the end of string
  if (i > length()) {
    return MyString();
  }
  // Allocate memory for substring
  //subString = new char[i];
  // Copy characters of substring
  MyString sub;
  char * substr;
  substr = new char[n + 1];
  int j = 0;
  while (j < n) {
    substr[j] = _s[i + j];
    j++;
  }
  substr[j] = '\0';

  sub = MyString(substr);
  // Return substring
  return sub;
}*/

MyString MyString::substring(int i, int n) {
    if (i > length())
    {
        return MyString();
    }

    char *_sub;
    _sub = new char[n + 1];

    int j;
    for (j = 0; j < n; j++)
    {
        _sub[j] = _s[i+j];
    }

    _sub[n] = '\0';

    MyString sub;
    sub = MyString(_sub);

    return sub;
}

// Remove at most n chars starting at location i
void MyString::remove(int i, int n)
{
  // Add implementation here
  // If i is beyond the end of string return
  if (i > length()) {
    return;
  }
  // If i+n is beyond the end trunc string
  int l = 0;
  if (i + n - 1 > length()) {
    _s[i] = '\0';
  } else {
    while (_s[i + n - 1] != '\0') {
    _s[i] = _s[i + n];
      i++;
    }
  }
  // Remove characters
}

// Return true if strings "this" and s are equal
bool MyString::operator == (const MyString & s)
{
  // Add implementation here
  int i = 0;
  while ((_s[i] != '\0' && s._s[i] != '\0') && (_s[i] == s._s[i])) {
    i++;
  }
  return ((_s[i] - s._s[i]) == 0);
}


// Return true if strings "this" and s are not equal
bool MyString::operator != (const MyString &s)
{
  // Add implementation here
  int i = 0;
  while ((_s[i] != '\0' && s._s[i] != '\0') && (_s[i] == s._s[i])) {
    i++;
  }

  return ((_s[i] - s._s[i]) != 0);
}

// Return true if string "this" and s is less or equal
bool MyString::operator <= (const MyString &s)
{
  // Add implementation here
  int i = 0;
  while ((_s[i] != '\0' && s._s[i] != '\0') && (_s[i] == s._s[i])) {
    i++;
  }

  return ((_s[i] - s._s[i]) <= 0);
}

// Return true if string "this" is greater than s
bool MyString::operator > (const MyString &s)
{
  // Add implementation here
  int i = 0;
  while ((_s[i] != '\0' && s._s[i] != '\0') && (_s[i] == s._s[i])) {
    i++;
  }
  return ((_s[i] - s._s[i]) > 0);
}

// Return character at position i.  Return '\0' if out of bounds.
char MyString::operator [] (int i)
{
  // Add implementation here
  char * ret = new char();
  if (i < length()) {
    ret = _s + i;
    return *ret;
  }
  return '\0';
}

// Return C representation of string
const char * MyString::cStr()
{
  // Add implementation here
  return _s;
}

// Get string length of this string.
int MyString::length() const
{
  // Add implementation here
  return slength(_s);
}

// Destructor. Deallocate the space used by _s
MyString::~MyString()
{
  // Add implementation here
  delete [] _s;
}

// Concatanate two strings (non member method)
MyString operator + (const MyString &s1, const MyString &s2)
{
  // Add implementation here

  // Allocate memory for the concatenated string
  MyString s;
  int size;

  size = s1.length() + s2.length() + 1;

  s._s = new char[size];

  // Add s1
  int i = 0;
  while (s1._s[i] != '\0') {
    s._s[i] = s1._s[i];
    i++;
  }
  // Add s2
  int j = 0;
  while (s2._s[j] != '\0') {
    s._s[i] = s2._s[j];
    i++;
    j++;
  }
  return s;
}
