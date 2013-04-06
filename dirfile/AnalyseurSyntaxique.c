/**
 *
 * @file: AnalyseurSyntaxique.c
 *
 * @author: lublud
 *
 * @date: 21/2/13
 *
 * @brief Vérifie que le programme en pascal est correct syntaxiquement
 *
**/

#define Accept(attendu) AcceptLex(attendu, __FILE__, __LINE__)
#define Erreur(attendu) ErreurLex(attendu, __FILE__, __LINE__)

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

	Accept (PROGRAM);

	Noeud->Fils1.Nom = CreerChaine (yytext);
	Noeud->TypeF1 = TYPE_NOM;
	Accept (IDENTIFIANT);

	Accept (';');
	Noeud->Fils2.Fils = Body ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;
	Accept ('.');

	Noeud->Type = PRGM;
	return Noeud;
} // ProgrammePascal ()

SNoeud *Body ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	// Recursivité
	if (BEGIN == lexeme)
		return Bloc ();
	else if (VAR == lexeme)
	{
		Noeud->Fils1.Fils = DeclarationVariable ();
		Noeud->TypeF1 = TYPE_SNOEUDFILS;
	}
	else if (FUNCTION == lexeme || PROCEDURE == lexeme)
	{
		Noeud->Fils1.Fils = DeclarationProcFun ();
		Noeud->TypeF1 = TYPE_SNOEUDFILS;
		Accept (';');
	}
	else
		Erreur (lexeme);

	Noeud->Fils2.Frere = Body ();
	Noeud->TypeF2 = TYPE_SNOEUDFRERE;

	Noeud->Type = BODY;
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

	Noeud->Fils1.Nom = CreerChaine (yytext);
	Noeud->TypeF1 = TYPE_NOM;
	Accept (IDENTIFIANT);

	Accept ('(');
	Noeud->Fils2.Fils = ListeParametres ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;

	Accept (')');
	Accept (':');

	if (ARRAY == lexeme)
	{
		Noeud->Fils3.Fils = Type ();
		Noeud->TypeF3 = TYPE_SNOEUDFILS;
	}
	else
	{
		SNoeud *tmp;
		tmp = CreerNoeud ();
		tmp = Type ();
		Noeud->Fils3.Nombre = tmp->Fils1.Nombre;
		Noeud->TypeF3 = TYPE_INT;
	}

	Accept (';');
	Noeud->Fils4.Fils = Body ();
	Noeud->TypeF4 = TYPE_SNOEUDFILS;

	Noeud->Type = DECLFUNC;
	return Noeud;

} // DeclarationFonction ()

SNoeud *DeclarationProcedure ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Accept (PROCEDURE);

	Noeud->Fils1.Nom = yytext;
	Noeud->TypeF1 = TYPE_NOM;
	Accept (IDENTIFIANT);
	Accept ('(');
	Noeud->Fils2.Fils = ListeParametres ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;
	Accept (')');
	Accept (';');
	Noeud->Fils3.Fils = Body ();
	Noeud->TypeF3 = TYPE_SNOEUDFILS;

	Noeud->Type = DECLPROC;
	return Noeud;

} // DeclarationProcedure ()

SNoeud *ListeParametres ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (IDENTIFIANT == lexeme)
	{
		Noeud = ListeID (PARAMETRE);
		Accept (':');

		if (ARRAY == lexeme)
		{
			Noeud->Fils3.Fils = Type ();
			Noeud->TypeF3 = TYPE_SNOEUDFILS;
		}
		else
		{
			SNoeud *tmp;
			tmp = CreerNoeud ();
			tmp = Type ();
			Noeud->Fils3.Nombre = tmp->Fils1.Nombre;
			Noeud->TypeF3 = TYPE_INT;
		}
		if (';' == lexeme)
		{
			Accept (';');
			Noeud->Fils4.Frere = ListeParametres ();
			Noeud->TypeF4 = TYPE_SNOEUDFRERE;
		}
	}
	else
		Erreur (lexeme);

	Noeud->Type = PARAMETRE;
	return Noeud;

} // ListeParametres ()

SNoeud *DeclarationVariable ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (VAR == lexeme)
	{
		Accept (VAR);
		Noeud->Fils1.Fils = CorpsDeclVariable ();
		Noeud->TypeF1 = TYPE_SNOEUDFILS;
	}
	else if (BEGIN == lexeme || PROCEDURE == lexeme || FUNCTION == lexeme)
	{
		Noeud->Type = DECLVAR;
		return Noeud;
	}
	else
		Erreur (lexeme);

	Noeud->Type = DECLVAR;
	return Noeud;

} // DeclarationVariable ()

SNoeud *CorpsDeclVariable ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (IDENTIFIANT == lexeme)
	{
		Noeud = ListeID (ID);
		Accept (':');

		if (ARRAY == lexeme)
		{
			Noeud->Fils3.Fils = Type ();
			Noeud->TypeF3 = TYPE_SNOEUDFILS;
		}
		else
		{
			SNoeud *tmp;
			tmp = CreerNoeud ();
			tmp = Type ();
			Noeud->Fils3.Nombre = tmp->Fils1.Nombre;
			Noeud->TypeF3 = TYPE_INT;
		}
		Accept (';');
		Noeud->Fils4.Fils = SuiteDeclVariable ();
		Noeud->TypeF4 = TYPE_SNOEUDFILS;
	}
	else
		Erreur (lexeme);

	Noeud->Type = ID;
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

SNoeud *ListeID (int TypeAppelant)
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Nom = CreerChaine(yytext);
	Noeud->TypeF1 = TYPE_NOM;
	Accept (IDENTIFIANT);
	if (',' == lexeme)
	{
		Accept (',');
		Noeud->Fils2.Frere = ListeID (TypeAppelant);
		Noeud->Fils2.Frere->Type = TypeAppelant;
		Noeud->TypeF2 = TYPE_SNOEUDFRERE;
	}

	return Noeud;

} // ListeID ()

SNoeud *Type ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (ARRAY == lexeme)
	{
		Accept (ARRAY);
		Accept ('[');
		Noeud->Fils1.Nombre = atoi (yytext);
		Noeud->TypeF1 = TYPE_INT;
		Accept (NOMBRE);
		Accept (BORNETABLEAU);
		Noeud->Fils2.Nombre = atoi (yytext);
		Noeud->TypeF2 = TYPE_INT;
		Accept (NOMBRE);
		Accept (']');
		Accept (OF);

		if (INTEGER == lexeme || BOOL == lexeme)
		{
			Noeud->Fils3.Nombre = lexeme;
			Noeud->TypeF3 = TYPE_INT;
			Accept (lexeme);
		}
		else
			Erreur (lexeme);
	}
	else if (INTEGER == lexeme || BOOL == lexeme)
	{
		Noeud->Fils1.Nombre = lexeme;
		Noeud->TypeF1 = TYPE_INT;
		Accept (lexeme);
	}
	else
		Erreur (lexeme);

	return Noeud;

} // Type ()

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
		Noeud->TypeF1 = TYPE_NOM;
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
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	if (';' == lexeme)
	{
		Accept (';');
		if (END == lexeme)
		{
			Noeud->Type = INSTRUCTION;
			return Noeud;
		}
		Noeud->Fils2.Frere = ListeInstructions ();
		Noeud->TypeF2 = TYPE_SNOEUDFRERE;
	}

	Noeud->Type = INSTRUCTION;
	return Noeud;

} // ListeInstructions ()

SNoeud *WhileDo ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Accept (WHILE);
	Noeud->Fils1.Fils = Expression ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	Accept (DO);
	Noeud->Fils2.Fils = Instruction ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;

	Noeud->Type = WHILEDO;
	return Noeud;

} // WhileDo ()

SNoeud *IfThenElse ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Accept (IF);
	Noeud->Fils1.Fils = Expression ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	Accept (THEN);
	Noeud->Fils2.Fils = Instruction ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;
	if (ELSE == lexeme)
	{
		Accept (ELSE);
		Noeud->Fils3.Fils = Instruction ();
		Noeud->TypeF3 = TYPE_SNOEUDFILS;
	}

	Noeud->Type = IFTHENELSE;
	return Noeud;

} // IfThenElse ()

SNoeud *Affectation ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	Noeud->Fils2.Fils = Variable ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;
	Accept (AFFECTATION);
	Noeud->Fils3.Fils = Expression ();
	Noeud->TypeF3 = TYPE_SNOEUDFILS;

	Noeud->Type = AFF;
	return Noeud;

} // Affectation ()

SNoeud *AppelProcedure ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	// Je ne met pas le IDENTIFIANT que j'ai déjà lu dans Instruction ()
	Accept ('(');
	Noeud->Fils2.Fils = ListeArguments ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;
	Accept (')');

	Noeud->Type = APPELPROC;
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
		Noeud->TypeF1 = TYPE_SNOEUDFILS;
		if (',' == lexeme)
		{
			Accept (',');
			Noeud->Fils2.Frere = ListeArguments ();
			Noeud->TypeF2 = TYPE_SNOEUDFRERE;
		}
	}
	else if (')' == lexeme)
		return Noeud;
	else
		Erreur (lexeme);

	Noeud->Type = ARGUMENTS;
	return Noeud;

} // ListeArguments ()

SNoeud *Bloc ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Accept (BEGIN);
	Noeud->Fils1.Fils = ListeInstructions ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	Accept (END);

	Noeud->Type = BLOC;
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
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	if (OR == lexeme)
	{
		Accept (OR);
		Noeud->Fils2.Frere = Expression ();
		Noeud->TypeF2 = TYPE_SNOEUDFRERE;
	}

	Noeud->Type = EXPRESSION;
	return Noeud;

} // Expression ()

SNoeud *Conjonction ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Fils = Comparaison ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	if (AND == lexeme)
	{
		Accept (AND);
		Noeud->Fils2.Frere = Conjonction ();
		Noeud->TypeF2 = TYPE_SNOEUDFRERE;
	}

	Noeud->Type = CONJONCTION;
	return Noeud;

} // Conjonction ()

SNoeud *Comparaison ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (NOT == lexeme)
		Accept (NOT);

	Noeud->Fils1.Fils = ExpressionArithmetique ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	Noeud->Fils2.Fils = SuiteComparaison ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;

	Noeud->Type = COMPARAISON;
	return Noeud;

} // Comparaison ()

SNoeud *SuiteComparaison ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (AND  != lexeme && OR  != lexeme  && ')' != lexeme  &&
		','  != lexeme && ']' != lexeme  && ';'  != lexeme &&
		THEN != lexeme && DO  != lexeme  && END != lexeme)
	{
		Noeud->Fils1.Nombre = OperateurRelationnel ();
		Noeud->TypeF1 = TYPE_INT;
		Noeud->Fils2.Fils = ExpressionArithmetique ();
		Noeud->TypeF2 = TYPE_SNOEUDFILS;
	}

	Noeud->Type = SUITECOMPARAISON;
	return Noeud;

} // SuiteComparaison ()

SNoeud *ExpressionArithmetique ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Nombre = Signe ();
	Noeud->TypeF1 = TYPE_INT;
	Noeud->Fils2.Fils = Terme ();
	Noeud->TypeF2 = TYPE_SNOEUDFILS;
	if ('-' == lexeme || '+' == lexeme)
	{
		Noeud->Fils3.Nombre = Addition ();
		Noeud->TypeF3 = TYPE_INT;
		Noeud->Fils4.Frere = ExpressionArithmetique ();
		Noeud->TypeF4 = TYPE_SNOEUDFRERE;
	}

	Noeud->Type = EXPRARITH;
	return Noeud;

} // ExpressionArithmetique ()

SNoeud *Terme ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	Noeud->Fils1.Fils = Facteur ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	if ('*' == lexeme || DIV == lexeme || MOD == lexeme)
	{
		Noeud->Fils2.Nombre = Multiplication ();
		Noeud->TypeF2 = TYPE_INT;
		Noeud->Fils3.Frere = Terme ();
		Noeud->TypeF3 = TYPE_SNOEUDFRERE;
	}

	Noeud->Type = TERME;
	return Noeud;

} // Terme ()

SNoeud *Facteur ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if ('(' == lexeme)
	{
		Accept ('(');
		Noeud->Fils1.Fils = Expression ();
		Noeud->TypeF1 = TYPE_SNOEUDFILS;
		Accept (')');
	}
	else if (TRUE == lexeme || FALSE == lexeme)
	{
		Noeud->Fils1.Nombre = Boolean ();
		Noeud->TypeF1 = TYPE_INT;
	}
	else if (IDENTIFIANT == lexeme)
	{
		Noeud->Fils1.Nom = CreerChaine (yytext);
		Noeud->TypeF1 = TYPE_NOM;
		Accept (IDENTIFIANT);
		if ('(' == lexeme)
		{
			Accept ('(');
			Noeud->Fils2.Fils = ListeExpressions ();
			Noeud->TypeF2 = TYPE_SNOEUDFILS;
			Accept (')');
		}
		else if ('[' == lexeme)
		{
			Noeud->Fils2.Fils = Variable ();
			Noeud->TypeF2 = TYPE_SNOEUDFILS;
		}
	}
	else if (NOMBRE == lexeme)
	{
		Noeud->Fils1.Nombre = atoi (yytext);
		Noeud->TypeF1 = TYPE_INT;
		Accept (NOMBRE);
	}

	Noeud->Type = FACTEUR;
	return Noeud;

} // Facteur ()

SNoeud *ListeExpressions ()
{
	SNoeud *Noeud;
	Noeud = CreerNoeud ();

	if (')' == lexeme)
		return Noeud;

	Noeud->Fils1.Fils = Expression ();
	Noeud->TypeF1 = TYPE_SNOEUDFILS;
	if (',' == lexeme)
	{
		Accept (',');
		Noeud->Fils2.Frere = ListeExpressions ();
		Noeud->TypeF2 = TYPE_SNOEUDFRERE;
	}

	Noeud->Type = LISTEEXPR;
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
		Noeud->Fils1.Fils = Expression ();
		Noeud->TypeF1 = TYPE_SNOEUDFILS;
		Accept (']');
	}

	Noeud->Type = VARIABLE;
	return Noeud;

} // Variable ()
