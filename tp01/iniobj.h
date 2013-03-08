#define NB_MAX_TYPE_OBJ 3

typedef struct produit{
    char* nom;
    float prix;
    int stock;
}produit;

produit* iniobj(){
    //Creation d'une liste d'objet vide
    produit* liste = (produit*) malloc(NB_MAX_TYPE_OBJ*sizeof(produit));
    if (liste == NULL){
        printf("malloc: Erreur d'allocation memoire");
        return NULL;
    }

    //Remplissage de la liste

    liste[0].nom = "Stylo";
    liste[0].prix = 10.5;
    liste[0].stock = 25;

    liste[1].nom = "Pantalon";
    liste[1].prix = 35;
    liste[1].stock = 12;

    liste[2].nom = "Chemise";
    liste[2].prix = 25;
    liste[2].stock = 18;

    return liste;

}