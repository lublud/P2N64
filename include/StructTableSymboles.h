/**
 *
 * @file: StructTableSymboles.h
 *
 * @author: lublud
 *
 * @date: 28/3/2013
 *
 * @brief Déclaration struct et union pour la création de la table des symboles.
 *
 **/

#if ! defined __STRUCTTABLESYMBOLES_H__
#define 	  __STRUCTTABLESYMBOLES_H__

typedef struct STableSymbole
{
	char *Nom;
	int Type;
	int IndiceDebut;
	int IndiceFin;
	int Adresse;
	struct STableSymbole *Parametre;
	struct STableSymbole *SuivantElement;
} STableSymbole;

typedef struct SPile
{
	STableSymbole *TableSymbole;
	struct Pile *Suivant;
} SPile;

extern STableSymbole *AjoutElementTableSymbole (char *Nom, int Type,
									int IndiceDebut, int IndiceFin, int Adresse);

extern SPile *AjoutTableSymboleSurPile (STableSymbole *TableSymbole);

extern void CreationTableSymbole (SNoeud *Racine, SPile *Courant);
extern SPile *CreationPile (SNoeud *Racine);
extern int VerifierDispoVariable (STableSymbole *PremiereTablePile,
							char *NomVariable, char *NomFonctionCourante);
extern void AfficherPile (SPile *Pile);

#include "StructTableSymboles.hxx"
#endif /* __STRUCTTABLESYMBOLES_H__ */
