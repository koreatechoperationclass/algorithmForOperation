/*
��¥ 19. 03 .25
�˰��� ��Ʈ : ȫ����, ������

�ۼ��� :ȫ����
SPN ���� �õ�
*/

#include <cstdio>
#include <iostream>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;
// ���μ��� ����ü
struct Process {
	static int count;				// ���μ����� ���� ���� ī����
	int number;						// ���μ��� �⺻Ű
	string name;					// ���μ����� �̸�
	int arrival_time;			// �޸𸮿� �� �ð�
	int burst_time;				// ����ð�
	int waiting_time;			// ��ٸ� �ð�
	int turnaround_time ;			// �� ����ð�
	double normalized_time;			// ����ȭ �ð�
	int running_time ;			// �����ð�
	int remain_time; 				// �����ϱ⿡ ���� �ð�.
	int process_arrival_time;		// ���μ����� ���� �ð�.
	int dice_number;				// �ֻ����� ũ�� 
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
		dice_number = 0;					// ó�� �����Ǹ� �ֻ��� ���ڴ� 0
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

int Process::count = 0;			//���μ��� ���� �� ���� ���� static count �� �߰��ؼ� ���μ����� �߰��Ѵ�. 

// SPN ���μ��� �Լ� 
template <typename T>
int spn_process(T* origin_queue, T* table_queue);

//SRTN ���μ��� �Լ�
template <typename T>
int srtn_process(T* origin_queue, T* table_queue);

//hrrn ���μ��� �Լ�
template <typename T>
int hrrn_process(T* origin_queue, T* table_queue);

template <typename T>
int dice_process(T* origin_queue, T* table_queue);

template <typename T>
int rr_process(T* origin_queue, T* table_queue, int quantum);

template <typename T>
int fcfs_process(T* origin_queue, T* table_queue);

// ���μ��� �� �Լ�,  �Է½ð����� �����ϴ� �Լ�
struct comparison {
	bool operator()(Process t, Process u) {
		return t.arrival_time > u.arrival_time;
	}
};

// srtn ���Լ�
struct srtn_comparison {
	bool operator()(Process t, Process u) {
		return t.remain_time > u.remain_time;
	}
};

// spn �� �Լ�
struct spn {
	bool operator()(Process t, Process u) {
		return t.burst_time > u.burst_time;
	}
};

// hrrn �� �Լ�
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

// Ű������ �����ϴ� �Լ�
struct key {
	bool operator()(Process t, Process u) {
		return t.number > u.number;
	}
};

// �ܼ��� que�� burst time �� ������ִ� �Լ� (�ܼ� �׽�Ʈ�� ) 
template <typename T>
void showing(T que , T table_queue);

template <typename T>
void queue_dice_rolling(T* memory_queue, T* dice_queue);

int main() {
	queue<Process> que;			// ������ �Է��� ť
	queue<Process> table_que;		// ���̺� ����� ���� ť
								//Process p{ "p1",0,2,3,4,5,6,7 };
	
	//printf("?���μ��� �̸� : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name, p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);	//
	que.push(Process{ "p1",1,1000,0,0,0,0,0 });
	//printf("?%s\n", p.name.c_str());
	//printf("?���μ��� �̸� : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %lf, running time : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time);
	que.push(Process{ "p2",10,20,0,0,0,0,0 });
	//que.push(Process{ "p3",3,2,0,0,0,0,0 });
	//que.push(Process{ "p4",5,5,0,0,0,0,0 });
	//que.push(Process{ "p5",6,3,0,0,0,0,0 });

	showing(que, table_que);

	printf("----------------- �׷��� ��� ��� -------------\n");
	printf("�� ����ð� : %d\n", srtn_process(&que, &table_que));			// ť�� �ּҸ� �Լ��� �Ű������� ���
	
	showing(que, table_que);

	return 0;
}

template <typename T>
int fcfs_process(T* origin_queue, T* table_queue) {
	queue<Process> memory_queue;		// �޸� ť
	priority_queue<Process, vector<Process>, comparison> tmp;		// �ѽð��� ����ϱ� ���� �ӽ� ���� ť.( ����̹� Ÿ������ ������ �ȴ�.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// ���̺� ť�� �ֱ� ���� ����ť
	Process processor;										// ���μ��� ���
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// ��ü ���ư� �� �ð�.

	// ���� ť�� �Է¹޾Ƽ� ���Ľ�Ű�鼭 temp �켱���� ť�� �Է½�Ų��.
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
		// tmp ť���� �޸� ť�� �ֱ�
		int tmp_size = tmp.size();		// tmp ť�� ������
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// �޸� ť�� ������ arrival time�� ���� �� �������� �ڵ� ������ �ȴ�.
				tmp.pop();
				//				printf("%d �ʿ� �޸� ť�� %s�� ���Խ��ϴ�. \n", t,tmp_process.name.c_str());
			}
		}


		// processor�� ����� �ȵǾ����� ���� �޸��� �տ� �ִ� ���� ���� ��Ͻ�Ų��.
		if (processor.isNull()) {
			// �޸�ť���� ���μ����� ��Ͻ�Ų��.
			if (!memory_queue.empty()) {
				processor = memory_queue.front();
				memory_queue.pop();
				processor.running_time = 0;			// �ʱ�ȭ ��Ų��.
				processor.process_arrival_time = t;		// ���μ����� ���� �ð� ���.
														//printf("%d �ʿ� ���μ����� %s�� ���Խ��ϴ�. \n", t, processor.name.c_str());			// ����... �� 1�ʾ� ���ܼ� �����°�?
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
				//				printf("%d �ʿ� ���μ��� %s�� origin_queue�� ���Խ��ϴ�.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				processor.setNull(true);
				if (t > total_time) {					// ��ü �� ���� �ð��� ���� �ð����� ������ �� ���� �ð��� ���� �ð����� 
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
	queue<Process> memory_queue;		// �޸� ť
	priority_queue<Process, vector<Process>, comparison> tmp;		// �ѽð��� ����ϱ� ���� �ӽ� ���� ť.( ����̹� Ÿ������ ������ �ȴ�.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// ���̺� ť�� �ֱ� ���� ����ť
	Process processor;										// ���μ��� ���
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// ��ü ���ư� �� �ð�.
	int count = 0;

										// ���� ť�� �Է¹޾Ƽ� ���Ľ�Ű�鼭 temp �켱���� ť�� �Է½�Ų��.
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
		// tmp ť���� �޸� ť�� �ֱ�
		int tmp_size = tmp.size();		// tmp ť�� ������
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// �޸� ť�� ������ burst time�� ���� �� �������� �ڵ� ������ �ȴ�.
				tmp.pop();
				//				printf("%d �ʿ� �޸� ť�� %s�� ���Խ��ϴ�. \n", t,tmp_process.name.c_str());
			}
		}


		// processor�� ����� �ȵǾ����� ���� �޸��� �տ� �ִ� ���� ���� ��Ͻ�Ų��.
		if (processor.isNull()) {
			// �޸�ť���� ���μ����� ��Ͻ�Ų��.
			if (!memory_queue.empty()) {
				processor = memory_queue.front();
				memory_queue.pop();
				processor.running_time = 0;			// �ʱ�ȭ ��Ų��.
				processor.process_arrival_time = t;		// ���μ����� ���� �ð� ���.
														//printf("%d �ʿ� ���μ����� %s�� ���Խ��ϴ�. \n", t, processor.name.c_str());			// ����... �� 1�ʾ� ���ܼ� �����°�?
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
					//printf("%d�ʿ� ���μ��� %s�� �޸� ť�� �ִ� ���μ��� %s�� ��ȯ�Ͽ����ϴ�.", t, processor.name.c_str(), temp_process.name.c_str());
					processor = temp_process;
					processor.process_arrival_time = t;		// ���μ����� ���� �ð� ��� (ü���� �� �ð�)
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
				//				printf("%d �ʿ� ���μ��� %s�� origin_queue�� ���Խ��ϴ�.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				processor.setNull(true);
				count = 0;
				if (t > total_time) {					// ��ü �� ���� �ð��� ���� �ð����� ������ �� ���� �ð��� ���� �ð����� 
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
// SPN ���μ��� �Լ� ���⼭�� ť�� �����͸� �Է¹ް� �� ť�� SPN ���μ��� �����층�� �����Ͽ� ��⿭�� �ְ� �����ϴ� �Լ�
// �� �Լ��� �����ϸ� ��.
// ������ ť�� �켱���� ť���ƴ� �Ϲ� ���� stl ť�� ���
template <typename T>
int spn_process(T* origin_queue, T* table_queue) {						// tableť�� �Է¹��� ������� ���̺��� ������ �� �ִ� ť
	priority_queue<Process, vector<Process>, spn> memory_queue;		// �޸� ť
	priority_queue<Process, vector<Process>, comparison> tmp;		// �ѽð��� ����ϱ� ���� �ӽ� ���� ť.( ����̹� Ÿ������ ������ �ȴ�.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// ���̺� ť�� �ֱ� ���� ����ť
	Process processor;										// ���μ��� ���
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// ��ü ���ư� �� �ð�.

	// ���� ť�� �Է¹޾Ƽ� ���Ľ�Ű�鼭 temp �켱���� ť�� �Է½�Ų��.
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
	
	// time�� t���� time�ʱ��� ī��Ʈ �Ѵ�.
	for (int t = 0; t <= time; t++) {
		// tmp ť���� �޸� ť�� �ֱ�
		int tmp_size = tmp.size();		// tmp ť�� ������
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// �޸� ť�� ������ burst time�� ���� �� �������� �ڵ� ������ �ȴ�.
				tmp.pop();
//				printf("%d �ʿ� �޸� ť�� %s�� ���Խ��ϴ�. \n", t,tmp_process.name.c_str());
			}
		}


		// processor�� ����� �ȵǾ����� ���� �޸��� �տ� �ִ� ���� ���� ��Ͻ�Ų��.
		if (processor.isNull()) {
			// �޸�ť���� ���μ����� ��Ͻ�Ų��.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// �ʱ�ȭ ��Ų��.
				processor.process_arrival_time = t;		// ���μ����� ���� �ð� ���.
				//printf("%d �ʿ� ���μ����� %s�� ���Խ��ϴ�. \n", t, processor.name.c_str());			// ����... �� 1�ʾ� ���ܼ� �����°�?
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
//				printf("%d �ʿ� ���μ��� %s�� origin_queue�� ���Խ��ϴ�.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				processor.setNull(true);
				if (t > total_time) {					// ��ü �� ���� �ð��� ���� �ð����� ������ �� ���� �ð��� ���� �ð����� 
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

// Strn ���μ��� �Լ� ���⼭�� ť�� �����͸� �Է¹ް� �� ť�� hrrn ���μ��� �����층�� �����Ͽ� ��⿭�� �ְ� �����ϴ� �Լ�
// ������ ť�� �켱���� ť���ƴ� �Ϲ� ���� stl ť�� ���
// ���� �����̸� spn�� ���� ����̴�. ���μ����� ��� �ð��� ����ؼ� ��ȸ�� ������
// response ratio= (WT+BT) /BT (�����)�� ���� ���μ����� �켱���� �����.
template <typename T>
int srtn_process(T* origin_queue, T* table_queue) {
	priority_queue<Process, vector<Process>, srtn_comparison> memory_queue;		// �޸� ť
	priority_queue<Process, vector<Process>, comparison> tmp;
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// ���̺� ť�� �ֱ� ���� ����ť
	Process processor;										// ���μ��� ���
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// ��ü ���ư� �� �ð�.

	// ���� ť�� �Է¹޾Ƽ� ���Ľ�Ű�鼭 temp �켱���� ť�� �Է½�Ų��.
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

	// time�� t���� 20�ʱ��� ī��Ʈ �Ѵ�.
	for (int t = 0; t <= time; t++) {
		// tmp ť���� �޸� ť�� �ֱ�
		int tmp_size = tmp.size();		// tmp ť�� ������
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// �޸� ť�� ������ burst time�� ���� �� �������� �ڵ� ������ �ȴ�.
				tmp.pop();
				//				printf("%d �ʿ� �޸� ť�� %s�� ���Խ��ϴ�. \n", t,tmp_process.name.c_str());
			}
		}

		// processor�� ����� �ȵǾ����� ���� �޸��� �տ� �ִ� ���� ���� ��Ͻ�Ų��.
		// �׸��� SRTN�� ���� ���μ��� ������� ��ϵ� �޸��� ���� remain time�� �� ������ ü����. 
		if (processor.isNull()) {
			// �޸�ť���� ���μ����� ��Ͻ�Ų��.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// �ʱ�ȭ ��Ų��.
				processor.process_arrival_time = t;		// ���μ����� ���� �ð� ���.
				//printf("%d �ʿ� ���μ����� %s�� ���Խ��ϴ�. \n", t, processor.name.c_str());			// ����... �� 1�ʾ� ���ܼ� �����°�?
			}
		}
		if (!processor.isNull()) {
			// ���μ����� �ִ� waiting time���� �޸� ť�� remain time�� �� ������ ���μ��� ť�� �޸�ť�� �ְ�. �޸�ť�� ���μ����� �ִ´�.
			if (!memory_queue.empty()) {
				Process temp_process = memory_queue.top();
				if (temp_process.remain_time < processor.remain_time) {
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					//printf("%d�ʿ� ���μ��� %s�� �޸� ť�� �ִ� ���μ��� %s�� ��ȯ�Ͽ����ϴ�.", t, processor.name.c_str(), temp_process.name.c_str());
					processor = temp_process;
					processor.process_arrival_time = t;		// ���μ����� ���� �ð� ��� (ü���� �� �ð�)
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
				//				printf("%d �ʿ� ���μ��� %s�� origin_queue�� ���Խ��ϴ�.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				if (t > total_time) {					// ��ü �� ���� �ð��� ���� �ð����� ������ �� ���� �ð��� ���� �ð����� 
					total_time = t;
				}
				processor.setNull(true);
			}

		}
	}

	//���̺� ť �� �ִ� �۾�.
	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}

	return total_time + 1;
}

// HRRN ���μ��� �Լ� ���⼭�� ť�� �����͸� �Է¹ް� �� ť�� HRRN ���μ��� �����층�� �����Ͽ� ��⿭�� �ְ� �����ϴ� �Լ�
// �� �Լ��� �����ϸ� ��.
// ������ ť�� �켱���� ť���ƴ� �Ϲ� ���� stl ť�� ���
template <typename T>
int hrrn_process(T* origin_queue, T* table_queue) {
	priority_queue<Process, vector<Process>, spn> memory_queue;		// �޸� ť
	priority_queue<Process, vector<Process>, comparison> tmp;
	priority_queue<Process, vector<Process>, hrrn> hrrn_queue;		// hrrn �����ϴ� ť.
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// ���̺� ť�� �ֱ� ���� ����ť
	Process processor;										// ���μ��� ���
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// ��ü ���ư� �� �ð�.

	// ���� ť�� �Է¹޾Ƽ� ���Ľ�Ű�鼭 temp �켱���� ť�� �Է½�Ų��.
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
	//		cout << "�ӽ� ť"<<tmp.top().arrival_time << endl;
	//		tmp.pop();
	//	}
	// time�� t���� 20�ʱ��� ī��Ʈ �Ѵ�.
	for (int t = 0; t <= time; t++) {
		// tmp ť���� �޸� ť�� �ֱ�
		int tmp_size = tmp.size();		// tmp ť�� ������
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				memory_queue.push(tmp_process);		// �޸� ť�� ������ burst time�� ���� �� �������� �ڵ� ������ �ȴ�.
				tmp.pop();
				//				printf("%d �ʿ� �޸� ť�� %s�� ���Խ��ϴ�. \n", t,tmp_process.name.c_str());
			}
		}

		// processor�� ����� �ȵǾ����� ���� �޸��� �տ� �ִ� ���� ���� ��Ͻ�Ų��.
		if (processor.isNull()) {

			// �޸� ť�� �ִ� ���� hrrn ť��(hrrn ���μ��� ������ ������ �ٽ� ���Ľ�Ų��.)
			while (!memory_queue.empty()) {
				Process temp_process = memory_queue.top();
				temp_process.waiting_time = t - temp_process.arrival_time;
				hrrn_queue.push(temp_process);
				memory_queue.pop();
			}

			// hrrnť���� ���μ����� ��Ͻ�Ų��.
			if (!hrrn_queue.empty()) {
				processor = hrrn_queue.top();
				hrrn_queue.pop();
				processor.running_time = 0;			// �ʱ�ȭ ��Ų��.
				processor.process_arrival_time = t;		// ���μ����� ���� �ð� ���.
				//printf("%d �ʿ� ���μ����� %s�� ���Խ��ϴ�. \n", t, processor.name.c_str());			// ����... �� 1�ʾ� ���ܼ� �����°�?
			}

			// �����ִ� hrrn ť�� �ٽ� �޸� ť�� �ִ´�.
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
				//				printf("%d �ʿ� ���μ��� %s�� origin_queue�� ���Խ��ϴ�.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				processor.setNull(true);
				if (t > total_time) {					// ��ü �� ���� �ð��� ���� �ð����� ������ �� ���� �ð��� ���� �ð����� 
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

// �ܼ��� que�� burst time �� ������ִ� �Լ� (�ܼ� �׽�Ʈ�� ) 
template <typename T>
void showing(T que , T table_queue) {
	while (!que.empty()) {
		Process p = que.front();
		que.pop();
		printf("���μ��� �̸� : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d ���μ��� ���� �ð� : %d \n", p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time, p.process_arrival_time);
	}
	if (!table_queue.empty()) {
		printf("----------------���̺� ��� ----------------------\n");
	}
	
	while (!table_queue.empty()) {
		Process p = table_queue.front();
		table_queue.pop();
		printf("���μ��� Ű : %d ���μ��� �̸� : %s arrive time : %d Burst Time : %d , Waiting time : %d , Turnaround Time : %d , normalized time %.2lf, running time : %d ���μ��� ���� �ð� : %d \n", p.number,p.name.c_str(), p.arrival_time, p.burst_time, p.waiting_time, p.turnaround_time, p.normalized_time, p.running_time, p.process_arrival_time);
	}
}

// �ֻ��� ���μ��� ( �츮���� �˰��� ) 
// �ֻ��� ���μ��� �� �� �ʸ���. �ֻ����� ������ � ���μ����� ���μ����� ������� �����Ѵ�.
template <typename T>
int dice_process(T* origin_queue, T* table_queue) {
	priority_queue<Process, vector<Process>, dice> memory_queue;		// �޸� ť
	priority_queue<Process, vector<Process>, comparison> tmp;  // �ѽð��� ����ϱ� ���� �ӽ� ���� ť.( ����̹� Ÿ������ ������ �ȴ�.)
	priority_queue<Process, vector<Process>, key> table_queue_tmp;	// ���̺� ť�� �ֱ� ���� ����ť
	priority_queue<Process, vector<Process>, dice> dice_queue;
	Process processor;										// ���μ��� ���
	Process tmp_process;
	int time = 0;
	int time_burst_sum = 0;
	int total_time = 0;					// ��ü ���ư� �� �ð�.

										// ���� ť�� �Է¹޾Ƽ� ���Ľ�Ű�鼭 temp �켱���� ť�� �Է½�Ų��.
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
	// ������ �� �ð��� �󸶳� �ɸ������� ����ϴ� �˰����̴�.

	// time�� t���� 20�ʱ��� ī��Ʈ �Ѵ�.
	for (int t = 0; t <= time; t++) {
		// tmp ť���� �޸� ť�� �ֱ�
		int tmp_size = tmp.size();		// tmp ť�� ������
		for (int i = 0; i < tmp_size; i++) {
			int arr_time = (int)tmp.top().arrival_time;
			if (arr_time == t) {
				tmp_process = tmp.top();
				tmp_process.rolling_dice();			// ���μ��� ������ �ֻ����� ������.
				memory_queue.push(tmp_process);		// �޸� ť�� ������ dice ���ڰ� ���� ū�� ������ ������ �ȴ�.
				tmp.pop();
				//				printf("%d �ʿ� �޸� ť�� %s�� ���Խ��ϴ�. \n", t,tmp_process.name.c_str());
			}
		}

		// ���� �ڵ�� arrival time�� t�� �ð��� �ɶ����� memory_queue�� �ִ´�.
		queue_dice_rolling(&memory_queue, &dice_queue);
		

		// �ֻ��� ������ �ڵ�! (���� �ֻ����� ���ڰ� ū���� ���ļ� ���� ��츦 �����Ѵ�.)
		// processor�� ����� �ȵǾ����� ���� �ֻ����� ������ ���� ū ���ڰ� ���� ���� ����Ѵ�. (������ ��ģ��� �� �ٽ� ū�ͳ��� �ֻ����� ������.)
		if (processor.isNull()) {
			// �޸�ť���� ���μ����� ��Ͻ�Ų��.
			if (!memory_queue.empty()) {
				processor = memory_queue.top();
				memory_queue.pop();
				processor.running_time = 0;			// �ʱ�ȭ ��Ų��.
				processor.process_arrival_time = t;		// ���μ����� ���� �ð� ���.
														//printf("%d �ʿ� ���μ����� %s�� ���Խ��ϴ�. \n", t, processor.name.c_str());			// ����... �� 1�ʾ� ���ܼ� �����°�?
			}
		}
		if (!processor.isNull()) {
			
			int processor_number = processor.number;

			//memory_queue.push(processor);		// ���⼭ ����!! �ְ� ���� if���� else ������ ���� ���� ���ִ� �ڵ尡 ����.
			// ���μ����� �ִ� waiting time���� �޸� ť�� remain time�� �� ������ ���μ��� ť�� �޸�ť�� �ְ�. �޸�ť�� ���μ����� �ִ´�.

			queue_dice_rolling(&memory_queue, &dice_queue);			// ��ü memory ť�� ����ִ� ���� �ֻ����� ���� ���Ľ�Ų��.
			if (!memory_queue.empty()) {
				if(processor.running_time !=0) {				// �ٸ� ���� ���� ��
					Process temp_process = memory_queue.top();
					memory_queue.pop();
					memory_queue.push(processor);
					origin_queue->push(processor);
					//printf("%d�ʿ� ���μ��� %s�� �޸� ť�� �ִ� ���μ��� %s�� ��ȯ�Ͽ����ϴ�.", t, processor.name.c_str(), temp_process.name.c_str());
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
				//				printf("%d �ʿ� ���μ��� %s�� origin_queue�� ���Խ��ϴ�.\n ", t, processor.name.c_str());
				origin_queue->push(processor);
				table_queue_tmp.push(processor);
				if (t > total_time) {					// ��ü �� ���� �ð��� ���� �ð����� ������ �� ���� �ð��� ���� �ð����� 
					total_time = t;
				}
				processor.setNull(true);
			}

		}
	}

	//���̺� ť �� �ִ� �۾�.
	while (!table_queue_tmp.empty()) {
		table_queue->push(table_queue_tmp.top());
		table_queue_tmp.pop();
	}

	return total_time + 1;
}

// memory_queue�� ����ִ� ���� �ϳ� �ϳ��� �ֻ����� ������ �ֻ����� ���� ������������ ���ĵǵ��� �Ѵ�. 
template <typename T>
void queue_dice_rolling(T* memory_queue , T* dice_queue) {
	int dice_max = 0;
	while (!memory_queue->empty()) {		// ��� �޸� ť�� �ִ� ������ �������� �ֻ����� ���� �� dice_max ���� �����Ѵ�.
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
		count = 0;		// dice ť�� �ִ� ���μ����� ��ĥ������ ī���͸� ��Ų��.
		int temp_dice_max = dice_max;
		while (!dice_queue->empty()) {	// ���̽� ť�� ��������
			Process dice_temp = dice_queue->top();
			if ((dice_max != dice_temp.dice_number) || (dice_queue->size() == 1)) {			// ���� dice_max�� �ֻ��� ���ڰ� �ٸ��� ����ϰ��� �ϴ� ���� �ƴϹǷ�.  �޸� ť�� �ִ´�.
				memory_queue->push(dice_temp);			// �޸� ť�� �ִ´�.
				dice_queue->pop();
			}
			else {
				int rolling_temp = dice_temp.rolling_dice(dice_temp.dice_number);
				if (rolling_temp > temp_dice_max) {					// ���� �ֻ��� �ƽ����� ����
					temp_dice_max = rolling_temp;
				}
				dice_queue->push(dice_temp);
				count++;
				dice_queue->pop();
			}
		}
		dice_max = temp_dice_max;
	} while (count != 0);		// ���� ī��Ʈ�� 0 �̸� dice_queue�� ���� ū���� �ֻ��� ���� �����Ƿ�.
}