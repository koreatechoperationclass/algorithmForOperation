#include "algorithmForScheduling.h"

int main() {
	queue<Process> que;			// ������ �Է��� ť
	queue<Process> table_que;		// ���̺� ����� ���� ť
									//Process p{ "p1",0,2,3,4,5,6,7 };

									//printf("?���μ��� �̸� : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name, p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);	//
	que.push(Process{ "p1",1,100,0,0,0,0,0 });
	que.push(Process{ "p2",10,20,0,0,0,0,0 });
	//que.push(Process{ "p3",5,2,0,0,0,0,0 });

	showing(que, table_que);

	printf("----------------- �׷��� ��� ��� -------------\n");
	printf("�� ����ð� : %d\n", rr_process(&que, &table_que, 4));			// ť�� �ּҸ� �Լ��� �Ű������� ���

	showing(que, table_que);

	return 0;
}