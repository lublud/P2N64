/**
 * 
 * @Author: lublud
 *
 * @Date: 21/2/13
 *
 * @Synopsis: Vérifie que le programme en pascal est correct syntaxiquement
 *
**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AnalyseurLex.c"

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

#define Accept(attendu) AcceptLex(attendu, __FILE__, __LINE__)

int lexeme;

void AcceptLex (const int attendu, const char *File, const int Line)
{
	if (attendu != lexeme)
	{
		printf ("Erreur dans le fichier %s:%d\nAttendu : %d\nReçu : %d\n",
				File, Line);
		exit (1);
	}
	printf ("%d\n", lexeme);
	lexeme = yylex();
} // Accept()

void ProgrammePascal ()
{
	Accept (PROGRAM);
	Accept (IDENTIFIANT);
	Accept (';');
	Body ();
	Accept ('.');
} // ProgrammePascal ()

void Body ()
{
	if (BEGIN == lexeme)
	{
		Bloc ();
		return;
	}
	else if (VAR == lexeme)
		DeclarationVariable ();
	else if (FUNCTION == lexeme || PROCEDURE == lexeme)
	{
		DeclarationProcFun ();
		Accept (';');
	}

	Body();
} // Body()

void DeclarationProcFun ()
{
	if (FUNCTION == lexeme)
		DeclarationFonction ();
	else if (PROCEDURE == lexeme)
		DeclarationProcedure ();

} // DeclarationProcFun ()

void DeclarationFonction ()
{
	Accept (FUNCTION);
	Accept (IDENTIFIANT);
	Accept ('(');
	ListeParametres ();
	Accept (')');
	Accept (':');
	Type ();
	Accept (';');
	Body ();

} // DeclarationFonction ()

void DeclarationProcedure ()
{
	Accept (PROCEDURE);
	Accept (IDENTIFIANT);
	Accept ('(');
	ListeParametres ();
	Accept (')');
	Accept (';');
	Body ();

} // DeclarationProcedure ()

void ListeParametres ()
{
	if (IDENTIFIANT == lexeme)
	{
		ListeID ();
		Accept (':');
		Type ();
		if (';' == lexeme)
		{
			Accept (';');
			ListeParametres ();
		}
	}

} // ListeParametres ()

void DeclarationVariable ()
{
	if (VAR == lexeme)
	{
		Accept (VAR);
		CorpsDeclVariable ();
	}
	else if (BEGIN == lexeme || PROCEDURE == lexeme || FUNCTION == lexeme)
		return;

} // DeclarationVariable ()

void CorpsDeclVariable ()
{
	ListeID ();
	Accept (':');
	Type ();
	Accept (';');
	SuiteDeclVariable ();

} // CorpsDeclVariable ()

void SuiteDeclVariable ()
{
	if (VAR == lexeme)
		DeclarationVariable ();
	else if (IDENTIFIANT == lexeme)
		CorpsDeclVariable ();

} // SuiteDeclVariale ()

void ListeID ()
{
	Accept (IDENTIFIANT);
	if (',' == lexeme)
	{
		Accept (',');
		ListeID ();
	}

} // ListeID ()

void Type ()
{
	if (ARRAY == lexeme)
	{
		Accept (ARRAY);
		Accept ('[');
		Accept (NOMBRE);
		Accept (BORNETABLEAU);
		Accept (NOMBRE);
		Accept (']');
		Accept (OF);
		if (INTEGER == lexeme || BOOL == lexeme)
			TypeSimple ();
	}
	else if (INTEGER == lexeme || BOOL == lexeme)
		TypeSimple ();

} // Type ()

void TypeSimple ()
{
	Accept (lexeme);
} // TypeSimple ()

void Instruction ()
{
	if (WHILE == lexeme)
		WhileDo ();
	else if (IF == lexeme)
		IfThenElse ();
	else if (BEGIN == lexeme)
		Bloc ();
	else if (IDENTIFIANT == lexeme)
	{
		Accept (IDENTIFIANT);
		if ('(' == lexeme)
			AppelProcedure ();
		else
			Affectation ();
	}

} // Instruction ()

void ListeInstructions ()
{

	if (END == lexeme)
		return;

	Instruction ();
	if (';' == lexeme)
	{
		Accept (';');
		ListeInstructions ();
	}

} // ListeInstructions ()

void WhileDo ()
{
	Accept (WHILE);
	Expression ();
	Accept (DO);
	Instruction ();

} // WhileDo ()

void IfThenElse ()
{
	Accept (IF);
	Expression ();
	Accept (THEN);
	Instruction ();
	if (ELSE == lexeme)
	{
		Accept (ELSE);
		Instruction ();
	}

} // IfThenElse ()

void Affectation ()
{
	Variable ();
	Accept (AFFECTATION);
	Expression ();
} // Affectation ()

void AppelProcedure ()
{
	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	Accept ('(');
	ListeArguments ();
	Accept (')');
} // AppelProcedure ()

void ListeArguments ()
{
	if ('-' == lexeme || '+' == lexeme || NOT == lexeme ||
		'(' == lexeme || TRUE == lexeme || FALSE == lexeme ||
		IDENTIFIANT == lexeme || NOMBRE == lexeme)
	{
		Expression ();
		if (',' == lexeme)
		{
			Accept (',');
			ListeArguments ();
		}
	}
	else if (')' == lexeme)
		return;

} // ListeArguments ()

void Bloc ()
{
	Accept (BEGIN);
	ListeInstructions ();
	Accept (END);
} // Bloc ()

void Signe ()
{
	if (IDENTIFIANT == lexeme || '(' == lexeme || NOMBRE == lexeme ||
		TRUE == lexeme || FALSE == lexeme)
		return;
	if ('-' == lexeme)
		Accept ('-');
	else if ('+' == lexeme)
		Accept ('+');
} // Signe ()

void Addition ()
{
	if ('-' == lexeme)
		Accept ('-');
	else if ('+' == lexeme)
		Accept ('+');

} // Addition ()

void Multiplication ()
{
	if ('*' == lexeme)
		Accept ('*');
	else if (DIV == lexeme)
		Accept (DIV);
	else if (MOD == lexeme)
		Accept (MOD);

} // Multiplication ()

void OperateurRelationnel ()
{
	if ('<' == lexeme)
		Accept ('<');
	else if ('>' == lexeme)
		Accept ('>');
	else if ('=' == lexeme)
		Accept ('=');
	else if (SUPEGAL == lexeme)
		Accept (SUPEGAL);
	else if (INFEGAL == lexeme)
		Accept (INFEGAL);
	else if (DIFFERENT == lexeme)
		Accept (DIFFERENT);

} // OperateurRelationnel ()

void Boolean ()
{
	if (TRUE == lexeme)
		Accept (TRUE);
	else if (FALSE == lexeme)
		Accept (FALSE);

} // Boolean ()

void Expression ()
{
	Conjonction ();
	if (OR == lexeme)
	{
		Accept (OR);
		Expression ();
	}
} // Expression ()

void Conjonction ()
{
	Comparaison ();
	if (AND == lexeme)
	{
		Accept (AND);
		Conjonction ();
	}
} // Conjonction ()

void Comparaison ()
{
	if (NOT == lexeme)
		Accept (NOT);
	ExpressionArithmetique ();
	SuiteComparaison ();
} // Comparaison ()

void SuiteComparaison ()
{
	if (AND == lexeme || OR == lexeme || ')' == lexeme ||
		',' == lexeme || ']' == lexeme)
		return;

	OperateurRelationnel ();
	ExpressionArithmetique ();
} // SuiteComparaison ()

void ExpressionArithmetique ()
{
	Signe ();
	Terme ();
	if ('-' == lexeme || '+' == lexeme)
	{
		Addition ();
		ExpressionArithmetique ();
	}

} // ExpressionArithmetique ()

void Terme ()
{
	Facteur ();
	if ('*' == lexeme || DIV == lexeme || MOD == lexeme)
	{
		Multiplication ();
		Terme ();
	}

} // Terme ()

void Facteur ()
{
	if ('(' == lexeme)
	{
		Accept ('(');
		Expression ();
		Accept (')');
	}
	else if (TRUE == lexeme || FALSE == lexeme)
		Boolean ();
	else if (IDENTIFIANT == lexeme)
	{
		Accept (IDENTIFIANT);
		if ('(' == lexeme)
		{
			Accept ('(');
			ListeExpressions ();
			Accept (')');
		}
		else if ('[' == lexeme)
			Variable ();
	}
	else if (NOMBRE == lexeme)
		Accept (NOMBRE);

} // Facteur ()

void ListeExpressions ()
{
	if (')' == lexeme)
		return;

	Expression ();
	if (',' == lexeme)
	{
		Accept (',');
		ListeExpressions ();
	}

} // ListeExpressions ()

void Variable ()
{
	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	if ('[' == lexeme)
	{
		Accept ('[');
		Expression ();
		Accept (']');
	}
} // Variable ()


int main (int argc, char *argv[])
{

	if (1 + 1 != argc)
	{
		printf ("You're a n00b, expected only one argument");
		exit (1);
	}

	yyin = fopen (argv[1], "r");
	lexeme = yylex ();
	ProgrammePascal ();
	fclose (yyin);
} // main()
