#include <bits/stdc++.h>
#include <vector>
#include <thread>
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
			cout<<a[i]<<"     ";
		}
	}
}
int main(){
	int n;
	cout<<"ENter n\n";
	cin>>n;
	int k;
	cout<<"ENter k"<<endl;
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
	std::thread threads[k];
	for(int i=0; i<k; i++){
		threads[i]=std::thread (prime,a[i]);
	}
	for (int i=0; i<k; i++)
	{
		threads[i].join();
	}
	return 0;
}