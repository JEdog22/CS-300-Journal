
//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Jordan Epperson
// Version     : 1.0
// Description : ABCU Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

//============================================================================
// Course Structure
//============================================================================

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course() {
        courseNumber = "";
        courseTitle = "";
    }
};

//============================================================================
// Binary Search Tree Node
//============================================================================

struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) {
        course = aCourse;
        left = nullptr;
        right = nullptr;
    }
};

//============================================================================
// Binary Search Tree Class
//============================================================================

class BinarySearchTree {
private:
    Node* root;

    void addNode(Node* node, Course course) {

        if (course.courseNumber < node->course.courseNumber) {

            if (node->left == nullptr) {
                node->left = new Node(course);
            }
            else {
                addNode(node->left, course);
            }
        }
        else {

            if (node->right == nullptr) {
                node->right = new Node(course);
            }
            else {
                addNode(node->right, course);
            }
        }
    }

    void inOrder(Node* node) {

        if (node != nullptr) {

            inOrder(node->left);

            cout << node->course.courseNumber
                << ", "
                << node->course.courseTitle << endl;

            inOrder(node->right);
        }
    }

    Course search(Node* node, string courseNumber) {

        while (node != nullptr) {

            if (node->course.courseNumber == courseNumber) {
                return node->course;
            }

            if (courseNumber < node->course.courseNumber) {
                node = node->left;
            }
            else {
                node = node->right;
            }
        }

        return Course();
    }

public:

    BinarySearchTree() {
        root = nullptr;
    }

    void Insert(Course course) {

        if (root == nullptr) {
            root = new Node(course);
        }
        else {
            addNode(root, course);
        }
    }

    void PrintCourseList() {
        inOrder(root);
    }

    Course Search(string courseNumber) {
        return search(root, courseNumber);
    }
};

//============================================================================
// Load Courses From File
//============================================================================

void loadCourses(string fileName, BinarySearchTree* bst) {

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    string line;

    while (getline(file, line)) {

        stringstream ss(line);
        string token;

        Course course;

        // Course Number
        getline(ss, token, ',');
        course.courseNumber = token;

        // Course Title
        getline(ss, token, ',');
        course.courseTitle = token;

        // Remaining fields are prerequisites
        while (getline(ss, token, ',')) {
            course.prerequisites.push_back(token);
        }

        bst->Insert(course);
    }

    file.close();

    cout << "Courses loaded successfully." << endl;
}

//============================================================================
// Display Individual Course Information
//============================================================================

void printCourseInformation(BinarySearchTree* bst) {

    string courseNumber;

    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    transform(courseNumber.begin(),
        courseNumber.end(),
        courseNumber.begin(),
        ::toupper);

    Course course = bst->Search(courseNumber);

    if (course.courseNumber.empty()) {

        cout << "Course not found." << endl;
        return;
    }

    cout << endl;
    cout << course.courseNumber << ", "
        << course.courseTitle << endl;

    if (course.prerequisites.empty()) {

        cout << "Prerequisites: None" << endl;
    }
    else {

        cout << "Prerequisites: ";

        for (unsigned int i = 0; i < course.prerequisites.size(); i++) {

            cout << course.prerequisites[i];

            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

//============================================================================
// Main
//============================================================================

int main() {

    BinarySearchTree bst;

    int choice = 0;
    string fileName;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {

        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
		}

        switch (choice) {

        case 1:

            cout << "Enter file name: ";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			getline(cin, fileName);

            loadCourses(fileName, &bst);
            break;

        case 2:

            cout << endl;
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            bst.PrintCourseList();
            break;

        case 3:

            printCourseInformation(&bst);
            break;

        case 9:

            cout << "Thank you for using the course planner!" << endl;
            break;

        default:

            cout << choice
                << " is not a valid option."
                << endl;
        }
    }

    return 0;
}

