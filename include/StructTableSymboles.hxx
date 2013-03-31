/**
 *
 * @file: StructTableSymboles.hxx
 *
 * @author: lublud
 *
 * @date: 28/3/2013
 *
 * @version: V1.0
 *
 * @synopsis: Déclaration struct et union pour la création de l'arbre syntaxique
 *
 **/

#if ! defined __STRUCTTABLESYMBOLES_HXX__
#define 	  __STRUCTTABLESYMBOLES_HXX__

#include "StructTableSymboles.h"

SElement *AjoutElement (char *Nom, int Type, int IndiceDebut, int IndiceFin, int Adresse)
{
	SElement *elem;
	elem->Nom = Nom;
	elem->Type = Type;
	elem->IndiceDebut = IndiceDebut;
	elem->IndiceFin = IndiceFin;
	elem->Adresse = Adresse;

	return elem;

} // AjoutElement ()

#endif /* __STRUCTTABLESYMOBLES_HXX__ */
