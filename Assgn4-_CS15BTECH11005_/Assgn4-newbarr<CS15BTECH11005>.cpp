#include <bits/stdc++.h>
#include <thread>
#include <fstream>
#include <ctime>
#include <climits>
#include <unistd.h>
#include <mutex>
#include <semaphore.h>
#include <chrono>
#include <sys/types.h>
using namespace std;


ifstream input("inp-params.txt");
ofstream out("new-barr-log.txt");				//File Stream
ofstream avg("Average_time.txt");

void barrier_point();
void testBarrier(int i, int k, float pre, float post);		// find barrier point
int n;				
sem_t mutex1;			
sem_t barrier;			
void init(int n);
int ct=0;
double avgtimetaken=0;
//class Barrier{
//	public:
				    // initalise barrier
//}b;

void init(){
	sem_init(&mutex1, 0, 1);
	sem_init(&barrier, 0, 0);
}

void barrier_point(){
	sem_wait(&mutex1);
	ct++;
	sem_post(&mutex1);
	if (ct == n){		
		sem_post(&barrier);
	}
	sem_wait(&barrier);		
	sem_post(&barrier);	
}

void testBarrier(int id, int k, float pre, float post){
	int beforeBarrSleep, afterBarrSleep;
	clock_t tim;
	tim=clock();
	for (int i = 1; i <= k; ++i)
	{		
		string st = "";
		srand(pre);
		beforeBarrSleep=rand()%1000;
		const time_t beforeTime = time(0);
		st = "Going to sleep before the " +  to_string(i) + "th barrier invocation by Thread " + to_string(id) + " at time: " + asctime(localtime(&beforeTime));// + "\n";
		out<<st;
		usleep(beforeBarrSleep);
		st.clear();
		const time_t beforepointTime = time(0);
		st = "Before the " + to_string(i) + "th barrier invocation by Thread " + to_string(id) + "at time: " + asctime(localtime(&beforepointTime));// + "\n";
		out<<st;
		st.clear();
		barrier_point();
		const time_t afterpointTime = time(0);
		st = "After the " + to_string(k) + "th barrier invocation by Thread " + to_string(id)+ " at time : " + asctime(localtime(&afterpointTime));// + "\n";
		out<<st;
		st.clear();
		srand(post);
		afterBarrSleep=rand()%1000;
		const time_t afterTime = time(0);
		st = "Going to sleep after the " +  to_string(i) + "th barrier invocation by Thread " + to_string(id) + " at time: " + asctime(localtime(&afterTime));// + "\n";
		out<<st;
		st.clear();
		usleep(afterBarrSleep);
	}
	avgtimetaken += ((double)tim)/CLOCKS_PER_SEC;
	avgtimetaken = (double)avgtimetaken/(double)k;
	cout.precision(10);
	avg<<fixed<<avgtimetaken<<endl;
}

int main(){
	int k;
	float pre,post;
	input>>n;
	input>>k;
	input>>pre;
	input>>post;
	init();
	std::thread testthreads[n];
	for (int i = 0; i < n; ++i)
	{
		testthreads[i]=std::thread(testBarrier,i+1,k,pre,post);
	}
	for (int i = 0; i < n; ++i)
	{
		testthreads[i].join();
	}
	return 0;
}