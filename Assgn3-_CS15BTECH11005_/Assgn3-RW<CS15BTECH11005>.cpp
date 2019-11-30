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
ofstream out("RW-log.txt");				//File Stream
ofstream avg("Average_time.txt");
void writer(int id, int kw, int csSeed, int remSeed);
void reader(int id, int kr, int csSeed, int remSeed);
int nw,nr,kw,kr,csSeed,remSeed;
double avgtimetakenw=0;
double avgtimetakenr=0;
int readcount = 0;
sem_t rw_mutex;
sem_t mutex1;
int main(){
	sem_init(&rw_mutex, 0, 1);
	sem_init(&mutex1, 0, 1);
	input>>nw;
	input>>nr;
	input>>kw;
	input>>kr;
	input>>csSeed;
	input>>remSeed;
	std::thread writerthreads[nw];
	std::thread readerthreads[nr];
	for (int i = 0; i < nw; ++i)
	{
		writerthreads[i]=std::thread(writer,i+1,kw,csSeed,remSeed); 	
	}
	for (int i = 0; i < nr; ++i)
	{
		readerthreads[i]=std::thread(reader,i+1,kr,csSeed,remSeed);
	} 		
	for (int i=0; i<nw; i++)
	{
		writerthreads[i].join();						// join the threads
	}
	for (int i=0; i<nr; i++)
	{
		readerthreads[i].join();						// join the threads
	}
	avgtimetakenw /= nw;
	avgtimetakenr /= nr;
	avg<<"\nAverage waiting time for Writer Thread : "<<avgtimetakenw<<endl;
	avg<<"\nAverage waiting time for Reader Thread : "<<avgtimetakenr<<endl;
	sem_destroy(&rw_mutex);
	sem_destroy(&mutex1);
	return 0;
}	

void writer(int id,int kw,int csSeed,int remSeed){
	clock_t tim;
	for (int i = 1; i <= kw; ++i)
	{
		string st = "";
		const time_t reqTime = time(0);
		st = to_string(i) + "th CS request by Writer thread " + to_string(id) + " at " + asctime(localtime(&reqTime)) + "\n";
		out<<st;
		tim=clock();
		sem_wait(&rw_mutex);
		const time_t enterTime=time(0);
		st.clear();
		st = to_string(i) + "th CS entry by Writer thread " + to_string(id) + " at " + asctime(localtime(&enterTime)) + "\n";
		out<<st;
		tim=clock()-tim;
		avgtimetakenw+=((double)tim)/CLOCKS_PER_SEC;	
		int randCSTime;
		srand(csSeed);
		randCSTime=rand()%100;														//Taking mod so as to not let the thread to sleep for very long amount of time
		usleep(randCSTime);
		sem_post(&rw_mutex);									//exit to cs
		const time_t exitTime=time(0);
		st.clear();
		st += to_string(i) + "th CS exit by Writer thread " + to_string(id) + " at " + asctime(localtime(&enterTime)) + "\n";
		int randRemTime;
		srand(remSeed);
		randRemTime=rand()%100;
		usleep(randRemTime);
		out<<st;
		
	}
}

void reader(int id, int kr, int csSeed, int remSeed){
	clock_t tim;
	for (int i = 1; i <= kr; ++i){
		string st = "";
		const time_t reqTime = time(0);
		st = to_string(i) + "th CS request by Reader thread " + to_string(id) + " at " + asctime(localtime(&reqTime)) + "\n";
		out<<st;
		tim=clock();
		sem_wait(&mutex1);
		readcount++;
		if (readcount == 1){
			sem_wait(&rw_mutex);
		}
		sem_post(&mutex1);
		const time_t enterTime=time(0);
		st.clear();
		st = to_string(i) + "th CS entry by Reader thread " + to_string(id) + " at " + asctime(localtime(&enterTime)) + "\n";
		out<<st;
		tim=clock()-tim;
		avgtimetakenr+=((double)tim)/CLOCKS_PER_SEC;	
		int randCSTime;
		srand(csSeed);
		randCSTime=rand()%100;														//Taking mod so as to not let the thread to sleep for very long amount of time
		usleep(randCSTime);
		sem_wait(&mutex1);
		readcount--;
		if (readcount == 0){
			sem_post(&rw_mutex);
		}
		sem_post(&mutex1);						//exit to cs
		const time_t exitTime=time(0);
		st.clear();
		st = to_string(i) + "th CS exit by Reader thread " + to_string(id) + " at " + asctime(localtime(&enterTime)) + "\n";
		out<<st;
		int randRemTime;
		srand(remSeed);
		randRemTime=rand()%100;
		usleep(randRemTime);
	}
}