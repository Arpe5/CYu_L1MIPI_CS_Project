#include <stdio.h>
#include <stdlib.h>
#include <time.h> //We'll need time.h for a couple of things based on the current time of the computer.

void init_carte(int carte[20][20]);
void affiche_carte(int carte[20][20],int position_personnage[2]);
int deplace_personnage(int carte[20][20],int position_personnage[2]);

int verifier_carte(int carte[20][20],int position_personnage[2]); //We have a random generation. We must check the placing aren't absurds!
int reguler_carte(int carte[20][20]); //We study the map population.
int deplace_monstre(int carte[20][20],int position_personnage[2],int case_avant); //Intelligent monster that moves toward the player.
int donner_un_chiffre_aleatoire();

int main()
{
    //For the peer, if they ask you the question, say: "It's according to the ANSI standard"
    char rouge[8] = "\033[31m";
    char vert[8] = "\033[32m";
    char jaune[8] = "\033[33m";
    char blanc[8] = "\033[37m";
    srand(time(NULL)); //Initialize the random number generator based on TIME.

    int carte[20][20];
    int carte_valide = 0;
    int carte_satisfiable;
    int position_personnage[2];
    int case_avant; //Only needed for the monster to know if there was a square inside the monster before moving.
    int nombre_de_tours = 0; //We work by turn in the game. We want this variable especially for our intelligent monster.
    int case_actuelle;

    int vie = 10;
    int piece_or = 0;

    case_avant = 100; //We prepare an absurd value for the monster in case where it would be on the edge of the map. JUST IN CASE.
    position_personnage[0] = donner_un_chiffre_aleatoire();
    position_personnage[1] = donner_un_chiffre_aleatoire();

    do
    {
        init_carte(carte);
        carte_satisfiable = reguler_carte(carte);
        carte_valide = verifier_carte(carte,position_personnage);
    } while (!carte_valide || !carte_satisfiable);

    printf("%sWelcome in the game!%s\n",vert,blanc);
    printf("Use respectively the keys 8,4,6 and 2 to go\n");
    printf("up, down, left and right. %sYou are a red cross at point (%d,%d)%s\n",jaune,position_personnage[0],position_personnage[1],blanc);
    affiche_carte(carte,position_personnage); //We remember the problem of course. So we pull it out of the loop for the beginning.
    while ((case_actuelle != -1) && (vie > 0 && piece_or < 10)) //Either he leaves or he have lost/won.
    {
        case_actuelle = deplace_personnage(carte,position_personnage);
        affiche_carte(carte,position_personnage);
        switch (case_actuelle)
        {
            case 0: printf("You walk on the %sgrass%s\n",vert,blanc);
                    break;
            case 1: printf("You walk on a %sflower%s\n",rouge,blanc);
                    break;
            case 4: printf("You walk on a %skey%s\n",jaune,blanc);
                    break;
            case 5: printf("You get a %sgold coin%s\n",jaune,blanc);
                    piece_or++;
                    carte[position_personnage[0]][position_personnage[1]] = 0;
                    break;
            case 6: printf("You walk on a %slock%s\n",jaune,blanc);
                    break;
            case 7: printf("YOU HAVE WALKED ON A %sTRAP%s!! You lose %s1 %slife%s\n",rouge,blanc,rouge,vert,blanc);
                    vie--;
                    carte[position_personnage[0]][position_personnage[1]] = 0;
                    break;
            case 8: printf("THE MONSTER %sCATCHES YOU UP%s!! You lose %s1 %slife%s\n",rouge,blanc,rouge,vert,blanc);
                    vie--;
                    break;
            case -1: printf("End of the game.\n");
                     break;

            //We're done now with the basics rules from dntt. We make our own rules here for "our small problems".
            case 9: printf("%sYou hit the edge of the map!%s\n",rouge,blanc);
                    break;
            case 10: printf("%sYou try to cross the %stree%s but the tree won't??%s\n",rouge,vert,rouge,blanc);
                     break;
            case 11: printf("It's a %srock%s!\n",rouge,blanc);
                     break;
        }
        printf("%sHP: %s%d\t%sGold coins: %s%d\n",rouge,blanc,vie,jaune,blanc,piece_or);

        nombre_de_tours++;
        if (nombre_de_tours%1 == 0) //Every 2 turns we move the monster toward the player.
        {
            case_avant = deplace_monstre(carte,position_personnage,case_avant);
            if (carte[position_personnage[0]][position_personnage[1]] == 8)
            {
                printf("THE MONSTER IS ON %sYOU%s!! You lose %s1 %slife%s\n",rouge,blanc,rouge,vert,blanc);
                vie--;
            }
        }
    }
    
    //The game has ended. What happened?
    if (piece_or == 10)
    {
        printf("%sYou managed to get %s10 gold coins! %sYou have won!\n",vert,jaune,blanc);
    }
    else if (vie == 0)
    {
        printf("%sYou have lost. But you may take your revenge.%s\n",rouge,blanc);
        printf("%sBe careful of the %smonster%s that follows you since the beginning.\n",rouge,jaune,blanc);
    }
    printf("Thank you for playing our game. The game was the result of a teamwork from:\n");
    printf("%sArpe5\t%sL1MIPI B2\t%sArpe5@GitHub%s\n",rouge,jaune,vert,blanc);
    printf("%sANONYMOUS\t%sL1MIPI A2\t%sanonymous@CYu%s\n",rouge,jaune,vert,blanc);
    printf("Come back as many times as you want.\n");

    return 0;
}

void init_carte(int carte[20][20])
{
    int il_existe_un_unique_monstre = 0; //We only want one monster for a 20x20, it's enough using "artificial" intelligence.

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
    //For the peer, if they ask you the question, say: "It's according to the ANSI standard"
    char rouge[] = "\033[31m";
    char vert[] = "\033[32m";
    char jaune[] = "\033[33m";
    char blanc[] = "\033[37m";
    //For the peer, if they ask you the question, say: "We don't know the size of X therefore we let the compiler takes charge of it"
    char herbe[] = "^ ";     //0
    char fleur[] = "🌷";     //1
    char arbre[] = "🌲";     //2, obstacle
    char rocher[] = "🪨";   //3, obstacle
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
                switch (carte[x][y]) //We remember of the common function switch(x) where x is an INTEGER. Since carte[x][y] is an INTEGER. Therefore.
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
                    //There isn't a default here because it's a map made by ourselves therefore there isn't absurdity in the values of the squares.
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
	    case 6: printf("You go on the right.\n");
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
	    case 4: printf("You go on the left.\n");
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
	    case 8: printf("You go up.\n");
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
	    case 2: printf("You go down.\n");
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
	    default: printf("Input error, the character doesn't move!\n");
                 break;
	}
}

int verifier_carte(int carte[20][20],int position_personnage[2])
{
    //We do 20-1 because we look at n+1 the squares for the obstacles.
    for (int i=0;i<20-1;i++)
    {
        for (int j=0;j<20-1;j++)
        {
            //Let Personnage(x,y) be a pair of integers (x,y), if i+1 and j+1 are obstacles. The hypothesis is enough to know he'll be stuck.
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
    //Calculation rules for the population: Do not exceed 15%!
    int population_cadenas = 15; //Take only 20% of locks in the map.
    int population_pieces_or = 15; //Take only 20% of gold coins in the map.

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

    if ((nombre_cadenas/400.0*100 < population_cadenas) || (nombre_pieces_or/400.0*100 < population_pieces_or)) //cf. Division problem in dntt's TUTORIAL.
    {
        return 0; //The map is not satisfiable. We restart everything from the beginning.
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

    //Fixing absurdities dues to earlier changes.
    for (int i=0;i<20;i++)
    {
        for (int j=0;j<20;j++)
        {
            if ((nombre_cadenas > nombre_clefs) && (carte[i][j] == 6)) //There are more locks than keys. We must fix it.
            {
                carte[i][j] = 0;
                nombre_cadenas--;
            }
            else if ((nombre_clefs > nombre_cadenas) && (carte[i][j] == 4)) //Same.
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

    if (position_personnage[0] > position_monstre[0]) //He is on the right.
    {
        case_choisi[0] = position_monstre[0]+1;
        case_choisi[1] = position_monstre[1]; //We don't move on y-axis.
    }
    else if (position_personnage[0] < position_monstre[0]) //He is on the left.
    {
        case_choisi[0] = position_monstre[0]-1;
        case_choisi[1] = position_monstre[1];
    }
    else if (position_personnage[1] > position_monstre[1]) //He is above.
    {
        case_choisi[0] = position_monstre[0]; //We don't move on x-axis.
        case_choisi[1] = position_monstre[1]+1;
    }
    else //He is below.
    {
        case_choisi[0] = position_monstre[0];
        case_choisi[1] = position_monstre[1]-1;
    }

    //We put in memory the square to be modified.
    case_apres[0] = case_choisi[0];
    case_apres[1] = case_choisi[1];
    case_apres[2] = carte[case_apres[0]][case_apres[1]]; //We get what is inside the square.
    //It could happen that the monster is on the edge of the map at the beginning of the game. Let's check if this is the case.
    if ((case_apres[0] == position_monstre[0]+1) && (position_monstre[0] == 0)) //There is nothing on the left. Not good!
    {
        case_apres[2] = 100; //Absurd value to say that we didn't touch the square before the monster.
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0; //Let's suppose there was grass before.
    }
    else if ((case_apres[0] == position_monstre[0]-1) && (position_monstre[0] == 19)) //There is nothing on the right. Not good!
    {
        case_apres[2] = 100;
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0;
    }
    else if ((case_apres[1] == position_monstre[1]-1) && (position_monstre[1] == 19)) //There is nothing below. Not good!
    {
        case_apres[2] = 100;
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0;
    }
    else if ((case_apres[1] == position_monstre[1]+1) && (position_monstre[1] == 0)) //There is nothing above. Not good!
    {
        case_apres[2] = 100;
        carte[case_choisi[0]][case_choisi[1]] = 8;
        carte[position_monstre[0]][position_monstre[1]] = 0;
    }
    else //The monster isn't on the edge of the map. We can use the variable case_avant without problem.
    {
        carte[case_choisi[0]][case_choisi[1]] = 8; //We move the monster.
        if (case_avant == 100) //It's the first time the monster moves.
        {
            carte[position_monstre[0]][position_monstre[1]] = 0; //We suppose too there was grass before his move.
        }
        else //No. He already moved beforehand therefore we know what was on the map.
        {
            carte[position_monstre[0]][position_monstre[1]] = case_avant; //We put back what there was using the variable case_avant we put in memory.
        }
    }
    
    //We save the changes for later, especially for case_avant.
    return case_apres[2];
}

int donner_un_chiffre_aleatoire()
{
    return rand()%9;
}
