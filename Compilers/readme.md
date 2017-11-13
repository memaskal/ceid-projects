# Programming Languages & Compilers

The purpose of this assignment was to build a simple parser for the [Golub](#golub-grammar) language. Loop structures where added and appropriate loop handling mechanisms were developed. Namely, the parser calculates the number of loop iterations in a for loop, and perform a for to while conversion, when the #iterations > 3, otherwise loop unrolls the contents of the loop - body.

# Tools

For the description of this language the [Bison](https://www.gnu.org/software/bison/manual/bison.html) syntax analyzer and the [Flex](http://flex.sourceforge.net/) lexical analyzer were used. These tools work together seamlessly to create a parser, bonded together with some ❤ and a bit of C. The description files in `src` directory are used to compile the language.     

# Usage

To compile the parser, simply run the `compile.sh` file from a shell. An executable named `myParser` will be created. Suppling the program with an input file written in **Golub**, like the ones in `data` directory produces an `output` file containing the code with loop handling code when applicable, and a status message on succesful analysis.

# Golub Grammar

``` bnf
<program> 		::= <extern_declarations><program_header><definitions_section><commands_section>
<extern_declarations> 	::= (<external_prototype>)* 
<external_prototype> 	::= extern <function_prototype> 
<program_header> 	::= void<id>()
<definitions_section>	::= (<deffinition>)*
<deffinition>		::= <variable_definition>|<function_definition>|<function_prototype>
<variable_definition>	::= <data_types><variable_list>;
<data_types>		::= integer|boolean|string
<variable_list>		::= <id>(,<id>)*
<function_definition>	::= <function_header><definitions_section><commands_section>
<function_prototype>	::= <function_header>;
<function_header>	::= <function_data_types><id>([<typical_par_list>])
<function_data_types>	::= integer|boolean|void
<typical_par_list>	::= <typical_par>(,<typical_par>)*
<typical_par>		::= <data_types>[&]<id>
<commands_section>	::= begin(<command>)*end
<command>		::= <simple_command>;|<structured_command>|<complex_command>
<complex_command>	::= {(<command>)*}					  
<structured_command>	::= <if_command>					  
<simple_command>	::= <assignment>|<function_call>|<return_command>|<null_command>
<if_command>		::= if(<gen_expression>)<command>[<else-cluase>]
<else-clause>		::= else<command>							
<assignment>		::= <id>=<gen_expression> 
<function_call>		::= <id>([<real_par_list>])
<real_par_list>		::= <real_par>(,<real_par>)*
<real_par>		::= <gen_expression>
<return_command>	::= return[<gen_expression>]					  
<null_command>		::= ε
<gen_expression>	::= <gen_term>(||<gen_term>)*
<gen_term>		::= <gen_factor>(&&<gen_factor>)*
<gen_factor>		::= [!]<gen_first_factor>					  
<gen_first_factor>	::= <sim_expression>[<compare_section>] 
<compare_section>	::= <compare-operator><sim_expression>
<compare-operator>	::= ==|!=|<|>|<=|>=
<sim_expression>	::= <sim_term>((+|-)<sim_term>)*
<sim_term>		::= <sim_factor>((*|/|%)<sim_factor>)*
<sim_factor>		::= [+|-]<sim_first_term>
<sim_first_term>	::= <id>|<constant>|<function_call>|(<gen_expression>)					  
<constant>		::= <int-constant>|<string-constant>|true|false
```
