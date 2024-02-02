#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

int maze[MAX_HEIGHT][MAX_WIDTH];
int width, height;


void lectureFich() {
  // Ouverture du fichier contenant le labyrinthe et lecture du labyrinthe
  FILE *fp;
  fp = fopen("gen.txt", "r");
  if (fp == NULL) {
      printf("Erreur : impossible d'ouvrir le fichier.\n");
      exit(EXIT_FAILURE);
  }

  // Lecture de la taille du labyrinthe
  fscanf(fp, "%d %d", &width, &height);

  // Lecture du labyrinthe
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          fscanf(fp, "%1d", &maze[y][x]); // On lit un seul caractère à la fois
      }
  }
  fclose(fp);

}





void affichageSimple() {
  //Affichage du labyrinthe
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          if (maze[y][x] == 0)
              printf("#");
          else if (maze[y][x] == 1)
              printf(" ");
          else if (maze[y][x] == 2)
              printf("E");
          else if (maze[y][x] == 3)
              printf("S");
      }
      printf("\n");

    }
      printf("\n");
}




void solutionSimple() {

    // Recherche la Position (x,y) de E (2) l'entrée du labyrinthe
    int x = 0, y = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(maze[i][j] == 2) {
                x = j;
                y = i;
                break;
            }
        }
    }
  // solution
  while (maze[y][x] != 3) { // Tant qu'on n'a pas atteint la sortie
      maze[y][x] = -1; // On marque la case comme visitée

      // Recherche de la case suivante
      if (x < width - 1 && maze[y][x+1] != 0 && maze[y][x+1] != -1) { // À droite
          x++;
      } else if (x > 0 && maze[y][x-1] != 0 && maze[y][x-1] != -1) { // À gauche
          x--;
      } else if (y < height - 1 && maze[y+1][x] != 0 && maze[y+1][x] != -1) { // En bas
          y++;
      } else if (y > 0 && maze[y-1][x] != 0 && maze[y-1][x] != -1) { // En haut
          y--;
      } else { // Si toutes les cases autour sont bloquées
          printf("Aucune solution trouvée.\n");
          exit(EXIT_FAILURE);
      }
  }
}




void affichageSol() {
  // Affichage de la solution
  for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (maze[y][x] == -1)
                printf(".");
            else if (maze[y][x] == 0)
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }

}



void* solve(void* arg) {
    int x = ((int*)arg)[0];
    int y = ((int*)arg)[1];

    if (maze[y][x] == 3) {
        printf("Solution trouvée !\n");
        affichageSol();
        pthread_exit(NULL);
    }



 while (maze[y][x] != 3) {// Tant qu'on n'a pas atteint la sortie
          maze[y][x] = -1; // Marquer la case comme visitée
    // Chercher le chemin suivant
    if (x > 0 && maze[y][x-1] != 0 && maze[y][x-1] != -1) { // Gauche
        int next_pos[2] = {x-1, y};
        pthread_t thread;
        pthread_create(&thread, NULL, solve, (void*)next_pos);
    }
    if (y > 0 && maze[y-1][x] != 0 && maze[y-1][x] != -1) { // Haut
        int next_pos[2] = {x, y-1};
        pthread_t thread;
        pthread_create(&thread, NULL, solve, (void*)next_pos);
    }
    if (x < width-1 && maze[y][x+1] != 0 && maze[y][x+1] != -1) { // Droite
        int next_pos[2] = {x+1, y};
        pthread_t thread;
        pthread_create(&thread, NULL, solve, (void*)next_pos);
    }
    if (y < height-1 && maze[y+1][x] != 0 && maze[y+1][x] != -1) { // Bas
        int next_pos[2] = {x, y+1};
        pthread_t thread;
        pthread_create(&thread, NULL, solve, (void*)next_pos);
    }

    pthread_exit(NULL);
}
}


void solutionThread() {
    // Recherche la position (x,y) de E (2) l'entrée du labyrinthe
    int x = 0, y = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(maze[i][j] == 2) {
                x = j;
                y = i;
                break;
            }
        }
    }

    // Création du thread principal
    pthread_t thread;
    int pos[2] = {x, y};
    pthread_create(&thread, NULL, solve, (void*)pos);

    // Attente de la fin du thread
    pthread_join(thread, NULL);
}




int main() {
  // Lecture du labyrinthe depuis un fichier
    lectureFich();

    printf("Labyrinthe original :\n");
    affichageSimple();

    // Résolution du labyrinthe en utilisant un seul thread
    printf("Labyrinthe résolu avec une solution simple :\n");
    solutionSimple();
    affichageSol();

    // Résolution du labyrinthe en utilisant des threads
    printf("Labyrinthe résolu avec des threads :\n");
    // Attente de la fin du thread principal
    solutionThread();
    affichageSol();

    return 0;
}
