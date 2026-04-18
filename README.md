# 📂 Digital Content Management System

A fully functional Digital Content Management System built in C++ using **Binary Search Trees (BST)** implemented entirely from scratch with dynamic node-based data structures. 

**Zero STL containers. Zero static arrays.**

This project was developed as a Data Structures assignment to demonstrate the real-world application of BSTs, tree traversal, manual memory management, and recursive algorithms.

---

## 📖 What This Project Does
This system simulates how a large digital media organization might manage thousands of content items—such as articles, blogs, scripts, and research drafts—each with a complete and meticulously tracked revision history.

* **Global Indexing:** All content items are stored in a global BST, ordered alphabetically by title.
* **Version Control:** Every individual content item maintains its *own* BST of revisions, ordered sequentially by timestamp.

The system supports robust searching, deep-copy snapshotting, data exporting, and structural analysis of the underlying trees over time.

---

## ✨ Features

### Content Management
* Create new content items with an initial revision.
* Add multiple revisions to any existing content item.
* Delete content and safely free all associated dynamic memory.
* Duplicate title detection (silently ignores duplicates).

### Revision System
* Each content item owns a BST of its revisions keyed by a unique timestamp.
* Retrieve a revision by an exact timestamp or the closest earlier timestamp.
* Revision numbers are strictly tracked and monotonically increasing per item.

### Search Capabilities
* **Title Search:** Find content titles using partial, case-insensitive pattern matching.
* **Deep Search:** Search inside revision text across *all* content items by keyword.
* **Exact Lookup:** Pull up any content item instantly by its exact title.

### Snapshot & State Management
* Take a full deep-copy snapshot of the entire system state at any point in time.
* Restore the system to any previously saved snapshot.
* View a listed history of all saved snapshots.

### Deep Analysis & Analytics
* Per-content revision history with size change tracking (+/- bytes).
* Balance Factor analysis for every single node in every revision BST.
* System-wide statistics: total content count, total revisions, total storage used, BST heights, and skew detection.

### Import / Export
* Export any specific revision directly to a `.txt` file.
* Save the entire system state to your local disk.
* Load a previously saved system state from a file.

### Interface
* Fully interactive terminal menu featuring categorized sub-menus for straightforward navigation.

---

## 🏗️ Architecture & Data Structures

All structures are manually implemented using dynamic memory allocation. 

| Structure | Purpose |
| :--- | :--- |
| **BST** *(ContentMetadata nodes)* | Global index of all content items, keyed by title. |
| **BST** *(Revision nodes)* | Per-content revision history, keyed by timestamp. |
| **Linked List** *(Snapshot)* | Stores full system snapshots in a singly linked list. |
| **Linked List** *(SearchResultNode)* | Captures and returns search results. |

---

## 📁 Project Structure

```text
├── Header.h       # All class definitions and implementations
├── main.cpp       # Menu-driven driver program
└── Tests.cpp      # Google Test unit tests
Core Classes
Revision — A single revision node storing text, timestamp, size, revision number, and BST pointers.

ContentMetadata — Stores metadata for a specific content item and owns its internal revision BST.

ContentManagementSystem — The primary system class handling all global operations.

Snapshot — A deep copy of the entire global content tree captured at a specific point in time.

SearchResultNode — A linked list node utilized for returning complex search operations.

🚀 How to Run
1. Run the Main Application
Compile and run the menu-driven program using g++:

Bash
g++ main.cpp -o cms
./cms
2. Run the Unit Tests
Compile and run the Google Test suite:

Bash
g++ Tests.cpp -o tests -lgtest -lgtest_main -pthread
./tests
(Note: These tests were verified using Visual Studio with the Google Test framework.)

🧠 Concepts Demonstrated
Binary Search Tree insertion, search, and deletion algorithms.

Recursive BST traversal (Inorder, Preorder, Postorder).

Deep copying of complex, nested tree structures (for snapshot functionality).

Balance Factor computation (without relying on AVL self-rotations).

Manual memory management utilizing custom destructors and recursive cleanup.

File I/O operations for persistent data storage.

⚠️ Important Notes
Strictly No STL: No Standard Template Library containers (std::vector, std::map, std::list, etc.) were used anywhere in this project.

Fully Dynamic: No static arrays are used. All data structures are entirely dynamic and node-based.

Timestamps: Timestamps are managed as globally unique and monotonically increasing values using a static counter
