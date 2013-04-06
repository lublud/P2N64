/**
 *
 * @file: StructTableSymboles.hxx
 *
 * @author: lublud
 *
 * @date: 28/3/2013
 *
 * @brief Déclaration struct et union pour la création de l'arbre syntaxique
 *
 **/

#if ! defined __STRUCTTABLESYMBOLES_HXX__
#define 	  __STRUCTTABLESYMBOLES_HXX__

#include "StructTableSymboles.h"

STableSymbole *AjoutElementTableSymbole (char *Nom, int Type,
							int IndiceDebut, int IndiceFin, int Adresse)
{
	STableSymbole *elem;
	elem = (STableSymbole *) malloc (sizeof (STableSymbole));

	elem->Nom = Nom;
	elem->Type = Type;
	elem->IndiceDebut = IndiceDebut;
	elem->IndiceFin = IndiceFin;
	elem->Adresse = Adresse;
	elem->Parametre = NULL;
	elem->SuivantElement = NULL;

	return elem;

} // AjoutElementTableSymbole ()

SPile *AjoutTableSymboleSurPile (STableSymbole *TableSymbole)
{
	SPile *pile;
	pile = (SPile *) malloc (sizeof (SPile));

	pile->TableSymbole = TableSymbole;
	pile->Suivant = NULL;

	return pile;

} // AjoutTableSymboleSurPile ()

int VerifierDispoVariable (STableSymbole *PremiereTablePile, char *NomVariable)
{

	STableSymbole *TS = PremiereTablePile;

	for (; ; TS = TS->SuivantElement)
	{
		if (NomVariable == TS->Nom)
		{
			fprintf (stderr, "Variable %s already exists.\n", NomVariable);
			exit (1);
		}
		if (NULL == TS->SuivantElement)
			break;
	}


} // VerifierDispoVariable ()

STableSymbole *CreationTableSymbole (SNoeud *Racine, SPile *Courant)
{
	STableSymbole *TableCourante;
	SNoeud *NoeudCourant = CreerNoeud ();
	NoeudCourant = Racine;
	TableCourante = Courant->TableSymbole;
	unsigned Adresse = 0;
	int IndiceDebut, IndiceFin, TypeVariable;

	for (;;)
	{
		if (PRGM == NoeudCourant->Type)
		{
			TableCourante = AjoutElementTableSymbole (NoeudCourant->Fils1.Nom, NULL, NULL, NULL, NULL);
			NoeudCourant = NoeudCourant->Fils2.Fils->Fils1.Fils;
			continue;

		}
		else if (DECLVAR == NoeudCourant->Type)
		{
			NoeudCourant = NoeudCourant->Fils1.Fils;
			if (TYPE_SNOEUDFILS == NoeudCourant->TypeF3)
			{
				IndiceDebut = NoeudCourant->Fils3.Fils->Fils1.Nombre;
				IndiceFin = NoeudCourant->Fils3.Fils->Fils2.Nombre;
				TypeVariable = NoeudCourant->Fils3.Fils->Fils3.Nombre;
			}
			else
			{
				IndiceDebut = IndiceFin = NULL;
				TypeVariable = NoeudCourant->Fils3.Nombre;
			}

			if (TYPE_SNOEUDFRERE != NoeudCourant->TypeF2)
			{
				VerifierDispoVariable (Courant->TableSymbole, NoeudCourant->Fils1.Nom);
				TableCourante->SuivantElement = AjoutElementTableSymbole (NoeudCourant->Fils1.Nom,
														TypeVariable, IndiceDebut, IndiceFin, Adresse);
				Adresse += 4;
			}
			else
			{
				for (SNoeud *tmp = NoeudCourant; ; tmp = tmp->Fils2.Frere)
				{
					VerifierDispoVariable (Courant->TableSymbole, tmp->Fils1.Nom);
					TableCourante->SuivantElement = AjoutElementTableSymbole (tmp->Fils1.Nom, TypeVariable,
															IndiceDebut, IndiceFin, Adresse);
					Adresse += 4;
					if (NULL == tmp->Fils2.Frere)
						break;
					TableCourante = TableCourante->SuivantElement;
				}
			}
			break;
		} // DECLVAR
		else if (DECLFUNC == NoeudCourant->Type)
		{
			if (TYPE_SNOEUDFILS == NoeudCourant->TypeF3)
			{
				IndiceDebut = NoeudCourant->Fils3.Fils->Fils1.Nombre;
				IndiceFin = NoeudCourant->Fils3.Fils->Fils2.Nombre;
				TypeVariable = NoeudCourant->Fils3.Fils->Fils3.Nombre;
			}
			else
			{
				IndiceDebut = IndiceFin = NULL;
				TypeVariable = NoeudCourant->Fils3.Nombre;
			}

			VerifierDispoVariable (Courant->TableSymbole, NoeudCourant->Fils1.Nom);
			TableCourante->SuivantElement = AjoutElementTableSymbole (NoeudCourant->Fils1.Nom, TypeVariable,
					IndiceDebut, IndiceFin, Adresse);
			Adresse += 4;
			
			TableCourante = TableCourante->SuivantElement;

			STableSymbole *TStmp = TableCourante;
			for (SNoeud *tmp = NoeudCourant->Fils2.Fils; ; tmp = tmp->Fils2.Frere)
			{

				if (TYPE_SNOEUDFILS == tmp->TypeF3)
				{
					IndiceDebut = tmp->Fils3.Fils->Fils1.Nombre;
					IndiceFin = tmp->Fils3.Fils->Fils2.Nombre;
					TypeVariable = tmp->Fils3.Fils->Fils3.Nombre;
				}
				else
				{
					IndiceDebut = IndiceFin = NULL;
					TypeVariable = tmp->Fils3.Nombre;
				}

				VerifierDispoVariable (Courant->TableSymbole, tmp->Fils1.Nom);
				TableCourante->Parametre = AjoutElementTableSymbole (tmp->Fils1.Nom, TypeVariable,
						IndiceDebut, IndiceFin, Adresse);
				Adresse += 4;


				if (NULL == tmp->Fils2.Frere)
					break;
				TableCourante = TableCourante->Parametre;

			}

			TableCourante = TStmp;
			break;

		} //DECLFUNC
	}

	free (NoeudCourant);
	return Courant->TableSymbole;

} // CreationTableSymbole ()

SPile * CreationPile (SNoeud *Racine)
{
	SPile *Pile;
	STableSymbole *TS;
	TS = (STableSymbole *) malloc (sizeof (STableSymbole));
	Pile = AjoutTableSymboleSurPile (TS);
	free (TS);
	Pile->TableSymbole = CreationTableSymbole (Racine, Pile);

	return Pile;

} // CreationPile ()

void AfficherPile (SPile *Pile)
{

	for (;; Pile = Pile->Suivant)
	{
		for (; ; Pile->TableSymbole = Pile->TableSymbole->SuivantElement)
		{
			printf ("Nom : %s\nType : %d\nIndice début : %d\nIndice Fin : %d\nAdresse : %d\n\n",
					Pile->TableSymbole->Nom, Pile->TableSymbole->Type, Pile->TableSymbole->IndiceDebut,
					Pile->TableSymbole->IndiceFin, Pile->TableSymbole->Adresse);

			if (NULL != Pile->TableSymbole->Parametre)
			{
				STableSymbole *tmp = Pile->TableSymbole->Parametre;
				for (;; tmp = tmp->Parametre)
				{
					printf ("Nom : %s\nType : %d\nIndice début : %d\nIndice Fin : %d\nAdresse : %d\n\n",
						tmp->Nom, tmp->Type, tmp->IndiceDebut, tmp->IndiceFin, tmp->Adresse);
					if (NULL == tmp->Parametre)
						break;
				}
			}

			if (NULL == Pile->TableSymbole->SuivantElement)
				break;

		}
		printf ("\n\n");
		if (NULL == Pile->Suivant)
			break;
	}

} // AfficherPile ()

#endif /* __STRUCTTABLESYMOBLES_HXX__ */
