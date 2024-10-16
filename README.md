# indexed-priority-queue

Design Documentation:

The IPQ class is a template class, allowing users to incorporate different initial queue capacities. This provides flexibility in memory allocation and meets the assignment requirements. The IPQ class has a nested MinHeap class to implement the underlying data structure for the Indexed-Priority Queue. The encapsulation ensures that data related to the heap is protected. Changes to the heap are only done by code in the MinHeap class and IPQ class functions do not directly operate on the heap data structure. 

The IPQ's parameterized constructor calls a function, build_heap, in the MinHeap class. The build_heap function takes in two vectors to construct the heap and calls the make_heap function to ensure heap properties. I also added a display function as a public function in the IPQ class. The display function ensures that the contents are correct after performing operations on the heap, specifically showing that task IDs are paired to the correct priorities.

The map taskIndices reflects a task ID's position in the heap. The map is a member of the MinHeap class. During heap operations, the taskIndices map is used to get the index of the task being operated on. After a heap operation is completed, the map is updated accordingly to reflect the changes made to the task indices within the heap.

The use of std::pair in the heap implementation allowed me to store the task ID and priority in a single object. It also made accessing the task ID and priority easy and straightforward. In the MinHeap class, the insert function uses make_pair to insert the new task ID and priority in the heap. It handles duplicate task IDs, which are ignored, and calls resize_heap when the heap is full. Also, insert handles duplicate priorities, the function allows the same priority to be inserted without violating the heap order property. It then calls percolate_up to restore heap properties.The deleteMin of the MinHeap class handles the removal of the minimum element from the heap. The deleteMin function of the IPQ class returns a reference to the minID member variable of the MinHeap class. Similarly, the getMin function returns a reference to the task ID with the smallest priority. The minimum element refers to the task with the lowest numerical priority, i.e., the root. 

The updatePriority function updates the specified task's priority. It checks if the new priority is greater than the old priority, this determines which percolate function is called. If it is less than the old priority, percolate_up is called to move the task up. Otherwise, percolate_down is called to move the task down, as it decreases in priority. The remaining
MinHeap functions follow the assignment instructions and are fairly straight-forward to implement. 

The private functions of the MinHeap class include percolate_down, percolate_up, and resize_heap. Both percolate_up and  percolate_down are implemented recursively. These functions are internal implementation details of the MinHeap class and are not meant to be accessed or called directly by IPQ. These functions maintain the heap structure and ensure the heap property is not violated.

Testing Regimen:

The test program for the IPQ implementation thoroughly validates the functionality and correctness of the IPQ class. The test program ensures that all core functions of the IPQ class perform as expected, including insertion, deletion, updating priorities, and accessing the minimum element. The IPQ is tested with various edge cases, such as empty, queues, duplicate priorities, duplicate task IDs, size boundaries, and the removal of elements. 

When duplicate priorities are inserted, the heap can contain tasks of the same priority without any issues. The heap algorithm can handle duplicate priorities. For duplicate task IDs, the insert function of the MinHeap class checks if the specified task ID is already in the heap. If it is a duplicate, it is not added to the heap and a message is printed out to communicate that
the task ID already exists and it can't be inserted.

The program tests if the task IDs and priorities are being inserted correctly. It also ensures the size of the IPQ is being properly updated after removals and insertions. The display function is helpful in that it shows the task IDs and the associated priority, this helps ensure that they are paired correctly even after a priority is updated at the root or an internal node. 

The clear function is also tested by creating an IPQ and using isEmpty to check if the clear function removes all the tasks in the IPQ. Additionally, the program displays the current task at the root. This ensures that even after operations are made to the root, such as updatePriority and removal, the IPQ has the correct root task.