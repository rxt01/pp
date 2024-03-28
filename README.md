## Definitions
- Pour eviter de la repetition inutile, un fichier fais references, dans le present document, a un fichier electronique ou a un message

- Chiffrer un fichier est l'acte de rendre le contenu du fichier incomprehensible pour ceux ne conaissant pas le secret permetant de le  ( dans ce cas ci, un mot de passe).
- Dechiffrer un fichier est l'acte de rendre comprehensible le contenu du fichier, de renverser les effets du chiffrage.
- Un mot de passe est une serie de charactere qui n'est connu que de ceux pouvant acceder au contenu du fichier.
- Un bit est la plus petite partie de toute chose informatique, ils peuvent prendre les valeurs de 0  ou 1.
## Presentation de l'application
Cette apllication sert a chiffrer des fichiers.

Elle a ete realisee dans le cadre du projet personnel de l'IB.

## Foncitonnement de l'application
### Traitement des fichiers
L'application prend un fichier original ainsi qu'un mot de passe.

Elle separe ensuite ce fichier en plusieurs bloc chacun de la taille du mot de passe.

### Chifrement de chaque bloc
Pour chaque bloc, elle prend chaque bit du bloc original et le compare avec  le bit du mot de passe ayant le meme index.

Elle met dans le bloc chifrer un bit selon la relation suivante:
    
        0 si les bits sont les memes
        1 si les bits ne sont pas les memes

Dans le code cela est realiser par le bout suivant:
```
tmp.push_back(original[i] ^ key[i])
``` 
Ou le `tmp` fait reference au bloc chiffrer, `original[i]` le bit du fichier original, `key[i]` le bit du mot de passe et `^` la relation expliquer plus haut.

Pour les bloc suivant le premier, le bloc chifrer du bloc precedent remplace le mot de passe.

L'operation est ensuite repete avec le mot de passe.

### Dechifrage de chaque bloc
Le dechifrage est tres similaire au chifrage.

On commence par la meme operation que pour le chifrage en utilisant le mot de passe.

L'operation est ensuit repeter avec le bloc original du dernier bloc pour obtenir le bloc dechifrer.

Dans le cas du premier bloc, on utilise le mot de passe lors de la deuxieme realisation  de l'operation.
### Recombination
Lorse que tout les blocs ont ete chifrees ou dechifrees, ils sont tous recombiner en un seul fichier