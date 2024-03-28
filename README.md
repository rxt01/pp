## Définitions
- Pour eviter de la répétition inutile, un fichier fait références, dans le présent document, a un fichier électronique ou a un message

- Chiffrer un fichier est l'acte de rendre le contenu du fichier incompréhensible pour ceux ne connaissant pas le secret permettant de le comprendre  ( dans ce cas ci, un mot de passe).
- Déchiffrer un fichier est l'acte de rendre compréhensible le contenu du fichier, de renverser les effets du chiffrage.
- Un mot de passe est une série de caractères qui n'est connue que de ceux pouvant accéder au contenu du fichier.
- Un bit est la plus petite partie de toute chose informatiques, ils peuvent prendre les valeurs de 0  ou 1.
- L'index d'un élément est sa position dans une suite d'éléments.
## Présentation de l'application
Cette apllication sert a chiffrer des fichiers.

Elle a été réalisée dans le cadre du projet personnel de l'IB.

## Fonctionnement de l'application
### Traitement des fichiers
L'application prend un fichier original ainsi qu'un mot de passe.

Elle sépare ensuite le fichier en plusieurs blocs chacun de la taille du mot de passe.

### Chiffrement de chaque bloc
Pour chaque bloc, elle prend chaque bit du bloc original et le compare avec le bit du mot de passe ayant le même index.

Elle met dans le bloc chiffré un bit selon la relation suivante:
    
        0 si les bits sont les mêmes 
        1 si les bits ne sont pas les mêmes 

Dans le code cela est réaliser par le morceau de code suivant:
```
tmp.push_back(original[i] ^ key[i])
``` 
Où le `tmp` fait référence au bloc chiffré, `original[i]` le bit du fichier original, `key[i]` le bit du mot de passe et `^` la relation expliquée plus haut.

Pour les bloc suivant le premier, le bloc chiffré du bloc le précédant remplace le mot de passe.

L'opération est ensuite répété avec le mot de passe.
![diagram chiffrage](raw.githubusercontent.com/rxt01/pp/main/Capture%20d'écran%202024-03-28%20103225.png)
### Déchiffrage de chaque bloc
Le déchiffrage est très similaire au chiffrage.

On commence par la meme opération que pour le chiffrage en utilisant le mot de passe.

L'opération est ensuite répéter avec le bloc original du dernier bloc pour obtenir le bloc déchiffrer.

Dans le cas du premier bloc, on utilise le mot de passe lors de la deuxième réalisation  de l'opération.
### Recombination
Lorsque tous les blocs ont été chiffrées ou déchiffrées, ils sont tous recombinés en un seul fichier.
