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

unsigned Adresse = 0;


STableSymbole *AjoutElementTableSymbole (char *Nom, int Type, int NbParametre,
							int IndiceDebut, int IndiceFin, int Adresse, char *NomFonction)
{
	STableSymbole *elem;
	elem = (STableSymbole *) malloc (sizeof (STableSymbole));

	elem->Nom = Nom;
	elem->Type = Type;
	elem->IndiceDebut = IndiceDebut;
	elem->IndiceFin = IndiceFin;
	elem->Adresse = Adresse;
	elem->NbParametre = NbParametre;
	elem->NomFonction = NomFonction;
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

void VerifierDispoVariable (STableSymbole *PremiereTablePile, char *NomVariable)
{

	STableSymbole *TS = PremiereTablePile;

	for (; ; TS = TS->SuivantElement)
	{
		if (0 == strcmp (NomVariable, TS->Nom))
		{
			fprintf (stderr, "Variable %s already exists.\n", NomVariable);
			exit (1);
		}

		if (NULL == TS->SuivantElement)
			break;

	}


} // VerifierDispoVariable ()

void CreationTableSymbole (SNoeud *Racine, SPile *Courant)
{
	STableSymbole *TableCourante;
	SNoeud *NoeudCourant;
	int IndiceDebut, IndiceFin, TypeVariable;

	for (TableCourante = Courant->TableSymbole; ; TableCourante = TableCourante->SuivantElement)
		if (NULL == TableCourante->SuivantElement)
			break;


	if (PRGM == Racine->Type)
	{
		TableCourante = AjoutElementTableSymbole (Racine->Fils1.Nom, NULL, -1, NULL, NULL, NULL, NULL);
		return CreationTableSymbole (Racine->Fils2.Fils, Courant);

	}

	if (TYPE_SNOEUDFILS == Racine->TypeF1)
	{
		NoeudCourant = Racine->Fils1.Fils;

		if (DECLVAR == NoeudCourant->Type)
		{

			for (SNoeud *tmp = NoeudCourant->Fils1.Fils; ; tmp = tmp->Fils4.Fils)
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

				for (SNoeud * tmpMemeType = tmp; ; tmpMemeType = tmpMemeType->Fils2.Frere)
				{
					VerifierDispoVariable (Courant->TableSymbole, tmpMemeType->Fils1.Nom);
					TableCourante->SuivantElement = AjoutElementTableSymbole (tmpMemeType->Fils1.Nom,
							TypeVariable, -1, IndiceDebut, IndiceFin, Adresse, NULL);
					Adresse += 4;

					if (NULL == tmpMemeType->Fils2.Frere)
						break;
					TableCourante = TableCourante->SuivantElement;
				}

				TableCourante = TableCourante->SuivantElement;
				if (TYPE_NOM != tmp->Fils4.Fils->TypeF1)
					break;
			}

		} // DECLVAR
		else if (DECLFUNC == NoeudCourant->Type || DECLPROC == NoeudCourant->Type)
		{

			if (DECLFUNC == NoeudCourant->Type)
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

			else
				TypeVariable = IndiceDebut = IndiceFin = NULL;

			char *NomFonction = NoeudCourant->Fils1.Nom;
			VerifierDispoVariable (Courant->TableSymbole, NomFonction);
			TableCourante->SuivantElement = AjoutElementTableSymbole (NomFonction, TypeVariable, -1,
					IndiceDebut, IndiceFin, Adresse, NULL);
			Adresse += 4;

			TableCourante = TableCourante->SuivantElement;


			int NbParametre = 0;
			if (NULL != NoeudCourant->Fils2.Fils->Fils1.Nom)
			{
				STableSymbole *TStmp, *PremiereTablePile;
				TStmp = (STableSymbole *) malloc (sizeof (STableSymbole));

				SPile *PileTmp;

				for (PileTmp = Courant; ; PileTmp = PileTmp->Suivant)
					if (NULL == PileTmp->Suivant)
					{
						PileTmp->Suivant = AjoutTableSymboleSurPile (TStmp);
						PileTmp = PileTmp->Suivant;
						break;
					}

				free (TStmp);

				TStmp = PremiereTablePile = PileTmp->TableSymbole;

				int AdresseNouvellePile = 0;

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

						if (1 < ++NbParametre)
						{
							VerifierDispoVariable (PremiereTablePile, ParamMemeType->Fils1.Nom);

							TStmp->SuivantElement = AjoutElementTableSymbole (ParamMemeType->Fils1.Nom,
									TypeVariable, -1, IndiceDebut, IndiceFin,
									AdresseNouvellePile, NomFonction);
							TStmp = TStmp->SuivantElement;
						}
						else

							TStmp = AjoutElementTableSymbole (ParamMemeType->Fils1.Nom, TypeVariable,
									-1, IndiceDebut, IndiceFin, AdresseNouvellePile, NomFonction);

						AdresseNouvellePile += 4;


						if (NULL == ParamMemeType->Fils2.Frere)
							break;

					}

					if (NULL == tmp->Fils4.Frere)
						break;
				}

			}

			TableCourante->NbParametre = NbParametre;

		} // DECLFUNC || DECLPROC

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

	for (SPile *Piletmp = Pile; ; Piletmp = Piletmp->Suivant)
	{
		printf ("\n== Nouvelle pile ==\n");
		for (STableSymbole *tmp = Piletmp->TableSymbole; ; tmp = tmp->SuivantElement)
		{
			printf ("Nom : %s\nType : %d\nIndice début : %d\nIndice Fin : %d\n"
						"Adresse : %d\nNombre Paramètre : %d\nNom Fonction : %s\n\n",
					tmp->Nom, tmp->Type, tmp->IndiceDebut,
					tmp->IndiceFin, tmp->Adresse,
					tmp->NbParametre, tmp->NomFonction);

			if (NULL == tmp->SuivantElement)
				break;

		}

		if (NULL == Piletmp->Suivant)
			break;
	}

} // AfficherPile ()

#endif /* __STRUCTTABLESYMOBLES_HXX__ */
