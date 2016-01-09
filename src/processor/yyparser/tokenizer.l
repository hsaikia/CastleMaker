%{
#include "grammar.tab.hpp"
#include <cstdio>
#include <vector>
#include <string>
%}

%option noyywrap
%x ML_COMMENT

%%

<INITIAL>"/*"                  BEGIN(ML_COMMENT);
<ML_COMMENT>"*/"               BEGIN(INITIAL);  
<ML_COMMENT>.|"\n"              { }

("-")?([0-9]+|[0-9]+\.[0-9]+|\.[0-9]+) {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return VALUE;
}

"include" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return INCLUDE;
}

"attr" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	//printf("Attr found\n"); 
	return ATTR;
}

"=" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return EQUALS;
}

"(" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return LP;
}

")" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return RP;
}

"{" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return LCP;
}

"}" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret; 
	return RCP;
}


"[" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	return LBP;
}

"]" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret; 
	return RBP;
}

"-->" {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);
	yylval.sym = ret;
	//printf("Transform found\n"); 
	return TRANSFORM;
}

[a-z][a-z0-9A-Z_.+]* {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);	
	yylval.sym = ret;
	return VARIABLE;
}

[A-Z][a-zA-Z0-9_]* {
	char *ret = new char[strlen(yytext) + 1];
	strcpy(ret, yytext);	
	yylval.sym = ret;
	return LITERAL;
}

";" {
	yylval.sym = yytext; 
	return END;
}

"\"" {
	yylval.sym = yytext;
	return DQ;
}

[:|, \t\n]* {

}

. {
	printf("Error in rule file!\n"); 
	//exit(1);
}

%%


