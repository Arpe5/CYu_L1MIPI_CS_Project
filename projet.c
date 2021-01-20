#include <stdio.h>
#include <stdlib.h>
#include <time.h> //On aura besoin pour un certains nombre de trucs en fonction de l'heure actuelle de l'ordinateur.

void init_carte(int carte[20][20]);
void affiche_carte(int carte[20][20],int position_personnage[2]);
int deplace_personnage(int carte[20][20],int position_personnage[2]);

int verifier_carte(int carte[20][20],int position_personnage[2]); //On a une génération aléatoire. Il faut vérifier que les placements ne sont pas absurdes!
int reguler_carte(int carte[20][20]); //on étudie la population de la carte.
int deplace_monstre(int carte[20][20],int position_personnage[2],int case_avant); //Monstre intelligent qui se déplace vers le joueur.
int donner_un_chiffre_aleatoire();

int main()
{
    system("chcp 65001");
    //Pour le binôme, si on te pose la question, dit: <<C'est selon le standard ANSI>>
    char rouge[8] = "\033[31m";
    char vert[8] = "\033[32m";
    char jaune[8] = "\033[33m";
    char blanc[8] = "\033[37m";
    srand(time(NULL)); //Initialiser le générateur aléatoire de nombre en fonction du TEMPS.

    int carte[20][20];
    int carte_valide = 0;
    int carte_satisfiable;
    int position_personnage[2];
    int case_avant; //Ne sert uniquement que pour le monstre pour savoir s'il y avait une case dans le monstre avant de se déplacer.
    int nombre_de_tours = 0; //On fonctionne par tour dans ce jeu. On le veut surtout cette variable pour notre monstre intelligent.
    int case_actuelle;

    int vie = 10;
    int piece_or = 0;

    case_avant = 100; //Préparons une valeur absurde pour le monstre au cas où il serait en bord de carte. JUSTE AU CAS OU.
    position_personnage[0] = donner_un_chiffre_aleatoire();
    position_personnage[1] = donner_un_chiffre_aleatoire();

    do
    {
        init_carte(carte);
        carte_satisfiable = reguler_carte(carte);
        carte_valide = verifier_carte(carte,position_personnage);
    } while (!carte_valide || !carte_satisfiable);

    printf("%sBienvenue dans le jeu!%s\n",vert,blanc);
    printf("Utilisez respectivement les touches 8,4,6 et 2 pour aller en\n");
    printf("haut, en bas, à gauche et à droite. %sVous êtes une croix rouge au point (%d,%d)%s\n",jaune,position_personnage[0],position_personnage[1],blanc);
    affiche_carte(carte,position_personnage); //On se souvient bien sûr du problème. Donc on le sort de la boucle pour le début.
    while ((case_actuelle != -1) && (vie > 0 && piece_or < 10)) //Soit il quitte, soit il a perdu, soit il a gagné.
    {
        case_actuelle = deplace_personnage(carte,position_personnage);
        affiche_carte(carte,position_personnage);
        switch (case_actuelle)
        {
            case 0: printf("Vous marchez sur %sl'herbe%s\n",vert,blanc);
                    break;
            case 1: printf("Vous marchez sur une %sfleur%s\n",rouge,blanc);
                    break;
            case 4: printf("Vous marchez sur une %sclef%s\n",jaune,blanc);
                    break;
            case 5: printf("Vous récupérez une %spièce d'or%s\n",jaune,blanc);
                    piece_or++;
                    carte[position_personnage[0]][position_personnage[1]] = 0;
                    break;
            case 6: printf("Vous marchez sur un %scadena%s\n",jaune,blanc);
                    break;
            case 7: printf("VOUS AVEZ MARCHE SUR UN %sPIEGE%s!! Vous perdez %s1 %svie%s\n",rouge,blanc,rouge,vert,blanc);
                    vie--;
                    carte[position_personnage[0]][position_personnage[1]] = 0;
                    break;
            case 8: printf("LE MONSTRE VOUS %sRATTRAPE%s!! Vous perdez %s1 %svie%s\n",rouge,blanc,rouge,vert,blanc);
                    vie--;
                    break;
            case -1: printf("Fin du jeu.\n");
                     break;

            //On en a maintenant finit avec les règles de base de dntt. On fait nos propres règles ici pour <<nos petits problèmes>>.
            case 9: printf("%sVous butez sur la frontière de la carte!%s\n",rouge,blanc);
                    break;
            case 10: printf("%sVous essayez de traverser l'%sarbre%s mais l'arbre ne veut pas??%s\n",rouge,vert,rouge,blanc);
                     break;
            case 11: printf("C'est un %srocher%s!\n",rouge,blanc);
                     break;
        }
        printf("%sPV: %s%d\t%sPièces d'or récupérés: %s%d\n",rouge,blanc,vie,jaune,blanc,piece_or);

        nombre_de_tours++;
        if (nombre_de_tours%1 == 0) //Tous les 2 tours on déplace le monstre vers le joueur.
        {
            case_avant = deplace_monstre(carte,position_personnage,case_avant);
            if (carte[position_personnage[0]][position_personnage[1]] == 8)
            {
                printf("LE MONSTRE EST SUR %sVOUS%s!! Vous perdez %s1 %svie%s\n",rouge,blanc,rouge,vert,blanc);
                vie--;
            }
        }
    }
    
    //Le jeu s'est terminé. Que s'est-il passé?
    if (piece_or == 10)
    {
        printf("%sVous avez pu récupéré %s10 pièces d'or! %sVous avez gagné!\n",vert,jaune,blanc);
    }
    else if (vie == 0)
    {
        printf("%sVous avez perdu. Mais vous pouvez reprendre votre revenge.%s\n",rouge,blanc);
        printf("%sFaîtes attention au %smonstre%s qui vous suit dès le départ.\n",rouge,jaune,blanc);
    }
    printf("Merci d'avoir joué à notre jeu. Le jeu fut le travail d'équipe entre:\n");
    printf("%sArpe5\t%sL1MIPI B2\t%sArpe5@GitHub%s\n",rouge,jaune,vert,blanc);
    printf("%sANONYME\t%sL1MIPI A2\t%sanonymous@CYu%s\n",rouge,jaune,vert,blanc);
    printf("Revenez autant de fois que vous voulez.\n");

    return 0;
}

void init_carte(int carte[20][20])
{
    int il_existe_un_unique_monstre = 0; //On ne veut que un monstre pour du 20x20, c'est très sufffisant à l'aide d'une intelligence <<artificielle>>.

    for (int i=0;i<20;i++)
    {
        for (int j=0;j<20;j++)
        {
            int chiffre = donner_un_chiffre_aleatoire();
            if (chiffre == 8 && il_existe_un_unique_monstre)
            {
                do
                {
                    chiffre = donner_un_chiffre_aleatoire();
                    carte[i][j] = chiffre;
                } while(carte[i][j] == 8);
            }
            else if (chiffre == 8)
            {
                carte[i][j] = 8;
                il_existe_un_unique_monstre = 1;
            }
            else
            {
                carte[i][j] = chiffre;
            }
        }
    }
}

void affiche_carte(int carte[20][20],int position_personnage[2])
{
    //Pour le binôme, si on te pose la question, dit: <<C'est selon le standard ANSI>>
    char rouge[] = "\033[31m";
    char vert[] = "\033[32m";
    char jaune[] = "\033[33m";
    char blanc[] = "\033[37m";
    //Pour le binôme, si on te pose la question, dit: <<On ne connaît pas la taille de X donc on laisse le compilateur se charger de ceci>>
    char herbe[] = "^ ";     //0
    char fleur[] = "🌷";     //1
    char arbre[] = "🌲";     //2, obstacle
    char rocher[] = "🕳 ";   //3, obstacle
    char clef[] = "🔑";      //4, objet
    char piece_or[] = "💰";  //5, objet
    char cadena[] = "🔒";    //6, objet
    char piege[] = "☐ ";     //7
    char monstre[] = "♞ ";   //8
    
    for (int y=0;y<20;y++)
    {
        for (int x=0;x<20;x++)
        {
            if (position_personnage[0] == x && position_personnage[1] == y)
            {
                printf("❌");
            }
            else
            {
                switch (carte[x][y]) //On se souvient de la fonction usuelle switch(x) où x est un ENTIER. Or, carte[x][y] est un ENTIER. Donc.
                {
                    case 0: printf("%s%s%s",vert,herbe,blanc);
                            break;
                    case 1: printf("%s",fleur);
                            break;
                    case 2: printf("%s",arbre);
                            break;
                    case 3: printf("%s",rocher);
                            break;
                    case 4: printf("%s",clef);
                            break;
                    case 5: printf("%s",piece_or);
                            break;
                    case 6: printf("%s",cadena);
                            break;
                    case 7: printf("%s",piege);
                            break;
                    case 8: printf("%s%s%s",rouge,monstre,blanc);
                            break;
                    //Il n'y a de default ici car il s'agit d'une carte créée par nos soins donc il n'y a pas d'absurdité dans les valeurs des cases.
                }
            }
        }
        printf("\n");
    }
}

int deplace_personnage(int carte[20][20],int position_personnage[2])
{
    int deplacement;
    scanf("%d",&deplacement);

	switch (deplacement)
	{
	    case 6: printf("Vous allez à droite.\n");
	    		position_personnage[0]=position_personnage[0]+1;
	    		if (position_personnage[0] > 19)
	    		{
	    			position_personnage[0]=position_personnage[0]-1;
                    return 9;
	    		}
                else if (carte[position_personnage[0]][position_personnage[1]] == 2)
                {
                    position_personnage[0]=position_personnage[0]-1;
                    return 10;
                }
                else if (carte[position_personnage[0]][position_personnage[1]] == 3)
                {
                    position_personnage[0]=position_personnage[0]-1;
                    return 11;
                }
                else
                {
                    return carte[position_personnage[0]][position_personnage[1]];
                }
	    		break;
	    case 4: printf("Vous allez à gauche.\n");
	    		position_personnage[0]=position_personnage[0]-1;
	    		if (position_personnage[0] < 0)
	    		{
	    			position_personnage[0]=position_personnage[0]+1;
                    return 9;
	    		}
                else if (carte[position_personnage[0]][position_personnage[1]] == 2)
                {
                    position_personnage[0]=position_personnage[0]+1;
                    return 10;
                }
                else if (carte[position_personnage[0]][position_personnage[1]] == 3)
                {
                    position_personnage[0]=position_personnage[0]+1;
                    return 11;
                }
                else
                {
                    return carte[position_personnage[0]][position_personnage[1]];
                }
	    		break;
	    case 8: printf("Vous allez en haut.\n");
	    		position_personnage[1]=position_personnage[1]-1;
	    		if (position_personnage[1] < 0)
	    		{
	    			position_personnage[1]=position_personnage[1]+1;
                    return 9;
	    		}
                else if (carte[position_personnage[0]][position_personnage[1]] == 2)
                {
                    position_personnage[1]=position_personnage[1]+1;
                    return 10;
                }
                else if (carte[position_personnage[0]][position_personnage[1]] == 3)
                {
                    position_personnage[1]=position_personnage[1]+1;
                    return 11;
                }
                else
                {
                    return carte[position_personnage[0]][position_personnage[1]];
                }
	    		break;
	    case 2: printf("Vous allez en bas.\n");
	    		position_personnage[1]=position_personnage[1]+1;
	    		if (position_personnage[1] > 19)
	    		{
	    			position_personnage[1]=position_personnage[1]-1;
                    return 9;
	    		}
                else if (carte[position_personnage[0]][position_personnage[1]] == 2)
                {
                    position_personnage[1]=position_personnage[1]-1;
                    return 10;
                }
                else if (carte[position_personnage[0]][position_personnage[1]] == 3)
                {
                    position_personnage[1]=position_personnage[1]-1;
                    return 11;
                }
                else
                {
                    return carte[position_personnage[0]][position_personnage[1]];
                }
	    		break;
        case 0: return -1;
                break;
	    default: printf("Erreur de saisie, le personnage ne bouge pas!\n");
                 break;
	}
}

int verifier_carte(int carte[20][20],int position_personnage[2])
{
    //On fait 20-1 car on regarde à n+1 les cases pour les obstacles.
    for (int i=0;i<20-1;i++)
    {
        for (int j=0;j<20-1;j++)
        {
            //Soit Personnage(x,y) avec (x,y) un couple d'entiers, si i+1 et j+1 sont des obstacles. L'Hypothèse est suffisante pour savoir s'il va être coincé.
            if ((position_personnage[0] == i && position_personnage[1] == j) && (carte[i][j+1] == 2 || carte[i][j+1] == 3) && (carte[i+1][j] == 2 || carte[i+1][j] == 3))
            {
                return 0;
            }
            else
            {
                return 1;
            }
            
        }
    }
}

int reguler_carte(int carte[20][20])
{
    //Règle de calcul pour la population: Ne pas dépasser 15%!
    int population_cadenas = 15; //Ne prendre que 20% de cadenas dans la carte.
    int population_pieces_or = 15; //Ne prendre que 20% de pièces d'or dans la carte.

    int nombre_cadenas = 0;
    int nombre_clefs = 0;
    int nombre_pieces_or = 0;

    for (int i=0;i<20;i++)
    {
        for (int j=0;j<20;j++)
        {
            if (carte[i][j] == 6)
            {
                nombre_cadenas++;
            }
            else if (carte[i][j] == 4)
            {
                nombre_clefs++;
            }
            else if (carte[i][j] == 5)
            {
                nombre_pieces_or++;
            }
        }
    }

    if ((nombre_cadenas/400.0*100 < population_cadenas) || (nombre_pieces_or/400.0*100 < population_pieces_or)) //cf. Problème de division dans le TD de dntt.
    {
        return 0; //La carte n'est pas satisfiable. On recommence tout depuis le début.
    }

    for (int i=0;i<20;i++)
    {
        for (int j=0;j<20;j++)
        {
            if ((nombre_cadenas/20*100 > population_cadenas) && (carte[i][j] == 6))
            {
                carte[i][j] = 0;
                nombre_cadenas--;
            }
            else if ((nombre_pieces_or/20*100 > population_pieces_or) && (carte[i][j] == 5))
            {
                carte[i][j] = 0;
                nombre_pieces_or--;
            }
        }
    }

    //Correction des absurdités dûs aux changements précédents.
    for (int i=0;i<20;i++)
    {
        for (int j=0;j<20;j++)
        {
            if ((nombre_cadenas > nombre_clefs) && (carte[i][j] == 6)) //Il y a plus de cadenas que de clefs. Il faut corriger ça.
            {
                carte[i][j] = 0;
                nombre_cadenas--;
            }
            else if ((nombre_clefs > nombre_cadenas) && (carte[i][j] == 4)) //Même chose.
            {
                carte[i][j] = 0;
                nombre_clefs--;
            }
        }
    }

    return 1;
}

int deplace_monstre(int carte[20][20],int position_personnage[2],int case_avant)
{
    int case_apres[3];
    int position_monstre[2];
    int case_choisi[2];

    for (int y=0;y<20;y++)
    {
        for (int x=0;x<20;x++)
        {
            if (carte[x][y] == 8)
            {
                position_monstre[0] = x;
                position_monstre[1] = y;
            }
        }
    }

    if (position_personnage[0] > position_monstre[0]) //Il est à droite.
    {
        case_choisi[0] = position_monstre[0]+1;
        case_choisi[1] = position_monstre[1]; //On ne bouge pas en ordonnée.
    }
    else if (position_personnage[0] < position_monstre[0]) //Il est à gauche.
    {
        case_choisi[0] = position_monstre[0]-1;
        case_choisi[1] = position_monstre[1];
    }
    else if (position_personnage[1] > position_monstre[1]) //Il est en haut.
    {
        case_choisi[0] = position_monstre[0]; //On ne bouge pas en abscisse.
        case_choisi[1] = position_monstre[1]+1;
    }
    else //Il est en bas.
    {
        case_choisi[0] = position_monstre[0];
        case_choisi[1] = position_monstre[1]-1;
    }

    //On met en mémoire la case qui va être modifier.
    case_apres[0] = case_choisi[0];
    case_apres[1] = case_choisi[1];
    case_apres[2] = carte[case_apres[0]][case_apres[1]]; //On récupère ce qu'il y a dans la case.
    //Il se peut que le monstre soit au bord de la carte au début du jeu. Vérifions que c'est le cas.
    if ((case_apres[0] == position_monstre[0]+1) && (position_monstre[0] == 0)) //Il n'y a rien à gauche. C'est pas bon signe!
    {
        case_apres[2] = 100; //Valeur absurde pour dire qu'on n'a pas touché la case avant le monstre.
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0; //Supposons que il y avait de l'herbe avant.
    }
    else if ((case_apres[0] == position_monstre[0]-1) && (position_monstre[0] == 19)) //Il n'y a rien à droite. C'est pas bon signe!
    {
        case_apres[2] = 100; //Valeur absurde pour dire qu'on n'a pas touché la case avant le monstre.
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0; //Supposons que il y avait de l'herbe avant.
    }
    else if ((case_apres[1] == position_monstre[1]-1) && (position_monstre[1] == 19)) //Il n'y a rien en bas. C'est pas bon signe!
    {
        case_apres[2] = 100;
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0; //Supposons que il y avait de l'herbe avant.
    }
    else if ((case_apres[1] == position_monstre[1]+1) && (position_monstre[1] == 0)) //Il n'y a rien en haut. C'est pas bon signe!
    {
        case_apres[2] = 100;
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0; //Supposons que il y avait de l'herbe avant.
    }
    else //Le monstre n'est pas au bord d'une carte. On peut utiliser la variable case_avant sans problème.
    {
        carte[case_choisi[0]][case_choisi[1]] = 8; //On bouge le monstre.
        if (case_avant == 100) //C'est la première fois que le monstre bouge.
        {
            carte[position_monstre[0]][position_monstre[1]] = 0; //On suppose aussi que il y avait de l'herbe avant son déplacement.
        }
        else //Non. Il s'est déjà déplacé auparavant donc on connâit ce qu'il y avait sur la carte.
        {
            carte[position_monstre[0]][position_monstre[1]] = case_avant; //On remet ce qu'il y avait grâce à la variable case_avant qu'on a mis en mémoire.
        }
    }
    
    //On enregistre les modifications pour plus tard, notemment pour case_avant.
    return case_apres[2];
}

int donner_un_chiffre_aleatoire()
{
    return rand()%9;
}