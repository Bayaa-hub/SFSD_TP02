#include<stdio.h>
#include<string.h>
#include "struct.h"
#include <stdlib.h>

Game creategame()
{
    Game game_info;

    FILE *file = fopen("game_info.bin", "r+b");
    if (file == NULL)
    {
        file = fopen("game_info.bin", "w+b");

        Entete entete;
        entete.nbblock = 1;
        entete.nins = 0;
        Game_Block b1;
        b1.NB = 0;

        fseek(file, 0, SEEK_SET);
        fwrite(&entete, sizeof(Entete), 1, file);

        fwrite(&b1, sizeof(Game_Block), 1, file);
    }

    Entete entete;
    fseek(file, 0, SEEK_SET);
    fread(&entete, sizeof(Entete), 1, file);

    Game_Block lastBlock;
    fseek(file, sizeof(Entete) + sizeof(Game_Block) * (entete.nbblock - 1), SEEK_SET);
    fread(&lastBlock, sizeof(Game_Block), 1, file);

    printf("Enter the title of the game:\n");
    scanf("%99s", game_info.title);

    printf("Enter the rental price of the game:\n");
    scanf("%f", &game_info.rentalPrice);

    if (lastBlock.NB < 150)
    {
        lastBlock.tab[lastBlock.NB] = game_info;
        lastBlock.NB += 1;

        fseek(file, sizeof(Entete) + sizeof(Game_Block) * (entete.nbblock - 1), SEEK_SET);
        fwrite(&lastBlock, sizeof(Game_Block), 1, file);
    }
    else
    {

        Game_Block newBlock;
        newBlock.tab[0] = game_info;
        newBlock.NB = 1;

        fseek(file, 0, SEEK_END);
        fwrite(&newBlock, sizeof(Game_Block), 1, file);

        entete.nbblock += 1;
        fseek(file, 0, SEEK_SET);
        fwrite(&entete, sizeof(Entete), 1, file);
    }

    entete.nins += 1;
    fseek(file, 0, SEEK_SET);
    fwrite(&entete, sizeof(Entete), 1, file);

    fclose(file);

    return game_info;
}
