%{
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#define BUFF_SIZE 256
	#define BUFF_LARGE 16*1024
		
	// Extern vars
	extern FILE *yyin;
	extern FILE *yyout;
	extern char* yytext;
	extern int yylineno;
	
	int errors = 0;
	
	// Loop handling vars declaration 
	typedef enum {FORTOWHILE, UNROLLING, NONE} loopHandling;
	loopHandling opperation = NONE;
	int maxIndex, minIndex, stepIndex;
	char *varName;
	
	int diff = 0, changes = 0, depth = 0;
	char helperBuff[BUFF_SIZE + 1];
	char unrollBuffer[BUFF_LARGE + 1];
	
	// Function declarations
	void yyerror(const char*);
	void print(const char*);
%}

/* For better error reporting */
%error-verbose

%union {
	int intval;
	char *strval;
	int subtok;
}

/* Tokens declarations */
%token EXTERN_K;
%token VOID_T;
%token INTEGER_T; 
%token BOOLEAN_T; 
%token STRING_T; 
%token BEGIN_K; 
%token END_K; 
%token IF_K; 
%token ELSE_K; 
%token RETURN_K; 

%token WHILE_K;
%token FOR_K;

%token INT;
%token BOOL;
%token STRING;
%token ALPHANUMERIC;

%token OR_OP;
%token AND_OP;
%token COMPARISON;
	
/* Resolve recuce - reduce conflict in if else stamements */
%nonassoc LOWER_THAN_ELSE	// lower precedence
%nonassoc ELSE_K			// higher precedence 

/* Set some token data types used in loop handling */
%type <intval> INT;				/* int constants */
%type <strval> ALPHANUMERIC;	/* string constants */

%%	
	program 			: extern_declarations  program_header  definitions_section  commands_section
						  ;
	
	extern_declarations : external_prototype extern_declarations 
						  | /*  empty  */ 
						  ;
	
	external_prototype 	: EXTERN_K { print("extern"); } function_prototype 
						  ;
	
	program_header 		: VOID_T  ALPHANUMERIC '('')'  { print("void "); print($2); print("()"); }
						  ;
	
	definitions_section	: deffinition definitions_section 
						  | /*  empty  */ 
						  ;
	
	deffinition			: variable_definition
						  | function_definition
						  | function_prototype
						  ;
	
	variable_definition	: basic_data_alpha variable_list_zom ';' { print(yytext); }
						  | STRING_T { print(yytext); } variable_list_zom ';' { print(yytext); }
	                      | error {yyclearin;}
						  ;
	
	/* Limited lookahead trick  */
	basic_data_alpha	: basic_data_types ALPHANUMERIC	{ print($2); }
						  ;

	basic_data_types	: INTEGER_T		{ print(yytext); }
						  | BOOLEAN_T   { print(yytext); }
						  ;
	
	variable_data_type	: basic_data_types
						  | STRING_T { print(yytext); } /* just for vars */
						  ;
	
	variable_list_zom	: ',' ALPHANUMERIC { print(","); print($2);} variable_list_zom
						  | /*  empty  */
						  ; 

	function_definition	: function_header definitions_section commands_section
						  ;
	
	function_prototype	: function_header ';' { print(yytext); }
						  ;
				
	function_header		: basic_data_alpha '(' { print("("); } typical_par_list ')' { print(")"); }
						  | VOID_T ALPHANUMERIC { print("void "); print($2); } '(' { print("("); } typical_par_list ')' { print(")"); }
						  ;
						  
	typical_par_list	: typical_par typical_par_zom
						  | /*  empty  */ 
						  ; 
						  
	typical_par_zom 	: ',' { print(","); } typical_par typical_par_zom
						  | /*  empty  */ 
						  ; 
	
	typical_par			: variable_data_type '&'  ALPHANUMERIC { print("&"); print($3); }
						  | variable_data_type ALPHANUMERIC { print($2); }
						  ;
	
	commands_section	: BEGIN_K { print(yytext); } commands_zom END_K { print(yytext); }
						  ;
	
	commands_zom		: command commands_zom 
						  | /*  empty  */ 
						  ;
	
	command				: simple_command ';' { print(yytext); }
						  | structured_command
						  | complex_command
						  | error {yyclearin;}
						  ;
	
	complex_command		: '{' { print(yytext); } commands_zom '}' { print(yytext); }
						  ;
						  
	structured_command	: if_command
						  | while_command
						  | for_command
						  ;
						  
	simple_command		: assignment
						  | function_call
						  | return_command
						  | null_command
						  ;
	
	
	/*red-reduce hax*/
	if_cmnd_name_print  : IF_K  { print(yytext); } '(' { print("("); } gen_expression ')' { print(")"); } command 
						;

	if_command			: if_cmnd_name_print %prec LOWER_THAN_ELSE;
						  | if_cmnd_name_print ELSE_K { print(yytext); } command
						  ;
	
	while_command		: WHILE_K { print(yytext); } '(' { print("("); } gen_expression ')' { print(")"); } command ;
			
	for_command			: FOR_K  '(' ALPHANUMERIC ';' INT ';' INT ';' INT ')' { 
							
							if (opperation == NONE) {
								varName   = $3;
								minIndex  = $5;
								maxIndex  = $7; 
								stepIndex = $9;
								
								diff = (maxIndex - minIndex) / stepIndex;								
								if ( diff > 3 ) {
									opperation = FORTOWHILE;
									sprintf(helperBuff, "%s = %d;\nwhile (%s < %d) {", varName, minIndex, varName, maxIndex);
									print(helperBuff);
								}
								else if (diff >= 0) {
									// no negative steps
									opperation = UNROLLING;
									unrollBuffer[0] = '\0';		
									changes = 0;
								}
							 }
							 else {
								// loop-unroll on single level :(
								sprintf(helperBuff, "for ( %s ; %d ; %d ; %d )", $3, $5, $7, $9);
								print(helperBuff);
							 }
							 ++depth;
						  }
						  command {
							if (--depth == 0) {
							   switch(opperation) {
								case FORTOWHILE:
									opperation = NONE;
									sprintf(helperBuff, "\n\t%s = %s + %d;\n}", varName, varName, stepIndex);
									print(helperBuff);
								break;
								case UNROLLING:
									opperation = NONE;	

									int i, j, len = strlen(unrollBuffer);
									char *p, *token = strtok(unrollBuffer, "#");
									while (token != NULL) token = strtok(NULL, "#");

									for (i = 0; i < diff; ++i) {
										j = 0;
										p = unrollBuffer;
										sprintf(helperBuff, "%d", i * stepIndex + minIndex);		// calculate index val					
										while (p < unrollBuffer + len) {
											print(p);
											if (++j <= changes) {
												print(helperBuff);
											}
											p += strlen(p) + 1;										
										}									
									}								 
							   }	
							}							   
						  }
						  ;
						  
						 		
	assignment			: ALPHANUMERIC '=' { print($1); print("="); } gen_expression 
						  ;
	
	/*reduce- reduce function call*/
	fun_name_print		: ALPHANUMERIC { print($1); }
	
	
	function_call		: fun_name_print '('  { print("("); } real_par_list ')' { print(")"); }
						  | fun_name_print '(' { print("("); } ')'  { print(")"); }
						  ;
	
	real_par_list		: real_par real_paz_zom
						  ;
	
	real_paz_zom		: ',' { print(yytext); } real_par real_paz_zom
						  | /*  empty  */ 
						  ; 
						  
	real_par			: gen_expression
						  ;
						  
	return_command		: RETURN_K { print("return "); }
						  | RETURN_K { print("return "); } gen_expression
						  ;
						  
	null_command		: /*  empty  */ 
						  ;
	
	gen_expression		: gen_term gen_term_zom
						  ;
	
	gen_term_zom		: OR_OP { print("||"); } gen_term gen_term_zom
						  | /*  empty  */ 
						  ;

	gen_term			: gen_factor gen_factor_zom
						  ;
						  
	gen_factor_zom		: AND_OP { print("&&"); } gen_factor gen_factor_zom
						  | /*  empty  */ 
						  ;
	
	gen_factor			: '!' { print("!"); } gen_first_factor
						  | gen_first_factor
						  ;
						  
	gen_first_factor	: sim_expression
						  | sim_expression compare_section 
						  ;
	
	compare_section		: COMPARISON { print(yytext); } sim_expression
						  ;
						  
	sim_expression		: sim_term sim_term_zom
						  ;
	
	sim_term_zom		: '+' 	{ print(yytext); }  sim_term sim_term_zom
						  | '-' { print(yytext); }  sim_term sim_term_zom
						  | /*  empty  */ 
						  ;
	
	sim_term			: sim_factor sim_factor_zom
						  ;
						  
	sim_factor_zom		: '*' { print(yytext); }  sim_factor sim_factor_zom
						  | '/' { print(yytext); }  sim_factor sim_factor_zom
						  | '%' { print(yytext); }  sim_factor sim_factor_zom
						  | /*  empty  */ 
						  ;
	
	sim_factor			: sim_first_term
						  | '+' { print(yytext); }  sim_first_term
						  | '-' { print(yytext); }  sim_first_term
						  ;
						  
						  // On loop unrolling replace alphanumeric token with a "#" character
	sim_first_term		: ALPHANUMERIC { if (opperation == UNROLLING && strcmp(varName, $1) == 0) { print("#"); ++changes; } else print($1); }
						  | constant
						  | function_call
						  | '(' { print("("); } gen_expression ')' { print(")"); }
						  ;
						  
	constant			: INT   	{print(yytext);} 
						  | STRING	{print(yytext);} 
						  | BOOL	{print(yytext);} 
						  ;
						  
%%

/*	Prints content to output stream	 */
void print(const char *s) {
	// If opperation is unrolling put data to buffer
	if (opperation == UNROLLING) {
		strcat(unrollBuffer, s);
	} 
	else {
		fprintf(yyout, "%s", s);
	}
}


/*	Outputs the error message	*/
void yyerror(const char *s) {
	++ errors;
	fprintf(stderr, "Error: %s, in line %d\n", s, yylineno);
}


int main(int ac, char **av) {
	
	if(ac > 1 && (yyin = fopen(av[1], "rb")) == NULL) {
		perror(av[1]);
		exit(1);
	}
	if ((yyout = fopen("output.txt", "w")) == NULL) {
		perror("Can't create output file");
		exit(1);
	}
	yyparse();	
	if(!errors)
		printf("\nGolub parse completed with no errors\n");
	else
		printf("\nGolub parse failed with %d errors", errors);
	
	// close open files
	fclose(yyin);
	fclose(yyout);
	return 0;
}
  