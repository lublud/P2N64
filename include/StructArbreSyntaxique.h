/**
 *
 * @file: StructArbreSyntaxique.h
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
	int TypeF1;
	union MyUnion Fils2;
	int TypeF2;
	union MyUnion Fils3;
	int TypeF3;
	union MyUnion Fils4;
	int TypeF4;
} SNoeud;

extern SNoeud *CreerNoeud ();
extern char *CreerChaine (char *chaine);
extern void CreationFichierDot (char *pathname, SNoeud *Racine);
extern void CreationArbreDot (char *pathname, SNoeud *Racine, char *Courant);
extern void GenerationArbre (char *pathname);
extern void AffichageArbre (char *pathname);

#include "StructArbreSyntaxique.hxx"
#endif /* __STRUCTARBRESYNTAXIQUE_H__ */
