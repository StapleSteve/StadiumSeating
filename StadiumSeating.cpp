/*
*	    name: cts.cpp
*        author: Thomas Moody
* date modified: 28 October 2012
*
*       purpose: Maintains a seating chart for a stadium, allows users to 
*                buy seats, cancel reservations, print out a seating chart, 
*                or close out the program
*
*
*/

#include <iostream>
using namespace std;

struct Seating {
	string price;
	string custID;
	string row;	
	bool filled;
	bool discount;
	int column;
};
 

string const row_letter[26] =
{"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S",
"T" ,"U", "V", "W" , "X","Y","Z"};
int const num_row = 14;
int const num_col = 20;
int const max_num_tix = 12;
int const min_num_tix = 1;
int const hi_price_right_col_limit = 5; //first row = 1
int const hi_price_left_col_limit = 16; //first row = 1
int const hi_price_back_row_limit = 8; //first column = 1
double const lo_price = 150.00;
double const hi_price = 200.00;
double const discount = 0.10;

void populate_stadium ( Seating stadium_array[][num_col] );
void read_in_input ( Seating stadium_array[][num_col], Seating
					  hypothetical_seating[max_num_tix] );
void QU ();
void PR ( Seating stadium_array[][num_col],
		  Seating hypothetical_seating[max_num_tix] );
void CA ( Seating stadium_array[][num_col], string custID, Seating
		  hypothetical_seating[max_num_tix] );
void RQ ( Seating stadium_array[][num_col], string custID, string num_seats,
		  string price );
bool bad_number_seats (int num_seats, string custID);
bool custID_already_reserved (Seating stadium_array[][num_col], string custID);


int main () {

	Seating stadium_array[num_row][num_col];
	Seating hypothetical_seating[max_num_tix]; 
	
	populate_stadium ( stadium_array );
	read_in_input( stadium_array, hypothetical_seating );
	

	return 0;
}

//purpose: fills stadium_array, appropriately fills member variables
//arguments: 2D array of seating
//returns: Nothing (call-by-reference on array)
//effects: Stadium array is filled with empty seats, appropriately priced
//notes: Should only be called once.
void populate_stadium ( Seating stadium_array[][num_col] ) {
	
	int row_number = 0;
	int column = 1;
	
	Seating standard_seating = {"lo","","",false,false,0};

	// i represents row, x represents column
	for ( int i = 0; i < num_row; i++ ) {
			for (int x = 0; x < num_col; x++ ) {
				stadium_array[i][x] = standard_seating;
				stadium_array[i][x].column = column;
				stadium_array[i][x].row = row_letter[row_number];
				
				column++;
				
				
				// set hi price limit based on const values, not ascii for col
				if ( ( x > hi_price_right_col_limit )
					&& ( x < hi_price_left_col_limit ) && ( i <
					hi_price_back_row_limit ) )
					
					stadium_array[i][x].price = "hi";
			}
			row_number++;
		
	}	
	
	
	
}	

//purpose: reads in user request and sends info to appropriate functions
//arguments: 2D array of seating, hypothetical 1D seating array
//returns: nothing
//effects: cout's to screen with appropriate message
//notes: Generally for delegating tasks
void read_in_input ( Seating stadium_array[][num_col], Seating
				      hypothetical_seating[max_num_tix] ) {
	string request;
	string custID;
	int num_seats;
	string price;

	
	cin >> request;
	
	if ( request == "QU" ) {
		return;
	}
	
	if ( request == "PR" ) {
			PR ( stadium_array, hypothetical_seating );
			return;
	}	
	
	cin >> custID;
	
	if ( request == "CA" ) {
			CA ( stadium_array, custID, hypothetical_seating );
			return;
	}		
	
	cin >> num_seats;
	cin >> price;
	
}

//purpose: Quits program
//arguments: none
//returns: bool
//effects: program terminates (passed along through read_in_input)
//notes: none
void QU () {
	return;	
}


//purpose: prints out current seating arrangement
//arguments: stadium array, hypothetical seatin array
//returns: nothing
//effects: cout's the current array in a readable way, calculates revenue
//notes: Doesn't alter array, only time revenue is counted and needs to
//check every seat anyway, so contains code for summing revenue
void PR ( Seating stadium_array[][num_col],
		  Seating hypothetical_seating[max_num_tix] ) {
	
	double sum = 0.0;
	
	for ( int i = 0; i < num_row; i++ ) {
		cout << endl;
			for (int x = 0; x < num_col; x++ ) {
				if ( !stadium_array[i][x].filled ) {
					cout << "---";
				}
				else {
						cout << stadium_array[i][x].custID;
						
						if ( stadium_array[i][x].discount ) {
							if ( stadium_array[i][x].price == "hi" ) {
									sum += ( hi_price*discount );
							}
							else {
									sum+= ( lo_price*discount );
							}	
						
						}
						else {
							if ( stadium_array[i][x].price == "hi" ) {
									sum += ( hi_price );
							}
							else {
									sum+= ( lo_price );
							}									
						}	
				}	
			}
	}
	cout << endl << "Revenue: " << ( sum ) << endl;
	read_in_input( stadium_array, hypothetical_seating );
}	
	
//purpose: Cancels an existing reservation, refunds money
//arguments: stadium array, string custID, hypothetical array
//returns: nothing
//effects: empties the array of seats owned by a certain custID
//notes: Changes stadium array	
void CA ( Seating stadium_array[][num_col], string custID, Seating
		  hypothetical_seating[max_num_tix] ) {
	
	bool cancelled = false;
	
	for ( int i = 0; i < num_row; i++ ) {
		for (int x = 0; x < num_col; x++ ) {	
				if ( stadium_array[i][x].custID == custID ) {
					stadium_array[i][x].custID = "";					
					stadium_array[i][x].filled = false;
					stadium_array[i][x].discount = false;
					cancelled = true;
				}	
		}
	}
	if (cancelled) {
			cout << "OK " << custID << " cancelled" << endl;
	}
	else {
			cout << "NO " << custID << " no-reservation" << endl;
	}	
	
	read_in_input(stadium_array, hypothetical_seating);
}	

//purpose: Handles reservations, fills seats (and member variables)
//arguments: Stadium array, hypothetical seating array, string custID, string
//#seats, string prices
//returns: nothing
//effects: Passes on info to approriate function based on its arguments for
//filling seats, checks if all the neccesary conditions are met
//then tries to fill seats. Returns true when seating was succesful
//notes: changes array when necc. conditions are met
void RQ ( Seating stadium_array[][num_col], string custID, int num_seats,
		  string price ) {
	
	if ( bad_number_seats (num_seats, custID) ) {
		return;	
	}
	
	if ( custID_already_reserved (stadium_array, custID) ) {
		return;
	}	
	
	
	
	
	
	
}

//purpose: checks if the # of seats requested is legal
//arguments: string # seats
//returns: bool
//effects: none itself, affects RQ
//notes:
bool bad_number_seats (int num_seats, string custID){
	
	if ( (num_seats >= min_num_tix) && (num_seats <= max_num_tix) ) {
		return false;
	}	
	
	else {
		cout << "NO " << custID << " bad-number" << endl;
		return true;
	}	
	
	
}	

bool custID_already_reserved (Seating stadium_array[][num_col],
							   string custID) {
	
	for ( int i = 0; i < num_row; i++ ) {
		for (int x = 0; x < num_col; x++ ) {
			if ( stadium_array[i][x].custID == custID ) {
				cout << "NO " << custID << " already-reserved" << endl;
				return true;
			}
		}	
	}
	
	return false;
}	

	