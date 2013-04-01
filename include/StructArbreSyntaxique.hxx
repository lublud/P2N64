/**
 *
 * @file: StructArbreSyntaxique.hxx
 *
 * @author: lublud
 *
 * @date: 14/3/2013
 *
 * @version: V1.0
 *
 * @synopsis: Coprs des fonctions pour la création/génération de l'arbre syntaxique
 *
 **/

#if ! defined __STRUCTARBRESYNTAXIQUE_HXX__
#define 	  __STRUCTARBRESYNTAXIQUE_HXX__

#include "StructArbreSyntaxique.h"

int TabType[2048];
int TabDiffSuivant[2048];
int DiffSuivant, iterator;
DiffSuivant = 0;
iterator = 0;

SNoeud *CreerNoeud ()
{
	SNoeud *p;
	p = (SNoeud *) malloc (sizeof (SNoeud));

	if (NULL == p)
	{
		sprintf (stderr, "Error: malloc ()\n");
		exit (1);
	}

	return p;
} // CreerNoeud ()

char *CreerChaine (char *chaine)
{
	char *c;
	c = (char *) malloc (strlen (chaine) + 1);

	if (NULL == c)
	{
		sprintf (stderr, "Error malloc\n");
		exit (EXIT_FAILURE);
	}

	c = strcpy (c, chaine);
	return c;
} // CreerChaine ()

void CreationFichierDot (char *pathname, SNoeud *Racine)
{
	char buf[128];
	int fd;

	sprintf (buf, "digraph ArbreSyntaxique\n{\n");

	fd = open (pathname, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd < 0)
	{
		perror ("open ()");
		exit (1);
	}

	if (write (fd, buf, strlen (buf)) < 0)
	{
		perror ("write ()");
		exit (1);
	}
	close (fd);

	CreationArbreDot (pathname, Racine, "PROGRAMME");

	fd = open (pathname, O_RDWR | O_APPEND);
	if (fd < 0)
	{
		perror ("open ()");
		exit (1);
	}

	unsigned i;
	char *LabelDot = (char *) malloc (256);
	char *IDLabel = (char *) malloc (256);
	for (i = 0; NULL != TabType[i]; ++i)
	{
		if (1000 == TabType[i])
			sprintf (IDLabel, "PRGM");
		else if (1001 == TabType[i])
			sprintf (IDLabel, "BODY");
		else if (1002 == TabType[i])
			sprintf (IDLabel, "DECLVAR");
		else if (1003 == TabType[i])
			sprintf (IDLabel, "DECLFUNC");
		else if (1004 == TabType[i])
			sprintf (IDLabel, "DECALPROC");
		else if (1005 == TabType[i])
			sprintf (IDLabel, "PARAMETRE");
		else if (1006 == TabType[i])
			sprintf (IDLabel, "INSTRUCTION");
		else if (1007 == TabType[i])
			sprintf (IDLabel, "WHILEDO");
		else if (1008 == TabType[i])
			sprintf (IDLabel, "IFTHENELSE");
		else if (1009 == TabType[i])
			sprintf (IDLabel, "AFF");
		else if (1010 == TabType[i])
			sprintf (IDLabel, "ID");
		else if (1011 == TabType[i])
			sprintf (IDLabel, "APPELPROC");
		else if (1012 == TabType[i])
			sprintf (IDLabel, "ARGUMENTS");
		else if (1013 == TabType[i])
			sprintf (IDLabel, "BLOC");
		else if (1014 == TabType[i])
			sprintf (IDLabel, "EXPRESSION");
		else if (1015 == TabType[i])
			sprintf (IDLabel, "CONJONCTION");
		else if (1016 == TabType[i])
			sprintf (IDLabel, "COMPARAISON");
		else if (1017 == TabType[i])
			sprintf (IDLabel, "SUITECOMPARAISON");
		else if (1018 == TabType[i])
			sprintf (IDLabel, "EXPRARITH");
		else if (1019 == TabType[i])
			sprintf (IDLabel, "TERME");
		else if (1020 == TabType[i])
			sprintf (IDLabel, "FACTEUR");
		else if (1021 == TabType[i])
			sprintf (IDLabel, "LISTEEXPR");
		else if (1022 == TabType[i])
			sprintf (IDLabel, "VARIABLE");
		else if (257 == TabType[i])
			sprintf (IDLabel, "IDENTIFIANT");
		else if (258 == TabType[i])
			sprintf (IDLabel, "NOMBRE");
		else if (259 == TabType[i])
			sprintf (IDLabel, "<>");
		else if (260 == TabType[i])
			sprintf (IDLabel, "<=");
		else if (261 == TabType[i])
			sprintf (IDLabel, ">=");
		else if (262 == TabType[i])
			sprintf (IDLabel, ":=");
		else if (263 == TabType[i])
			sprintf (IDLabel, "..");
		else if (264 == TabType[i])
			sprintf (IDLabel, "DIV");
		else if (265 == TabType[i])
			sprintf (IDLabel, "MOD");
		else if (266 == TabType[i])
			sprintf (IDLabel, "OR");
		else if (267 == TabType[i])
			sprintf (IDLabel, "AND");
		else if (268 == TabType[i])
			sprintf (IDLabel, "NOT");
		else if (269 == TabType[i])
			sprintf (IDLabel, "VAR");
		else if (270 == TabType[i])
			sprintf (IDLabel, "INTEGER");
		else if (271 == TabType[i])
			sprintf (IDLabel, "BOOL");
		else if (272 == TabType[i])
			sprintf (IDLabel, "TRUE");
		else if (273 == TabType[i])
			sprintf (IDLabel, "FALSE");
		else
			sprintf (IDLabel, "%c", TabType[i]);

		sprintf (LabelDot, "\"%d_%d\" [label=\"%s\"];\n", TabType[i], TabDiffSuivant[i], IDLabel);
		if (write (fd, LabelDot, strlen (LabelDot)) < 0)
		{
			perror ("write ()");
			exit (1);
		}

	}

	free (IDLabel);
	free (LabelDot);

	sprintf (buf, "}");
	if (write (fd, buf, strlen (buf)) < 0)
	{
		perror ("write ()");
		exit (1);
	}

	close (fd);


} // CreationfichierDot ()


void CreationArbreDot (char *pathname, SNoeud *Racine, char *Courant)
{
	char * Suivant = (char *)malloc (256);
	char *Precedent = (char *) malloc (256);
	char *tmp = (char *) malloc (256);
	char buf[128];
	int fd;

	fd = open (pathname, O_WRONLY | O_APPEND);
	if (fd < 0)
	{
		perror ("open ()");
		exit (1);
	}

	if (PRGM == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit (1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // PRGM
	else if (BODY == Racine->Type)
	{
		strcpy (Precedent, Courant);
		if (NULL != Racine->Fils1.Fils)
		{
			if (isalpha (Racine->Fils1.Fils->Fils1.Nom[0]))
				sprintf (Suivant, "%s_%d", Racine->Fils1.Fils->Fils1.Nom, ++DiffSuivant);
			else
				sprintf (Suivant, "NULL_%d", ++DiffSuivant);

			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit (1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
			strcpy (Precedent, tmp);
		}

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit (1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // BODY
	else if (DECLFUNC == Racine->Type)
	{
		strcpy (Precedent, Courant);
		//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> PARAMETRE\n", Racine->Fils1.Nom);

		sprintf (Suivant, "PARAMETRE_%d", ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);

		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (TYPE_SNOEUDFILS == Racine->TypeF3)
		{
			sprintf (Suivant, "ARRAY_%d", ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
			strcpy (Precedent, tmp);
		}
		else
		{
			sprintf (Suivant, "%d_%d", Racine->Fils3.Nombre, ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils3.Nombre;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}

		sprintf (Suivant, "%d_%d", Racine->Fils4.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils4.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils4.Fils, Suivant);
		strcpy (Precedent, tmp);

	} //DECLFUNC
	else if (DECLPROC == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "PARAMETRE_%d", ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);

		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

		sprintf (Suivant, "%d_%d", Racine->Fils3.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils3.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // DECLPROC
	else if (DECLVAR == Racine->Type)
	{
		strcpy (Precedent, Courant);
		if (NULL == Racine->Fils1.Fils)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // DECLVAR
	else if (PARAMETRE == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%s_%d", Racine->Fils1.Nom, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		if (NULL != Racine->Fils2.Frere)
		{
			sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
			sprintf (buf, "\t%s-> \"%s\";\n", Precedent, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils2.Frere->Type;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
			strcpy (Precedent, tmp);
		}

		if (TYPE_SNOEUDFILS == Racine->TypeF3)
		{
			sprintf (Suivant, "ARRAY_%d", ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
			strcpy (Precedent, tmp);
		}
		else
		{
			sprintf (Suivant, "%d_%d", Racine->Fils3.Nombre, ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils3.Nombre;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}

		if (NULL == Racine->Fils4.Frere)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils4.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils4.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils4.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // PARAMETRE
	else if (INSTRUCTION == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // INSTRUCTION
	else if (WHILEDO == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // WHILEDO
	else if (IFTHENELSE == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils3.Fils)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils3.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils3.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // IFTHENELSE
	else if (AFF == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

		sprintf (Suivant, "%d_%d", Racine->Fils3.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils3.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // AFF
	else if (APPELPROC == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // APPELPROC
	else if (ARGUMENTS == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // ARGUMENTS
	else if (BLOC == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // BLOC
	else if (ID == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%s_%d",  Racine->Fils1.Nom, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		if (NULL != Racine->Fils2.Frere)
		{
			sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
			sprintf (buf, "\t%s-> \"%s\";\n", Precedent, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils2.Frere->Type;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}

			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
			strcpy (Precedent, tmp);
		}

		if (TYPE_SNOEUDFILS == Racine->TypeF3)
		{
			sprintf (Suivant, "ARRAY_%d", ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
			strcpy (Precedent, tmp);
		}
		else
		{
			sprintf (Suivant, "%d_%d", Racine->Fils3.Nombre, ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils3.Nombre;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}

		sprintf (Suivant, "%d_%d", Racine->Fils4.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils4.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils4.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // ID
	else if (EXPRESSION == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils2.Frere)
			return;
		sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // EXPRESSION
	else if (CONJONCTION == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils2.Frere)
			return;
		sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // CONJONCTION
	else if (COMPARAISON == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // COMPARAISON
	else if (SUITECOMPARAISON == Racine->Type)
	{
		strcpy (Precedent, Courant);
		if (NULL == Racine->Fils1.Nombre)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils1.Nombre, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Nombre;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

	} //SUITECOMPARAISON
	else if (EXPRARITH == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Nombre, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Nombre;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils3.Nombre)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils3.Nombre, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils3.Nombre;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d_%d", Racine->Fils4.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils4.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils4.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // EXPRARITH
	else if (TERME == Racine->Type)
	{
		strcpy (Precedent, Courant);
		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils2.Nombre)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils2.Nombre, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Nombre;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d_%d", Racine->Fils3.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils3.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils3.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // TERME
	else if (FACTEUR == Racine->Type)
	{
		strcpy (Precedent, Courant);
		if (TYPE_INT == Racine->TypeF1)
		{
			sprintf (Suivant, "%d_%d", Racine->Fils1.Nombre, ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}
		else if (TYPE_NOM == Racine->TypeF1)
		{
			sprintf (Suivant, "%s_%d", Racine->Fils1.Nom, ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}

			if (NULL == Racine->Fils2.Fils)
				return;

			sprintf (Suivant, "%d_%d", Racine->Fils2.Fils->Type, ++DiffSuivant);
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils2.Fils->Type;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
			strcpy (Precedent, tmp);
		}
		else
		{
			sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type), ++DiffSuivant;
			sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
			TabDiffSuivant[iterator] = DiffSuivant;
			TabType[iterator++] = Racine->Fils1.Fils->Type;
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			strcpy (tmp, Precedent);
			CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
			strcpy (Precedent, tmp);

		}

	} // FACTEUR
	else if (LISTEEXPR == Racine->Type)
	{
		strcpy (Precedent, Courant);
		if (NULL == Racine->Fils1.Fils)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils2.Frere->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Precedent, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils2.Frere->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		strcpy (Precedent, tmp);

	} // LISTEEXPR
	else if (VARIABLE == Racine->Type)
	{
		strcpy (Precedent, Courant);
		if (NULL == Racine->Fils1.Fils)
			return;

		sprintf (Suivant, "%d_%d", Racine->Fils1.Fils->Type, ++DiffSuivant);
		sprintf (buf, "\t\"%s\" -> \"%s\";\n", Courant, Suivant);
		TabDiffSuivant[iterator] = DiffSuivant;
		TabType[iterator++] = Racine->Fils1.Fils->Type;
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		strcpy (tmp, Precedent);
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		strcpy (Precedent, tmp);

	} // VARIABLE

	close (fd);
	free (Suivant);
	free (Precedent);
	free (tmp);

} // CreationArbreDot ()

void GenerationArbre (char *pathname)
{
	char *outfile;
	char *file;
	char *cmd;
	int pFils;

	if (( pFils = fork () ) < 0 )
	{
		perror ("fork()");
		exit (1);
	}

	if (0 == pFils) // fils
	{
		outfile = (char *) malloc (256);
		file = (char *) malloc (256);
		cmd = (char *) malloc (256);

		sprintf (outfile, "-o%s.png", pathname);
		sprintf (file, "%s.dot", pathname);

		sprintf (cmd, "dot -Tpng %s  %s", outfile, file);
		if (system (cmd) < 0)
		{
			perror ("system ()");
			exit (1);
		}

		free (cmd);
		free (outfile);
		free (file);

		exit (0);
	}
	wait (NULL);

} // GenerationArbre ()

void AffichageArbre (char *pathname)
{
	int pFils;
	char *cmd;
	if (( pFils = fork () ) < 0 )
	{
		perror ("fork()");
		exit (1);
	}

	if (0 == pFils) // fils
	{
		cmd = (char *) malloc (256);
		sprintf (cmd, "shotwell %s", pathname);
		if (system (cmd) < 0)
		{
			perror ("system ()");
			exit (1);
		}

		exit (0);
	}
	wait (NULL);

} // AffichageArbre ()

#endif /* __STRUCTARBRESYNTAXIQUE_HXX__ */

