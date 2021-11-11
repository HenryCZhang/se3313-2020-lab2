#include <iostream>
#include "SharedObject.h"
#include "thread.h"
#include <string> 
using namespace std;

struct MyShared{
	int sharedThreadID;
	int sharedReportID;
	int sharedDelay;
	bool sharedRunning;
};

class WriterThread : public Thread{
public:
		int delay;
		int threadID;
		int reportID = 1;
		bool flag;
		
		WriterThread(int in, int threadid):Thread(8*1000){
			this->delay = in;
			this->threadID = threadid;
		}

		virtual long ThreadMain(void) override{
			
			Shared<MyShared> sharedMemory ("sharedMemory");
			while(true)// Here the memory values of the thread must be updated
			{
				sharedMemory->sharedThreadID = threadID; //sets thread ID
				sharedMemory->sharedReportID = reportID; //sets number of reports
				sharedMemory->sharedDelay = delay;//sets delay
				reportID ++; //increments the number of reports 
				sleep(delay); //sleeps the thread for the amount of delay set by the user
				if(flag){
					break;
				}
			}
		}
};



int main(void)
{
	string userInput;
	string threadDelay;
	int numThreads=1; //Used for creating writerThread objects where numThreads will be used to give threads ID #s

	cout << "I am a Writer" << endl;
	
	WriterThread * thread; //declare thread 

	Shared<MyShared> shared("sharedMemory", true); //This is the owner of sharedMamory

	shared ->sharedRunning = true;	//Set the running value to true so the reader can poll the shared memory

	while(true){
		cout << "Would you like to create a writer thread? Enter yes or no: "<< endl;
		cin >> userInput;
		cin.ignore();
		if(userInput == "yes"){
			cout << "Please enter a delay time for this thread: "<< endl;
			cin >> threadDelay;
			cin.ignore();
			int delay = atoi(threadDelay.c_str()); //Convert the string to and int

			//Create a new writerThread object 
			thread = new WriterThread(delay,numThreads);//instantiate thread 
			numThreads++; //Increase the number of threads 
		}
		else if (userInput == "no"){
			//If the user enters N then the while statement will break and threads will be handled 
			if(numThreads -1 !=0){ //Avoids core dump in the case N is entered first and no threads were created
				shared->sharedRunning = false;
				break;
			}
			else{
				break;
			}
		}
		else
		{
			//To handle inproper inputs. 
			cout << "Invalid input entered." << endl;
		}
	}//end while 
	if(numThreads-1 != 0){ //Avoids core dump in the case N is entered first and no threads were created
		thread ->flag = true;
		delete thread; 
	}
	

	return 0;	
}//end main 