#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Funktion prototyypit

void valikko(void);
void aloitusRuutu(void);
int tarkistaPin(FILE *tiliTiedot);
void lueValinta(int saldo, FILE *tiliTiedot);
void naytaSaldo(int saldo);
int nostaRahaa(int saldo);
void laskeSetelit(int nosto);
void lueRoskat(void);
int lueKokonaisluku(void);


int main()
{
 int saldo;

 FILE *tiliTiedot;

    aloitusRuutu();


    saldo = tarkistaPin(tiliTiedot);
    valikko();
    lueValinta(saldo, tiliTiedot);





    return 0;
}
//funktio joka printtaa aloitusruudun
void aloitusRuutu(void){

    printf("Tervetuloa kayttamaan pankkiautomaattia\n");

}
//funktio joka tarkistaa onko sy�tetty tunnusluku oikein
int tarkistaPin(FILE *tiliTiedot){


    char pinKoodi[256];
    char tarkistaPinKoodi[256];
    int tulos;
    int saldo;
    int virheet = 3;

    int luku = 0;
    char tiliNumero[256];

     do{
        printf("\nSyota tilinumero > ");
        fgets(tiliNumero, 256, stdin);

        if (tiliNumero[strlen(tiliNumero) - 1] == '\n')
            tiliNumero[strlen(tiliNumero) - 1] = '\0';
        else
            lueRoskat();

        strcat(tiliNumero, ".acc");

        if ((tiliTiedot = fopen(tiliNumero, "r")) == NULL){
            printf("\nVaara tilinumero, yrita uudelleen\n");
        }else
            luku = luku+1;

        } while (luku == 0);


        printf("\nSyota pin-koodi > ");
        fgets (pinKoodi, 256, stdin);

        if (pinKoodi[strlen(pinKoodi) - 1] == '\r'){
            pinKoodi[strlen(pinKoodi) - 1] = '\0';
        }


        fgets(tarkistaPinKoodi, 256, tiliTiedot);

        do {
            if (tarkistaPinKoodi[strlen(tarkistaPinKoodi) - 1] == '\r')
            tarkistaPinKoodi[strlen(tarkistaPinKoodi) - 1] = '\0';
            if ((tulos = strcmp(pinKoodi, tarkistaPinKoodi)) == 0){
                fscanf(tiliTiedot, "%d", &saldo);
            }else{
                virheet--;
                if(virheet == 0){
                    printf("\nPin-koodi syotetty liian monta kertaa vaarin, automaatti suljetaan...");
                    exit (0);
                }
                printf("\nVirhe! Vaara pin-koodi");
                printf("\nYrityksia jaljella %d, yrita uudelleen > ", virheet);
                fgets (pinKoodi, 256, stdin);
            }
                if (pinKoodi[strlen(pinKoodi) - 1] == '\r'){
                    pinKoodi[strlen(pinKoodi) - 1] = '\0';
                }
            }while (tulos != 0);
            fclose(tiliTiedot);
    return saldo;
}


//funktio valikon tulostukseen
void valikko(void){

    printf("\n1. Nayta saldo\n");
    printf("2. Nosta rahaa\n");
    printf("0. Poistu valikosta\n");
}


//funktio joka lukee k�ytt�j�n valinnan valikossa
void lueValinta(int saldo, FILE *tiliTiedot){
    int nostSum;
    int valinta;
    do{
    printf("\nValitse toiminto syottamalla numero ja painamalla enter > ");
    valinta = lueKokonaisluku();
    switch (valinta){

        case 1:
            naytaSaldo(saldo);
            valikko();
            break;

        case 2:
            nostSum = nostaRahaa(saldo);
            saldo = nostSum;
            valikko();
            break;

        case 0:

            printf("Kiitos kaynnista!");
            break;

        default:
            printf("Syota oikea luku valikon kayttoa varten\n");
            valikko();
    }
    } while (valinta != 0);

}


void naytaSaldo(int saldo){
    printf("Tilisi saldo on %d euroa\n\n", saldo);

}


//funktio jolla rahan nosto toimii
int nostaRahaa(int saldo){

    int nosto;
    int nostoRaja = 1000;
    int minNosto = 20;

    printf("Nostettavat summat ovat 20, 40, 50, 60...\n");
    printf("\nSyota nostettava summa > ");
    nosto = lueKokonaisluku();     //luetaan k�ytt�j�n sy�tt�m� summa

    if (saldo == 0){
    printf("Nosto epaonnistui, tilillasi on %d euroa jaljella\n", saldo);
    return 0;
    }

        if (nosto < minNosto || nosto > nostoRaja || nosto == 30 || (nosto % 10) != 0)
    do{
        printf("\nVirheellinen nosto\n");
        printf("Minimi nosto on 20 euroa ja suurin nosto 1000 euroa\n");
        printf("Nostettavat summat ovat 20, 40, 50, 60, 70, 80...");
        printf("\nSyota nostettava summa uudestaan > ");
        nosto = lueKokonaisluku();
    }while (nosto < minNosto || nosto > nostoRaja || nosto == 30 || (nosto % 10) != 0);

    if (nosto > saldo)
    do{
        printf("Tilillasi ei ole tarpeeksi katetta nostoa varten. Tilillasi on %d euroa\n", saldo);
        printf("Syota nostettava summa uudelleen\n");
        scanf("%d", &nosto);
    }while (nosto > saldo);
    saldo = saldo - nosto;

    laskeSetelit(nosto);      //lasketaan setelien m��r� jotka annetaan k�ytt�j�lle
    if(saldo >= nosto){
    printf("Tililta nostettiin %d euroa tililla on jaljella %d euroa\n\n", nosto, saldo);
    }

    return saldo;
}

//funktio joka laskee setelien m��r�n joita k�ytt�j�lle annetaan

void laskeSetelit(int nosto){


    int seteli20 = 0;
    int seteli50 = 0;
    int jaannos = 0;


    //jos noston jakoj��nn�s 50:ll� = 0, annetaan vain 50e seteleit�
    if (nosto % 50 == 0){
        seteli50 = nosto / 50;

    //jos yll�oleva ehto ei toteutu, otetaan j��nn�s talteen
    }
    else if (nosto % 50 != 0){
        jaannos = nosto % 50;
    }

        /*tarkistetaan onko jaannoksen jakoj��nn�s 20:ll� = 0, jos on
          niin annetaan 50e seteleit� noston mukaan ja 20e seteleit� j��nn�ksen mukaan */
        if (jaannos % 20 == 0){
            seteli50 = nosto / 50;
            seteli20 = jaannos / 20;
        }
        /*jos yll�oleva ehto ei toteudu, eli j��nn�ksen jakoj��nn�s 20:ll�
          on kaikkea muuta kuin 0, niin v�hennet��n 50e seteleist� yksi seteli,
          lis�t��n j��nn�kseen 50 euroa ja lasketaan 20e setelien m��r� j��nn�ksell� */
        else if (jaannos % 20 != 0){
            seteli50 = nosto / 50 - 1;
            jaannos = jaannos + 50;
            seteli20 = jaannos / 20;
    }   else{
        printf("Virheellinen nosto");
    }



    printf("Sait %d kpl 20e seteleita ja %d kpl 50e seteleita\n", seteli20, seteli50);
}

void lueRoskat(void){

   // Tyhjennet��n puskuri
   while( fgetc(stdin) != '\n');
}

int lueKokonaisluku(void){
   int luku = 0;
   char mki =' ';
   int status = 0;

   while (((status = scanf("%d%c", &luku, &mki)) == 0)  || (2 == status && mki != '\n')){
      lueRoskat();
      printf("\nVirheellinen syote, kayta kokonaislukuja. Yrita uudelleen > ");

   }

   return luku;
}






