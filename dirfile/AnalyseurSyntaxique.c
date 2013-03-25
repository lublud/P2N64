/**
 *
 * @file: AnalyseurSyntaxique.c
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
#include "StructArbreSyntaxique.h"

#define Accept(attendu) AcceptLex(attendu, __FILE__, __LINE__)
#define Erreur(attendu) ErreurLex(attendu, __FILE__, __LINE__)

char *FilePas;
int lexeme;

void AcceptLex (const int attendu, const char *File, const int Line)
{
	if (attendu != lexeme)
	{
		printf ("Error in file %s:%d[%s:%d]\nReceived: %s\n",
				FilePas, LinePas, File, Line, yytext);
		exit (1);
	}
	printf ("%d\n", lexeme);
	lexeme = yylex();
} // Accept()

void ErreurLex (const int attendu, const char *File, const int Line)
{
		printf ("Error in file %s:%d[%s:%d]\nReceived: %s\n",
				FilePas, LinePas, File, Line, yytext);
		exit (1);

} // Erreur ()

SNoeud * ProgrammePascal ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = PROGRAM;
	Accept (PROGRAM);

	Noeud->Fils1.Nom = CreerChaine (yytext)
	Accept (IDENTIFIANT);

	Accept (';');
	Noeud->Fils2.Fils = Body ();
	Accept ('.');

	return Noeud;
} // ProgrammePascal ()

SNoeud * Body ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	for (;;)
		if (BEGIN == lexeme)
		{
			Noeud->Type = BEGIN;
			Noeud->Fils1.Fils = Bloc ();
			break;
		}
		else if (VAR == lexeme)
		{
			Noeud->Type = VAR;
			Noeud->Fils1.Fils = DeclarationVariable ();
		}
		else if (FUNCTION == lexeme || PROCEDURE == lexeme)
		{
			Noeud->Type = lexeme;
			Noeud->Fils1.Fils = DeclarationProcFun ();
			Accept (';');
		}
		else
			Erreur (lexeme);

	return Noeud;
} // Body()

SNoeud * DeclarationProcFun ()
{
	if (FUNCTION == lexeme)
	{
		return DeclarationFonction ();
	}
	else if (PROCEDURE == lexeme)
	{
		return DeclarationProcedure ();
	}
	else
		Erreur (lexeme);


} // DeclarationProcFun ()

SNoeud * DeclarationFonction ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Accept (FUNCTION);
	Noeud->Type = FUNCTION;

	Noeud->Fils1.Nom = CreerChaine (yytext);
	Accept (IDENTIFIANT);

	Accept ('(');
	Noeud->Fils2.Fils = ListeParametres ();

	Accept (')');
	Accept (':');
	Noeud->Fils3.Fils = Type ();

	Accept (';');
	Noeud->Fils4.Fils = Body ();

	return Noeud;

} // DeclarationFonction ()

SNoeud * DeclarationProcedure ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = PROCEDURE;
	Accept (PROCEDURE);

	Noeud->Fils1.Nom = yytext;
	Accept (IDENTIFIANT);
	Accept ('(');
	Noeud->Fils2.Fils = ListeParametres ();
	Accept (')');
	Accept (';');
	Noeud->Fils3.Fils = Body ();

	return Noeud;

} // DeclarationProcedure ()

SNoeud * ListeParametres ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (IDENTIFIANT == lexeme)
	{
		Noeud = ListeID ();
		Accept (':');
		Noeud->Fils3.Fils = Type ();
		Noeud->Type = Noeud->Fils3.Fils->Type;
		if (';' == lexeme)
		{
			Accept (';');
			Noeud->Fils4.Frere = ListeParametres ();
		}
		else
			break;
	}
	else
		Erreur (lexeme);

	return Noeud;

} // ListeParametres ()

SNoeud * DeclarationVariable ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (VAR == lexeme)
	{
		Noeud->Type = VAR;
		Accept (VAR);
		Noeud->Fils1.Fils = CorpsDeclVariable ();
	}
	else if (BEGIN == lexeme || PROCEDURE == lexeme || FUNCTION == lexeme)
		return Noeud;
	else
		Erreur (lexeme);

	return Noeud;

} // DeclarationVariable ()

SNoeud * CorpsDeclVariable ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (IDENTIFIANT == lexeme)
	{
		Noeud = ListeID ();
		Accept (':');
		Noeud->Fils3.Fils = Type ();
		Noeud->Type = Noeud->Fils3.Fils->Type;
		Accept (';');
		Noeud->Fils4.Frere = SuiteDeclVariable ();
	}
	else
		Erreur (lexeme);

	return Noeud;

} // CorpsDeclVariable ()

SNoeud * SuiteDeclVariable ()
{
	if (VAR == lexeme || FUNCTION == lexeme || PROCEDURE == lexeme ||
		BEGIN == lexeme)
		return DeclarationVariable ();
	else if (IDENTIFIANT == lexeme)
		return CorpsDeclVariable ();
	}
	else
		Erreur (lexeme);

} // SuiteDeclVariale ()

SNoeud * ListeID ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

		Noeud->Fils1.Nom = CreerChaine(yytext);
		Accept (IDENTIFIANT);
		if (',' == lexeme)
		{
			Noeud->Fils2.Frere = ListeID ();
			Accept (',');
		}

	return Noeud;

} // ListeID ()

SNoeud * Type ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (ARRAY == lexeme)
	{
		Noeud->Type = ARRAY;
		Accept (ARRAY);
		Accept ('[');
		Noeud->Fils1.Nombre = atoi (yytext);
		Accept (NOMBRE);
		Accept (BORNETABLEAU);
		Noeud->Fils2.Nombre = atoi (yytext);
		Accept (NOMBRE);
		Accept (']');
		Accept (OF);

		if (INTEGER == lexeme || BOOL == lexeme)
		{
			Noeud->Fils3.Nombre = lexeme;
			Accept (lexeme);
		}
		else
			Erreur (lexeme);
	}
	else if (INTEGER == lexeme || BOOL == lexeme)
	{
		Noeud->Type = lexeme;
		Accept (lexeme);
	}
	else
		Erreur (lexeme);

} // Type ()

/* * /
SNoeud * TypeSimple ()
{
	if (INTEGER == lexeme || BOOL == lexeme)
		Accept (lexeme);
	else
		Erreur (lexeme);
} // TypeSimple ()
/ *  */

SNoeud * Instruction ()
{
	if (WHILE == lexeme)
		return WhileDo ();
	else if (IF == lexeme)
		return IfThenElse ();
	else if (BEGIN == lexeme)
		return Bloc ();
	else if (IDENTIFIANT == lexeme)
	{
		SNoeud *Noeud;
		Noeud = CreerNoeud ();
		char *tmp = CreerChaine (yytext);

		Accept (IDENTIFIANT);
		if ('(' == lexeme)
			Noeud = AppelProcedure ();
		else
			Noeud = Affectation ();
		Noeud->Fils1.Nom = tmp;
		return Noeud;
	}
	else
		Erreur(lexeme);

} // Instruction ()

SNoeud * ListeInstructions ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (END == lexeme)
		return Noeud;

	Noeud->Fils1.Fils = Instruction ();
	if (';' == lexeme)
	{
		Accept (';');
		Noeud->Fils2.Frere = ListeInstructions ();
	}

	return Noeud;

} // ListeInstructions ()

SNoeud * WhileDo ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = WHILE;
	Accept (WHILE);
	Noeud->Fils1.Fils = Expression ();
	Accept (DO);
	Noeud->Fils2.Fils = Instruction ();

	return Noeud;

} // WhileDo ()

SNoeud * IfThenElse ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = IF;
	Accept (IF);
	Noeud->Fils1.Fils = Expression ();
	Accept (THEN);
	Noeud->Fils2.Fils = Instruction ();
	if (ELSE == lexeme)
	{
		Accept (ELSE);
		Noeud->Fils3.Fils = Instruction ();
	}

	return Noeud;

} // IfThenElse ()

SNoeud * Affectation ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = AFFECTATION;
	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	Noeud->Fils2.Fils = Variable ();
	Accept (AFFECTATION);
	Noeud->Fils3.Fils = Expression ();

	return Noeud;

} // Affectation ()

SNoeud * AppelProcedure ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = IDENTIFIANT;
	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	Accept ('(');
	Noeud->Fils2.Fils = ListeArguments ();
	Accept (')');

	return Noeud;

} // AppelProcedure ()

SNoeud * ListeArguments ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	for (;;)
	{
		if ('-' == lexeme || '+' == lexeme || NOT == lexeme ||
				'(' == lexeme || TRUE == lexeme || FALSE == lexeme ||
				IDENTIFIANT == lexeme || NOMBRE == lexeme)
		{
			Noeud = Expression ();
			if (',' == lexeme)
				Accept (',');
			else
				break;
		}
		else if (')' == lexeme)
			return Noeud;
		else 
			Erreur (lexeme);
	}

	return Noeud;

} // ListeArguments ()

SNoeud * Bloc ()
{
	Accept (BEGIN);
	ListeInstructions ();
	Accept (END);
} // Bloc ()

SNoeud * Signe ()
{
	if (IDENTIFIANT == lexeme || '(' == lexeme || NOMBRE == lexeme ||
		TRUE == lexeme || FALSE == lexeme)
		return;
	if ('-' == lexeme)
		Accept ('-');
	else if ('+' == lexeme)
		Accept ('+');
	else
		Erreur (lexeme);

} // Signe ()

SNoeud * Addition ()
{
	if ('-' == lexeme)
		Accept ('-');
	else if ('+' == lexeme)
		Accept ('+');
	else
		Erreur (lexeme);

} // Addition ()

SNoeud * Multiplication ()
{
	if ('*' == lexeme)
		Accept ('*');
	else if (DIV == lexeme)
		Accept (DIV);
	else if (MOD == lexeme)
		Accept (MOD);
	else
		Erreur (lexeme);

} // Multiplication ()

SNoeud * OperateurRelationnel ()
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
	else
		Erreur (lexeme);

} // OperateurRelationnel ()

SNoeud * Boolean ()
{
	if (TRUE == lexeme)
		Accept (TRUE);
	else if (FALSE == lexeme)
		Accept (FALSE);
	else
		Erreur (lexeme);

} // Boolean ()

SNoeud * Expression ()
{
	Conjonction ();
	if (OR == lexeme)
	{
		Accept (OR);
		Expression ();
	}
} // Expression ()

SNoeud * Conjonction ()
{
	Comparaison ();
	if (AND == lexeme)
	{
		Accept (AND);
		Conjonction ();
	}
} // Conjonction ()

SNoeud * Comparaison ()
{
	if (NOT == lexeme)
		Accept (NOT);
	ExpressionArithmetique ();
	SuiteComparaison ();
} // Comparaison ()

SNoeud * SuiteComparaison ()
{
	if (AND == lexeme || OR == lexeme  || ')' == lexeme ||
		',' == lexeme || ']' == lexeme || ';' == lexeme ||
		THEN == lexeme|| DO == lexeme  || END == lexeme)
		return;

	OperateurRelationnel ();
	ExpressionArithmetique ();
} // SuiteComparaison ()

SNoeud * ExpressionArithmetique ()
{
	Signe ();
	Terme ();
	if ('-' == lexeme || '+' == lexeme)
	{
		Addition ();
		ExpressionArithmetique ();
	}

} // ExpressionArithmetique ()

SNoeud * Terme ()
{
	Facteur ();
	if ('*' == lexeme || DIV == lexeme || MOD == lexeme)
	{
		Multiplication ();
		Terme ();
	}

} // Terme ()

SNoeud * Facteur ()
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
	else
		Erreur (lexeme);

} // Facteur ()

SNoeud * ListeExpressions ()
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

SNoeud * Variable ()
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
	FilePas = argv[1];

	SNoeud *Racine = CreerNoeud ();

	yyin = fopen (argv[1], "r");
	lexeme = yylex ();
	Racine = ProgrammePascal ();
	fclose (yyin);
} // main()
