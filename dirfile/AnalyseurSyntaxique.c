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

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
//
//#include "AnalyseurLex.c"
//#include "DeclarationFonctionAnalyseurSyntaxique.c"
//#include "StructArbreSyntaxique.h"
#include "include.h"

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


SNoeud *ProgrammePascal ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = PROGRAM;
	Accept (PROGRAM);

	Noeud->Fils1.Nom = CreerChaine (yytext);
	Accept (IDENTIFIANT);

	Accept (';');
	Noeud->Fils2.Fils = Body ();
	Accept ('.');

	return Noeud;
} // ProgrammePascal ()

SNoeud *Body ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	// Recursivité
	if (BEGIN == lexeme)
	{
		Noeud->Type = BEGIN;
		Noeud->Fils1.Fils = Bloc ();
		return Noeud;
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

	Noeud->Fils2.Frere = Body ();

	return Noeud;

} // Body()

SNoeud *DeclarationProcFun ()
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

SNoeud *DeclarationFonction ()
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

SNoeud *DeclarationProcedure ()
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

SNoeud *ListeParametres ()
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
	}
	else
		Erreur (lexeme);

	return Noeud;

} // ListeParametres ()

SNoeud *DeclarationVariable ()
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

SNoeud *CorpsDeclVariable ()
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
		Noeud->Fils4.Fils = SuiteDeclVariable ();
	}
	else
		Erreur (lexeme);

	return Noeud;

} // CorpsDeclVariable ()

SNoeud *SuiteDeclVariable ()
{
	if (VAR == lexeme || FUNCTION == lexeme || PROCEDURE == lexeme ||
		BEGIN == lexeme)
		return DeclarationVariable ();
	else if (IDENTIFIANT == lexeme)
		return CorpsDeclVariable ();
	else
		Erreur (lexeme);

} // SuiteDeclVariale ()

SNoeud *ListeID ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Nom = CreerChaine(yytext);
	Accept (IDENTIFIANT);
	if (',' == lexeme)
	{
		Accept (',');
		Noeud->Fils2.Frere = ListeID ();
	}

	return Noeud;

} // ListeID ()

SNoeud *Type ()
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

	return Noeud;

} // Type ()

/* * /
SNoeud *TypeSimple ()
{
	if (INTEGER == lexeme || BOOL == lexeme)
		Accept (lexeme);
	else
		Erreur (lexeme);
} // TypeSimple ()
/ *  */

SNoeud *Instruction ()
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

SNoeud *ListeInstructions ()
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

SNoeud *WhileDo ()
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

SNoeud *IfThenElse ()
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

SNoeud *Affectation ()
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

SNoeud *AppelProcedure ()
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

SNoeud *ListeArguments ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	// Recursivité
	if ('-' == lexeme || '+' == lexeme || NOT == lexeme ||
			'(' == lexeme || TRUE == lexeme || FALSE == lexeme ||
			IDENTIFIANT == lexeme || NOMBRE == lexeme)
	{
		Noeud->Fils1.Fils = Expression ();
		if (',' == lexeme)
		{
			Accept (',');
			Noeud->Fils2.Frere = ListeArguments ();
		}
	}
	else if (')' == lexeme)
		return Noeud;
	else
		Erreur (lexeme);

	return Noeud;

} // ListeArguments ()

SNoeud *Bloc ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Accept (BEGIN);
	ListeInstructions ();
	Accept (END);

	return Noeud;

} // Bloc ()

unsigned Signe ()
{
	unsigned tmp = lexeme;

	if (IDENTIFIANT == lexeme || '(' == lexeme || NOMBRE == lexeme ||
		TRUE == lexeme || FALSE == lexeme)
		return;
	if ('-' == lexeme)
		Accept ('-');
	else if ('+' == lexeme)
		Accept ('+');

	return tmp;

} // Signe ()

unsigned Addition ()
{
	unsigned tmp = lexeme;

	if ('-' == lexeme)
		Accept ('-');
	else if ('+' == lexeme)
		Accept ('+');
	else
		Erreur (lexeme);

	return tmp;

} // Addition ()

unsigned Multiplication ()
{
	unsigned tmp = lexeme;

	if ('*' == lexeme)
		Accept ('*');
	else if (DIV == lexeme)
		Accept (DIV);
	else if (MOD == lexeme)
		Accept (MOD);
	else
		Erreur (lexeme);

	return tmp;

} // Multiplication ()

unsigned OperateurRelationnel ()
{
	unsigned tmp = lexeme;

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

	return tmp;

} // OperateurRelationnel ()

unsigned Boolean ()
{
	unsigned tmp = lexeme;

	if (TRUE == lexeme)
		Accept (TRUE);
	else if (FALSE == lexeme)
		Accept (FALSE);
	else
		Erreur (lexeme);

	return  tmp;

} // Boolean ()

SNoeud *Expression ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Fils = Conjonction ();
	if (OR == lexeme)
	{
		Noeud->Type = OR;
		Accept (OR);
		Noeud->Fils2.Frere = Expression ();
	}

	return Noeud;

} // Expression ()

SNoeud *Conjonction ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Fils = Comparaison ();
	if (AND == lexeme)
	{
		Noeud->Type = AND;
		Accept (AND);
		Noeud->Fils2.Frere = Conjonction ();
	}

	return Noeud;

} // Conjonction ()

SNoeud *Comparaison ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (NOT == lexeme)
	{
		Noeud->Type = NOT;
		Accept (NOT);
	}

	Noeud->Fils1.Fils = ExpressionArithmetique ();
	Noeud->Fils2.Fils = SuiteComparaison ();

	return Noeud;

} // Comparaison ()

SNoeud *SuiteComparaison ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (AND  != lexeme || OR  != lexeme  || ')' != lexeme ||
		','  != lexeme || ']' != lexeme || ';'  != lexeme ||
		THEN != lexeme || DO  != lexeme  || END != lexeme)
	{
		Noeud->Fils1.Fils = OperateurRelationnel ();
		Noeud->Fils2.Fils = ExpressionArithmetique ();
	}

	return Noeud;

} // SuiteComparaison ()

SNoeud *ExpressionArithmetique ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Type = Signe ();
	Noeud->Fils1.Fils = Terme ();
	if ('-' == lexeme || '+' == lexeme)
	{
		Noeud->Fils2.Fils = Addition ();
		Noeud->Fils3.Frere = ExpressionArithmetique ();
	}

	return Noeud;

} // ExpressionArithmetique ()

SNoeud *Terme ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Fils = Facteur ();
	if ('*' == lexeme || DIV == lexeme || MOD == lexeme)
	{
		Noeud->Fils2.Fils = Multiplication ();
		Noeud->Fils3.Frere = Terme ();
	}

	return Noeud;

} // Terme ()

SNoeud *Facteur ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if ('(' == lexeme)
	{
		Accept ('(');
		return Expression ();
		Accept (')');
	}
	else if (TRUE == lexeme || FALSE == lexeme)
	{
		Noeud->Fils1.Nombre = lexeme;
		Noeud->Type = Boolean ();
	}
	else if (IDENTIFIANT == lexeme)
	{
		Noeud->Fils1.Nom = CreerChaine (yytext);
		Noeud->Type = IDENTIFIANT;
		Accept (IDENTIFIANT);
		if ('(' == lexeme)
		{
			Accept ('(');
			Noeud->Fils2.Fils = ListeExpressions ();
			Accept (')');
		}
		else if ('[' == lexeme)
			Noeud->Fils2.Fils = Variable ();
	}
	else if (NOMBRE == lexeme)
	{
		Noeud->Type = NOMBRE;
		Noeud->Fils1.Nombre = atoi (yytext);
		Accept (NOMBRE);
	}

	return Noeud;

} // Facteur ()

SNoeud *ListeExpressions ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (')' == lexeme)
		return Noeud;

	Noeud->Fils1.Fils = Expression ();
	if (',' == lexeme)
	{
		Accept (',');
		Noeud->Fils1.Frere = ListeExpressions ();
	}

	return Noeud;

} // ListeExpressions ()

SNoeud *Variable ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	if ('[' == lexeme)
	{
		Accept ('[');
		Noeud->Fils2.Fils = Expression ();
		Accept (']');
	}

	return Noeud;

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
