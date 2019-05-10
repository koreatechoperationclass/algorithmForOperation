#include "algorithmForScheduling.h"

int main() {
	queue<Process> que;			// 가상의 입력의 큐
	queue<Process> table_que;		// 테이블 출력을 위한 큐
									//Process p{ "p1",0,2,3,4,5,6,7 };

									//printf("?프로세스 이름 : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name, p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);	//
	que.push(Process{ "p1",1,100,0,0,0,0,0 });
	que.push(Process{ "p2",10,20,0,0,0,0,0 });
	//que.push(Process{ "p3",5,2,0,0,0,0,0 });

	showing(que, table_que);

	printf("----------------- 그래프 출력 결과 -------------\n");
	printf("총 실행시간 : %d\n", rr_process(&que, &table_que, 4));			// 큐의 주소를 함수의 매개변수로 사용

	showing(que, table_que);

	return 0;
}