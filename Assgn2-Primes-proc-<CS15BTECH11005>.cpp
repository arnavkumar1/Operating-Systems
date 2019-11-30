#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/stdc++.h>
using namespace std;

void prime(vector<int> a){
	for(int i=0; i<a.size(); i++){
		int isprime=1;
		for(int j=2; j<=sqrt(a[i]); j++){
			if(a[i]%j==0){
				isprime=0;
				break;
			}
		}
		if(isprime){
			cout<<a[i]<<"    ";
		}
	}
	cout<<endl;
}

int main(){
	int n,k;
	cout<<"Enter n"<<endl;
	cin>>n;
	cout<<"Enter k"<<endl;
	cin>>k;
	vector<vector<int>> a(k);
	for(int i=2; i<=n; i++){
		a[i%k].push_back(i);
	}
	cout<<"The array is\n";
	for(int i=0; i<k; i++){
		for(int j=0; j<a[i].size(); j++){
			cout<<a[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	pid_t pid;
	cout<<"\nPrimes are : "<<endl;
	for(int i=0;i<k;i++){
	
		pid = fork();
		if(pid<0){
			perror("\nFork Failed\n");
		}
		else if(pid == 0){
			prime(a[i]);
			exit(0);
	 
		}
		else{
			wait(NULL);
		}
	}

	return 0;
}