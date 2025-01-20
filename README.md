# class_db
# Margotte Storme 3SI4

Ce projet implémente une mini base de données en langage C, capable de gérer des opérations simples comme la création de tables,
la sélection de tables actives, l'insertion de données, et la recherche dans une structure en mémoire basée sur un arbre binaire. 
Le projet inclut également une persistance sur disque et un fichier pour effectuer des tests de manière simple.

Fonctionnalités principales :

Création de tables : Créez des tables pour structurer vos données avec la commande CREATE TABLE.

Changement de table active : Utilisez USE pour sélectionner la table active sur laquelle effectuer des opérations.

Insertion de données : Ajoutez des lignes dans la table active avec INSERT.

Requêtes de sélection : Récupérez des données spécifiques grâce à SELECT.

Arbre binaire : Gestion des données en mémoire avec un arbre binaire pour des performances optimales.

Persistance des données : Sauvegardez vos données sur disque pour les réutiliser après l'arrêt du programme.

Commandes supplémentaires :

.exit : Quitte la base de données.

.status : Affiche la table actuellement utilisée dans la base de données ainsi que le nombre de tables actuel.

Pour compiler le projet : make
			 ./db_project
Pour compiler le fichier de tests : gcc -o test tests.c repl.c btree.c
