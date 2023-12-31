#include <stdlib.h>
#include <stdio.h>

#define PV 1
#define P 2
#define VAR 3
#define DP 4
#define ID 5
#define V 6
#define INTEGER 7
#define CHAR 8
#define BEGIN 9
#define END 10
#define OPAFF 11
#define IF 12
#define THEN 13
#define ELSE 14
#define WHILE 15
#define DO 16
#define PO 17
#define PF 18
#define WRITE 19
#define READ 20
#define WRITELN 21
#define READLN 22
#define OPREL 23
#define OPADD 24
#define OPMUL 25
#define PROGRAM 26
#define NB 27
#define PPE 28
#define PPQ 29
#define PGQ 30
#define PGE 31
#define EGA 32
#define DIF 33

int n = 0;

typedef struct Mots{
    char lexème[8];
    int type;
    long int address;
}mot;


typedef struct Symbole{
    int UL;
    int attribute;
}symbole;

typedef struct motcle{
    int UL;
    char att[8];
}motcle;

motcle tabSymbols[] = {
        {PV, ";"},
        {P, "."},
        {VAR, "var"},
        {DP, ":"},
        {ID, "id"},
        {V, ","},
        {INTEGER, "integer"},
        {CHAR, "char"},
        {BEGIN, "begin"},
        {END, "end"},
        {OPAFF, ":="},
        {IF, "if"},
        {THEN, "then"},
        {ELSE, "else"},
        {WHILE, "while"},
        {DO, "do"},
        {PO, "("},
        {PF, ")"},
        {WRITE, "write"},
        {READ, "read"},
        {WRITELN, "writeln"},
        {READLN, "readln"},
        {OPREL, "PPE"},
        {OPREL, "PPQ"},
        {OPREL, "PGQ"},
        {OPREL, "PGE"},
        {OPREL, "EGA"},
        {OPREL, "DIF"},
        {OPADD, "+"},
        {OPMUL, "*"},
        {PROGRAM, "program"},
        {NB, ""}
};


void strcopy(char ch1[], char ch2[]){
    for (int i=0; ch2[i] != '\0'; i++){
        ch1[i] = ch2[i];
    }
}

void addExtendTab(mot** tab) {
    mot* tab2 = (mot*)malloc((n + 10) * sizeof(mot));
    for (int i = 0; i < n; i++) {
        tab2[i] = (*tab)[i];
    }
    free(*tab);
    *tab = tab2;
}



int check(char ch[]){
    for (int i=0; i<32; i++){
        if (strcmp(ch, tabSymbols[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int UnilexId(char ch[]) {
    printf("chaine a traiter par UnilexId %s \n", ch);
    for (int i=0; i<sizeof(tabSymbols)/sizeof(symbole); i++){
        if (strcmp(ch,tabSymbols[i].att) == 0){
            printf("attribut de symbole %s\n", tabSymbols[i].att);
            printf("mot clé trouvé %s \n",tabSymbols[i].att);
            return tabSymbols[i].UL;
        }
    }
    return tabSymbols[4].UL;
}

int RangerId(char ch[],mot* tab) {

    int a = UnilexId(ch);
    printf("le symbole trouvé %d \n", a);
    if (a == ID){
        for (int i=0; i<n; i++){
            if (strcmp(ch,(tab)[i].lexème) == 0){
                printf("variable trouvé a la ligne %d \n",i);
                return i;
            }
        }
        printf("variable non trouvé a la ligne \n");
        mot newMot;
        /*if ((sizeof(*tab)/sizeof(mot)) == n){
            printf("tableau complet\n");
            addExtendTab(tab);
        }*/
        strcpy(newMot.lexème, ch);
        newMot.type = 1;
        //newMot.address = *(tab[n]);
        printf("newMot = %s\n", newMot.lexème);
        (tab)[n] = newMot;
        printf("newMot dans tab = %s\n", (tab)[n].lexème);
        printf("variable non trouvé a la ligne %d\n", n);
        printf("\n unilexid %d \n",n);
        return n++;
    }
    else{
        return 0;
    }
}

void echec() {
    printf("echec \n");
}

symbole analyseur(FILE * f, mot* tab){
    char c;
    int etat = 0;
    char chaine [10];
    int i = 0;
    symbole symbole;

    while ( (c = fgetc(f)) != EOF ) {

        switch(etat){
            case 0:
                printf("case 0 %c \n", c);
                if (c == ' ' || c == '\n' || c == '\t'){
                    i = 0;
                    etat = 0;
                
                }

                else if (isalpha(c))
                {
                    chaine[i++] = c;
                    etat = 1;

                }

                else if ( c == '<' ) {
                    etat = 5;
    
                }

                else if ( c == '=' ) {
                    etat = 9;
                    
                }

                else if ( c == '>' ) {
                    etat = 10;
                    
                }

                else if ( c == ':' ) {
                    etat = 15;
                }

                else if (c == ';'){
                    symbole.UL = PV;
                    symbole.attribute = PV;
                    etat = 0;
                }
                else if (c == ','){
                    symbole.UL = V;
                    symbole.attribute = V;
                    etat = 0;
                }
                else if (c == '.'){
                    symbole.UL = P;
                    symbole.attribute = P;
                    etat = 0;
                }

                else if (c == '()'){
                    symbole.UL = P;
                    symbole.attribute = P;
                    etat = 0;
                }

                else {
                    etat = 14;
                }
                break;

            case 1:
                printf("case 1 %c \n", c);
                if (isalpha(c) || isdigit(c))
                {
                    chaine[i++] = c;
                    etat = 1;
                }
                else{
                    etat = 2;
                }
                break;

            case 2:
                printf("case 2 %c \n", c);
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = UnilexId(chaine);
                int a = RangerId(chaine,tab);

                symbole.attribute= a ;
                printf("mot a traiter case 2 %s \n", chaine);
                return symbole;
                

            case 3:
                if (isdigit(c))
                {
                    chaine[i++] = c;
                    etat = 3;
                }

                else{
                    etat = 4;
                }
                break;

            case 4:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = NB;
                strcopy(symbole.attribute,chaine);
                printf("mot a traiter case 4 %s \n", chaine);
                return symbole;
                

            case 5:
                if ( c == '='){
                    etat = 6;
                }

                else if (c == '>'){
                    etat = 7;
                }

                else{
                    etat = 8;
                }
                break;

            case 6:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = OPREL;
                symbole.attribute = PPE;
                printf("mot a traiter case 6 %s \n", chaine);
                return symbole;



            case 7:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = OPREL;
                symbole.attribute = IF;
                printf("mot a traiter case 7 %s \n", chaine);
                return symbole;
                


            case 8:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = OPREL;
                symbole.attribute = PPQ;
                printf("mot a traiter case 8 %s \n", chaine);
                return symbole;
                


            case 9:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = OPREL;
                symbole.attribute = EGA;
                printf("mot a traiter case 9 %s \n", chaine);
                return symbole;
                


            case 10:
                if( c == '='){
                    etat = 11;
                }
                else{
                    etat = 12;
                }
                break;

            case 11:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = OPREL;
                symbole.attribute= PGE ;
                printf("mot a traiter case 11 %s \n", chaine);
                return symbole;
                


            case 12:
                chaine[i++] = '\0';
                fseek(f, -1, SEEK_CUR);
                symbole.UL = OPREL;
                symbole.attribute = PGQ;
                printf("mot a traiter case 12 %s \n", chaine);
                return symbole;
                

            case 14:
                echec();
                etat = 0;
                break;


            case 15:
                if (c == '='){
                    symbole.UL = OPAFF;
                    symbole.attribute = OPAFF;
                }
                else{
                    echec();
                }
                etat = 0;
                break;
        }
    }
    
}


int main(){

    FILE* f = fopen("C:/Users/azizn/Documents/3eme/Logique/projet/test.txt", "r");
    mot* tab = (mot*) malloc(10*sizeof(mot));
    printf("size of tab %d \n",sizeof(tab));
    char c;
    symbole symbole;
    
    while ((c =fgetc(f)) != EOF){
        fseek(f, -1, SEEK_CUR);
        printf("unité lexicale %d \n",symbole.attribute);
        symbole = analyseur(f,tab);
        printf("symbole UL %d \n",symbole.UL);
        printf("symbole attribute %d \n",symbole.attribute);
        printf("tab contient %s \n",tab[10].lexème);
    }
    
    
        
    fclose(f);
    free(tab);
}