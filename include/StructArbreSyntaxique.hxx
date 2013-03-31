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
 * @synopsis: Déclaration struct et union pour la création de l'arbre syntaxique
 *
 **/

#if ! defined __STRUCTARBRESYNTAXIQUE_HXX__
#define 	  __STRUCTARBRESYNTAXIQUE_HXX__

#include "StructArbreSyntaxique.h"

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

	fd = open (pathname, O_WRONLY | O_APPEND);
	if (fd < 0)
	{
		perror ("open ()");
		exit (1);
	}

	sprintf (buf, "}");
	if (write (fd, buf, strlen (buf)) < 0)
	{
		perror ("write ()");
		exit (1);
	}

	close (fd);


} // CreationfichierDot ()

int i, diffType;
char *Precedent;
diffType = 0;
i = 0;
void CreationArbreDot (char *pathname, SNoeud *Racine, char *Courant)
{
	char * Suivant = (char *)malloc (256);
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
		Precedent = Courant;
		sprintf (Suivant, "BODY%d", ++i);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit (1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

	} // PRGM
	else if (BODY == Racine->Type)
	{
		if (NULL != Racine->Fils1.Fils)
		{
			if (isalpha (Racine->Fils1.Fils->Fils1.Nom[0]))
					Suivant = Racine->Fils1.Fils->Fils1.Nom;
			else
				sprintf (Suivant, "NULL");

			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit (1);
			}
			CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);
		}

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "BODY%d", ++i);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit (1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);

	} // BODY
	else if (DECLFUNC == Racine->Type)
	{
		//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> PARAMETRE\n", Racine->Fils1.Nom);

		sprintf (Suivant, "PARAMETRE");
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

		if (TYPE_SNOEUDFILS == Racine->TypeF3)
		{
			//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> ARRAY\n", Racine->Fils1.Nom);

			sprintf (Suivant, "ARRAY");
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
		}
		else
		{
			//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> %d\n", Racine->Fils1.Nom, Racine->Fils3.Nombre);
			sprintf (Suivant, "\"%d_%d\"", Racine->Fils3.Nombre, ++diffType);
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}

		//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> BODY%d\n", Racine->Fils1.Nom, ++i);

		sprintf (Suivant, "BODY%d", Racine->Fils1.Nom, ++i);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils4.Fils, Suivant);

	} //DECLFUNC
	else if (DECLPROC == Racine->Type)
	{
		//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> PARAMETRE\n", Racine->Fils1.Nom);
		sprintf (Suivant, "PARAMETRE");
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

		//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> BODY%d\n", Racine->Fils1.Nom, ++i);
		sprintf (Suivant, "BODY%d", ++i);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);

	} // DECLPROC
	else if (DECLVAR == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

	} // DECLVAR
	else if (PARAMETRE == Racine->Type)
	{
		Suivant = Racine->Fils1.Nom;
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		if (NULL != Racine->Fils2.Frere)
		{
			sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
			sprintf (buf, "\t%s-> %s;\n", Precedent, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}

			CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		}

		if (TYPE_SNOEUDFILS == Racine->TypeF3)
		{
			//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> ARRAY\n", Racine->Fils1.Nom);
			sprintf (Suivant, "ARRAY");
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
		}
		else
		{
			//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> %d\n", Racine->Fils1.Nom, Racine->Fils3.Nombre);
			sprintf (Suivant, "\"%d_%d\"", Racine->Fils3.Nombre, ++diffType);
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}

		if (NULL == Racine->Fils4.Frere)
			return;

		sprintf (Suivant, "%d", Racine->Fils4.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils4.Frere, Suivant);

	} // PARAMETRE
	else if (INSTRUCTION == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);

	} // INSTRUCTION
	else if (WHILEDO == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

	} // WHILEDO
	else if (IFTHENELSE == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

		if (NULL == Racine->Fils3.Fils)
			return;

		sprintf (Suivant, "%d", Racine->Fils3.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);

	} // IFTHENELSE
	else if (AFF == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

		sprintf (Suivant, "%d", Racine->Fils3.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);

	} // AFF
	else if (APPELPROC == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

	} // APPELPROC
	else if (ARGUMENTS == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);

	} // ARGUMENTS
	else if (BLOC == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

	} // BLOC
	else if (ID == Racine->Type)
	{
		Suivant = Racine->Fils1.Nom;
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		if (NULL != Racine->Fils2.Frere)
		{
			sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
			sprintf (buf, "\t%s-> %s;\n", Precedent, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}

			CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);
		}

		if (TYPE_SNOEUDFILS == Racine->TypeF3)
		{
			//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> ARRAY\n", Racine->Fils1.Nom);
			sprintf (Suivant, "ARRAY");
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			CreationArbreDot (pathname, Racine->Fils3.Fils, Suivant);
		}
		else
		{
			//sprintf (buf, "\t\"DECLARATION FONCTION | %s\" -> %d\n", Racine->Fils1.Nom, Racine->Fils3.Nombre);
			sprintf (Suivant, "\"%d_%d\"", Racine->Fils3.Nombre, ++diffType);
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}

		sprintf (Suivant, "%d", Racine->Fils4.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils4.Fils, Suivant);

	} // ID
	else if (EXPRESSION == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		if (NULL == Racine->Fils2.Frere)
			return;
		sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);

	} // EXPRESSION
	else if (CONJONCTION == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		if (NULL == Racine->Fils2.Frere)
			return;
		sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);

	} // CONJONCTION
	else if (COMPARAISON == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

	} // COMPARAISON
	else if (SUITECOMPARAISON == Racine->Type)
	{
		if (NULL == Racine->Fils1.Nombre)
			return;

		sprintf (Suivant, "%d", Racine->Fils1.Nombre);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

	} //SUITECOMPARAISON
	else if (EXPRARITH == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Nombre);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);

		if (NULL == Racine->Fils3.Nombre)
			return;

		sprintf (Suivant, "%d", Racine->Fils3.Nombre);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d", Racine->Fils4.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils4.Frere, Suivant);

	} // EXPRARITH
	else if (TERME == Racine->Type)
	{
		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		if (NULL == Racine->Fils2.Nombre)
			return;

		sprintf (Suivant, "%d", Racine->Fils2.Nombre);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}

		sprintf (Suivant, "%d", Racine->Fils3.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils3.Frere, Suivant);

	} // TERME
	else if (FACTEUR == Racine->Type)
	{
		if (TYPE_INT == Racine->TypeF1)
		{
			sprintf (Suivant, "%d", Racine->Fils1.Nombre);
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
		}
		else if (TYPE_NOM == Racine->TypeF1)
		{
			Suivant = Racine->Fils1.Nom;
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}

			if (NULL == Racine->Fils2.Fils)
				return;

			sprintf (Suivant, "%d", Racine->Fils2.Fils->Type);
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			CreationArbreDot (pathname, Racine->Fils2.Fils, Suivant);
		}
		else
		{
			sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
			sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
			if (write (fd, buf, strlen (buf)) < 0)
			{
				perror ("write ()");
				exit(1);
			}
			CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		}

	} // FACTEUR
	else if (LISTEEXPR == Racine->Type)
	{
		if (NULL == Racine->Fils1.Fils)
			return;

		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

		if (NULL == Racine->Fils2.Frere)
			return;

		sprintf (Suivant, "%d", Racine->Fils2.Frere->Type);
		sprintf (buf, "\t%s -> %s;\n", Precedent, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils2.Frere, Suivant);

	} // LISTEEXPR
	else if (VARIABLE == Racine->Type)
	{
		if (NULL == Racine->Fils1.Fils)
			return;

		sprintf (Suivant, "%d", Racine->Fils1.Fils->Type);
		sprintf (buf, "\t%s -> %s;\n", Courant, Suivant);
		if (write (fd, buf, strlen (buf)) < 0)
		{
			perror ("write ()");
			exit(1);
		}
		CreationArbreDot (pathname, Racine->Fils1.Fils, Suivant);

	} // VARIABLE

close (fd);
free (Suivant);

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

