P2N64 - Pascal To Nintendo 64
==

Résumé :
Ce programme réalisé dans le cadre du cours de compilation va lire un programme en Pascal pour ressortir du code assembleur MIPS.
Il va donc effectuer une analyse lexical, syntaxique et sémantique avant de générer du code assembleur.


HowTo:
Pour compiler le programme, il suffit d'être dans le dossier dirfile du projet puis de faire "make".
Pour l'exécuter, il faut taper "./P2N64.run" suivi du nom du programme pascal en premier et seul argument.
"make clean" va supprimer le fichier créer lors de la compilation.


Description :
Le programme va dans un premier temps faire une analyse lexical du programme et récupérer les lexeme lus.

Par la suite une analyse syntaxique va être faite en même temps que la création de l'arbre syntaxique.

À partir de celui-ci, on va créer la table des symboles et vérifier que chaque variable est déclarée,
que les types corresondent lors d'une affectation ou pour les arguments passés dans une fonction...


Implémentation :
L'analyse lexical et syntaxique se font à partir d'une grammaire simplifiée (pas de for par exemple)
donnée en cours.

Pour la création de l'arbre, une struct pour créer chaque noeud à été faite. Cette struct permet de créer
jusqu'à 4 fils de type différent grâce à une union (un nom, un nombre ou un nouveau noeud (fils ou frère))

Afin de savoir quel type de fils je crée parmi les 4, j'ai ajouté pour chaque fils une variable dans
laquelle je met une constante (TYPE_INT, TYPE_NOM, TYPE_SNOEUDFRERE, TYPE_SNOEUDFILS) qui me permettra
de faciliter le parcours de l'arbre.
En effet, il n'y a plus besoin de savoir dans quel fonction je suis pour savoir si je dois aller dans un
noeud ou pas, la seule vérification du type suffit.

Néanmoins, ce qui m'a simplifié le parcours de l'abre pour son affichage et pour la table des symboles
m'a pénalisé pour la génération du code assembleur dans la mesure où j'ai besoin de savoir où je suis,
par seulement le type des fils du noeud sur lequel je suis...

Pour la création de la table des symboles, le parcours n'est pas entièrement récursif. L'ajout des
symboles lors de la déclaration de variable ou de fonction/procédure se fait de manière itérative.
Le parcours de ces sous-arbres correspondants ne se fait donc pas de manière récursive. En
revanche le reste du parcours durant lequel se déroulera aussi la vérification des types, fonction...
se fait bien de manière récursive.


Notes :
Tous les affichages pour chaque étape de la création du compilateur : analyse lexical, syntaxique et
sémantique, sont en commentaire dans le code. Ainsi, seul s'affiche les messages d'erreurs ou d'alerte
si nécessaire.
