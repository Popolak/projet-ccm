#include "pile.h"

/*
  Nom  :  vide
  Fct  :  retourne un booleen indiquant si la pile est vide
  Entree  :  (p) adresse de la pile
  Sortie  :  booleen indiquant que la pile est vide
*/

unsigned short int vide(pile_t * p)
{
  return ((p->n) == -1); /* la pile est vide si n = -1 */
}

/*
  Nom  :  init
  Fct  :  cree une nouvelle pile et retourne son adresse
  Entree  :  (pas) nombre d'elements d'une section de la pile
  Sortie  :  adresse de la nouvelle pile
*/

pile_t * init(int pas)
{
  pile_t * adr_pile = NULL; /* l'adresse de la pile est NULL si l'allocation echoue */
  
  adr_pile = (pile_t *)malloc(sizeof(pile_t)); /* adresse de la pile cree */
  if ( adr_pile )
  {
    adr_pile->n = -1;       /* nombre d'elements presents (aucun) */
    adr_pile->pas = pas;    /* nombre d'elements par section */
    adr_pile->tete = NULL;  /* la pile est vide */
    adr_pile->sect = NULL;  /* la pile est vide */
  }
  
  return adr_pile;
}

/*
  Nom  :  empile
  Fct  :  empile un nouvel element sur la pile
  Entree  :  (p) adresse de la pile
             (x) element a rajouter
*/

void empile(pile_t * p, variant x)
{
  if ( vide(p) )  /* si la pile est vide */
  {
    p->tete = (section_t *)malloc(sizeof(section_t));              /* creation d'une premiere section */
    p->sect = p->tete;
    p->sect->tab = (variant *)malloc((p->pas) * sizeof(variant));  /* allocation de la nouvelle section */
    p->sect->suiv = NULL;                                          /* pas de section suivante */
    p->sect->prec = NULL;                                          /* pas de section precedente */
  }
  else
  {
    if (!((p->n + 1) % (p->pas)))  /* si la section est pleine */
    {
      p->sect->suiv = (section_t *)malloc(sizeof(section_t));        /* rajout d'une nouvelle section */
      p->sect->suiv->prec = p->sect;                                 /* maj section precedente */
      p->sect = p->sect->suiv;                                       /* maj derniere section */
      p->sect->tab = (variant *)malloc((p->pas) * sizeof(variant));  /* allocation de la nouvelle section */
      p->sect->suiv = NULL;                                          /* pas de section suivante */
    }
  }
  p->n = p->n + 1;                          /* incrementation du nombre d'elements */
  *(p->sect->tab + (p->n) % (p->pas)) = x;  /* rajout du nouvel element sur la pile */
}

/*
  Nom  :  depile
  Fct  :  depile un element de la pile et renvoie un booleen indiquant que tout s'est bien passe
  Entree  :  (p) adresse de la pile
             (x) adresse de stockage de l'element depile
  Sortie  :  booleen indiquant que tout s'est bien passe
*/

unsigned short int depile(pile_t * p, variant * x)
{
  unsigned short int succes = 0;
  
  if ( !vide(p) )
    {
      *x = *(p->sect->tab + (p->n) % (p->pas)); /* sortie du dernier element rajoute */
      p->n = p->n - 1;  /* decrementation du nombre d'elements */
      if (!((p->n + 1) % (p->pas)))  /* si la section est vide */
      {
        if ( p->sect->prec != NULL )  /* s'il y a au moins une section */
        {
          p->sect = p->sect->prec;  /* mise a jour de la derniere section */
          free(p->sect->suiv);      /* liberation de la derniere section */
          p->sect->suiv = NULL;     /* pas d'element suivant sur la derniere section */
        }
        else
        {
          free(p->tete);  /* liberation de la section */
          p->sect = NULL; /* raz du pointeur de derniere section */
        }
      }
      succes = 1;  /* marquage de la reussite */
    }
  return succes;
}

/*
Nom  :  supprime
Fct  :  libère toute la mémoire occupée par une pile
Entree  :  (p) adresse de la pile
*/

void supprime(pile_t * p)
{
  section_t * cour = p->tete, * suiv;
  if ( cour != NULL )  /* s'il existe au moins une section */
  {
    suiv = p->tete->suiv;
    while ( cour != NULL )  /* tant qu'il reste des sections non supprimees */
    {
      suiv = cour->suiv;  /* progression du pointeur suivant */
      free(cour->tab);    /* liberation du contenu de la section courante */
      free(cour);         /* liberation de la section courante */
      cour = suiv;        /* progression du pointeur courant */
    }
  }
  free(p);  /* liberation de la structure pile */
}
