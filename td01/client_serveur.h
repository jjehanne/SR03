#define NB_MAX_TYPE_OBJ 3

typedef struct{
	long  type;
	char operation;
	int numero_client;
	char objet[NB_MAX_TYPE_OBJ][100];
	int id_produit;
	float prix;
	int stock;

}msg;
