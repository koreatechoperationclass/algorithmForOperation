// 기본 형태
/*

https://twpower.github.io/93-how-to-use-priority_queue-in-cpp
priority_queue<T, Container, Compare> :원하는 자료형 및 클래스 T를 통해 생성, 여기서 Container는 vertor와 같은 컨테이너이며 Compare는 비교함수 클래스이다.

insert(element) :우선순위 큐에 원소 추가
pop() 우선순위 큐에서 top의 원소를 삭제

top() top의 있는 원소를 반환

empty() :비어있으면 true 아니면 false를 반환
size() :우선순위 큐에 포함되어 있는 원소들의 수를 반환

*/

#include <iostream>
#include <queue>

using namespace std;

int main() {

	// priority_queue
	priority_queue<int, vector<int>, less<int>> pq;
	// or priority_queue<int> pq;


	//push(element)
	pq.push(5);
	pq.push(2);
	pq.push(8);
	pq.push(9);
	pq.push(1);
	pq.push(14);
	


	//pop()
	pq.pop();
	pq.pop();

	//top();
	cout << "pq top : " << pq.top() << "\n";


	//empty() , size()
	if (!pq.empty()) cout << "pq size : " << pq.size() << "\n";

	while (!pq.empty()) {
		cout << pq.top() << " ";
		pq.pop();
	}

	cout << "\n";

	return 0;
}