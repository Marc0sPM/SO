#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NPLAYERS 4
#define MAXREBOUND 70

struct player {
    int dorsal; // dorsal (unique) of the player
    int efShot; // integer between 1 and 100 that determines the effectiveness of the shot.
    int efRebound; // integer between 1 and 100 that determines his effectiveness in rebounding
    int points; // points scored during the training session
};

struct player players[NPLAYERS] = {
    {0,30,MAXREBOUND,0}, // player with dorsal 0, efShot 30, efRebound MAXREBOUND
    {1,80,20,0} ,
    {2,40,MAXREBOUND,0} ,
    {3,50,50,0}
};

enum stateBall {
    POSESION,
    AIR
};
enum stateBall ball = AIR;
int bounceDifficulty = 0;

pthread_mutex_t cerrojo;
pthread_cond_t jugando;
pthread_cond_t enAire;



void throw(struct player * Player){
    
    int difficultyShot = rand() % 100; 
    bounceDifficulty = rand() % MAXREBOUND; 

    if (difficultyShot < Player->efShot){
        Player->points += 2;
        printf("-Player: %d, SCOOOORREEEDD!!!\n", Player->dorsal);
    }else
        printf("Water, needed %d to score, actual: %d\n", difficultyShot, Player->efShot);
    ball = AIR;
    pthread_cond_broadcast(&enAire);
    

}
void run (){
    usleep(100);
}
void rebound(struct player * Player){
    pthread_mutex_lock(&cerrojo);
    while(ball != AIR && bounceDifficulty > Player->efRebound){
        pthread_cond_wait(&enAire,&cerrojo);
    }
    //Este thread coje el rebote
    printf("-Player: %d, got the rebound!!!, Player:%d ,Needed:%d \n", Player->dorsal, Player->efRebound, bounceDifficulty);
    ball = POSESION;
    printf("-Player: %d, runs...\n", Player->dorsal);
    run();
    printf("-Player: %d, throw...\n", Player->dorsal);
    throw(Player);
    run();
    pthread_mutex_unlock(&cerrojo);
}

void *jugador(void* arg){
    struct player * myPlayer = (struct player *)arg; 
    rebound(arg);
    
}


int main(int argc, char *argv[]){
    pthread_t thread[NPLAYERS];

    for (int i = 0; i < NPLAYERS; i++) {

		pthread_create(&thread[i], NULL, jugador, &players[i]);
  	}

    for (int i = 0; i < NPLAYERS; i++) {
    	pthread_join(thread[i], NULL);
  	}

    return 0;
}
