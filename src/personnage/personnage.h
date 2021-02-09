typedef struct position
{
	int x;
	int y;
}
pos_t;

typedef struct perso_s
{
	char * nom;
	int vie;
	int taille;
	pos_t * position;
	char * nom_sprite; //pour animer après
	int vitesse; //gestion de déplacement

	void (*animer)(struct perso_s * const);
    void (*detruire)(struct perso_s ** );

}perso_t;

extern perso_t * perso_creer(char * nom, int vie, int taille, pos_t * position, char * nom_sprite, int vitesse);
extern void perso_existe( perso_t * const personnage );