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

        if(taskIDs.size() != priorities.size()) {
            cout << "Vectors must be the same size." << endl;
            return;
        }

        minHeap.build_heap(taskIDs, priorities);
        size = taskIDs.size();
    }

    void insert(const string& tid, int p) {
        // insert task ID tid with priority p
        minHeap.insert(tid, p);
        ++size;

    }

    string& deleteMin() {
        // remove and return (a reference to) a task ID with smallest p
        
        if(isEmpty()) {
            static string emptyID; 
            cout << "Priority Queue is empty." << endl;
            return emptyID;
        }

        --size;
        // call deleteMin in to remove minID in the heap
         string& minID = minHeap.deleteMin();
        
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

    void display() const {
        // prints out the size and contents 
        cout << "Size of the IPQ: " << size << endl;
        cout << "Contents of the IPQ: " << endl;
        for (int i = 1; i <= minHeap.tasksInPQ; ++i) {
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

        MinHeap() : heap(new pair<int, string>[initialCapacity + 1]), capacity(initialCapacity), size(0), tasksInPQ(0) {}

        ~MinHeap() {
            delete[] heap;
            // free dynamically allocated memory
        }

        void insert(const string& tid, int p) {
            if (taskIndices.find(tid) != taskIndices.end()) {
                cout << "Task ID " << tid << " already exists. Cannot insert duplicate." << endl;
                return;
            }

            // check if the heap is full
            if (tasksInPQ >= capacity) {
                resize_heap();
            }

            heap[++tasksInPQ] = make_pair(p, tid); // insert the new taskID and priority into heap
            taskIndices[tid] = tasksInPQ; // insert taskID index in the map
            percolate_up(tasksInPQ);
            // call percolate_up to restore heap properties
            
            ++size;
        }

        string& deleteMin() {
            if(isEmpty()) {
                static string emptyID; 
                cout << "Priority Queue is empty." << endl;
                return emptyID; 
            }

            --size;
            minID = heap[1].second; // get the task ID of the min element
            taskIndices.erase(minID); // remove min element from map
            
            // move the last element to the root
            heap[1] = heap[tasksInPQ--]; 
            
            // update indices
            if (tasksInPQ > 0) {
                taskIndices[heap[1].second] = 1; // set back to 1
            }

            // restore heap property
            percolate_down(1);
            return minID;
        }

        string& getMin() {
            if(isEmpty()) {
                cout << "Priority Queue is empty." << endl;
            }
            // return the priority associated with first element in the heap
            return heap[1].second;
        }

        void updatePriority(const string& tid, int p) {
            // get the task ID index in the heap 
            int index = taskIndices[tid];
            // store old priority
            int oldPriority = heap[index].first;
            // update priority of the task at the specified index
            heap[index].first = p;

            // check if new priority is less than the old priority
            if(p < oldPriority) {
                percolate_up(index); 
            } else {
                percolate_down(index); 
            }
        }

        void remove(const string& tid) {
            // get the index of the task id
            int index = taskIndices[tid];
            // remove from the heap and update map
            heap[index] = heap[tasksInPQ--];
            taskIndices[heap[index].second] = index;
            percolate_down(index);
        }

        bool isEmpty() {
            // check if the heap is empty
            return tasksInPQ == 0;
        }

        void clear() {
            // free memory allocated for the heap array
            delete[] heap; 
            heap = nullptr;
            capacity = initialCapacity; 

            // reset size and tasksInPQ to 0
            size = 0; 
            tasksInPQ = 0; 
            taskIndices.clear(); // clear the map
        }

        void reserve(int i) {
            // increase the capacity if necessary
            if (i > capacity) {
                capacity = i;
                // create new heap array with the new capacity
                pair<int, string>* newHeap = new pair<int, string>[capacity + 1];

                // copy the elements in the new heap array
                for(int j = 1; j <= tasksInPQ; ++j) {
                    newHeap[j] = heap[j];
                }
                // delete old heap array
                delete[] heap;
                // update the heap pointer to point to new heap array
                heap = newHeap;

            }
        }

        void make_heap(int i) {
            // calculate indices of children
            int smallest = i; 
            int leftChild = 2 * i; 
            int rightChild = 2 * i + 1; 

            // find the index of the smallest child
            if(leftChild <= tasksInPQ && heap[leftChild].first < heap[smallest].first) {
                smallest = leftChild;
            }

            if(rightChild <= tasksInPQ && heap[rightChild].first < heap[smallest].first) {
                smallest = rightChild;
            }

            // if the smallest child is different from current index, swap and continue recursively
            if(smallest != i) {
                swap(heap[i], heap[smallest]);
                make_heap(smallest);
            }
        }

        void build_heap(const vector<string>& taskIDs, const vector<int>& priorities) {
            // build heap from the given taskIDs vector, and priorities vector
            int size = taskIDs.size();
            // increase capacity if necessary
            if(size > capacity) {
                reserve(size);
            }

            // set number of tasks in the heap
            tasksInPQ = size; 
            for (int i = 1; i <= size; ++i) {
                // insert the tasks into the heap
                heap[i] = make_pair(priorities[i - 1], taskIDs[i - 1]);
                taskIndices[taskIDs[i - 1]] = i;
            }
            
            // call make heap
            for(int i = size / 2; i >= 1; --i) {
                make_heap(i);
            }
         }

    private:

        string minID;

        void percolate_up(int index) {
            if (index <= 1) return; // base case: stop when we reach the root
            // call the index of the parent root
            int parent = index / 2;
            // if current has a smaller priority than parent, swap
            if (heap[index].first < heap[parent].first) {
                swap(heap[index], heap[parent]);

                // update indices in the taskIndices map
                taskIndices[heap[index].second] = index;
                taskIndices[heap[parent].second] = parent;

                percolate_up(parent); // recur to check the parent
            }
        }


        void percolate_down(int index) {
            // calculate indices of children
            int leftChild = 2 * index;
            int rightChild = 2 * index + 1;
            int smallest = index;

            // find index of smallest child
            if (leftChild <= tasksInPQ && heap[leftChild].first < heap[smallest].first) {
                smallest = leftChild;
            }

            if(rightChild <= tasksInPQ && heap[rightChild].first < heap[smallest].first) {
                smallest = rightChild;
            }

            if(smallest != index) {
                swap(heap[index], heap[smallest]);

                // update indices in the taskIndices map
                taskIndices[heap[index].second] = index;
                taskIndices[heap[smallest].second] = smallest;

                percolate_down(smallest); // recursion to check the swapped child
            }
        }

        void resize_heap() {
            // double capactiy of heap
            capacity *= 2;
            // creates new heap array with the updated capacity
            pair<int, string>* newHeap = new pair<int, string>[capacity + 1];
            // copy heap elements into new heap array
            for(int i = 1; i <= tasksInPQ; ++i) {
                newHeap[i] = heap[i];
            }
            // delete old heap array
            delete[] heap;
            // update pointer to new heap array
            heap = newHeap;
        }
    };

    // data members for IPQ
    MinHeap minHeap; // Instance of MinHeap class
    int size;
};
