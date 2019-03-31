#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

struct Process {
	string process_name;
	double arrival_time;			// 메모리에 들어간 시간
	double burst_time;				// 실행시간
	double waiting_time;			// 기다린 시간
	double turnaround_time;			// 총 실행시간
	double normalized_time;			// 정규화 시간
	Process(string process_name, double arrival_time, double burst_time) : process_name(process_name), arrival_time(arrival_time), burst_time(burst_time) {}

};

template <typename T>
void showing(T que) {
	while (!que.empty()) {
		Process p = que.front();
		que.pop();

		cout << "Name of Process: " << p.process_name << endl;
		cout << "Arrival time: " << p.arrival_time << endl;
		cout << "Burst time: " << p.burst_time << endl;
		cout << "Waiting_time: " << p.waiting_time << endl;
		cout << "Turnaround_time: " << p.turnaround_time << endl;
		cout << "Normalized_time: " << (double)p.normalized_time << endl;
		
		cout << endl;
	}
}
template <typename C>
void caculate(C &que, C &que2, int &elasped_time) {      //1초씩 실행?
	while(!que.empty()) {			  //실행하고 있는건 몇초 남았나?
		Process p = que.front();

		elasped_time += que.front().burst_time;
		que.front().turnaround_time = elasped_time - que.front().arrival_time;
		que.front().waiting_time = que.front().turnaround_time - que.front().burst_time;
		que.front().normalized_time = (double)que.front().turnaround_time / (double)que.front().burst_time;	
		que2.push(que.front());

		que.pop(); 

	}
}

bool compare(Process &a, Process &b) {
	if (a.arrival_time < b.arrival_time)
		return true;
	else
		return false;
}
int main() {
	int num, y, z;
	string n;
	int elapsed_time = 0;

	queue<Process> que;			// 가상의 입력의 큐
	vector<struct Process> v;
	queue<Process> que_temp;

	cout << "Enter number of processes : ";
	cin >> num;
	cout << endl;
	
	for (int i = 0; i < num ; i++) {
		cout << "Enter name of process name: ";
		cin >> n;
		cout << "Enter arrival time & burst time: ";
		cin >> y >> z;
		v.push_back(Process(n, y, z));
		
		cout << endl;

	}
	
	sort(v.begin(), v.end(), compare);
	

	for (Process p : v) {
		que.push(p);
	}

	caculate(que, que_temp, elapsed_time);

	showing(que_temp);

	return 0;
	
}