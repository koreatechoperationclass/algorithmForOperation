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
#include <cstdlib>
#include <ctime>

using namespace std;
// 프로세스 구조체
struct Process {
	static int count;				// 프로세스가 들어온 개수 카운터
	int number;						// 프로세스 기본키
	string name;					// 프로세스의 이름
	int arrival_time;			// 메모리에 들어간 시간
	int burst_time;				// 실행시간
	int waiting_time;			// 기다린 시간
	int turnaround_time ;			// 총 실행시간
	double normalized_time;			// 정규화 시간
	int running_time ;			// 돌린시간
	int remain_time; 				// 실행하기에 남은 시간.
	int process_arrival_time;		// 프로세서에 들어온 시간.
	int dice_number;				// 주사위의 크기 
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
		number = Process::count++;
		dice_number = 0;					// 처음 생성되면 주사위 숫자는 0
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

	int rolling_dice() {
		srand((unsigned int)time(NULL));
		return dice_number = (rand() % 6) + 1;
	}
	int rolling_dice(int post_dice) {
		return dice_number = (rand() % 6) + 1 + post_dice;
	}
};

int Process::count = 0;			//프로세스 생성 전 정적 변수 static count 를 추가해서 프로세스를 추가한다. 

// SPN 프로세스 함수 
template <typename T>
int spn_process(T* origin_queue, T* table_queue);

//SRTN 프로세스 함수
template <typename T>
int srtn_process(T* origin_queue, T* table_queue);

//hrrn 프로세스 함수
template <typename T>
int hrrn_process(T* origin_queue, T* table_queue);

template <typename T>
int dice_process(T* origin_queue, T* table_queue);

template <typename T>
int rr_process(T* origin_queue, T* table_queue, int quantum);

template <typename T>
int fcfs_process(T* origin_queue, T* table_queue);

// 프로세스 비교 함수,  입력시간으로 정렬하는 함수
struct comparison {
	bool operator()(Process t, Process u) {
		return t.arrival_time > u.arrival_time;
	}
};

// srtn 비교함수
struct srtn_comparison {
	bool operator()(Process t, Process u) {
		return t.remain_time > u.remain_time;
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

struct dice {
	bool operator()(Process t, Process u) {
		return t.dice_number > u.dice_number;
	}
};

// 키값으로 정렬하는 함수
struct key {
	bool operator()(Process t, Process u) {
		return t.number > u.number;
	}
};

// 단순히 que의 burst time 만 출력해주는 함수 (단순 테스트용 ) 
template <typename T>
void showing(T que , T table_queue);

template <typename T>
void queue_dice_rolling(T* memory_queue, T* dice_queue);

int main() {
	queue<Process> que;			// 가상의 입력의 큐
	queue<Process> table_que;		// 테이블 출력을 위한 큐
								//Process p{ "p1",0,2,3,4,5,6,7 };
	
	//printf("?프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name, p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);	//
	que.push(Process{ "p1",1,1000,0,0,0,0,0 });
	//printf("?%s\n", p.name.c_str());
	//printf("?프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);
	que.push(Process{ "p2",10,20,0,0,0,0,0 });
	//que.push(Process{ "p3",3,2,0,0,0,0,0 });
	//que.push(Process{ "p4",5,5,0,0,0,0,0 });
	//que.push(Process{ "p5",6,3,0,0,0,0,0 });

	showing(que, table_que);

	printf("----------------- 그래프 출력 결과 -------------\n");
	printf("총 실행시간 : %d\n", srtn_process(&que, &table_que));			// 큐의 주소를 함수의 매개변수로 사용
	
	showing(que, table_que);

	return 0;
}

template <typename T>
int fcfs_process(T* origin_queue, T* table_queue) {
	queue<Process> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;		// 총시간을 계산하기 위한 임시 저장 큐.( 어라이벌 타임으로 정렬이 된다.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// 테이블 큐에 넣기 위한 템프큐
	Process processor;										// 프로세서 등록
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// 전체 돌아간 총 시간.

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
	while (!origin_queue->empty()) {
		Process temp = origin_queue->front();
		tmp.push(temp);

		int lastArrival_time = 0;
		//2 4  0 8
		if (lastArrival_time < temp.arrival_time)
			lastArrival_time = temp.arrival_time;

		time_burst_sum += temp.burst_time;

		int time_temp = time_burst_sum + lastArrival_time;
		if (time < time_temp) {
			time = time_burst_sum + lastArrival_time;
		}
		

		origin_queue->pop();
	}

	for (int t = 0; t <= time; t++) {
		// tmp 큐에서 메모리 큐로 넣기
		int tmp_size = tmp.size();		// tmp 큐의 사이즈
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// 메모리 큐는 넣으면 arrival time이 작은 것 기준으로 자동 정렬이 된다.
				tmp.pop();
				//				printf("%d 초에 메모리 큐에 %s가 들어왔습니다. \n", t,tmp_process.name.c_str());
			}
		}


		// processor가 등록이 안되어있을 때는 메모리의 앞에 있는 것을 빼서 등록시킨다.
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.front();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.
														//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
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
				table_queue_tmp.push(processor);
				processor.setNull(true);
				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
			}

		}

	}

	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}
	return total_time + 1;
}

template <typename T>
int rr_process(T* origin_queue, T* table_queue, int quantum) {
	queue<Process> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;		// 총시간을 계산하기 위한 임시 저장 큐.( 어라이벌 타임으로 정렬이 된다.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// 테이블 큐에 넣기 위한 템프큐
	Process processor;										// 프로세서 등록
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// 전체 돌아간 총 시간.
	int count = 0;

										// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
	while (!origin_queue->empty()) {
		Process temp = origin_queue->front();
		tmp.push(temp);

		int lastArrival_time = 0;
		//2 4  0 8
		if (lastArrival_time < temp.arrival_time)
			lastArrival_time = temp.arrival_time;

		time_burst_sum += temp.burst_time;

		int time_temp = time_burst_sum + lastArrival_time;
		if (time < time_temp) {
			time = time_burst_sum + lastArrival_time;
		}
		origin_queue->pop();
	}

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
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.front();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.
														//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
				count = 0;
			}
		}
		if (!processor.isNull()) {
			if (count >= quantum) {
				if (!memory_queue.empty()) {
					Process temp_process = memory_queue.front();
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					//printf("%d초에 프로세스 %s가 메모리 큐에 있는 프로세서 %s와 교환하였습니다.", t, processor.name.c_str(), temp_process.name.c_str());
					processor = temp_process;
					processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록 (체인지 된 시간)
					count = 0;
					processor.running_time = 0;
				}
			}
			if (processor.remain_time > 0) {
				processor.running_time++;
				processor.remain_time--;
				count++;
			}
			
			if (processor.remain_time == 0) {
				processor.turnaround_time = t - processor.arrival_time + 1;
				processor.waiting_time = processor.turnaround_time - processor.burst_time;
				processor.normalized_time = (double)processor.turnaround_time / processor.burst_time;
				//				printf("%d 초에 프로세서 %s가 origin_queue에 들어왔습니다.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				processor.setNull(true);
				count = 0;
				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
			}

		}

	}

	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}
	return total_time + 1;
}
// SPN 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 SPN 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 이 함수만 개발하면 됨.
// 오리진 큐는 우선순위 큐가아닌 일반 적인 stl 큐를 사용
template <typename T>
int spn_process(T* origin_queue, T* table_queue) {						// table큐는 입력받은 순서대로 테이블을 보여줄 수 있는 큐
	priority_queue<Process, vector<Process>, spn> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;		// 총시간을 계산하기 위한 임시 저장 큐.( 어라이벌 타임으로 정렬이 된다.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// 테이블 큐에 넣기 위한 템프큐
	Process processor;										// 프로세서 등록
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// 전체 돌아간 총 시간.

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
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
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.
				//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
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
				table_queue_tmp.push(processor);
				processor.setNull(true);
				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
			}
			
		}
		
	}

	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}
	return total_time + 1;
}

// Strn 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 hrrn 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 오리진 큐는 우선순위 큐가아닌 일반 적인 stl 큐를 사용
// 비선점 형태이며 spn의 변형 방법이다. 프로세스의 대기 시간을 고려해서 기회를 제공함
// response ratio= (WT+BT) /BT (응답률)이 높은 프로세스를 우선으로 고려함.
template <typename T>
int srtn_process(T* origin_queue, T* table_queue) {
	priority_queue<Process, vector<Process>, srtn_comparison> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// 테이블 큐에 넣기 위한 템프큐
	Process processor;										// 프로세서 등록
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// 전체 돌아간 총 시간.

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
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
	//printf("time : %d\n", time);

	// time은 t변수 20초까지 카운트 한다.
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
		// 그리고 SRTN은 선점 프로세스 방식으로 등록된 메모리의 남은 remain time이 더 작으면 체인지. 
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.
				//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
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
					//printf("%d초에 프로세스 %s가 메모리 큐에 있는 프로세서 %s와 교환하였습니다.", t, processor.name.c_str(), temp_process.name.c_str());
					processor = temp_process;
					processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록 (체인지 된 시간)
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
				table_queue_tmp.push(processor);
				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
				processor.setNull(true);
			}

		}
	}

	//테이블 큐 에 넣는 작업.
	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}

	return total_time + 1;
}

// HRRN 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 HRRN 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 이 함수만 개발하면 됨.
// 오리진 큐는 우선순위 큐가아닌 일반 적인 stl 큐를 사용
template <typename T>
int hrrn_process(T* origin_queue, T* table_queue) {
	priority_queue<Process, vector<Process>, spn> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;
	priority_queue<Process, vector<Process>, hrrn> hrrn_queue;		// hrrn 적용하는 큐.
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// 테이블 큐에 넣기 위한 템프큐
	Process processor;										// 프로세서 등록
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// 전체 돌아간 총 시간.

	// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
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


	//printf("time : %d\n", time);

	//	while (!tmp.empty()) {
	//		cout << "임시 큐"<<tmp.top().arrival_time << endl;
	//		tmp.pop();
	//	}
	// time은 t변수 20초까지 카운트 한다.
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
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.
				//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
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
				table_queue_tmp.push(processor);
				processor.setNull(true);
				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
			}

		}

	}

	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}
	return total_time + 1;
}

// 단순히 que의 burst time 만 출력해주는 함수 (단순 테스트용 ) 
template <typename T>
void showing(T que , T table_queue) {
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
		printf("프로세스 키 : %d 프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d 프로세서 들어온 시간 : %d \n", p.number,p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time, p.process_arrival_time);
	}
}

// 주사위 프로세스 ( 우리만의 알고리즘 ) 
// 주사위 프로세스 각 매 초마다. 주사위를 굴려서 어떤 프로세스가 프로세서를 사용할지 결정한다.
template <typename T>
int dice_process(T* origin_queue, T* table_queue) {
	priority_queue<Process, vector<Process>, dice> memory_queue;		// 메모리 큐
	priority_queue<Process, vector<Process>, comparison> tmp;  // 총시간을 계산하기 위한 임시 저장 큐.( 어라이벌 타임으로 정렬이 된다.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// 테이블 큐에 넣기 위한 템프큐
	priority_queue<Process, vector<Process>, dice> dice_queue;
	Process processor;										// 프로세서 등록
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// 전체 돌아간 총 시간.

										// 원래 큐를 입력받아서 정렬시키면서 temp 우선순위 큐로 입력시킨다.
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
	//printf("time : %d\n", time);
	// 위쪽은 총 시간이 얼마나 걸리는지를 계산하는 알고리즘이다.

	// time은 t변수 20초까지 카운트 한다.
	for (int t = 0; t <= time; t++) {
		// tmp 큐에서 메모리 큐로 넣기
		int tmp_size = tmp.size();		// tmp 큐의 사이즈
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				tmp_process.rolling_dice();			// 프로세스 들어가기전 주사위를 굴린다.
				memory_queue.push(tmp_process);		// 메모리 큐에 넣으면 dice 숫자가 가장 큰거 순으로 정렬이 된다.
				tmp.pop();
				//				printf("%d 초에 메모리 큐에 %s가 들어왔습니다. \n", t,tmp_process.name.c_str());
			}
		}

		// 위의 코드는 arrival time이 t의 시간이 될때마다 memory_queue에 넣는다.
		queue_dice_rolling(&memory_queue, &dice_queue);
		

		// 주사위 굴리는 코드! (만약 주사위의 숫자가 큰것이 겹쳐서 나올 경우를 생각한다.)
		// processor가 등록이 안되어있을 때는 주사위를 굴려서 가장 큰 숫자가 나온 것을 출력한다. (하지만 겹친경우 또 다시 큰것끼리 주사위를 돌린다.)
		if (processor.isNull()) {
			// 메모리큐에서 프로세서로 등록시킨다.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// 초기화 시킨다.
				processor.process_arrival_time = t;		// 프로세서에 들어온 시간 기록.
														//printf("%d 초에 프로세서에 %s가 들어왔습니다. \n", t, processor.name.c_str());			// 문제... 왜 1초씩 땡겨서 들어가지는가?
			}
		}
		if (!processor.isNull()) {
			
			int processor_number = processor.number;

			//memory_queue.push(processor);		// 여기서 오류!! 넣고 이중 if문의 else 문에서 넣은 것을 빼주는 코드가 없다.
			// 프로세서에 있는 waiting time보다 메모리 큐의 remain time이 더 작으면 프로세서 큐는 메모리큐에 넣고. 메모리큐는 프로세서에 넣는다.

			queue_dice_rolling(&memory_queue, &dice_queue);			// 전체 memory 큐에 들어있는 값을 주사위로 돌려 정렬시킨다.
			if (!memory_queue.empty()) {
				if(processor.running_time !=0) {				// 다른 것이 떳을 때
					Process temp_process = memory_queue.top();
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					//printf("%d초에 프로세스 %s가 메모리 큐에 있는 프로세서 %s와 교환하였습니다.", t, processor.name.c_str(), temp_process.name.c_str());
					processor = temp_process;
					processor.running_time = 0;
					processor.process_arrival_time = t;
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
				table_queue_tmp.push(processor);
				if (t > total_time) {					// 전체 총 돌린 시간이 현재 시간보다 적으면 총 돌린 시간은 지금 시간으로 
					total_time = t;
				}
				processor.setNull(true);
			}

		}
	}

	//테이블 큐 에 넣는 작업.
	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}

	return total_time + 1;
}

// memory_queue에 들어있는 원소 하나 하나를 주사위를 굴려서 주사위의 눈이 내림차순으로 정렬되도록 한다. 
template <typename T>
void queue_dice_rolling(T* memory_queue , T* dice_queue) {
	int dice_max = 0;
	while (!memory_queue->empty()) {		// 모든 메모리 큐에 있는 내용을 꺼내봐서 주사위를 굴린 뒤 dice_max 값을 저장한다.
		Process dice_temp = memory_queue->top();
		dice_temp.rolling_dice();
		if (dice_max < dice_temp.dice_number) {
			dice_max = dice_temp.dice_number;
		}
		dice_queue->push(dice_temp);
		memory_queue->pop();
	}

	int count = 0;
	do {
		count = 0;		// dice 큐에 있는 프로세스가 겹칠때마다 카운터를 시킨다.
		int temp_dice_max = dice_max;
		while (!dice_queue->empty()) {	// 다이스 큐가 빌때까지
			Process dice_temp = dice_queue->top();
			if ((dice_max != dice_temp.dice_number) || (dice_queue->size() == 1)) {			// 만일 dice_max와 주사위 숫자가 다르면 출력하고자 하는 값이 아니므로.  메모리 큐에 넣는다.
				memory_queue->push(dice_temp);			// 메모리 큐에 넣는다.
				dice_queue->pop();
			}
			else {
				int rolling_temp = dice_temp.rolling_dice(dice_temp.dice_number);
				if (rolling_temp > temp_dice_max) {					// 최종 주사위 맥스값을 갱신
					temp_dice_max = rolling_temp;
				}
				dice_queue->push(dice_temp);
				count++;
				dice_queue->pop();
			}
		}
		dice_max = temp_dice_max;
	} while (count != 0);		// 만일 카운트가 0 이면 dice_queue에 최종 큰값의 주사위 수가 있으므로.
}