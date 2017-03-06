#!/bin/bash

################################################################
# LAB 1 PART 2 GRADING SCRIPT
################################################################
#
# The script needs to be in same directory as the SimpleC File
# and Expects there to be a tests folder with all of the
# provided c files. The provided token.py file should also
# be in the same directory
#
# There are two kinds of tests
# 1) Specific tokens (ex. "char*", "'c'")
# 2) The provided c files (ex. hello.c, ampersand.c)
#
# The script runs the output through the SimpeC Compiler and
# uses the token.py script to remove token names and replace
# them with integer values. The integers are assigned in
# increasing order and all subsequent tokens of that type
# will have the same integer ID. This allows the tokens
# names to be arbitrary as long as no two tokens have the
# same name
#
# Here is an example for hello.c
# The first token is void, the second is id...
# 
# void -> 0
# main -> 1
# ( -> 2
# ) -> 3
# { -> 4
# printf -> 1
# ( -> 2
# Hello world...\n -> 5
# ) -> 3
# ; -> 6
# } -> 7
################################################################


# test lexing a single token
testtok() {
    token=$1
    weight=$2

    echo "--------------------------------------------------"
    echo "token: $token $weight pts"
    echo "--------------------------------------------------"
    solution_out=$(printf $token | java SimpleC | token.py)
    echo "$solution_out"
    echo ""
}

# test lexing all of the tokens in a file
testfile() {
    file=$1
    weight=$2

    echo "--------------------------------------------------"
    echo "file: $file $weight pts"
    echo "--------------------------------------------------"
    solution_out=$(java SimpleC < $file | token.py)
    echo "$solution_out"
    echo ""
}

testkeywords() {
    # Test keywords 15 points
    testtok "char*"                 1
    testtok "char**"                1
    testtok "long*"                 1
    testtok "long"                  1
    testtok "void"                  1
    testtok "if"                    1
    testtok "else"                  1
    testtok "while"                 1
    testtok "do"                    1
    testtok "for"                   1
    testtok "continue"              1
    testtok "break"                 1
    testtok "return"                1
    testtok "double"                1
    testtok "double*"               1
}

testbasicsymbols() {
    #test operators/symbols 23 points
    testtok "("                     1
    testtok ")"                     1
    testtok "["                     1
    testtok "["                     1
    testtok "{"                     1
    testtok "}"                     1
    testtok ","                     1
    testtok ";"                     1
    testtok "="                     1
    testtok "&"                     1
    testtok "||"                    1
    testtok "&&"                    1
    testtok "=="                    1
    testtok "!="                    1
    testtok "<"                     1
    testtok ">"                     1
    testtok "<="                    1
    testtok ">="                    1
    testtok "+"                     1
    testtok "-"                     1
    testtok '\052'                  1
    testtok '/'                     1
    testtok '\045'                  1
}

testcomplex() {
    # test regex based tokens 22 points
    testtok '/\052\052/test'        2
    testtok '/\052comment\052/test' 1
    testtok '/\052double(10)\052/test' 1
    testtok "//\ntest"              2
    testtok "//comment\ntest"       1
    testtok "//double(10)\ntest"    1
    testtok "hello"                 2
    testtok "ThIsIs_4_T35T"         2
    testtok "'c'"                   2
    testtok "'!'"                   2
    testtok "'\\045'"               1
    testtok "10"                    1
    testtok "1234567891234567890"   2
    testtok "1234567890.0987654321" 2
    testtok ".1337E4"               1
}

testfiles() {
    # test against provided files 32 points
    files=(tests/*)
    for ((i=0; i<${#files[@]}; i++)); do
        testfile "${files[$i]}" 1
    done
} 

testkeywords
testbasicsymbols
testcomplex
testfiles
