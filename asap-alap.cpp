/*
 * asap-alap.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: Santhosh Ramaiah
 */

#include <cstdio>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <cmath>

#define MAX 300 // maximum number of nodes or edges
using namespace std;

enum colors {BLACK, WHITE, GRAY}; // description for node state
int color[MAX], d[MAX], p[MAX], f[MAX],ftemp[MAX], t,kh=0, vertex,checkDirected;
int noresClass, noofOper, Clock, asap[MAX],alap[MAX],alapn[MAX], tect[MAX], latency;
int edge,k=0,snode,cont=0, roat=0, contrev=0, ectf=0; //ectf=earliest completion time final
vector<int> bkeeping[MAX]={}; // book keeping for edges

int NIL = numeric_limits<int>::min();// 0 value to NIL
int INF = numeric_limits<int>::max(); // max value representing infinity
bool bflag=false,sflag=true; // bflag- flag for back edge represents cyclic or not. sflag- flag for edge repeat.
bool sccflag=false; // Strongly connected component flag
vector<int> contgra[MAX]={};  // Connected graph book keeping.
vector<int> contgrarev[MAX]={};	//reverse Connected graph book keeping.
vector<int> resClass[MAX]={};
vector<int> resClasstype[MAX]={};

void DFS(vector<int>[]); // DFS
void DFS_VISIT(vector<int>[],int, int);// DFS visit


int main(void)
{

		string line, lineR, filename;
		char filename1[40];
		int intArray[MAX][MAX]={}; // temporary variable which stores node and its adjacent edges
		string resArray [MAX][MAX]={};
		int temp,tempres, temp2; // checks if the graph is directed or not.


		std::vector<int> adjList[MAX]; // list of node and its adjacent edges

	 	cout<<"Enter the name of the graph file: "<<endl;
	 	cin>>filename;
	 	strcpy(filename1, filename.c_str()); //string to character store to pass

	    ifstream myfile; // read the file.
	    myfile.open(filename1);// open the file



	    ifstream resourcefile; // read the file
	    resourcefile.open("RESOURCE_FILE.txt"); // open the resource file

	    if(resourcefile.is_open()){
	    	while ( getline (resourcefile,lineR) ) // get every line and store it to line
	    	{
	    		std::stringstream ss1(lineR); // converts string to int
	    		int templength1=lineR.length();
	    		for(int idx = 0; idx < templength1; idx++)
	    		{
	    			ss1>>resArray[kh][idx]; // store the int to array
	    			//cout<<resArray[kh][idx]<<" ";

	    		}//cout<<endl;
	    		kh++;
	    	}


	    	resourcefile.close();
	    }
	    else{
	    	cout<<"resource file not available"<<endl; // close resource file // used later on.
	    	return 0;
	    }


	    if (myfile.is_open()) // check if the file is open.
	    {
	      while ( getline (myfile,line) ) // get every line and store it to line
	      {
	    	  std::stringstream ss(line);  // converts string to int
	    	  int templength2=line.length();
	    	  for(int idx = 0; idx < templength2; idx++)
	          {
	        	  ss>>intArray[k][idx]; // store the int to array
	          }
	          k++;
	      }
	      myfile.close();
	    }

	    else {
	    	cout << "Unable to open file";
	    	//return 0;
	    }

	  istringstream buffer(resArray[0][0]);
	  istringstream buffer2(resArray[1][0]);
  	  buffer >>  noresClass;
  	  buffer2 >> noofOper;

	//cout<<resClass<<" "<<noofOper<<endl;

    vertex=intArray[1][0]; // store the vertex
    checkDirected=intArray[0][0]; // store the directed/undirected graph

    if (checkDirected!=0){
    	cout<<endl<<"ASAP/ALAP can only be applied for Directed Acyclic Graph"<<endl;
    	return 0;
    }


    snode=0;// source node is 0.

    for(int e=0; e<vertex; e++) { // read all the vertices
    	for(int ed=0; ed<intArray[e+2][2];ed++){ // read all the edges
			temp=intArray[e+2][ed+3];
			adjList[e].push_back(temp); //for directed graph
			//revadjList[temp].push_back(e); //for SCC graph transpose

			if(checkDirected==(-1)){
				adjList[temp].push_back(e);//for undirected graph.
			}
    	}
    }

    for(int e=0; e<(noofOper); e++) {
    	stringstream buffer3(resArray[e+2][2]);
    	buffer3 >>  tempres;
    	resClass[e].push_back(tempres);
    	//cout<<resClass[e][0]<<endl;
    }

    stringstream buffer4(resArray[noofOper+2][0]);
    buffer4 >>  Clock;
    //cout<<Clock<<endl;

    for(int e=0; e<(noresClass); e++) {
    	for(int ed=0; ed<4;ed++){
        	stringstream buffer5(resArray[e+noofOper+3][ed]);
        	buffer5 >>  tempres;
        	resClasstype[e].push_back(tempres);
        	//cout<<resClasstype[e][ed]<<" ";
        }//cout<<endl;
    }


    DFS(adjList); // do DFS visit

    int vert=vertex-1, swap;
    for(int v=0; v<vertex; v++) {
    		//printf("v%d (%d/%d)\n", v, d[v], f[v]);
            ftemp[v]=(f[v]); // store the finish times for sorting

        }

    if (bflag){
        	cout<<endl<<"ASAP/ALAP can only be applied for Directed Acyclic Graph"<<endl;
        	return 0;
        }

    // bubble sort the finish times
   for (int c = 0 ; c < vertex; c++)
   {
	   for (int d = 0 ; d < vertex - c - 1; d++)
	   {
		   if (ftemp[d] < ftemp[d+1]) /* For decreasing order use < */
		   {
			   swap       = ftemp[d];
			   ftemp[d]   = ftemp[d+1];
			   ftemp[d+1] = swap;
		   }
	   }
   }

   // finish times sorted.
   cout<<endl;
   //ASAP Calculation
	for(int e=0; e<vertex; e++) {
		float temp3=(resClasstype[resClass[intArray[e+2][1]][0]][1]);
		float temp4=Clock;
		tect[e]=ceil(temp3/temp4);
		//cout<<tect[e]<<endl;
	}
	asap[0]=0;
	alap[0]=0;
	for(int e=1; e<vertex; e++) {
		asap[e]=1;
		//alap[e]=INF;
	}
	ectf=0;
	for(int e=0; e<vertex; e++) {
		for (int j = 0; j <= vert; j++){
			if(ftemp[e]==f[j]){
				temp2=j;
				//cout<<j<<endl;
			}
		}

		 int tempsize1=adjList[temp2].size();
		for(int v=0; v<tempsize1; v++) {
			int temp6=adjList[temp2][v];
			asap[temp6]=std::max(asap[temp6],(asap[temp2]+tect[temp2]));
			//cout<<endl<<ect[v]<<" ";
		}
		ectf=std::max(ectf,(asap[temp2]+tect[temp2]));
	}

	//ALAP Calculation
	for(int e=vert; e>0; e--) {
		for (int j = 0; j <= vert; j++){
			if(ftemp[e]==f[j]){
				temp2=j;
				//cout<<j;
			}
		}
		//cout<<" "<<tect[temp2]<<endl;
		alap[temp2]=ectf-tect[temp2];
		 int tempsize2=adjList[temp2].size();
		for(int v=0; v<tempsize2; v++) {
			int temp7=adjList[temp2][v];
			alap[temp2]=std::min(alap[temp2],(alap[temp7]-tect[temp2]));
			//cout<<alap[temp2]<<endl;
		}
		//ectf=std::min(ectf,(alap[temp2]+tect[temp2]));
	}


	cout<<"Minimum Possible Latency = "<<ectf<<endl;
	cout<<"Vertex (ASAP/ALAP)"<<endl;
	for(int v=0; v<vertex; v++) { // print all the vertices and their ASAP/ALAP
		printf("v%d (%d/%d)\n", v, asap[v], alap[v]);

	}

	//for different latency
	cout<<endl;
	cout<<"Enter the Latency (>Minimum Possible Latency)"<<endl;
	cin>>latency;
	if(latency>=ectf){
		int temp9=latency-ectf;
		for(int v=0; v<vertex; v++) {
			alapn[v]=(alap[v]+temp9);
		}
		alapn[0]=0;

		for(int v=0; v<vertex; v++) {
			printf("v%d (%d/%d)\n", v, asap[v], alapn[v]);
		}
	}
	else{
		cout<<"Error ... !!!!!"<<endl;
		cout<<"Latency is less than Minimum Possible Latency="<<ectf<<endl;
	}

   return 0;
    }



void DFS(vector<int> G[]) {
    for(int u=0; u<vertex; u++) { // initially paint all nodes white
        color[u] = WHITE;
        p[u] = NIL;
    }
    t = 0; // reset time

    if(color[snode] == WHITE) { // do DFS visit for source node
    	contgra[cont].push_back(snode);
    	DFS_VISIT(G,snode,cont);
    	cont++;
    }

    for(int u=0; u<vertex; u++) { // do DFS visit for all nodes other than source node.
    	if(u==snode){continue; 	} // not needed as it is backed by color.
    	else{
    		if(color[u] == WHITE) {
    			contgra[cont].push_back(u);
    			DFS_VISIT(G,u,cont);
				cont++;
			}
    	}
    }

}

void DFS_VISIT(vector<int> G[], int u, int contg) {
    t = t + 1; // increment time
    d[u] = t; // put discovery time for node
    color[u] = GRAY; // paint gray to node
    int tempsize3=G[u].size();
    for(int v=0; v<tempsize3; v++) { // do for all adjacent nodes

			for (int sj=0;sj<roat;sj++){ //check for edge repetition.
    			if(((bkeeping[sj][1]==u)&&(bkeeping[sj][0]==G[u][v]))||((bkeeping[sj][1]==u)&&(bkeeping[sj][0]==G[u][v]))){
					sflag=false;
					break;
				}
					else{
						sflag=true;

					}
				}
			bkeeping[roat].push_back(u); //put edge in book keeping array.
			bkeeping[roat].push_back(G[u][v]); //put edge in book keeping array.

			roat++;

		if(color[G[u][v]] == WHITE) { // check for nodes which are undiscovered

            p[G[u][v]] = u;
            if(sccflag==false){// check for SCC
            	contgra[contg].push_back(G[u][v]);
            	//cout<<"edge "<<u<<"----"<<G[u][v]<<" Tree Edge"<<endl;
            }
            else{
            	contgrarev[contg].push_back(G[u][v]);
            }

            DFS_VISIT(G,G[u][v],contg);

        }

        else if(color[G[u][v]] == GRAY&&sflag==true&&sccflag==false){ //check for back edge
        	//cout<<"edge "<<u<<"----"<<G[u][v]<<" Back Edge"<<endl;
        	bflag=true;
        }
        else if(color[G[u][v]] == BLACK&&sflag==true&&sccflag==false){ // check for forward/cross edge
        		if(d[u]<d[v]){
                	//cout<<"edge "<<u<<"----"<<G[u][v]<<" Forward Edge"<<endl;
                }
        		else{
        			//cout<<"edge "<<u<<"----"<<G[u][v]<<" Cross Edge"<<endl;
        		}
        }
        sflag=false;
    }
    color[u] = BLACK; // paint node black
    t = t + 1; //increment time
    f[u] = t; // put finish time for node
}
