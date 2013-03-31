
/**
 *
 * @file: DeclarationVariablesGlobales.h
 *
 * @Author: lublud
 *
 * @Date: 28/3/13
 *
 * @Synopsis: Liste des variables globales utilisée dans les fichiers sources.
 *
**/

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

enum
{
PRGM = 1000,
BODY,
DECLVAR,
DECLFUNC,
DECLPROC,
PARAMETRE,
INSTRUCTION,
WHILEDO,
IFTHENELSE,
AFF,
ID,
APPELPROC,
ARGUMENTS,
BLOC,
EXPRESSION,
CONJONCTION,
COMPARAISON,
SUITECOMPARAISON,
EXPRARITH,
TERME,
FACTEUR,
LISTEEXPR,
VARIABLE
};

enum
{
	TYPE_SNOEUDFILS = 2000,
	TYPE_SNOEUDFRERE,
	TYPE_INT,
	TYPE_NOM
};

FILE *yyin;
char yytext[1024];
int LinePas = 1;
char *FilePas;
int lexeme;
