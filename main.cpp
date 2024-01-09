#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>


using namespace std;
using namespace sql;

int displayMenu();
void listTasks(sql::Connection *con, const string &filterStatus= "not_ready");
void addTask(sql::Connection *con, const string &task, const string &status = "not_ready");
void deleteTask(sql::Connection *con, int taskId);
void updateTaskStatus(sql::Connection *con, int taskId, const string &newStatus);


int main() {

    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        /* Create a connection */
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "");

        /* Connect to the MySQL test database */
        con->setSchema("todo_app");

        sql::Statement *stmt;
        sql::ResultSet *res;

        vector<string> tasks;
        string userInput;
        int choice;
        string task;
        int taskId; // For delete option
        int idToUpdate;
        string newStatus;

        while (true) {
        
        choice = displayMenu();

        // Handle the user's choice
        switch (choice) {
            case 1:
                listTasks(con, "not_ready");
                break;
            case 2:
                listTasks(con, "");
                break;
            case 3:
                listTasks(con, "ready");
                break;
            case 4:
                std::cout << "Enter new task description: ";
                cin.ignore();  // Clear newline left in the input stream
                getline(cin, task);  // Use getline to handle spaces in task
                addTask(con, task);
                break;
            case 5:
                std::cout << "Enter ID of the task to delete: ";
                cin >> taskId;
                deleteTask(con, taskId);
                break;
            case 6:
                cout << "Enter ID of the task to update: ";
                cin >> idToUpdate;
                cin.ignore(); // Clear newline left in the input stream

                cout << "Enter new status (not_ready/ready): ";
                getline(cin, newStatus); // Get the entire line for status

                updateTaskStatus(con, idToUpdate, newStatus);
                break;
            case 7: {
              
                cout << "Goodbye!" << endl;
                break;
            break;
        }
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice == 6) {
            break;  // Break out of the while loop
        }
    }


    delete con;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
    //Black: \033[30m
    // Red: \033[31m
    // Green: \033[32m
    // Yellow: \033[33m
    // Blue: \033[34m
    // Magenta: \033[35m
    // Cyan: \033[36m
    // White: \033[37m

    return 0;
}

void addTask(sql::Connection *con, const string &task, const string &status) {
    try {
        // Prepare a SQL statement to insert data
        sql::PreparedStatement *pstmt;

        pstmt = con->prepareStatement("INSERT INTO tasks(task, status) VALUES (?, ?)");

        // Bind parameters
        pstmt->setString(1, task);  // bind the task parameter
        pstmt->setString(2, status); // bind the status parameter, default 'not_ready'

        // Execute the statement
        pstmt->executeUpdate();

        delete pstmt; // Clean up the statement
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}


void listTasks(sql::Connection *con, const string &filterStatus) {
    try {
        sql::Statement *stmt;
        sql::ResultSet *res;
        stmt = con->createStatement();

        string query = "SELECT * FROM tasks";
        if (!filterStatus.empty()) {
            query += " WHERE status = '" + filterStatus + "'";
        }

        res = stmt->executeQuery(query);

        while (res->next()) {

            string colorCode;

            if(res->getString("status") == "not_ready"){
                colorCode = "\033[33m";
            } else if(res->getString("status") == "ready"){
                colorCode = "\033[32m";
            } else {
                colorCode = "\033[37m";
            }

            cout << colorCode;
            cout << "(";
            cout << res->getString("id");
            cout << ") ";
            cout << res->getString("task");
            cout << " -> ";
            cout << res->getString("status");
            cout << endl;

        }

        cout << "\033[37m";

        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}


void deleteTask(sql::Connection *con, int taskId) {
    try {
        sql::PreparedStatement *pstmt;

        // Prepare the SQL delete statement
        pstmt = con->prepareStatement("DELETE FROM tasks WHERE id = ?");

        // Bind the task ID to the parameter in the prepared statement
        pstmt->setInt(1, taskId);

        // Execute the delete statement
        pstmt->executeUpdate();

        delete pstmt; // Clean up the statement
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void updateTaskStatus(sql::Connection *con, int taskId, const string &newStatus) {
    try {
        sql::PreparedStatement *pstmt;

        // Prepare the SQL update statement
        pstmt = con->prepareStatement("UPDATE tasks SET status = ? WHERE id = ?");

        // Bind the new status and task ID to the parameters in the prepared statement
        pstmt->setString(1, newStatus);
        pstmt->setInt(2, taskId);

        // Execute the update statement
        pstmt->executeUpdate();

        cout << "Task status updated successfully." << endl;

        delete pstmt; // Clean up the statement
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}


int displayMenu() {
    int choice;

    // Display the menu
    cout << "\nMenu:" << endl;
    cout << "1. List not ready tasks" << endl;
    cout << "2. List all tasks" << endl;
    cout << "3. List ready tasks" << endl;
    cout << "4. Create new task" << endl;
    cout << "5. Delete task" << endl;
    cout << "6. Update task status" << endl;
    cout << "7. Exit" << endl;

    // Prompt the user for their choice
    cout << "Enter your choice: ";
    cin >> choice;

    // Check if the input was valid
    if (cin.fail()) {
        cin.clear();              // Clear the error state
        cin.ignore(1000, '\n');   // Ignore up to 1000 characters or until a newline
        cout << "Invalid input. Please try again.\n";
        return -1; // Return an invalid choice
    }

    return choice;
}

