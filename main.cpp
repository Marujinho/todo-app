#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {

    vector<string> tasks;
    string userInput;
    int choice;
    string task;

    while (true) {
        // Display the menu
        cout << "Menu:" << endl;
        cout << "1. Listar as tarefas de hoje" << endl;
        cout << "2. Incluir tarefa nova" << endl;
        cout << "3. Sair" << endl;


        // Prompt the user for their choice
        cout << "Enter your choice: ";
        cin >> choice;

         // Check if the input was valid
        if (cin.fail()) {
            cin.clear();              // Clear the error state
            cin.ignore(1000, '\n');   // Ignore up to 1000 characters or until a newline
            cout << "Invalid input. Please try again." << endl;
            continue;
        }

        // Handle the user's choice
        switch (choice) {
            case 1:
                cout << "You selected Option 1." << endl;
                break;
            case 2:
                cout << "Insira uma tarefa nova: " << endl;
                cin >> task;
                tasks.push_back(task);
                cout << "Tarefa inserida com sucesso! " << endl;
                break;
            case 3:
                cout << "Goodbye!" << endl;
                return 0; // Exit the program
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }


    // cout << "Enter a task: ";
    // getline(cin, userInput);

    // tasks.push_back(userInput);
    
    cout << "Tasks: ";
    for (const string& str : tasks) {
        cout << str << "  ";
    }
    cout << endl;

    // Your code goes here
    return 0;
}