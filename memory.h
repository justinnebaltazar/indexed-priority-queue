#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility> // for std::pair
using namespace std;

template <int initialCapacity = 10>
class IPQ {
public:
    IPQ() : size(0) {
        // make an empty IPQ
    }

    IPQ(const vector<string>& taskIDs, const vector<int>& priorities) {
        // construct IPQ from two vectors
        // IPQ is constructed such that ID v1[i] has priority v2[i]

        if (taskIDs.size() != priorities.size()) {
            cout << "Vectors must be the same size." << endl;
            return;
        }

        // initialize minHeap using the vectors
        minHeap = MinHeap(taskIDs, priorities);
        size = taskIDs.size();
       
    }

    void insert(const string& tid, int p) {
        // insert task ID tid with priority p
        minHeap.insert(tid, p);
        ++size;

        cout << "Inserted " << tid << " at index " << minHeap.tasksInPQ << endl; // Debug output
    }

    string& deleteMin() {
        // remove and return (a reference to) a task ID with smallest p
        
        if(isEmpty()) {
            static string emptyID; 
            cout << "Priority Queue is empty." << endl;
            return emptyID;
        }

        --size;
         string& minID = minHeap.deleteMin();
         cout << "IPQ: Just removed: " <<  minID << endl; // Debug output
        // string& minID = minHeap.deleteMin();
        
        // cout << "Just removed: " <<  minID << endl;
        
        return minID;
    }

    string& getMin() {
        // return (and a reference to) a task ID with the smallest priority
        if(isEmpty()) {
            cout << "Priority Queue is empty." << endl;
        }
        return minHeap.getMin();
    }

    void updatePriority(const string& tid, int p) {
        // change the priority for task tid to p
        if(minHeap.taskIndices.find(tid) == minHeap.taskIndices.end()) {
            cout << "Task ID not found." << endl;
        }
        int index = minHeap.taskIndices[tid];
        minHeap.updatePriority(tid, p);

        cout << "Updated priority of " << tid << " at index " << index << endl; // Debug output
    }

    void remove(const string& tid) {
        // remove the task with ID tid from the PQ
        if(minHeap.taskIndices.find(tid) == minHeap.taskIndices.end()) {
            cout << "Task ID not found. " << tid << " cannot be removed." << endl;
        }
        minHeap.remove(tid);
        --size;
    }

    bool isEmpty() {
        // return true if PQ is empty, otherwise false
        return size == 0;
    }

    int get_size() {
        return size;
    }

    void clear() {
        // remove all tasks from the PQ
        minHeap.clear();
        size = 0;
    }

    void reserve(int i) {
        // changes the heap array size to be max {i, 10}
        minHeap.reserve(i);
    }

    // DEBUG output
    string& getRoot() {
        if(isEmpty()) {
        cout << "Priority Queue is empty." << endl;
        }   
        return minHeap.heap[1].second;
    }

    void display() const {

        cout << "The current size of the priority queue is: " << size << endl;
        cout << "Current contents of the priority queue:" << endl;
        for (size_t i = 1; i <= minHeap.tasksInPQ; ++i) {
            cout << "Priority: " << minHeap.heap[i].first << ", Task ID: " << minHeap.heap[i].second << endl;
        }
    }

    
private:
    class MinHeap {
    public:
        pair<int, string>* heap;
        int capacity; // capacity of the heap array
        int size;
        int tasksInPQ; // number of tasks in the PQ
        unordered_map<string, int> taskIndices; // Map task IDs to heap node numbers

        MinHeap() : heap(nullptr), capacity(initialCapacity), size(0), tasksInPQ(0) {}

        MinHeap(const vector<string>& taskIDs, const vector<int>& priorities) {
            int size = taskIDs.size();
            heap = new pair<int, string>[size+1];

            if(size > capacity) {
                reserve(size);
            }

            tasksInPQ = size; 
            for(int i = 0; i < size; ++i) {
                heap[i + 1] = make_pair(priorities[i], taskIDs[i]); 
                taskIndices[taskIDs[i]] = i + 1; 
                // update map with initial indices 
            }
        
        }

        ~MinHeap() {
            delete[] heap;
            // free dynamically allocated memory
        }

        void insert(const string& tid, int p) {

            if(tasksInPQ >= capacity) {
                resize_heap();
            }

            heap[++tasksInPQ] = make_pair(p, tid);
            taskIndices[tid] = tasksInPQ;
            percolate_up(tasksInPQ);
            
            cout << "Inserted task: " << tid << " with priority: " << p << endl; // debug output
            ++size;
        }

        string& deleteMin() {
            if(isEmpty()) {
                static string emptyID; // new
                cout << "Priority Queue is empty." << endl;
                return emptyID; // new
            }
            --size;
            minID = heap[1].second; // get the task ID of the min element
            taskIndices.erase(minID); // remove min element from map
            cout << "MinHeap: Just removed: " <<  minID << endl; // Debug output
            
            heap[1] = heap[tasksInPQ--]; // move the last element to the root
            
            // update indices
            if (tasksInPQ > 0) {
                taskIndices[heap[1].second] = 1; // set back to 1
            }
        
            percolate_down(1);
            return minID;
        }

        string& getMin() {
            if(isEmpty()) {
                cout << "Priority Queue is empty." << endl;
            }

            return heap[1].second;
        }

        void updatePriority(const string& tid, int p) {
            int index = taskIndices[tid];
            int oldPriority = heap[index].first;
            heap[index].first = p;

            if(p < oldPriority) {
                percolate_up(index);
            } else {
                percolate_down(index);
            }
        }

        void remove(const string& tid) {
            int index = taskIndices[tid];
            heap[index] = heap[tasksInPQ--];
            taskIndices[heap[index].second] = index;
            percolate_down(index);
        }

        bool isEmpty() {
            return tasksInPQ == 0;
        }

        void clear() {
            tasksInPQ = 0;
        }

        void reserve(int i) {
            if (i > capacity) {
                capacity = i;
                pair<int, string>* newHeap = new pair<int, string>[capacity + 1];
                for(int j = 1; j <= tasksInPQ; ++j) {
                    newHeap[j] = heap[j];
                }
                delete[] heap;
                heap = newHeap;
            }
        }

        void make_heap(pair<int, string>* arr, int n, int i) {
            int smallest = i; 
            int leftChild = 2 * i; 
            int rightChild = 2 * i + 1; 

            if (leftChild <= n && arr[leftChild].first < arr[smallest].first) {
                smallest = leftChild;
            }

            if (rightChild <= n && arr[rightChild].first < arr[smallest].first) {
                smallest = rightChild;
            }

            if (smallest != i) {
                swap(arr[i], arr[smallest]);
                make_heap(arr, n, smallest);
            }
        }

    private:

        string minID;

        void percolate_up(int index) {
            if (index <= 1) return; // base case: stop when we reach the root
            int parent = index / 2;
            if (heap[index].first < heap[parent].first) {
                swap(heap[index], heap[parent]);

                // Update indices in the taskIndices map
                taskIndices[heap[index].second] = index;
                taskIndices[heap[parent].second] = parent;

                percolate_up(parent); // Recur to check the parent
            }
        }


        void percolate_down(int index) {
            int leftChild = 2 * index;
            int rightChild = 2 * index + 1;
            int smallest = index;

            if(leftChild <= tasksInPQ && heap[leftChild].first < heap[smallest].first) {
                smallest = leftChild;
            }

            if(rightChild <= tasksInPQ && heap[rightChild].first < heap[smallest].first) {
                smallest = rightChild;
            }

            if(smallest != index) {
                swap(heap[index], heap[smallest]);

                // Update indices in the taskIndices map
                taskIndices[heap[index].second] = index;
                taskIndices[heap[smallest].second] = smallest;

                percolate_down(smallest); // Recur to check the swapped child
            }
        }

        void resize_heap() {
            capacity *= 2;
            pair<int, string>* newHeap = new pair<int, string>[capacity + 1];
            for(int i = 1; i <= tasksInPQ; ++i) {
                newHeap[i] = heap[i];
            }

            delete[] heap;
            heap = newHeap;
        }
    };

    // data members for IPQ
    MinHeap minHeap; // Instance of MinHeap class
    int size;
};