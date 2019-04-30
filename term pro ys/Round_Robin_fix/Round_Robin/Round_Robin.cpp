#include <cstdio>
#include <iostream>
#include <queue>
#include <cstring>
#include <string>

using namespace std;
// 프로세스 구조체
struct Process {
	static int count;				// 프로세스가 들어온 개수 카운터
	int number;						// 프로세스 기본키
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
		number = Process::count++;
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

int Process::count = 0;



struct comparison {
	bool operator()(Process t, Process u) {
		return t.arrival_time > u.arrival_time;
	}
};

template <typename R>
int rr_process(R* origin_queue, R* table_que, int quant);

struct key {
	bool operator()(Process t, Process u) {
		return t.number > u.number;
	}
};

template <typename T>
void showing(T que, T table_queue) {
	while (!que.empty()) {
		Process p = que.front();
		que.pop();
		printf("프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d 프로세서 들어온 시간 : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time, p.process_arrival_time);
	}
	if (!table_queue.empty()) {
		printf("----------------테이블 출력 ----------------------\n");
	}

	while (!table_queue.empty()) {
		Process p = table_queue.front();
		table_queue.pop();
		printf("프로세스 키 : %d 프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d 프로세서 들어온 시간 : %d \n", p.number, p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time, p.process_arrival_time);
	}
}

int main() {
	int num, y, z;
	string n;
	int quant;

	queue<Process> origin_que;			// 가상의 입력의 큐
	queue<Process> table_que;

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

	showing(origin_que,table_que);

	rr_process(&origin_que, &table_que, quant);

	showing(origin_que,table_que);

	return 0;
}
template <typename R>
int rr_process(R* origin_queue,R* table_queue, int quant) {
	queue<Process> memory_queue;
	priority_queue<Process, vector<Process>, comparison> tmp;
	priority_queue<Process, vector<Process>, key> table_queue_tmp;
	Process processor;
	Process	tmp_process;
	int time = 0;   //현재 진행시간
	int total_time = 0;
	int time_burst_sum = 0;
	int remain_q = 0;

	// 가장 늦게 들어간 어라이브타임 + 총 버스트타임
	while (!origin_queue->empty()) {
		Process temp = origin_queue->front();
		tmp.push(temp);

		int lastArrival_time = 0;
		//2 4  0 8
		if (lastArrival_time < temp.arrival_time)
			lastArrival_time = temp.arrival_time;

		time_burst_sum += temp.burst_time;

		time = time_burst_sum + lastArrival_time;

		origin_queue->pop();
	}

	// time은 t변수 time초까지 카운트 한다.
	for (int t = 0; t <= 2*time; t++) {
		// tmp 큐에서 메모리 큐로 넣기
		cout << "현재 t는 " << t << endl;
		if(memory_queue.size() > 0)
			cout << "현재 메모리탑 " << memory_queue.front().name << endl;
		int tmp_size = tmp.size();		// tmp 큐의 사이즈
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				cout << "메모리큐에넣기 " << tmp_process.name << endl;
				memory_queue.push(tmp_process);		// 메모리 큐는 넣으면 burst time이 작은 것 기준으로 자동 정렬이 된다.
				tmp.pop();
				//printf("%d 초에 메모리 큐에 %s가 들어왔습니다. \n", t,tmp_process.name.c_str());
			}
		}

		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.front();
				cout << "프로세서 설정 " << processor.name << endl;
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.

				remain_q = quant;
				//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
			}
		}

		// processor가 등록이 안되어있을 때는 메모리의 앞에 있는 것을 빼서 등록시킨다.
		if (!processor.isNull()) {
			
			// 프로세서에 있는 waiting time보다 메모리 큐의 remain time이 더 작으면 프로세서 큐는 메모리큐에 넣고. 메모리큐는 프로세서에 넣는다.
			/*if (!memory_queue.empty()) {
				Process temp_process = memory_queue.front();
				if (temp_process.arrival_time < processor.arrival_time) { //arrive타임으로 우선순위를 가지게 된다.
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					processor = temp_process;
					processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록 (체인지 된 시간)
				}
			}*/	
			

			if (processor.burst_time > quant && remain_q >= 0){        //만약 들어온 burst타임이 quant보다 크다면
				processor.running_time += 1;      //현재 진행상황은 quant만큼만 증가시킨다.
				processor.remain_time -= 1;
				//processor.burst_time -= 1;
				cout << "일한 프로세서 " << processor.name << " 일한 양 " << 1 << " 남은 일양 " << processor.burst_time << endl;

				
				if (remain_q == 1) {
					memory_queue.push(processor);
					cout << processor.name << "프로세서 나감" << endl;
					processor.setNull(true);
				}
				cout << "남은 퀀텀 값 " << remain_q << endl;

				remain_q -= 1;
				cout << "빼고 남은 퀀텀 값 " << remain_q << endl;
			}
			
			else if (processor.burst_time <= quant) {
				cout << "일한 프로세서 " << processor.name << " 마지막 일한 양 " << processor.burst_time << endl;
				processor.running_time += processor.burst_time;
				processor.remain_time = 0;
			}

			if (processor.remain_time == 0) {
				cout << t << " " << processor.name << " " << processor.arrival_time << " " << processor.burst_time << endl;
				processor.turnaround_time = t - processor.arrival_time + 1;
				processor.waiting_time = processor.turnaround_time - processor.burst_time;
				processor.normalized_time = (double)processor.turnaround_time / processor.burst_time;
				//printf("%d 초에 프로세서 %s가 origin_queue에 들어왔습니다.\n ", t, processor.name.c_str());
				cout << "나감 " << processor.name << endl;

				origin_queue->push(processor);
				table_queue_tmp.push(processor);
			

				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
				processor.setNull(true);
			}
		}
	}
	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}

	return total_time + 1;
}