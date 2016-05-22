#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

unsigned fq[256];// retine nr de aparitii al fiecarui caracter
char *cods[256];// retine codificarea fiecarui caracter

struct arb_node{
	unsigned char c;
	struct arb_node *st, *dr;

};

struct list_node {
	int v;
	unsigned char c;
	struct list_node *next;
	struct list_node *st, *dr;
};

void sorted_insert(struct list_node **root, int v, char c, struct list_node *st, struct list_node *dr){

	struct list_node *aux=malloc(sizeof(struct list_node));
	aux->v=v;
	aux->c=c;
	aux->st=st;
	aux->dr=dr;

	if (*root==NULL){
		aux->next=NULL;
		*root=aux;
	}
	else {
		struct list_node *it=*root;
		if (v < it->v) {//insereaza la inceput
			aux->next=*root;
			*root=aux;
		}
		else {
			while (it->next!=NULL){//insereaza intre doua elemente
				if (it->v <= v && it->next->v > v) {
					aux->next=it->next;
					it->next=aux;
					break;
				}
				else it=it->next;
			}

			if (it->next==NULL){//insereaza la sfarsit
				it->next=aux;
				aux->next=NULL;
			}
		}
	}
}

void pop2(struct list_node** root){
	*root=(*root)->next->next;
}

void iter_tree(struct list_node* root, char codification[20], int level, int fd_out){

	if (root->st==NULL && root->dr==NULL){
		cods[root->c]=malloc(level*sizeof(char));
		memcpy(cods[root->c],codification,level);
		write(fd_out,&root->c,sizeof(char));
		write(fd_out,&level,sizeof(int));

		int i;
		for(i=0;i<level;i++)
			write(fd_out,&codification[i],sizeof(char));
	}
	else{
		if (root->st!=NULL){
			codification[level]='0';
			iter_tree(root->st, codification, level+1, fd_out);
		}
		if (root->dr!=NULL){
			codification[level]='1';
			iter_tree(root->dr, codification, level+1, fd_out);
		}
	}
}

void compress(char* path_in, char* path_out){

	int fd_in=open(path_in, O_RDONLY, 0644);
	int fd_out=open(path_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	long size=0;
	unsigned i,j;
	unsigned char buf[1024];
	unsigned r;
	r=read(fd_in,&buf,1024);

	while(r!=0){
		size+=r;
		for(i=0;i<r;i++){
			fq[buf[i]]++;
			//printf("%d %d\n",buf[i],fq[buf[i]]);
		}
		r=read(fd_in,&buf,1024);
	}

	int count=0;
	struct list_node *root=NULL;
	for(r=0;r<256;r++){
		if (fq[r]!=0) {
			count++;
			sorted_insert(&root, fq[r],r, NULL, NULL);
		}
	}

	write(fd_out, &size, sizeof(size));
	write(fd_out, &count, sizeof(count));

	int v;
	while(count>1){
		count--;
		v=root->v+root->next->v;
		sorted_insert(&root,v,' ',root,root->next);
		pop2(&root);
	}

	// scriu codificarile pentru fiecare caracter
	char cod[20]="";
	iter_tree(root, cod, 0, fd_out);

	// scriu textul codificat
	lseek(fd_in,0,SEEK_SET);
	char aux=0,index=7;

	r=read(fd_in,&buf,1024);
	while(r!=0){
		for(i=0;i<r;i++){
			for(j=0;j<strlen(cods[buf[i]]);j++){
				if (cods[buf[i]][j]=='1') aux |= 1 << index;
				index--;
				if (index<0) {
					write(fd_out,&aux,sizeof(char));
					aux=0;
					index=7;
				}
			}
		}
		r=read(fd_in,&buf,1024);
	}
	if (index!=7) write(fd_out,&aux,sizeof(char));

	close(fd_in);
	close(fd_out);
}

void tree_insert(struct arb_node **root, unsigned char c, int length, char codification[20]){

	int i;
	struct arb_node *it=*root;
	for(i=0;i<length;i++){
		if (codification[i]=='0'){
			if(it->st==NULL) {
				struct arb_node *aux=malloc(sizeof(struct arb_node));
				aux->st=NULL;
				aux->dr=NULL;
				it->st=aux;
			}
			it=it->st;
		}
		else if (codification[i]=='1'){
			if(it->dr==NULL) {
				struct arb_node *aux=malloc(sizeof(struct arb_node));
				aux->st=NULL;
				aux->dr=NULL;
				it->dr=aux;
			}
			it=it->dr;
		}
	}
	it->c=c;
}

void decompress(char* path_in, char* path_out){

	int fd_in=open(path_in, O_RDONLY, 0644);
	int fd_out=open(path_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);

	long size;
	int i,j,length,r,count;
	char codification[20];
	unsigned char c,bit;
	struct arb_node *root=malloc(sizeof(struct arb_node));
	root->st=NULL;
	root->dr=NULL;

	// mai intai reconstruiesc arborele pentru
	// ca am nevoie de el la decodificare
	read(fd_in,&size,sizeof(size));
	read(fd_in,&count,sizeof(count));
	for(i=0;i<count;i++){
		read(fd_in,&c,sizeof(char));
		read(fd_in,&length,sizeof(int));
		memset(codification,0,20);
		for(j=0;j<length;j++)
			read(fd_in,&codification[j],sizeof(char));
		tree_insert(&root,c,length,codification);
	}

	r=read(fd_in,&c,sizeof(char));
	struct arb_node *it=root;
	while(r!=0){
		for(i=7;i>=0;i--){
			if (size==0) break;
			bit = (c >> i) & 1;
			if (bit==0) it=it->st;
			else if (bit==1) it=it->dr;

			if (it->st==NULL && it->dr==NULL){
				write(fd_out,&it->c,sizeof(char));
				it=root;
				size--;
			}
		}
		if (size==0) break;
		r=read(fd_in,&c,1);
	}

	close(fd_in);
	close(fd_out);
}

int main(int argc, char *argv[]){

	if (argc!=4) {
		printf("must be called:\n%s <command> <input_file_path> <output_file_path>\n",argv[0]);
		return -1;
	}

	if (strcmp(argv[1],"c")==0) compress(argv[2], argv[3]);
	else if (strcmp(argv[1],"d")==0) decompress(argv[2], argv[3]);
	else printf("Error, command not recognized\n");

	return 0;
}
