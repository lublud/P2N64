/**
 *
 * @file: StructArbreSyntaxique
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

#if ! defined __STRUCTARBRESYNTAXIQUE_H__
#define 	  __STRUCTARBRESYNTAXIQUE_H__

union MyUnion
{
	struct SNoeud *Fils;
	int Nombre;
	char *Nom;
	struct SNoeud *Frere;
};

typedef struct SNoeud
{
	int Type;
	union MyUnion Fils1;
	union MyUnion Fils2;
	union MyUnion Fils3;
	union MyUnion Fils4;
} SNoeud;

#include "StructArbreSyntaxique.hxx"
#endif /* __STRUCTARBRESYNTAXIQUE_H__ */
