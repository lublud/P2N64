/**
 *
 * @file: AnalyseurLex.c
 *
 * @Author: lublud
 *
 * @Date: 7/2/13
 *
 * @Synopsis: Lire un fichier texte caractère par caractère (en pascal), 
 * 				et renvoyer des lexèmes (mots) à l'analyseur syntaxique
 *
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PROGRAM 256
#define IDENTIFIANT 257
#define NOMBRE 258
#define DIFFERENT 259
#define INFEGAL 260
#define SUPEGAL 261
#define AFFECTATION 262
#define BORNETABLEAU 263 // correspond au .. lors de la création 
						 // d'un tableau array[0..1] tableau entre 0 et 1.
#define DIV 264 // division entière !
#define MOD 265
#define OR 266
#define AND 267
#define NOT 268
#define VAR 269
#define INTEGER 270
#define BOOL 271
#define TRUE 272
#define FALSE 273
#define PROCEDURE 274
#define FUNCTION 275
#define BEGIN 276
#define END 277
#define WHILE 278
#define DO 279
#define IF 280
#define THEN 281
#define ELSE 282
#define READLN 283
#define WRITELN 284
#define ARRAY 285
#define OF 286

enum
{
	FIRSTCHAR = 0,
	DIGIT,
	CHAR,
	DPOINT,
	POINT,
	INF,
	SUP
};

FILE *yyin;
char yytext[1024];
int LinePas = 1;

int isotherchar (int c)
{
	if ('[' == c || ']' == c || '(' == c || ')' == c ||
		'+' == c || '-' == c || '*' == c || ',' == c ||
		':' == c || ';' == c || '=' == c)
		return 1;
	return 0;
} // isotherchar ()

int ChercherLexeme (char yytext[])
{
	char *lexeme[24] = {"div", "mod", "or", "and", "not", "program", "var",
				"integer", "boolean", "true", "false", "procedure",
				"function", "begin", "end", "while", "do", "if", "then",
				"else", "readln", "writeln", "array", "of"};
	int Lexeme[24] = {DIV, MOD, OR, AND, NOT, PROGRAM, VAR,
				INTEGER, BOOL, TRUE, FALSE, PROCEDURE,
				FUNCTION, BEGIN, END, WHILE, DO, IF, THEN,
				ELSE, READLN, WRITELN, ARRAY, OF};

	int i;
	for (i = 0; i < 24; ++i)
		if (!strcmp(tolower(yytext), lexeme[i]))
			break;

	return 23 < i ? IDENTIFIANT : Lexeme[i];

} // ChercherLexeme ()

int yylex()
{
	int etat, pText, c;
	etat = pText = 0;

	for (;;)
	{

		c = fgetc (yyin);

		switch (etat)
		{
			case FIRSTCHAR:
				if ('\n' == c)
					++LinePas;
				if (isspace (c))
					break;
				yytext[pText++] = c;

				if (isalpha (c) || '_' == c)
					etat = CHAR;
				else if (isdigit (c))
					etat = DIGIT;
				else if (':' == c)
					etat = DPOINT;
				else if ('<' == c)
					etat = INF;
				else if ('>' == c)
					etat = SUP;
				else if ('.' == c)
					etat = POINT;
				else
				{
					yytext[pText++] = '\0';
					return c;
				}

				break;

			case DIGIT:
				if (isdigit(c))
					yytext[pText++] = c;
				else
				{
					ungetc(c, yyin);
					yytext[pText++] = '\0';
					return NOMBRE;
				}
				break;

			case CHAR:
				if (isalnum(c) || '_' == c)
					yytext[pText++] = c;
				else
				{
					ungetc(c, yyin);
					yytext[pText++] = '\0';
					return ChercherLexeme(yytext);
				}
				break;

			case DPOINT:
				if ('=' == c)
				{
					yytext[pText++] = c;
					yytext[pText++] = '\0';
					return AFFECTATION;
				}
				else
				{
					ungetc(c, yyin);
					yytext[pText++] = '\0';
					return ':';
				}
			break;

			case INF:
				if ('>' == c)
				{
					yytext[pText++] = c;
					yytext[pText++] = '\0';
					return DIFFERENT;
				}
				else if ('=' == c)
				{
					yytext[pText++] = c;
					yytext[pText++] = '\0';
					return INFEGAL;
				}
				else
				{
					ungetc(c, yyin);
					yytext[pText++] = '\0';
					return '<';
				}
				break;

			case SUP:
				if ('=' == c)
				{
					yytext[pText++] = c;
					yytext[pText++] = '\0';
					return SUPEGAL;
				}
				else
				{
					ungetc(c, yyin);
					yytext[pText++] = '\0';
					return '>';
				}
				break;

			case POINT:
				if ('.' == c)
				{
					yytext[pText++] = c;
					yytext[pText++] = '\0';
					return BORNETABLEAU;
				}
				else
				{
					ungetc(c, yyin);
					yytext[pText++] = '\0';
					return '.';
				}
				break;

		}
	}
} // yylex()

//int main (int argc, char *argv[])
//{
//	
//	int lexeme;
//	if (1 + 1 != argc)
//	{
//		printf ("You're a n00b, expected only one argument");
//		exit (1);
//	}
//
//	yyin = fopen (argv[1], "r");
//	for (lexeme = yylex(); EOF != lexeme; lexeme = yylex())
//		printf ("%d\n", lexeme);
//	fclose (yyin);
//} // main()
