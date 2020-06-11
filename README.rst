Organiseur_TReamilial - Programme de gestion du quotidien
=========================================================

Description générale
====================

Vous trouverez les sources permettant de compiler un programme de gestion des tâches quotidiennes.


Initialisation
==============

Ce programme est lié à une base de donnée locale, gérée via la librairie SQLite, qui va contenir les différentes tâches qui seront créées.
Le prorgamme se charge de créer automatiquement, s'il n'existe pas déjà, un dossier qui va contenir cette base de données.
Cependant, l'utilisateur peut tout à fait créer l'arborescence lui-même, ainsi que le fichier de base de données (en respectant la structure de celle-ci telle que définie dans les sources du code).


Ajout d'une tâche
=================

Dans la partie gauche de la fenêtre figure un bouton **"Ajouter une tâche"** qui, comme son nom l'indique, permet d'ajouter une tâche.

Le programme permet de gérer :

- des tâches **non datées** : elles apparaîtront alors dans la partie droite de la fenêtre,
- des tâches **normales**, i.e. datées :  elles apparaîtront alors dans la partie centrale/basse (dite *calendaire*) de la fenêtre.


Tâches importantes
==================

Les tâches **non datées**, ainsi que les tâches **normales**, peuvent être considérées comme importantes (case à cocher **"Tâche importante"** lors de la création de la tache).

Si tel est le cas une tâche apparaîtra alors dans la partie supérieure de la fenêtre (bandeau des *tâches importantes*).

.. hint::
    Cela vous permettra de rapidement visualiser les tâches à réaliser en priorité, celles-ci étant colorées en orange
    (dans le bandeau des *tâches importantes* ainsi que dans la partie *calendaire* de la fenêtre).


Rappels
=======

Les tâches **normales** peuvent également posséder un rappel (option disponible lors de l'ajout d'une tâche). On peut alors choisir combien de semaines avant l'occurence de la tâche on souhaite être prévenu.
Ces rappels apparaîtront en bleu dans la partie *calendaires* de la fenêtre.

Les rappels, ainsi que les tâches importantes figurant dans le bandeau des *tâches importantes*, ne sont pas des tâches à proprement parler.

.. hint::
    On peut double cliquer dessus pour ouvrir une fenêtre de dialogue qui donnera rapidement des informations sur la tâche associée.


Tâches périodiques
==================

On peut également définir des tâches **périodiques** (option disponible lors de l'ajout d'une tâche).

Si une tâche est définie comme périodique, et que l'on souhaite y ajouter un rappel, la périodicité ne pourra pas être inférieure à 2 semaines.

Qui plus est, le nombre de semaines avant la tâche (défini dans la partie concernant le rappel) ne pourra pas être supérieur ou égal à la périodicité.


Modification d'une tâche
========================

Chaque tâche, **non datée**, **normale** ou **périodique** (importante ou non), pourra être modifiée.

Pour ce faire, il suffit de cliquer sur la tâche souhaitée (dans la partie *calendaire* ou dans la partie des *tâches non datées*).
La partie gauche de la fenêtre changera alors pour afficher les options associées à la tâche sélectionnée.
Il faudra alors modifier, selon son choix, puis cliquer sur le bouton **"Modifier la tâche"**.

.. hint::
    Si l'on ne veut pas modifier la tâche sélectionnée (suite, par exemple, à une erreur de manipulation)
    il suffit de choisir entre les deux options suivantes :
    
    - cliquer sur une autre tâche (**non datée**, **normale** ou **périodique**),
    - cliquer sur la tâche déjà sélectionnée.


Validation d'une tâche
======================

On pourra également valider une tâche réalisée en cliquant dessus puis en cliquant sur le bouton **"Valider la tâche"**.


Sortir du programme
===================

Pour sortir du programme deux possibilitées :

- cliquer sur **Menu->Quitter** (combinaison de touches : **Alt+M, Q**) ;
- appuyer sur la touche **Echap**.


Spécification
=============

Le code a été développé dans avec les spécifités suivantes :

- langage de programmation : C++ (c++11)
- librairies spécifiques : Sqlite3, Boost System et Boost Filesystem (cf section **LIBS** du fichier .pro)
- IDE : Qt Creator 4.11.1
- Système d'exploitation : Ubuntu 18.04.4 LTS


Licence
=======

Le code est sous licence WTFPL_.

.. _WTFPL: https://fr.wikipedia.org/wiki/WTFPL

