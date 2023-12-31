

#include "lexer.h"

using namespace std;

lexer::lexer(std::ifstream* source) {
	sourceFile = source; 
	lineCount = 1;
	charCount = 1;
}

lexer::~lexer() {
}

void lexer::lexerReset(){
	sourceFile->clear();
	sourceFile->seekg(0, ios::beg);
}

bool lexer::isPunction(char c){
  if('(' == c || ')' == c || ';' == c || ',' == c)
    return true;
  else
    return false;
}

bool lexer::isOperatorSymbol(char c){
	 if ('+' == c || '-' == c || '*' == c || '<' == c || '>' == c || '*' == c || '&' == c || '.' == c
	    || '@' == c|| '/' == c|| ':'== c || '=' == c|| '~' == c|| '|'== c || '$'== c || '!' == c
	    || '#'== c || '%'== c || '^'== c || '_'== c || '['== c || ']'== c || '{'== c || '}'== c
	    || '"'== c || '`'== c || '?'== c)
	        return true;
	    else
	        return false;
}

bool lexer::isCommentChar(char c){
	if (39 == c) 
		return true;
	else
		return false;
}

string lexer::tokenIdentifier(){
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do {
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	}while (!(EOF == nextPeek) && (isalpha(nextPeek) || isdigit(nextPeek) || (95 == nextPeek)));
	return tokStr;
}

string lexer::tokenInteger(){
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do {
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	}while (!(EOF == nextPeek) && isdigit(nextPeek));
	return tokStr;
}

string lexer::tokenStrings(){
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do{
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
		if (nextChar == '\\'){
		    if ((nextPeek == 't' || nextPeek == 'n' || nextPeek == '\\' || nextPeek == '\''))
		    {
		        continue;
		    } else {
		        printf ("Invalid escape sequence\n");
		        exit(0);
		    }
		}
	} while (!(EOF == nextPeek) && !(39 == nextPeek));
	sourceFile->get(nextChar);
	tokStr += nextChar;
	return tokStr;
}

string lexer::tokenSpaces(){
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do{
		sourceFile->get(nextChar);
		charCount++;
		if (10 == nextChar || 13 == nextChar){
			charCount = 1;
			lineCount++;
		}
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	} while (!(EOF == nextPeek) && isspace(nextPeek));
	return tokStr;
}

string lexer::tokenComment(){
	string tokStr= "";
	char nextPeek;
	char nextChar;
	sourceFile->get(nextChar);
	charCount++;
	tokStr += nextChar;
	nextPeek = sourceFile->peek();
	if (47 == nextPeek){
		sourceFile->get(nextChar);
		tokStr += nextChar;
		do{
			sourceFile->get(nextChar);
			tokStr += nextChar;
			nextPeek = sourceFile->peek();
		} while (!(EOF == nextPeek) && !(10 == nextPeek || 13 == nextPeek));
		return tokStr;
	} else {
		return tokStr;
	}
}

string lexer::tokenOperator(){
	string tokStr= "";
	char nextPeek;
	char nextChar;
	do{
		sourceFile->get(nextChar);
		charCount++;
		tokStr += nextChar;
		nextPeek = sourceFile->peek();
	} while (!(EOF == nextPeek) && isOperatorSymbol(nextPeek));
	return tokStr;
}

// The read method in the parser invokes this function to retrieve the new token. 
// According to the rpal lexicon, the first character determines the applicable rule, and a specific function is called accordingly. 
// The function continues reading subsequent characters until the newly read character no longer conforms to the selected rule. 
// This process signifies the completion of the token, which is then passed back to the parser.


token* lexer::getNextToken(){
	token* tok = new token();
	string tokenizedLex;
	int nextChar;
	char readNext;
	nextChar = sourceFile->peek();
	if (isalpha(nextChar)){
		if (LEXLOGS) printf ("This is an identifier\n");
		tokenizedLex = tokenIdentifier();
        tok->tokType = TOK_IDENTIFIER;
	} else if (isdigit(nextChar)) {
		if (LEXLOGS) printf ("This is an integer\n");
		tokenizedLex = tokenInteger();
		tok->tokType = TOK_INTEGER;
	} else if (39 == nextChar){
		if (LEXLOGS) printf ("This is a string\n");
		tokenizedLex = tokenStrings();
		tok->tokType = TOK_STRING;
	} else if (isspace(nextChar)){
		if (LEXLOGS) printf ("This is an space\n");
		tokenizedLex = tokenSpaces();
		tok->tokType = TOK_DELETE;
	} else if (isPunction(nextChar)){
		if (LEXLOGS) printf ("This is a punction\n");
		sourceFile->get(readNext);
		charCount++;
		tokenizedLex = readNext;
		tok->tokType = TOK_PUNCTION;
	} else if (47 == nextChar) {
		tokenizedLex = tokenComment();
		if (!tokenizedLex.compare("/")){
			if (LEXLOGS) printf ("This is an operator\n");
			tok->tokType = TOK_OPERATOR;
		} else{
			if (LEXLOGS) printf ("This is a comment\n");
			tok->tokType = TOK_DELETE;
		}
	} else if (isOperatorSymbol(nextChar)){
		if (LEXLOGS) printf ("This is an operator\n");
		tokenizedLex = tokenOperator();
		tok->tokType = TOK_OPERATOR;
	} else if (EOF == nextChar) {
		if (LEXLOGS) printf ("EOF reached\n");
		tok->tokType = TOK_EOF;
	}
	tok->tokValue = tokenizedLex;
	tok->charCount = charCount;
	tok->lineCount = lineCount;
	if (LEXLOGS) printf ("Lexer returning token type = %d, charCount = %d, lineCount = %d, value = %s\n",
			tok->tokType, tok->charCount, tok->lineCount, tok->tokValue.c_str());
	return tok;
}
