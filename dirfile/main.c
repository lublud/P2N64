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

	char Reponse[1];

	SNoeud *Racine = CreerNoeud ();

	yyin = fopen (argv[1], "r");
	lexeme = yylex ();
	Racine = ProgrammePascal ();
	fclose (yyin);

	printf ("Voulez vous créer l'arbre"
			"(GraphViz est nécessaire pour continuer...) (o/n) ");
	scanf ("%s", Reponse);

	if ('o' == Reponse[0])
	{
		char *pathname;
		char *NomFichier;
		NomFichier = (char *) malloc (256);
		pathname = (char *) malloc (256);

		printf ("Nom du fichier : ");
		scanf ("%s", NomFichier);

		sprintf (pathname, "%s.dot", NomFichier);
		CreationFichierDot (pathname, Racine);
		GenerationArbre (NomFichier);

		printf ("Afficher l'arbre (o/n) ");
		scanf ("%s", Reponse);

		if ('o' == Reponse[0])
		{
			sprintf (pathname, "%s.png", NomFichier);
			AffichageArbre (pathname);
		}

		free (NomFichier);
		free (pathname);

	}
	printf ("\n");

} // main()
