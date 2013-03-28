/**
 *
 * @file: main.c
 *
 * @Author: lublud
 *
 * @Date: 28/3/13
 *
 * @Synopsis: Programme principale du projet
 *
**/

#include "include.h"

int main (int argc, char *argv[])
{

	if (1 + 1 != argc)
	{
		printf ("You're a n00b, expected only one argument");
		exit (1);
	}
	FilePas = argv[1];

	SNoeud *Racine = CreerNoeud ();

	yyin = fopen (argv[1], "r");
	lexeme = yylex ();
	Racine = ProgrammePascal ();
	fclose (yyin);
} // main()
