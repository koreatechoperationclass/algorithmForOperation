#운영체제 알고리즘 완성
-------
# 사용법

algorithForSchduling.h를 include해서 사용
--------------
# Process 구조체 내부
1. number : 기본키
2. name :프로세스의 이름
3. arrival_time : 메모리에 들어간 시간
4. burst_time : 실행시간
5. waiting_time : 기다린 시간
6. turnaround_time : 총 실행시간
7. normalized_time : 정규화 시간
8. running_time : 돌아간 시간
9. process_arrival_time : 프로세서에 들어온 시간

--------------------------------
# 프로세싱 함수 인터페이스
origin_queue : 입력부에서 입력받은 큐 (나중에 간트 차트 자료구조를 입력받는 큐)
table_queue : 테이블 출력을 받기위한 큐
1. int fcfs_process((stl queue) origin_queue , (stl queue) table_queue);

2. int rr_process(origin_queue, table_queue);
3. int spn_process(origin_queue, table_queue);
4. int srtn_process(origin_queue, table_queue);
5. int hrrn_process(origin_queue, table_queue);
6. int dice_process(origin_queue, table_queue);

각 함수의 출력 값은 **총 실행 시간**을 출력함
origin_queue는 나중에 출력이 간트 차트의 값으로 출력받음.


by 홍영주
