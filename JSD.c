#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<fcntl.h>
#include<ctype.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<dirent.h>
#include<math.h>


typedef struct Trie{
    char value;
    int end;
    int count;
    float fre;
    struct Trie *bro;
    struct Trie *son;
} Trie;


int Tr_ini(Trie *T){
    T->fre=0.0;
    T->end=2;
    T->count=0;
    T->son = NULL;
    T->bro = NULL;
    return 0;
}

int Tr_init(Trie *T,char k){

    T->end=0;
    T->value=k;
    T->count=0;
    T->fre=0.0;
    T->son = NULL;
    T->bro = NULL;
    return 0;
}

int Tr_addWord(Trie *Te, char *k){
    Trie *T=Te;
    if (T->end==2) {
      T->end=0;
      T->value=k[0];
      T->son = NULL;
      T->bro = NULL;
      if (strlen(k)==1) {
        T->end=1;
        T->count=1;
        return 0;
      }
      int t;
      t=Tr_addWord(T,k);
      return t;
    }
    if(strlen(k)==1){
      while (T->value!=k[0]) {
        if (T->bro==NULL) {
          Trie *tem=(Trie*)malloc(sizeof(Trie));
          if (!tem) return 1;
          Tr_init(tem, k[0]);
          tem->count=1;
          tem->end=1;
          T->bro=tem;
          return 0;
        }
        T=T->bro;
      }
      T->end=1;
      T->count=T->count+1;
      return 0;
    }
    while (T->value!=k[0]) {
      if (T->bro==NULL) {
        T->bro=(Trie*)malloc(sizeof(Trie));
        if (!T->bro) return 1;

        Tr_init(T->bro, k[0]);

        int t;
        char *de=malloc(sizeof(char) * (strlen(k)));
        if (!de) return 1;
        strncpy(de,k+1,strlen(k)-1);

        de[strlen(k)-1]='\0';
        T=T->bro;
        T->son=(Trie*)malloc(sizeof(Trie));
        if (!T->son) return 1;
        Tr_init(T->son,k[1]);

        t=Tr_addWord(T->son,de);
        free(de);
        return t;
      }
      T=T->bro;
    }
    if (T->son==NULL) {
      Trie *tem=(Trie*)malloc(sizeof(Trie));
      if (!tem) return 1;
      Tr_init(tem,k[1]);
      T->son=tem;
    }
    char *des=malloc(sizeof(char) * (strlen(k)));
    if (!des) return 1;
    int t;
    strncpy(des,k+1,strlen(k)-1);
    des[strlen(k)-1]='\0';
    t=Tr_addWord(T->son,des);
    free(des);
    return t;
}

Trie* Tr_copy(Trie *T){
  if (T==NULL) {
    return NULL;
  }
  Trie *Tz=(Trie*)malloc(sizeof(Trie));
  Tz->end=T->end;
  Tz->value=T->value;
  Tz->fre=T->fre;
  Tz->count=T->count;
  Tz->son = Tr_copy(T->son);
  Tz->bro = Tr_copy(T->bro);
  return Tz;
}

Trie* Tr_copyban(Trie *T){
  if (T==NULL) {
    return NULL;
  }
  Trie *Tz=(Trie*)malloc(sizeof(Trie));
  Tz->end=T->end;
  Tz->value=T->value;
  Tz->fre=T->fre/2;
  Tz->count=T->count;
  Tz->son = Tr_copyban(T->son);
  Tz->bro = Tr_copyban(T->bro);
  return Tz;
}
void Tr_destroy(Trie *T){
  if (T->bro!=NULL) {
    Tr_destroy(T->bro);
  }
  if (T->son!=NULL) {
    Tr_destroy(T->son);
  }
  free(T);
  return;
}

int Tr_He(Trie *Tzz,Trie *Tyy,Trie *Tx){
  int fn=0;
  if (Tzz==NULL&&Tyy==NULL) {
    Tx=NULL;
    return 0;
  }
  if (Tzz==NULL) {
    Tx->end=Tyy->end;
    Tx->value=Tyy->value;
    Tx->fre=Tyy->fre/2;
    Tx->count=Tyy->count;
    Tx->son = Tr_copyban(Tyy->son);
    Tx->bro = Tr_copyban(Tyy->bro);

    return 0;
  }
  if (Tyy==NULL) {
    Tx->end=Tzz->end;
    Tx->value=Tzz->value;
    Tx->fre=Tzz->fre/2;
    Tx->count=Tzz->count;
    Tx->son = Tr_copyban(Tzz->son);
    Tx->bro = Tr_copyban(Tzz->bro);

    return 0;
  }
  Trie *Tz=Tzz;
  Trie *Cur=Tx;
  Trie *Tyyy=Tr_copy(Tyy);
  int fl=1;
  while (Tz) {
    Trie *Ty=Tyyy;
    Trie *Ty2=Tyyy;
    int flag=1;
    int fla=0;
    if (fl==0) {
      Cur=Cur->bro;
    }
    while (Ty) {
      if (Tz->value==Ty->value) {
        Tr_init(Cur,Tz->value);
        Cur->count=Cur->count+Ty->count+Tz->count;
        Cur->fre=(Ty->fre+Tz->fre)/2;
        Cur->end=0;
        if (Cur->count!=0) {
          Cur->end=1;
        }
        Trie *tem=(Trie*)malloc(sizeof(Trie));
        if (!tem) return 1;
        tem->end=3;
        Cur->son=tem;
        fn=Tr_He(Tz->son,Ty->son,Cur->son);
        if (Cur->son->end==3) {
          free(Cur->son);
          Cur->son=NULL;
        }
        fla=1;
        if (flag==1){
          Trie *Tyy=Tyyy;
          Tyyy=Ty2->bro;
          if (Tyy->son!=NULL) {
            Tr_destroy(Tyy->son);
          }
          free(Tyy);
          break;
        }
        Ty2->bro=Ty->bro;
        if (Ty->son!=NULL) {
          Tr_destroy(Ty->son);
        }
        free(Ty);
        break;
      }
      if (flag==0) {
        Ty2=Ty2->bro;
      }
      flag=0;
      Ty=Ty->bro;
    }

    if (fla==0) {
      Cur->end=Tz->end;
      Cur->value=Tz->value;
      Cur->fre=(Tz->fre)/2;
      Cur->count=Tz->count;
      Cur->son = Tr_copyban(Tz->son);
    }
    Trie *tem=(Trie*)malloc(sizeof(Trie));
    if (!tem) return 1;
    Cur->bro = tem;
    Tz=Tz->bro;
    fl=0;
  }
  free(Cur->bro);
  if (!Tyyy) {
    free(Tyyy);
    Cur->bro=NULL;
    return fn;
  }
  Cur->bro=Tr_copyban(Tyyy);
  Tr_destroy(Tyyy);
  return fn;
}

int generateTr(int fd,Trie *T){
	int bytes;
	char b[1];
	while ((bytes = read(fd, b, 1)) > 0) {
    int size=3;
		char *buf= malloc(sizeof(char)*size);
    if (!buf) return 1;
		while (b[0]==' '||b[0]=='\n') {
			bytes=read(fd, b, 1);
			if(bytes<=0) break;
		}
		int i=0;
		while (b[0]!='\n'&&b[0]!=' '&&bytes>0) {
			if (i>=size) {
				size=size*2;
				char *p = realloc(buf, sizeof(char) * size);
				buf=p;
				}
      b[0]=tolower(b[0]);
      if (isalpha(b[0]) || isdigit(b[0])||b[0]=='-') {
        buf[i]=b[0];
  			i++;
      }
			bytes=read(fd, b, 1);
		}
		char *p = realloc(buf, sizeof(char) * i+2);
    if (!p) return 1;
		buf=p;
		buf[i]='\0';

    if (strlen(buf)!=0) {
      Tr_addWord(T,buf);
    }

    free(buf);
	}
  if (bytes < 0) {
		perror("Read error");
  }
  close(fd);
	return 0;
}

float Tr_totalC(Trie *T){
  if (!T) {
    return 0.0;
  }
    return (float)T->count+Tr_totalC(T->son)+Tr_totalC(T->bro);
}

void Tr_fre(Trie *T,int toa){
  if (T==NULL) {
    return;
  }
  T->fre=(float)T->count/(float)toa;
  Tr_fre(T->son,toa);
  Tr_fre(T->bro,toa);
}
float KLD(Trie *Tx,Trie *Ty){
  if (Tx==NULL) {
    return 0.0;
  }
  Trie *Tyy=Ty;
  while (Tyy) {
    if (Tx->value==Tyy->value) {
      if (Tyy->fre!=0 && Tx->fre!=0) {
        float value = Tx->fre*log(Tx->fre/Tyy->fre)/log(2)+KLD(Tx->bro,Ty)+KLD(Tx->son,Tyy->son);
        return value;
      }
      float value = KLD(Tx->bro,Ty)+KLD(Tx->son,Tyy->son);

      return value;
    }
    Tyy=Tyy->bro;
  }

  return -1;
}

float JSD(Trie *Tx,Trie *Ty){
  if (Tx->end==2||Ty->end==2) {
    if (Tx->end==2&&Ty->end==2) {
      return 0.0000;
    }
    return 1.0000;
  }
  float toaTx=Tr_totalC(Tx);
  Tr_fre(Tx, toaTx);
  float toaTy=Tr_totalC(Ty);
  Tr_fre(Ty, toaTy);

  Trie *Tz=(Trie*)malloc(sizeof(Trie));
  if (!Tz) return -1;
  Tr_He(Tx,Ty,Tz);

  float re=sqrt(KLD(Tx,Tz)/2+KLD(Ty,Tz)/2);
  Tr_destroy(Tz);
  return re;

}

typedef struct {
	char** data;
	unsigned count;
	unsigned head;
	pthread_mutex_t lock;
} queue_B;
int qb_init(queue_B *Q){
	Q->data = malloc(0);
	Q->count = 0;
	Q->head = 0;
	pthread_mutex_init(&Q->lock, NULL);
	return 0;
}
int qb_destroy(queue_B *Q){
	pthread_mutex_destroy(&Q->lock);
	free(Q->data);
	return 0;
}
int qb_enqueue(queue_B *Q, char* item){
	pthread_mutex_lock(&Q->lock);
	unsigned i = Q->count;
	Q->data = realloc(Q->data, (Q->count+1)*sizeof(Q->data[0]));
	Q->data[i] = malloc(sizeof(char) * (strlen(item)+1));
	strcpy(Q->data[i], item);
	++Q->count;
	pthread_mutex_unlock(&Q->lock);
	return 0;
}
int qb_dequeue(queue_B *Q, char **item){
	pthread_mutex_lock(&Q->lock);

	if (Q->count == Q->head) {
		pthread_mutex_unlock(&Q->lock);
		return -1;
	}
  *item = malloc(sizeof(char) * (strlen( Q->data[Q->head])+1));
	strcpy(*item, Q->data[Q->head]);
  free(Q->data[Q->head]);
	++Q->head;
	pthread_mutex_unlock(&Q->lock);
	return 0;
}

typedef struct {
	char** data;
	int tsize;
	Trie** t;
	unsigned count;
	unsigned head;
	pthread_mutex_t lock;
} queue_U;
int qu_init(queue_U *Q){
	Q->t=malloc(0);
	Q->data = malloc(0);
	Q->tsize=0;
	Q->count = 0;
	Q->head = 0;
	pthread_mutex_init(&Q->lock, NULL);
	return 0;
}
int qu_destroy(queue_U *Q){
	pthread_mutex_destroy(&Q->lock);
  for (size_t i = 0; i < Q->head; i++) {
    Tr_destroy(Q->t[i]);
    free(Q->data[i]);
  }
  free(Q->t);
	free(Q->data);
	return 0;
}
int qu_enqueue(queue_U *Q, char* item){
	pthread_mutex_lock(&Q->lock);
	unsigned i = Q->count;
  Q->data = realloc(Q->data, (Q->count+1)*sizeof(Q->data[0]));
	Q->data[i] = malloc(sizeof(char) * (strlen(item)+1));
	strcpy(Q->data[i], item);
	++Q->count;
	pthread_mutex_unlock(&Q->lock);
	return 0;
}
int qu_dequeue(queue_U *Q){
	pthread_mutex_lock(&Q->lock);

	if (Q->count == Q->head) {
		pthread_mutex_unlock(&Q->lock);
		return -1;
	}

	Q->tsize=Q->tsize+1;
	Q->t = realloc(Q->t, Q->tsize*sizeof(Trie));
	Q->t[Q->head]=(Trie*)malloc(sizeof(Trie));
	Tr_ini(Q->t[Q->head]);
	int fd;
	fd = open(Q->data[Q->head], O_RDWR);
  if (!fd){
      perror("error\n");
      return -2;
  }
	generateTr(fd, Q->t[Q->head]);
	++Q->head;
	pthread_mutex_unlock(&Q->lock);
  close(fd);
	return 0;
}

int isdir(char *name) {
	struct stat data;

	int err = stat(name, &data);

	// should confirm err == 0
	if (err) {
		perror(name);  // print error message
		return 0;
	}

	if (S_ISDIR(data.st_mode)) {
		// S_ISDIR macro is true if the st_mode says the file is a directory
		// S_ISREG macro is true if the st_mode says the file is a regular file

		return 1;
	}

	return 0;
}


typedef struct dArgs {
	queue_U *U;
	queue_B *B;
	char* suf;
}dArgs;
typedef struct fArg {
	queue_U *U;
}fArg;

typedef struct aArg {
	queue_U *U;
  int start;
  int end;
}aArg;

int suff(char* str, char* suffix){
    int slen = strlen(str);
    int changedslen = strlen(suffix);
    if(changedslen <= slen && strcmp(str + slen - changedslen, suffix)!=0){
    	return 1;
	}
	return 0;
}

void* directory_threads(void* in){

    int* retval = malloc(sizeof(int));
    *retval = EXIT_SUCCESS;
    dArgs* arg = (dArgs*) in;
    queue_B* QueueDir = arg->B;
    queue_U* QueueFile = arg->U;
    char* suffix = arg->suf;
		while (QueueDir->count!=QueueDir->head) {
      char* dirname;
      char* str;
      qb_dequeue(QueueDir,&dirname);
      struct dirent *de;
      DIR *dirp = opendir(dirname);
      if (!dirp){
          perror("error\n");
          *retval = EXIT_FAILURE;
          return retval;
      }
      while ((de = readdir(dirp))) {

          str = de->d_name;
          if(str[0] != '.'){
              int length = strlen(dirname)+strlen(str)+2;
              char* subpath = malloc(length * sizeof(char));
              subpath[0]='\0';
              strcat(subpath, dirname);
              subpath[strlen(dirname)]='/';
              subpath[strlen(dirname)+1]='\0';
              strcat(subpath, str);
              subpath[strlen(dirname)+strlen(str)+1]='\0';

              if (!isdir(subpath) && !suff(str, suffix)) {
                qu_enqueue(QueueFile, subpath);
              }
              else if (isdir(subpath)){
                  qb_enqueue(QueueDir, subpath);
              }
              free(subpath);
          }
      }
      closedir(dirp);
      free(de);
      free(dirname);
    }

    return retval;
	}

void* file_threads(void* in){
      int* retval = malloc(sizeof(int));
      *retval = EXIT_SUCCESS;
      fArg* arg = (fArg*) in;
      queue_U* QueueFile = arg->U;
      while(1){
        int o=0;
        o=qu_dequeue(QueueFile);
        if (o==-1) {
          break;
        }
        if (o==-2) {
          *retval = EXIT_FAILURE;
          break;
        }
      }
      return retval;
  }

void* analysis_threads(void* in){
      int* retval = malloc(sizeof(int));
      *retval = EXIT_SUCCESS;
      aArg* arg = (aArg*) in;
      int start = arg->start;
      int end=arg->end;
      queue_U* QueueFile = arg->U;
      if (start==0) {
        for (int d = 1; d < QueueFile->head; d++) {
          if (JSD(QueueFile->t[0],QueueFile->t[d])<0||JSD(QueueFile->t[0],QueueFile->t[d])>1) {
            perror("error\n");
            *retval = EXIT_FAILURE;
          }
          printf("%f ", JSD(QueueFile->t[0],QueueFile->t[d]));
          printf("%s ", QueueFile->data[0]);
          printf("%s\n", QueueFile->data[d]);
        }
      }
      for (int i = start+1; i <= end; i++) {
        for (int d = i+1; d < QueueFile->head; d++) {
          if (JSD(QueueFile->t[0],QueueFile->t[d])<0||JSD(QueueFile->t[0],QueueFile->t[d])>1) {
            perror("error\n");
            *retval = EXIT_FAILURE;
          }
          printf("%f ", JSD(QueueFile->t[i],QueueFile->t[d]));
          printf("%s ", QueueFile->data[i]);
          printf("%s\n", QueueFile->data[d]);
        }
      }
      return retval;
    }

int first(char* str, char* prefix){
  	int i;
      for(i = 0; i < strlen(prefix); i++){
          if(str[i]!= prefix[i]){
              return 1;
          }
      }
      return 0;
  }


int normal(int argc, char **argv, char* suf, queue_U* QueueFile, queue_B* QueueDir){
	int i;
    for(i = 1; i < argc; i++){
        if(first(argv[i], "-") == 0){
            continue;
        }
        else if(isdir(argv[i])){
            qb_enqueue(QueueDir, argv[i]);

        }
        else if(!isdir(argv[i])){
					if(!suff(argv[i], suf)){
							qu_enqueue(QueueFile, argv[i]);
					}
        }
    }
    return EXIT_SUCCESS;
}

int opt(int argc, char **argv, int* dThread, int* fThread, int* aThread, char** suf){
	int i;
    for(i = 1; i < argc; i++){
        if(first(argv[i], "-") == 0){
            int length = strlen(argv[i])-1;
            char* substr = malloc(length);
            memcpy(substr, argv[i]+2, length);
            substr[length - 1] = '\0';
            if(first(argv[i], "-s") == 0){
                int len = strlen(substr)+1;
                *suf = realloc(*suf, len);
                strcpy(*suf, substr);
            }
            else if(first(argv[i], "-f") == 0){
                *fThread = atoi(substr);
            }
            else if(first(argv[i], "-d") == 0){
                *dThread = atoi(substr);
            }
            else if(first(argv[i], "-a") == 0){
                *aThread = atoi(substr);
            }
            free(substr);
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv){
	int dThread, fThread, aThread;
	dThread = 1;
	fThread = 1;
	aThread = 1;
  queue_U QueueFile;
  queue_B QueueDir;
  qu_init(&QueueFile);
  qb_init(&QueueDir);
  char* defaultSuffix = ".txt";
  char* suf = malloc(sizeof(defaultSuffix) + 1);
  memcpy(suf, defaultSuffix, strlen(defaultSuffix));
  suf[strlen(defaultSuffix)] = '\0';
  int rc = EXIT_SUCCESS;
  rc = opt(argc, argv, &dThread, &fThread, &aThread, &suf);
  void* retval = NULL;
  normal(argc, argv, suf, &QueueFile, &QueueDir);
  pthread_t* dTids = (pthread_t*)malloc(dThread * sizeof(pthread_t));
	pthread_t* fTids = (pthread_t*)malloc(fThread * sizeof(pthread_t));
  pthread_t* aTids = (pthread_t*)malloc(aThread * sizeof(pthread_t));
  dArgs darg;
  int i;
  darg.B = &QueueDir;
  darg.U = &QueueFile;
  darg.suf = suf;
  for(i = 0; i < dThread; i++){
      pthread_create(&dTids[i], NULL, directory_threads, &darg);
  }
  for(i = 0; i < dThread; i++){
      pthread_join(dTids[i], &retval);
      if (*((int*)retval) == EXIT_FAILURE){
          rc = EXIT_FAILURE;
      }
      free(retval);
  }
  free(dTids);
  qb_destroy(&QueueDir);
  fArg farg;
  farg.U = &QueueFile;
  for(i = 0; i < fThread; i++){
      pthread_create(&fTids[i], NULL, file_threads, &farg);
  }
  for(i = 0; i < fThread; i++){
      pthread_join(fTids[i], &retval);
      if (*((int*)retval)==EXIT_FAILURE) {
        rc = EXIT_FAILURE;
      }
      free(retval);
  }
  if (QueueFile.head<=1) {
    perror("less than 2 files found in the collection phase");
    qu_destroy(&QueueFile);
    free(suf);
    free(aTids);
    free(fTids);
    return EXIT_FAILURE;
  }
  free(fTids);
  aArg* aarg = malloc(aThread*sizeof(aArg));
  int le=QueueFile.head;
  int mo=le%aThread;
  int md=mo;
  int step=(le-mo)/aThread;
  if ((le-mo)/aThread==0) {
    step=1;
    md=0;
  }
  int coo=0;
  int con=0;
  for(i = 0; i < aThread; i++){
      aarg[i].start=coo;
      if ( coo+md+step>=le ) {
        aarg[i].end=le;
        aarg[i].U = &QueueFile;
        con++;
        break;
      }
      aarg[i].end=coo+step;
      coo=coo+step;
      aarg[i].U = &QueueFile;
      con++;
  }

  for(i = 0; i < con; i++){
      pthread_create(&aTids[i], NULL, analysis_threads, &aarg[i]);
  }
  for(i = 0; i < con; i++){
      pthread_join(aTids[i], &retval);
      if (*((int*)retval)==EXIT_FAILURE) {
        rc = EXIT_FAILURE;
      }
      free(retval);
  }
  free(aTids);
  free(aarg);
  free(suf);
  qu_destroy(&QueueFile);
  return rc;
}
