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
		fprintf (stderr, "Error: malloc ()\n");
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
		fprintf (stderr, "Error malloc\n");
		exit (EXIT_FAILURE);
	}

	c = strcpy (c, chaine);
	return c;
} // CreerChaine ()

#endif /* __STRUCTARBRESYNTAXIQUE_HXX__ */

