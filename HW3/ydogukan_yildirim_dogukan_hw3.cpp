#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

using namespace std;

// Dogukan Yildirim

/*INSERTION SORT*/
template <class Comparable>
void insertionSort(vector <Comparable>& a) {
	int j;
	// loop over the passes
	for (int p = 1; p < a.size(); p++) {
		Comparable tmp = a[p];
		// loop over the elements
		for (j = p; j > 0 && tmp < a[j-1]; j--)
			a[j] = a[j-1];
		a[j] = tmp;
	}
}

/*QUICK SORT*/
template <class Comparable>
const Comparable& median3(vector<Comparable>& a, int left, int right) {
	int center = (left + right) / 2;
	if (a[center] < a[left])
		swap(a[left], a[center]);
	if (a[right] < a[left])
		swap(a[left], a[right]);
	if (a[right] < a[center])
		swap(a[center], a[right]);

	// Place pivot at position right - 1
	swap(a[center], a[right - 1]);
	return a[right - 1];
}

template <class Comparable>
void quicksort(vector<Comparable>& a, int left, int right) {
	if (left + 10 <= right) {
		Comparable pivot = median3(a, left, right);
		// Begin partitioning
		int i = left, j = right - 1;
		for (;;)
		{
			while (a[++i] < pivot) {}

			while (pivot < a[--j]) {}

			if (i < j)
				swap(a[i], a[j]);
			else
				break;
		}

		swap(a[i], a[right - 1]);   // Restore pivot
		quicksort(a, left, i - 1);       // Sort small elements
		quicksort(a, i + 1, right);    // Sort large elements
	}

	else  // Do an insertion sort on the subarray
		insertionSort(a, left, right);
}

template <class Comparable>
void quicksort(vector<Comparable>& a) {
	quicksort(a, 0, a.size() - 1);
}

template <class Comparable>
void insertionSort(vector<Comparable>& a, int left, int right) {
	for (int p = left + 1; p <= right; p++) {
		Comparable tmp = a[p];
		int j;
		for (j = p; j > left && tmp < a[j - 1]; j--)
			a[j] = a[j - 1];
		a[j] = tmp;
	}
}

/*IN-PLACE MERGE SORT*/
template <class Comparable>
void merge(vector<Comparable>& a, 
	int leftPos, int rightPos, int rightEnd) {
	int leftEnd = rightPos - 1;

	// Main loop
	Comparable temp;
	while (leftPos <= leftEnd && rightPos <= rightEnd) {
		if (a[leftPos] > a[rightPos]) {
			temp = a[leftPos];
			a[leftPos] = a[rightPos];
			for (int i = rightPos; i > leftPos; i--) {
				a[i] = a[i - 1];
			}
			leftPos++;
			a[leftPos] = temp;
			leftEnd++;
			rightPos++;
		}

		else {
			leftPos++;
		}
	}
}

template <class Comparable>
void mergeSort(vector<Comparable>& a, int left, int right) {
	if (left < right) {
		int center = (left + right) / 2;
		mergeSort(a, left, center);
		mergeSort(a, center + 1, right);
		merge(a, left, center + 1, right);
	}
}

template <class Comparable>
void mergeSort(vector<Comparable>& a) {
	mergeSort(a, 0, a.size() - 1);
}

/*HEAP SORT*/
inline int leftChild(int i) {
	return 2 * i + 1;
}

template <class Comparable>
void percDown(vector<Comparable>& a, int i, int n) {
	// for deleteMax
	// a is the array, i is the position to percolate down from
	// n is the logical size of the array
	int child;
	Comparable tmp;

	for (tmp = a[i]; leftChild(i) < n; i = child) {
		child = leftChild(i);
		if (child != n - 1 && a[child] < a[child + 1])
			child++;
		if (a[child] > tmp)
			a[i] = a[child];
		else
			break;
	}
	a[i] = tmp;
}

template <class Comparable>
void heapsort(vector<Comparable>& a) {
	// buildHeap
	for (int i = a.size() / 2; i >= 0; i--)
		percDown(a, i, a.size());
	// sort
	for (int j = a.size() - 1; j > 0; j--) {
		swap(a[0], a[j]);    // swap max to the last pos.
		percDown(a, 0, j); // re-form the heap
	}
}

/*THE REST*/
struct Contact { // Struct for Contacts stored in the phoneBook
	string name, fullName, phoneNum, city;

	Contact() {}
	Contact(string aName, string aSurname, string aPhoneNum, string aCity) 
		: name(aName), fullName(aName + " " + aSurname), phoneNum(aPhoneNum), city(aCity) {}

	bool operator < (const Contact& rhs) const {
		return this->fullName < rhs.fullName;
	}

	bool operator <= (const Contact& rhs) const {
		return this->fullName <= rhs.fullName;
	}

	bool operator > (const Contact& rhs) const {
		return this->fullName > rhs.fullName;
	}

	bool operator >= (const Contact& rhs) const {
		return this->fullName >= rhs.fullName;
	}
};

ostream& operator << (ostream& os, const Contact& myContact) {
	os << myContact.fullName << " " << myContact.phoneNum << " " << myContact.city << endl;
	return os;
}

string capitalized(string myStr) {
	for (int i = 0; i < myStr.length(); i++) {
		myStr.at(i) = toupper(myStr.at(i));
	}
	return myStr;
}

/*
	HOW THIS FUNCTION WORKS:
	Reads data from file, creates Contact objects with the data, stores the
	Contacts in a created phoneBook vector and returns the phoneBook vector.
*/
vector<Contact> readFileToPhoneBook() {
	string contactFileName;
	cout << "Please enter the contact file name:" << endl;
	cin >> contactFileName;
	cin.ignore();

	ifstream input;
	input.open(contactFileName.c_str());

	if (input.fail()) {
		cout << "ERROR: File named " << contactFileName << " cannot be opened." << endl;
	}

	else {
		vector<Contact> phoneBook;
		string line;

		while (getline(input, line)) {
			istringstream words(line);
			vector<string> contents; // Temporary vector to store the contents of the line so that we can use the Contact constructor to assign values in a single line
			string word;

			while (words >> word) {
				contents.push_back(word);
			}

			phoneBook.push_back(Contact(capitalized(contents[0]), capitalized(contents[1]), contents[2], contents[3]));
		}

		input.close();
		return phoneBook;
	}
}

/*
	Given a phoneBook vector and a function pointer to a sorting function, returns the time it takes to
	sort the given phoneBook vector with the sorting function pointed by the pointer.
*/
long long sortPhoneBookAndGetTime(vector<Contact>& phoneBook, void(*func)(vector<Contact>&)) {
	auto start = std::chrono::high_resolution_clock::now();
	func(phoneBook);
	auto end = std::chrono::high_resolution_clock::now();
	long long timer = (end - start).count() / 1.0;
	return timer;
}

/*
	Given a phoneBook vector and a search word, returns the indexes that contain the search word.
	Function behaviour changes if the search is going to be a partial search, i.e. only names of the Contacts are going to be checked.
*/
vector<int> sequentialSearch(const vector<Contact> & phoneBook, const string & searchWord, bool partial) {
	vector<int> indexes;
	int size = phoneBook.size();

	if (partial) { // If partial search is to be made.
		for (int i = 0; i < size; i++) {
			if (phoneBook[i].name.substr(0, searchWord.length()) == searchWord) { // Get all the indexes that contain the search word.
				indexes.push_back(i);
			}
		}
	}

	else { // If we are doing a full name search.
		for (int i = 0; i < size; i++) {
			if (phoneBook[i].fullName == searchWord) { // There exists only one person with the said full name, so return the index.
				indexes.push_back(i);
				return indexes;
			}
		}
	}

	return indexes; // Return all indexes or an empty vector.
}

/*
	Given a phoneBook vector and a search word, returns the indexes that contain the search word.
	Function behaviour changes if the search is going to be a partial search, i.e. only names of the Contacts are going to be checked.
*/
vector<int> binarySearch(const vector<Contact>& phoneBook, const string& searchWord, bool partial) { // Function behaviour changes if a partial search is to be made.
	vector<int> indexes;
	int start = 0, end = phoneBook.size() - 1, mid, lastIndexOfVector = end;

	if (partial) { // If partial search is to be made.
		while (start <= end) {
			mid = (end + start) / 2;
			string partialName = phoneBook[mid].name.substr(0, searchWord.length());

			if (partialName == searchWord) {
				// Go up (or left) until the index of the first occurence of the searchWord is found.
				while (mid >= 0 && phoneBook[mid].name.substr(0, searchWord.length()) == searchWord) {
					mid--;
				}
				
				mid++; // This is the index of the first occurence.

				// Go down (or right) until there are no more indexes where searchWord exists and push the 
				// indexes that contain the searchWord to the indexes vector while iterating.
				while (mid <= lastIndexOfVector && phoneBook[mid].name.substr(0, searchWord.length()) == searchWord) {
					indexes.push_back(mid);
					mid++;
				}

				return indexes;
			}

			else if (partialName < searchWord) {
				start = mid + 1;
			}

			else {
				end = mid - 1;
			}
		}
	}

	else { // If we are doing a full name search, classic binary search.
		while (start <= end) {
			mid = (end + start) / 2;

			if (phoneBook[mid].fullName == searchWord) { // There exists only one person with the said full name, so return the index.
				indexes.push_back(mid);
				return indexes;
			}

			else if (phoneBook[mid].fullName < searchWord) {
				start = mid + 1;
			}

			else {
				end = mid - 1;
			}
		}
	}

	return indexes; // Return an empty vector if there are no indexes.
}

void displaySearchResults(const vector<Contact>& phoneBook, const vector<int> indexes, string searchWord) {
	if (indexes.size() == 0) {
		cout << searchWord << " does NOT exist in the dataset" << endl;
	}

	else {
		for (int i = 0; i < indexes.size(); i++) {
			cout << phoneBook[indexes[i]];
		}
	}
}

int main() {
	vector<Contact> phoneBookIS = readFileToPhoneBook(), phoneBookQS = phoneBookIS, phoneBookMS = phoneBookIS, phoneBookHS = phoneBookIS;

	string searchWord;
	cout << "Please enter the word to be queried:" << endl;
	getline(cin, searchWord);

	cout << endl << "Sorting the vector copies" << endl << "======================================" << endl;

	long long timerQS = sortPhoneBookAndGetTime(phoneBookQS, quicksort), timerIS = sortPhoneBookAndGetTime(phoneBookIS, insertionSort),
		timerMS = sortPhoneBookAndGetTime(phoneBookMS, mergeSort), timerHS = sortPhoneBookAndGetTime(phoneBookMS, heapsort);
	
	cout << "Quick Sort Time: " << timerQS << " Nanoseconds" << endl << "Insertion Sort Time: " << timerIS << " Nanoseconds" << endl
		<< "Merge Sort Time: " << timerMS << " Nanoseconds" << endl << "Heap Sort Time: " << timerHS << " Nanoseconds" << endl;

	cout << endl << "Searching for " << searchWord << endl << "======================================" << endl;

	searchWord = capitalized(searchWord);
	bool partial = false;
	if (searchWord.find(" ") == string::npos) { partial = true; } // Checking if there is more than one word, if not, we are doing a partial search.

	int N = 1000; // Going to search 100 times to "have sharper timings".
	vector<int> indexesBS;
	cout << "Search results for Binary Search:" << endl;
	auto startBS = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		indexesBS = binarySearch(phoneBookIS, searchWord, partial);
	}
	auto endBS = std::chrono::high_resolution_clock::now();
	long long timerBS = (endBS - startBS).count() / 1.0;

	displaySearchResults(phoneBookIS, indexesBS, searchWord);
	cout << endl << "Binary Search Time: " << timerBS / N << " Nanoseconds" << endl;

	vector<int> indexesSS;
	cout << endl << "Search results for Sequential Search:" << endl;
	auto startSS = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < N; i++) {
		indexesSS = sequentialSearch(phoneBookIS, searchWord, partial);
	}
	auto endSS = std::chrono::high_resolution_clock::now();
	long long timerSS = (endSS - startSS).count() / 1.0;

	displaySearchResults(phoneBookIS, indexesSS, searchWord);
	cout << endl << "Sequential Search Time: " << timerSS / N << " Nanoseconds" << endl;

	cout << endl << "SpeedUp between Search Algorithms" << endl << "======================================" << endl;
	cout << "(Sequential Search / Binary Search) SpeedUp = " << (double) timerSS / timerBS << endl;

	cout << endl << "SpeedUps between Sorting Algorithms" << endl << "======================================" << endl;
	cout << "(Insertion Sort / Quick Sort) SpeedUp = " << (double) timerIS / timerQS << endl << "(Merge Sort / Quick Sort) SpeedUp = " 
		<< (double) timerMS / timerQS << endl << "(Heap Sort / Quick Sort) SpeedUp = "  << (double) timerHS / timerQS << endl;

	return 0;
}