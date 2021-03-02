Compilation instructions: 
1.	gcc MChan_mem.cpp -o MChan_mem.exe -lstdc++
2.	./MChan_mem.exe 1048576   //example

Program design:
This is a program for simulating the contiguous memory allocation. In this project, we only have to implement the worst fit strategy for this project. I chose linked list to store the process and memory space. Each process would link to another process or free space. There are 3 functions to implement for RQ, RL and C commands. 

For RQ command, first take the input from user’s keyboard then run the worst fit function, the function would search through the linked list and save the biggest space. The next step would be put the process to the space and fill the space that the process needs. If any memory left in the space, create a free space node and assign the remaining space memory to it.  

For RL, search the matched process id then release the space it occupied and set it to a free memory space.

For  C, this is the most complicated part. Find every free space that is not the last one of the linked list. The previous process node would disconnect it and the nodes after that would move forward. If previous process connects to a free space, keep the previous process as temp node. If not, temp becomes the next node and so on until all free space compacted.        

