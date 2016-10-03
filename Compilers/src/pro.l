%{
	#include "y.tab.h"  
	#include <stdlib.h>
	#include <stdio.h>
	
	extern int outputEnabled;
	void yyerror(const char*);
	void print(const char*);
	
	extern FILE *yyin;	
%}

/* for case sensitive */
%option noyywrap case-insensitive
/* for line numbering */
%option yylineno

%%
extern 	 { return EXTERN_K;  }
void   	 { return VOID_T;    }
integer	 { return INTEGER_T; }
boolean  { return BOOLEAN_T; }
string	 { return STRING_T;  }
begin	 { return BEGIN_K;   }
end		 { return END_K;     }
if		 { return IF_K;      }
else	 { return ELSE_K;    }
return   { return RETURN_K;  }

while	 { return WHILE_K; 	 }
for		 { return FOR_K;     }

true 	 { yylval.intval = 1; return BOOL; }
false 	 { yylval.intval = 0; return BOOL; }

[0-9]+					{ yylval.intval = atoi(yytext); return INT; }
[a-zA-Z_][a-zA-Z0-9_]*  { yylval.strval = strdup(yytext); return ALPHANUMERIC; }
\"(\\.|\"\"|[^"\n])*\"  { yylval.strval = strdup(yytext); return STRING; }

[\t\r\n]  { print(yytext); 
			}

[-+&()!,;\{\}=*\/%] 	{ return yytext[0]; }


"&&" 	 { return AND_OP; }
"||"	 { return OR_OP; }
"==" 	 { yylval.subtok = 1; return COMPARISON; }
">=" 	 { yylval.subtok = 2; return COMPARISON; }
">" 	 { yylval.subtok = 3; return COMPARISON; }
"<=" 	 { yylval.subtok = 4; return COMPARISON; }
"<" 	 { yylval.subtok = 5; return COMPARISON; }
"!=" 	 { yylval.subtok = 6; return COMPARISON; }

%%