# Programming Languages & Compilers

The purpose of this assignment was to build a simple parser for the **Golub** language. The basic grammar rules of the language were given and loop control structures were added later. Namely, the parser calculates the number of loop iterations in a for loop, and perform a for to while conversion, when the #iterations > 3, otherwise loop unrolls the contents of the loop - body.

# Tools

For the description of this language the [Bison](https://www.gnu.org/software/bison/manual/bison.html) syntax analyzer and the [Flex](http://flex.sourceforge.net/) lexical analyzer were used. These tools work together seamlessly to create a parser, and are bonded together with some C. The description files in `src` directory are used to compile the language.     

# Usage

To compile the parser, simply run the `compile.sh` file from a shell. An executable named `myParser` will be created. Suppling the program with an input file written in **Golub**, like the ones in `data` directory produces an `output` file containing the code with loop handling code when applicable, and a status message on succesful analysis.