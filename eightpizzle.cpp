#include <iostream>
#include <vector>
#include <numeric>      
#include <algorithm>    
#include <math.h>
#include <stdlib.h>     

using namespace std;

const int n=3;                  //The board size is n*n

struct state{
	int board[n][n];
	int depth;
	int heuristic;
	int cost;                       
	int num;
	int blanki, blankj;        //The i,j position of blank element
	int parentpos;             //The relative blank position of the parent node. 1 2 3 4 represent
	                           //up right down left
};

vector<state> queue;
int functiontype;
int totalnode=1;                               // number of total created nodes
int maxqueuesize=0;
char traceflag;

 state remove_front(){
	 state front;
	 vector<state>::iterator front_pos = queue.begin();
	 int mincost=100000000;
     for(vector<state>::iterator i=queue.begin(); i!=queue.end(); i++){
		 if(mincost>(*i).cost){
			 front = *i;
			 mincost = (*i).cost;
			 front_pos = i;
		 }
	 }
	 queue.erase(front_pos);
	 return front;
 }

 bool goal_test(state &front){
	 int i,j;
	 for(i=0;i<n;i++){
		 for(j=0;j<n;j++){
			 if(front.board[i][j]!=i*n+j+1) 
				 return false;
		 }
	  }
	 return true;

}

 void compute_heuristic(state &newnode){
	 int i,j,k,l;
	 newnode.heuristic = 0;
	 switch (functiontype){
	     case 1:   // uniform cost search
		     break;
	     case 2:   // misplaced tile heuristic
             for(i=0;i<n;i++){
		         for(j=0;j<n;j++){
             		 if(newnode.board[i][j]!=i*n+j+1) 
						 newnode.heuristic++;
				 }
			 }
			 break;
	     case 3:                  // manhattan distance heuristic
			 int misplacedi;         // the position i,j of misplaced elements
			 int misplacedj;
		     for(i=0;i<n;i++){
		         for(j=0;j<n;j++){
					 if(newnode.board[i][j]!=i*n+j+1){
						 for(k=0;k<n;k++){
							 for(l=0;l<n;l++){
								 if(newnode.board[k][l]==i*n+j+1){
									 misplacedi = k;
									 misplacedj = l;
								 }
							 }
						 }

						 newnode.heuristic += (abs(i-misplacedi) + abs(j-misplacedj));
					 }

				 }
			 }
			 break;
	 }
 }



 void create_one_node(state newnode, state &front){
	 int temp;
	 //swap the elements in the old blank position and new blank position
	 temp = newnode.board[newnode.blanki][newnode.blankj];
	 newnode.board[newnode.blanki][newnode.blankj] = n*n;
	 newnode.board[front.blanki][front.blankj] = temp;
	 compute_heuristic(newnode);
	 newnode.cost = newnode.depth+newnode.heuristic;
	 totalnode= totalnode+1;
	 newnode.num = totalnode;
	 queue.push_back(newnode);
 }

 
 
 void expand_nodes(state &front){
	 int i,j;
	 state newnode;
	 newnode.depth = front.depth+1;
	 for(i=0;i<n;i++){
		 for(j=0;j<n;j++){
             newnode.board[i][j] = front.board[i][j];
		 }
	 }

	 if(front.blanki!=0 && front.parentpos!=1){             //blank element of front not at boundary and not expand to its parent 
		 newnode.blanki = front.blanki-1;
		 newnode.blankj = front.blankj;	
		 newnode.parentpos = 3;
		 create_one_node(newnode, front);
	 }

	 if(front.blankj!=0 && front.parentpos!=4){
		 newnode.blanki = front.blanki;
		 newnode.blankj = front.blankj-1;	
		 newnode.parentpos = 2;
		 create_one_node(newnode, front);
	 }

	 if(front.blanki!=n-1&&front.parentpos!=3){
		 newnode.blanki = front.blanki+1;
		 newnode.blankj = front.blankj;	
		 newnode.parentpos = 1;
		 create_one_node(newnode, front);
	 }

	 if(front.blankj!=n-1 && front.parentpos!=2){
		 newnode.blanki = front.blanki;
		 newnode.blankj = front.blankj+1;	
		 newnode.parentpos = 4;
		 create_one_node(newnode, front);
	 }

	 
 }

 void showstate(state front){
	int i,j;
	cout<<"The state of "<<totalnode<<" expanded node. Depth of solution is "<<front.depth<<endl;
    for(i=0;i<n;i++){
	    for(j=0;j<n;j++){
			if(front.board[i][j]!=n*n){
			    cout<<front.board[i][j]<<" ";
			}
			else 
				cout<<"* ";
		}
		cout<<endl;
	}
	cout<<endl;
}

state gamestart(state &initstate){
	state output;
	output.num = -1;
	queue.push_back(initstate);
	state front = initstate;	
	while(queue.size()!=0){               //general search algorithm
		if(queue.size()>maxqueuesize){
			maxqueuesize = queue.size();
		}
		front = remove_front();
		if(traceflag=='Y'){
		    showstate(front);
		}
		if(goal_test(front)==true){
			output = front;
			return output;
		}
		else{
			expand_nodes(front);
		}
	}
	return output;
}



int main ()
{
 
  int i,j;
  char temp;
  state output;
  
  cout<<"please input which function type you would like to use:"<<endl;
  cout<<"uniform cost -- 1"<<endl;
  cout<<"A* with the Misplaced Tile heuristic -- 2"<<endl;
  cout<<"A* with the Manhattan Distance heuristic -- 3"<<endl;

  cin>>functiontype;

  state initstate;
  initstate.depth=0;
  initstate.num=1;
 
  
  cout<<"please input the value of initial states as the following format"<<endl;
  cout<<"such as 1 2 3"<<endl;
  cout<<"        4 5 6"<<endl;
  cout<<"        7 8 *"<<endl;
  cout<<"Input the initial state:"<<endl;

  for(i=0;i<n;i++){
	  for(j=0;j<n;j++){
	      cin>>temp;
		  if(temp!='*')
			  initstate.board[i][j] = temp - '0';
		  else{
			  initstate.blanki = i;
			  initstate.blankj = j;
			  initstate.board[i][j] = n*n;
		  }
	  }
  }
  
  cout<<"Do you want to show the trace of expanded nodes? (Y/N) ";
  cin>>traceflag;
  cout<<endl;
  
  compute_heuristic(initstate);
  initstate.cost = initstate.heuristic;
  initstate.parentpos = -1;
  output = gamestart(initstate);
  if(output.num==-1){
	  cout<<"Search failed"<<endl;
  }
  else{
      cout<<"The depth of solution is: "<<output.depth<<endl;
      cout<<"The total number of expanded nodes is: "<<totalnode<<endl;
	  cout<<"Max queue size is: "<<maxqueuesize<<endl;

  }

  return 0;
}