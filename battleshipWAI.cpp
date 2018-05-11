#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>
using namespace std;

void initializeBoards(char [][10], char [][10], char [][10]);
void displayBoards(char [][10], char [][10]);
void displayBoard(char [][10]);
void placeEnemyShips(char [][10]);
void addPlayerShips(char [][10]);
void attack(char [][10], char [][10], char [][10], int []);
void revPush(int, int, char);
void clear();
void pause();

bool isShipsOverlap(char [][10], int, int, int, int);

char didHit(char [][10], int, int);
char hitRandom(char [][10], char [][10]);

int charToNum(char);
int shipToNum(char);

bool shouldUseLogic = false;
bool isMultipleShips = false;
bool AIPrevHit = false;
int numOfShips = 0;
int shipsLeft;
int AIDir =  -1;
int AIEnds = 0;
stack <int> xStack;
stack <int> yStack;
stack <char> dirStack;

int main() {
	srand(time(NULL));
	char attackBoard[10][10];
	char shipBoard[10][10];
	char enemyBoard[10][10];
	char hit;
	bool done = false;
	bool didSink = false;
	bool playerWin = false;
	bool enemyWin = false;

	int playerHealth[] = {5, 4, 3, 3, 2};
	int enemyHealth[] = {5, 4, 3, 3, 2};
	string shipNames[] = {"Aircraft Carrier", "Battleship", "Submarine", "Cruiser", "Destroyer"};
	
	initializeBoards(attackBoard, shipBoard, enemyBoard);
	placeEnemyShips(enemyBoard);
	addPlayerShips(shipBoard);
	clear();
	displayBoards(attackBoard, shipBoard);
	cout << "\n\n\n";
	cout << "Press enter to continue...";
	cin.get();
	cin.get();
	clear();
	while(!done) {
		clear();
		hit = hitRandom(shipBoard, attackBoard);
		if(hit != '#') {
			int hitNum = shipToNum(hit);
			playerHealth[hitNum]--;
		}
		for(int i = 0; i < 5; i++) {
			if(playerHealth[i] == 0) {
				pause();
				displayBoards(attackBoard, shipBoard);
				cout << "\n\nThe enemy sunk your " << shipNames[i] << "...\n";
				playerHealth[i] = -1;
				if(!isMultipleShips) {
				    xStack = stack<int>();
				    yStack = stack<int>();
				    dirStack = stack<char>();
				    shouldUseLogic = false;
                    AIPrevHit = false;
                    numOfShips = 0;
                    shipsLeft = 0;
                    AIDir =  -1;
                    AIEnds = 0;
				} else {
				    shipsLeft--;
				    if(shipsLeft == 0) {
				        xStack = stack<int>();
    				    yStack = stack<int>();
    				    dirStack = stack<char>();
    				    shouldUseLogic = false;
                        AIPrevHit = false;
                        numOfShips = 0;
                        shipsLeft = 0;
                        AIDir =  -1;
                        AIEnds = 0;
				    }
				}
				didSink = true;
			}
		}
		if(didSink) {
			cout << "Press enter to continue...";
			didSink = false;
		} else {
			cout << "Press enter to continue...";
		}
		cin.get();
		attack(shipBoard, attackBoard, enemyBoard, enemyHealth);
		for(int i = 0; i < 5; i++) {
			if(enemyHealth[i] == 0) {
				clear();
				displayBoards(attackBoard, shipBoard);
				cout << "\n\nYou sunk the enemy's " << shipNames[i] << "!\n";
				enemyHealth[i] = -1;
				cout << "Press enter to continue...";
				cin.get();
			}
		}
		playerWin = true;
		enemyWin = true;
		for(int i = 0; i < 5; i++) {
			if(playerHealth[i] != -1) {
				enemyWin = false;
			}
			if(enemyHealth[i] != -1) {
				playerWin = false;
			}
		}
		if(playerWin || enemyWin) {
			done = true;
		}
	}
	clear();
	if(playerWin) {
		cout << "Congratulations! You Won!";
	} else {
		cout << "Sorry, you lose, try again...";
	}
}

void initializeBoards(char attackBoard[][10], char shipBoard[][10], char enemyBoard[][10]) {
	for(int i = 0; i < 10; i++) {
		for(int a = 0; a <  10; a++) {
			attackBoard[a][i] = ' ';
			shipBoard[a][i] = ' ';
		    enemyBoard[a][i] = ' ';
		}
	}
}

void displayBoards(char attackBoard[][10], char enemyBoard[][10]) {
	cout << "          Attack Board\n" << "   A  B  C  D  E  F  G  H  I  J \n";
	for(int i = 0; i < 10; i++) {
		if(i + 1 != 10) {
			cout << " ";
		}
		cout << i + 1;
		for(int a = 0; a <  10; a++) {
			cout << "[" << attackBoard[a][i] << "]";
		}
		cout << endl;
	}
	cout << "\n           Ship Board\n" << "   A  B  C  D  E  F  G  H  I  J \n";
	for(int i = 0; i < 10; i++) {
		if(i + 1 != 10) {
			cout << " ";
		}
		cout << i + 1;
		for(int a = 0; a <  10; a++) {
			cout << "[" << enemyBoard[a][i] << "]";
		}
		cout << endl;
	}
}

void displayBoard(char board[][10]) {
	cout << "            Your Board\n" << "   A  B  C  D  E  F  G  H  I  J \n";
	for(int i = 0; i < 10; i++) {
		if(i + 1 != 10) {
			cout << " ";
		}
		cout << i + 1;
		for(int a = 0; a <  10; a++) {
			cout << "[" << board[a][i] << "]";
		}
		cout << endl;
	}
}

void placeEnemyShips(char enemyBoard[][10]) {
    bool done;
    int dir;
    int startX;
    int startY;
    int shipLength[] = {5, 4, 3, 3, 2};
    char shipLetter[] = {'A', 'B', 'S', 'C', 'D'};
    for(int ship = 0; ship < 5; ship++) {
        done = false;
        while(!done) {
            dir = rand() % 2;
            startX = rand() % 10;
            startY = rand() % 10;
            done = isShipsOverlap(enemyBoard, startX, startY, dir, shipLength[ship]);
            if(startX + shipLength[ship] >= 10 || startY + shipLength[ship] >= 10) {
				done = false;
            }
        }
        if(dir == 0) {
            for(int x = startX; x < startX + shipLength[ship]; x++) {
                enemyBoard[x][startY] = shipLetter[ship];
            }
        } else {
            for(int y = startY; y < startY + shipLength[ship]; y++) {
                enemyBoard[startX][y] = shipLetter[ship];
            }
        }
    }
}

void addPlayerShips(char shipBoard[][10]) {
    char xChar;
    int x;
    int y;
    int dir;
    int shipNumber = 0;
	int manRand = 0;
    string names[] = {"Aircraft Carrier", "Battleship", "Submarine", "Cruiser", "Destroyer"};
    char shipLetter[] = {'A', 'B', 'S', 'C', 'D'};
    int lengths[] = {5, 4, 3, 3, 2};
    bool done = false;
	bool isOverlap = true;
	
	cout << "Would you like to place your ships manually, or at random?\n";
	cout << "1. Manual\n";
	cout << "2. Random\n";
	cout << "Enter choice (1/2): ";
	cin >> manRand;
	if(manRand == 2) {
		done = true;
		placeEnemyShips(shipBoard);
	}
	
    while(!done) {
		clear();
		displayBoard(shipBoard);
		cout << "\nYou are placing the " << names[shipNumber] << ".\n";
		cout << "Its length is " << lengths[shipNumber] << ".\n\n";
		cout << "Where should the front of the ship be? (A-J): ";
		cin >> xChar;
		while(xChar != 'A' && xChar != 'B' && xChar != 'C' && xChar != 'D' && xChar != 'E' && xChar != 'F' && xChar != 'G' && xChar != 'H' && xChar != 'I' && xChar != 'J') {
			cout << "Please enter a valid starting position\n\n";
			cout << "You are placing the " << names[shipNumber] << ".\n";
			cout << "Its length is " << lengths[shipNumber] << ".\n";
			cout << "Where should the front of the ship be? (A-J): ";
			cin >> xChar;
		}
		x = charToNum(xChar);
		clear();
		displayBoard(shipBoard);
		cout << "\n\nPlease enter the other starting position. (1-10): ";
		cin >> y;
		y--;
		int startX = x;
		int startY = y;
		clear();
		displayBoard(shipBoard);
		cout << "What direction would you like the ship to face?\n";
		cout << "1. vertical\n";
		cout << "2. horizontal\n";
		cin >> dir;
		while(dir != 1 && dir != 2) {
				clear();
				displayBoard(shipBoard);
				cout << "Please enter a valid direction:\n\n";
				cout << "What direction would you like the ship to face?\n";
				cout << "1. vertical\n";
				cout << "2. horizontal\n";
				cin >> dir;
		}
		if(dir == 2) {
			dir -= 2;
		}
		isOverlap = isShipsOverlap(shipBoard, startX, startY, dir, lengths[shipNumber]);
		if(isOverlap) {
			if(dir == 0) {
				for(;x < startX + lengths[shipNumber]; x++) {
					shipBoard[x][y] = shipLetter[shipNumber];
				}
			} else {
				for(;y < startY + lengths[shipNumber]; y++) {
					shipBoard[x][y] = shipLetter[shipNumber];
				}
			}
			if(shipNumber < 4) {
				shipNumber++;
			} else {
				done = true;
			}
		} else {
			cout << "Please enter valid coordinates for the ship...\n";
			pause();
			cin.get();
		}
    }
}

char hitRandom(char shipBoard[][10], char attackBoard[][10]) {
	int x;
	int y;
	bool done = false;
	bool stackGood = false;
	bool justMiss = false;
    char didHitChar;
	string letters = "ABCDEFGHIJ";
	x = rand() % 10;
	y = rand() % 10;
	if(shipBoard[x][y] == '#' || shipBoard[x][y] == '@') {
		done = true;
		hitRandom(shipBoard, attackBoard);
	}
	if(!xStack.empty()) {
	    while(!stackGood) {
	        if(xStack.top() < 0 || yStack.top() < 0 || shipBoard[xStack.top()][yStack.top()] == '@' || shipBoard[xStack.top()][yStack.top()] == '#') {
	            if(xStack.top() < 0 || yStack.top() < 0 || xStack.top() > 0 || yStack.top() > 0) {
	                AIEnds++;
	            }
	            xStack.pop();
	            yStack.pop();
	            dirStack.pop();
	        } else {
	            stackGood = true;
	        }
	    }
	}
	if(!done) {
		if(!shouldUseLogic) {
		    didHitChar = didHit(shipBoard, x, y);
			if(didHit(shipBoard, x, y) != '#') {
				shipBoard[x][y] = '#';
				displayBoards(attackBoard, shipBoard);
				cout << "\nEnemy attacked at " << letters.at(x) << " " << y + 1 << endl;
				cout << "They hit...\n";
				shouldUseLogic = true;
			    numOfShips++;

				xStack.push(x + 1);
				yStack.push(y);
				dirStack.push('R');

				xStack.push(x - 1);
				yStack.push(y);
				dirStack.push('L');

				xStack.push(x);
				yStack.push(y + 1);
				dirStack.push('D');

				xStack.push(x);
				yStack.push(y - 1);
				dirStack.push('U');

			} else {
				shipBoard[x][y] = '@';
				displayBoards(attackBoard, shipBoard);
				cout << "\nEnemy attacked at " << letters.at(x) << " " << y + 1 << endl;
				cout << "They missed!\n";
			}  
		} else {
		    didHitChar = didHit(shipBoard, xStack.top(), yStack.top());
			if(didHit(shipBoard, xStack.top(), yStack.top()) != '#') {
				shipBoard[xStack.top()][yStack.top()] = '#';
				displayBoards(attackBoard, shipBoard);
				cout << "\nEnemy attacked at " << letters.at(xStack.top()) << " " << yStack.top() + 1 << endl;
				cout << "They hit...\n";
				if(!isMultipleShips) {
			        numOfShips++;
				}
			    AIPrevHit = true;
				x = xStack.top();
				y = yStack.top();
		        xStack.pop();
		        yStack.pop();
			    switch(dirStack.top()) {
				case 'U':   dirStack.pop();
				            revPush(x - 1, y, 'L');
				            revPush(x + 1, y, 'R');

							xStack.push(x);
			                yStack.push(y - 1);
		                	dirStack.push('U');
							break;

				case 'D':	dirStack.pop();
				            revPush(x - 1, y, 'L');
				            revPush(x + 1, y, 'R');

							xStack.push(x);
			                yStack.push(y + 1);
		                	dirStack.push('D');
							break;

				case 'L':	dirStack.pop();
				            revPush(x, y - 1, 'U');
				            revPush(x, y + 1, 'D');

							xStack.push(x - 1);
			                yStack.push(y);
			                dirStack.push('L');
							break;

				case 'R':	dirStack.pop();
				            revPush(x, y - 1, 'U');
				            revPush(x, y + 1, 'D');

							xStack.push(x + 1);
            				yStack.push(y);
            				dirStack.push('R');
							break;
				}
			} else {
				shipBoard[xStack.top()][yStack.top()] = '@';
				displayBoards(attackBoard, shipBoard);
				cout << "\nEnemy attacked at " << letters.at(xStack.top()) << " " << yStack.top() + 1 << endl;
				cout << "They missed!\n";
		    	xStack.pop();
		    	yStack.pop();
		    	dirStack.pop();
		    	if(AIPrevHit) {
		    	    AIEnds++;
		    	}
			}
			if(AIEnds == 2 && !isMultipleShips) {
		        isMultipleShips = true;
		        shipsLeft = numOfShips;
		    }
		}
	}
	return didHitChar;
}

void attack(char shipBoard[][10], char attackBoard[][10], char enemyBoard[][10], int enemyHealth[]) {
	int x;
	int y;
	char charX;
	
	clear();
	displayBoards(attackBoard, shipBoard);
	cout << "\n\n\nPlease enter the x cord of the attack (A-J): ";
	cin >> charX;
	clear();
	displayBoards(attackBoard, shipBoard);
	cout << "\n\n\nPlease enter the y cord of the attack (1-10): ";
	cin >> y;
	
	x = charToNum(charX);
	y -= 1;
	
	if(enemyBoard[x][y] == '#') {
		clear();
		cout << "Please enter a coordinate that has not already been entered...\n";
		pause();
		attack(shipBoard, attackBoard, enemyBoard, enemyHealth);
	} else if(enemyBoard[x][y] != ' ') {
		int shipType = shipToNum(enemyBoard[x][y]);
		enemyHealth[shipType]--;
		clear();
		enemyBoard[x][y] = '#';
		attackBoard[x][y] = '#';
		displayBoards(attackBoard, shipBoard);
		cout << "\n\nHit!\n";
		cout << "Press enter to continue...";
		cin.get();
		cin.get();
	} else {
		clear();
		enemyBoard[x][y] = '@';
		attackBoard[x][y] = '@';
		displayBoards(attackBoard, shipBoard);
		cout << "\n\nMiss...\n";
		cout << "Press enter to continue...";
		cin.get();
		cin.get();
	}
}

void revPush(int x, int y, char dir) {
    stack <int> tempX;
    stack <int> tempY;
    stack <char> tempDir;
    while(!xStack.empty()) {
        tempX.push(xStack.top());
        tempY.push(yStack.top());
        tempDir.push(dirStack.top());
        
        xStack.pop();
        yStack.pop();
        dirStack.pop();
    }
	xStack = stack<int>();
	yStack = stack<int>();
	dirStack = stack<char>();
	xStack.push(x);
	yStack.push(y);
	dirStack.push(dir);
	
	while(!tempX.empty()) {
        xStack.push(tempX.top());
        yStack.push(tempY.top());
        dirStack.push(tempDir.top());
        
        tempX.pop();
        tempY.pop();
        tempDir.pop();
    }
}

void clear() {
    for(int i = 0; i < 100; i++) {
        cout << endl;
    }
}

void pause() {
	cout << "Press enter to continue...";
	cin.get();
	clear();
}

bool isShipsOverlap(char board[][10], int x, int y, int dir, int shipLength) {
    bool clear = true;
    int startX = x;
    int startY = y;
    if(dir == 0) {
        for(; x < startX + shipLength; x++) {
            if(board[x][y] != ' ') {
                clear = false;
            }
        }
    } else {
        for(; y < startY + shipLength; y++) {
            if(board[x][y] != ' ') {
                clear = false;
            }
        }
    }
    return clear;
}

char didHit(char board[][10], int x, int y) {
	char shipType;
	if(board[x][y] != ' ') {
		shipType = board[x][y];
	} else {
		shipType = '#';
	}
	return shipType;
}

int charToNum(char letter) {
	return letter - 65;
}

int shipToNum(char letter) {
	int num = 0;
	switch(letter) {
		case 'A':
			num = 0;
		break;
		
		case 'B':
			num = 1;
		break;
		
		case 'S':
			num = 2;
		break;
		
		case 'C':
			num = 3;
		break;
		
		case 'D':
			num = 4;
		break;
	}
	return num;
}