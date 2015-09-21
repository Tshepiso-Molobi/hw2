
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>


pthread_mutex_t m_tex;
typedef struct dict {
  char *word;
  int count;
  struct dict *next;
} dict_t;
dict_t *d;
char *
make_word( char *word ) {
  return strcpy( malloc( strlen( word )+1 ), word );
}

dict_t *
make_dict(char *word) {
  dict_t *nd = (dict_t *) malloc( sizeof(dict_t) );
  nd->word = make_word( word );
  nd->count = 1;
  nd->next = NULL;
  return nd;
}

dict_t *
insert_word( dict_t *d, char *word ) {
  
  
  dict_t *nd;
  dict_t *pd = NULL;		
  dict_t *di = d;		
  
  while(di && ( strcmp(word, di->word ) >= 0) ) { 
    if( strcmp( word, di->word ) == 0 ) { 
      di->count++;		
      return d;			
    }
    pd = di;			
    di = di->next;
  }
  nd = make_dict(word);		 
  nd->next = di;		 
  if (pd) {
    pd->next = nd;
    return d;			
  }
  return nd;
}

void print_dict(dict_t *d) {

  while (d) {
    printf("[%d] %s\n", d->count, d->word);
    d = d->next;
  }

}

int
get_word( char *buf, int n, FILE *infile) { 

  int inword = 0;
  int c;  
  while( (c = fgetc(infile)) != EOF ) {
    if (inword && !isalpha(c)) {
      buf[inword] = '\0';	
      return 1;
    } 
    if (isalpha(c)) {
      buf[inword++] = c;
    }
  
    }
  return 0;	
}

#define MAXWORD 3500
void * dic_words(void * args){

  FILE * infile = (FILE *) args;
  
  char w_buf[MAXWORD];
  
  while(get_word( w_buf, MAXWORD, infile ) ) {

    d = insert_word(d, w_buf); 
  }
 
}

int
main( int argc, char *argv[] ) {

if(pthread_mutex_init(&m_tex,NULL)!=0){
printf("Mutex lock not created...\n");
return 1;
}
   d = NULL;

  FILE *infile = stdin;
  if (argc >= 2) {
    infile = fopen(argv[1],"r");
  }
  if( !infile ) {
    printf("Unable to open %s\n",argv[1]);
    exit( EXIT_FAILURE );
  }
 

//int i=0;
//while(i<4){


//i++;
//}

pthread_t thr_1, thr_2, thr_3, thr_4, thr_5, thr_6;

pthread_create(&thr_1, NULL, &dic_words, infile);
pthread_join(thr_1, NULL);

pthread_create(&thr_2, NULL, &dic_words, infile);
pthread_join(thr_2, NULL);

pthread_create(&thr_3, NULL, &dic_words, infile);
pthread_join(thr_3, NULL);

pthread_create(&thr_4, NULL, &dic_words, infile);
pthread_join(thr_4, NULL);

pthread_create(&thr_5, NULL, &dic_words, infile);
pthread_join(thr_5, NULL);

pthread_create(&thr_6, NULL, &dic_words, infile);
pthread_join(thr_6, NULL);

print_dict(d);



  fclose( infile );
}

