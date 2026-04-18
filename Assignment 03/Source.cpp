#include "Header.h"

void printMainMenu() {
	cout << endl;
	cout << "========================================" << endl;
	cout << "   Digital Content Management System    " << endl;
	cout << "========================================" << endl;
	cout << "  1.  Content Operations" << endl;
	cout << "  2.  Search Operations" << endl;
	cout << "  3.  Revision Control" << endl;
	cout << "  4.  Print & Export" << endl;
	cout << "  5.  File Operations" << endl;
	cout << "  6.  System Analysis" << endl;
	cout << "  0.  Exit" << endl;
	cout << "========================================" << endl;
	cout << "Enter choice: ";
}

void printContentMenu() {
	cout << endl;
	cout << "-------- Content Operations --------" << endl;
	cout << "  1. Create New Content" << endl;
	cout << "  2. Add Revision to Content" << endl;
	cout << "  3. Delete Content" << endl;
	cout << "  4. Get Revision by Timestamp" << endl;
	cout << "  5. Get Latest Timestamp" << endl;
	cout << "  0. Back" << endl;
	cout << "------------------------------------" << endl;
	cout << "Enter choice: ";
}

void printSearchMenu() {
	cout << endl;
	cout << "-------- Search Operations --------" << endl;
	cout << "  1. Search Content Titles" << endl;
	cout << "  2. Search Text Inside Content" << endl;
	cout << "  3. Find Content by Title" << endl;
	cout << "  0. Back" << endl;
	cout << "-----------------------------------" << endl;
	cout << "Enter choice: ";
}

void printRevisionMenu() {
	cout << endl;
	cout << "-------- Revision Control --------" << endl;
	cout << "  1. Create Snapshot" << endl;
	cout << "  2. Restore Snapshot" << endl;
	cout << "  3. List Snapshots" << endl;
	cout << "  4. Analyze Content History" << endl;
	cout << "  0. Back" << endl;
	cout << "----------------------------------" << endl;
	cout << "Enter choice: ";
}

void printPrintExportMenu() {
	cout << endl;
	cout << "-------- Print & Export --------" << endl;
	cout << "  1. Print Revisions of Content" << endl;
	cout << "  2. Export Content to File" << endl;
	cout << "  0. Back" << endl;
	cout << "--------------------------------" << endl;
	cout << "Enter choice: ";
}

void printFileMenu() {
	cout << endl;
	cout << "-------- File Operations --------" << endl;
	cout << "  1. Save System to Disk" << endl;
	cout << "  2. Load System from File" << endl;
	cout << "  0. Back" << endl;
	cout << "---------------------------------" << endl;
	cout << "Enter choice: ";
}

void printSearchResults(SearchResultNode* head) {
	if (!head) {
		cout << "No results found." << endl;
		return;
	}
	cout << endl << "--- Search Results ---" << endl;
	SearchResultNode* curr = head;
	while (curr) {
		cout << "Title: " << curr->title;
		if (curr->revisionNumber != 0)
			cout << " | Revision: " << curr->revisionNumber;
		cout << endl;
		curr = curr->next;
	}
	cout << "---------------------" << endl;
}

void freeSearchResults(SearchResultNode* head) {
	while (head) {
		SearchResultNode* temp = head;
		head = head->next;
		delete temp;
	}
}

void handleContentOperations(ContentManagementSystem& cms) {
	int choice;
	do {
		printContentMenu();
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			string title, text;
			cout << "Enter title: ";
			getline(cin, title);
			cout << "Enter text: ";
			getline(cin, text);
			cms.createContent(title, text);
			cout << "Content created successfully." << endl;
		}
		else if (choice == 2) {
			string title, text;
			cout << "Enter title: ";
			getline(cin, title);
			cout << "Enter new revision text: ";
			getline(cin, text);
			cms.addRevision(title, text);
		}
		else if (choice == 3) {
			string title;
			cout << "Enter title to delete: ";
			getline(cin, title);
			cms.deleteContent(title);
			cout << "Content deleted (if it existed)." << endl;
		}
		else if (choice == 4) {
			string title;
			long ts;
			cout << "Enter title: ";
			getline(cin, title);
			cout << "Enter timestamp: ";
			cin >> ts;
			cin.ignore();
			Revision* rev = cms.getRevision(title, ts);
			if (rev) {
				cout << endl << "--- Revision Found ---" << endl;
				cout << "Revision Number : " << rev->revisionNumber << endl;
				cout << "Timestamp       : " << rev->timestamp << endl;
				cout << "Size            : " << rev->size << " bytes" << endl;
				cout << "Text            : " << rev->text << endl;
				cout << "----------------------" << endl;
			}
			else {
				cout << "No revision found at or before that timestamp." << endl;
			}
		}
		else if (choice == 5) {
			string title;
			cout << "Enter title: ";
			getline(cin, title);
			long ts = cms.getLatestTimestamp(title);
			if (ts == -1)
				cout << "Content not found or has no revisions." << endl;
			else
				cout << "Latest timestamp for \"" << title << "\": " << ts << endl;
		}
		else if (choice != 0) {
			cout << "Invalid choice. Try again." << endl;
		}

	} while (choice != 0);
}

void handleSearchOperations(ContentManagementSystem& cms) {
	int choice;
	do {
		printSearchMenu();
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			string pattern;
			cout << "Enter title pattern to search: ";
			getline(cin, pattern);
			SearchResultNode* results = cms.searchContentTitles(pattern);
			printSearchResults(results);
			freeSearchResults(results);
		}
		else if (choice == 2) {
			string keyword;
			cout << "Enter keyword to search inside content: ";
			getline(cin, keyword);
			SearchResultNode* results = cms.searchText(keyword);
			printSearchResults(results);
			freeSearchResults(results);
		}
		else if (choice == 3) {
			string title;
			cout << "Enter exact title: ";
			getline(cin, title);
			ContentMetadata* meta = cms.findContent(title);
			if (!meta) {
				cout << "Content not found." << endl;
			}
			else {
				cout << endl << "--- Content Found ---" << endl;
				cout << "Title          : " << meta->title << endl;
				cout << "Total Revisions: " << meta->totalRevisions << endl;
				cout << "Total Size     : " << meta->totalSize << " bytes" << endl;
				cout << "Last Modified  : " << meta->lastModified << endl;
				cout << "---------------------" << endl;
			}
		}
		else if (choice != 0) {
			cout << "Invalid choice. Try again." << endl;
		}

	} while (choice != 0);
}

void handleRevisionControl(ContentManagementSystem& cms) {
	int choice;
	do {
		printRevisionMenu();
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			string name;
			cout << "Enter snapshot name: ";
			getline(cin, name);
			cms.createSnapshot(name);
			cout << "Snapshot \"" << name << "\" created successfully." << endl;
		}
		else if (choice == 2) {
			string name;
			cout << "Enter snapshot name to restore: ";
			getline(cin, name);
			cms.restoreSnapshot(name);
		}
		else if (choice == 3) {
			cms.listSnapshots();
		}
		else if (choice == 4) {
			string title;
			cout << "Enter title to analyze: ";
			getline(cin, title);
			cms.analyzeContentHistory(title);
		}
		else if (choice != 0) {
			cout << "Invalid choice. Try again." << endl;
		}

	} while (choice != 0);
}

void handlePrintExport(ContentManagementSystem& cms) {
	int choice;
	do {
		printPrintExportMenu();
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			string title;
			cout << "Enter title: ";
			getline(cin, title);
			cms.printRevisions(title);
		}
		else if (choice == 2) {
			string title, outFile;
			int revNo;
			cout << "Enter title: ";
			getline(cin, title);
			cout << "Enter revision number: ";
			cin >> revNo;
			cin.ignore();
			cout << "Enter output file name: ";
			getline(cin, outFile);
			cms.exportContent(title, revNo, outFile);
		}
		else if (choice != 0) {
			cout << "Invalid choice. Try again." << endl;
		}

	} while (choice != 0);
}

void handleFileOperations(ContentManagementSystem& cms) {
	int choice;
	do {
		printFileMenu();
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			string filename;
			cout << "Enter filename to save to: ";
			getline(cin, filename);
			cms.saveToDisk(filename);
		}
		else if (choice == 2) {
			string filepath;
			cout << "Enter file path to load from: ";
			getline(cin, filepath);
			cms.loadFromFile(filepath);
		}
		else if (choice != 0) {
			cout << "Invalid choice. Try again." << endl;
		}

	} while (choice != 0);
}

int main() {
	ContentManagementSystem cms;

	int choice;
	do {
		printMainMenu();
		cin >> choice;
		cin.ignore();

		if (choice == 1) {
			handleContentOperations(cms);
		}
		else if (choice == 2) {
			handleSearchOperations(cms);
		}
		else if (choice == 3) {
			handleRevisionControl(cms);
		}
		else if (choice == 4) {
			handlePrintExport(cms);
		}
		else if (choice == 5) {
			handleFileOperations(cms);
		}
		else if (choice == 6) {
			cms.systemAnalysis();
		}
		else if (choice != 0) {
			cout << "Invalid choice. Try again." << endl;
		}

	} while (choice != 0);

	cout << "Exiting system. Goodbye!" << endl;
	return 0;
}