#pragma once
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class Revision {
public:

	string title;
	string text;
	int size;
	long timestamp;
	int revisionNumber;

	Revision* left;
	Revision* right;

	int height;
	Revision() {

		left = right = nullptr;
		size = 0;
		height = 1;
		timestamp = 0;
	}
	Revision(long ts) {
		left = right = nullptr;
		size = 0;
		height = 1;
		timestamp = ts;
	}

};

class ContentMetadata {
public:
	string title;
	Revision* currentRevision;
	Revision* revisionRoot;
	int totalRevisions;
	long lastModified;
	int totalSize;
	ContentMetadata* left;
	ContentMetadata* right;

	ContentMetadata() {
		currentRevision = revisionRoot = nullptr;
		left = right = nullptr;
		totalRevisions = 0;
		totalSize = 0;
		lastModified = 0;
	}
	int getBalanceFactor(Revision* node) {
		if (node == nullptr) return 0;
		return getNodeHeight(node->left) - getNodeHeight(node->right);
	}
	void printRevisionsInOrder(Revision* root, int& prevSize) {
		if (!root) return;

		printRevisionsInOrder(root->left, prevSize);

		cout << "r" << root->revisionNumber
			<< " | T: " << root->timestamp
			<< " | size: " << root->size;

		if (prevSize != -1) {
			int diff = root->size - prevSize;
			if (diff >= 0)
				cout << " (+" << diff << ")";
			else
				cout << " (" << diff << ")";
		}

		cout << endl;

		prevSize = root->size;

		printRevisionsInOrder(root->right, prevSize);
	}
	void printBalanceFactors(Revision* root) {
		if (!root) return;

		printBalanceFactors(root->left);

		cout << "Node (T=" << root->timestamp << ") BF = "
			<< getBalanceFactor(root) << endl;

		printBalanceFactors(root->right);
	}
	int getNodeHeight(Revision* node) {
		if (node == nullptr)
			return 0;
		return node->height;
	}
	Revision* createRevision(Revision* root, Revision* newNode) {

		if (root == nullptr) {
			return newNode;
		}

		if (newNode->timestamp < root->timestamp) {
			root->left = createRevision(root->left, newNode);
		}
		else {
			root->right = createRevision(root->right, newNode);
		}
		int leftH = getNodeHeight(root->left);
		int rightH = getNodeHeight(root->right);

		root->height = 1 + (leftH > rightH ? leftH : rightH);

		return root;
	}
	void createRevision(string title, string text, long ts, int revNum) {

		Revision* newNode = new Revision(ts);
		newNode->title = title;
		newNode->text = text;
		newNode->size = text.size();
		newNode->revisionNumber = revNum;

		revisionRoot = createRevision(revisionRoot, newNode);

		currentRevision = findLatest(revisionRoot);
	}
	Revision* findLatest(Revision* root) {
		if (!root) return nullptr;

		while (root->right)
			root = root->right;

		return root;
	}
	void printRV(Revision* root) {
		if (!root)
			return;

		printRV(root->left);

		cout << "Revision Number : " << root->revisionNumber << endl;
		cout << "Revision Title : " << root->title << endl;
		cout << "Text : " << root->text << endl;
		cout << "Size : " << root->size << endl;
		cout << "Height : " << root->height << endl;
		cout << "Timestamp : " << root->timestamp << endl;
		cout << "-----------------------------" << endl;

		printRV(root->right);
	}
	void printRV() {
		printRV(revisionRoot);
	}

};

class SearchResultNode {
public:
	string title;
	int revisionNumber;
	SearchResultNode* next;
};

class Snapshot {
public:
	string name;
	ContentMetadata* rootCopy;
	Snapshot* next;
public:
	void displayInfo() {
		cout << "Snapshot : " << name << endl;
	}
};

class ContentManagementSystem {
public:
	static long timeVar;

	ContentMetadata* contentRoot;
	Snapshot* snapshots;

	ContentManagementSystem() {
		contentRoot = nullptr;
		snapshots = nullptr;
	}
	Revision* findLatest(Revision* root) {
		if (!root) return nullptr;

		while (root->right)
			root = root->right;

		return root;
	}

	void createContent(string title, string text) {
		contentRoot = createContent(contentRoot, title, text);
	}
	void addRevision(string title, string newText) {

		ContentMetadata* temp = findNode(contentRoot, title);

		if (temp) {
			temp->totalRevisions++;
			temp->createRevision(title, newText, timeVar++, temp->totalRevisions);

			temp->totalSize += newText.size();

			temp->lastModified = timeVar;
		}
		else {
			cout << "Content with title " << title << " not found." << endl;
		}
	}
	void deleteContent(string title) {
		contentRoot = deleteContent(contentRoot, title);
	}
	ContentMetadata* createContent(ContentMetadata* root, string title, string text) {

		if (root == nullptr) {
			ContentMetadata* temp = new ContentMetadata;

			temp->title = title;
			temp->totalRevisions = 1;
			temp->createRevision(title, text, timeVar++, 1);
			temp->totalSize = text.size();
			temp->lastModified = timeVar;

			return temp;
		}

		else if (title < root->title) {
			root->left = createContent(root->left, title, text);
		}

		else if (title > root->title) {
			root->right = createContent(root->right, title, text);
		}

		else {
			return root;
		}
		return root;
	}
	ContentMetadata* findNode(ContentMetadata* root, string title) {

		if (root == nullptr) {
			return nullptr;
		}
		if (root->title == title) {
			return root;
		}
		else if (title < root->title) {
			return findNode(root->left, title);
		}
		else {
			return findNode(root->right, title);
		}
	}
	void deleteRevisionTree(Revision* root) {

		if (root == nullptr) {
			return;
		}
		deleteRevisionTree(root->left);
		deleteRevisionTree(root->right);
		delete root;
	}
	ContentMetadata* deleteContent(ContentMetadata* root, string title) {

		if (root == nullptr) return root;

		if (title < root->title) {
			root->left = deleteContent(root->left, title);
		}
		else if (title > root->title) {
			root->right = deleteContent(root->right, title);
		}
		else {
			if (root->left == nullptr) {
				ContentMetadata* temp = root->right;

				deleteRevisionTree(root->revisionRoot);
				delete root;

				return temp;
			}
			else if (root->right == nullptr) {
				ContentMetadata* temp = root->left;

				deleteRevisionTree(root->revisionRoot);
				delete root;

				return temp;
			}

			ContentMetadata* temp = minValueNode(root->right);

			deleteRevisionTree(root->revisionRoot);

			root->title = temp->title;
			root->revisionRoot = copyRevisionTree(temp->revisionRoot);
			root->currentRevision = findLatest(root->revisionRoot);
			root->totalRevisions = temp->totalRevisions;
			root->lastModified = temp->lastModified;
			root->totalSize = temp->totalSize;

			root->right = deleteContent(root->right, temp->title);
		}

		return root;
	}
	ContentMetadata* minValueNode(ContentMetadata* node) {
		ContentMetadata* current = node;

		while (current && current->left != nullptr) {
			current = current->left;
		}
		return current;
	}
	void deleteContentTree(ContentMetadata* root) {

		if (root == nullptr) {
			return;
		}

		deleteContentTree(root->left);
		deleteContentTree(root->right);

		deleteRevisionTree(root->revisionRoot);

		delete root;
	}
	void analyzeContentHistory(string title) {

		ContentMetadata* temp = findNode(contentRoot, title);

		if (!temp) {
			cout << "No content with Title : " << title << " exist." << endl;
			return;
		}

		cout << "Revision History for: " << temp->title << endl;
		cout << "Total Revisions: " << temp->totalRevisions << endl;
		cout << "Total Storage Used: " << temp->totalSize << " bytes" << endl;

		cout << endl << "--- Revisions ---" << endl;

		int prevSize = -1;
		temp->printRevisionsInOrder(temp->revisionRoot, prevSize);

		cout << endl << "--- Balance Factor Analysis ---" << endl;
		temp->printBalanceFactors(temp->revisionRoot);
	}
	void createSnapshot(string snapshotName) {

		Snapshot* newSnap = new Snapshot;
		newSnap->next = nullptr;
		newSnap->name = snapshotName;

		newSnap->rootCopy = copyContentTree(contentRoot);

		if (snapshots == nullptr) {
			snapshots = newSnap;
		}
		else {
			newSnap->next = snapshots;
			snapshots = newSnap;
		}
	}
	void restoreSnapshot(string snapshotName) {
		Snapshot* curr = snapshots;
		while (curr != nullptr) {
			if (curr->name == snapshotName) {
				break;
			}
			curr = curr->next;
		}
		if (!curr) {
			cout << "Snapshot not found!" << endl;
			return;
		}
		deleteContentTree(contentRoot);
		contentRoot = copyContentTree(curr->rootCopy);
		cout << "Snapshot : " << snapshotName << " restored successfully" << endl;
	}
	void listSnapshots() {

		if (!snapshots) {
			cout << "No snapshots available" << endl;
			return;
		}

		Snapshot* temp = snapshots;

		cout << "============== Current Snapshots ==============" << endl;
		while (temp) {
			temp->displayInfo();
			temp = temp->next;
		}
	}
	void printRevisions(string title) {
		ContentMetadata* node = findNode(contentRoot, title);
		if (!node) {
			cout << "No Content with title : " << title << " was found" << endl;
			return;
		}
		node->printRV();
	}
	ContentMetadata* copyContentTree(ContentMetadata* root) {
		if (!root) return nullptr;

		ContentMetadata* temp = new ContentMetadata;

		temp->title = root->title;
		temp->totalRevisions = root->totalRevisions;
		temp->totalSize = root->totalSize;
		temp->lastModified = root->lastModified;

		temp->revisionRoot = copyRevisionTree(root->revisionRoot);

		temp->currentRevision = findLatest(temp->revisionRoot);

		temp->left = copyContentTree(root->left);
		temp->right = copyContentTree(root->right);

		return temp;
	}
	Revision* copyRevisionTree(Revision* root) {
		if (!root) return nullptr;

		Revision* temp = new Revision(root->timestamp);

		temp->title = root->title;
		temp->text = root->text;
		temp->size = root->size;
		temp->revisionNumber = root->revisionNumber;
		temp->height = root->height;

		temp->left = copyRevisionTree(root->left);
		temp->right = copyRevisionTree(root->right);

		return temp;
	}
	void exportContent(string title, int revisionNo, const string& outFileName) {
		ContentMetadata* temp = findNode(contentRoot, title);
		if (!temp) {
			cout << "The Content with title : " << title << " does not exist" << endl;
			return;
		}
		Revision* rev = findRevisionByNumber(temp->revisionRoot, revisionNo);
		if (!rev) {
			cout << "The revision No. " << revisionNo << " does not exist" << endl;
			return;
		}
		ofstream outFile(outFileName);
		if (!outFile) {
			cout << "File opening Error." << endl;
			return;
		}
		outFile << "Title: " << rev->title << endl;
		outFile << "Revision Number: " << rev->revisionNumber << endl;
		outFile << "Timestamp: " << rev->timestamp << endl;
		outFile << "Size: " << rev->size << endl;
		outFile << "---------------------------------\n";
		outFile << rev->text << endl;

		outFile.close();

		cout << "Content exported successfully to " << outFileName << endl;
	}
	Revision* findRevisionByNumber(Revision* root, int revNo) {
		if (!root) return nullptr;

		if (root->revisionNumber == revNo)
			return root;

		Revision* left = findRevisionByNumber(root->left, revNo);
		if (left) return left;

		return findRevisionByNumber(root->right, revNo);
	}
	Revision* getRevision(string title, long timestamp) {

		ContentMetadata* node = findNode(contentRoot, title);

		if (!node) {
			cout << "Content not found!" << endl;
			return nullptr;
		}

		return getRevisionHelper(node->revisionRoot, timestamp);
	}
	Revision* getRevisionHelper(Revision* root, long timestamp) {

		Revision* best = nullptr;

		while (root != nullptr) {

			if (root->timestamp == timestamp) {
				return root;
			}

			else if (timestamp < root->timestamp) {
				root = root->left;
			}

			else {
				best = root;
				root = root->right;
			}
		}

		return best;
	}
	SearchResultNode* searchText(string keyword) {

		SearchResultNode* head = nullptr;

		searchInContent(contentRoot, keyword, head);

		return head;
	}
	void searchInContent(ContentMetadata* root, string keyword, SearchResultNode*& head) {

		if (!root) return;

		searchInContent(root->left, keyword, head);

		searchInRevisions(root->revisionRoot, keyword, root->title, head);

		searchInContent(root->right, keyword, head);
	}
	void searchInRevisions(Revision* root, string keyword, string title, SearchResultNode*& head) {

		if (!root) return;

		searchInRevisions(root->left, keyword, title, head);

		if (root->text.find(keyword) != string::npos) {
			head = insertResult(head, title, root->revisionNumber);
		}

		searchInRevisions(root->right, keyword, title, head);
	}
	SearchResultNode* insertResult(SearchResultNode* head, string title, int revNo) {

		SearchResultNode* temp = new SearchResultNode;
		temp->title = title;
		temp->revisionNumber = revNo;
		temp->next = nullptr;

		if (!head) return temp;

		SearchResultNode* curr = head;
		while (curr->next)
			curr = curr->next;

		curr->next = temp;

		return head;
	}
	SearchResultNode* searchContentTitles(string pattern) {

		SearchResultNode* head = nullptr;

		searchTitlesHelper(contentRoot, pattern, head);

		return head;
	}
	void searchTitlesHelper(ContentMetadata* root, string pattern, SearchResultNode*& head) {

		if (!root) return;

		searchTitlesHelper(root->left, pattern, head);

		if (isMatch(root->title, pattern)) {
			head = insertResult(head, root->title, 0);
		}

		searchTitlesHelper(root->right, pattern, head);
	}
	bool isMatch(string title, string pattern) {
		title = toLower(title);
		pattern = toLower(pattern);

		return title.find(pattern) != string::npos;
	}
	string toLower(string s) {
		for (int i = 0; i < s.length(); i++) {
			if (s[i] >= 'A' && s[i] <= 'Z')
				s[i] = s[i] + 32;
		}
		return s;
	}
	long getLatestTimestamp(string title) {

		ContentMetadata* node = findNode(contentRoot, title);

		if (!node || !node->revisionRoot)
			return -1;

		Revision* curr = node->revisionRoot;

		while (curr->right)
			curr = curr->right;

		return curr->timestamp;
	}
	void saveToDisk(string filename) {

		ofstream out(filename);

		if (!out) {
			cout << "Error opening file!\n";
			return;
		}
		saveContent(contentRoot, out);

		out.close();

		cout << "Data saved successfully." << endl;
	}
	void saveContent(ContentMetadata* root, ofstream& out) {
		if (!root) return;

		saveContent(root->left, out);

		out << "CONTENT|" << root->title << "|"
			<< root->totalRevisions << "|"
			<< root->totalSize << "|"
			<< root->lastModified << endl;

		saveRevisions(root->revisionRoot, out);

		out << "ENDCONTENT" << endl;

		saveContent(root->right, out);
	}
	void saveRevisions(Revision* root, ofstream& out) {
		if (!root) return;

		saveRevisions(root->left, out);

		out << root->timestamp << "|"
			<< root->revisionNumber << "|"
			<< root->size << "|"
			<< root->text << endl;

		saveRevisions(root->right, out);
	}
	void systemAnalysis() {

		int totalContent = 0;
		int totalRevisions = 0;
		int totalStorage = 0;

		collectStats(contentRoot, totalContent, totalRevisions, totalStorage);

		cout << "============== System Analysis ==============" << endl;
		cout << "Total Content Items: " << totalContent << endl;
		cout << "Total Revisions: " << totalRevisions << endl;
		cout << "Total Storage Used: " << totalStorage << " bytes" << endl;
		cout << "Content BST Height: " << getContentHeight(contentRoot) << endl;

		analyzeAllTrees(contentRoot);
	}
	void collectStats(ContentMetadata* root, int& totalContent, int& totalRevisions, int& totalStorage) {
		if (!root) return;

		collectStats(root->left, totalContent, totalRevisions, totalStorage);

		totalContent++;
		totalRevisions += root->totalRevisions;
		totalStorage += root->totalSize;

		collectStats(root->right, totalContent, totalRevisions, totalStorage);
	}
	int getContentHeight(ContentMetadata* root) {
		if (!root) return 0;
		int leftH = getContentHeight(root->left);
		int rightH = getContentHeight(root->right);
		return 1 + (leftH > rightH ? leftH : rightH);
	}
	void analyzeAllTrees(ContentMetadata* root) {
		if (!root) return;

		analyzeAllTrees(root->left);

		int h = root->revisionRoot ? root->revisionRoot->height : 0;
		int bf = root->getBalanceFactor(root->revisionRoot);
		cout << "Content: " << root->title
			<< " | Revision BST Height: " << h
			<< " | Root BF: " << bf;

		if (bf > 1)
			cout << " | Left Skewed";
		else if (bf < -1)
			cout << " | Right Skewed";
		else
			cout << " | Balanced";

		cout << endl;

		analyzeAllTrees(root->right);
	}
	ContentMetadata* findContent(string title) {
		return findNode(contentRoot, title);
	}
	int getBalanceFactor(Revision* node) {
		if (!node) return 0;
		if (!contentRoot) return 0;
		return contentRoot->getBalanceFactor(node);
	}
	void loadFromFile(string filePath) {
		ifstream in(filePath);
		if (!in) {
			cout << "Error opening file!" << endl;
			return;
		}
		deleteContentTree(contentRoot);
		contentRoot = nullptr;

		string line;
		string currentTitle = "";
		int currentTotalRevisions = 0;

		while (getline(in, line)) {
			if (line.substr(0, 8) == "CONTENT|") {
				int p1 = 8;
				int p2 = line.find('|', p1);
				currentTitle = line.substr(p1, p2 - p1);
				int p3 = line.find('|', p2 + 1);
				currentTotalRevisions = stoi(line.substr(p2 + 1, p3 - p2 - 1));
			}
			else if (line == "ENDCONTENT") {
				currentTitle = "";
			}
			else if (currentTitle != "") {
				int p1 = line.find('|');
				int p2 = line.find('|', p1 + 1);
				int p3 = line.find('|', p2 + 1);
				if (p1 == string::npos) continue;
				long ts = stol(line.substr(0, p1));
				int revNum = stoi(line.substr(p1 + 1, p2 - p1 - 1));
				string text = line.substr(p3 + 1);

				ContentMetadata* node = findNode(contentRoot, currentTitle);
				if (!node) {
					contentRoot = createContent(contentRoot, currentTitle, text);
				}
				else {
					node->totalRevisions++;
					node->createRevision(currentTitle, text, ts, revNum);
					node->totalSize += text.size();
				}
			}
		}
		in.close();
		cout << "Data loaded successfully." << endl;
	}
	~ContentManagementSystem() {
		deleteContentTree(contentRoot);
		deleteSnaps();
	}
	void deleteSnaps() {
		Snapshot* curr = snapshots;

		while (curr != nullptr) {
			Snapshot* temp = curr;
			curr = curr->next;
			deleteContentTree(temp->rootCopy);
			delete temp;
		}

		snapshots = nullptr;
	}
};

long ContentManagementSystem::timeVar = 1;