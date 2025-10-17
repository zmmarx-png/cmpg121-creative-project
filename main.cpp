// Zandrie Marx
// 55838294
// Creative project

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Question {
    string room;
    string text;
    string optionA;
    string optionB;
    string optionC;
    char correctAnswer;
};

void loadQuestions(const string& filename, vector<Question>& questions) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        Question q;

        // Check for "Room: " line or assign default
        if (line.size() >= 6 && line.substr(0, 6) == "Room: ") {
            q.room = line.substr(6);
            if (!getline(file, q.text)) continue;
        } else if (!line.empty()) {
            q.room = "Unknown Chamber";
            q.text = line;
        } else {
            continue; // Skip empty lines
        }

        // Load options and answer
        if (!getline(file, q.optionA) ||
            !getline(file, q.optionB) ||
            !getline(file, q.optionC) ||
            !getline(file, line)) {
            cerr << "Incomplete question block in " << filename << ". Skipping...\n";
            continue;
        }

        if (line.size() >= 8 && line.substr(0, 7) == "Answer:") {
            q.correctAnswer = line.back();
            questions.push_back(q);
        } else {
            cerr << "Malformed answer line: \"" << line << "\". Skipping...\n";
        }
    }

    file.close();
}

void playLevel(const vector<Question>& questions, int& score, const string& levelName) {
    cout << "\nEntering " << levelName << "...\n";
    for (const Question& q : questions) {
        cout << "\n[" << q.room << "] " << q.text << endl;
        cout << q.optionA << endl;
        cout << q.optionB << endl;
        cout << q.optionC << endl;
        // cout << "Your answer (A/B/C): ";
        char answer;
        // cin >> answer;

    do {
            cout << "Your answer (A/B/C): ";
            cin >> answer;
            answer = toupper(answer);
            if (answer != 'A' && answer != 'B' && answer != 'C') {
                cout << "\nInvalid input. Please enter A, B, or C.\n";
            }
     } while (answer != 'A' && answer != 'B' && answer != 'C');

        if (toupper(answer) == q.correctAnswer) {
            cout << "\nCorrect! +10 points\n";
            score += 10;
        } else {
            cout << "\nIncorrect. Correct answer: " << q.correctAnswer << "\n";
        }
    }
}

void saveProgress(const string& filename, const string& playerName, int score) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "Could not write to file: " << filename << endl;
        return;
    }
    file << playerName << " completed the quest with " << score << " points.\n";
    file.close();
}

int main() {
    vector<Question> level1, level2, level3, boss;
    string playerName;
    int score = 0;

    cout << "Welcome to Chronicles of Chemistry: Quest for the Elemental Crown!\n";
    cout << "Enter your alchemist name: ";
    getline(cin, playerName);

    loadQuestions("level1.txt", level1);
    loadQuestions("level2.txt", level2);
    loadQuestions("level3.txt", level3);
    loadQuestions("boss.txt", boss);

    if (level1.empty() || level2.empty() || level3.empty() || boss.empty()) {
        cerr << "One or more levels failed to load. Please check your files.\n";
        return 1;
    }

    playLevel(level1, score, "Chamber of Atoms");
    playLevel(level2, score, "Chamber of Reactions");
    playLevel(level3, score, "Chamber of Organic Shadows");

    cout << "\nYou have reached the final battle: The Bond Master!\n";
    playLevel(boss, score, "Boss Battle");

    cout << "\nFinal Score: " << score << "\n";
    saveProgress("progress.txt", playerName, score);
    cout << "Progress saved. Farewell, " << playerName << "!\n";

    return 0;
}
