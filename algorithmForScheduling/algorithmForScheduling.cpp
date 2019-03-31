/*
날짜 19. 03 .25
알고리즘 파트 : 홍영주, 허윤석

작성자 :홍영주
SPN 구현 시도
*/

#include <cstdio>
#include <iostream>
#include <queue>
#include <cstring>

using namespace std;
// 프로세스 구조체
struct Process {
	string name;					// 프로세스의 이름
	int arrival_time;			// 메모리에 들어간 시간
	int burst_time;				// 실행시간
	int waiting_time;			// 기다린 시간
	int turnaround_time ;			// 총 실행시간
	double normalized_time;			// 정규화 시간
	int running_time ;			// 돌린시간
	int remain_time; 				// 실행하기에 남은 시간.
	Process() :name("null") {};
	Process(string aname, int aarrival_time, int aburst_time, int awaiting_time, int aturnaround_time, double anormalized_time, int arunning_time, int aremain_time){
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

// SPN 프로세스 함수 
template <typename T>
void spn_process(T* origin_queue);

//SRTN 프로세스 함수
template <typename T>
void srtn_process(T* origin_queue);

//hrrn 프로세스 함수
template <typename T>
void hrrn_process(T* origin_queue);

// 프로세스 비교 함수,  입력시간으로 정렬하는 함수
struct comparison {
	bool operator()(Process t, Process u) {
		return t.arrival_time > u.arrival_time;
	}
};

// spn 비교 함수
struct spn {
	bool operator()(Process t, Process u) {
		return t.burst_time > u.burst_time;
	}
};

// hrrn 비교 함수
struct hrrn {
	bool operator()(Process t, Process u) {
		double t_response_ratio = ((double)t.waiting_time + t.burst_time) / t.burst_time;
		double u_response_ratio = ((double)u.waiting_time + u.burst_time) / u.burst_time;
		return t_response_ratio < u_response_ratio;
	}
};

// 단순히 que의 burst time 만 출력해주는 함수 (단순 테스트용 ) 
template <typename T>
void showing(T que) {
	while (!que.empty()) {
		Process p = que.front();
		que.pop();
		printf("프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);
	}
}

int main() {
	queue<Process> que;			// 가상의 입력의 큐
								//Process p{ "p1",0,2,3,4,5,6,7 };
	Process p{"dd",1,2,3,4,5,6,7};
	//printf("?프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name, p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);	//
	que.push(Process{ "p1",0,3,0,0,0,0,0 });
	//printf("?%s\n", p.name.c_str());
	//printf("?프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);
	que.push(Process{ "p2",1,7,0,0,0,0,0 });
	que.push(Process{ "p3",3,2,0,0,0,0,0 });
	que.push(Process{ "p4",5,5,0,0,0,0,0 });
	que.push(Process{ "p5",6,3,0,0,0,0,0 });

	showing(que);

	hrrn_process(&que);			// 큐의 주소를 함수의 매개변수로 사용
	
	showing(que);

	return 0;
}

// SPN 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 SPN 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 이 함수만 개발하면 됨.
// 오리진 큐는 우선순위 큐가아닌 일반 적인 stl 큐를 사용
template <typename T>
void spn_process(T* origin_queue) {
	priority_queue<Process, vector<Process>, spn> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;
	Process processor;										// 프로세서 등록
	Process tmp_process;

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
	while (!origin_queue->empty()) {
		tmp.push(origin_queue->front());
		origin_queue->pop();
	}
	
//	while (!tmp.empty()) {
//		cout << "임시 큐"<<tmp.top().arrival_time << endl;
//		tmp.pop();
//	}
	// time은 t변수 20초까지 카운트 한다.
	for (int t = 0; t <= 20; t++) {
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
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
			}
		}
		if (!processor.isNull()) {
			if (processor.remain_time > 0) {
				processor.running_time++;
				processor.remain_time--;
			}
			if (processor.remain_time == 0) {
				processor.turnaround_time = t - processor.arrival_time + 1;
				processor.waiting_time = processor.turnaround_time - processor.burst_time;
				processor.normalized_time = (double)processor.turnaround_time / processor.burst_time;
//				printf("%d 초에 프로세서 %s가 origin_queue에 들어왔습니다.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				processor.setNull(true);
			}
			
		}
		
	}
}



// Strn 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 hrrn 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 오리진 큐는 우선순위 큐가아닌 일반 적인 stl 큐를 사용
// 비선점 형태이며 spn의 변형 방법이다. 프로세스의 대기 시간을 고려해서 기회를 제공함
// response ratio= (WT+BT) /BT (응답률)이 높은 프로세스를 우선으로 고려함.
template <typename T>
void srtn_process(T* origin_queue) {
	priority_queue<Process, vector<Process>, spn> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;
	Process processor;										// 프로세서 등록
	Process tmp_process;

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
	while (!origin_queue->empty()) {
		tmp.push(origin_queue->front());
		origin_queue->pop();
	}

	// time은 t변수 20초까지 카운트 한다.
	for (int t = 0; t <= 20; t++) {
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
		// 그리고 SRTN은 선점 프로세스 방식으로 등록된 메모리의 남은 remain time이 더 작으면 체인지. 
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
			}
		}
		if (!processor.isNull()) {
			// 프로세서에 있는 waiting time보다 메모리 큐의 remain time이 더 작으면 프로세서 큐는 메모리큐에 넣고. 메모리큐는 프로세서에 넣는다.
			if (!memory_queue.empty()) {
				Process temp_process = memory_queue.top();
				if (temp_process.remain_time < processor.remain_time) {
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					printf("%d초에 프로세스 %s가 메모리 큐에 있는 프로세서 %s와 교환하였습니다.", t, processor.name.c_str(), temp_process.name.c_str());
					processor = temp_process;
				}
			}
			
			if (processor.remain_time > 0) {
				processor.running_time++;
				processor.remain_time--;
			}
			if (processor.remain_time == 0) {
				processor.turnaround_time = t - processor.arrival_time + 1;
				processor.waiting_time = processor.turnaround_time - processor.burst_time;
				processor.normalized_time = (double)processor.turnaround_time / processor.burst_time;
				//				printf("%d 초에 프로세서 %s가 origin_queue에 들어왔습니다.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				processor.setNull(true);
			}

		}

	}
}

// HRRN 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 HRRN 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 이 함수만 개발하면 됨.
// 오리진 큐는 우선순위 큐가아닌 일반 적인 stl 큐를 사용
template <typename T>
void hrrn_process(T* origin_queue) {
	priority_queue<Process, vector<Process>, spn> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;
	priority_queue<Process, vector<Process>, hrrn> hrrn_queue;		// hrrn 적용하는 큐.
	Process processor;										// 프로세서 등록
	Process tmp_process;

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
	while (!origin_queue->empty()) {
		tmp.push(origin_queue->front());
		origin_queue->pop();
	}

	//	while (!tmp.empty()) {
	//		cout << "임시 큐"<<tmp.top().arrival_time << endl;
	//		tmp.pop();
	//	}
	// time은 t변수 20초까지 카운트 한다.
	for (int t = 0; t <= 20; t++) {
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
		if (processor.isNull()) {

			// 메모리 큐에 있는 것을 hrrn 큐로(hrrn 프로세스 기준을 적용해 다시 정렬시킨다.)
			while (!memory_queue.empty()) {
				Process temp_process = memory_queue.top();
				temp_process.waiting_time = t - temp_process.arrival_time;
				hrrn_queue.push(temp_process);
				memory_queue.pop();
			}

			// hrrn큐에서 프로세서로 등록시킨다.
			if (!hrrn_queue.empty()) {
				processor = hrrn_queue.top();
				hrrn_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
			}

			// 남아있는 hrrn 큐를 다시 메모리 큐로 넣는다.
			while (!hrrn_queue.empty()) {
				memory_queue.push(hrrn_queue.top());
				hrrn_queue.pop();
			}
		}
		if (!processor.isNull()) {
			if (processor.remain_time > 0) {
				processor.running_time++;
				processor.remain_time--;
			}
			if (processor.remain_time == 0) {
				processor.turnaround_time = t - processor.arrival_time + 1;
				processor.waiting_time = processor.turnaround_time - processor.burst_time;
				processor.normalized_time = (double)processor.turnaround_time / processor.burst_time;
				//				printf("%d 초에 프로세서 %s가 origin_queue에 들어왔습니다.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				processor.setNull(true);
			}

		}

	}
}