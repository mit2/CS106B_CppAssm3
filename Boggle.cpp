/*
 * File: Boggle.cpp
 * ----------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the main starter file for Assignment #4, Boggle.
 * [TODO: extend the documentation]
 */
 
#include <iostream>
#include <cmath>
#include <array>
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
#include "foreach.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;
const int STANDART_CUBES_NUM = 16;
const int BIG_BOGGLE_CUBES_NUM = 25;
const int OUT = 2;

const string STANDARD_CUBES[16]  = {
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function prototypes */

void welcome();
void giveInstructions();
void takeAction(int res, Grid<char> &board);
bool playGame(string word, Player player, Grid<char> &board, Set<string> &wordsList, Lexicon &english);
bool humanPlayRec(string word, Grid<char> &board, Set<float> &visitedPos, bool wordStart,  int match[], Vector<float> &showPos);
bool computerPlayRec(string word, Grid<char> &board, Set<float> &visitedPos, bool startPos, int tryPos[], Set<string> &foundWords, Set<string> &wordsList, Lexicon &english);
bool position(string word, Grid<char> &board, int *newMatch, Set<float> &visitedPos);

/* Main program */

int main() {	
   initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
   Lexicon english("EnglishWords.dat");
   Grid<char> board((int)sqrt((float)STANDART_CUBES_NUM), (int)sqrt((float)STANDART_CUBES_NUM));
   Set<string> wordsList;
   Player player; // defined players in gboggle.h
   char respond = '?';

   // Game setup. 
   // User Game customisation by cmd-line
   welcome();

   // Question-1 user action
   cout << "Do you need instructions?  y or n: ";
   while(respond != 'y' && respond != 'n'){
	   cin >> respond;   
	   cout << endl <<endl;
	   if(respond == 'y'){
		   takeAction(1, board); // give instruction to user	   
	   }
	   if(respond != 'y' && respond != 'n')
		   cout << "Please respond 'y' or 'n'?: ";
   } 
   respond = '?'; // clear respond

   // Question-2 user action
   cout << "You can choose standart Boggle <4x4> grid or Big Boggle <5x5>." <<endl;
   cout << "Would you like Big Boggle? 'y' or 'n': ";
   while(respond != 'y' && respond != 'n'){
	   cin >> respond;
	   cout << endl <<endl;
	   if(respond == 'y'){
		   takeAction(2, board); // setup Big Boggie	   
	   }
	   if(respond != 'y' && respond != 'n')
		   cout << "Please respond 'y' or 'n'?: ";
   } 	
   respond = '?'; // clear respond

   // Question-3 user action
   cout << "I will give you the chance to setup the Board to your specification, with make it easier to confirm your Boggle Program is working." <<endl;
   cout << "Do you want to force board configuration? y or n: ";
   while(respond != 'y' && respond != 'n'){
	   cin >> respond;
	   cout << endl <<endl; 
	   if(respond == 'y')
			takeAction(3, board); // force board configurations by user requirements
	   else if(respond == 'n')			 
			takeAction(4, board); // display default random configuration choosen by program;
	   if(respond != 'y' && respond != 'n')
		   cout << "Please respond 'y' or 'n'?: ";
   }
   respond = '?'; // clear respond

   // Start the Game
   cout << endl << endl;
   cout << "Setup is finished. Lets begin!" <<endl;
   cout << "OK, take all the time you wish and find all the words uou can! Signal that you are finished by entering empty line." <<endl;
   cout << "Enter the word: ";
   
   // Play Game multiple times
   player = HUMAN;
   string word;
   cin.ignore(); //  flush the newline character out of the buffer, clear input before game start
   while(player != OUT){
	   // plays HUMAN
	   if(player == HUMAN){
		   getline(cin, word);
		   if(word.empty()){ // if Human enter empty line to finish his tern
			   cout << "EMPTY LINE" <<endl;
			   player = COMPUTER;
			   continue;
		   }
		   if(word.size() < 4) cout << "The word did't meet minimum 4 chars length requirement. Try again: ";
		   else if(wordsList.contains(word)) cout << "The word has been used already. Try again: ";
		   else if(!english.contains(word)) cout << "NOT A VALID ENGLISGH WORD. TRY AGAIN: ";
		   else{ 
			   wordsList.add(word);
			   if(playGame(word, player, board,	wordsList , english)) recordWordForPlayer(word, player); // display word erned points into GUI
			   else cout << "IMPOSSIBLE TO BUILD WORD ON THE BOARD. TRY AGAIN!" <<endl;   
		   }
		   
	   }else{
			// plays COMPUTER
			cout << "PLAYS COMPUTER"<< endl;
			//cin >> word; // dummy stop code exec
			word = ""; // clear word after last HUMAN match
			playGame(word, player, board, wordsList, english);
			cout << "GAME IS OVER! WOULD YOU LIKE TO PLAY NEW GAME?"<< endl;
			getline(cin, word);
			// ... START NEW GAME, REINIT BOARD...
			if(word == "yes" || word == "y"){
			   // erase board for new game
			   initGraphics(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
			   drawBoard(board.numRows(), board.numCols()); 

			   // Back to Question-3 user action
			   cout << "I will give you the chance to setup the Board to your specification, with make it easier to confirm your Boggle Program is working." <<endl;
			   cout << "Do you want to force board configuration? y or n: ";
			   while(respond != 'y' && respond != 'n'){
				   cin >> respond;
				   cout << endl <<endl; 
				   if(respond == 'y')
						takeAction(3, board); // force board configurations by user requirements
				   else if(respond == 'n')			 
						takeAction(4, board); // display default random configuration choosen by program;
				   if(respond != 'y' && respond != 'n')
					   cout << "Please respond 'y' or 'n'?: ";
			   }
			   respond = '?'; // clear respond

			   // Start the Game
			   cout << endl << endl;
			   cout << "Setup is finished. Lets begin!" <<endl;
			   cout << "OK, take all the time you wish and find all the words uou can! Signal that you are finished by entering empty line." <<endl;
			   cout << "Enter the word: ";
			   
			   // Play Game multiple times
			   player = HUMAN;
			   cin.ignore(); //  flush the newline character out of the buffer, clear input before game start
			}
			else exit(0); // exit the game

	   }	   
   }
   return 0;
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
   cout << "Welcome!  You're about to play an intense game ";
   cout << "of mind-numbing Boggle.  The good news is that ";
   cout << "you might improve your vocabulary a bit.  The ";
   cout << "bad news is that you're probably going to lose ";
   cout << "miserably to this little dictionary-toting hunk ";
   cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
   cout << endl;
   cout << "The boggle board is a grid onto which I ";
   cout << "I will randomly distribute cubes. These ";
   cout << "6-sided cubes have letters rather than ";
   cout << "numbers on the faces, creating a grid of ";
   cout << "letters on which you try to form words. ";
   cout << "You go first, entering all the words you can ";
   cout << "find that are formed by tracing adjoining ";
   cout << "letters. Two letters adjoin if they are next ";
   cout << "to each other horizontally, vertically, or ";
   cout << "diagonally. A letter can only be used once ";
   cout << "in each word. Words must be at least four ";
   cout << "letters long and can be counted only once. ";
   cout << "You score points based on word length: a ";
   cout << "4-letter word is worth 1 point, 5-letters ";
   cout << "earn 2 points, and so on. After your puny ";
   cout << "brain is exhausted, I, the supercomputer, ";
   cout << "will find all the remaining words and double ";
   cout << "or triple your paltry score." << endl << endl;
   cout << "Hit return when you're ready...";
   getLine(); // easy to get clear from input stream 'Enter' symbol, as last symbol in in-stream
   getLine(); // wait for new input
   cout << endl << endl;
}

/*
 * Respond to user Actions.
 * res - 'y' or 'n' action
 */
void takeAction(int res, Grid<char> &board){
	Vector<string> cubes;
	string temp, letters;
	int boardSize = 0;
	int r; // random pos in cubes vector
	switch (res) {
	case 1: giveInstructions();
			break;
	case 2: // setup Big Boggle board, feel-up board with empty cells for show it in GUI
			board.resize((int)sqrt((float)BIG_BOGGLE_CUBES_NUM), (int)sqrt((float)BIG_BOGGLE_CUBES_NUM));
			for (int i = 0; i < board.numRows(); i++){
				for(int j = 0; j < board.numCols(); j++){										
					board[i][j] = '?';
				}
			}
			// display Game board
			drawBoard(board.numRows(), board.numCols());
			break;
	case 3: // force board configurations by user requirements
			cout << "Enter 25-characters string to identify witch letters you want on the cubes." <<endl;
			cout << "The first 5 lettres are the cubes on the top row, from left to right, the next 5 letters are the second row, and so on." <<endl;
			cout << "Enter the string: ";
			cin >> letters;
			while(letters.size() < 25){
				cout << "Letters string is less then 25 chars long: ";
				cin >> letters;
			}
			
			// if is setup Standart Boggle board, to feel-up board with empty cells for show it in GUI
			if(board.numRows() == (int)sqrt((float)STANDART_CUBES_NUM)){
				for (int i = 0; i < board.numRows(); i++){
					for(int j = 0; j < board.numCols(); j++){										
						board[i][j] = '?';
					}
			}
			// display Game board
			drawBoard(board.numRows(), board.numCols());
			}
			

			// fill the board with letters
			for (int i = 0, num = 0; i < board.numRows(); i++){
				if(num > 25) break; // in case if user letter string.size > 25 ignore others letters
				for(int j = 0; j < board.numCols(); j++){
					board[i][j] = letters[num];
					labelCube(i, j, letters[num]);
					num++;
				}
			}
			break;
	case 4: 
			// define board size
			if(board.numRows() < (int)sqrt((float)BIG_BOGGLE_CUBES_NUM))
				boardSize = (int)sqrt((float)STANDART_CUBES_NUM);	
			else
				boardSize = (int)sqrt((float)BIG_BOGGLE_CUBES_NUM);

			// set display to default random configuration choosen by program;
			if(boardSize < (int)sqrt((float)BIG_BOGGLE_CUBES_NUM)){
				for(int i = 0; i < STANDART_CUBES_NUM; i ++)
					cubes.add(STANDARD_CUBES[i]);
			}else{
				for(int i = 0; i < BIG_BOGGLE_CUBES_NUM; i ++)
					cubes.add(BIG_BOGGLE_CUBES[i]);
			}

			// shuffle the cubes in vector by pos
			for(int i = 0; i < cubes.size(); i ++){
				r = randomInteger(i, cubes.size() - 1);
				temp = cubes[i];
				cubes[i] = cubes[r];
				cubes[r] = temp;
			}

			// fill-up Game board with random/shuffle cube side
			for (int i = 0, pos = 0; i < boardSize; i++){				
				for(int j = 0; j < boardSize; j++){					
					string cubeSides = cubes.get(pos);
					board[i][j] = cubeSides.at(randomInteger(0, cubeSides.size() - 1));
					pos++;
				}
			}
			

			// display Game board
			drawBoard(boardSize, boardSize);
			for (int i = 0; i < board.numRows(); i++){
				for(int j = 0; j < board.numCols(); j++){
					labelCube(i, j, board[i][j]);
				}
			}
			break;
	default: cout << "Wrong action is choosen! ???" <<endl;
   }
}

/*
 * Game processor.
 * word - is a word to process
 */
bool playGame(string word, Player player, Grid<char> &board, Set<string> &wordsList, Lexicon &english){
	Set<float> visitedPos; // to track matched/visited letters
	Vector<float>showPos;
	Set<string> foundWords; // words collection found by COMPUTER
	switch (player){
	case HUMAN: cout << "HUMAN PLAYS: "<< word <<endl;
				if(humanPlayRec(word, board, visitedPos, false, 0, showPos)){	// MEMO: zerro 0  as it is by definition the null pointer.
					// higlight word on the board;
					cout << "TEST OK!"<<endl;
					cout << "VISITED-POS SIZE: " << visitedPos.size() <<endl;
					foreach(float pos in visitedPos)
						cout << pos <<" ";
					cout << endl;
					foreach(float pos in showPos){
						highlightCube((int)pos, (int)(pos*10)%10, true);
						for(int i = 0; i < 60000000; i++) i++; // some dummy wait
						highlightCube((int)pos, (int)(pos*10)%10, false);
						cout << (int)pos << ":" << (int)(pos*10)%10 << " ";						
					}
					return true;
				}else{
					cout << "TEST NOT PASS!"<<endl;
					cout << "VISITED-POS SIZE: " << visitedPos.size() <<endl;
					foreach(float pos in visitedPos)
						cout << pos <<" ";

				}
				return false;
	case COMPUTER: computerPlayRec(word, board, visitedPos, false, 0, foundWords, wordsList, english);
				   // add list of all found words on board
				   foreach(string word in foundWords)
					   recordWordForPlayer(word, player);
				break;
	default: cout << "NOT existing player in System???" <<endl;
	}	
}


/*
 * Human plays the Game. VERSION 2
 * word - is a word to process
 * board - Boggle game board
 */
bool humanPlayRec(string word, Grid<char> &board, Set<float> &visitedPos, bool wordStart, int match[], Vector<float> &showPos){
	string memo = word;
	int newMatch[2];
	if(match != 0){ // null or 0 just for starting sentinel for searching the word on the board
		newMatch[0] = match[0];
		newMatch[1] = match[1];
	}
	float pos;
	if(word.length() == 0) return true;	// base case

	if(!wordStart){
		for(int i = 0; i < board.numRows(); i++){
			for(int j = 0; j < board.numCols(); j++){
				newMatch[0] = i;
				newMatch[1] = j;
				if(position(word, board, newMatch, visitedPos)){
					pos = newMatch[0] + newMatch[1]/10.0f; // convert newMatch to float for easy store in Set
					cout << "ADD-POS :" << pos<<endl;
					visitedPos.add(pos);
					showPos.add(pos);
					wordStart = true;
					if(humanPlayRec(word.erase(0, 1), board, visitedPos, wordStart, newMatch, showPos)) return true;
					visitedPos.remove(pos);
					showPos.removeAt(showPos.size() - 1);
					cout << "REMOVE-POS :" << pos<<endl;
					//wordStart = false;
					word = memo;
				}
			}
		}
	}else if(position(word, board, newMatch, visitedPos)){
		pos = newMatch[0] + newMatch[1]/10.0f;
		cout << "ADD-POS :" << pos<<endl;
		visitedPos.add(pos);
		showPos.add(pos);
		if(humanPlayRec(word.erase(0, 1), board, visitedPos, wordStart, newMatch, showPos)) return true;
		visitedPos.remove(pos);
		showPos.removeAt(showPos.size() - 1);
		cout << "REMOVE-POS :" << pos<<endl;
		return false;
	}
	
	return false;
}
/*
 * Computer plays the Game.
 * word - is string used to build word
 * board - Boggle game board
 * TESTING CUBES:     IOEGSYSXEBHSTFLA, STANFORD'S AND MINE RESULTS FOR COMPUTER IS 24!!!!!!!!!!!
 */
bool computerPlayRec(string word, Grid<char> &board, Set<float> &visitedPos, bool startPos, int tryPos[], Set<string> &foundWords, Set<string> &wordsList, Lexicon &english){
	// 2 F..KING BUGGS, LIKE 2 TIMES WAS DEFINED int x, y(count wrong all 8 pos) and return false in base case when was pos alredy visited cost 10 HOURES OF DEBUGGING!!!!!!!!!!!!!!!!!!
	int newPos[2];
	int x = 0, y = 0;
	float pos, match;
	if(tryPos != 0){
		x = tryPos[0]; // setup x
		y = tryPos[1]; // setup y
	}
	// 8 all possible destination from cell x,y with new (x*,y*) locations.
	int allpos[][2] = { {x, y-1} , { x+1, y-1} , {x+1, y} , {x+1, y+1}, {x, y+1} , {x-1, y+1} , {x-1, y} , {x-1, y-1} }; 
	/*if(tryPos != 0) cout << "TRYPOS: " << tryPos[0] << tryPos[1]<<endl;
	for(int i = 0; i < 8; i++){
		cout << allpos[i][0] << "," << allpos[i][1] << " ";
	}
	cout <<endl;*/
	

	if(!startPos){
		for(int i = 0; i < board.numRows(); i++){
			for(int j = 0; j < board.numCols(); j++){
				newPos[0] = i;
				newPos[1] = j;
				word.push_back(board[i][j]); // add 1st letter to word string
				pos = newPos[0] + newPos[1]/10.0f;
				visitedPos.add(pos);
				startPos = true;
				if(computerPlayRec(word, board, visitedPos, startPos, newPos, foundWords, wordsList, english)) return true;
				visitedPos.remove(pos);
				word = "";
			}
		}
	}else{
		if(!english.containsPrefix(word)){
			cout << "  NO PREFIX FOUND RETURN FALSE: " << word <<endl;
			return false;	// base case
		}
		if(english.contains(word) && !wordsList.contains(word) && word.length() >= 4) foundWords.add(word); // add word found by COMPUTER	
		for(int i = 0; i < 8; i++){
			cout << "LOOP TIMES: " << i << " " << word <<endl;
			if(board.inBounds(allpos[i][0], allpos[i][1])){
				cout << "POS IN BOUNDS"<<endl;
				match = allpos[i][0] + allpos[i][1]/10.0f; // convert pos to float for check in visitedPos
				if(visitedPos.contains(match)){
					cout << "  POS ALREDY WAS VISITED STOP SEARCH: " <<  board[allpos[i][0]][allpos[i][1]] << " " << match << endl;
					continue; // base case, stop search if pos/letter was already visited
				}
				newPos[0] = allpos[i][0];
				newPos[1] = allpos[i][1];
				word.push_back(board[newPos[0]][newPos[1]]); // append new letter to word
				pos = newPos[0] + newPos[1]/10.0f;
				visitedPos.add(pos);
				if(computerPlayRec(word, board, visitedPos, startPos, newPos, foundWords, wordsList, english)) return true;
				visitedPos.remove(pos);
				word.erase(word.length() - 1); // delete from word last added letter
			}
		}
		return false; // all 8 cells arround tryPos is checked, backtrack now
	}	
	return true;  // all startPos is checked, exit
}

/*
 * Checking if cell's char match word's 1st letter
 * pos - is cell pos on the game board
 * word - user's entered word
 * return - matched pos
 */
bool position(string word, Grid<char> &board, int *newMatch, Set<float> &visitedPos){ // *newMatch, pass by reference array
	cout << "NEW-MATCH: " << newMatch[0] << newMatch[1]<<endl;
	int x = newMatch[0]; // setup x
	int y = newMatch[1]; // setup y
	float match;
	int allpos[][2] = { {x, y}, {x, y-1} , { x+1, y-1} , {x+1, y} , {x+1, y+1}, {x, y+1} , {x-1, y+1} , {x-1, y} , {x-1, y-1} }; // 9 all possible destination from cell x,y with new (x*,y*) locations.

	for(int i = 0; i < 9; i++){
		match = allpos[i][0] + allpos[i][1]/10.0f; // convert pos to float for check in visitedPos
		if(board.inBounds(allpos[i][0], allpos[i][1]) && board[allpos[i][0]][allpos[i][1]] == word[0] && !visitedPos.contains(match)){
			cout << "ALLPOS: " << allpos[i][0] << allpos[i][1]<<endl;
			cout << "BOARD MATCH: " << board[allpos[i][0]][allpos[i][1]] <<endl;
			newMatch[0] = allpos[i][0];
			newMatch[1] = allpos[i][1];
			return true;
		}
	}
		
	return false;	// MEMO: zerro 0  as it is by definition the null pointer.
}
