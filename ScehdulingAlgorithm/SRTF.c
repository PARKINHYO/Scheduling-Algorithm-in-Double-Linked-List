#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define TIME_QUANTUM 20 // �ð� �Ҵ緮�� 20���� ����

typedef struct priority_queue* queue_pointer;
struct priority_queue {
	int pid; // ���μ��� id�̴�. 
	int priority; // �켱�����̴�. 
	int computing_time; // ���� �ð��̴�. 
	int turn_around_time; // ���μ����� ���ͼ� ���� �������� �ð��̴�. 
	int remaining_time; // ���� �ð�
	//ó���� ���� �ǽð����� ���ָ鼭 ���ε��� ���μ����� ����ð��� �����ִ� �����̴�. 
	int arrived_time; // ���� �ð��̴�. 
	queue_pointer left_link, right_link; // ���� ���μ��� ���� �翷�� �ּҸ� ����Ű�� �����̴�. 
}priority_queue;

typedef struct queue_head* head_pointer;
struct queue_head {
	queue_pointer left_link;
	queue_pointer right_link; // ���μ��� ť�� ��� �����̴�. 
};

head_pointer queue; // ��带 ���� �������̴�. 
double sum_normalized_turn_around_time; // ����ȭ�� ��ȯ�ð��� ���� ���ϱ� ���� ���� �����̴�.
int inter_time; // ���μ����� �����ٸ��ɶ� ������� �ð��� ǥ���ϱ� ���� ���� �����̴�. 

void initialize_queue();
int insert_queue(int pid, int priority, int computing_time, int arrived_time);
void scheduling_queue();
	
int main(void) {
	FILE* fp = NULL;
	int type, process_id, priority, computing_time;
	int arrived_time = 0;
	int r;
	int count = 0;
	fp = fopen("input4.txt", "r"); // ������ �������ش�.(�б� ���)
	if (fp == NULL) printf(" File open failed. \n");

	initialize_queue(); // ť�� �ʱ�ȭ�Ѵ�. 
	inter_time = 0; // �ð��� 0���� �����Ѵ�.

	printf("\n");
	printf("��������������������������������������������\n");
	printf("���ü�� ���� ��������\n");
	printf("��11��                ��\n");
	printf("��2015154017 ����ȿ   ��\n");
	printf("��������������������������������������������\n");

	printf("\n\n<SRT �����ٸ�>\n\n");
	printf("Process_id\tpriority\tcomputing_time\tarrived_time\tfinished_time\tturn_around_time\tnormalized_turn_around_time\n");
	puts("--------------------------------------------------------------------------------------------------------------------------------------");
	while (1)
	{
		r = fscanf(fp, "%d\t%d\t%d\t%d\n", &type, &process_id, &priority, &computing_time);
		if (r == EOF) {
			printf("������ ������ϴ�.\n");
			return 0;
		}
		//������ �Է¹ް� type ���� ���� ť�� ���μ����� ������ �����ٸ��� �������� �������� �����Ѵ�.
		if (type == 0)
		{
			insert_queue(process_id, priority, computing_time, arrived_time); //type ���� 0�̸� ���μ����� ť�� ���� �ִ´�.
			count++; // ������ ���μ����� ������ �������� �����̴�.
		}
		else if (type == 1) // type�� 1�̸� �����ٸ��� �����Ѵ�. 
		{
			scheduling_queue(); // �����ٸ��� �����ϱ����� �Լ��̴�.
			arrived_time += TIME_QUANTUM; // ������ ���μ����� ������ �Ǹ� �����ð��� 20���� �������ֱ� ���� 20���Ѵ�.
		}
		else if (type == -1)
		{
			while (queue->right_link != NULL) /*��� ���μ����� computing_time�� ª�� ������ ������ �Ǿ��ְ� ť��
											  ���μ����� ���������� ��� scheduling_aueue()�� ȣ�����ش�.*/
			{
				scheduling_queue();
			}
			break;
		}
	}
	puts("--------------------------------------------------------------------------------------------------------------------------------------");
	printf("normalized average turn_around time: %.5f \n\n", (double)sum_normalized_turn_around_time / count);
	fclose(fp); // ������ �������ش�.
	return 0;
}

void initialize_queue(void) {
	queue = (head_pointer)malloc(sizeof(head_pointer));
	queue->left_link = NULL;
	queue->right_link = NULL;
}

int insert_queue(int pid, int priority, int computing_time, int arrived_time) { // ���� ���� �ð������� �������ش�.
	queue_pointer process;
	queue_pointer new_process = (queue_pointer)malloc(sizeof(priority_queue));
	new_process->left_link = (queue_pointer)malloc(sizeof(priority_queue));
	new_process->right_link = (queue_pointer)malloc(sizeof(priority_queue));
	// ���μ��� ���� �� �ʱ�ȭ
	new_process->left_link = NULL;
	new_process->right_link = NULL;
	new_process->pid = pid;
	new_process->priority = priority;
	new_process->computing_time = computing_time;
	new_process->turn_around_time = 0;
	new_process->remaining_time = computing_time;
	new_process->arrived_time = arrived_time;

	if (queue->right_link == NULL) { // ť�� ���μ����� �Ѱ��� ���ٸ�(queue->right_link�� ����̴�.)
		queue->right_link = new_process; // ��尡 ���ο� ���μ����� ����Ų��.
		new_process->left_link = queue->right_link; // ���ο� ���μ����� ���� ��ũ�� �ڱ� �ڽ��� ����Ų��.
		new_process->right_link = NULL;
	}
	else // ť�� ���� �̹� ���μ����� ���� ������
	{
		process = queue->right_link; // ��带 process ������ ������ �����Ѵ�.
		if (process->remaining_time > computing_time) // ù��° ���μ����� ���� �ð��� ���� ���� ���� �ð����� ũ��
		   //�� remain_time�� ������������ ť�� ������ �Ǿ������� ������ ���� ���ʿ� ���ԵǾ���Ѵ�.)
		{
			//ť�� ���� ���� �� ù��° ��忡 �����ϱ� ���� ����
			new_process->left_link = queue->right_link; // ���ε��� ���μ����� ���� ��ũ�� ���� ������Ų��.
			new_process->right_link = process; // ���ε��� ���μ����� ������ ��ũ�� ������ ��带 ����Ų��.
			queue->right_link = new_process; // ��尡 ���ο� ���μ����� ����Ű�� �����.
			process->left_link = new_process;// ���� ����� ���� ��ũ�� ���ο� ���μ����� ����Ų��.
		
		}
		else // ù��° ���μ����� ���� �ð��� ���� ���� ���� �ð����� �۰ų� ������
		{
			while (process->remaining_time <= computing_time) // �۰ų� ��������
			{
				if (process->right_link == NULL) /*(2). ���� process�� ���� ������ ���̻� ����������
												 �̵��� ���� ���� ���, �� ť�� �ǳ��� �����Ѵ�.*/
				{
					process->right_link = new_process;
					// ����ť�� �ǳ� ���μ����� ������ ��ũ�� ���ο� ���� ���μ����� ����Ų��.
					new_process->left_link = process;
					// ���ε��� ���μ����� ���� ��ũ�� ����ť�� ���μ����� ����Ų��. 
					new_process->right_link = NULL;
					break;
				}
				process = process->right_link; // (1). ��尡 ����Ǿ� �ִ� ���� process�� ��ĭ �� ���������� �̵���Ű�鼭 �ٲ��ش�.
			}
			//process->remaining_time > computing_time �̵Ǹ�, �� ���Ḯ��Ʈ�� �߰��� ������ �Ǿ� �ϴ� ���!!!
			new_process->left_link = process->left_link;
			// ���� ���� ���μ����� ���� ��ũ�� ���� �ϴ°��� ���� ���μ����� ����Ű�� �Ѵ�.
			new_process->right_link = process;
			// ���ε��� ���μ����� ������ ��ũ�� ������ ���μ����� ����Ű�� �Ѵ�.
			//process�� ����Ű�� ���μ��� ���ʿ� ���� �Ѵ�.)(���� Ž���� ���μ����� ����Ű�� �Ѵ�.
			process->left_link->right_link = new_process;
			// ���ε��� ���μ��� ���� ���μ����� ������ ��ũ�� ���ο� ���μ����� ����Ű�� �Ѵ�.
			process->left_link = new_process;
			// ������ ���μ����� ���� ��ũ�� ���ο� ���μ����� ����Ű�� �Ѵ�.
		}
	}
	return 0;
}

void scheduling_queue(void) {
	queue_pointer process = (queue_pointer)malloc(sizeof(priority_queue));
	int counting = TIME_QUANTUM; //���ð� 20���� �����ٸ��� ����� �Ǳ� ������ counting ������ 20�� �����Ѵ�. 
	int tmp = 0;

	process = queue->right_link; // ��带 process ������ ������ �����Ѵ�.
	tmp = process->remaining_time; // ť�� ����ִ� ù��° ����� �����ð��� tmp������ �����Ѵ�.

	/*�ݺ��� ���ִ� ����
	  ������� �ð� 20��ŭ �����ٸ��� ���ִ� �Լ��̴�. �׷��� ó������ ���� �ð��� 20���� ���� ���
	  ó���� �Ϸ��ϰ� ������ �ð���ŭ ���� ���μ����� ó��������Ѵ�. �׷��Ƿ� ���� �ݺ����� �ʿ��ϴ�.*/
	while (1) {
		while (counting != 0) { // ���ð� 20��ŭ ó���Ⱑ ó���� �����Ѵ�.
			tmp--; // ���� �ð��� 1���̰�
			inter_time++; // ���� �ð��� 1�ø���
			counting--; // �����ٸ� �ð��� 1���δ�.
			process->remaining_time = tmp; // �پ�� �����ð��� remaining_time�� �����Ѵ�.
			if (tmp == 0) // ���� tmp�� 0�̵Ǹ� �� ���� �ð��� 0�̵Ǹ�(counting ������ 0�̵Ǳ�����) �������´�.
				// �̸��� �ｼ �����ð��� �����ٸ� �ð� 20���� �۴ٴ� ���̴�
				break;
		}
		if (tmp == 0) { // ���� �ð��� 0�ϋ�, �� ���μ��� �Ѱ��� ó���� �Ϸ�Ǿ���.
			process->turn_around_time = inter_time - process->arrived_time; // turn_around_time: ����ð����� ������ �ð��� ���ش�.
			printf("%d \t \t %d \t \t %d \t \t %d \t \t %d \t \t %d \t \t \t %.2f\n", process->pid,
				process->priority, process->computing_time, process->arrived_time, inter_time, process->turn_around_time,
				(double)process->turn_around_time / (double)process->computing_time); // ����ȭ�� ��ȯ�ð�: turn_around_time/computing_time
			sum_normalized_turn_around_time += (double)process->turn_around_time / (double)process->computing_time;
			//��� ���μ����� ����ȭ�� ��ȯ�ð��� ����� ���ϱ����� ��ü ���� �����ش�.(���� ������ ����)
			if (queue->right_link->right_link == NULL) // ����� ������ ������, �� ť�� �����. ��� ���μ����� ó���� �Ϸ�Ǿ���.
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
				queue->right_link = process->right_link; // ���� ���μ����� ��带 �������ش�.
				process->right_link->left_link = queue->right_link; // ���� ���μ����� ���� ��ũ�� ��带 ����Ű�� �Ѵ�. 
				process->left_link = NULL; // ���� ��ũ�� ���������ν� ���� ���μ����� ť���� ���� ��Ų��.
				process->right_link = NULL; // ������ ��ũ�� ���������ν� ���� ���μ����� ť���� ���� ��Ų��.

				process = queue->right_link; // process ������ ���� ���μ����� ����Ű�� �Ѵ�.
				tmp = process->remaining_time; // ���� ���μ����� ���� �ð��� tmp������ ����
			}
		}
		if (counting == 0) // 20��ŭ ó���Ⱑ ó���� �Ϸ����� �� ���ѷ����� ���� ���´�.
			/*��� ���μ����� ������ �� ������(�������� ���ð� 60���� computing_time�� 20���� ū ���μ����� ������
			�켱 20��ŭ ó���� ���� �Լ��� ���������� �ٽ� �Լ��� ȣ���ؼ� 20��ŭ ó���� ���ش�.*/
			break;
	}
}



