/*  By A Sahu IIT Guwahati
 *  asahu@iitg.ac.in
 *  
 *  Program to generate Maze file for testing
 *
 *  $g++ generateMaze.cpp -o generateMaze
 *   *  $ ./generateMaze Height:int Width:int FaultProbability:fp<0,1> SrcX SrcY DstX DstY
 *  Example $./generateMaze 10 10 0.3 1 1 9 9
 *
 *  It generate "Mazedata.pl" file with contain facts for prolog
 *  It also generate graph1.png, which shows the connectivity of Maze
 *  The source node and destination node
 *
 *  This code require graphviz. If graphviz is not available in your system
 *  please install it 
 *  			#yum install graphviz       
 *  			or
 *			$sudo apt-get install graphviz
 * */


#include<iostream>
#include<fstream>
#include<cstdlib>

using namespace std;
int Width, Height; 

ofstream MazeData, MazeGraph;
int **F;

void faultynodefact(int i, int j){
	MazeData<<"\nfaultynode("<<i*Width+j<<").";
}

void  Link(int i, int j, int k, int l){
		//if(F[k][l]!=1){	//No link to Faulty Node
		MazeData<<"\nmazelink("<<i*Width+j<<","<<k*Width+l<<").";
		MazeGraph<<"\n"<<(i*Width+j)<<"->"<<k*Width+l<<"\n";
		//}
}


int main(int argc, char*argv[]){

		
	float FaultProbability;
	int i,j,k,l;
	int srcX,srcY, dstX, dstY;
	if (argc<7){
			cout<<"./genrateMaze Height:int Width:int FaultProbability:fp<0,1> SrcX SrcY DstX DstY\n";
			exit(1);
	}
	Width=atoi(argv[1]);
	Height=atoi(argv[2]);
	FaultProbability=atof(argv[3]);
	srcX=atoi(argv[4]); srcY=atoi(argv[5]); dstX=atoi(argv[6]); dstY=atoi(argv[7]);
	
	MazeData.open("Mazedata.pl");
	MazeGraph.open("MazeGraph.dot");

	MazeGraph<<"\ndigraph G {\n";

	// Create F array
	F = (int **) malloc (sizeof(int*)*Height); 
	for(i=0;i<Height;i++)
		F[i]=(int *) malloc(sizeof(int)*Width); 

	//CreateFaultNode with FaultProbability
	int IntFaultProbability=(int) (FaultProbability*100.0);
	for(i=0;i<Height;i++)	
		for(j=0;j<Width;j++) {
				F[i][j]= ( (random()%100) < IntFaultProbability ) ? 1:0;
				// Src and Dst cann't be Faulty Node; Override
				F[srcX][srcY]=0; F[dstX][dstY]=0;
				if (F[i][j]==1) {
					MazeGraph<< i*Width+j
						 <<"[shape=box,style=filled,color=red, pos=\""
						 <<i<<","<<j<<"!\"]\n";
					faultynodefact(i,j);	
					}
				else 	MazeGraph<< i*Width+j
						 <<"[shape=box, pos=\""
						 <<i<<","<<j<<"!\"]\n";
				
				}
	
	// Src and Dst cann't be Faulty Node
	F[srcX][srcY]=0; F[dstX][dstY]=0;
	MazeGraph<<i*srcX+srcY<<"[style=filled,color=green]\n";
	MazeGraph<<i*dstX+dstY<<"[style=filled,color=green]\n";
	//Create Link
	MazeData<<"\n\n";
	for(i=0;i<Height;i++){	
		for(j=0;j<Width;j++) {
				//if (F[i][j]!=1) {
					 if(i!=0) Link(i,j,i-1,j); 
					 if(j!=0) Link(i,j,i,j-1);
					 if(i!=(Height-1)) Link(i,j,i+1,j); 
					 if(j!=(Width-1)) Link(i,j,i,j+1); 
				//}
	 	}
	 }
	 MazeGraph<<"\n}\n";
	 MazeGraph.close();
	 //MazeData<<"\nStart="<<i*srcX+srcY<<".";
	 //MazeData<<"\nFinish="<<i*dstX+dstY<<".";
	 MazeData.close();
	// Create MazeGraph file in Png Format from DOT file
	 system("dot -Kfdp -n -Tpng MazeGraph.dot -o graph1.png");
	
	 return 0;
}

