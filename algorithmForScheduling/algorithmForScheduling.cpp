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
	double arrival_time;			// 메모리에 들어간 시간
	double burst_time;				// 실행시간
	double waiting_time;			// 기다린 시간
	double turnaround_time;			// 총 실행시간
	double normalized_time;			// 정규화 시간
	Process(double arrival_time, double burst_time) : arrival_time(arrival_time), burst_time(burst_time), waiting_time(0), turnaround_time(0), normalized_time(0) {}
};

// SPN 프로세스 함수 
template <typename T>
void spn_process(T* aa);

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
	que.push(Process(3, 2));
	que.push(Process(5, 5));
	que.push(Process(6, 3));

	spn_process(&que);			// 큐의 주소를 함수의 매개변수로 사용
	
	showing(que);

	return 0;
}

// SPN 프로세스 함수 여기서는 큐의 포인터를 입력받고 그 큐에 SPN 프로세스 스케쥴링을 적용하여 대기열을 넣고 종료하는 함수
// 이 함수만 개발하면 됨.
template <typename T>
void spn_process(T* aa) {
	priority_queue<Process, vector<Process>, spn> pq;

}