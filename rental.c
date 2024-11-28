#include<stdio.h>
#include<string.h>
#include "struct.h"
#include <stdlib.h>
#include "costumer.h"
#include "game.h"

int calculateDays(Date start, Date end)
{
    int days = (end.year - start.year) * 365 + (end.month - start.month) * 30 + (end.day - start.day);
    return days > 0 ? days : 0;
}

Rental createRental()
{
    Rental rental_info;

    FILE *file = fopen("rental_info.bin", "r+b");
    if (file == NULL)
    {
        file = fopen("rental_info.bin", "w+b");

        Entete entete ;
        entete.nbblock = 1;
        entete.nins = 0;
        Rental_Block b1;
        b1.NB = 0;

        fseek(file, 0, SEEK_SET);
        fwrite(&entete, sizeof(Entete), 1, file);

        fwrite(&b1, sizeof(Rental_Block), 1, file);
    }

    Entete entete;
    fseek(file, 0, SEEK_SET);
    fread(&entete, sizeof(Entete), 1, file);

    Rental_Block lastBlock;
    fseek(file, sizeof(Entete) + sizeof(Rental_Block) * (entete.nbblock - 1), SEEK_SET);
    fread(&lastBlock, sizeof(Rental_Block), 1, file);

    printf("Enter rental ID:\n");
    scanf("%d", &rental_info.rentalID);
    

    rental_info.customer = createCustomer();
    rental_info.game = creategame();

    printf("Enter the rental date (day month year):\n");
    scanf("%d %d %d", &rental_info.rentalDate.day, &rental_info.rentalDate.month, &rental_info.rentalDate.year);
    printf("Enter the return date (day month year):\n");
    scanf("%d %d %d", &rental_info.returnDate.day, &rental_info.returnDate.month, &rental_info.returnDate.year);

    int rentalDays = calculateDays(rental_info.rentalDate, rental_info.returnDate);
    float totalPrice = rentalDays * rental_info.game.rentalPrice;

    printf("Total price for rental: %.2f\n", totalPrice);

    if (lastBlock.NB < 400)
    {
        lastBlock.tab[lastBlock.NB] = rental_info;
        lastBlock.NB += 1;

        fseek(file, sizeof(Entete) + sizeof(Rental_Block) * (entete.nbblock - 1), SEEK_SET);
        fwrite(&lastBlock, sizeof(Rental_Block), 1, file);
    }
    else
    {
        Rental_Block newBlock;
        newBlock.tab[0] = rental_info;
        newBlock.NB = 1;

        fseek(file, 0, SEEK_END);
        fwrite(&newBlock, sizeof(Rental_Block), 1, file);

        entete.nbblock += 1;
        fseek(file, 0, SEEK_SET);
        fwrite(&entete, sizeof(Entete), 1, file);
    }

    entete.nins += 1;
    fseek(file, 0, SEEK_SET);
    fwrite(&entete, sizeof(Entete), 1, file);

    fclose(file);

    return rental_info;
}
