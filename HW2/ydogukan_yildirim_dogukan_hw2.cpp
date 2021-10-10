#include <iostream>
#include "BinarySearchTree.h"
#include "AvlTree.h"
#include <string>
#include <fstream>
#include <chrono>

using namespace std;

// Dogukan Yildirim

struct Item { // Sections will have Items in them.
	string title;
	string info;

	Item() {}
	Item(string myTitle) : title(myTitle) {}
	Item(string myTitle, string myInfo) : title(myTitle), info(myInfo) {}

	bool operator < (const Item& rhs) const {
		return this->title < rhs.title;
	}

	bool operator > (const Item& rhs) const {
		return this->title > rhs.title;
	}
};

ostream& operator << (ostream& os, const Item& myItem) {
	os << myItem.title;
	return os;
}

template <class treeType>
struct Section { // Templated Section struct for both AVL and Binary Search trees.
	string title;
	treeType tree;

	Section<treeType>() {}
	Section<treeType>(string myTitle) : title(myTitle) {}

	bool operator < (const Section<treeType>& rhs) const {
		return this->title < rhs.title;
	}

	bool operator > (const Section<treeType>& rhs) const {
		return this->title < rhs.title;
	}

	const Section<treeType>& operator=(const Section<treeType>& rhs) {
		title = rhs.title;
		tree = rhs.tree;
		return *this;
	}
};

ostream& operator << (ostream& os, const Section<AvlTree<Item>>& mySection) {
	os << mySection.title;
	return os;
}

ostream& operator << (ostream& os, const Section<BinarySearchTree<Item>>& mySection) {
	os << mySection.title;
	return os;
}

/*
	This function reads the file line by line that contains Section and Item data of the notebook, and inserts this data
	to two notebooks. The first notebook is an AVL tree that contains Sections that have items stored in their respective AVL 
	trees, while the second notebook is essentially the same thing but with Binary Trees instead of AVL trees.
*/
void readFileToNotebooks(AvlTree<Section<AvlTree<Item>>>& notebookAVL, BinarySearchTree<Section<BinarySearchTree<Item>>>& notebookBST) {
	string inputFileName = "data.txt";
	ifstream input;
	input.open(inputFileName.c_str());

	if (input.fail()) {
		cout << "ERROR: File named " << inputFileName << " cannot be opened." << endl;
	}

	else {
		string line;
		getline(input, line); // Getting the first line to create the first Sections
		Section<AvlTree<Item>> currentSectionAVL = Section<AvlTree<Item>>(line);
		Section<BinarySearchTree<Item>> currentSectionBST = Section<BinarySearchTree<Item>>(line);
		long long timerAVL = 0, timerBST = 0;

		while (getline(input, line)) { // A loop to fill and create Sections
			if (line.at(0) != '-') { // If the line represents a new Section
				auto startAVL = std::chrono::high_resolution_clock::now();
				notebookAVL.insert(currentSectionAVL); // Insert the old Section
				auto endAVL = std::chrono::high_resolution_clock::now();

				timerAVL += (endAVL - startAVL).count() / 1000.0;
				cout << "Section " << currentSectionAVL.title << " has been inserted into AVL notebook." << endl
					<< "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
				timerAVL = 0; // Reset the timer
				currentSectionAVL = Section<AvlTree<Item>>(line); // Create a new Section

				auto startBST = std::chrono::high_resolution_clock::now();
				notebookBST.insert(currentSectionBST); // Insert the old Section
				auto endBST = std::chrono::high_resolution_clock::now();

				timerBST += (endBST - startBST).count() / 1000.0;
				cout << "Section " << currentSectionBST.title << " has been inserted into BST notebook." << endl
					<< "[BST] Elapsed time: " << timerBST << " microseconds" << endl << endl;
				timerBST = 0; // Reset the timer
				currentSectionBST = Section<BinarySearchTree<Item>>(line); // Insert the old Section
			}

			else { // If the line represents a new Item
				int seperatorIndex = line.find('-', 1); // Seperate Item title from Item info

				auto startAVL = std::chrono::high_resolution_clock::now();
				currentSectionAVL.tree.insert(Item(line.substr(1, seperatorIndex - 1), line.substr(seperatorIndex + 1))); // Create a new Item with the given title and info, and add it to the Section
				auto endAVL = std::chrono::high_resolution_clock::now();
				timerAVL += (endAVL - startAVL).count() / 1000.0; // Add to the timer

				auto startBST = std::chrono::high_resolution_clock::now();
				currentSectionBST.tree.insert(Item(line.substr(1, seperatorIndex - 1), line.substr(seperatorIndex + 1))); // Create a new Item with the given title and info, and add it to the Section
				auto endBST = std::chrono::high_resolution_clock::now();
				timerBST += (endBST - startBST).count() / 1000.0; // Add to the timer
			}
		}

		// The last Section is not inserted, so let's do that
		auto startAVL = std::chrono::high_resolution_clock::now();
		notebookAVL.insert(currentSectionAVL); // Insert the last Section
		auto endAVL = std::chrono::high_resolution_clock::now();

		timerAVL += (endAVL - startAVL).count() / 1000.0;
		cout << "Section " << currentSectionAVL.title << " has been inserted into AVL notebook." << endl
			<< "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;

		auto startBST = std::chrono::high_resolution_clock::now();
		notebookBST.insert(currentSectionBST); // Insert the last Section
		auto endBST = std::chrono::high_resolution_clock::now();

		timerBST += (endBST - startBST).count() / 1000.0;
		cout << "Section " << currentSectionBST.title << " has been inserted into BST notebook." << endl
			<< "[BST] Elapsed time: " << timerBST << " microseconds" << endl << endl;

		input.close();
	}
}

void displayMainMenu() {
	cout << "MENU\nPlease enter an input between [1 - 6]:" << endl << "1 - Display the sections [AVL]" << endl << "2 - Display the sections [BST]" << endl
		<< "3 - Select a section" << endl << "4 - Add new section" << endl << "5 - Delete a section" << endl << "6 - Exit";
}

void displaySectionMenu(string sectionTitle) {
	cout << endl << "Selected section -> " << sectionTitle << endl << "Please enter an input between [1 - 7]:" << endl << "1 - Display the items [AVL]" << endl
		<< "2 - Display the items [BST]" << endl << "3 - Display the information of a item" << endl << "4 - Add new item" << endl << "5 - Update the information of a item"
		<< endl << "6 - Delete an item" << endl << "7 - Return to main menu" << endl << "Input: ";
}

template <class treeType>
void printTreeWithStars(const treeType& tree) {
	cout << endl << "*****" << endl;
	if (!tree.isEmpty()) {
		tree.printTree();
	}
	cout << "*****" << endl;
}

bool sectionExistsInNotebooks(const AvlTree<Section<AvlTree<Item>>>& notebookAVL, const BinarySearchTree<Section<BinarySearchTree<Item>>>& notebookBST, string sectionTitle) {
	return notebookAVL.find(Section<AvlTree<Item>>(sectionTitle)).title.length() != 0 && notebookBST.find(Section<BinarySearchTree<Item>>(sectionTitle)).title.length() != 0;
}


int main() {
	cout << "Welcome to the Notes" << endl << endl;

	AvlTree<Section<AvlTree<Item>>> notebookAVL;
	BinarySearchTree<Section<BinarySearchTree<Item>>> notebookBST;
	readFileToNotebooks(notebookAVL, notebookBST);

	string userInput;
	displayMainMenu();
	cout << endl << "Input: ";
	cin >> userInput;
	cin.ignore();

	while (userInput != "6") {
		if (userInput == "1") { // Main Menu Option 1: Display the Sections (AVL)
			printTreeWithStars(notebookAVL);
		}

		else if (userInput == "2") { // Main Menu Option 2: Display the Sections (BST)
			printTreeWithStars(notebookBST);
		}

		else if (userInput == "3") { // Main Menu Option 3: Select a Section
			string inputSectionTitle;
			cout << "Enter the title of the section: ";
			getline(cin, inputSectionTitle);

			// Trying to find the Section with the given title to check whether it exists in the Notebooks
			Section<AvlTree<Item>> selectedSectionAVL = notebookAVL.find(Section<AvlTree<Item>>(inputSectionTitle));
			Section<BinarySearchTree<Item>> selectedSectionBST = notebookBST.find(Section<BinarySearchTree<Item>>(inputSectionTitle));

			if (selectedSectionAVL.title.length() != 0 && selectedSectionBST.title.length() != 0) { // If both are not ITEM_NOT_FOUND, i.e. the Section exists
				string userInputSec;
				displaySectionMenu(inputSectionTitle);
				cin >> userInputSec;
				cin.ignore();

				while (userInputSec != "7") {
					if (userInputSec == "1") { // Section Option 1: Display the Items (AVL) 
						printTreeWithStars(selectedSectionAVL.tree);
					}

					else if (userInputSec == "2") { // Section Option 2: Display the Items (BST)
						printTreeWithStars(selectedSectionBST.tree);
					}

					else if (userInputSec == "3") { // Section Option 3: Display the information of an Item
						string inputItemTitle;
						cout << "Enter the title of the item: ";
						getline(cin, inputItemTitle);
						
						// Searching for the Item with the given title in the both AVL and BST Sections and displaying the elapsed time
						Item searchingFor(inputItemTitle), foundItemAVL, foundItemBST;

						auto startAVL = std::chrono::high_resolution_clock::now();
						foundItemAVL = selectedSectionAVL.tree.find(searchingFor);
						auto endAVL = std::chrono::high_resolution_clock::now();
						long long timerAVL = (endAVL - startAVL).count() / 1000.0;
						cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;

						auto startBST = std::chrono::high_resolution_clock::now();
						foundItemBST = selectedSectionBST.tree.find(searchingFor);
						auto endBST = std::chrono::high_resolution_clock::now();
						long long timerBST = (endBST - startBST).count() / 1000.0;
						cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;

						if (foundItemAVL.title.length() != 0 && foundItemBST.title.length() != 0 && foundItemAVL.info == foundItemBST.info) { // If the Item DOES exist and info's match
							cout << foundItemAVL.info << endl;
						}

						else { // If an Item with the given title does not exist
							cout << "Invalid title." << endl;
						}
					}

					else if (userInputSec == "4") { // Section Option 4: Add a new Item
						string inputItemTitle;
						cout << "Enter the title of the item: ";
						getline(cin, inputItemTitle);

						// Searching for the Item with the given title in the both AVL and BST Sections and adding the elapsed time to query time
						Item searchingFor(inputItemTitle), foundItemAVL, foundItemBST;

						auto startAVL = std::chrono::high_resolution_clock::now();
						foundItemAVL = selectedSectionAVL.tree.find(searchingFor);
						auto endAVL = std::chrono::high_resolution_clock::now();
						long long timerAVL = (endAVL - startAVL).count() / 1000.0;

						auto startBST = std::chrono::high_resolution_clock::now();
						foundItemBST = selectedSectionBST.tree.find(searchingFor);
						auto endBST = std::chrono::high_resolution_clock::now();
						long long timerBST = (endBST - startBST).count() / 1000.0;

						if (foundItemAVL.title.length() != 0 && foundItemBST.title.length() != 0) { // If the Item with the given title already exists
							cout << "Item \"" << inputItemTitle << "\" already exists in the \"" << selectedSectionAVL.title << "\"." << endl;
						}

						else { // If an Item with the given title does not exist
							string inputItemInfo;
							cout << "Enter a description for the item: ";
							getline(cin, inputItemInfo);

							// Inserting the Item with the given title and info to the both AVL and BST Sections and displaying the total query time
							// Also updating the Notebook
							Item itemToBeInserted(inputItemTitle, inputItemInfo);
							Section<AvlTree<Item>> sectionToBeUpdatedAVL(inputSectionTitle);
							Section<BinarySearchTree<Item>> sectionToBeUpdatedBST(inputSectionTitle);

							auto startAVL2 = std::chrono::high_resolution_clock::now();
							selectedSectionAVL.tree.insert(itemToBeInserted);
							notebookAVL.findAndUpdate(sectionToBeUpdatedAVL, selectedSectionAVL);
							auto endAVL2 = std::chrono::high_resolution_clock::now();
							timerAVL += (endAVL2 - startAVL2).count() / 1000.0;
							cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;

							auto startBST2 = std::chrono::high_resolution_clock::now();
							selectedSectionBST.tree.insert(itemToBeInserted);
							notebookBST.findAndUpdate(sectionToBeUpdatedBST, selectedSectionBST);
							auto endBST2 = std::chrono::high_resolution_clock::now();
							timerBST += (endBST2 - startBST2).count() / 1000.0;
							cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;

							cout << "The new item \"" << inputItemTitle << "\" has been inserted." << endl;
						}
					}

					else if (userInputSec == "5") { // Section Option 5: Update the information of an Item
						string inputItemTitle;
						cout << "Enter the title of the item: ";
						getline(cin, inputItemTitle);

						// Searching for the Item with the given title in the both AVL and BST Sections and displaying the elapsed time
						Item searchingFor(inputItemTitle), foundItemAVL, foundItemBST;

						auto startAVL = std::chrono::high_resolution_clock::now();
						foundItemAVL = selectedSectionAVL.tree.find(searchingFor);
						auto endAVL = std::chrono::high_resolution_clock::now();
						long long timerAVL = (endAVL - startAVL).count() / 1000.0;
						cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;

						auto startBST = std::chrono::high_resolution_clock::now();
						foundItemBST = selectedSectionBST.tree.find(searchingFor);
						auto endBST = std::chrono::high_resolution_clock::now();
						long long timerBST = (endBST - startBST).count() / 1000.0;
						cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;

						if (foundItemAVL.title.length() != 0 && foundItemBST.title.length() != 0) { // If the Item with the given title already exists
							string inputItemInfo;
							cout << "Enter the new information: ";
							getline(cin, inputItemInfo);

							// Finding and updating the Item, also updating the Notebook
							Item toBeFound = Item(inputItemTitle);
							Item updated = Item(inputItemTitle, inputItemInfo);
							Section<AvlTree<Item>> sectionToBeUpdatedAVL(inputSectionTitle);
							Section<BinarySearchTree<Item>> sectionToBeUpdatedBST(inputSectionTitle);

							selectedSectionAVL.tree.findAndUpdate(toBeFound, updated);
							selectedSectionBST.tree.findAndUpdate(toBeFound, updated);

							notebookAVL.findAndUpdate(sectionToBeUpdatedAVL, selectedSectionAVL);
							notebookBST.findAndUpdate(sectionToBeUpdatedBST, selectedSectionBST);

							cout << "The content " << inputItemTitle << " has updated." << endl;
						}

						else { // If an Item with the given title does not exist
							cout << "Item \"" << inputItemTitle << "\" does not exist in the \"" << selectedSectionAVL.title << "\"." << endl;
						}
					}

					else if (userInputSec == "6") { // Section Option 6: Delete an Item
						string inputItemTitle;
						cout << "Enter the title of the item: ";
						getline(cin, inputItemTitle);
						Item searchingFor(inputItemTitle), foundItemAVL, foundItemBST;

						auto startAVL = std::chrono::high_resolution_clock::now();
						foundItemAVL = selectedSectionAVL.tree.find(searchingFor);
						auto endAVL = std::chrono::high_resolution_clock::now();
						long long timerAVL = (endAVL - startAVL).count() / 1000.0;

						auto startBST = std::chrono::high_resolution_clock::now();
						foundItemBST = selectedSectionBST.tree.find(searchingFor);
						auto endBST = std::chrono::high_resolution_clock::now();
						long long timerBST = (endBST - startBST).count() / 1000.0;

						if (foundItemAVL.title.length() == 0 && foundItemBST.title.length() == 0) { // If the Item with the given title does not exist
							cout << "Item \"" << inputItemTitle << "\" does not exist in the \"" << selectedSectionAVL.title << "\"." << endl;
						}

						else { // If the Item with the given title already exists
							// Finding and removing the Item from the Section and also updating the Notebook
							Item itemToBeRemoved(inputItemTitle);
							Section<AvlTree<Item>> sectionToBeUpdatedAVL(inputSectionTitle);
							Section<BinarySearchTree<Item>> sectionToBeUpdatedBST(inputSectionTitle);

							auto startAVL2 = std::chrono::high_resolution_clock::now();
							selectedSectionAVL.tree.remove(itemToBeRemoved);
							notebookAVL.findAndUpdate(sectionToBeUpdatedAVL, selectedSectionAVL);
							auto endAVL2 = std::chrono::high_resolution_clock::now();
							timerAVL += (endAVL2 - startAVL2).count() / 1000.0;
							cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;

							auto startBST2 = std::chrono::high_resolution_clock::now();
							selectedSectionBST.tree.remove(itemToBeRemoved);
							notebookBST.findAndUpdate(sectionToBeUpdatedBST, selectedSectionBST);
							auto endBST2 = std::chrono::high_resolution_clock::now();
							timerBST += (endBST2 - startBST2).count() / 1000.0;
							cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;

							cout << "The item \"" << inputItemTitle << "\" has been deleted." << endl;
						}
					}

					cout << endl << "Input: ";
					cin >> userInputSec;
					cin.ignore();
				}

				if (userInputSec == "7") { // Section Option 7: Return to Main Menu
					cout << endl;
					displayMainMenu();
				}
			}

			else {
				cout << "Invalid title!" << endl;
			}

		}

		else if (userInput == "4") { // Main Menu Option 4: Add new Section
			string inputSectionTitle;
			cout << "Enter a title for the section: ";
			getline(cin, inputSectionTitle);

			if (sectionExistsInNotebooks(notebookAVL, notebookBST, inputSectionTitle)) {
				cout << "Section \"" << inputSectionTitle << "\" already exists." << endl;
			}

			else {
				// Insert a Section with the given title to both Notebooks
				notebookAVL.insert(Section<AvlTree<Item>>(inputSectionTitle));
				notebookBST.insert(Section<BinarySearchTree<Item>>(inputSectionTitle));
				cout << "The new section \"" << inputSectionTitle << "\" has been inserted." << endl;
			}
		}

		else if (userInput == "5") { // Main Menu Option 5: Delete a Section
			string inputSectionTitle;
			cout << "Enter a title for the section: ";
			getline(cin, inputSectionTitle);

			if (!sectionExistsInNotebooks(notebookAVL, notebookBST, inputSectionTitle)) {
				cout << "Section \"" << inputSectionTitle << "\" does not exist." << endl;
			}

			else {
				notebookAVL.remove(Section<AvlTree<Item>>(inputSectionTitle));
				notebookBST.remove(Section<BinarySearchTree<Item>>(inputSectionTitle));
				cout << "The section has been deleted." << endl;
			}
		}

		cout << endl << "Input: ";
		cin >> userInput;
		cin.ignore();
	}

	cout << endl << "Terminating..." << endl;
	return 0;
}