#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "IPQ.h"
using namespace std;

int main() {

    vector<string> taskIDs;
    taskIDs.push_back("Task1");
    taskIDs.push_back("Task2");
    taskIDs.push_back("Task3");
    taskIDs.push_back("Task4");
    taskIDs.push_back("Task5");

    vector<int> priorities;
    priorities.push_back(5);
    priorities.push_back(3);
    priorities.push_back(7);
    priorities.push_back(2);
    priorities.push_back(6);


    IPQ<> ipq(taskIDs, priorities);

    // show the task IDs and priorities are being inserted properly
    cout << "Initial IPQ: " << endl;
    ipq.display();

    cout << "\nIs the IPQ empty? " << (ipq.isEmpty()? "Yes" : "No") << endl; 

    // show getMin returns correct value
    cout << "Root Task: " << ipq.getMin() << endl; 

    ipq.insert("Task10", 9);
    ipq.insert("Task7", 1);
    ipq.insert("Task6", 4);

    cout << endl;
    ipq.updatePriority("Task1", 8);
    ipq.updatePriority("Task7", 15); // update priority at root
    cout << endl;
    ipq.display();
    cout << endl;
    
    ipq.insert("Task3", 16); // duplicate taskIDs are not inserted
    ipq.insert("Task12", 4); // duplicate priorities is allowed 
    ipq.insert("Task13", 10);
    ipq.insert("Task8", 7);
    ipq.insert("Task9", 11); 
    cout << "Root Task: " << ipq.getMin() << endl; // root Task4
    cout << "After four insertions, the current size of the IPQ: " << ipq.get_size() << endl;

    ipq.remove("Task3");
    ipq.remove("Task4");
    cout << "\nAfter removing Task3 and Task4: " << endl; 
    ipq.display();

    cout << "\nRoot Task: after removals " << ipq.getMin() << endl;

    // make sure removing in the correct order
    while (!ipq.isEmpty()) {
    
        string minTask = ipq.deleteMin();
        if(!ipq.isEmpty()) {
            cout << "Just removed: " << minTask << endl;
        }
    }

    // test the clear function
    IPQ<> ipq2(taskIDs, priorities);
    cout << endl; 
    ipq2.display();
    ipq2.clear();
    cout << "After clear operation, is the IPQ empty? " << (ipq2.isEmpty()? "Yes" : "No") << endl; 

    return 0;
}