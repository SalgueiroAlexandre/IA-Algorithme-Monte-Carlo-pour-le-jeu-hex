# Titre : Bot IA pour le jeu d'Hex #

## Introduction : ##
Ce projet consiste en un bot IA pour le jeu d'Hex, développé en C++ et utilisant la méthode de Monte Carlo 
pour l'apprentissage et l'amélioration du bot. Le jeu d'Hex est un jeu de stratégie à deux joueurs, où 
l'objectif est de relier deux côtés opposés du plateau avec une chaîne ininterrompue de pions.

## Description du jeu : ##
Le jeu d'Hex est un jeu de stratégie à deux joueurs, où les joueurs placent alternativement des pions de 
leur couleur sur un plateau hexagonal. L'objectif est de relier deux côtés opposés du plateau avec une
chaîne ininterrompue de pions, tandis que les joueurs peuvent également bloquer le chemin de leur 
adversaire en plaçant des pions sur le plateau. Le jeu est souvent considéré comme un casse-tête, 
car les joueurs doivent trouver des stratégies créatives pour bloquer leur adversaire et créer des
opportunités pour eux-mêmes. Il a également été utilisé dans des études scientifiques pour explorer
les stratégies cognitives et les capacités de raisonnement.

## Fonctionnalités du projet : ##
Le projet inclut un arbitre d'entraînement permettant de faire jouer le bot contre lui-même, contre un joueur
aléatoire ou contre un joueur manuel. De plus, un arbitre normal est disponible pour jouer contre le bot.
L'algorithme de Monte Carlo est utilisé pour simuler plusieurs parties avec différents coups possibles, 
afin de déterminer le meilleur coup à jouer.

## Installation : ##
Pour ouvrir ce projet, il est nécessaire de créer un dossier "build" en utilisant la commande suivante dans
le terminal :

```
mkdir build
```

Ensuite, allez dans le dossier build et entrez la commande suivante pour configurer CMake :

```
cd build
cmake ..
```

Enfin, le jeu peut être lancé en tapant la commande suivante :

```
./Arene_light
```

## Utilisation : ##
Le fichier ***main.cpp*** contient le code principal pour le jeu. Les paramètres du jeu peuvent être modifiés 
en ajustant les valeurs des variables telles que "nb_parties" pour déterminer le nombre de parties jouées 
par l'arbitre d'entraînement. Les joueurs disponibles sont les joueurs aléatoires, manuels et "guezmer". 
Le comportement de l'IA peut également être modifié en ajustant les paramètres de l'algorithme de Monte Carlo.

Fonctionnement de l'algorithme de Monte Carlo :
La méthode de Monte Carlo est utilisée pour simuler plusieurs parties avec différents coups possibles, 
afin de déterminer le meilleur coup à jouer. En simulant des parties de manière aléatoire, l'algorithme 
de Monte Carlo peut fournir une estimation du gain attendu pour chaque coup possible, permettant ainsi au
bot de prendre la meilleure décision possible.

## Fichier Guezmer : ##
La méthode ***initMoves()*** permet d'initialiser la table de hachage "movesMap", qui stocke les coups connus par
l'IA avec leur score et le nombre de fois où ils ont été joués. Cette table est remplie à partir d'un fichier 
CSV nommé "coups.csv" à l'aide d'un objet Lecteur. Chaque ligne de ce fichier correspond à un coup avec son 
score et le nombre de parties où il a été joué.

La méthode ***recherche_coup()*** utilise l'algorithme Monte Carlo Tree Search pour trouver le meilleur coup à 
jouer pour l'IA. Elle commence par mettre à jour l'état de la partie avec la méthode "majEtatPartie()", qui 
récupère la grille de jeu et ajoute le dernier coup joué. Ensuite, elle récupère la liste des coups possibles 
à partir de la grille de jeu et vérifie s'ils sont tous connus par l'IA. Si ce n'est pas le cas, elle choisit 
un coup qui lie les deux extrémités de la grille pour éviter de jouer un coup aléatoire parmi les coups 
inconnus avec la méthode "choisirCoupNonConnu()". Si tous les coups sont connus, elle sélectionne le 
meilleur coup à jouer en utilisant la formule QUBC (Upper Confidence Bound applied to Trees), qui 
prend en compte le score et le nombre de parties jouées pour chaque coup.

La méthode ***rollback()***  est utilisée pour mettre à jour les scores et le nombre de parties jouées pour 
chaque coup lorsque l'IA doit annuler un coup précédemment joué en raison d'une erreur ou d'un rollback 
du jeu. Elle récupère l'état de la partie précédent le coup annulé, puis parcourt chaque coup enregistré 
depuis cet état en ordre inverse, en mettant à jour le score et le nombre de parties jouées pour chaque 
coup dans la table de hachage "movesMap".

La méthode ***coupEstConnu()*** est utilisée pour vérifier si un coup donné est déjà connu par l'IA.
Elle construit une chaîne de caractères unique pour chaque coup en concaténant les indices de sa ligne 
et de sa colonne. Cette chaîne est ensuite utilisée comme clé pour rechercher le coup dans la table de 
hachage "movesMap".

La méthode ***nbPartiePere()*** est utilisée pour récupérer le nombre de parties jouées depuis le coup précédent 
dans l'arbre de recherche. Elle prend en entrée l'identifiant du coup et retourne le nombre de parties jouées 
depuis le coup précédent (s'il y en a un) ou le nombre total de coups connus par l'IA (s'il n'y a pas de coup 
précédent).

Enfin, la méthode ***qubc()*** calcule la formule QUBC pour un coup donné. La formule QUBC est une formule 
utilisée pour l'exploration d'un arbre de recherche. Elle combine le score moyen d'un coup (score) avec 
son taux de succès (nombre de parties gagnées) et le nombre de parties totales jouées pour obtenir un 
score qui représente la confiance de l'IA dans le coup. La formule est :

***QUBC = score + C * racine_carree(ln(N) / N')***

Où :

***score*** est le score moyen du coup,
***N*** est le nombre total

## Auteurs : ##
Ce projet a été développé par Thomas Landais, Alexandre Salgueiro et Ahmad Houhou dans le cadre d'un projet 
universitaire.
