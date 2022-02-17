#include "cgen.h"
#include "TuL.h"
#include "ext/io.h"
#include <stdio.h>
#include <string.h>
#include "stack.h"

#define s_length 11

typedef char s2[s_length];

void pointCal(int *sc1,int *sc2){
	int cal = *sc1 - *sc2;
	if (!cal){*sc1=*sc2=1;return;}
	if (cal>0){*sc1=3;*sc2=0;return;}
	else {*sc1=0;*sc2=3;return;}
}

void freeS(gtype s){
	free (s.s);
} 

rbm_t readfile (char *fname){
    FILE* f1= fopen(fname,"r"); if(!f1) {return NULL;}
    s2 n1,n2;int sc1,sc2;rbm_ires res;char*tmps;
	static int n=0;
	char * tmpline = NULL;
	rbm_t m = rbm_create(gtype_cmp_s,freeS,NULL);
    while(cgetline(&tmpline,0,f1)){
		remove_tail_lf(tmpline);
		sscanf(tmpline,"%s\t%s\t%d\t%d",n1,n2,&sc1,&sc2);
		pointCal(&sc1,&sc2);
		
		tmps = strdup(n1);
		res = rbm_insert(m,gtype_s(tmps),gtype_l(sc1));
		if(!res.inserted){res.value->l +=sc1;free(tmps);}
		else{n++;printf("nut thu %d, dia chi %p\n",n,rbm_search(m,gtype_s(n1)));}

		tmps = strdup(n2);
		res = rbm_insert(m,gtype_s(tmps),gtype_l(sc2));
		if(!res.inserted){res.value->l +=sc2;free(tmps);}
		else{n++;printf("nut thu %d, dia chi %p\n",n,rbm_search(m,gtype_s(n2)));}
    };
	fclose(f1);
	free(tmpline);
    return m;
}


void printFomat(gtype*k,gtype*v){
		printf("team %s get %ld point(s)\n",k->s,v->l);
}

void printNode(rbm_node_t node){
	printFomat(&(node->key),&(node->value));
}

void printData(rbm_t data){
	rbm_traverse (k,v,data){
		printFomat(k,v);
	}
}

void inFile(char*fname,rbm_t data){
	FILE*f = fopen(fname,"w+");
	rbm_traverse (k,v,data){
		fprintf(f,"%s\t%ld\n",k->s,v->l);
	}
	fclose(f);
}

void timkiem(rbm_t data){
	s2 find;
	printf("tim kiem :");
	scanf("%s",find);
	rbm_node_t node= rbm_search(data,gtype_s(find));
	if(!node){printf("ko co doi nay\n");}
	else{printNode(node);}
}

void xuonghang(rbm_t data){
	int X;
	printf("diem X :");
	scanf("%d",&X);
	gvec_t delist = gvec_create(5,NULL);
	rbm_traverse(k,v,data){
		if(v->l<X) gvec_append(delist,*k);
	}
	gvec_traverse(cur,delist){
		rbm_remove(data,*cur);
	}
	gvec_free(delist);
	printData(data);
}

void menu(){
	printf("Chon che do\n1.Doc file\n2.hien thi\n3.tim kiem\n4.loc danh sach\n5.In danh sach\n6.Thoat\n");
}

int main(int argv, char* argc[]){
	int mode;
	rbm_t data =NULL;
	do{
		menu();
		scanf("%d",&mode);
		switch(mode){
			case 1:data = readfile("../BongDa.txt");break;
			case 2:printData(data);break;
			case 3:timkiem(data);break;
			case 4:xuonghang(data);break;
			case 5:inFile("KetQua.txt",data);break;
			case 6:printf("exiting\n");

		}
	}while(mode!=6);

	rbm_free(data);
	
	
}