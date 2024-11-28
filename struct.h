#ifndef struct_h
#define struct_h

typedef struct {
    int customerID;
    char firstName[50];
    char lastName[50];
    char contactInfo[100];
} Customer;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char title[100];
    float rentalPrice;
} Game;

typedef struct {
    int rentalID;
    Customer customer;
    Game game;
    Date rentalDate;
    Date returnDate;
} Rental;

typedef struct {
    int rentalID;
}RentalID;

typedef struct {
    Customer tab[250];
    int NB;
}Customer_Block;
typedef struct {
    Date tab[50];
    int NB;
}Date_Block;
typedef struct {
    Game tab[150];
    int NB;
}Game_Block;
typedef struct {
    Rental tab[400];
    int NB;
}Rental_Block;
typedef struct {
    RentalID tab[50];
    int NB;
}RentalID_Block;

typedef struct Entete{
    int nbblock;
    int nins;
}Entete;

typedef struct TOF
{
    char file_name[200];
    Entete entete;
}TOF;

#endif
