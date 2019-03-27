/*
날짜 19. 03 .25
알고리즘 파트 : 홍영주, 허윤석

작성자 :홍영주
SPN 구현 시도
*/

#include <iostream>
#include <queue>

using namespace std;
// 프로세스 구조체
struct Process {
	string name;					// 프로세스의 이름
	double arrival_time;			// 메모리에 들어간 시간
	double burst_time;				// 실행시간
	double waiting_time;			// 기다린 시간
	double turnaround_time;			// 총 실행시간
	double normalized_time;			// 정규화 시간
	double running_time;			// 돌린시간
	Process(double arrival_time, double burst_time) : arrival_time(arrival_time), burst_time(burst_time), waiting_time(0), turnaround_time(0), normalized_time(0) {}
};

// SPN 프로세스 함수 
template <typename T>
void spn_process(T* aa);

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

// 단순히 que의 burst time 만 출력해주는 함수 (단순 테스트용 ) 
template <typename T>
void showing(T que) {
	while (!que.empty()) {
		Process p = que.front();
		que.pop();
		cout << p.burst_time << endl;
	}
}

int main() {
	queue<Process> que;			// 가상의 입력의 큐
	que.push(Process(0, 3));
	que.push(Process(1, 7));
	que.push(Process(5, 2));
	que.push(Process(3, 5));
	que.push(Process(6, 3));

	spn_process(&que);			// 큐의 주소를 함수의 매개변수로 사용
	
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
		int arr_time = tmp.top().arrival_time;
		if (arr_time == t) {
			memory_queue.push(tmp.top);
			tmp.pop();

		}
	}
}