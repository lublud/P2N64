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

unsigned Adresse;

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
	Adresse = 0;

	return pile;

} // AjoutTableSymboleSurPile ()

int VerifierDispoVariable (STableSymbole *PremiereTablePile, char *NomVariable, char *NomFonctionCourante)
{

	STableSymbole *TS = PremiereTablePile;

	for (; ; TS = TS->SuivantElement)
	{
		if (0 == strcmp (NomVariable, TS->Nom))
		{
			fprintf (stderr, "Variable %s already exists.\n", NomVariable);
			exit (1);
		}

		if ((NULL != TS->Parametre) && (0 == strcmp (NomFonctionCourante, TS->Nom)))
		{
			STableSymbole *TSParam = TS->Parametre;
			for (; ; TSParam = TSParam->Parametre)
			{
				if (0 == strcmp (NomVariable, TSParam->Nom))
				{
					fprintf (stderr, "Variable %s already exists.\n", NomVariable);
					exit (1);
				}
				if (NULL == TSParam->Parametre)
					break;

			}
		}

		if (NULL == TS->SuivantElement)
			break;

	}


} // VerifierDispoVariable ()

STableSymbole *TableCourante;
void CreationTableSymbole (SNoeud *Racine, SPile *Courant)
{

	for (TableCourante = Courant->TableSymbole; ; TableCourante = TableCourante->SuivantElement)
		if (NULL == TableCourante->SuivantElement)
			break;

	SNoeud *NoeudCourant;
	int IndiceDebut, IndiceFin, TypeVariable;

	if (PRGM == Racine->Type)
	{
		TableCourante = AjoutElementTableSymbole (Racine->Fils1.Nom, NULL, NULL, NULL, NULL);
		return CreationTableSymbole (Racine->Fils2.Fils,
													Courant);

	}

	if (TYPE_SNOEUDFILS == Racine->TypeF1)
	{
		NoeudCourant = Racine->Fils1.Fils;

		if (DECLVAR == NoeudCourant->Type)
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

			if (TYPE_SNOEUDFRERE != NoeudCourant->TypeF2)
			{
				VerifierDispoVariable (Courant->TableSymbole, NoeudCourant->Fils1.Nom, NULL);
				TableCourante->SuivantElement = AjoutElementTableSymbole (NoeudCourant->Fils1.Nom,
						TypeVariable, IndiceDebut, IndiceFin, Adresse);
				Adresse += 4;
			}
			else
			{
				for (SNoeud *tmp = NoeudCourant; ; tmp = tmp->Fils2.Frere)
				{
					VerifierDispoVariable (Courant->TableSymbole, tmp->Fils1.Nom, NULL);
					TableCourante->SuivantElement = AjoutElementTableSymbole (tmp->Fils1.Nom, TypeVariable,
							IndiceDebut, IndiceFin, Adresse);
					Adresse += 4;
					TableCourante = TableCourante->SuivantElement;
					if (NULL == tmp->Fils2.Frere)
						break;
				}
			}

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

			char *NomFonction = NoeudCourant->Fils1.Nom;
			VerifierDispoVariable (Courant->TableSymbole, NomFonction, NomFonction);
			TableCourante->SuivantElement = AjoutElementTableSymbole (NomFonction, TypeVariable,
					IndiceDebut, IndiceFin, Adresse);
			Adresse += 4;

			TableCourante = TableCourante->SuivantElement;

			STableSymbole *TStmp = TableCourante;
			for (SNoeud *tmp = NoeudCourant->Fils2.Fils; ; tmp = tmp->Fils4.Frere)
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

				for (SNoeud *ParamMemeType = tmp; ;
						ParamMemeType = ParamMemeType->Fils2.Frere)
				{
					if (NULL == ParamMemeType->Fils1.Nom)
						break;

					VerifierDispoVariable (Courant->TableSymbole, ParamMemeType->Fils1.Nom, NomFonction);
					TableCourante->Parametre = AjoutElementTableSymbole (ParamMemeType->Fils1.Nom,
							TypeVariable, IndiceDebut, IndiceFin, Adresse);
					Adresse += 4;


					if (NULL == ParamMemeType->Fils2.Frere)
						break;
					TableCourante = TableCourante->Parametre;

				}
				if (NULL == tmp->Fils4.Frere)
					break;
				TableCourante = TableCourante->Parametre;
			}

			TableCourante = TStmp;

		} //DECLFUNC
		else if (DECLPROC == NoeudCourant->Type)
		{
		} // DECLFUNC

		CreationTableSymbole (Racine->Fils1.Fils, Courant);
	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF1)
	{
		NoeudCourant = Racine->Fils1.Frere;
		CreationTableSymbole (Racine->Fils1.Frere, Courant);
	}
	else if (TYPE_INT == Racine->TypeF1)
	{

	}
	else if (TYPE_NOM == Racine->TypeF1)
	{

	}

	if (TYPE_SNOEUDFILS == Racine->TypeF2)
	{
		NoeudCourant = Racine->Fils2.Fils;
		CreationTableSymbole (Racine->Fils2.Fils, Courant);
	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF2)
	{
		NoeudCourant = Racine->Fils2.Frere;
		CreationTableSymbole (Racine->Fils2.Frere, Courant);
	}
	else if (TYPE_INT == Racine->TypeF2)
	{

	}
	else if (TYPE_NOM == Racine->TypeF2)
	{

	}
	if (TYPE_SNOEUDFILS == Racine->TypeF3)
	{
		NoeudCourant = Racine->Fils3.Fils;
		CreationTableSymbole (Racine->Fils3.Fils, Courant);
	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF3)
	{
		NoeudCourant = Racine->Fils3.Frere;
		CreationTableSymbole (Racine->Fils3.Frere, Courant);
	}
	else if (TYPE_INT == Racine->TypeF3)
	{

	}
	else if (TYPE_NOM == Racine->TypeF3)
	{

	}
	if (TYPE_SNOEUDFILS == Racine->TypeF4)
	{
		NoeudCourant = Racine->Fils4.Fils;
		CreationTableSymbole (Racine->Fils4.Fils, Courant);
	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF4)
	{
		NoeudCourant = Racine->Fils4.Frere;
		CreationTableSymbole (Racine->Fils4.Frere, Courant);
	}
	else if (TYPE_INT == Racine->TypeF4)
	{

	}
	else if (TYPE_NOM == Racine->TypeF4)
	{

	}

} // CreationTableSymbole ()

SPile * CreationPile (SNoeud *Racine)
{
	SPile *Pile;
	STableSymbole *TS;
	TS = (STableSymbole *) malloc (sizeof (STableSymbole));
	Pile = AjoutTableSymboleSurPile (TS);
	free (TS);
	CreationTableSymbole (Racine, Pile);

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
