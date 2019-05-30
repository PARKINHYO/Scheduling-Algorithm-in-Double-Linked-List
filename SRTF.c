#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define TIME_QUANTUM 20 // 시간 할당량을 20으로 설정

typedef struct priority_queue* queue_pointer;
struct priority_queue {
	int pid; // 프로세스 id이다. 
	int priority; // 우선순위이다. 
	int computing_time; // 실행 시간이다. 
	int turn_around_time; // 프로세스가 들어와서 나올 때까지의 시간이다. 
	int remaining_time; // 남은 시간
	//처리한 양을 실시간으로 빼주면서 새로들어온 프로세스의 실행시간과 비교해주는 변수이다. 
	int arrived_time; // 도착 시간이다. 
	queue_pointer left_link, right_link; // 현재 프로세스 기준 양옆의 주소를 가르키는 변수이다. 
}priority_queue;

typedef struct queue_head* head_pointer;
struct queue_head {
	queue_pointer left_link;
	queue_pointer right_link; // 프로세스 큐의 헤드 역할이다. 
};

head_pointer queue; // 헤드를 위한 포인터이다. 
double sum_normalized_turn_around_time; // 정규화된 반환시간의 합을 구하기 위한 전역 변수이다.
int inter_time; // 프로세스가 스케줄링될때 상대적인 시간을 표현하기 위한 전역 변수이다. 

void initialize_queue();
int insert_queue(int pid, int priority, int computing_time, int arrived_time);
void scheduling_queue();
	
int main(void) {
	FILE* fp = NULL;
	int type, process_id, priority, computing_time;
	int arrived_time = 0;
	int r;
	int count = 0;
	fp = fopen("input4.txt", "r"); // 파일을 오픈해준다.(읽기 모드)
	if (fp == NULL) printf(" File open failed. \n");

	initialize_queue(); // 큐를 초기화한다. 
	inter_time = 0; // 시간을 0으로 설정한다.

	printf("\n");
	printf("┌────────────────────┐\n");
	printf("│운영체제 설계 과제물│\n");
	printf("│11조                │\n");
	printf("│2015154017 박인효   │\n");
	printf("└────────────────────┘\n");

	printf("\n\n<SRT 스케줄링>\n\n");
	printf("Process_id\tpriority\tcomputing_time\tarrived_time\tfinished_time\tturn_around_time\tnormalized_turn_around_time\n");
	puts("--------------------------------------------------------------------------------------------------------------------------------------");
	while (1)
	{
		r = fscanf(fp, "%d\t%d\t%d\t%d\n", &type, &process_id, &priority, &computing_time);
		if (r == EOF) {
			printf("파일이 비었습니다.\n");
			return 0;
		}
		//파일을 입력받고 type 값에 따라 큐에 프로세스를 넣을지 스케줄링을 진행할지 결정할지 결정한다.
		if (type == 0)
		{
			insert_queue(process_id, priority, computing_time, arrived_time); //type 값이 0이면 프로세스를 큐에 집어 넣는다.
			count++; // 생성된 프로세스의 개수를 세기위한 변수이다.
		}
		else if (type == 1) // type가 1이면 스케줄링을 진행한다. 
		{
			scheduling_queue(); // 스케줄링을 진행하기위한 함수이다.
			arrived_time += TIME_QUANTUM; // 다음번 프로세스가 들어오게 되면 도착시간을 20으로 설정해주기 위해 20더한다.
		}
		else if (type == -1)
		{
			while (queue->right_link != NULL) /*모든 프로세스가 computing_time이 짧은 순서로 정렬이 되어있고 큐에
											  프로세스가 없을때까지 계속 scheduling_aueue()를 호출해준다.*/
			{
				scheduling_queue();
			}
			break;
		}
	}
	puts("--------------------------------------------------------------------------------------------------------------------------------------");
	printf("normalized average turn_around time: %.5f \n\n", (double)sum_normalized_turn_around_time / count);
	fclose(fp); // 파일을 종료해준다.
	return 0;
}

void initialize_queue(void) {
	queue = (head_pointer)malloc(sizeof(head_pointer));
	queue->left_link = NULL;
	queue->right_link = NULL;
}

int insert_queue(int pid, int priority, int computing_time, int arrived_time) { // 가장 적은 시간순서로 정렬해준다.
	queue_pointer process;
	queue_pointer new_process = (queue_pointer)malloc(sizeof(priority_queue));
	new_process->left_link = (queue_pointer)malloc(sizeof(priority_queue));
	new_process->right_link = (queue_pointer)malloc(sizeof(priority_queue));
	// 프로세스 생성 및 초기화
	new_process->left_link = NULL;
	new_process->right_link = NULL;
	new_process->pid = pid;
	new_process->priority = priority;
	new_process->computing_time = computing_time;
	new_process->turn_around_time = 0;
	new_process->remaining_time = computing_time;
	new_process->arrived_time = arrived_time;

	if (queue->right_link == NULL) { // 큐에 프로세스가 한개도 없다면(queue->right_link는 헤드이다.)
		queue->right_link = new_process; // 헤드가 새로운 프로세스를 가리킨다.
		new_process->left_link = queue->right_link; // 새로운 프로세스의 왼쪽 링크가 자기 자신을 가리킨다.
		new_process->right_link = NULL;
	}
	else // 큐에 만약 이미 프로세스가 들어와 있으면
	{
		process = queue->right_link; // 헤드를 process 포인터 변수에 저장한다.
		if (process->remaining_time > computing_time) // 첫번째 프로세스의 남은 시간이 현재 들어온 서비스 시간보다 크면
		   //즉 remain_time의 오름차순으로 큐에 정렬이 되어있으면 무조건 제일 왼쪽에 삽입되어야한다.)
		{
			//큐의 제일 왼쪽 즉 첫번째 노드에 삽입하기 위한 과정
			new_process->left_link = queue->right_link; // 새로들어온 프로세스의 왼쪽 링크를 헤드로 설정시킨다.
			new_process->right_link = process; // 새로들어온 프로세스의 오른쪽 링크가 기존의 헤드를 가르킨다.
			queue->right_link = new_process; // 헤드가 새로운 프로세스를 가르키게 만든다.
			process->left_link = new_process;// 기존 헤드의 왼쪽 링크가 새로운 프로세스를 가르킨다.
		
		}
		else // 첫번째 프로세스의 남은 시간이 현재 들어온 서비스 시간보다 작거나 같으면
		{
			while (process->remaining_time <= computing_time) // 작거나 같을동안
			{
				if (process->right_link == NULL) /*(2). 만약 process가 끝에 도달해 더이상 오른쪽으로
												 이동할 곳이 없을 경우, 즉 큐의 맨끝에 삽입한다.*/
				{
					process->right_link = new_process;
					// 기존큐의 맨끝 프로세스의 오른쪽 링크가 새로운 들어온 프로세스를 가리킨다.
					new_process->left_link = process;
					// 새로들어온 프로세스의 왼쪽 링크가 기존큐의 프로세스를 가리킨다. 
					new_process->right_link = NULL;
					break;
				}
				process = process->right_link; // (1). 헤드가 저장되어 있는 변수 process를 한칸 씩 오른쪽으로 이동시키면서 바꿔준다.
			}
			//process->remaining_time > computing_time 이되면, 즉 연결리스트의 중간에 삽입이 되야 하는 경우!!!
			new_process->left_link = process->left_link;
			// 새로 들어온 프로세스의 왼쪽 링크가 들어가야 하는곳의 왼쪽 프로세스를 가르키게 한다.
			new_process->right_link = process;
			// 새로들어온 프로세스의 오른쪽 링크가 오른쪽 프로세스를 가르키게 한다.
			//process가 가르키는 프로세스 왼쪽에 들어가야 한다.)(현재 탐색된 프로세스를 가리키게 한다.
			process->left_link->right_link = new_process;
			// 새로들어온 프로세스 왼쪽 프로세스의 오른쪽 링크가 새로운 프로세스를 가르키게 한다.
			process->left_link = new_process;
			// 오른쪽 프로세스의 왼쪽 링크가 새로운 프로세스를 가르키게 한다.
		}
	}
	return 0;
}

void scheduling_queue(void) {
	queue_pointer process = (queue_pointer)malloc(sizeof(priority_queue));
	int counting = TIME_QUANTUM; //상대시간 20마다 스케줄링을 해줘야 되기 때문에 counting 변수에 20을 저장한다. 
	int tmp = 0;

	process = queue->right_link; // 헤드를 process 포인터 변수에 저장한다.
	tmp = process->remaining_time; // 큐에 들어있는 첫번째 노드의 남은시간을 tmp변수에 저장한다.

	/*반복을 해주는 이유
	  상대적인 시간 20만큼 스케줄링을 해주는 함수이다. 그러나 처리기의 서비스 시간이 20보다 작을 경우
	  처리가 완료하고 나머지 시간만큼 다음 프로세스를 처리해줘야한다. 그러므로 무한 반복문이 필요하다.*/
	while (1) {
		while (counting != 0) { // 상대시간 20만큼 처리기가 처리를 진행한다.
			tmp--; // 남은 시간을 1줄이고
			inter_time++; // 현재 시간을 1늘리고
			counting--; // 스케줄링 시간을 1줄인다.
			process->remaining_time = tmp; // 줄어든 남은시간을 remaining_time에 저장한다.
			if (tmp == 0) // 만약 tmp가 0이되면 즉 남은 시간이 0이되면(counting 변수가 0이되기전에) 빠져나온다.
				// 이말은 즉슨 남은시간이 스케줄링 시간 20보다 작다는 뜻이다
				break;
		}
		if (tmp == 0) { // 남은 시간이 0일떄, 즉 프로세스 한개가 처리가 완료되었다.
			process->turn_around_time = inter_time - process->arrived_time; // turn_around_time: 현재시간에서 도착한 시간을 빼준다.
			printf("%d \t \t %d \t \t %d \t \t %d \t \t %d \t \t %d \t \t \t %.2f\n", process->pid,
				process->priority, process->computing_time, process->arrived_time, inter_time, process->turn_around_time,
				(double)process->turn_around_time / (double)process->computing_time); // 정규화된 반환시간: turn_around_time/computing_time
			sum_normalized_turn_around_time += (double)process->turn_around_time / (double)process->computing_time;
			//모든 프로세스의 정규화된 반환시간의 평균을 구하기위해 전체 합을 구해준다.(전역 변수로 저장)
			if (queue->right_link->right_link == NULL) // 헤드의 다음이 없을때, 즉 큐가 비었다. 모든 프로세스의 처리가 완료되었다.
			{
				if (counting != 0)
				{
					while (counting != 0)
					{
						inter_time++;
						counting--;
					}
				}
				queue->right_link = NULL;
				process->left_link = NULL;
				process->right_link = NULL;
			}
			else
			{
				queue->right_link = process->right_link; // 다음 프로세스로 헤드를 지정해준다.
				process->right_link->left_link = queue->right_link; // 다음 프로세스의 왼쪽 링크가 헤드를 가르키게 한다. 
				process->left_link = NULL; // 왼쪽 링크를 삭제함으로써 현재 프로세스를 큐에서 삭제 시킨다.
				process->right_link = NULL; // 오른쪽 링크를 삭제함으로써 현재 프로세스를 큐에서 삭제 시킨다.

				process = queue->right_link; // process 변수가 다음 프로세스를 가르키게 한다.
				tmp = process->remaining_time; // 다음 프로세스의 남은 시간을 tmp변수에 저장
			}
		}
		if (counting == 0) // 20만큼 처리기가 처리를 완료했을 때 무한루프를 빠져 나온다.
			/*모든 프로세스가 완전히 다 들어오고(예제에서 상대시간 60이후 computing_time이 20보다 큰 프로세스를 만나면
			우선 20만큼 처리를 진행 함수를 빠져나오고 다시 함수를 호출해서 20만큼 처리를 해준다.*/
			break;
	}
}



