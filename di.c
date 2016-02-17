#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#define INFINITY 99999

typedef struct vertex{
	char name[9];
	int idx;
}Vert;

typedef struct edge{
	Vert A;
	Vert B;
	int distance;
}Edge;

typedef struct vertList{
	Vert vertex;
	struct vertList* next;
}VertList;

typedef struct edgeList{
	Edge edge;
	struct edgeList* next;
}EdgeList;

typedef struct graph{
	VertList* vList;
	EdgeList* eList;
	int vCnt;
	int** gArr;
}Graph;

void readEdge(FILE* fp, Graph* G){		//read edges from text file
	int i=0;
	EdgeList* eList;
	char tmpStr[30];
	char tmpA[9];
	char tmpB[9];
	int tmpDis =0 ;

	eList = (EdgeList*)malloc(sizeof(EdgeList));

	while(!feof(fp)){
		fgets(tmpStr,30,fp);
		if(strlen(tmpStr)<3) continue;
		sscanf(tmpStr, "%s %s %d", tmpA, tmpB, &tmpDis);
		if(strlen(tmpA)<1) continue;
		if(tmpDis==0) continue;
		if(i==0){
			G->eList = eList;
			i=1;
		}
		eList->edge.distance = tmpDis;
		if(strcmp(tmpA,tmpB)>0){		//smaller vertex set to A
			strcpy(eList->edge.A.name,tmpB);
			strcpy(eList->edge.B.name,tmpA);
		}
		else{
			strcpy(eList->edge.A.name,tmpA);
			strcpy(eList->edge.B.name,tmpB);
		}

		eList->next = (EdgeList*)malloc(sizeof(EdgeList));	
		eList = eList->next;
		eList->next = NULL;
	}
	eList = G->eList;
	while(eList->next!=NULL){			//print read edges
		printf("%s\t%s\t%d\n",eList->edge.A.name,eList->edge.B.name,eList->edge.distance);
		eList = eList->next;
	}
}

void parseEdge(Graph* G){		//parse edges and get vertex list
	VertList* vList;
	VertList* vFirst;
	int i=0;
	int idx=0;
	EdgeList* eList;
	char tmpA[9];
	char tmpB[9];

	vList = (VertList*)malloc(sizeof(VertList));
	vList->next = NULL;
	vFirst = vList;
	eList = G->eList;

//edge list를 만들 때 마지막에 비어있는 edge가 하나 더 만들어 지기 때문에 마지막 edge 는 next값이 null, 들어 있는 값은 없는 edge가 된다.
//따라서 그 마지막 edge를 무시하기 위해 while의 검사를 eList->next가 null인 경우로 하여 실행한다
	while(eList->next!=NULL){			
		strcpy(tmpA,eList->edge.A.name);
		strcpy(tmpB,eList->edge.B.name);

		while(1){		//edge의 vertex A부터 index를 설정해준다
			if(strcmp(vList->vertex.name,tmpA)==0){
				eList->edge.A.idx = vList->vertex.idx;
				break;
			}
			else if(vList->next==NULL){	
				vList->next = (VertList*)malloc(sizeof(VertList));
				strcpy(vList->next->vertex.name,tmpA);
				vList->next->vertex.idx = idx;
				eList->edge.A.idx = idx;
				idx++;
				vList = vList->next;
				vList->next = NULL;
				break;
			}
			else vList = vList->next;
		}
		
		vList = vFirst;		//검사하기 위해 vertex list 처음으로 복귀

		while(1){			//edge의 vertex B의 index를 설정
			if(strcmp(vList->vertex.name,tmpB)==0){
				eList->edge.B.idx = vList->vertex.idx;
				break;
			}
			else if(vList->next==NULL){ 
//vertex list를 끝까지 보았는데도 vertex list에 현재 edge의 vertex 값이 없을 때
				vList->next = (VertList*)malloc(sizeof(VertList));
				strcpy(vList->next->vertex.name,tmpB);
				vList->next->vertex.idx = idx;
				eList->edge.B.idx = idx;
				idx++;
				vList = vList->next;
				vList->next = NULL;
				break;
			}
			else vList = vList->next;

		}
		eList = eList->next;
		vList = vFirst;
	}
	
	G->vList = vFirst->next;

	vList = G->vList;
	while(vList!=NULL){		//print name and index of vertices
		printf("%s : %d\n",vList->vertex.name,vList->vertex.idx);
		vList = vList->next;
	}
}

void mkArr(Graph* G){		//make 2 dimension array for dijstra algorithm
	int i,j;
	int n=0;		//the number of vertice
	int idx1, idx2;
	VertList* vList;
	EdgeList* eList;

	vList = G->vList;
	eList = G->eList;

	while(vList!=NULL){		//count the number of vertice
		n++;
		vList = vList->next;
	}
	G->vCnt = n;
	G->gArr = (int**)malloc(sizeof(int*)*n);
	for(i=0;i<n;i++){
		G->gArr[i] = (int*)malloc(sizeof(int)*n);
	}
	
	for(i=0;i<n;i++){		//array initialize
		for(j=0;j<n;j++){
			G->gArr[i][j] = -1;
		}
		G->gArr[i][i] = 0;
	}
	
	while(eList->next!=NULL){
		idx1 = eList->edge.A.idx;
		idx2 = eList->edge.B.idx;

		G->gArr[idx1][idx2] = G->gArr[idx2][idx1] = eList->edge.distance;

		eList = eList->next;
	}
	
	printf("[Adjacency matrix] \n");
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(G->gArr[i][j]<0)	printf("INF\t");
			else printf("%d\t",G->gArr[i][j]);
		}
		printf("\n");
	}

}

void printPath(int* prev, int src, int des, int* dist,int n,VertList* vFirst){
	int* path;
	int i=0,j=0;
	int v=des;
	VertList* vList;
	int* eachDist;		//distance for vertex to vertex
	
	vList = vFirst;	

	eachDist = (int*)malloc(sizeof(int)*n);
	path = (int*)malloc(sizeof(int)*n);
	for(i=0;i<n;i++){
		path[i] = -1;
		eachDist[i] = -1;
	}

	path[0] = v;
	i=1;
	while(v!=src){
		path[i] = prev[v];
		eachDist[j] = dist[v]-dist[prev[v]];
		v = prev[v];
		j++;
		i++;
	}

	for(j=0;j<n;j++){
		if(eachDist[j]<0) break;
	}

	j = j-1;
		
	for(i=n-1;i>=1;i--){
		if(path[i]<0) continue;
		while(vList!=NULL){
			if(vList->vertex.idx == path[i]){
				printf("%s -> ",vList->vertex.name);
				printf("(%d) -> ",eachDist[j]);
				j--;
				vList = vFirst;
				break;
			}
			vList = vList->next;
		}
	}
	vList = vFirst;
	while(vList!=NULL){
		if(vList->vertex.idx == path[0]){
			printf("%s\n",vList->vertex.name);
			break;
		}
		vList = vList->next;
	}
	printf("distance: %d\n",dist[des]);
}

int minV(int* dist, int* checked, int n){	//find min distance vertex
	int i;
	int min = INFINITY;
	int minIdx;

	for(i=0;i<n;i++){
		if(checked[i] != 1){
			if(min > dist[i]){
				min = dist[i];
				minIdx = i;
			}
		}
	}

	return minIdx;
}

void findShortest(Graph* G){	//find shortest path by dijkstra algorithm
	int* prev;
	int* dist;
	int* checked;
	char src[8];
	char des[8];
	VertList* vList;
	int n,i,v;
	int sIdx;
	int dIdx;
	int nowV;

	n = G->vCnt;	

	prev = (int*)malloc(sizeof(int)*n);
	dist = (int*)malloc(sizeof(int)*n);
	checked = (int*)malloc(sizeof(int)*n);

	for(i=0;i<n;i++){		//initialize distance and checked vertices arr
		dist[i] = INFINITY;
		checked[i] = 0;
	}

	printf("where is start vertex?: ");
	scanf("%s",src);
	fflush(stdin);
	printf("where is destination vertex?: ");
	scanf("%s",des);

	vList = G->vList;
	
	while(vList!=NULL){		//find index mapped source vertex 
		if(strcmp(vList->vertex.name,src)==0){
			sIdx = vList->vertex.idx;
			vList = G->vList;
			break;
		}
		else vList = vList->next;
	}
	if(vList == NULL){
		printf("input is not matched:src\n");
		return;
	}

	while(vList!=NULL){		//find index mapped destination vertex
		if(strcmp(vList->vertex.name,des)==0){
			dIdx = vList->vertex.idx;
			vList = G->vList;
			break;
		}
		else vList = vList->next;
	}
	if(vList == NULL){
		printf("input is not matched:des\n");
		return;
	}

	dist[sIdx] = 0;

	for(i=0;i<n;i++){		//find distance
		nowV = minV(dist,checked,n);
		checked[nowV] = 1;
		for(v=0; v<n; v++){
			if(G->gArr[nowV][v]>0 && dist[v]>dist[nowV]+G->gArr[nowV][v]){
				dist[v] = G->gArr[nowV][v] + dist[nowV];
				prev[v] = nowV;
			}
		}	
	}
	printPath(prev,sIdx,dIdx,dist,n,G->vList);

}

int main(int argc, char** args){
	FILE* fp;
	Graph* G;
	
	G  = (Graph*)malloc(sizeof(Graph));

	fp = fopen(args[1], "r");

	if(fp==NULL){
		printf("file open error:can't find file\n");
		return 0;
	}

	readEdge(fp, G);
	parseEdge(G);
	mkArr(G);
	findShortest(G);
	
	return 0;
}
