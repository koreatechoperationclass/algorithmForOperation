#include <cstdio>
#include <iostream>
#include <queue>
#include <string>
#include <cstring>

using namespace std;
// 프로세스 구조체
struct Process {
	string name;					// 프로세스의 이름
	int arrival_time;			// 메모리에 들어간 시간
	int burst_time;				// 실행시간
	int waiting_time;			// 기다린 시간
	int turnaround_time;			// 총 실행시간
	double normalized_time;			// 정규화 시간
	int running_time;			// 돌린시간
	int remain_time; 				// 실행하기에 남은 시간.
	int process_arrival_time;		// 프로세서에 들어온 시간.
	Process() :name("null") {};
	Process(string aname, int aarrival_time, int aburst_time, int awaiting_time, int aturnaround_time, double anormalized_time, int arunning_time, int aremain_time) {
		name = aname;
		arrival_time = aarrival_time;
		burst_time = aburst_time;
		waiting_time = awaiting_time;
		turnaround_time = aturnaround_time;
		normalized_time = anormalized_time;
		running_time = arunning_time;
		remain_time = burst_time;
	}

	bool isNull() {
		if (name == "null") {
			return true;
		}
		else {
			return false;
		}
	}

	void setNull(bool isNull) {
		if (isNull == true) {
			name = "null";
		}
	}
};

template <typename T>
void showing(T que) {
	while (!que.empty()) {
		Process p = que.front();
		que.pop();
		printf("프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d 프로세서 들어온 시간 : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time, p.process_arrival_time);
	}
}

struct rr {
	bool operator()(Process t, Process u) {
		return t.arrival_time > u.arrival_time;
	}
};

struct comparison {
	bool operator()(Process t, Process u) {
		return t.arrival_time > u.arrival_time;
	}
};

template <typename R>
int rr_process(R* origin_queue, int quant);
/*bool compare(Process &a, Process &b) {
	if (a.arrival_time < b.arrival_time)
		return true;
	else
		return false;

}
*/



int main() {
	int num, y, z;
	string n;
	int quant;

	queue<Process> origin_que;			// 가상의 입력의 큐

	cout << "Enter number of processes : ";
	cin >> num;
	cout << "Enter Time Quantum: ";
	cin >> quant;
	cout << endl;

	for (int i = 0; i < num; i++) {
		cout << "Enter name of process name: ";
		cin >> n;
		cout << "Enter arrival time & burst time: ";
		cin >> y >> z;
		origin_que.push(Process(n, y, z,0,0,0,0,0));

		cout << endl;

	}

	//showing(origin_que);

	rr_process(&origin_que, quant);

	showing(origin_que);

	return 0;
}
template <typename R>
int rr_process(R* origin_queue,int quant) {
	priority_queue<Process, vector<Process>, rr>memory_queue;
	priority_queue<Process, vector<Process>, comparison > tmp;
	Process processor;
	Process	tmp_process;
	int time = 0;
	int time_burst_time;
	int total_time = 0;
	int time_burst_sum = 0;

	while (!origin_queue->empty()) {
		Process temp = origin_queue->front();
		tmp.push(temp);
		if (time < (temp.arrival_time + time_burst_sum)) {
			time_burst_sum += temp.burst_time;
			time = temp.arrival_time + time_burst_sum;
		}
		origin_queue->pop();
	}
	//printf("time : %d\n", time);

	// time은 t변수 time초까지 카운트 한다.
	for (int t = 0; t <= time; t++) {
		// tmp 큐에서 메모리 큐로 넣기
		int tmp_size = tmp.size();		// tmp 큐의 사이즈
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// 메모리 큐는 넣으면 burst time이 작은 것 기준으로 자동 정렬이 된다.
				tmp.pop();
				//				printf("%d 초에 메모리 큐에 %s가 들어왔습니다. \n", t,tmp_process.name.c_str());
			}
		}

		// processor가 등록이 안되어있을 때는 메모리의 앞에 있는 것을 빼서 등록시킨다.
		if (!processor.isNull()) {
			// 프로세서에 있는 waiting time보다 메모리 큐의 remain time이 더 작으면 프로세서 큐는 메모리큐에 넣고. 메모리큐는 프로세서에 넣는다.
			if (!memory_queue.empty()) {
				Process temp_process = memory_queue.top();
				if (temp_process.arrival_time < processor.arrival_time) { //arrive타임으로 우선순위를 가지게 된다.
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					processor = temp_process;
					processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록 (체인지 된 시간)
				}
			}

			if (processor.remain_time > 0) {
				processor.running_time++;

				processor.remain_time = processor.burst_time - quant;  //남은시간 = 총burst타임 - quantum타임
				if (processor.remain_time <= 0){
					processor.remain_time = 0;
				}
			}
			if (processor.remain_time == 0) {
				processor.turnaround_time = t - processor.arrival_time + 1;
				processor.waiting_time = processor.turnaround_time - processor.burst_time;
				processor.normalized_time = (double)processor.turnaround_time / processor.burst_time;
				//printf("%d 초에 프로세서 %s가 origin_queue에 들어왔습니다.\n ", t, processor.name.c_str());
				origin_queue->push(processor);

				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
				processor.setNull(true);
			}

		}
	}

	return total_time + 1;
}