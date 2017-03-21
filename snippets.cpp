
#include <iostream>
#include <cmath>
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
using namespace std;
/*
 if(respond == 'y')
	   takeAction(2); // get big boggle configuration setup
   else if(respond == 'n'){
	   cout << "I will give you the chance to setup the Board to your specification, with make it easier to confirm your Boggle Program is working." <<endl;
	   cout << "Do you want to force board configuration? y or n: ";
	   cin >> respond;
	   cout << endl <<endl;
	   if(respond == 'y')
			 takeAction(3); // force board configurations by user requirements
	   else if(respond == 'n')
			 takeAction(4); // display default random configuration choosen by program;
	   }

	   cout << "TEST-1: " << boardSize <<endl;
			cout << "TEST-1: " << board.size() <<endl;
			cout << "TEST-1: " << board[0].size() <<endl;

*/


	// PLM WITH FOREACH LOOOP TO ITERATE OTHER ARRAY, foreach only for collections!

	/*foreach(int pos[] in allpos){
		match = pos[0] + pos[1]/10.0f; // convert pos to float for check in visitedPos
		if(board.inBounds(pos[x], pos[y]) && board[pos[x]][pos[y]] == word[0] && !visitedPos.contains(match)){
			newMatch[0] = pos[x];
			newMatch[1] = pos[y];
			return true;
		}
	}*/

/* Main program */

int mai() {
	int n = 0;
	string test = "abc";
	string test2 = "abb";
	string numstr = "str_";
	// how to store the pos(x, y) in easy way in set
	float n1 = 4 + 4/10.0f; // pos (4,4) => 4,4
	float n2 = 4/10.0f + 4/100.0f;
	float n3 = n1 + n2;
	/*for (int i = 0, pos = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					cout << "rest";
					
				}
			}*/

	//if(!n) cout << "n = 0";
	//cout << test.erase(0, 1); // print bc
	//cout << 0/10.0 + 1/100.0; // 0.01
	//if(n1 == n2)cout << "true"; //
	float f = 4.0f;
	cout << (int)f << "/" << (int)(f*10)%10;
	
	//cout << 1 + 0/10.0f << endl; // 1 but not 1.0
	//cout << 0 + 4/10.0f; // 1 but not 0.4
	//cout << 4.%
	//if(test[0] == test2[0]) cout << "MATCH!" << endl;

	/*int x = 0; // setup x
	int y = 0; // setup y
	
	int allpos[1][2] = {{x, y-1}};
	int hours[3][2] = {{8, 5}, {7, 9}, {x, y-1}};
	cout << y-1 <<endl;
	for(int i = 0; i < 3; i++){
		cout << "ALLPOS: " << hours[i][0] << hours[i][1]<<endl;
		
		
		}*/
	

	return 0;
}

