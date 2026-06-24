// Project description: This systmem allow users to register, login, 
//						book flights, edit bookings, make payment, check in 
//						and print invoice
// Author: Kang Kah Yi, Wong Pei En, Teh Ping Sin
// Date start: 02/04/2025
// Date end: 04/05/2025

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cctype>
#include <windows.h> //for decoration
using namespace std;

// Structure to store name information
struct Name {
	string firstName;
	string lastName;
};

// Structure to store user account information
struct User {
	Name name;
	string mobileNo;
	string email;
	string username;
	string password;
};

// Structure to store flight booking details
struct Booking {
	Name name;
	int depart;
	string dateDepart;
	int timeDepart;
	int Return;
	string dateReturn;
	int timeReturn;
};

// Structure to store check in details
struct CheckIn {
	Name name;
	string passport;
	Name contactName;
	string contactNo;
};

// Structure to store payment and check-in status and details
struct PayCheck {
	double amountPay = 0;
	string statusPay;
	CheckIn passenger[10];
	string statusCheckin;
};


#define SIZE 100 
string currentUser; //current user to use in whole program
Booking flight[SIZE]; //flight detail to use in payment and checkin
int noTicket = 0; //number of ticket to use in payment, readPaymentCheckIn, checkIn and printInvoice

//core system functions
void mainmenu();
void menu();
void FlightSchedule();
void registration();
void readUser(User[], int&);
int login(User[], int);
void performBooking();
void readBooking(Booking[], int&);
void editBooking(Booking[], int);
void payment();
void readPaymentCheckIn(PayCheck&);
void checkIn(PayCheck&);
void printInvoice(Booking[], PayCheck);
void printPaymentDetail(const string[], const int[], const string[], int, int, int);

//validation functions
bool isValidEmail(string);
bool isUsernameTaken(string);
bool isValidPassword(string);
bool isLeapYear(int);
bool isValidDate(string);
bool checkDateDepartReturn(string, string);
bool cardNumValidation(string);
int cardDateValidation(string);
bool cardSectyCode(string);
bool phoneValidation(string);
bool passportValidation(string);
bool isNum(string);
void formatName(string& name);

//decoration functions
void printRed(const string&);
void printGreen(const string&);
void printYellow(const string&);
void printBlue(const string&);
void printPink(const string&);
void flightSlot();
void flightTimeSlot();
void printHeader();
void printLoginPage();
void printRegistrationPage();
void printBookingPage();
void printEditBookingPage();
void printBookingDetail(const Booking[]);
void printBookingFlightDetail(const Booking[]);
void printPaymentPage();
void printCheckinPage();
void printQuitPage();


// ================= Main Function ===============
int main() {
	User user[SIZE]; //store registered users
	Booking book[SIZE]; //store bookings
	PayCheck payCheck; //store payment and checkin info

	//control variable
	string choice, control = "1", select = "0", cont = "1";
	int numUser = 0, numBook = 0, loginStatus = 0;
	bool createdFile = false;

	//check if user.txt exist
	ifstream infile(currentUser + "user.txt");
	if (infile.is_open())
		createdFile = true;
	infile.close();


	do {
		//Display main menu and get user choose to register or login
		mainmenu();
		cout << "						Choice: ";
		cin >> choice;
		cin.ignore();

		//Registation
		if (choice == "1") {
			system("cls");
			registration();
			createdFile = true;

			do {
				cout << "Do you want to continue login? (1-yes, 2-no):";
				cin >> control;

				if (control == "1" || control == "2")
					break;

				else {
					printRed("Incorrect input!");
				}
			} while (true);

			if (control == "2") {
				system("cls");
				printQuitPage();
				return 0;
			}
		}

		//Login
		else if (choice == "2") {
			system("cls");
			if (createdFile == true) {
				readUser(user, numUser);
			}
			loginStatus = login(user, numUser);

			if (loginStatus == 1) { //username and password correct
				printGreen("Login successful.");
				Sleep(1000);
				control = 2;
			}

			else if (loginStatus == 2) //password wrong
				printRed("Username correct, password incorrect.");

			else { //username and password wrong
				printRed("Username and password incorrect.");
				printRed("You may need to register first!");
			}
			cout << endl;

			if (loginStatus != 1) {
				do {
					cout << "Do you want to continue login? (1-yes, 2-no):";
					cin >> control;
					if (control == "2") {
						system("cls");
						printQuitPage();
						return 0;
					}
					else if (control == "1") {
						break;
					}
					else {
						printRed("Incorrect input!");
						cout << endl;
					}
				} while (true);
			}
		}

		//Quit program
		else if (choice == "3") {
			system("cls");
			printQuitPage();
			return 0;
		}

		//validation
		else {
			printRed("Invalid option!! Please reselect (1-3)");
			system("pause");
		}

		system("cls");

	} while (control == "1");

	//Check if _Booking.txt of current user exist
	createdFile = false;
	ifstream infile1(currentUser + "_Booking.txt");
	if (infile1.is_open())
		createdFile = true;
	infile1.close();

	//After login successful
	do {
		//Display menu and get user choose to book flights, 
		//edit bookings, make payment, check in or print invoice
		printHeader();
		cout << endl;
		cout << "  				Username: " << currentUser << endl;
		menu();

		//get booking details and payment check in detail after each loop if file exist
		//and store the booking details into global struct flight
		if (createdFile == true) {
			readBooking(book, numBook);
			noTicket = numBook;

			readPaymentCheckIn(payCheck);

			for (int i = 0; i < noTicket; i++) {
				flight[i] = book[i];
			}
		}

		cout << "				Enter your choice (1-6): ";
		cin >> select;

		system("cls");

		cin.ignore();

		//book flight
		if (select == "1") {
			performBooking();
			createdFile = true; //after perform booking, _Booking.txt exist
			//reset payment and checkin status
			payCheck.statusPay = "Not Paid";
			payCheck.statusCheckin = "Not Check";
		}

		//edit bookings
		else if (select == "2") {
			//if no perform booking yet, user cannot edit booking
			if (numBook == 0) {
				printRed("No booking available, please book your flight ticket(s)!");
				system("pause");
			}

			else {
				editBooking(book, numBook);
				//reset payment and checkin status
				payCheck.statusPay = "Not Paid";
				payCheck.statusCheckin = "Not Check";
			}
		}

		//make payment
		else if (select == "3") {
			//if no perform booking yet, user cannot make payment
			if (numBook == 0) {
				printRed("No booking available, please book your flight ticket(s) first!");
				system("pause");
			}
			//if payment had done, use cannot re-payment
			else if (payCheck.statusPay == "Paid") {
				printGreen("Payment for flight ticket(s) already completed, please proceed to check-in flight!");
				system("pause");
			}
			else {
				payment();
			}
		}

		//check in flight
		else if (select == "4") {
			//if no perform booking yet, user cannot check in flight
			if (numBook == 0) {
				printRed("No booking available, please book your flight ticket(s) first!");
				system("pause");
			}
			//if check in had done, user cannot re-check in
			else if (payCheck.statusCheckin == "Checked") {
				printGreen("Check-in of flight already completed!");
				system("pause");
			}
			else if (payCheck.statusPay == "Paid" && payCheck.statusCheckin == "Not Check") {
				checkIn(payCheck);
			}
			//if payment not yet paid, user cannot check in flight
			else {
				cout << "Payment Status: Not Paid" << endl;
				printRed("No payment for the flight ticket(s) yet, please proceed to make payment before check-in. Thank you!");
				system("pause");
			}
		}

		//print invoice
		else if (select == "5") {
			//if no perform booking yet, user cannot print invoice
			if (numBook == 0) {
				printRed("No booking available, please book your flight ticket(s) first!");
				system("pause");
			}
			else if (payCheck.statusPay == "Paid" && payCheck.statusCheckin == "Checked")
				printInvoice(book, payCheck);
			//if either payment or checkin not yet done, user cannot print invoice
			else {
				cout << "Payment Status: " << payCheck.statusPay << endl;
				cout << "Check-in Status: " << payCheck.statusCheckin << endl;
				printRed("Please proceed to make payment or check-in before print invoice. Thank you!");
				system("pause");
			}
		}

		//quit program
		else if (select == "6") {
			printQuitPage();
			system("pause");
			cont = "0";
		}

		//validation
		else {
			printRed("Not available choice!! Please reselect (1-6)");
			system("pause");
		}

		//reset _paymentCheckIn.txt and _invoice.txt of current user if booking changed
		if (select == "1" || select == "2") {
			ofstream outFile1(currentUser + "_paymentCheckIn.txt");
			outFile1 << payCheck.amountPay << endl << "Not Paid" << endl;
			//write NULL to reset check in
			for (int i = 0; i < noTicket * 6; i++) {
				outFile1 << "NULL" << endl;
			}
			outFile1 << "Not Check";
			outFile1.close();

			ofstream outFile2(currentUser + "_invoice.txt");
			outFile2.close();
		}

		system("cls");
	} while (cont == "1");

	return 0;
}

//Display main menu selection for register, login and quit
void mainmenu() {
	printBlue(R"(
			  __        __  _____   _        ____    ___    __  __   _____     _____    ___  
			  \ \      / / | ____| | |      / ___|  / _ \  |  \/  | | ____|   |_   _|  / _ \ 
	 		   \ \ /\ / /  |  _|   | |     | |     | | | | | |\/| | |  _|       | |   | | | |
			    \ V  V /   | |___  | |___  | |___  | |_| | | |  | | | |___      | |   | |_| |
			     \_/\_/    |_____| |_____|  \____|  \___/  |_|  |_| |_____|     |_|    \___/ 
     _   ____        _   _  __       _      ___   ____    _       ___   _   _   _____   ____      ____   __   __  ____    _____   _____   __  __ 
    | | / ___|      | | | |/ /      / \    |_ _| |  _ \  | |     |_ _| | \ | | | ____| / ___|    / ___|  \ \ / / / ___|  |_   _| | ____| |  \/  |
 _  | | \___ \   _  | | | ' /      / _ \    | |  | |_) | | |      | |  |  \| | |  _|   \___ \    \___ \   \ V /  \___ \    | |   |  _|   | |\/| |
| |_| |  ___) | | |_| | | . \     / ___ \   | |  |  _ <  | |___   | |  | |\  | | |___   ___) |    ___) |   | |    ___) |   | |   | |___  | |  | |
 \___/  |____/   \___/  |_|\_\   /_/   \_\ |___| |_| \_\ |_____| |___| |_| \_| |_____| |____/    |____/    |_|   |____/    |_|   |_____| |_|  |_|)");

	printYellow(R"(
						+--------------------------------+
						|           MAIN MENU            |
						+--------------------------------+
						|  1  | Register New User        |
						|  2. | Login                    |
						|  3. | Quit                     |
						+--------------------------------+)");
}

//Display menu selection after login succesfull for book flights, 
//edit bookings, make payment, check in, print invoice or quit
void menu() {
	cout << "				+-----+------------------------------+\n";
	cout << "				| No. |          OPTIONS             |\n";
	cout << "				+-----+------------------------------+\n";
	cout << "				|  1  | Book Flight Ticket(s)        |\n";
	cout << "				+-----+------------------------------+\n";
	cout << "				|  2  | Edit Booking                 |\n";
	cout << "				+-----+------------------------------+\n";
	cout << "				|  3  | Perform Payment              |\n";
	cout << "				+-----+------------------------------+\n";
	cout << "				|  4  | Check In Flight              |\n";
	cout << "				+-----+------------------------------+\n";
	cout << "				|  5  | Print Invoice                |\n";
	cout << "				+-----+------------------------------+\n";
	cout << "				|  6  | Quit                         |\n";
	cout << "				+-----+------------------------------+\n\n";
}

//Display flight schedule include route, price and time slot
void FlightSchedule() {
	cout << "  =========================================       ==================================\n";
	cout << "  |         FLIGHT ROUTE & PRICES         |       |   DEPARTURE/RETURN TIME SLOT   |\n";
	cout << "  +-----+--------------------+------------+       +-----+--------------------------+\n";
	cout << "  | No. |       Route        |   Price    |       | No. |          Time            |\n";
	cout << "  +-----+--------------------+------------+       +-----+--------------------------+\n";
	cout << "  |  1  | KL - Penang        |   RM200    |       |  1  | 8:00 A.M.                |\n";
	cout << "  |  2  | Penang - KL        |   RM200    |       |  2  | 13:00 P.M.               |\n";
	cout << "  |  3  | KL - Johor         |   RM200    |       |  3  | 18:00 P.M.               |\n";
	cout << "  |  4  | Johor - KL         |   RM200    |       |  4  | 23:00 P.M.               |\n";
	cout << "  |  5  | KL - Singapore     |   RM250    |       +-----+--------------------------+\n";
	cout << "  |  6  | Singapore - KL     |   RM250    |\n";
	cout << "  |  7  | KL - Bangkok       |   RM300    |\n";
	cout << "  |  8  | Bangkok - KL       |   RM300    |\n";
	cout << "  +-----+--------------------+------------+\n\n";
}

//Function readUser - to register new user and save into "user.txt"
void registration() {
	User newUser;
	string confirm;

	//get user first name, last name, mobile number, email, username and passowrd
	printRegistrationPage();
	cout << "Please key in details for registration:\n";
	cout << "First name: ";
	getline(cin, newUser.name.firstName);
	formatName(newUser.name.firstName);
	cout << "Last name: ";
	getline(cin, newUser.name.lastName);
	formatName(newUser.name.lastName);
	// Mobile number input and validation
	do {
		cout << "Mobile No: ";
		getline(cin, newUser.mobileNo);
		if (!phoneValidation(newUser.mobileNo)) {
			printRed("Invalid mobile phone number! Try again.");
			cout << endl;
		}
	} while (!phoneValidation(newUser.mobileNo));

	// Email input and validation
	do {
		cout << "Email: ";
		getline(cin, newUser.email);
		if (!isValidEmail(newUser.email)) {
			printRed("Invalid Email! Try again. (xxx@gmail.com)");
			cout << endl;
		}
	} while (!isValidEmail(newUser.email));

	// Username input and validation
	do {
		cout << "Username:";
		getline(cin, newUser.username);
		if (isUsernameTaken(newUser.username)) {
			printRed("Username already taken! Try another.");
			cout << endl;
		}
	} while (isUsernameTaken(newUser.username));

	// Password input and validation
	do {
		cout << "Password (At least 8 characters, 1 symbol, 1 uppercase letter, 1 number): ";
		getline(cin, newUser.password);
		if (!isValidPassword(newUser.password)) {
			printRed("Invalid password! Try again.");
			cout << endl;
		}
	} while (!isValidPassword(newUser.password));

	do {
		cout << "Confirm to register? (y-yes, n-no): ";
		cin >> confirm;

		//validation
		for (int i = 0; i < confirm.length(); i++) {
			confirm[i] = tolower(confirm[i]);
		}

		//if user confirm to register, store user information into user.txt
		if (confirm == "y") {
			ifstream inFile("user.txt");
			ofstream outFile("user.txt", ios::app);
			if (outFile.is_open()) {
				if (inFile.is_open()) {
					outFile << endl;
				}
				outFile << newUser.name.firstName << endl;
				outFile << newUser.name.lastName << endl;
				outFile << newUser.mobileNo << endl;
				outFile << newUser.email << endl;
				outFile << newUser.username << endl;
				outFile << newUser.password;
				printGreen("Registration successful!");
			}
			else {
				printRed("Error writing to file.");
			}
			inFile.close();
			outFile.close();
			break;
		}

		//if user not confirm to register, registration cancelled
		else if (confirm == "n") {
			printRed("Registration cancelled.");
			return;
		}

		//validation
		else {
			printRed("Invalid input! Try again.");
			cout << endl;
		}
	} while (true);
}

//Function readUser - read all the registered users listed in "user.txt"
//                    and store the details into the respective parameters
void readUser(User userList[], int& numUser) {
	ifstream inFile("user.txt");
	numUser = 0;

	if (inFile.is_open()) {
		//read first name, last name mobile number, email, username and password of registered user
		while (getline(inFile, userList[numUser].name.firstName)) {
			getline(inFile, userList[numUser].name.lastName);
			getline(inFile, userList[numUser].mobileNo);
			getline(inFile, userList[numUser].email);
			getline(inFile, userList[numUser].username);
			getline(inFile, userList[numUser].password);
			numUser++;
		}
	}
	else {
		printRed("Unable to open the file.");
	}
	inFile.close();
}

//Function login - for user to login with the registered username and password
int login(User userList[], int numUser) {
	string usernameInput, passwordInput;
	bool usernameFound = false;
	bool passwordCorrect = false;

	printLoginPage();
	//get username and password
	cout << "Username: ";
	getline(cin, usernameInput);
	cout << "Password: ";
	getline(cin, passwordInput);

	//check whether the username and password can be found
	for (int i = 0; i < numUser; i++) {
		if (userList[i].username == usernameInput) {
			usernameFound = true;
			if (userList[i].password == passwordInput) {
				passwordCorrect = true;
				currentUser = userList[i].username; // Set global current user
				return 1; // username and password correct
			}
		}
		else if (userList[i].password == passwordInput) {
			passwordCorrect = true;
		}
	}

	if (usernameFound && !passwordCorrect)
		return 2; // username correct, password wrong
	else
		return 3; // username and password wrong
}

//Function performBooking - book flight ticket(s) and store the booking information into "XX_Booking.txt"
void performBooking() {
	int numPassenger = 0;
	string FName[10], LName[10];
	int depart = 0, timeDepart = 0, Return = 0, timeReturn = 0;
	string dateDepart, dateReturn;

	//validation variable
	string confirm;
	string numPassengerStr;
	bool repeat;
	string departStr, timeDepartStr, ReturnStr, timeReturnStr;

	//Make booking
	if (noTicket == 0) {
		do {
			//display flight schedule
			printBookingPage();
			FlightSchedule();

			//get total number of passenger to book
			cout << "Number of passenger(s):";
			cin >> numPassengerStr;
			//validation
			if (isNum(numPassengerStr)) {
				numPassenger = stoi(numPassengerStr);
			}
			if (numPassenger > 0 && numPassenger <= 100)
			{
				cout << "\nPlease provide the details as below:\n\n";

				//get flight details include route, price and time slot 
				//of departure and return flight
				cout << "====Flight Details====" << endl;
				cout << "Departure Flight: ";
				cin >> departStr;
				//depart flight validation
				if (isNum(departStr)) {
					depart = stoi(departStr);
				}
				while (depart < 1 || depart > 8) {
					cout << "Please input valid Departure Flight Number (1-8): ";
					cin >> departStr;
					if (isNum(departStr)) {
						depart = stoi(departStr);
					}
				}

				cout << "Date of Departure (DD/MM/YYYY): ";
				cin >> dateDepart;
				//depart date validation
				while (!isValidDate(dateDepart)) {
					cout << "Please input valid Date of Departure (DD/MM/YYYY): ";
					cin >> dateDepart;
				}

				cout << "Slot of Departure: ";
				cin >> timeDepartStr;
				//depart time validation
				if (isNum(timeDepartStr)) {
					timeDepart = stoi(timeDepartStr);
				}
				while (timeDepart < 1 || timeDepart > 4) {
					cout << "Please input valid Slot of Departure (1-4): ";
					cin >> timeDepartStr;
					if (isNum(timeDepartStr)) {
						timeDepart = stoi(timeDepartStr);
					}
				}

				cout << "Return Flight: ";
				cin >> ReturnStr;
				//return flight validation
				if (isNum(ReturnStr)) {
					Return = stoi(ReturnStr);
				}
				while (Return < 1 || Return > 8) {
					cout << "Please input valid Return Flight Number (1-8): ";
					cin >> ReturnStr;
					if (isNum(ReturnStr)) {
						Return = stoi(ReturnStr);
					}
				}

				cout << "Date of Return (DD/MM/YYYY): ";
				cin >> dateReturn;
				//return date validation
				while (!isValidDate(dateReturn)) {
					cout << "Please input valid Date of Return (DD/MM/YYYY): ";
					cin >> dateReturn;
				}
				while (!checkDateDepartReturn(dateDepart, dateReturn)) {
					printRed("Return date must later than departure date!!!");
					cout << endl;
					cout << "Date of Return (DD/MM/YYYY): ";
					cin >> dateReturn;
					while (!isValidDate(dateReturn)) {
						cout << "Please input valid Date of Return (DD/MM/YYYY): ";
						cin >> dateReturn;
					}
				}

				cout << "Slot of Return: ";
				cin >> timeReturnStr;
				//return time validation
				if (isNum(timeReturnStr)) {
					timeReturn = stoi(timeReturnStr);
				}
				while (timeReturn < 1 || timeReturn > 4) {
					cout << "Please input valid Slot of Return (1-4): ";
					cin >> timeReturnStr;
					if (isNum(timeReturnStr)) {
						timeReturn = stoi(timeReturnStr);
					}
				}
				cout << endl << endl;

				//get all passenger details
				cin.ignore();
				cout << "====Passenger Details====" << endl;
				for (int i = 0; i < numPassenger; i++) {
					do {
						repeat = false;
						cout << "Passenger " << i + 1 << endl;
						cout << "First Name: ";
						getline(cin, FName[i]);
						formatName(FName[i]);
						cout << "Last Name: ";
						getline(cin, LName[i]);
						formatName(LName[i]);
						//validation for no repeated passenger
						for (int j = 0; j < i; j++) {
							if (FName[j] == FName[i] && LName[j] == LName[i]) {
								printRed("The passenger had booked the ticket!! Please enter a new passenger!!");
								repeat = true;
								break;
							}
						}
						cout << endl;
					} while (repeat == true);
				}

				do {
					cout << "\nConfirm to book the flight? (y-yes, n-no): ";
					cin >> confirm;

					//validation
					for (int i = 0; i < confirm.length(); i++) {
						confirm[i] = tolower(confirm[i]);
					}

					//If user confirm of booking, store the details into "XX_Booking.txt"
					if (confirm == "y") {
						ofstream outfile(currentUser + "_Booking.txt", ios::app);
						for (int i = 0; i < numPassenger; i++) {
							outfile << FName[i] << endl << LName[i] << endl << depart << endl << dateDepart << endl
								<< timeDepart << endl << Return << endl << dateReturn << endl << timeReturn;
							if (i != numPassenger - 1)
								outfile << endl;
						}
						outfile.close();
						printGreen("BOOKING SUCCESSFUL!!!");
						system("pause");
					}

					//if user not confirm booking, booking cancelled
					else if (confirm == "n") {
						printRed("BOOKING CANCELLED !!!");
						system("pause");
					}

					//validation
					else {
						printRed("INVALID OPTION !!!");
					}
				} while (confirm != "y" && confirm != "n");
			}

			//validation
			else {
				printRed("Invalid, Must between 1-100 passenger");
				system("pause");
				system("cls");
			}
		} while (numPassenger <= 0 || numPassenger > 100);
	}

	//Make re-booking - add passenger
	else {
		do {
			//display booking detail booked before
			printBookingPage();
			cout << "This is the booking details booked before" << endl;
			printBookingDetail(flight);

			//get total number of passenger to book
			printYellow("~Proceed to add on passenger~");
			cout << "Number of passenger(s):";
			cin >> numPassengerStr;
			//validation
			if (isNum(numPassengerStr)) {
				numPassenger = stoi(numPassengerStr);
			}
			if (numPassenger > 0 && numPassenger <= 100) {
				//get all passenger details
				cin.ignore();
				cout << "====Passenger Details====" << endl;
				for (int i = 0; i < numPassenger; i++) {
					do {
						repeat = false;
						cout << "Passenger " << i + 1 << endl;
						cout << "First Name: ";
						getline(cin, FName[i]);
						formatName(FName[i]);
						cout << "Last Name: ";
						getline(cin, LName[i]);
						formatName(LName[i]);
						//validation for no repeated passenger
						for (int j = 0; j < i; j++) {
							if (FName[j] == FName[i] && LName[j] == LName[i]) {
								printRed("The passenger had booked the ticket!! Please enter a new passenger!!");
								repeat = true;
								break;
							}
						}
						for (int k = 0; k < noTicket; k++) {
							if (flight[k].name.firstName == FName[i] && flight[k].name.lastName == LName[i]) {
								printRed("The passenger had booked the ticket!! Please enter a new passenger!!");
								repeat = true;
								break;
							}
						}
						cout << endl;
					} while (repeat == true);
				}

				do {
					cout << "\nConfirm to book the flight? (y-yes, n-no): ";
					cin >> confirm;

					//validation
					for (int i = 0; i < confirm.length(); i++) {
						confirm[i] = tolower(confirm[i]);
					}

					//If user confirm of booking, store the details into "XX_Booking.txt"
					if (confirm == "y") {
						ofstream outfile(currentUser + "_Booking.txt", ios::app);
						outfile << endl;
						for (int i = 0; i < numPassenger; i++) {
							outfile << FName[i] << endl << LName[i] << endl << flight[0].depart << endl << flight[0].dateDepart << endl
								<< flight[0].timeDepart << endl << flight[0].Return << endl << flight[0].dateReturn << endl << flight[0].timeReturn;
							if (i != numPassenger - 1)
								outfile << endl;
						}
						outfile.close();
						printGreen("RE-BOOKING SUCCESSFUL!!!");
						printGreen("Please make payment and checkin again~");
						system("pause");
					}

					//if user not confirm booking, booking cancelled
					else if (confirm == "n") {
						printRed("RE-BOOKING CANCELLED !!!");
						system("pause");
					}

					//validation
					else {
						printRed("INVALID OPTION !!!");
					}
				} while (confirm != "y" && confirm != "n");
			}

			//validation
			else {
				printRed("Invalid, Must between 1-100 passenger");
				system("pause");
				system("cls");
			}
		} while (numPassenger <= 0 || numPassenger > 100);
	}
}

//Function readBooking - read all the items listed in "XX_Booking.txt"
//						 and store the data into the respective parameters
void readBooking(Booking book[], int& count) {
	ifstream infile(currentUser + "_Booking.txt");
	count = 0;

	if (infile.is_open()) {
		//read first name and last name of passenger, departure route, departure date, 
		//departure time,  return route, return date, return time
		while (!infile.eof()) {
			getline(infile, book[count].name.firstName);
			getline(infile, book[count].name.lastName);
			infile >> book[count].depart;
			infile >> book[count].dateDepart;
			infile >> book[count].timeDepart;
			infile >> book[count].Return;
			infile >> book[count].dateReturn;
			infile >> book[count].timeReturn;
			infile.ignore();
			count++;
		}
	}
	else
		printRed("Unable to open the file!");

	infile.close();
}

//Function editBooking - enable user to edit any of the details of the passenger(s) 
void editBooking(Booking book[], int count) {
	int change = 0;
	int select = 0;
	string edit, amend1, amend2;
	int amendNum = 0;
	string confirm;

	//validation
	string changeStr;
	string selectStr;
	bool repeat;

	//show booking details of current user
	printEditBookingPage();
	cout << "Follow the details of the 1st Passenger " << book[0].name.firstName << " " << book[0].name.lastName << endl;
	printBookingDetail(book);

	do {
		//get which detials user want to change
		cout << "\n1 - Passenger Name" << endl;
		cout << "2 - Flight Detail" << endl;
		cout << "3 - Quit" << endl;
		cout << "Which details do you want to change?: ";
		cin >> changeStr;
		//validation
		if (isNum(changeStr)) {
			change = stoi(changeStr);
		}

		//change passenger name
		if (change == 1) {
			system("cls");
			//diaplay current passenger name
			printYellow("~Edit Passenger Name~");
			cout << "+-----------+----------------------------------------+" << endl;
			cout << "| Passenger | Name                                   |" << endl;
			cout << "+-----------+----------------------------------------+" << endl;
			for (int i = 0; i < count; i++) {
				cout << "|     " << setw(5) << left << (to_string(i + 1))
					<< " | " << setw(39) << left << (book[i].name.firstName + " " + book[i].name.lastName) << "|" << endl;
			}
			cout << "+-----------+----------------------------------------+" << endl;

			do {
				//get which passanger user want to change
				cout << "Select which passenger name you want to change (1-" << count << "): ";
				cin >> selectStr;
				//validation
				if (isNum(selectStr)) {
					select = stoi(selectStr);
				}
				cin.ignore();
				if (select > 0 && select <= count) {
					do {
						//get new first name and last name of passenger
						repeat = false;
						cout << "New first name: ";
						getline(cin, amend1);
						formatName(amend1);
						cout << "New last name: ";
						getline(cin, amend2);
						formatName(amend2);
						//check repeated passenger
						for (int i = 0; i < noTicket; i++) {
							if (book[i].name.firstName == amend1 && book[i].name.lastName == amend2) {
								printRed("The passenger had booked the ticket!! Please enter a new passenger!!");
								repeat = true;
								break;
							}
						}
					} while (repeat == true);
				}

				//validation
				else {
					printRed("INVALID CHOICE !!!");
					cout << endl;
				}
			} while (select <= 0 || select > count);

		}

		//change flight details
		else if (change == 2) {
			system("cls");
			//get which flight details user want to change
			printBookingFlightDetail(book);
			cout << endl;
			cout << "  Please select the item to amend" << endl;
			cout << "  +-----+-----------------------+" << endl;
			cout << "  | No. |      Amendment        |" << endl;
			cout << "  +-----+-----------------------+" << endl;
			cout << "  |  1  | Departure Route       |" << endl;
			cout << "  |  2  | Departure Date        |" << endl;
			cout << "  |  3  | Departure Slot        |" << endl;
			cout << "  |  4  | Return Route          |" << endl;
			cout << "  |  5  | Return Date           |" << endl;
			cout << "  |  6  | Return Slot           |" << endl;
			cout << "  +-----+-----------------------+" << endl;
			cout << "  Select: ";
			cin >> selectStr;
			//validation
			if (isNum(selectStr)) {
				select = stoi(selectStr);
			}
			while (select < 1 || select > 6) {
				printRed("INVALID SELECT NUMBER !!!");
				cout << "Please input valid item to amend (1-6): ";
				cin >> selectStr;
				if (isNum(selectStr)) {
					select = stoi(selectStr);
				}
			}

			system("cls");
			cout << "Follow the details of the 1st Passenger " << book[0].name.firstName << " " << book[0].name.lastName << endl;
			printBookingFlightDetail(book);
			cout << endl;

			//change departure route
			if (select == 1) {
				flightSlot();
				printYellow("~Amend Departure Route~");
				cout << "Old departure route: " << book[0].depart << endl;
				cout << "New departure route: ";
				cin >> edit;
				//validation
				if (isNum(edit)) {
					amendNum = stoi(edit);
				}
				while (amendNum < 1 || amendNum > 8) {
					cout << "Invalid input. Please enter a valid departure route (1-8): ";
					cin >> edit;
					if (isNum(edit)) {
						amendNum = stoi(edit);
					}
				}
			}

			//change departure date
			else if (select == 2) {
				printYellow("~Amend Departure Date~");
				cout << "Old departure date: " << book[0].dateDepart << endl;
				cout << "New departure date: ";
				cin >> amend1;
				//check date format
				while (!isValidDate(amend1)) {
					cout << "Please input valid New Date of Departure (DD/MM/YYYY): ";
					cin >> amend1;
				}
				//check depart date is earlier than return date
				while (!checkDateDepartReturn(amend1, book[0].dateReturn)) {
					printRed("Depart date must earlier than Return date!!!");
					cout << endl;
					cout << "New departure date: ";
					cin >> amend1;
					while (!isValidDate(amend1)) {
						cout << "Please input valid New Date of Departure (DD/MM/YYYY): ";
						cin >> amend1;
					}
				}
			}

			//change departure slot
			else if (select == 3) {
				flightTimeSlot();
				printYellow("~Amend Departure Slot~");
				cout << "Old departure slot: " << book[0].timeDepart << endl;
				cout << "New departure slot: ";
				cin >> edit;
				//validation
				if (isNum(edit)) {
					amendNum = stoi(edit);
				}
				while (amendNum < 1 || amendNum > 4) {
					cout << "Invalid input. Please enter a valid Slot of Departure (1-4): ";
					cin >> edit;
					if (isNum(edit)) {
						amendNum = stoi(edit);
					}
				}
			}

			//change return route
			else if (select == 4) {
				flightSlot();
				printYellow("~Amend Return Route~");
				cout << "Old return route: " << book[0].Return << endl;
				cout << "New return route: ";
				cin >> edit;
				//validation
				if (isNum(edit)) {
					amendNum = stoi(edit);
				}
				while (amendNum < 1 || amendNum > 8) {
					cout << "Invalid input. Please enter a valid return route (1-8): ";
					cin >> edit;
					if (isNum(edit)) {
						amendNum = stoi(edit);
					}
				}
			}

			//chnage return date
			else if (select == 5) {
				printYellow("~Amend Return Date~");
				cout << "Old return date: " << book[0].dateReturn << endl;
				cout << "New return date: ";
				cin >> amend1;
				//check date format
				while (!isValidDate(amend1)) {
					cout << "Please input valid New Date of Return (DD/MM/YYYY): ";
					cin >> amend1;
				}
				//check return date is later than departure date
				while (!checkDateDepartReturn(book[0].dateDepart, amend1)) {
					printRed("Return date must later than departure date!!!");
					cout << endl;
					cout << "New return date: ";
					cin >> amend1;
					while (!isValidDate(amend1)) {
						cout << "Please input valid New Date of Return (DD/MM/YYYY): ";
						cin >> amend1;
					}
				}
			}

			//change return slot
			else {
				flightTimeSlot();
				printYellow("~Amend Return Slot~");
				cout << "Old return slot: " << book[0].timeReturn << endl;
				cout << "New return slot: ";
				cin >> edit;
				//validation
				if (isNum(edit)) {
					amendNum = stoi(edit);
				}
				while (amendNum < 1 || amendNum > 4) {
					cout << "Invalid input. Please enter a valid Slot of Return (1-4): ";
					cin >> edit;
					if (isNum(edit)) {
						amendNum = stoi(edit);
					}
				}
			}
		}

		//quit edit booking function
		else if (change == 3) {
			return;
		}

		//validation
		else {
			printRed("INVALID CHOICE !!!");
			cout << endl;
		}

	} while (change != 1 && change != 2 && change != 3);

	//confirmation to change amendment
	if (change != 3) {
		do {
			cout << "Confirm Amendment? (y-yes, n-no): ";
			cin >> confirm;

			//validation
			for (int i = 0; i < confirm.length(); i++) {
				confirm[i] = tolower(confirm[i]);
			}

			//If confirm to amend, 
			if (confirm == "y") {
				//update the new passsenger and flight details into respective parameters
				if (change == 1) {
					book[select - 1].name.firstName = amend1;
					book[select - 1].name.lastName = amend2;
				}
				else if (change == 2) {
					if (select == 1) {
						for (int i = 0; i < count; i++) {
							book[i].depart = amendNum;
						}
					}

					else if (select == 2) {
						for (int i = 0; i < count; i++) {
							book[i].dateDepart = amend1;
						}
					}

					else if (select == 3) {
						for (int i = 0; i < count; i++) {
							book[i].timeDepart = amendNum;
						}
					}

					else if (select == 4) {
						for (int i = 0; i < count; i++) {
							book[i].Return = amendNum;
						}
					}

					else if (select == 5) {
						for (int i = 0; i < count; i++) {
							book[i].dateReturn = amend1;
						}
					}

					else {
						for (int i = 0; i < count; i++) {
							book[i].timeReturn = amendNum;
						}
					}
				}

				//rewrite the passenger and flight details into "XX_Booking.txt" 
				ofstream outfile(currentUser + "_Booking.txt");
				for (int i = 0; i < count; i++) {
					outfile << book[i].name.firstName << endl << book[i].name.lastName << endl << book[i].depart << endl << book[i].dateDepart << endl
						<< book[i].timeDepart << endl << book[i].Return << endl << book[i].dateReturn << endl << book[i].timeReturn;
					if (i != count - 1)
						outfile << endl;
				}
				outfile.close();
				printGreen("AMENDED SUCCESSFUL !!!");
				system("pause");
			}

			//If not confirm to amend, amendment cancelled
			else if (confirm == "n") {
				printRed("AMENDMENT CANCELLED !!!");
				system("pause");
			}

			//validation
			else {
				printRed("INVALID OPTION !!!");
			}
		} while (confirm != "y" && confirm != "n");
	}
}

//Function payment - user pay the flight tickets
void payment() {
	int priceFlight = 0;
	int paid = 0;
	int totalPay = 0;
	int payMethod = 0;
	string cont, confirm;
	int transAmt = 0;
	string cardName, cardNo, cardDate, sectyCode;
	string bank, username, password;

	//validation variables
	string payMethodStr;
	string transAmtStr;
	string changeCard;
	int cardValid = 0;

	//use for display flight details and calculation of total payment
	const string flights[] = { "KL - Penang" , "Penang - KL" , "KL - Johor" , "Johor - KL" , "KL - Singapore" , "Singapore - KL" , "KL - Bangkok" , "Bangkok - KL" };
	const int price[] = { 200 , 200 , 200 , 200 , 250 , 250 , 300 , 300 };
	const string timeslot[] = { "8:00 A.M." , "13:00 P.M." , "18:00 P.M." , "23:00 P.M." };

	//calculate total price to flight
	priceFlight = (price[flight[0].depart - 1] + price[flight[0].Return - 1]) * noTicket;

	//get payment paid before rebooking or edit booking
	ifstream infile(currentUser + "_paymentCheckIn.txt");
	infile >> paid;
	infile.close();

	//calculate total payment of flight
	totalPay = ((price[flight[0].depart - 1] + price[flight[0].Return - 1]) * noTicket) - paid;

	//Make payment
	if (totalPay > 0) {
		do {
			printPaymentPage();
			//display flight details and total payment of the flight
			printPaymentDetail(flights, price, timeslot, totalPay, paid, priceFlight);
			//ask if continue for payment
			cout << "Continue for payment? (y-yes, n-no): ";
			cin >> cont;

			//validation
			for (int i = 0; i < cont.length(); i++) {
				cont[i] = tolower(cont[i]);
			}

			//If continue for payment,
			if (cont == "y") {
				do {
					system("cls");
					//get payment method of user choose
					printPaymentPage();
					cout << endl;
					cout << "  Please choose your payment method:" << endl;
					cout << "  +-----+-----------------------------+\n";
					cout << "  | No. |      Payment Method         |\n";
					cout << "  +-----+-----------------------------+\n";
					cout << "  |  1  | Credit Card/Debit Card      |\n";
					cout << "  |  2  | Bank Transfer               |\n";
					cout << "  +-----+-----------------------------+\n";
					cout << "  Choice: ";
					cin >> payMethodStr;
					//validation
					if (isNum(payMethodStr)) {
						payMethod = stoi(payMethodStr);
					}

					//Pay with Credit Card/Debit Card
					if (payMethod == 1) {
						system("cls");
						//display flight details and total payment of the flight
						printPaymentDetail(flights, price, timeslot, totalPay, paid, priceFlight);
						cout << endl;
						printYellow("~Pay with Credit Card/Debit Card~");
						do {
							//get amount of transfer
							cout << "Transfer Amount: RM ";
							cin >> transAmtStr;
							//validation
							if (isNum(transAmtStr)) {
								transAmt = stoi(transAmtStr);
							}
							//check if transfer amount is same with total payment of flight
							if (transAmt != totalPay) {
								printRed("The Amount Transfer should SAME with total payment of flight ticket!");
								cout << endl;
							}
						} while (transAmt != totalPay);

						do {
							//get card holder name
							cout << "Card Holder Name: ";
							cin.ignore();
							getline(cin, cardName);

							//get card number
							do {
								cout << "Card Number: ";
								cin >> cardNo;
								//validation
								if (!cardNumValidation(cardNo)) {
									printRed("Invalid card number!! It should be 16 digits");
								}
							} while (!cardNumValidation(cardNo));

							do {
								//get card valid date
								cout << "Card Valid Date (MM/YYYY): ";
								cin >> cardDate;
								//check card validation
								cardValid = cardDateValidation(cardDate);
								//If wrong date format, get the date again
								if (cardValid == 1) {
									printRed("Invalid Date Format!!!");
									cout << endl;
								}
							} while (cardValid == 1);

							//If card expired, ask user if want to change card
							if (cardValid == 2) {
								printRed("Card Expired!!!");
								do {
									cout << "Do you want to change credit card/cebit card? (y-yes, n-no): ";
									cin >> changeCard;
									if (changeCard != "y" && changeCard != "n")
										printRed("Invalid input! Please try again!");
									cout << endl;
								} while (changeCard != "y" && changeCard != "n");
							}

						} while (changeCard == "y" && cardValid != 3);

						if (changeCard == "n") {
							printRed("Payment Cancelled!!!");
							system("pause");
						}

						//If card valid, get security code of the card
						if (cardValid == 3) {
							do {
								cout << "Security Code: ";
								cin >> sectyCode;
								//validation
								if (!cardSectyCode(sectyCode)) {
									printRed("Invalid security code!! It should be 3/4 digits");
								}
							} while (!cardSectyCode(sectyCode));
						}
					}

					//Pay by Bank Transfer
					else if (payMethod == 2) {
						system("cls");
						//display flight details and total payment of the flight
						printPaymentDetail(flights, price, timeslot, totalPay, paid, priceFlight);
						cout << endl;
						printYellow("~Pay by Bank Transfer~");
						do {
							//get amount of transfer
							cout << "Transfer Amount: RM ";
							cin >> transAmtStr;
							//validation
							if (isNum(transAmtStr)) {
								transAmt = stoi(transAmtStr);
							}
							//check if transfer amount is same with total payment of flight
							if (transAmt != totalPay) {
								printRed("The Amount Transfer should same with total payment of flight ticket!");
								cout << endl;
							}
						} while (transAmt != totalPay);

						//get bank to transfer, username, passwor and security code
						cout << "Bank to transfer: ";
						cin.ignore();
						getline(cin, bank);
						cout << "Username: ";
						cin >> username;
						cout << "Password: ";
						cin >> password;

						do {
							cout << "Security Code: ";
							cin >> sectyCode;
							//validation
							if (!cardSectyCode(sectyCode)) {
								printRed("Invalid security code!! It should be 3/4 digits");
							}
						} while (!cardSectyCode(sectyCode));
					}

					//validation
					else {
						printRed("Invalid Input! Please choose valid payment method (1/2).");
						system("pause");
						cout << endl;
					}

				} while (payMethod < 1 || payMethod > 2);

				//confirmation transaction
				if (changeCard != "n") {
					do {
						cout << "Confirm Transaction? (y-yes, n-no): ";
						cin >> confirm;

						//validation
						for (int i = 0; i < confirm.length(); i++) {
							confirm[i] = tolower(confirm[i]);
						}

						//If confirm transaction, store payment amount, payment status and checkin status into "XX_paymentCheckIn.txt"  
						if (confirm == "y") {
							ofstream outfile(currentUser + "_paymentCheckIn.txt");
							outfile << priceFlight << endl << "Paid" << endl;
							//write NULL as check in no yet perform
							for (int i = 0; i < noTicket * 6; i++) {
								outfile << "NULL" << endl;
							}
							outfile << "Not Check";
							outfile.close();

							printGreen("Transaction Successful!");
							cout << endl;
							system("pause");
						}

						//If not confirm transaction, transaction cancelled
						else if (confirm == "n") {
							printRed("Transaction Cancelled!");
							cout << endl;
							system("pause");
						}

						//validation
						else {
							printRed("Invalid Input!");
							cout << endl;
						}

					} while (confirm != "y" && confirm != "n");
				}
			}

			//If no continue for payment, payment cancelled and quit payment function
			else if (cont == "n") {
				printRed("Payment Cancelled!");
				system("pause");
				cout << endl;
			}

			//validation
			else {
				printRed("Invalid Input! Please input valid confirmation.");
				cout << endl;
				system("pause");
				system("cls");
			}

		} while (cont != "y" && cont != "n");
	}

	//If total payment of flight is exceeding total price of ticket after edit booking,
	else if (totalPay < 0) {
		printPaymentPage();
		printPaymentDetail(flights, price, timeslot, totalPay, paid, priceFlight);
		printGreen("Extra payment will be refund to your bank in 3 working days!");
		//update the "xxx_paymentCheckIn.txt"
		ofstream outfile(currentUser + "_paymentCheckIn.txt");
		outfile << priceFlight << endl << "Paid" << endl;
		//write NULL as check in no yet perform
		for (int i = 0; i < noTicket * 6; i++) {
			outfile << "NULL" << endl;
		}
		outfile << "Not Check";
		outfile.close();
		system("pause");
	}

	//If no payment needed after edit booking,
	else {
		printPaymentPage();
		printPaymentDetail(flights, price, timeslot, totalPay, paid, priceFlight);
		printGreen("No payment balance, please proceed to check in");
		//update the "xxx_paymentCheckIn.txt"
		ofstream outfile(currentUser + "_paymentCheckIn.txt");
		outfile << priceFlight << endl << "Paid" << endl;
		for (int i = 0; i < noTicket * 6; i++) {
			outfile << "NULL" << endl;
		}
		outfile << "Not Check";
		outfile.close();
		system("pause");
	}
}

//Function readPaymentCheckIn - read all items listed in "XX_paymentCheckIn.txt" 
//								and store the data into the respective parameters
void readPaymentCheckIn(PayCheck& payCheck) {
	ifstream infile(currentUser + "_paymentCheckIn.txt");
	if (infile.is_open()) {
		//read payment amount, payment status, first name and last name of passenger, 
		//passport number, first name and last name of contact person, and mobile number of contact person
		while (!infile.eof()) {
			infile >> payCheck.amountPay;
			infile.ignore();
			getline(infile, payCheck.statusPay);
			for (int i = 0; i < noTicket; i++) {
				getline(infile, payCheck.passenger[i].name.firstName);
				getline(infile, payCheck.passenger[i].name.lastName);
				getline(infile, payCheck.passenger[i].passport);
				getline(infile, payCheck.passenger[i].contactName.firstName);
				getline(infile, payCheck.passenger[i].contactName.lastName);
				getline(infile, payCheck.passenger[i].contactNo);
			}
			getline(infile, payCheck.statusCheckin);
		}
	}
	else
		printRed("Unable to open the file!");

	infile.close();
}

//Function checkIn - User check-in by key in the passenger(s) details
void checkIn(PayCheck& payCheck) {
	string confirm, same;
	int noPassenger = 0;

	//validation variables
	bool found = false;
	string noPassengerStr;

	printCheckinPage();
	//display payment status
	cout << "Payment Status: " << payCheck.statusPay << endl;
	cout << endl;

	//get each passenger details
	for (int i = 0; i < noTicket; i++) {
		//display passenger details that checked-in
		if (i != 0) {
			system("cls");
			printCheckinPage();
			cout << "============================================================================================================" << endl;
			cout << "|                                Check-In Summary for Previous Passengers                                  |" << endl;
			cout << "============================================================================================================" << endl;
			cout << "|" << left << setw(12) << "Passenger" << setw(25) << "Full Name" << setw(18) << "Passport No." << setw(25) << "Contact Person" << "Contact Person Phone No." << "  |" << endl;
			cout << "------------------------------------------------------------------------------------------------------------" << endl;
			for (int j = 0; j < i; j++) {
				cout << "|" << left << setw(12) << j + 1 << setw(25) << (payCheck.passenger[j].name.firstName + " " + payCheck.passenger[j].name.lastName) << setw(18) << payCheck.passenger[j].passport << setw(25) << (payCheck.passenger[j].contactName.firstName + " " + payCheck.passenger[j].contactName.lastName) << setw(26) << payCheck.passenger[j].contactNo << "|" << endl;
			}
			cout << "============================================================================================================" << endl << endl;
		}

		printYellow("~Proceed to Check-In~");
		cout << "+--------------------------+" << endl;
		cout << "|       Passenger " << i + 1 << "        |" << endl;
		cout << "+--------------------------+" << endl;

		found = false; //reset
		while (!found) {
			//get first name and last name of passenger
			cout << "First Name: ";
			getline(cin, payCheck.passenger[i].name.firstName);
			//format the name to capital letter for the first letter of each word
			formatName(payCheck.passenger[i].name.firstName);
			cout << "Last Name: ";
			getline(cin, payCheck.passenger[i].name.lastName);
			//format the name to capital letter for the first letter of each word
			formatName(payCheck.passenger[i].name.lastName);

			//check if name of check in passgenger is same as name of booking passenger
			if (flight[i].name.firstName == payCheck.passenger[i].name.firstName && flight[i].name.lastName == payCheck.passenger[i].name.lastName)
				found = true;

			//if name of check in passgenger is not same as name of booking passenger,
			//display error message
			if (!found) {
				printRed("The name of check-in passenger must same as booking passenger!");
				cout << endl;
			}
		}

		//get passport number of passenger
		cout << "Passport Number: ";
		cin >> payCheck.passenger[i].passport;
		//check format of passport number
		while (!passportValidation(payCheck.passenger[i].passport)) {
			printRed("Invalid Passport Number! It should be a letter (A/H/K) followed by an eight-digit number!");
			cout << endl;
			cout << "Passport Number: ";
			cin >> payCheck.passenger[i].passport;
		}

		//continue to get first passenger details
		if (i == 0) {
			//get contact person first name and last name
			cout << "Contact Person First Name: ";
			cin.ignore();
			getline(cin, payCheck.passenger[i].contactName.firstName);
			//format the name to capital letter for the first letter of each word
			formatName(payCheck.passenger[i].contactName.firstName);
			cout << "Contact Person Last Name: ";
			getline(cin, payCheck.passenger[i].contactName.lastName);
			//format the name to capital letter for the first letter of each word
			formatName(payCheck.passenger[i].contactName.lastName);

			//get phone number of contact person
			cout << "Contact Person Phone Number: ";
			cin >> payCheck.passenger[i].contactNo;
			//check the format of phone number
			while (!phoneValidation(payCheck.passenger[i].contactNo)) {
				printRed("Invalid Phone Number! ");
				cout << endl;
				cout << "Contact Person Phone Number: ";
				cin >> payCheck.passenger[i].contactNo;
			}
		}

		//If not the first passenger, system will ask if the contact person details is same
		else {
			do {
				cout << "Same contact person as previous passenger? (y-yes, n-no): ";
				cin >> same;

				//validation
				for (int i = 0; i < same.length(); i++) {
					same[i] = tolower(same[i]);
				}

				//If the contact person details is not same as previous passenger
				if (same == "n") {
					//get contact person first name and last name
					cout << "Contact Person First Name: ";
					cin.ignore();
					getline(cin, payCheck.passenger[i].contactName.firstName);
					//format the name to capital letter for the first letter of each word
					formatName(payCheck.passenger[i].contactName.firstName);
					cout << "Contact Person Last Name: ";
					getline(cin, payCheck.passenger[i].contactName.lastName);
					//format the name to capital letter for the first letter of each word
					formatName(payCheck.passenger[i].contactName.lastName);

					//get phone number of contact person
					cout << "Contact Person Phone Number: ";
					cin >> payCheck.passenger[i].contactNo;
					//check the format of phone number
					while (!phoneValidation(payCheck.passenger[i].contactNo)) {
						printRed("Invalid Phone Number! ");
						cout << endl;
						cout << "Contact Person Phone Number: ";
						cin >> payCheck.passenger[i].contactNo;
					}
				}

				//If the contact person details is not same as previous passenger
				else if (same == "y") {
					do {
						//get with which passenger same contact person details
						cout << "With which passenger same contact person? ";
						cin >> noPassengerStr;

						//validation
						noPassenger = 0; //reset
						if (isNum(noPassengerStr)) {
							noPassenger = stoi(noPassengerStr);
						}

						//store the selected passenger contact person details to respective parameter
						if (noPassenger > 0 && noPassenger <= i) {
							cout << "Contact Person First Name: ";
							payCheck.passenger[i].contactName.firstName = payCheck.passenger[noPassenger - 1].contactName.firstName;
							cout << payCheck.passenger[i].contactName.firstName << endl;
							cout << "Contact Person Last Name: ";
							payCheck.passenger[i].contactName.lastName = payCheck.passenger[noPassenger - 1].contactName.lastName;
							cout << payCheck.passenger[i].contactName.lastName << endl;
							cout << "Contact Person Phone Number: ";
							payCheck.passenger[i].contactNo = payCheck.passenger[noPassenger - 1].contactNo;
							cout << payCheck.passenger[i].contactNo << endl;
						}

						//validation
						else {
							cout << "Invalid Input! Please input a correct passenger number (1-" << i << ") " << endl;
							cout << endl;
						}
					} while (noPassenger <= 0 || noPassenger > i);
				}

				//validation
				else {
					printRed("Invalid Input!");
					cout << endl;
				}
			} while (same != "y" && same != "n");
		}
		cout << endl;
		cin.ignore();
	}

	do {
		system("cls");
		//display all the check-in details of passengers
		printCheckinPage();
		cout << "============================================================================================================" << endl;
		cout << "|                                            Check-In Summary                                              |" << endl;
		cout << "============================================================================================================" << endl;
		cout << "|" << left << setw(12) << "Passenger" << setw(25) << "Full Name" << setw(18) << "Passport No." << setw(25) << "Contact Person" << "Contact Person Phone No." << "  |" << endl;
		cout << "------------------------------------------------------------------------------------------------------------" << endl;
		for (int j = 0; j < noTicket; j++) {
			cout << "|" << left << setw(12) << j + 1 << setw(25) << (payCheck.passenger[j].name.firstName + " " + payCheck.passenger[j].name.lastName) << setw(18) << payCheck.passenger[j].passport << setw(25) << (payCheck.passenger[j].contactName.firstName + " " + payCheck.passenger[j].contactName.lastName) << setw(26) << payCheck.passenger[j].contactNo << "|" << endl;
		}
		cout << "============================================================================================================\n" << endl;
		//get confirmation to check-in
		cout << "Confirm check-in? (y-yes, n-no): ";
		cin >> confirm;

		//validation
		for (int i = 0; i < confirm.length(); i++) {
			confirm[i] = tolower(confirm[i]);
		}

		//if confirm to check-in, write the checkin details into "XX_paymentCheckIn.txt"
		if (confirm == "y") {
			ofstream outfile(currentUser + "_paymentCheckIn.txt");
			outfile << payCheck.amountPay << endl;
			outfile << payCheck.statusPay << endl;
			for (int i = 0; i < noTicket; i++) {
				outfile << payCheck.passenger[i].name.firstName << endl << payCheck.passenger[i].name.lastName << endl << payCheck.passenger[i].passport << endl
					<< payCheck.passenger[i].contactName.firstName << endl << payCheck.passenger[i].contactName.lastName << endl << payCheck.passenger[i].contactNo << endl;
			}
			outfile << "Checked";
			outfile.close();
			printGreen("Check-in Successful!");
		}

		//if not confirm to check-in, check-in cancelled
		else if (confirm == "n") {
			printRed("Check-in Cancelled!");
		}

		//validation
		else {
			printRed("Invalid Input!");
			cout << endl;
			system("pause");
		}
	} while (confirm != "y" && confirm != "n");
	system("pause");
}

//Function printInvoice - print invoice of the flight after payment and check in 
//						  and save the invoice into "XX_invoice.txt"
void printInvoice(Booking book[], PayCheck payCheck) {
	const string flights[] = { "KL - Penang", "Penang - KL", "KL - Johor", "Johor - KL", "KL - Singapore", "Singapore - KL", "KL - Bangkok", "Bangkok - KL" };
	const int price[] = { 200, 200, 200, 200, 250, 250, 300, 300 };
	const string timeslot[] = { "8:00 A.M.", "13:00 P.M.", "18:00 P.M.", "23:00 P.M." };

	ofstream outfile(currentUser + "_invoice.txt");

	outfile << "======================================================" << endl;
	outfile << "               JSJK AIRLINES - INVOICE" << endl;
	outfile << "======================================================" << endl;
	outfile << " Username: " << currentUser << endl;
	outfile << "------------------------------------------------------" << endl;
	outfile << "                    FLIGHT DETAILS" << endl;
	outfile << "------------------------------------------------------" << endl;
	outfile << " DEPARTURE:" << endl;
	outfile << "   Route:  " << flights[book[0].depart - 1] << endl;
	outfile << "   Date:   " << book[0].dateDepart << endl;
	outfile << "   Time:   " << timeslot[book[0].timeDepart - 1] << endl;
	outfile << "   Price:  RM" << price[book[0].depart - 1] << endl << endl;
	outfile << " RETURN:" << endl;
	outfile << "   Route:  " << flights[book[0].Return - 1] << endl;
	outfile << "   Date:   " << book[0].dateReturn << endl;
	outfile << "   Time:   " << timeslot[book[0].timeReturn - 1] << endl;
	outfile << "   Price:  RM" << price[book[0].Return - 1] << endl << endl;
	outfile << "------------------------------------------------------" << endl;
	outfile << "                PASSENGER DETAILS" << endl;
	outfile << "------------------------------------------------------" << endl;
	for (int i = 0; i < noTicket; i++) {
		outfile << " Passenger " << i + 1 << ":" << endl;
		outfile << "   Name: " << book[i].name.firstName << " " << book[i].name.lastName << endl;
		outfile << "   Passport Number: " << payCheck.passenger[i].passport << endl;
		outfile << "   Contact Person: " << payCheck.passenger[i].contactName.firstName << " " << payCheck.passenger[i].contactName.lastName << endl;
		outfile << "   Contact Person Mobile No: " << payCheck.passenger[i].contactNo << endl << endl;
	}
	outfile << "------------------------------------------------------" << endl;
	outfile << " TOTAL PAYMENT: RM " << payCheck.amountPay << endl << endl;
	outfile << "======================================================" << endl;
	outfile << "         Thank you for using JSJK Airlines!" << endl;
	outfile << "======================================================" << endl;

	outfile.close();

	printGreen("Invoice printed, please check your folder!");
	system("pause");
}

//Display flight details and total payment of the flight
void printPaymentDetail(const string flights[], const int price[], const string timeslot[], int totalPay, int paid, int priceFlight) {
	cout << "\n+---------------------+----------------------------------------+" << endl;
	cout << "| Item                | Details                                |" << endl;
	cout << "+---------------------+----------------------------------------+" << endl;
	cout << "| Total Tickets       | " << setw(38) << left << noTicket << " |" << endl;
	cout << "+---------------------+----------------------------------------+" << endl;
	cout << "| Departure Flight    |                                        |" << endl;
	cout << "| - Date              | " << setw(38) << left << flight[0].dateDepart << " |" << endl;
	cout << "| - Route             | " << setw(38) << left << flights[flight[0].depart - 1] << " |" << endl;
	cout << "| - Time              | " << setw(38) << left << timeslot[flight[0].timeDepart - 1] << " |" << endl;
	cout << "+---------------------+----------------------------------------+" << endl;
	cout << "| Return Flight       |                                        |" << endl;
	cout << "| - Date              | " << setw(38) << left << flight[0].dateReturn << " |" << endl;
	cout << "| - Route             | " << setw(38) << left << flights[flight[0].Return - 1] << " |" << endl;
	cout << "| - Time              | " << setw(38) << left << timeslot[flight[0].timeReturn - 1] << " |" << endl;
	cout << "+---------------------+----------------------------------------+" << endl;
	cout << "| Total Flight Price  | (RM " << price[flight[0].depart - 1]
		<< " + RM " << price[flight[0].Return - 1]
		<< ") * " << noTicket << " = RM " << priceFlight << setw(12 - to_string(priceFlight).length()) << " " << "|" << endl;
	cout << "| Payment Paid        | RM " << paid << setw(36 - to_string(paid).length()) << " " << "|" << endl;
	cout << "| Total Payment       | RM " << priceFlight << " - RM " << paid << " = RM " << totalPay << setw(24 - to_string(priceFlight).length() - to_string(paid).length() - to_string(totalPay).length()) << " " << "|" << endl;
	cout << "+---------------------+----------------------------------------+" << endl;
}

//Gmail validation
bool isValidEmail(string email) {
	int atPos = email.find('@');
	string domain = "@gmail.com";

	if (atPos == string::npos)
		return false; // no @
	if (email.substr(atPos) != domain)
		return false; // must end with @gmail.com
	if (atPos == 0)
		return false; // nothing before @

	return true;
}

//Check Duplicate username
bool isUsernameTaken(string username) {
	ifstream inFile("user.txt");
	if (!inFile.is_open())
		return false; //file not exist

	User tempUser;
	while (getline(inFile, tempUser.name.firstName)) {
		getline(inFile, tempUser.name.lastName);
		getline(inFile, tempUser.mobileNo);
		getline(inFile, tempUser.email);
		getline(inFile, tempUser.username);
		getline(inFile, tempUser.password);

		if (tempUser.username == username) {
			inFile.close();
			return true; //username found
		}
	}
	inFile.close();
	return false; //username not found
}

//Function to check password requirements
bool isValidPassword(string password) {
	bool hasUpper = false, hasDigit = false, hasSymbol = false;

	//check minimum length
	if (password.length() < 8)
		return false;

	for (int i = 0; i < password.length(); i++) {
		char c = password[i];
		//check uppercase
		if (c >= 'A' && c <= 'Z')
			hasUpper = true;
		//check digit
		if (c >= '0' && c <= '9')
			hasDigit = true;
		//check symbol
		if ((c >= 33 && c <= 47) || (c >= 58 && c <= 64))
			hasSymbol = true;
	}

	return hasUpper && hasDigit && hasSymbol;
}

//Check if the year is leap year
bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

//Check date format and validation
bool isValidDate(string date) {
	int daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
	int day;
	int month;
	int year;

	//check all character is digit or slash
	for (int i = 0; i < date.length(); i++) {
		if (int(date[i]) < 47 || int(date[i] > 57))
			return false;
	}

	//check length
	if (date.length() != 10)
		return false;

	//check slash position
	if (date[2] != '/' || date[5] != '/')
		return false;

	//extract day, month, year and convert into integer
	day = stoi(date.substr(0, 2));
	month = stoi(date.substr(3, 2));
	year = stoi(date.substr(6, 4));

	//check if is current or future year
	if (year < 2025)
		return false;

	//check month validity
	if (month < 1 || month > 12)
		return false;

	//check if is currrent month
	if (year == 2025 && month < 5)
		return false;

	//ajust days of February if is leap year
	if (month == 2 && isLeapYear(year))
		daysInMonth[1] = 29;

	//check day validity
	if (day < 1 || day > daysInMonth[month - 1])
		return false;

	return true;
}

//check validity of departure date and return date 
bool checkDateDepartReturn(string departD, string returnD) {
	//extract day, month, year of departure and return date 
	//and convert into integer
	int dayDepart = stoi(departD.substr(0, 2));
	int monthDepart = stoi(departD.substr(3, 2));
	int yearDepart = stoi(departD.substr(6, 4));
	int dayReturn = stoi(returnD.substr(0, 2));
	int monthReturn = stoi(returnD.substr(3, 2));
	int yearReturn = stoi(returnD.substr(6, 4));

	//compare year, month and day in order
	if (yearReturn < yearDepart) {
		return false;
	}

	if (yearReturn == yearDepart && monthReturn < monthDepart) {
		return false;
	}

	if (yearReturn == yearDepart && monthReturn == monthDepart && dayReturn <= dayDepart) {
		return false;
	}

	return true;
}

//check card number validation
bool cardNumValidation(string cardNum) {
	//check all character are digits
	for (int i = 0; i < cardNum.length(); i++) {
		if (cardNum[i] < 48 || cardNum[i]>57)
			return false;
	}

	//check the length is 16
	if (cardNum.length() != 16)
		return false;

	return true;
}

//check validity of debit and credit card and format
int cardDateValidation(string date) {
	int month;
	int year;

	//check all character are digits or slash
	for (int i = 0; i < date.length(); i++) {
		if (int(date[i]) < 47 || int(date[i] > 57))
			return 1;
	}

	//check slash position
	if (date.length() != 7 || date[2] != '/')
		return 1;

	//extract month and year and convert into integer
	month = stoi(date.substr(0, 2));
	year = stoi(date.substr(3, 4));

	//check valid year
	if (year < 0)
		return 1;

	//check valid month
	if (month < 1 || month > 12)
		return 1;

	//check expired year
	if (year < 2025)
		return 2;

	//check expired month
	if (year == 2025 && month < 5)
		return 2;

	//valid format and not expired
	return 3;
}

//check card security code validity
bool cardSectyCode(string code) {
	//check all character are digits
	for (int i = 0; i < code.length(); i++) {
		if (code[i] < 48 || code[i]>57)
			return false;
	}

	//check the length is 3/4
	if (code.length() != 3 && code.length() != 4)
		return false;

	return true;
}

//check phone number validation
bool phoneValidation(string phone) {
	//check all character are digits
	for (int i = 0; i < phone.length(); i++) {
		if (int(phone[i]) < 48 || int(phone[i] > 57))
			return false;
	}

	//must start with 01
	if (phone[0] != '0' && phone[1] != '1') {
		return false;
	}

	//check lenth of phone number
	//if 011/015, length must be 11
	if (phone[2] == '1' || phone[2] == '5') {
		if (phone.length() != 11)
			return false;
	}
	//if other than 011/015, length must be 10
	else {
		if (phone.length() != 10)
			return false;
	}

	return true; //valid phone number
}

//check validity of passport number
bool passportValidation(string passport) {
	//check length of passport number
	if (passport.length() != 9) {
		return false;
	}

	//check starting letter
	if (passport[0] != 'A' && passport[0] != 'H' && passport[0] != 'K') {
		return false;
	}

	//check remaining character are digits
	for (int i = 1; i < passport.length(); i++) {
		if (int(passport[i]) < 48 || int(passport[i] > 57))
			return false;
	}
	return true;
}

//check if string only contain digits
bool isNum(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}

//format name - first letter of each word uppercase
void formatName(string& name) {
	//capitalize first character
	name[0] = toupper(name[0]);

	for (size_t i = 1; i < name.length(); i++) {
		////capitalize character after space
		if (name[i - 1] == ' ') {
			name[i] = toupper(name[i]);
		}
		//lowercase other character
		else {
			name[i] = tolower(name[i]);
		}
	}
}

//Red text
void printRed(const string& message) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.dwFontSize.Y = 22; // Font height
	wcscpy_s(cfi.FaceName, L"Consolas"); // Font type
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	SetConsoleTextAttribute(hConsole, 12);

	cout << message << endl;

	SetConsoleTextAttribute(hConsole, 7);
}

//Green text
void printGreen(const string& message) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.dwFontSize.Y = 22; // Font height
	wcscpy_s(cfi.FaceName, L"Consolas"); // Font type
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	SetConsoleTextAttribute(hConsole, 10);

	cout << message << endl;

	SetConsoleTextAttribute(hConsole, 7);
}

//Yellow text
void printYellow(const string& message) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.dwFontSize.Y = 22;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	SetConsoleTextAttribute(hConsole, 14);

	cout << message << endl;

	SetConsoleTextAttribute(hConsole, 7);
}

//Blue text
void printBlue(const string& message) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.dwFontSize.Y = 22;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	SetConsoleTextAttribute(hConsole, 3);

	cout << message << endl;

	SetConsoleTextAttribute(hConsole, 7);
}

//Pink text
void printPink(const string& message) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
	cfi.dwFontSize.Y = 22;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

	SetConsoleTextAttribute(hConsole, 12);

	cout << message << endl;

	SetConsoleTextAttribute(hConsole, 7);
}

//Display flight routes and prices
void flightSlot() {
	cout << "  ============================================\n";
	cout << "  |         FLIGHT SCHEDULE & PRICES         |\n";
	cout << "  +-----+-----------------------+------------+\n";
	cout << "  | No. |       Route           |   Price    |\n";
	cout << "  +-----+-----------------------+------------+\n";
	cout << "  |  1  | KL - Penang           |   RM200    |\n";
	cout << "  |  2  | Penang - KL           |   RM200    |\n";
	cout << "  |  3  | KL - Johor            |   RM200    |\n";
	cout << "  |  4  | Johor - KL            |   RM200    |\n";
	cout << "  |  5  | KL - Singapore        |   RM250    |\n";
	cout << "  |  6  | Singapore - KL        |   RM250    |\n";
	cout << "  |  7  | KL - Bangkok          |   RM300    |\n";
	cout << "  |  8  | Bangkok - KL          |   RM300    |\n";
	cout << "  +-----+-----------------------+------------+\n\n";
}

//Display available time slots for flight
void flightTimeSlot() {
	cout << "  ==================================\n";
	cout << "  |   DEPARTURE/RETURN TIME SLOT   |\n";
	cout << "  +-----+--------------------------+\n";
	cout << "  | No. |          Time            |\n";
	cout << "  +-----+--------------------------+\n";
	cout << "  |  1  | 8:00 A.M.                |\n";
	cout << "  |  2  | 13:00 P.M.               |\n";
	cout << "  |  3  | 18:00 P.M.               |\n";
	cout << "  |  4  | 23:00 P.M.               |\n";
	cout << "  +-----+--------------------------+\n";
}

//Display main header for JSJK Airlines System
void printHeader() {
	printBlue(R"(
                                         _   ____        _   _  __
                                        | | / ___|      | | | |/ /
                                     _  | | \___ \   _  | | | ' / 
                                    | |_| |  ___) | | |_| | | . \ 
                                     \___/  |____/   \___/  |_|\_\
    _      ___   ____    _       ___   _   _   _____   ____      ____   __   __  ____    _____   _____   __  __ 
   / \    |_ _| |  _ \  | |     |_ _| | \ | | | ____| / ___|    / ___|  \ \ / / / ___|  |_   _| | ____| |  \/  |
  / _ \    | |  | |_) | | |      | |  |  \| | |  _|   \___ \    \___ \   \ V /  \___ \    | |   |  _|   | |\/| |
 / ___ \   | |  |  _ <  | |___   | |  | |\  | | |___   ___) |    ___) |   | |    ___) |   | |   | |___  | |  | |
/_/   \_\ |___| |_| \_\ |_____| |___| |_| \_| |_____| |____/    |____/    |_|   |____/    |_|   |_____| |_|  |_|)");

}

//Display login page header
void printLoginPage() {
	printYellow(R"(
 _                       _         
| |       ___     __ _  (_)  _ __  
| |      / _ \   / _` | | | | '_ \ 
| |___  | (_) | | (_| | | | | | | |
|_____|  \___/   \__, | |_| |_| |_|
                 |___/             
)");
}

//Display registration header
void printRegistrationPage() {
	printYellow(R"(
 ____                   _         _                    _     _                 
|  _ \    ___    __ _  (_)  ___  | |_   _ __    __ _  | |_  (_)   ___    _ __  
| |_) |  / _ \  / _` | | | / __| | __| | '__|  / _` | | __| | |  / _ \  | '_ \ 
|  _ <  |  __/ | (_| | | | \__ \ | |_  | |    | (_| | | |_  | | | (_) | | | | |
|_| \_\  \___|  \__, | |_| |___/  \__| |_|     \__,_|  \__| |_|  \___/  |_| |_|
                |___/                                                          
)");
}

//Display booking header
void printBookingPage() {
	printYellow(R"(
 ____                    _      _                 
| __ )    ___     ___   | | __ (_)  _ __     __ _ 
|  _ \   / _ \   / _ \  | |/ / | | | '_ \   / _` |
| |_) | | (_) | | (_) | |   <  | | | | | | | (_| |
|____/   \___/   \___/  |_|\_\ |_| |_| |_|  \__, |
                                            |___/ 
)");
}

//Display edit booking header
void printEditBookingPage() {
	printYellow(R"(
 _____       _   _   _       ____                    _      _                 
| ____|   __| | (_) | |_    | __ )    ___     ___   | | __ (_)  _ __     __ _ 
|  _|    / _` | | | | __|   |  _ \   / _ \   / _ \  | |/ / | | | '_ \   / _` |
| |___  | (_| | | | | |_    | |_) | | (_) | | (_) | |   <  | | | | | | | (_| |
|_____|  \__,_| |_|  \__|   |____/   \___/   \___/  |_|\_\ |_| |_| |_|  \__, |
                                                                        |___/ 
)");
}

//Display booking details with passenger details
void printBookingDetail(const Booking book[]) {
	const string flights[] = { "KL - Penang" , "Penang - KL" , "KL - Johor" , "Johor - KL" , "KL - Singapore" , "Singapore - KL" , "KL - Bangkok" , "Bangkok - KL" };
	const string timeslot[] = { "8:00 A.M." , "13:00 P.M." , "18:00 P.M." , "23:00 P.M." };

	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Item              | Booking Details                        |" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Passenger Name    |                                        |" << endl;
	for (int i = 0; i < noTicket; i++) {
		cout << "| - " << setw(15) << left << ("Passenger " + to_string(i + 1))
			<< " | " << setw(39) << left << (book[i].name.firstName + " " + book[i].name.lastName) << "|" << endl;
	}
	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Departure Flight  |                                        |" << endl;
	cout << "| - Route           | " << setw(39) << left << flights[book[0].depart - 1] << "|" << endl;
	cout << "| - Date            | " << setw(39) << left << book[0].dateDepart << "|" << endl;
	cout << "| - Time            | " << setw(39) << left << timeslot[book[0].timeDepart - 1] << "|" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Return Flight     |                                        |" << endl;
	cout << "| - Route           | " << setw(39) << left << flights[book[0].Return - 1] << "|" << endl;
	cout << "| - Date            | " << setw(39) << left << book[0].dateReturn << "|" << endl;
	cout << "| - Time            | " << setw(39) << left << timeslot[book[0].timeReturn - 1] << "|" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;
}

//Display booking details without passenger details
void printBookingFlightDetail(const Booking book[]) {
	const string flights[] = { "KL - Penang" , "Penang - KL" , "KL - Johor" , "Johor - KL" , "KL - Singapore" , "Singapore - KL" , "KL - Bangkok" , "Bangkok - KL" };
	const string timeslot[] = { "8:00 A.M." , "13:00 P.M." , "18:00 P.M." , "23:00 P.M." };

	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Item              | Details                                |" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Departure Flight  |                                        |" << endl;
	cout << "| - Route           | " << setw(39) << left << flights[book[0].depart - 1] << "|" << endl;
	cout << "| - Date            | " << setw(39) << left << book[0].dateDepart << "|" << endl;
	cout << "| - Time            | " << setw(39) << left << timeslot[book[0].timeDepart - 1] << "|" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;
	cout << "| Return Flight     |                                        |" << endl;
	cout << "| - Route           | " << setw(39) << left << flights[book[0].Return - 1] << "|" << endl;
	cout << "| - Date            | " << setw(39) << left << book[0].dateReturn << "|" << endl;
	cout << "| - Time            | " << setw(39) << left << timeslot[book[0].timeReturn - 1] << "|" << endl;
	cout << "+-------------------+----------------------------------------+" << endl;
}

//Display payment header
void printPaymentPage() {
	printYellow(R"(
   ____                                               _   
  |  _ \    __ _   _   _   _ __ ___     ___   _ __   | |_ 
  | |_) |  / _` | | | | | | '_ ` _ \   / _ \ | '_ \  | __|
  |  __/  | (_| | | |_| | | | | | | | |  __/ | | | | | |_ 
  |_|      \__,_|  \__, | |_| |_| |_|  \___| |_| |_|  \__|
                   |___/                                  )");
}

//Display check  in header
void printCheckinPage() {
	printYellow(R"(
     ____   _                     _        ___         
    / ___| | |__     ___    ___  | | __   |_ _|  _ __  
   | |     | '_ \   / _ \  / __| | |/ /    | |  | '_ \ 
   | |___  | | | | |  __/ | (__  |   <     | |  | | | |
    \____| |_| |_|  \___|  \___| |_|\_\   |___| |_| |_|
)");
}

//Display quit/exit page
void printQuitPage() {
	printYellow(R"(
                     _____   _   _      _      _   _   _  __   __   __   ___    _   _ 
                    |_   _| | | | |    / \    | \ | | | |/ /   \ \ / /  / _ \  | | | |
                      | |   | |_| |   / _ \   |  \| | | ' /     \ V /  | | | | | | | |
                      | |   |  _  |  / ___ \  | |\  | | . \      | |   | |_| | | |_| |
                      |_|   |_| |_| /_/   \_\ |_| \_| |_|\_\     |_|    \___/   \___/ )");
	printBlue(R"(
 _____    ___    ____      _____   _      __   __  ___   _   _    ____    __        __  ___   _____   _   _ 
|  ___|  / _ \  |  _ \    |  ___| | |     \ \ / / |_ _| | \ | |  / ___|   \ \      / / |_ _| |_   _| | | | |
| |_    | | | | | |_) |   | |_    | |      \ V /   | |  |  \| | | |  _     \ \ /\ / /   | |    | |   | |_| |
|  _|   | |_| | |  _ <    |  _|   | |___    | |    | |  | |\  | | |_| |     \ V  V /    | |    | |   |  _  |
|_|      \___/  |_| \_\   |_|     |_____|   |_|   |___| |_| \_|  \____|      \_/\_/    |___|   |_|   |_| |_|)");
	printPink(R"(
            _   ____        _   _  __       _      ___   ____    _       ___   _   _   _____   ____  
           | | / ___|      | | | |/ /      / \    |_ _| |  _ \  | |     |_ _| | \ | | | ____| / ___| 
        _  | | \___ \   _  | | | ' /      / _ \    | |  | |_) | | |      | |  |  \| | |  _|   \___ \ 
       | |_| |  ___) | | |_| | | . \     / ___ \   | |  |  _ <  | |___   | |  | |\  | | |___   ___) |
        \___/  |____/   \___/  |_|\_\   /_/   \_\ |___| |_| \_\ |_____| |___| |_| \_| |_____| |____/ )");
	printBlue(R"(
      .@@@=                                                                                      =@@@.                
   #@@     =@@@=                                                                            =@@@=     @@#             
    -@@.        =@@@=          @@%- .@=                              =@. -%@@          =@@@=        .@@-              
       .@@:          -%@@=  =@*      *@                              @*      *@=  =@@%-          :@@.                 
           %@-            @@:        @:                              :@        :@@            -@%                     
              #@=        @+         @*                                *@         +@        =@#                        
                 #@+   %@         .@:                                  :@.         @%   +@#                           
                    #@@.         @@                                      @@         .@@#                              
                    @*         @*:@                                      @:*@         *@                              
                  =@        -@=   -@                                    @-   =@-        @=                            
                 @*       +@+      #*                                  *#      +@+       *@                           
        ---:...+@       @%  @:      @.                                .@      :@  %@       @+...:---                  
     .@%      @*     -@*     @=      @                                @      =@     *@-     *@      %@.               
       =@@*..@-    %@         ##     #%                              %#     ##         @%    -@..*@@=                 
           :@   *@%@           #@     %*                            *%     @#           @%@*   @:                     
           @%#@@   @            *@     @                            @     @*            @   @@#%@                     
               @+  @             :@    -@                          @-    @:             @  +@                         
                @  @               @:   #+                        +#   :@               @  @                          
                -@@                 @* -@                          @- *@                 @@-                          
                 -                   @@@                            @@@                   -                           
                                      *                              *                                                             
)");

}
