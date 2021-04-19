#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"
#include "../../lib/entite/entite.h"
#include "../../lib/affichage/room_rendering.h"


/* Par Matthis */
/*Fonctions*/

extern 
void initTabDaffich(void * tab[NB_MAX_AFF]){
    int i;
    for(i=0;i<NB_MAX_AFF;i++){
        tab[i]=NULL;
    }
}

static
err_t detruire_tabl_textures(entite_t **ent){
    int i;
    for(i=0;i<(*ent)->nbTextures;i++){
        if((*ent)->textures[i] != NULL){
            SDL_DestroyTexture( (*ent)->textures[i]);
            
            (*ent)->textures[i]=NULL;
        }
    }
    free((*ent)->textures);
    (*ent)->textures=NULL;
    
    return OK;
}

extern 
char * entre_guillemet(FILE * file){
    char c,*chaine=NULL;
    int taille,j;
    while((c =getc(file))!='\"');

    c=' ';
    for(taille=0;(c=getc(file))!='\"';taille++);

    chaine=malloc(sizeof(char)* taille+1);
    fseek(file,-(taille+1),SEEK_CUR);
    if(!chaine)
        return NULL;
    for(j=0;j<taille;chaine[j]=getc(file), j++);
    chaine[taille]='\0';
    fseek(file,2,SEEK_CUR);
    return chaine;
}

extern 
SDL_Texture ** creer_tableau_textures_chaine(SDL_Renderer *ren, int *n,char * chaine, char * appel){
    *n=0;
    SDL_Texture ** textures = NULL;
    char *nom_text=NULL, *nom_final=NULL;
    int i,taille,j, taille_appel;

    for(taille_appel=0; appel[taille_appel]; taille_appel++);

    for(i=0;chaine[i];i++){
        if(chaine[i]== '\"'){
            for(j=i+1,taille=0;chaine[j]!='\"';j++, taille++);
            nom_text=realloc(nom_text,sizeof(char) * taille+1);
            nom_final=realloc(nom_final, sizeof(char) * taille_appel + taille + 1);
            strcpy(nom_final,appel);
            for(j=i+1;j<i+taille+1;j++){
                nom_text[j-i-1]=chaine[j];
            }
            nom_text[taille]='\0';
            strcat(nom_final, nom_text);
            (*n)++;
            textures=realloc(textures, sizeof(SDL_Texture*) * (*n));
            textures[*n-1]=creer_texture_image(ren,nom_final);
            if(textures[*n-1]==NULL){
                if((*n)-1 == 0){
                    free(textures);
                    textures=NULL;
                }
                textures=realloc(textures, sizeof(SDL_Texture*) * (*n-1));
                (*n)--;
            }
            i+=taille+1;
        }
    }
    free(nom_text);
    free(nom_final);
    return textures;
    
}

extern 
SDL_Texture ** creer_tableau_textures_manuel(SDL_Renderer * ren, int *n,...){
    *n=0;
    SDL_Texture ** tableau=NULL;
    va_list ap;
    char * nom_texture;
    va_start(ap,n);
    do{
        nom_texture= va_arg(ap,char *);
        if(strlen(nom_texture) >0){
            (*n)++;
            tableau=realloc(tableau,sizeof(SDL_Texture*) * (*n));
            tableau[*n-1]=creer_texture_image(ren,nom_texture);
        }
    }while(strlen(nom_texture) >0);
    va_end(ap);
    return tableau;
}

extern 
long seek_entity_type(FILE * index,char *type){
    char chaine[40], str[300];    
    do{
        fscanf(index,"%s ", chaine);
        if(!feof(index) && strcmp(chaine,type)!=0){
            fgets(str,299,index);
        }
    }while( !feof(index) && strcmp(chaine,type)!=0);
    if(feof(index)){
        printf("Le type %s n'est pas dans l'index\n", type);
        return -1;
    }
    return (ftell(index));
}

/*
    est_obstacle:
    paramètres:
        int contenu, contenu d'une unité d'un chunk
        int dir, direction d'une entité
    retourne 1 si le contenu est un obstacle a une entité compte tenu de la direction de l'entité 
*/
extern 
int est_mur(int contenu){
    return (contenu==MUR );
}

extern int est_pont(int contenu){
    return contenu==PONT;
}


extern
void echanger(void ** ptr1,void **ptr2 ){
    void * tempo=NULL;
    tempo=*ptr1;
    *ptr1=*ptr2;
    *ptr2=tempo; 
}


extern err_t afficher_tableau (void * tab_ent[NB_MAX_AFF], SDL_Renderer * ren, int WINW, int WINH){
    int i;
    for(i=0;i<NB_MAX_AFF && tab_ent[i]!=NULL; i++){
        ((entite_t*)tab_ent[i])->afficher_chunk(ren, ((entite_t*)tab_ent[i]), WINH, WINW);
    }
    return OK;
}

extern 
int ajouter_tableaux( void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ), void * ptr, err_t (*fonction_dest)(void **)){
    int i;
    for(i=0;i<NB_MAX_AFF && tab[i]!=NULL; i++);
    if(i>=NB_MAX_AFF)
        return 0;
    tab[i]=ptr;
    tab_destr[i]=fonction_dest;
}

extern 
void enlever_tableaux(void * tab[NB_MAX_AFF] , err_t (*tab_destr[NB_MAX_AFF])(void ** )){
    int i;
    for(i=0;i<NB_MAX_AFF && tab[i]!=NULL;i++);
    i--;
    if(i==NB_MAX_AFF || i<0|| tab[i]==NULL)
        return;
    tab_destr[i](&(tab[i]));
    tab[i]=NULL;
    tab_destr[i]=NULL;
}

extern 
void vider_tableaux(void * tab[NB_MAX_AFF] , err_t (*tab_destr[NB_MAX_AFF])(void ** )){
    int i;
    while(tab[0]!=NULL){
        enlever_tableaux(tab,tab_destr);
    }
}

extern
void enlever_element_tableau(void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ),void *element ){
    int i,j;
    void * tempo=NULL;
    for(i=0;i<NB_MAX_AFF && tab[i]!=NULL && tab[i]!=element;i++);
    if(!(i>=NB_MAX_AFF || tab[i]==NULL)){
        for(j=0; j< NB_MAX_AFF && tab[j]!=NULL; j++);
            j--;
        echanger((void **)(&(tab[i])), (void **)(&(tab[j])));
        echanger((void **)(&(tab_destr[i])), (void **)(&(tab_destr[j])));
        enlever_tableaux(tab, tab_destr);
    }
}

extern
void update_ennemis_input(void * tab[NB_MAX_AFF], perso_t * joueur){
    int i;
    for(i=0;i<NB_MAX_AFF && tab[i]!=NULL;i++){
        ((entite_t*)tab[i])->update_speed(tab[i],joueur,0);
    }
}

extern 
void synchro_tableau(void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ),double temps,  FILE * file_gen){
    int i,j;
    for(i=0; i< NB_MAX_AFF && tab[i]!=NULL; i++){
        if(((entite_t * )tab[i])->deplacer((void* )tab[i],temps )!=0 || ((entite_t * )tab[i])->existe == FAUX){
            enlever_element_tableau(tab,tab_destr,tab[i]);
        }
    }
}

extern 
void hitbox_tableau(SDL_Renderer * ren, void * tab[NB_MAX_AFF], int WINW, int WINH){
    int i;
    for(i=0; i<NB_MAX_AFF && tab[i]!=NULL; i++){
        ((entite_t*)tab[i])->hitbox(ren, ((entite_t*)tab[i]), WINH, WINW);
    }
}

extern 
void tableau_agit ( SDL_Renderer* ren, void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ),void * ent_a_verif, FILE * index, char * appel){
    int i;
    for(i=0; i<NB_MAX_AFF && tab[i]!=NULL;i++){
        ((entite_t*)ent_a_verif)->action_agit( ren, ent_a_verif, tab[i],tab,tab_destr,index,appel);
        ((entite_t*)tab[i])->action_agit(ren, tab[i], ent_a_verif,tab,tab_destr,index,appel);
        if(((entite_t*)tab[i])->existe == FAUX)
            enlever_element_tableau(tab,tab_destr,tab[i]);
    }   
}

static 
void entite_action_subit(void * ent_courante, int n){

}

static 
void entite_action_agit(SDL_Renderer * ren,void * ent_courante, void * ent_subit, void * tab[NB_MAX_AFF], err_t (*tab_destr[NB_MAX_AFF])(void ** ), FILE * index, char * appel ){

}



/*
    afficher_dans_chunk:
    paramètres: 
        pointeur sur SDL_Renderer, le renderer
        pointeur sur entite_t, l'entite a afficher
        int WINW,int WINH, la hauteur et la largeur de la fenetre
    retourne OK si tout s'est bien passé, une erreur sinon

    Utilisé pour afficher les entités qui sont sur le chunk a afficher
*/


static err_t afficher_dans_chunk(SDL_Renderer *ren,void *element,int WINH,int WINW){
    SDL_Texture * a_afficher=NULL;
    int w,h, w_immo,h_immo;
    float ratio_h, ratio_w;
    entite_t *entite= (entite_t *) element;
    if(entite->nbTextures ==0){
        return OK;
    }

    if(entite->lastSprite >= 7*entite->secSprite ){
        entite->lastSprite=0;
    }
    if(entite->vitesse_y==0){
        entite->lastSprite=0;
        a_afficher=entite->textures[IMMO];
    }

    if(entite->vitesse_x < 0 ){
        if(entite->nbTextures <= SAUT)
            a_afficher= entite->textures[IMMO];
        else 
            a_afficher=entite->textures[SAUT];
    }
    else if(entite->vitesse_x > 0 ){
        if(entite->nbTextures <= TOMBE)
            a_afficher= entite->textures[IMMO];
        else 
            a_afficher=entite->textures[TOMBE];
    }
    else {

        if(abs(entite->vitesse_y) && entite->nbTextures > IMMO && !entite->en_l_air(entite)){
            if(entite->secSprite > entite->lastSprite){
                if(entite->nbTextures <= POS_MOUV1)
                    a_afficher=entite->textures[IMMO];
                else   
                    a_afficher=entite->textures[POS_MOUV1];
            }
             
            else if (entite->lastSprite > 4 * entite->secSprite && entite->lastSprite < 5 * entite->secSprite ){
                if(entite->nbTextures <= POS_MOUV2)
                    a_afficher=entite->textures[IMMO];
                else   
                    a_afficher=entite->textures[POS_MOUV2];
            }
            else {
                if(entite->nbTextures <= NEUTRE)
                    a_afficher=entite->textures[IMMO];
                else    
                    a_afficher=entite->textures[NEUTRE];
            }
        }
    }
    if(a_afficher==NULL){
        a_afficher=a_afficher=entite->textures[IMMO];
    }
    
    SDL_QueryTexture(a_afficher,NULL,NULL,&w,&h);
    SDL_QueryTexture(entite->textures[IMMO],NULL,NULL,&w_immo,&h_immo);
    ratio_h=(1.0*h/h_immo) ;
    ratio_w=1.0*w/w_immo;
    entite->afficher_fenetre(ren,entite,(entite->w*WINW/CHUNKW)*ratio_w,(entite->h*WINW/CHUNKW )/ratio_h,entite->pos.x*WINW/CHUNKW, entite->pos.y*WINW/CHUNKW, a_afficher);
}


static 
void afficher_hitbox(SDL_Renderer * ren, entite_t * ent, int WINH, int WINW){
    SDL_Rect hitbox;
    hitbox.x=(ent->pos.y - ent->w_hitbox/2 + ent->offset_hitbox * (ent->dir ==DROITE ? 1 : -1))* WINW/CHUNKW; 
    hitbox.y=(ent->pos.x - ent->h_hitbox/2)* WINW/CHUNKW; 
    hitbox.w=ent->w_hitbox*WINW/CHUNKW;
    hitbox.h=ent->h_hitbox*WINW/CHUNKW;
    SDL_SetRenderDrawColor(ren,255,0,0,255);
    SDL_RenderDrawRect(ren,&hitbox);
}

/*
    afficher dans fenetre
    paramètres:
        pointeur sur SDL_Renderer, le renderer
        pointeur sur entite_t, l'entite a afficher
        int w, h la hauteur et largeur (en pixel) de l'entité a afficher
        int x, y les les coordonées (en pixel) de l'entité a afficher 
        pointeur sur SDL_Texture, la texture a utiliser
    retourne OK si tout se passe bien
*/

static err_t afficher_dans_fenetre(SDL_Renderer * ren,entite_t * entite, int w, int h, int x, int y, SDL_Texture * texture){
    
    float ratio;// ratio_width, ratio_height;
    SDL_Rect src,dst, src_neutre;
    SDL_Point centre;
    SDL_RendererFlip flip = entite->dir == DROITE ?  (SDL_RendererFlip)SDL_FLIP_NONE : (SDL_RendererFlip)SDL_FLIP_HORIZONTAL;
    if(!texture){
        printf("Pas de textures donnée pour l'entité : %s\n",entite->nom);
        return PAS_DALLOC;
    }
    src.x=0;
    src.y=0;
    src_neutre.x=0;
    src_neutre.y=0;
    dst.h=h;
    dst.w=w;
    dst.x=y-w/2;
    dst.y=x-h/2;
    
    
    SDL_QueryTexture(texture,NULL,NULL,&(src.w),&(src.h));
    SDL_QueryTexture(entite->textures[IMMO],NULL,NULL,&(src_neutre.w),&(src_neutre.h));
    
    ratio=1.0*src.w/src.h;

    centre.x=src.w/2;
    centre.y=src.h/2;
    if(x>=0 && y>=0){
        SDL_RenderCopyEx(ren,texture,&src,&dst,0,&centre,flip);     //En fonction de la direction de l'entité on retourne symétriquement ou pas la texture
    }
    return OK;

}

extern booleen_t dans_mur(entite_t * ent){
    pos_t pos_mur={-1,-1};
    return (mur_en_haut(ent).x!=-1 || 
    mur_en_bas(ent).x!=-1 || 
    mur_a_droite(ent).x!=-1 || 
    mur_a_gauche(ent).x!=-1);
}

/*
    mur_*:
    paramètre:
        pointeur sur entite_t
    retourne la position du premier mur que la fonction croise, -1 s'il n'y en a pas

    Ces 4 fonctions vérifient si l'entité est en collision avec un mur de chaque coté (gauch droit haut bas)
*/

extern
pos_t mur_a_gauche(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(i=0;i < ent->h/2-1 && i > -ent->h/2 ;i=-(i+add%2),add++){
        for(j=0;  j>-ent->w/2  ;j--){
            if(coord_correcte((int)(ent->pos.x+i),(int)(ent->pos.y+j))){
                if(est_mur(ent->chunk->chunk[(int)(ent->pos.x+i)][(int)(ent->pos.y+j)]->contenu)){
                    pos_mur.x=(int)(ent->pos.x+i);
                    pos_mur.y=(int)(ent->pos.y+j);
                    break;
                }
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}


booleen_t est_dans_mur (entite_t * ent){

}

extern
pos_t mur_a_droite(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(i=0;i < ent->h/2-1 && i > -ent->h/2;i=-(i+add%2),add++){
        for(j=0; j<ent->w/2 ;j++){
            if(coord_correcte((int)(ent->pos.x+i),(int)(ent->pos.y+j+1))){
                if(est_mur(ent->chunk->chunk[(int)(ent->pos.x+i)][(int)(ent->pos.y+j+1)]->contenu)){
                    pos_mur.x=(int)(ent->pos.x+i);
                    pos_mur.y=(int)(ent->pos.y+j+1);
                    break;
                }
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}

extern
pos_t mur_en_haut(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(j=0;j < ent->w/2-1 && j > -(ent->w/2)  ;j=-(j+add%2),add++){
        for(i=0;i>-ent->h/2  ;i--){
            if(coord_correcte((int)(ent->pos.x+i),(int)(ent->pos.y+j))){
                if(est_mur(ent->chunk->chunk[(int)(ent->pos.x+i)][(int)(ent->pos.y+j)]->contenu)){
                    pos_mur.x=(int)(ent->pos.x+i+1);
                    pos_mur.y=(int)(ent->pos.y+j);
                    break;
                }
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}

extern
pos_t mur_en_bas(entite_t * ent){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    for(j=0;j < ent->w/2-1 && j > -ent->w/2 ;j=-(j+add%2),add++){
        for(i=0;i<ent->h/2 ;i++){
            if(coord_correcte((int)(ent->pos.x+i),(int)(ent->pos.y+j))){
                if(est_mur(ent->chunk->chunk[(int)(ent->pos.x+i)][(int)(ent->pos.y+j)]->contenu)){
                    pos_mur.x=(int)(ent->pos.x+i+1);
                    pos_mur.y=(int)(ent->pos.y+j);
                    break;
                    
                }
            }
        }
        if(pos_mur.x!=-1)
            break;
    }
    return pos_mur;
}

pos_t contact_pont(entite_t * ent){
 pos_t pos_mur={-1,-1};
    int i;
    for(i=(int)(ent->pos.y-ent->w_hitbox/2 + ent->offset_hitbox * (ent->dir == DROITE ? 1 : -1)); i< (int)(ent->pos.y+ent->w_hitbox/2 + ent->offset_hitbox * (ent->dir == DROITE ? 1 : -1));i++){
        if(coord_correcte((int)(ent->pos.x+ent->h/2),i)  &&  ent->chunk->chunk[(int)(ent->pos.x+ent->h/2)][i]->contenu==PONT && ent->vitesse_x>=0){
            pos_mur.x=(int)(ent->pos.x+ent->h/2);
            pos_mur.y=i;
            return pos_mur;
        }
    }
    return pos_mur;
}

extern
pos_t pont_en_bas(entite_t * ent ){
    int i,j, add=0;
    pos_t pos_mur={-1,-1};
    pos_mur=contact_pont(ent);
    return pos_mur;
}

/*
    en_lair
    paramètre:
        pointeur sur entite_t
    retourne VRAI si l'entité ne touche pas d'obstacle en dessous d'elle, FAUX sinon
*/

static 
booleen_t en_lair(entite_t * ent){
    pos_t pos_mur;
    pos_mur=mur_en_bas(ent);
    if(pos_mur.x==-1){
        pos_mur=pont_en_bas(ent);
        if(pos_mur.x==-1)
            return VRAI;
    }
    return FAUX;
}

/*
    replacer
    paramètres:
        pointeur sur entite_t, l'entité a replacer hors d'un obstacle
        pos_t pos_mur, la position du mur
        int direction, la direction dans laquelle on doit replacer l'entité
    
*/
extern
void replacer(entite_t * ent, pos_t pos_mur, int direction){
    switch (direction){
        case (DROITE):
            ent->vitesse_y=0;
            ent->pos.y=pos_mur.y-ent->w/2;             //Si direction==DROITE, on replace l'entité vers la gauche du mur
            break;
        case (BAS):
            ent->vitesse_x=0;
            ent->pos.x=pos_mur.x-ent->h/2;             //Si direction==BAS, on replace l'entité vers le haut du mur etc...
            break;
        case (GAUCHE):
            ent->vitesse_y=0;
            ent->pos.y=pos_mur.y+ent->w/2;
            break;
        case (HAUT):
            ent->vitesse_x=0;
            ent->pos.x=pos_mur.x+ent->h/2;
            break;
    }
}

/*
    entitre_deplacement:
    paramètres:
        entite_t * ent, l'entité a déplacer
        double temps, le temps en secondes depuis le dernier déplacement
    
    déplace l'entité en fonction de ses vitesses et du chunk
*/


static 
int entite_deplacement(void * element,double temps ){
    pos_t pos_mur;
    chunk_t * chunk;
    booleen_t deja=FAUX;
    int w,h;
    float vitesse_tempo;
	entite_t * ent = (entite_t *)element; 
    ent->lastSprite+=temps;
    if(ent->vitesse_y > 0){                          //On adapte la direction de l'entité en fonction de sa vitesse
        if(ent->dir==GAUCHE)
            (ent->pos.y)-=(2*ent->offset_hitbox);
        ent->dir=DROITE;                            //Si elle est nulle, on ne change rien 
    }
    else if(ent->vitesse_y < 0){
        if(ent->dir==DROITE)
            (ent->pos.y)+=(2*ent->offset_hitbox);
        ent->dir=GAUCHE;
    }

    (ent->pos.y)+= (ent->vitesse_y)*temps;                  //On actualise la position horizontale grace a vitesse_y
                                                                                           
	if(coord_correcte(ent->pos.x,ent->pos.y)){				//Si on se retrouve dans un mur, on se replace
		pos_mur=mur_a_gauche(ent);   
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,GAUCHE);
		}
	}
	if(coord_correcte(ent->pos.x,ent->pos.y)){
		pos_mur=mur_a_droite(ent);                              //Pareil
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,DROITE);
		}
	}
	(ent->pos.x)+= (ent->vitesse_x)*temps; 
	if(coord_correcte(ent->pos.x,ent->pos.y)){               //Puis on actualise la position verticale
		pos_mur=mur_en_bas(ent);
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,BAS);                          //Si on se retrouve dans un mur, on se replace
		}
	}
	if(coord_correcte(ent->pos.x,ent->pos.y)){ 
		pos_mur=pont_en_bas(ent);
		if(pos_mur.x!=-1){
			replacer(ent,pos_mur,BAS);                          //Si on se retrouve dans un mur, on se replace
		}
	}
	if(coord_correcte(ent->pos.x,ent->pos.y)){ 
		pos_mur=mur_en_haut(ent);
		if(pos_mur.x!=-1 ){
			replacer(ent,pos_mur,HAUT);                         //Pareil
		}
	}
    if(ent->gravite){
        if(ent->en_l_air(ent)){                                 //Si on est en l'air, on tombe (La vitesse maximale est atteinte après 2 secondes de chute avec v0=0)
            ent->vitesse_x= (ent->vitesse_x + GRAVITE*temps > GRAVITE*2) ? GRAVITE*2:ent->vitesse_x + GRAVITE*temps;
        }
    }
	
		//Si la vitesse de l'entité n'est pas actualisée (soit par un input de l'utilisateur, soit par l'algorithme des ennemis)
    //Alors on la change grace a la décélération
    if(ent->vitesse_y){
        if(!ent->en_l_air(ent))
            ent->vitesse_y= ent->vitesse_y -  DECEL*COEFF_DECEL_SOL* temps * (ent->vitesse_y>0 ? 1 : -1)  ; //Sur le sol, la décélération est COEFF_DECEL_SOL fois plus grande

        else
            ent->vitesse_y= ent->vitesse_y -  DECEL*temps * (ent->vitesse_y>0 ? 1 : -1)  ;                  //Que dans l'air

        if(ent->vitesse_y > 0 && ent->dir == GAUCHE || ent->vitesse_y < 0 && ent->dir == DROITE)
            ent->vitesse_y=0;
    }
	
    
    if(!coord_correcte(ent->pos.x, ent->pos.y)){
        return 1;
    }
    return 0;

    

    
}


/*
    en_contact
    paramètres:
        2 pointeur sur entite_t
    Retourne VRAI si les deux entités sont en collision de hitbox, FAUX sinon
*/
static
booleen_t en_contact(entite_t * ent_courante, entite_t * ent_a_verif){
    if(ent_a_verif->chunk !=  ent_courante->chunk)
        return FAUX;
    if (ent_courante->w_hitbox == 0 && ent_courante->w_hitbox == 0 )
        return FAUX;
    if((abs(ent_courante->pos.y + ent_courante->offset_hitbox * (ent_courante->dir == DROITE ? 1 : -1) -(ent_a_verif->pos.y + ent_a_verif->offset_hitbox * (ent_a_verif->dir == DROITE ? 1 : -1))) < (ent_courante->w_hitbox/2 + ent_a_verif->w_hitbox)/2)
    && abs(ent_courante->pos.x - ent_a_verif->pos.x) <= (ent_courante->h_hitbox + ent_a_verif->h_hitbox)/2)
        return VRAI;
    return FAUX;
}


static booleen_t en_contact_porte(entite_t * ent){
    int i,j, add=0;
    for(i= ent->pos.x -ent->h/2 < 0 ? 0 :  (int)(ent->pos.x -ent->h); i< (int)(ent->pos.x+ent->h/2); i++){
        if(coord_correcte(i,(int)(ent->pos.y))){
            if(ent->chunk->chunk[i][(int)(ent->pos.y)]->contenu==PORTE)
                return VRAI;
        }
    }
    return FAUX;
}
/*
    str_creer_copier
    paramètre:
        chaine de caractères, chaine source a copier
    retourne un pointeur sur char différent de chaine_src mais avec la même chaine de caractères, NULL si ça s'est mal passé 
*/

extern 
char * str_creer_copier( char * chaine_src){
    char *chaine_dest=NULL;

    chaine_dest=malloc(sizeof(char)*(strlen(chaine_src)+1));
    if(!chaine_dest){
        printf("L'allocation de l'entite n'a pas pu etre effectuee\n");
        return NULL;
    }
    strcpy(chaine_dest,chaine_src);
    chaine_dest[strlen(chaine_src)]='\0';
    return chaine_dest;
}

/*
    entite_lire
    paramètre:
        pointeur sur entite_t
    affiche le nom et la description de l'entité
*/
static 
void entite_lire(entite_t * ent){
    printf ("%s :\n%s", ent->nom, ent->description);
}

/*
    entite_detruire 
    paramètre:
        pointeur sur pointeur sur entite_t
    retourne OK si tout s'est bien passé

    libère l'entité et toutes les allocations reliées a celle ci
*/

extern 
err_t entite_detruire(entite_t ** ent){
    if(*ent){
        if((*ent)->nom){
            free((*ent)->nom);
            (*ent)->nom=NULL;
        }
        if((*ent)->description){
            free((*ent)->description);
            (*ent)->description=NULL;
        }   
        if((*ent)->textures){
            (*ent)->detruire_textures(ent);
        }
    }
    free(*ent);
    
    *ent=NULL;
    return OK;
}

extern 
booleen_t entite_existe(entite_t * ent){
    if (ent==NULL)
        return FAUX;
    return VRAI;
}

extern 
entite_t * entite_creer(char * nom, 
                        char *description,
                        salle_t * salle,
                        chunk_t * chunk,
                        pos_t pos,
                        float vitesse_x, float vitesse_y, float vitesse_max_y,
                        float secSprite,
                        int w, int h, 
                        int w_hitbox, int h_hitbox, int offset_hitbox,
                        int nbTextures,
                        SDL_Texture ** textures)
{
    entite_t * entite=NULL;
    if (!(entite=malloc(sizeof(entite_t)))){
        printf("L'allocation de l'entite %s n'a pas pu etre effeectuee\n", nom);
        return  NULL;
    }

    entite->pos=pos;
    entite->w=w;
    entite->h=h;
    entite->w_hitbox=w_hitbox;
    entite->h_hitbox=h_hitbox;
    entite->nbTextures=nbTextures;
    entite->vitesse_x=vitesse_x;
    entite->vitesse_y=vitesse_y;
    entite->vitesse_max_y=vitesse_max_y;
    entite->secSprite=secSprite;
    entite->chunk=chunk;
    entite->salle=salle;
    entite->textures=textures;
    entite->dir= vitesse_y >= 0 ? DROITE : GAUCHE;
    entite->lastSprite=0;
    entite->gravite=VRAI;
    entite->existe=VRAI;
    entite->offset_hitbox=offset_hitbox;

    entite->detruire=entite_detruire;
    entite->lire=entite_lire;
    entite->afficher_chunk=afficher_dans_chunk;
    entite->afficher_fenetre=afficher_dans_fenetre;
    entite->deplacer=entite_deplacement;
    entite->en_l_air=en_lair;
    entite->contact=en_contact;
    entite->hitbox=afficher_hitbox;
    entite->detruire_textures=detruire_tabl_textures;
    entite->contact_porte=en_contact_porte;
    entite->action_subit=entite_action_subit;
    entite->action_agit=entite_action_agit;


    if((entite->nom = str_creer_copier(nom))==NULL){
        printf("Le nom %s n'a pas pu etre attribué\n",nom);
        entite->detruire(&entite);
        return NULL;
    }
    if((entite->description=str_creer_copier( description))==NULL){
        printf("La description de  %s n'a pas pu etre attribué\n",description);
        entite->detruire(&entite);
        return NULL;
    }

    return entite;
}