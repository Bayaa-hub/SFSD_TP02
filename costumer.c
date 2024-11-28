#include<stdio.h>
#include<string.h>
#include "struct.h"
#include <stdlib.h>

Customer createCustomer()
{
    Customer infocustomer;

    FILE *file = fopen("customer_info.bin", "r+b");
    if (file == NULL)
    {
        file = fopen("customer_info.bin", "w+b");

        Entete entete ;
        entete.nbblock = 1;
        entete.nins = 0;
        Customer_Block b1;
        b1.NB = 0;

        fseek(file, 0, SEEK_SET);
        fwrite(&entete, sizeof(Entete), 1, file);

        fwrite(&b1, sizeof(Customer_Block), 1, file);
    }
    Entete entete;
    fseek(file, 0, SEEK_SET);
    fread(&entete, sizeof(Entete), 1, file);

    Customer_Block lastBlock;
    fseek(file, sizeof(Entete) + sizeof(Customer_Block) * (entete.nbblock - 1), SEEK_SET);
    fread(&lastBlock, sizeof(Customer_Block), 1, file);

    printf("Enter your first name:\n");
    scanf("%s", infocustomer.firstName);

    printf("Enter your last name:\n");
    scanf("%s", infocustomer.lastName);

    printf("Enter your ID:\n");
    scanf("%d", &infocustomer.customerID);

    printf("Enter your contact info:\n");
    scanf("%99s", infocustomer.contactInfo);

    if (lastBlock.NB < 250)
    {
        lastBlock.tab[lastBlock.NB] = infocustomer;
        lastBlock.NB += 1;

        fseek(file, sizeof(Entete) + sizeof(Customer_Block) * (entete.nbblock - 1), SEEK_SET);
        fwrite(&lastBlock, sizeof(Customer_Block), 1, file);
    }
    else
    {
        Customer_Block newBlock;
        newBlock.tab[0] = infocustomer;
        newBlock.NB = 1;

        fseek(file, 0, SEEK_END);
        fwrite(&newBlock, sizeof(Customer_Block), 1, file);

        entete.nbblock += 1;
        fseek(file, 0, SEEK_SET);
        fwrite(&entete, sizeof(Entete), 1, file);
    }

    entete.nins += 1;
    fseek(file, 0, SEEK_SET);
    fwrite(&entete, sizeof(Entete), 1, file);

    fclose(file);

    return infocustomer;
}