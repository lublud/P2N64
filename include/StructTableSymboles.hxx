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
int TypeInstruction;


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
	elem->NbUtilisation = 0;
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


int VerifierExistenceVariable (STableSymbole *PremiereTablePile, char *NomVariable)
{

	STableSymbole *TS = PremiereTablePile;
	int OperandeGtmp = -1;
	int TypeReturn = -1;

	if (AFF == TypeInstruction && -1 == OperandeG->TypeOperande)
		OperandeGtmp = 1;

	for (; -1 == TypeReturn; TS = TS->SuivantElement)
	{
		if (0 == strcmp (NomVariable, TS->Nom))
		{
			++TS->NbUtilisation;

			if (-1 != TS->NbParametre && 1 != OperandeGtmp)
			{
				NomFonctionAVerif = TS->Nom;
				NbParamAVerif = TS->NbParametre;
				NbParamVerif = 0;
			}

			TypeReturn = TS->Type;
			IndiceDebGlob = TS->IndiceDebut;
			IndiceFinGlob = TS->IndiceFin;

			if (0 != TS->IndiceDebut && 0 != TS->IndiceFin)
				IsTableau = 1;
			else
				IsTableau = 0;
		}

		if (NULL == TS->SuivantElement)
			break;
	}

	TS = TableVarGlobal;

	for (; -1 == TypeReturn; TS = TS->SuivantElement)
	{
		if (0 == strcmp (NomVariable, TS->Nom))
		{
			++TS->NbUtilisation;

			if (-1 != TS->NbParametre && 1 != OperandeGtmp)
			{
				NomFonctionAVerif = TS->Nom;
				NbParamAVerif = TS->NbParametre;
				NbParamVerif = 0;
			}

			TypeReturn = TS->Type;
			IndiceDebGlob = TS->IndiceDebut;
			IndiceFinGlob = TS->IndiceFin;
			if (0 != TS->IndiceDebut && 0 != TS->IndiceFin)
				IsTableau = 1;
			else
				IsTableau = 0;
		}

		if (NULL == TS->SuivantElement)
			break;
	}

	if (COMPARAISON == TypeInstruction && BOOL == TypeReturn)
		printf ("Error: '%s' can't be a boolean.\n", NomVariable);
	else if (-1 != TypeReturn)
		return TypeReturn;
	else
		printf ("Error: '%s': undeclared identifier.\n", NomVariable);

	free (TS);
	exit (1);

} // VerifierExistenceVariable ()

void VerifierParametre (char *NomFonction, int TypeParam)
{

	/* Si on vérifie un nouvel arg alors qu'ils ont tous été vérifier */
	if (NbParamVerif >= NbParamAVerif)
	{
		/* C'est un arg de trop */
		++NbParamVerif; /* On ++ pour le message d'erreur ... */
		return;
	}

	SPile *PileTmp = DebPile->Suivant;
	STableSymbole *TS;
	char *TypeRecu = (char *) malloc (16);

	if (NomFonction == "read"   ||
			NomFonction == "readln" ||
			NomFonction == "write"  ||
			NomFonction == "writeln")
	{
		return;
	}


	for (; ; PileTmp = PileTmp->Suivant)
		if (0 == strcmp (NomFonction, PileTmp->TableSymbole->NomFonction) )
		{
			TS = PileTmp->TableSymbole;
			break;
		}

	for (int i = 0; ; TS = TS->SuivantElement)
	{
		if (i++ == NbParamVerif)
		{
			if (TypeParam == TS->Type)
			{
				++NbParamVerif;
				return;
			}
			else
				break;
		}

		if (NULL == TS->SuivantElement)
		{
			printf ("ERROR!\n");
			exit (1);
		}
	}

	if (INTEGER == TypeParam)
		sprintf (TypeRecu, "integer");
	else
		sprintf (TypeRecu, "boolean");

	printf ("Error: %s received in function %s.\n", TypeRecu, NomFonction);

	free (TS);
	free (TypeRecu);
	free (PileTmp);

	exit (1);

} // VerifierParametre ()


void VerifierUtilisationVariables (SPile * Pile)
{
	for (SPile *Piletmp = Pile; ; Piletmp = Piletmp->Suivant)
	{
		for (STableSymbole *tmp = Piletmp->TableSymbole; ; tmp = tmp->SuivantElement)
		{
			if (0 == tmp->NbUtilisation)
				printf ("Warning: %s not used in %s.\n", tmp->Nom, tmp->NomFonction);

			if (NULL == tmp->SuivantElement)
				break;

		}

		if (NULL == Piletmp->Suivant)
			break;
	}

} // VerifierUtilisationVariables ()

void ErreurTypes (SOperande *OpG, SOperande *OpD)
{
	char *TypeOpG, *TypeOpD;
	TypeOpG = (char *) malloc (16);
	TypeOpD = (char *) malloc (16);

	if (INTEGER == OpG->TypeOperande)
		sprintf (TypeOpG, "integer");
	else
		sprintf (TypeOpG, "boolean");

	if (INTEGER == OpD->TypeOperande)
		sprintf (TypeOpD, "integer");
	else
		sprintf (TypeOpD, "boolean");

	if (TYPE_NOM == OpG->TypeID && TYPE_NOM == OpD->TypeID)
		printf ("Error: type of %s (%s) does not correspond to %s (%s)\n",
				OpG->IDOperande.Nom, TypeOpG, OpD->IDOperande.Nom, TypeOpD);
	if (TYPE_NOM == OpG->TypeID && TYPE_INT == OpD->TypeID)
		printf ("Error: type of %s (%s) does not correspond to %d (%s)\n",
				OpG->IDOperande.Nom, TypeOpG, OpD->IDOperande.Nombre, TypeOpD);
	if (TYPE_INT == OpG->TypeID && TYPE_NOM == OpD->TypeID)
		printf ("Error: type of %d (%s) does not correspond to %s (%s)\n",
				OpG->IDOperande.Nombre, TypeOpG, OpD->IDOperande.Nom, TypeOpD);
	if (TYPE_INT == OpG->TypeID && TYPE_INT == OpD->TypeID)
		printf ("Error: type of %d (%s) does not correspond to %d (%s)\n",
				OpG->IDOperande.Nombre, TypeOpG, OpD->IDOperande.Nombre, TypeOpD);

	free (TypeOpG);
	free (TypeOpD);

	exit (1);

} // ErreurTypes ()

void CreationTableSymbole (SNoeud *Racine, SPile *Courant, char *NomFonctionCourante)
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
		TableCourante->NbUtilisation = 1;
		TableVarGlobal = TableCourante;
		DebPile = Courant;

		/* Ajout read (), write (), readln (), writeln () dans la table */
		TableCourante->SuivantElement = AjoutElementTableSymbole ("read", 270, -1,
				NULL, NULL, NULL, NomFonctionCourante);
		TableCourante = TableCourante->SuivantElement;
		TableCourante->NbUtilisation = 1;

		TableCourante->SuivantElement = AjoutElementTableSymbole ("write", NULL, 1,
				NULL, NULL, NULL, NomFonctionCourante);
		TableCourante = TableCourante->SuivantElement;
		TableCourante->NbUtilisation = 1;

		TableCourante->SuivantElement = AjoutElementTableSymbole ("readln", 270, -1,
				NULL, NULL, NULL, NomFonctionCourante);
		TableCourante = TableCourante->SuivantElement;
		TableCourante->NbUtilisation = 1;

		TableCourante->SuivantElement = AjoutElementTableSymbole ("writeln", NULL, 1,
				NULL, NULL, NULL, NomFonctionCourante);
		TableCourante = TableCourante->SuivantElement;
		TableCourante->NbUtilisation = 1;

		return CreationTableSymbole (Racine->Fils2.Fils, Courant, NomFonctionCourante);

	} // PRGM
	else if (AFF == Racine->Type)
		TypeInstruction = AFF;
	else if (COMPARAISON == Racine->Type && NULL != Racine->Fils3.Fils->Fils2.Fils)
		TypeInstruction = COMPARAISON;
	else if (INSTRUCTION == Racine->Type)
	{

		OperandeG->TypeOperande = OperandeD->TypeOperande = -1;

		TypeInstruction = -1;

	} // INSTRUCTION
	if (FACTEUR == Racine->Type || APPELPROC == Racine->Type)
	{
		int TypeVar;

		int NbParamAVerifTmp = NbParamAVerif;
		int NbParamVerifTmp = NbParamVerif;
		int NbParamLuTmp = NbArgLu;
		char *NomFoncTmp = NomFonctionAVerif;

		NbParamVerif = NbParamAVerif = -1;

		if (TYPE_NOM == Racine->TypeF1)
			TypeVar = VerifierExistenceVariable (Courant->TableSymbole, Racine->Fils1.Nom);

		if (-1 != NbParamAVerif)
		{
			/* Je suis sur l'appel d'une fonction */

			/* Je vérifie le nom de la fonction en cours si elle existe... */
			if (-1 != NbParamAVerifTmp)
			{
				VerifierParametre (NomFoncTmp, TypeVar);
				/* On modifie les nb param vérifier */
				/* comme on a vérifier la fonction dans laquelle on était */
				/* et non la fonction imbriquée dans laquelle on est */
				++NbParamVerifTmp;
				--NbParamVerif;
			}

			/* Je pars à la recherche des paramètres */
			CreationTableSymbole (Racine->Fils2.Fils, Courant, NomFonctionCourante);


			if (NbParamVerif > NbParamAVerif)
			{
				printf ("Error: Too many arguments for %s.\n", NomFonctionAVerif);
				exit (1);
			}
			else if (NbParamVerif < NbParamAVerif)
			{
				printf ("Error: Too few arguments for %s.\n", NomFonctionAVerif);
				exit (1);
			}

			NbParamAVerif = NbParamAVerifTmp;
			NbParamVerif = NbParamVerifTmp;
			NbArgLu = NbParamLuTmp;
			NomFonctionAVerif = NomFoncTmp;

			return;


		}

		NbParamAVerif = NbParamAVerifTmp;
		NbParamVerif = NbParamVerifTmp;
		NbArgLu = NbParamLuTmp;
		NomFonctionAVerif = NomFoncTmp;

	} // FACTEUR || APPELPROC

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
							TypeVariable, -1, IndiceDebut, IndiceFin, Adresse, NomFonctionCourante);
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
			int AdresseTmp = Adresse;
			Adresse = 0;

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
					IndiceDebut, IndiceFin, Adresse, NomFonctionCourante);

			TableCourante = TableCourante->SuivantElement;


			int NbParametre = 0;
			SPile *PileTmp;
			if (NULL != NoeudCourant->Fils2.Fils->Fils1.Nom)
			{
				STableSymbole *TStmp, *PremiereTablePile;
				TStmp = (STableSymbole *) malloc (sizeof (STableSymbole));


				for (PileTmp = Courant; ; PileTmp = PileTmp->Suivant)
					if (NULL == PileTmp->Suivant)
					{
						PileTmp->Suivant = AjoutTableSymboleSurPile (TStmp);
						PileTmp = PileTmp->Suivant;
						break;
					}

				free (TStmp);

				TStmp = PremiereTablePile = PileTmp->TableSymbole;

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
									TypeVariable, -1, IndiceDebut, IndiceFin, Adresse, NomFonction);
							TStmp = TStmp->SuivantElement;
						}
						else

							TStmp = AjoutElementTableSymbole (ParamMemeType->Fils1.Nom, TypeVariable,
									-1, IndiceDebut, IndiceFin, Adresse, NomFonction);

						Adresse += 4;


						if (NULL == ParamMemeType->Fils2.Frere)
							break;

					}

					if (NULL == tmp->Fils4.Frere)
						break;
				}

			}

			TableCourante->NbParametre = NbParametre;

			if (DECLFUNC == NoeudCourant->Type)
				CreationTableSymbole (NoeudCourant->Fils4.Fils, PileTmp, NomFonction);
			else
				CreationTableSymbole (NoeudCourant->Fils3.Fils, PileTmp, NomFonction);

			Adresse = AdresseTmp;

		} // DECLFUNC || DECLPROC
		else
			CreationTableSymbole (Racine->Fils1.Fils, Courant, NomFonctionCourante);

	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF1)
	{
		NoeudCourant = Racine->Fils1.Frere;
		CreationTableSymbole (Racine->Fils1.Frere, Courant, NomFonctionCourante);
	}
	else if (TYPE_INT == Racine->TypeF1)
	{
		if (-1 != NbParamAVerif && FACTEUR == Racine->Type)
			VerifierParametre (NomFonctionAVerif, INTEGER);

		if (FACTEUR == Racine->Type && AFF == TypeInstruction && -1 == NbParamAVerif)
		{
			if (1 == IsTableau &&
					(Racine->Fils1.Nombre < IndiceDebGlob ||
					 Racine->Fils1.Nombre > IndiceFinGlob))
			{
				printf ("Error: %d is not between %d and %d.\n", Racine->Fils1.Nombre,
						IndiceDebGlob, IndiceFinGlob);
				exit (1);
			}

			if (0 == IsTableau)
				if (-1 == OperandeG->TypeOperande)
				{
					OperandeG->TypeOperande = INTEGER;
					OperandeG->IDOperande.Nombre = Racine->Fils1.Nombre;
					OperandeG->TypeID = TYPE_INT;
				}
				else if (-1 == OperandeD->TypeOperande)
				{
					OperandeD->TypeOperande = INTEGER;
					OperandeD->IDOperande.Nombre = Racine->Fils1.Nombre;
					OperandeD->TypeID = TYPE_INT;

					if (OperandeG->TypeOperande != OperandeD->TypeOperande)
						ErreurTypes (OperandeG, OperandeD);
					OperandeD->TypeOperande = -1;
				}
		}
	}
	else if (TYPE_NOM == Racine->TypeF1)
	{
		int TypeVar = VerifierExistenceVariable (Courant->TableSymbole, Racine->Fils1.Nom);

		if (-1 != NbParamAVerif)
			VerifierParametre (NomFonctionAVerif, TypeVar);

		if (AFF == TypeInstruction && -1 == NbParamAVerif)
			if (-1 == OperandeG->TypeOperande)
			{
				OperandeG->TypeOperande = TypeVar;
				OperandeG->IDOperande.Nom = Racine->Fils1.Nom;
				OperandeG->TypeID = TYPE_NOM;
			}
			else if (-1 == OperandeD->TypeOperande)
			{
				OperandeD->TypeOperande = TypeVar;
				OperandeD->IDOperande.Nom = Racine->Fils1.Nom;
				OperandeD->TypeID = TYPE_NOM;

				if (OperandeG->TypeOperande != OperandeD->TypeOperande)
					ErreurTypes (OperandeG, OperandeD);
				OperandeD->TypeOperande = -1;
			}

	}


	if (TYPE_SNOEUDFILS == Racine->TypeF2)
	{
		NoeudCourant = Racine->Fils2.Fils;
		CreationTableSymbole (Racine->Fils2.Fils, Courant, NomFonctionCourante);

	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF2)
	{
		NoeudCourant = Racine->Fils2.Frere;
		CreationTableSymbole (Racine->Fils2.Frere, Courant, NomFonctionCourante);
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
		CreationTableSymbole (Racine->Fils3.Fils, Courant, NomFonctionCourante);

	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF3)
	{
		NoeudCourant = Racine->Fils3.Frere;
		CreationTableSymbole (Racine->Fils3.Frere, Courant, NomFonctionCourante);
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
		CreationTableSymbole (Racine->Fils4.Fils, Courant, NomFonctionCourante);

	}
	else if (TYPE_SNOEUDFRERE == Racine->TypeF4)
	{
		NoeudCourant = Racine->Fils4.Frere;
		CreationTableSymbole (Racine->Fils4.Frere, Courant, NomFonctionCourante);
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

	OperandeG = (SOperande *) malloc (sizeof (SOperande));
	OperandeD = (SOperande *) malloc (sizeof (SOperande));
	NbParamVerif = NbParamAVerif = -1;

	CreationTableSymbole (Racine, Pile, Racine->Fils1.Nom);

	free (OperandeG);
	free (OperandeD);

	VerifierUtilisationVariables (Pile);

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
