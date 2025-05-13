#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<cstring>
#include <sstream>


#define YELLOW "\033[1;33m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define RESET "\033[0m"
using namespace std;

class Movie {
	string timings;
	string genre;
	string movieName;
	double ticketPrice;

public:

	Movie() : timings(""), genre(""), movieName(""), ticketPrice(0.0) {}

	Movie(string movies, string genre, string timings, double ticketPrice) : movieName(movies), genre(genre), timings(timings), ticketPrice(ticketPrice) {}

	void displayData() const {
		cout << "Movie Name: " << movieName << ", Genre: " << genre << ", Show Timings: "
			<< timings << ", Price of the Ticket: $" << ticketPrice << endl;
	}

	double getPrice() {
		return ticketPrice;
	}

	string getmovie() {
		return movieName;
	}

	string getgenre() {
		return genre;

	}

	string getTimings() {
		return timings;

	}

};

class Theater {
	string** seatingPlan;
	int row = 10;
	int col = 10;

public:
	vector<Movie*> allmovies;


	Theater() {
		seatingPlan = new string * [row];
		for (int i = 0; i < row; i++) {
			seatingPlan[i] = new string[col];
			for (int j = 0; j < col; j++) {
				char rowname = 'A' + i;
				seatingPlan[i][j] = rowname + to_string(j + 1);

			}
		}
	}

	~Theater() {
		int i = 0;
		while (i < row) {
			delete[] seatingPlan[i];
			i++;
		}
		delete[] seatingPlan;
	}

	void displaySeats() {
		cout << "These are the Available Seats: " << endl;
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				cout << seatingPlan[i][j] << "  ";
			}
			cout << endl;
		}
	}

	void seatStorage(string& filename) {
		ofstream outFile(filename);
		if (!outFile) {
			cout << "The file could not be opened";
		}
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				outFile << seatingPlan[i][j] << "  ";

			}
			outFile << endl;
		}
		outFile.close();
		cout << "The seats have been stored." << endl;
	}

	void seatLoading(string& filename) {
		ifstream inFile(filename);
		if (!inFile) {
			cout << "The file could not have been opened." << endl;

		}
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				inFile >> seatingPlan[i][j];
			}

		}
		inFile.close();
		cout << "The seats have been loaded" << endl;
	}

	bool isBooked(int r, int c) {
		if (r < row && c < col && seatingPlan[r][c] != "X") {
			seatingPlan[r][c] = "X";
			return true;
		}
		cout << "The seat you entered is not available. " << endl;
		return false;
	}

	void addMovie(Movie* movie) {
		allmovies.push_back(movie);
		cout << "Movie has been added" << endl;
	}

	void displaymovie() {
		cout << "Available Movies: " << endl;
		for (int i = 0; i < allmovies.size(); i++) {
			cout << i + 1 << ". ";
			allmovies[i]->displayData();
		}
	}

	Movie* getMovie(int index) {
		if (index >= 0 && index < allmovies.size()) {
			return allmovies[index];
		}
		else {
			return nullptr;
		}
	}

};

class User {
	Theater* selectedTheater;
	Movie* selectedMovie;
	double currentbalance;

public:
	User() : selectedMovie(nullptr), selectedTheater(nullptr), currentbalance(2000) {}

	void theaterSelection(int theaterIndex, vector<Theater*>& theaters) {

		if (theaterIndex > 0 && theaterIndex <= theaters.size()) {
			selectedTheater = theaters[theaterIndex - 1];
			cout << "Theater " << theaterIndex << " has been selected." << endl;
		}
		else {
			selectedTheater = nullptr;
			cout << "An error has occurred." << endl;
		}
	}

	void movieSelection(int movieIndex, vector<Theater*>& theaters) {
		if (!selectedTheater) {
			cout << "You have not selected a theater. Try again." << endl;
			return;
		}
		if (selectedTheater) {
			selectedMovie = selectedTheater->getMovie(movieIndex - 1);
			selectedMovie->displayData();
		}
		else {
			cout << "Error: Theater not found." << endl;
		}
	}

	void booking(int row, int col) {
		if (!selectedTheater || !selectedMovie) {
			cout << "You need to select a theater and a movie: " << endl;
			return;
		}
		if (currentbalance >= selectedMovie->getPrice()) {
			if (selectedTheater->isBooked(row, col)) {
				cout << "seat has been booked " << selectedMovie->getmovie() << "." << endl;
				paymentmethod(selectedMovie->getPrice());
			}
		}
		else {
			cout << "Insufficient funds. Please recharge your account." << endl;

		}

	}

	void displayselection() {
		if (selectedMovie) {
			cout << "The movie you selected is" << selectedMovie->getmovie() << endl;

		}
		if (selectedTheater) {
			cout << "Available seats in this theater: " << endl;
			selectedTheater->displaySeats();
		}
	}
	void paymentmethod(double bill) {
		if (currentbalance >= bill) {
			currentbalance -= bill;
			cout << "Your payment of " << bill << " has gone through!" << endl;
			cout << "Your remaining balance is: " << currentbalance << endl;
		}
		else {
			cout << "Insufficient funds." << endl;

		}
	}
	void fillBalance(double fill) {
		currentbalance += fill;
		cout << "You have succesfully recharged your account. Your balance now is: " << currentbalance << endl;
	}
	double getBalance() {
		return currentbalance;
	}

};

class Admin : public  Movie, public Theater {
	vector<Theater*>theaters;

public:
	void addtheater(Theater* theater) {
		theaters.push_back(theater);
		cout << "The theater has been added" << endl;
		ofstream theaterFile("theaters.txt", ios::app);
		if (theaterFile.is_open()) {
			theaterFile << "Theater " << theaters.size() << "\n";
			theaterFile.close();
		}
		else {
			cout << "Failed to update theaters.txt." << endl;
		}
	}


	vector<Theater*> getTheaters() {
		return theaters;
	}

	void removetheater(int index) {
		if (index >= 0 && index < theaters.size()) {
			delete theaters[index];
			theaters.erase(theaters.begin() + index);
			cout << "The theater has been removed." << endl;
		}
		else {
			cout << "Theater not removed, retry.";
		}
	}

	void displayTheaters() {
		for (int i = 0; i < theaters.size(); i++) {
			cout << "Theater " << i + 1 << ":" << endl;
			theaters[i]->displaySeats();
		}
	}

	bool CheckBooking(User* user) {
		if (user) {
			user->displayselection();
			return true;
		}
		cout << "Invalid user." << endl;
		return false;
	}

	void addMovie(Movie* movie, int theaterIndex) {
		if (theaterIndex >= 0 && theaterIndex < theaters.size()) {
			theaters[theaterIndex]->allmovies.push_back(movie);
			cout << "Movie has been added" << endl;


			ofstream movieFile("movies.txt", ios::app);
			if (movieFile.is_open()) {
				movieFile << "Theater " << theaterIndex + 1 << ":\n";
				movieFile << "Title: " << movie->getmovie()
					<< ", Genre: " << movie->getgenre()
					<< ", Timing: " << movie->getTimings()
					<< ", Price: " << movie->getPrice() << "\n";
				movieFile.close();
			}
			else {
				cout << "Failed to update movies.txt." << endl;
			}
		}
		else {
			cout << "Invalid theater index. Movie not added." << endl;
		}
	}

	void initializeTheatersAndMovies() {
		for (int i = 0; i < 5; ++i) {
			Theater* theater = new Theater();
			addtheater(theater);
		}

		vector<vector<Movie*>> movies = {
			{new Movie("Whiplash", "Musical", "12:00", 5000),
			 new Movie("Smile 2", "horror", "2:00", 1500),
			 new Movie("Terrifier 4", "horror", "3:00", 2500),
			 new Movie("The Substance", "adventure", "4:00", 3000),
			 new Movie("2001 A space odyssey", "sci-fi", "5:00", 700)},

			{new Movie("Conclave", "mystery", "11:00", 2001),
			 new Movie("Strange Darling", "thriller", "1:00", 1500),
			 new Movie("Caddo lake", "adventure", "3:30", 1100),
			 new Movie("Lalaland", "romantic comedy", "5:00", 3000),
			 new Movie("Inglorius Basterds", "criminal", "6:00", 1150)},

			{new Movie("It ends with us", "romance", "10:30", 4000),
			 new Movie("Longlegs", "horror", "12:30", 3500),
			 new Movie("Ready or Not?", "action", "2:15", 1300),
			 new Movie("The killing of a sacred Deer", "mystery", "4:00", 500),
			 new Movie("Hereditary", "horror", "6:15", 900)},

			{new Movie("Barbarian", "thriller", "12:00", 1100),
			 new Movie("Twisters", "adventure", "1:30", 1700),
			 new Movie("Batman", "comic", "3:00", 1200),
			 new Movie("Spiderman: Into the spiderverse", "comic", "4:45", 950),
			 new Movie("Avengers: Endgame", "comic", "6:00", 3700)},

			{new Movie("Poor things", "thriller", "11:00", 1800),
			 new Movie("Top gun: Maverick", "adventure", "1:15", 2200),
			 new Movie("The Incredibles", "animated", "3:30", 2700),
			 new Movie("Frozen 2", "animated", "5:00", 3100),
			 new Movie("Conjuring 3", "Horror", "7:00", 2400)}
		};

		for (int i = 0; i < 5; ++i) {
			for (Movie* movie : movies[i]) {
				addMovie(movie, i);
			}
		}
	}

	bool timingValidation(string time) {
		if (time.length() != 5) {
			cout << "Incorrect timing entered. Use the 24 hour format 00:00." << endl;
			return false;
		}
		if (time[2] != ':') {
			cout << "Incorrect timing entered. Use the 24 hour format 00:00." << endl;
			return false;
		}
		for (int i = 0; i < 2; i++) {
			if (!isdigit(time[i])) {
				cout << "Incorrect timing entered. Use the 24 hour format 00:00." << endl;
				return false;
			}
		}
		for (int i = 3; i < 5; i++) {
			if (!isdigit(time[i])) {
				cout << "Incorrect timing entered. Use the 24 hour format 00:00." << endl;
				return false;
			}
		}
		int hours = (time[0] - '0') * 10 + (time[1] - '0');
		int minutes = (time[3] - '0') * 10 + (time[4] - '0');
		if (hours < 0 || hours>23 || minutes < 0 || minutes>59) {
			cout << "Incorrect timing entered. Use the 24 hour format 00:00." << endl;
			return false;
		}
		return true;
	}


	void saveMoviesAndTheatersToFile() {

		ofstream theaterFile("theaters.txt");
		if (theaterFile.is_open()) {
			for (int i = 0; i < theaters.size(); i++) {
				theaterFile << "Theater " << i + 1 << "\n";
			}
			theaterFile.close();
			cout << "theaters have been saved." << endl;
		}
		else {
			cout << "unable to open the file." << endl;
		}


		ofstream movieFile("movies.txt");
		if (movieFile.is_open()) {
			for (int i = 0; i < theaters.size(); i++) {
				movieFile << "Movies in Theater " << i + 1 << ":\n";
				for (Movie* movie : theaters[i]->allmovies) {
					movieFile << "Title: " << movie->getmovie()
						<< ", Genre: " << movie->getgenre()
						<< ", Timing: " << movie->getTimings()
						<< ", Price: " << movie->getPrice() << "\n";
				}
				movieFile << "\n";
			}
			movieFile.close();
			cout << "Movies have been saved." << endl;
		}
		else {
			cout << "unable to open file." << endl;
		}
	}

};
int validnum(const string& entry) {
	string input;
	while (true) {
		cout << entry;
		cin >> input;

		bool isValid = true;
		for (char c : input) {
			if (!isdigit(c)) {
				isValid = false;
				break;
			}
		}
		if (isValid) {
			return stoi(input);
		}
		else {
			cout << "Invalid input. Please enter a number." << endl;
		}
	}
}

int main() {
	const char* asciiArt = R"(
@@%%@##@@@@@@@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
@@*-#=+%%%#%@@@@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
***:*=-#%#-*@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%#%%#*%%%%%
=-*:==:#%=-#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%##%%%%%%%%%%+*%#=#*#%%
+:=:--:#+-*%%%%%%%%%%%%%%%%%%%%%%%%%****++*#%%%###################%%%%*=*#-+*+#*
#:::::--=*%%%%%%%%%%%%%%%%%%%%%%%%+-:-===++*#########################%#==+-=++#+
*:::--==+#@%%%%%%%%%%%%%%%%%%%%%#=:::-=+***#%%#########################*::----+#
*::-----=+*++*#%%%%%%%%%%%%%%%%%-...:=+++++*#@@####################*++*#=-+=:.-=
#---------=*#%%%%%%%%%%%%%%%%%%#:::::-=====+*%@@%##################**###*++++-:-
%=-=--==*#%%%%%%%%%%%%%%%%%%%%%#-::-=+++=+*##%@@%######################***+*%+-*
%=--==*%%%%%%%%%%%%%%%%%%%%%%%##-::+##*+=##*#%@@@#*##*###############*****+***+%
#-:-=+%%%%%%%%%%%%%%%%%%%%%%%#%*-::=+*+=-+%++#@@@*************************+===+#
*==+*#@%%%%%%%%%%%%%%%%%%%%%###*=::-+==--+%%**%@@*************************##%@@@
#***%@@%%%%%%%%%%%%%%%%%%%######+:-=====+*%@@%@@%*************************#%%##%
#**#%@@@%%%%%%%%%%%%%%%%#########=--=+++=+*%##@%***************************####%
%%%%%@@@%%%%%%%%%%%%%%%###########*=++====*%#%@****************************%###%
%%%%%@@@%%%%%%%%%%%%%%###########**=+*#*##%@@@@%*************************+#%%%%%
@%%%@@@@%%%%#+++**%*************+=%#*+***%@@@@@@@@%#***######*++++++++++++#%%%%%
@%%@@@@@%%%%=..-*#%#%%%%%#%%%###**@@*=+*%@@@@@@@@@@@@@@@@@@@@@%*++++++++++#@%%%@
@@@@@@@@%%%%==+##%%%@@@@@%%%@%@@%%@%#*%@%%@%%%@@@@@@@@@@@@@@@@@@%#++++++++%@%%@@
@@@@@@@@@@@%%%%%%%#%@@@@@%%%@%%%%@%#%@%%%%%#%%#@@@@@@@@@@@@@@@@@@@%####*++%@%@@@
@@@@@@@@@@%%%%%@@@#%@@@@@@%%%%%#%@%@%%%%%###*#*%@@@@@@@@@@@@@@@@@@@@@@@@%%@@@@@@
@@@@@@@@@%%@@@@@@@%%@@@@@@%@@%##@%%%#%%%######*#%%%@@%%@@@@@@@@@@@@@@@@%@@@@@@@@
%%@@@@@%@@@@@@@@@@%%%@@@@@%@@%#%@%%%%%#%%#%###*%%#%@@@@@@@@@@@@@@@@@@@@%@@@@@@@@
+*#%@@@@@@@@@@@@@@%%%@@@@%@%@@%@@#%##%%%%###%%*%%#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@%%@@@@@@@+##++*++*#++*+#**#+*=*#**%@@@@@@@@@@@@@@@@@@%#*++=++
@@@@@@%*##**%@@@@@%%%%%@@@@+--*-=++=+-*+=-%*=*-#:%+-*@@@@@@@@@@@@@@@@@#+==---=+*
@@@@@@@##%#+%@@@@@@%%%@@@@@*%**++**++#**#+*#++*#*@#**%@@@@@@@@@@@@@@@%+=+--==+##
@@@@@@@%%@@@%@%*++*%@@%*#@%*#@@@%*+*#*******#%*+#@@@@@#**%@@@@##%@@@@*+++--=+*##
@@@@@@@@@@@@@=.:=-..+@+ :@#  -%##- =%= .----+#.  *@@@*.  #@@@*  -@@@@+++=-=++#%%
@@@@@@@@@@@@* :@@@#-=@* :@# ..:*%- =%= -%%%%%#...:%@%:...%@@%.:- +@@%+++--=+*#%%
@@@@@@@@@@@@= =@@@@@@@+ :@# .*..#- =@= .:::-%#.:* =@+ *: %@@- *@:.%@@**+-=++*%%%
@@@@@@@@@@@@+ -@@@%**@+ :@# .%#:.: =@= -%%%@@%..@=.+.=@..%@*  -=: :@@#*+-=+*#%%%
@@@@@@@@@@@@%: =**: =@+ :@# .%@%:  +@= :***+#%  %@: .@%. %%..+***: =@%*+=***#%%@
@@@@@@@@@@@@@@*=--=#@@#=*@%=+@@@@+=#@*======*@=+@@#=#@@+=@*=*@@@@%=+@#**+**#%%@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*#*+*%%%%@@
    )";


	Admin admin;
	User user;

	admin.initializeTheatersAndMovies();
	admin.saveMoviesAndTheatersToFile();


	system("cls");
	int option;
	cout << YELLOW << "\t\t\t\tABSOLUTE CINEMA\t\t\t\t\t" << RESET << endl << endl;
	cout << YELLOW << asciiArt << RESET << endl;
	cout << YELLOW << "WELCOME TO THE MOVIE TICKET BOOKING SYSTEM!" << RESET << endl << endl;
	while (true) {
		cout << YELLOW << "1. Admin Menu" << endl;
		cout << "2. User Menu" << endl;
		cout << "3. Exit the system." << endl << endl;
		option = validnum("What would you like to select? ");
		cout << endl;
		if (option == 1) {
			int optionForAdmin;
			while (true) {
				cout << RED << "\nAdmin Menu" << RESET << endl;
				cout << RED << "1. Add a Theater " << RESET << endl;
				cout << RED << "2. Add a movie " << RESET << endl;
				cout << RED << "3. Display all Theaters " << RESET << endl;
				cout << RED << "4. Remove a Theater " << RESET << endl;
				cout << RED << "5. View the seating plan for a Theater " << RESET << endl;
				cout << RED << "6. Exit the Admin Menu and go back!" << RESET << endl << endl;

				optionForAdmin = validnum("Enter your choice again: ");

				if (optionForAdmin == 1) {
					Theater* newTheater = new Theater();
					admin.addtheater(newTheater);

				}
				else if (optionForAdmin == 2) {
					int theaterIndex;
					string name, genre, timing;
					double price;
					theaterIndex = validnum("Enter the index for the theater from 1 - X: ");

					if (theaterIndex<1 || theaterIndex >admin.getTheaters().size()) {
						cout << "You have entered a theater that doesn't exist. Retry. ";
						continue;

					}
					cout << "Enter the name of the movie you want to enter: " << endl;
					cin.ignore();
					getline(cin, name);
					cout << "Enter the genre of the movie you want to enter: " << endl;

					getline(cin, genre);
					while (true) {
						cout << "Enter the timing of the movie you want to enter: " << endl;
						getline(cin, timing);
						if (admin.timingValidation(timing)) {
							break;
						}

					}



					price = validnum("Enter the price of the ticket for the movie you want to enter: \n ");

					Movie* newMovie = new Movie(name, genre, timing, price);
					admin.addMovie(newMovie, theaterIndex - 1);


				}
				else if (optionForAdmin == 3) {
					admin.displayTheaters();
				}
				else if (optionForAdmin == 4) {
					int theaterIndex;
					cout << "Enter the index of the theater you want to remove: ";
					cin >> theaterIndex;
					admin.removetheater(theaterIndex - 1);

				}
				else if (optionForAdmin == 5) {
					int theaterIndex;
					cout << "Enter the theater index for viewing the seating arrangements. " << endl;
					cin >> theaterIndex;
					if (theaterIndex <1 || theaterIndex>admin.getTheaters().size()) {
						cout << "You have entered a theater that doesn't exist. Retry. ";
						continue;

					}
					admin.getTheaters()[theaterIndex - 1]->displaySeats();

				}
				else if (optionForAdmin == 6) {
					break;

				}
				else {
					cout << "You have entered an incorrect option. Try again" << endl;

				}
			}

		}
		else if (option == 2) {
			int optionForUser;
			while (true) {
				cout << GREEN << "User Menu" << RESET << endl;
				cout << GREEN << "1. View available Theaters" << RESET << endl;
				cout << GREEN << "2. Select a Theater" << RESET << endl;
				cout << GREEN << "3. View Available movies" << RESET << endl;
				cout << GREEN << "4. Choose a movie of your choice" << RESET << endl;
				cout << GREEN << "5. Select a seat for booking" << RESET << endl;
				cout << GREEN << "6. Check Bookings" << RESET << endl;
				cout << GREEN << "7. Recharge Account" << RESET << endl;
				cout << GREEN << "8. View Account Balance" << RESET << endl;
				cout << GREEN << "9. Exit the User Menu" << RESET << endl << endl;
				optionForUser = validnum("Enter your choice: ");


				if (optionForUser == 1) {
					vector<Theater*> theaters = admin.getTheaters();
					for (int i = 0; i < theaters.size(); i++) {
						cout << "Theater " << i + 1 << endl;
					}
				}
				else if (optionForUser == 2) {
					vector<Theater*> theaters = admin.getTheaters();
					int theaterIndex;
					theaterIndex = validnum("Enter the index of the theater you want to select from 1-X: \n");


					user.theaterSelection(theaterIndex, theaters);

				}
				else if (optionForUser == 3) {
					vector<Theater*> theaters = admin.getTheaters();
					if (!theaters.empty()) {
						int theaterIndex;
						theaterIndex = validnum("Enter the index of the Theater you want to view the movies for: \n");

						if (theaterIndex > 0 && theaterIndex <= theaters.size()) {
							theaters[theaterIndex - 1]->displaymovie();
						}
						else {
							cout << "You have entered a theater that doesn't exist. Try again" << endl;

						}
					}
					else {
						cout << "The theaters are unavailable. " << endl;

					}
				}

				else if (optionForUser == 4) {
					int movieIndex;
					vector<Theater*>theaters = admin.getTheaters();
					movieIndex = validnum("Enter a movie to select: \n");

					user.movieSelection(movieIndex, theaters);

				}
				else if (optionForUser == 5) {
					int row, col;
					row = validnum("Enter the row number from 0 to 9: \n");
					col = validnum("Enter the column number from 0 to 9: \n");


					user.booking(row, col);

				}
				else if (optionForUser == 6) {
					user.displayselection();
				}
				else if (optionForUser == 7) {
					double recharge;
					cout << "Enter the amount you want to recharge to your account: " << endl;
					cin >> recharge;
					user.fillBalance(recharge);

				}
				else if (optionForUser == 8) {
					cout << "Your current balance is : " << user.getBalance() << endl;
				}
				else if (optionForUser == 9) {
					break;

				}
				else {
					cout << "You have entered the wrong option. Try again." << endl;

				}
			}
		}
		else if (option == 3) {
			cout << "You have decided to exit the system. Thank you!" << endl;
			break;
		}
		else {
			cout << "You have entered the wrong option. Try again" << endl << endl;
			continue;
		}

	}
}