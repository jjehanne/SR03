#define REQ_NUMERO_CLIENT 0
#define REQ_LISTE_OBJ 1
#define REQ_STOCK_PRIX 2
#define REQ_FIN 3


typedef struct{
	long  type;
	int operation;
	int numero_client;
	char objet[100];

}msg;
