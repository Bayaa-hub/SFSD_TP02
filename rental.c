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
int searchRental(int rentalID, Rental *foundRental)
{

    FILE *file = fopen("rental_info.bin", "rb");
    if (file == NULL)
    {
        printf("Error: Could not open file.\n");
        return 0;
    }

    Entete entete;
    fseek(file, 0, SEEK_SET);
    fread(&entete, sizeof(Entete), 1, file);

    for (int i = 0; i < entete.nbblock; i++)
    {
        Rental_Block block;
        fseek(file, sizeof(Entete) + sizeof(Rental_Block) * i, SEEK_SET);
        fread(&block, sizeof(Rental_Block), 1, file);
        for (int j = 0; j < block.NB; j++)
        {
            if (block.tab[j].rentalID == rentalID)
            {
                *foundRental = block.tab[j];
                fclose(file);
                return 1;
            }
        }
    }

    fclose(file);
    printf("Rental with ID %d not found.\n", rentalID);
    return 0;
}
void displayRental(rentalRam r) {
    printf("Rental ID: %d\n", r.rentalID);
    printf("Customer: %s %s\n", r.customer.firstName, r.customer.lastName);
    printf("Contact Info: %s\n", r.customer.contactInfo);
    printf("Game: %s\n", r.game.title);
    printf("Rental Date: %02d/%02d/%04d\n", r.rentaldate.day, r.rentaldate.month, r.rentaldate.year);
    printf("Return Date: %02d/%02d/%04d\n", r.rentaldate.day, r.rentaldate.month, r.rentaldate.year);
    printf("Rental Price: %.2f\n", r.game.rentalPrice);
}


void saveBlocksToFile(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    Entete entete ;
    entete.nbblock = 1;
    entete.nins = 0;  
    Rental_Block block ;
    block.NB = 0; 

    fwrite(&entete, sizeof(Entete), 1, file);
    fwrite(&block, sizeof(Rental_Block), 1, file);

    fclose(file);
}

void loadBlocksFromFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    Entete entete;
    fread(&entete, sizeof(Entete), 1, file);

    for (int i = 0; i < entete.nbblock; i++) {
        Rental_Block block;
        fread(&block, sizeof(Rental_Block), 1, file);
        printf("Block %d:\n", i + 1);
        displayBlock(block);
    }

    fclose(file);
}


