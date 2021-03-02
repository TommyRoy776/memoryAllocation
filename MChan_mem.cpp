#include <iostream>
#include <cstring>
#include <unistd.h>
/*
gcc MChan_mem.cpp -o MChan_mem.exe -lstdc++
./MChan_mem.exe 1048576
*/


using namespace std;
struct process{ //struct of process in memory
  int startAddress;
  int endAddress;
  char process_id[10];
  int space;
  struct process *next;
};

struct process *head; //start of the process list
struct process *memory; //memory space pointer
struct process *temp;  //temp pointer  
int spaceAvaliable; //record current space avaliable
int maxSpace;   //save the maximum address
void worstFit(char processId[3], int spaceNeed){
    // struct process *newProcess = (struct process *)malloc(sizeof(struct process));
     int Remainspace = 0;  
     int largestHole = -1; //variable for size of largest memory space
     temp = head->next;  //start searching  
     while (temp != NULL){
        if (strcmp(temp->process_id, "Free") == 0 && temp->space >= spaceNeed){ //check is it free and large enough
            if(temp->space > largestHole){  
                 largestHole = temp->space; //replace if larger than previous
            }
        }
        temp = temp->next; 
      }
      temp = head->next; //reinitialize
       while (temp != NULL){ //find the largest space that we saved
        if (temp->space == largestHole){
            strcpy(temp->process_id, processId);  //assign data to the space
            temp->endAddress = temp->startAddress + spaceNeed -1 ;
            Remainspace = temp->space - spaceNeed;
            spaceAvaliable -= spaceNeed;
            temp->space = 0;
            if (Remainspace > 0){ //the remain space set to free
               struct process *newProcess = (struct process *)malloc(sizeof(struct process));
               strcpy(newProcess->process_id, "Free");
               newProcess->space = Remainspace;
               newProcess->startAddress = temp->endAddress + 1;
                newProcess->endAddress =  newProcess->startAddress + Remainspace;
                newProcess->next = temp->next;
                temp->next = newProcess;
            }
             return;

        }
          temp = temp -> next;
       }

}

void output();
void Compact(){
    temp = head;
    int emptyStart; //save the start address of free space
    int diff; //difference for calculate the end address
    while(temp->next!= NULL){
       if(strcmp(temp->next->process_id,"Free")==0 && temp->next->next != NULL){ //if the next process is free and not the last one 
           emptyStart = temp->next->startAddress;
           diff = temp->next->next->startAddress-temp->next->startAddress;
            temp->next = temp->next->next;     //disconnect the free process 
            if(temp->next->next == NULL){ //for the case of last process 
                //cout<<"Reached1"<<endl;
                temp->next->startAddress = emptyStart;
                temp->next->endAddress = maxSpace;
                temp->next->space = temp->next->endAddress-temp->next->startAddress;
            }else{
                       //cout<<"Reached2"<<endl;
              temp->next->startAddress = emptyStart;   //adjust start & end address to the next process  
               temp->next->endAddress =  temp->next->endAddress-diff;
            }

            struct process *ptr;
            ptr = temp->next;
            while(ptr->next != NULL){  //move down the after processes
                // int tempDiff = ptr->next->startAddress - ptr->endAddress-1 ;
                 ptr->next->startAddress = ptr->endAddress+1;
                 if(ptr->next->next == NULL){
                   // cout<<"Reached3"<<endl;
                    ptr->next->endAddress =  maxSpace;
                    ptr->next->space = ptr->next->endAddress-ptr->next->startAddress+1;
                 }else{
                    ptr->next->endAddress =  ptr->next->endAddress-diff;
                 }
                  ptr = ptr->next;
            }
       }
       if(strcmp(temp->next->process_id,"Free")==0 && temp->next->next != NULL){
          temp = temp;  //if the next process is free keep the temp 
       }else if(temp->next->next == NULL){
           break;
       }else{
         temp = temp->next;  
       }
       //temp = temp->next;
    }
    //cout<<"Finished Compact"<<endl;
   //output();
}
void release(char process_id[3]){
     int tempNum;
     temp = head->next;
     while(temp != NULL){
        if(strcmp(temp->process_id,process_id)==0){ //find the corresponding process
           break;
        }
        temp = temp->next;
     }
     tempNum = temp->endAddress - temp->startAddress+1; //save the size
     temp->space = tempNum; //free the space
     spaceAvaliable += tempNum; //add to avaliable space
     strcpy(temp->process_id, "Free");
       //output();
}

void output()
{
     cout <<"spaceAvaliable: "<<spaceAvaliable<<endl;
    temp = head;
    while (temp->next != NULL)
    {
        printf("Addresses [%d : %d] Process %s\n", temp->next->startAddress, temp->next->endAddress, temp->next->process_id);
        temp = temp->next;
    }
}


int main(int argc, char *argv[])
{
    int pSpace;
    char typeW[2];
    char command[10];
    char processID[3];
    char inputBuffer[128];
    int initalM = atoi(argv[1]);
    printf("allocator>");
    fflush(stdout);
    if (read(0, inputBuffer, 128) < 0){ //read input
         printf("input Error!!");
    }

    initalM -= 1;
    spaceAvaliable = initalM+1; //initial space
    maxSpace = initalM;  //end address
    head = (struct process *)malloc(sizeof(struct process));
    memory = (struct process *)malloc(sizeof(struct process));
    //initial head process
    strcpy(head->process_id, "Head");
    head->startAddress = -1;
    head->endAddress = -1;
    head->space = NULL;
    head->next = memory;
    //initial main memory space
    strcpy(memory->process_id, "Free");
    memory->startAddress = 0;
    memory->endAddress = initalM;
    memory->space = initalM+1;
    memory->next = NULL;
    while(inputBuffer[0] != '\n'){ //if input empty command, ends program
      if((sscanf(inputBuffer, "%s %s", command, processID)) < 0){
        printf("Error occurs\n");
      }
      if (strcmp("RQ", command) == 0){
          sscanf(inputBuffer, "%s %s %d %s", command, processID, &pSpace, typeW);
         worstFit(processID,pSpace);  //only worst fit in this project
        // output();
      }else if(strcmp("RL", command) == 0){
         sscanf(inputBuffer, "%s %s", command, processID);
         release(processID);

      }else if (strcmp("C", command) == 0){
          Compact();
      }else if(strcmp("STAT", command) == 0){
          output();
      }else if(strcmp("QUIT", command) == 0){
        return 0;
      }else{
        printf("Command failed !! \n");
      }
       memset(inputBuffer, 0, sizeof inputBuffer); //clean buffer
       printf("allocator>");
        fflush(stdout);
        if (read(0, inputBuffer, 128) < 0)
            printf("failed !! \n");
    }
   // return 0;
}
