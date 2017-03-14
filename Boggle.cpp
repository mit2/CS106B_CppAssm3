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
#include "gboggle.h"
#include "graphics.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
using namespace std;

/* Constants */

const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;
const int STANDART_CUBES_NUM = 16;
const int BIG_BOGGLE_CUBES_NUM = 25;
const int FINISH_THE_GAME = 2;

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
void playGame(string input, Player player, Grid<char> &board);
bool humanPlayRec(string word, Grid<char> &board);
bool computerPlayRec(string word, Grid<char> &board);

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
   while(player != FINISH_THE_GAME){
	   cin >> word;
	   if(word.size() < 4) cout << "The word did't meet minimum 4 chars length requirement. Try again: ";
	   else if(wordsList.contains(word)) cout << "The word has been used already. Try again: ";
	   else if(!english.contains(word)) cout << "NOT A VALID ENGLISGH WORD. TRY AGAIN: ";
	   else{ 
		   wordsList.add(word);
		   recordWordForPlayer(word, player); // display word erned points into GUI
		   playGame(word, player, board);
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
	case 2: // setup Big Boggle board
			board.resize((int)sqrt((float)BIG_BOGGLE_CUBES_NUM), (int)sqrt((float)BIG_BOGGLE_CUBES_NUM));
			for (int i = 0; i < board.numRows(); i++){
				for(int j = 0; j < board.numCols(); j++){										
					board[i][j] = ' ';
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
			
			// if is setup Standart Boggle board
			if(board.numRows() == (int)sqrt((float)STANDART_CUBES_NUM)){
				for (int i = 0; i < board.numRows(); i++){
					for(int j = 0; j < board.numCols(); j++){										
						board[i][j] = ' ';
					}
			}
			// display Game board
			drawBoard(board.numRows(), board.numCols());
			}
			

			// fill the board with letters
			for (int i = 0, num = 0; i < board.numRows(); i++){
				if(num > 25) break; // in case if user letter string.size > 25 ignore others letters
				for(int j = 0; j < board.numCols(); j++){
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
void playGame(string word, 	Player player, Grid<char> &board){
	switch (player){
	case HUMAN: if(humanPlayRec(word, board)){
					// higlight word on the board;
				}
				break;
	case COMPUTER: computerPlayRec(word, board);
				break;
	default: cout << "NOT existing player in System???" <<endl;
	}	
}

/*
 * Human plays the Game.
 * word - is a word to process
 * board - Boggle game board
 */
bool humanPlayRec(string word, Grid<char> &board){
	
	return false;
}

/*
 * Computer plays the Game.
 * word - is a word to process
 * board - Boggle game board
 */
bool computerPlayRec(string word, Grid<char> &board){
	return false;
}