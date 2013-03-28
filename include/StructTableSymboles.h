/**
 *
 * @file: StructTableSymboles.h
 *
 * @author: lublud
 *
 * @date: 28/3/2013
 *
 * @version: V1.0
 *
 * @synopsis: Déclaration struct et union pour la création de la table des symboles.
 *
 **/

#if ! defined __STRUCTTABLESYMBOLES_H__
#define 	  __STRUCTTABLESYMBOLES_H__

typedef struct SElement
{
	char Nom[128];
	int Type;
	int IndiceDebut;
	int IndiceFin;
	int Adresse;
} SElement;

extern SElement AjoutElement (char Nom[], int Type, int IndiceDebut, IndiceFin, Adresse);

#include "StructTableSymboles.hxx"
#endif /* __STRUCTTABLESYMBOLES_H__ */
