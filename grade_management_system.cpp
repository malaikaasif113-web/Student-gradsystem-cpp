#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// ==================== STUDENT CLASS ====================
class Student {
private:
    string name;
    int rollNumber;
    vector<double> marks;
    double totalMarks;
    double averageMarks;
    char letterGrade;

public:
    Student() {
        name = "";
        rollNumber = 0;
        totalMarks = 0.0;
        averageMarks = 0.0;
        letterGrade = 'F';
    }

    Student(string studentName, int roll) {
        name = studentName;
        rollNumber = roll;
        totalMarks = 0.0;
        averageMarks = 0.0;
        letterGrade = 'F';
    }
    
    void addMark(double mark) {
        if (mark >= 0 && mark <= 100) {
            marks.push_back(mark);
            updateAllStats();
        } else {
            cout << "Invalid mark! Marks should be between 0 and 100.\n";
        }
    }
    
    void calculateTotal() {
        totalMarks = accumulate(marks.begin(), marks.end(), 0.0);
    }
    
    void calculateAverage() {
        if (!marks.empty()) {
            averageMarks = totalMarks / marks.size();
        } else {
            averageMarks = 0.0;
        }
    }
    
    char calculateLetterGrade() {
        if (averageMarks >= 90) return 'A';
        else if (averageMarks >= 80) return 'B';
        else if (averageMarks >= 70) return 'C';
        else if (averageMarks >= 60) return 'D';
        else return 'F';
    }
    
    void updateAllStats() {
        calculateTotal();
        calculateAverage();
        letterGrade = calculateLetterGrade();
    }
    
    // Getters
    string getName() const { return name; }
    int getRollNumber() const { return rollNumber; }
    vector<double> getMarks() const { return marks; }
    double getTotalMarks() const { return totalMarks; }
    double getAverageMarks() const { return averageMarks; }
    char getLetterGrade() const { return letterGrade; }
    
    // Setters
    void setName(string studentName) { name = studentName; }
    void setRollNumber(int roll) { rollNumber = roll; }
    
    void displayStudent() const {
        cout << "Roll No: " << rollNumber 
             << " | Name: " << name
             << " | Total: " << totalMarks
             << " | Average: " << fixed << setprecision(2) << averageMarks
             << " | Grade: " << letterGrade << endl;
    }
};

// ==================== GRADE MANAGER CLASS ====================
class GradeManager {
private:
    vector<Student> students;
    int numberOfSubjects;

public:
    GradeManager() : numberOfSubjects(0) {}
    
    GradeManager(int subjects) : numberOfSubjects(subjects) {}
    
    void addStudent(const Student& student) {
        students.push_back(student);
    }
    
    void inputAllStudentData() {
        int numStudents;
        cout << "\n========== INPUT STUDENT DATA ==========\n";
        cout << "Enter number of students: ";
        cin >> numStudents;
        
        for (int i = 0; i < numStudents; i++) {
            cout << "\n--- Student " << (i + 1) << " ---\n";
            string name;
            int roll;
            
            cout << "Enter student name: ";
            cin.ignore();
            getline(cin, name);
            
            cout << "Enter roll number: ";
            cin >> roll;
            
            Student newStudent(name, roll);
            
            cout << "Enter marks for " << numberOfSubjects << " subjects:\n";
            for (int j = 0; j < numberOfSubjects; j++) {
                double mark;
                do {
                    cout << "Subject " << (j + 1) << " (0-100): ";
                    cin >> mark;
                    if (mark < 0 || mark > 100) {
                        cout << "Invalid! Please enter marks between 0 and 100.\n";
                    }
                } while (mark < 0 || mark > 100);
                newStudent.addMark(mark);
            }
            
            students.push_back(newStudent);
            cout << "✓ Student added successfully!\n";
        }
    }
    
    double calculateClassAverage() const {
        if (students.empty()) return 0.0;
        
        double total = 0.0;
        for (const auto& student : students) {
            total += student.getTotalMarks();
        }
        return total / students.size();
    }
    
    double findHighestMarks() const {
        if (students.empty()) return 0.0;
        
        double highest = students[0].getTotalMarks();
        for (const auto& student : students) {
            if (student.getTotalMarks() > highest) {
                highest = student.getTotalMarks();
            }
        }
        return highest;
    }
    
    double findLowestMarks() const {
        if (students.empty()) return 0.0;
        
        double lowest = students[0].getTotalMarks();
        for (const auto& student : students) {
            if (student.getTotalMarks() < lowest) {
                lowest = student.getTotalMarks();
            }
        }
        return lowest;
    }
    
    double calculateStandardDeviation() const {
        if (students.size() < 2) return 0.0;
        
        double mean = calculateClassAverage();
        double sumSquaredDiff = 0.0;
        
        for (const auto& student : students) {
            double diff = student.getTotalMarks() - mean;
            sumSquaredDiff += diff * diff;
        }
        
        return sqrt(sumSquaredDiff / students.size());
    }
    
    void displayAllStudents() const {
        cout << "\n========== STUDENT GRADE REPORT ==========\n";
        cout << left << setw(10) << "Roll No"
             << setw(20) << "Name"
             << setw(10) << "Total"
             << setw(12) << "Average"
             << setw(8) << "Grade" << endl;
        cout << string(60, '-') << endl;
        
        for (const auto& student : students) {
            cout << left << setw(10) << student.getRollNumber()
                 << setw(20) << student.getName()
                 << setw(10) << student.getTotalMarks()
                 << setw(12) << fixed << setprecision(2) << student.getAverageMarks()
                 << setw(8) << student.getLetterGrade() << endl;
        }
    }
    
    void sortStudentsByTotalMarks() {
        sort(students.begin(), students.end(), 
            [](const Student& a, const Student& b) {
                return a.getTotalMarks() > b.getTotalMarks();
            });
        
        cout << "\n✓ Students sorted by total marks (highest first)!\n";
        displayAllStudents();
    }
    
    void displayStudentsBelowThreshold(double passingMarks) const {
        vector<Student> belowThreshold;
        
        for (const auto& student : students) {
            if (student.getTotalMarks() < passingMarks) {
                belowThreshold.push_back(student);
            }
        }
        
        if (belowThreshold.empty()) {
            cout << "\n✨ Great news! All students are above the passing threshold!\n";
        } else {
            cout << "\n⚠️  STUDENTS BELOW PASSING THRESHOLD (" << passingMarks << " marks):\n";
            cout << string(50, '-') << endl;
            for (const auto& student : belowThreshold) {
                cout << "Roll " << student.getRollNumber() 
                     << " | " << student.getName()
                     << " | Total: " << student.getTotalMarks()
                     << " | Grade: " << student.getLetterGrade() << endl;
            }
            cout << "\n📚 " << belowThreshold.size() << " student(s) need improvement.\n";
        }
    }
    
    void generateGradeReport() const {
        cout << "\n========== COMPLETE STATISTICAL REPORT ==========\n";
        cout << fixed << setprecision(2);
        cout << "📊 Number of Students: " << students.size() << endl;
        cout << "📚 Number of Subjects: " << numberOfSubjects << endl;
        cout << "⭐ Class Average: " << calculateClassAverage() << endl;
        cout << "🏆 Highest Marks: " << findHighestMarks() << endl;
        cout << "📉 Lowest Marks: " << findLowestMarks() << endl;
        cout << "📈 Standard Deviation: " << calculateStandardDeviation() << endl;
        
        // Grade distribution
        int gradeACount = 0, gradeBCount = 0, gradeCCount = 0, gradeDCount = 0, gradeFCount = 0;
        for (const auto& student : students) {
            switch (student.getLetterGrade()) {
                case 'A': gradeACount++; break;
                case 'B': gradeBCount++; break;
                case 'C': gradeCCount++; break;
                case 'D': gradeDCount++; break;
                case 'F': gradeFCount++; break;
            }
        }
        
        cout << "\n📊 GRADE DISTRIBUTION:\n";
        cout << "A (90-100): " << gradeACount << " student(s) 🌟\n";
        cout << "B (80-89):  " << gradeBCount << " student(s) 👍\n";
        cout << "C (70-79):  " << gradeCCount << " student(s) 📘\n";
        cout << "D (60-69):  " << gradeDCount << " student(s) ⚠️\n";
        cout << "F (0-59):   " << gradeFCount << " student(s) ❌\n";
    }
    
    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            cout << "Error opening file for writing!\n";
            return;
        }
        
        outFile << numberOfSubjects << endl;
        outFile << students.size() << endl;
        
        for (const auto& student : students) {
            outFile << student.getRollNumber() << endl;
            outFile << student.getName() << endl;
            
            auto marks = student.getMarks();
            for (double mark : marks) {
                outFile << mark << " ";
            }
            outFile << endl;
        }
        
        cout << "✓ Data saved to " << filename << " successfully!\n";
        outFile.close();
    }
    
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            cout << "No existing data found. Starting with empty database.\n";
            return;
        }
        
        students.clear();
        inFile >> numberOfSubjects;
        
        int numStudents;
        inFile >> numStudents;
        
        for (int i = 0; i < numStudents; i++) {
            int roll;
            string name;
            
            inFile >> roll;
            inFile.ignore();
            getline(inFile, name);
            
            Student student(name, roll);
            
            for (int j = 0; j < numberOfSubjects; j++) {
                double mark;
                inFile >> mark;
                student.addMark(mark);
            }
            
            students.push_back(student);
        }
        
        cout << "✓ Data loaded from " << filename << " successfully!\n";
        inFile.close();
    }
    
    int getNumberOfSubjects() const { return numberOfSubjects; }
    bool hasStudents() const { return !students.empty(); }
};

// ==================== UTILITY FUNCTIONS ====================
void displayMenu() {
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║   STUDENT GRADE MANAGEMENT SYSTEM      ║\n";
    cout << "╠════════════════════════════════════════╣\n";
    cout << "║  1. Add Student Data                   ║\n";
    cout << "║  2. Display All Students               ║\n";
    cout << "║  3. Sort by Total Marks                ║\n";
    cout << "║  4. Show Statistical Report            ║\n";
    cout << "║  5. Find Students Below Threshold      ║\n";
    cout << "║  6. Save Data to File                  ║\n";
    cout << "║  7. Load Data from File                ║\n";
    cout << "║  8. Exit                               ║\n";
    cout << "╚════════════════════════════════════════╝\n";
    cout << "Enter your choice: ";
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// ==================== MAIN FUNCTION ====================
int main() {
    cout << "========================================\n";
    cout << "  WELCOME TO GRADE MANAGEMENT SYSTEM   \n";
    cout << "========================================\n";
    
    int numSubjects;
    cout << "How many subjects do you want to track? ";
    cin >> numSubjects;
    
    GradeManager manager(numSubjects);
    
    // Try to load existing data
    manager.loadFromFile("student_data.txt");
    
    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                manager.inputAllStudentData();
                break;
                
            case 2:
                if (manager.hasStudents()) {
                    manager.displayAllStudents();
                } else {
                    cout << "\n⚠️ No students found! Please add student data first.\n";
                }
                break;
                
            case 3:
                if (manager.hasStudents()) {
                    manager.sortStudentsByTotalMarks();
                } else {
                    cout << "\n⚠️ No students found! Please add student data first.\n";
                }
                break;
                
            case 4:
                if (manager.hasStudents()) {
                    manager.generateGradeReport();
                } else {
                    cout << "\n⚠️ No students found! Please add student data first.\n";
                }
                break;
                
            case 5: {
                if (manager.hasStudents()) {
                    double threshold;
                    cout << "Enter passing threshold (total marks): ";
                    cin >> threshold;
                    manager.displayStudentsBelowThreshold(threshold);
                } else {
                    cout << "\n⚠️ No students found! Please add student data first.\n";
                }
                break;
            }
                
            case 6:
                if (manager.hasStudents()) {
                    manager.saveToFile("student_data.txt");
                } else {
                    cout << "\n⚠️ No data to save! Please add student data first.\n";
                }
                break;
                
            case 7:
                manager.loadFromFile("student_data.txt");
                break;
                
            case 8:
                cout << "\n✨ Thank you for using the system! Goodbye! 👋\n";
                break;
                
            default:
                cout << "\n❌ Invalid choice! Please try again.\n";
        }
        
        if (choice != 8) {
            waitForEnter();
            clearScreen();
        }
        
    } while (choice != 8);
    
    return 0;
}
