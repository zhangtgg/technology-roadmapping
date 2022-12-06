// ����ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <limits.h>
#include <memory.h>
#include <algorithm>
#include <vector>
#include <queue>

// �����ռ䣺��ν namespace ��ָ��ʶ���ĸ��ֿɼ���Χ��
// C���� ��׼������е����б�ʶ������������һ����Ϊ std �� namespace �С�
using namespace std;

// ������ҵ�����̽ṹ��
typedef struct node
{
	int number;		 // ���
	int reach_time;	 // �ִ�ʱ��
	int need_time;	 // ִ��ʱ��
	int privilege;	 // ����Ȩ
	float excellent; // ��Ӧ��
	int start_time;	 // ��ʼʱ��
	int wait_time;	 // �ȴ�ʱ��
	int tr_time;	 // ��תʱ��
	double wtr_time; // ��Ȩ��תʱ��
	int run_time;	 // �ۼ���ִ��ʱ��
	bool visited;	 // �Ƿ񱻷��ʹ�
	int finish_time; // ���ʱ��
	bool reached;	 // �Ƿ�ִ�
} job;

//������������
const int MAX_NUM = 10;	   // �����ҵ����
const int MAX_LINE = 1024; //һ�е�size
int num = 0;			   // ʵ����ҵ���������
job jobs[MAX_NUM];		   // ��ҵ����̽ṹ������
int ready[MAX_NUM];		   // ��������
int order[MAX_NUM];		   // ����ʹ���ĸ���ֵ��Ϊ�������

// ��������
void initial_jobs();					 // ��ʼ��������ҵ��Ϣ
void init_Jobdata();					 // ��ʼ������
void initial_jobs_again();				 // ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��
void sleep(int n);						 // ͣ�ټ��룬ģ�����
void print_1(int a, int b, int c);		 // ִ�в����ӡ����-1
void print_2(int a, int b, float c);	 // ִ�в����ӡ����-2
void print_3(float a, float b, float c); // ִ�в����ӡ����-3
void FCFS();							 // �����ȷ��� FCFS --- ��־ӱ
void SJF();								 // ����ҵ���� SJF������ռʽ��--- ��ˬ
void HRRF();							 // ����Ӧ������ HRRF --- ë����
void HPF();								 // ʱ��Ƭ��ת RR --- �ܼ���
void RR();								 // �����ȼ����� HPF������ռʽ��--- �����

// ������
int main()
{
	initial_jobs(); // ��ʼ��������ҵ��Ϣ
	init_Jobdata(); // ��ʼ�������ļ�
	int choice = 1;
	while (choice != 0)
	{
		printf("----------------------------------------------------------------\n");
		printf("-----------------------��ҵ�����̵����㷨-----------------------\n\n");
		printf("            1�������ȷ���FCFS --- ��־ӱ\n");
		printf("            2������ҵ����SJF������ռʽ��--- ��ˬ\n");
		printf("            3������Ӧ������HRRF --- ë����\n");
		printf("            4��ʱ��Ƭ��תRR --- �ܼ���\n");
		printf("            5�������ȼ�����HPF������ռʽ��--- �����\n");
		printf("            0���˳�\n\n");
		printf("----------------------------------------------------------------\n");
		printf("----------------------------------------------------------------\n");
		printf("������һ����ţ�");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			FCFS();
			break;
		case 2:
			SJF();
			break;
		case 3:
			HRRF();
			break;
		case 4:
			RR();
			break;
		case 5:
			HPF();
			break;
		case 0:
			printf("�˳�����\n");
			break;
		}
	}
}

// ��ʼ��������ҵ��Ϣ
void initial_jobs()
{
	for (int i = 0; i < MAX_NUM; i++)
	{
		jobs[i].excellent = 0;
		jobs[i].need_time = 0;
		jobs[i].number = 0;
		jobs[i].privilege = 0;
		jobs[i].reach_time = 0;
		jobs[i].run_time = 0;
		jobs[i].start_time = 0;
		jobs[i].tr_time = 0;
		jobs[i].visited = false;
		jobs[i].wait_time = 0;
		jobs[i].wtr_time = 0;
		jobs[i].finish_time = 0;
		jobs[i].reached = false;
	}
}

// ��ʼ������
void init_Jobdata()
{
	// �ѻ����������ʣ�������������ں˻����������ͷ��ļ�ָ����йصĻ�������
	int mydata[7][4] = {{1, 800, 50, 0},
						{2, 815, 30, 1},
						{3, 830, 25, 2},
						{4, 835, 20, 2},
						{5, 845, 15, 2},
						{6, 700, 10, 1},
						{7, 820, 5, 0}};
	num++;
	while (num != 8)
	{
		jobs[num].number = mydata[num - 1][0];
		jobs[num].reach_time = mydata[num - 1][1];
		jobs[num].need_time = mydata[num - 1][2];
		jobs[num].privilege = mydata[num - 1][3];
		num++;
	}
	num--;
	printf("init data success!\n");
	printf("��ҵ������Ϊ: %d\n", num);
	// �����ʾ��ҵ�š���ҵ�ִ�ʱ�䡢��ҵ��ִ��ʱ�䡢��ҵ������Ȩ��
	printf("���\t�ִ�ʱ��\tִ��ʱ��\t����Ȩ\n");
	for (int j = 1; j <= num; j++)
	{
		printf("%d\t%d\t\t%d\t\t%d\n", jobs[j].number, jobs[j].reach_time, jobs[j].need_time, jobs[j].privilege);
	}
}

// ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��������ҵ�Ƿ񱻷��ʹ�����Ϊ false��
void initial_jobs_again()
{
	for (int i = 1; i <= num; i++)
	{
		jobs[i].finish_time = 0;
		jobs[i].excellent = 0;
		jobs[i].tr_time = 0;
		jobs[i].wtr_time = 0;
		jobs[i].wait_time = 0;
		jobs[i].visited = false;
	}
}

// ͣ�ټ��루ģ�⣩
void sleep(int n)
{
	clock_t goal;
	goal = (clock_t)n * CLOCKS_PER_SEC + clock();
	while (goal > clock())
		;
}

// ִ�в����ӡ����-1
void print_1(int a, int b, int c)
{
	printf("��%d����ҵ", a);
	printf("����ʱ�䣺%d,����ʱ�䣺%d\n", b, c);
	printf("����ҵ��������...........\n");
	printf("�������\n");
}

// ִ�в����ӡ����-2
void print_2(int a, int b, float c)
{
	printf("�ȴ�ʱ�䣺%d ��תʱ�䣺%d ��Ȩ��תʱ�䣺%0.2f\n", a, b, c);
}

// ִ�в����ӡ����-3
void print_3(float a, float b, float c)
{
	printf("************************������ҵ�������************************\n");
	printf("ƽ���ȴ�ʱ�䣺%.2f ƽ����תʱ�䣺%.2f ƽ����Ȩ��תʱ�䣺%.2f\n\n", a, b, c);
}

// �����ȷ��� FCFS --- ��־ӱ
void FCFS()
{
	int i, j, temp;
	double sum1 = 0; // �ܵĵȴ�ʱ��
	double sum2 = 0; // �ܵ���תʱ��
	double sum3 = 0; // �ܵĴ�Ȩ��תʱ��

	// num����ҵ�������
	for (i = 1; i <= num; i++)
	{
		order[i] = jobs[i].reach_time; // ����ҵ�ĵִ�ʱ��洢�� order ������
		ready[i] = i;				   // ��������
	}

	// ð������
	// ���յ���ʱ���С����
	// �� order[num] �����õ��ľ���˳�򸳸� ready[num]
	for (i = 1; i <= num; i++)
	{
		for (j = 1; j <= num - i; j++)
		{
			// ǰһ����ҵ�ĵִ�ʱ�� ���� ��һ����ҵ�ִ�ʱ��
			if (order[j] > order[j + 1])
			{
				// ��ʱ��
				temp = order[j];
				order[j] = order[j + 1];
				order[j + 1] = temp;
				// �Ŵ���
				temp = ready[j];
				ready[j] = ready[j + 1];
				ready[j + 1] = temp;
			}
		}
	}

	// ���еĵ���
	for (i = 1; i <= num; i++)
	{
		print_1(ready[i], jobs[ready[i]].reach_time, jobs[ready[i]].need_time);
		sleep(1); // ģ����ȣ�˯��һ�룩��

		// �����һ����ҵ�����ʱ�䣬Ϊ�ȴ�ʱ�丳ֵ��
		if (i == 1)
		{
			// ��һ����ҵ�����ʱ�� = �ִ�ʱ�� + ִ��ʱ��
			jobs[ready[i]].finish_time = jobs[ready[i]].reach_time + jobs[ready[i]].need_time;
			// ��һ����ҵ�ĵȴ�ʱ��Ĭ��Ϊ 0
			jobs[ready[i]].wait_time = 0;
		}
		else
		{
			// �����һ����ҵ�����ʱ�� > ��һ����ҵ�ĵ���ʱ�䣬����һ����ҵ�Ŀ�ʼʱ�����һ�������ʱ�俪ʼ��
			if (jobs[ready[i - 1]].finish_time > jobs[ready[i]].reach_time)
			{
				// ����ҵ�Ľ���ʱ�� = ��һ����ҵ�Ľ���ʱ�� + ����ҵ��ִ��ʱ�䡣
				jobs[ready[i]].finish_time = jobs[ready[i - 1]].finish_time + jobs[ready[i]].need_time;
				// ����ҵ�ĵȴ�ʱ�� = ��һ����ҵ�Ľ���ʱ�� - ����ҵ�ĵִ�ʱ�䡣
				jobs[ready[i]].wait_time = jobs[ready[i - 1]].finish_time - jobs[ready[i]].reach_time;
			}
			else
			{
				// ����ҵ�����ʱ�� = ����ҵ��ִ��ʱ�� + ����ҵ�ĵִ�ʱ�䡣
				jobs[ready[i]].finish_time = jobs[ready[i]].need_time + jobs[ready[i]].reach_time;
				// ����ҵ�ִ�ʱû����ҵ��ִ�У����Եȴ�ʱ��Ϊ 0��
				jobs[ready[i]].wait_time = 0;
			}
		}

		// ��ҵ��תʱ�� = ����ʱ�� - �ִ�ʱ�䡣
		jobs[ready[i]].tr_time = jobs[ready[i]].finish_time - jobs[ready[i]].reach_time;
		// ��ҵ��Ȩ��תʱ�� = ��תʱ�� / ִ��ʱ�䡣
		jobs[ready[i]].wtr_time = double(jobs[ready[i]].tr_time) / jobs[ready[i]].need_time;

		print_2(jobs[ready[i]].wait_time, jobs[ready[i]].tr_time, jobs[ready[i]].wtr_time);

		// �ۼ�
		sum1 += jobs[ready[i]].wait_time;
		sum2 += jobs[ready[i]].tr_time;
		sum3 += jobs[ready[i]].wtr_time;
	}

	// ��ƽ��ֵ�����
	print_3(sum1 / num, sum2 / num, sum3 / num);

	initial_jobs_again(); // ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��������ҵ�Ƿ񱻷��ʹ�����Ϊ false��
}

// int index = findNextSJF(jobs, num, finish);	����ҵ�����жϣ�ѡ����һ��Ҫִ�е���ҵ��
int findNextSJF(job j[MAX_NUM], int length, int time)
{
	// p ���Ѿ�������ӵ���������ʱ��Ľ��̵��±�
	// q ��û�е���Ľ�����ӵ�����絽��ʱ��Ľ��̵��±�
	int i, p, q;
	int minNeedTime, minReachTime, minTime;

	p = q = -1;
	minTime = minNeedTime = minReachTime = INT_MAX;

	for (i = 1; i <= length; i++)
	{
		// ����ҵû�б����ʹ���
		if (!j[i].visited)
		{
			// ��һ���
			// ����ҵ�ĵִ�ʱ�� <= ǰ������������ҵ�ִ�ʱ�䣬���Ҹ���ҵ��ִ��ʱ�� <= ��ǰ�������ҵִ��ʱ�䡣
			if (j[i].reach_time <= time && j[i].need_time <= minNeedTime)
			{
				p = i;
				minNeedTime = j[i].need_time;
			}
			// �ڶ������
			// ����ҵ�ĵִ�ʱ�� > ǰ������������ҵ�ִ�ʱ�䣬���Ҹ���ҵ�ִ�ʱ�� <= ��ǰ�������ҵ�ִ�ʱ�䡣
			else if (j[i].reach_time > time && j[i].reach_time <= minReachTime)
			{
				// ����ҵ��ִ��ʱ�� С�� minTime��
				if (j[i].need_time < minTime)
				{
					q = i;
					minReachTime = j[i].reach_time;
					minTime = j[i].need_time;
				}
			}
		}
	}

	// p Ϊ -1 ʱ,������ lastTime ʱ�̻�û���̵����ʱѡ����һ�����絽��Ľ��� q
	if (p != -1)
	{
		return p;
	}

	return q;
}

// ����ҵ���� SJF������ռʽ��--- ��ˬ
void SJF()
{
	int i, j;
	double sum1 = 0;	  // �ܵĵȴ�ʱ��
	double sum2 = 0;	  // �ܵ���תʱ��
	double sum3 = 0;	  // �ܵĴ�Ȩ��תʱ��
	int finish = INT_MAX; // ��ǰ���ʱ��

	for (i = 1; i <= num; i++)
	{
		// ���ظ����������е���Сֵ�� ��������ҵ�ִ�ʱ�丳ֵ�� finish��
		finish = min(finish, jobs[i].reach_time);
	}

	printf("����ҵ�����㷨: \n");
	for (i = 1; i <= num; i++)
	{
		// ����ҵ�����жϣ�ѡ����һ��Ҫִ�е���ҵ��
		int index = findNextSJF(jobs, num, finish);
		print_1(index, jobs[index].reach_time, jobs[index].need_time);
		sleep(1); // ģ����ȣ�˯��һ�룩��

		// ����ҵ�ִ�ʱ�� <= ǰ������������ҵ�ִ�ʱ�䡣
		if (jobs[index].reach_time <= finish)
		{
			// ����ҵ�ĵȴ�ʱ�� = ǰ������������ҵ�ִ�ʱ�� - ����ҵ�ִ�ʱ�䡣
			jobs[index].wait_time = finish - jobs[index].reach_time;
			// ����ҵ�Ŀ�ʼʱ�� = ǰ������������ҵʱ�䡣
			jobs[index].start_time = finish;
		}
		else
		{
			// ����ҵ�Ŀ�ʼʱ�� = ����ҵ�ĵִ�ʱ�䡣
			jobs[index].start_time = jobs[index].reach_time;
			// �������ҵһ����ִ�У�û�еȴ���
			jobs[index].wait_time = 0;
		}

		jobs[index].finish_time = jobs[index].start_time + jobs[index].need_time;	// ��ҵ���ʱ�� = ��ҵ��ʼʱ�� + ��ҵִ��ʱ�䡣
		jobs[index].tr_time = jobs[index].finish_time - jobs[index].reach_time;		// ��תʱ�� = ����ʱ�� - �ִ�ʱ�䡣
		jobs[index].wtr_time = (double)jobs[index].tr_time / jobs[index].need_time; // ��Ȩ��תʱ�� = ��תʱ�� / ִ��ʱ�䡣
		jobs[index].visited = true;													// ������ҵ�ķ��ʱ�־��Ϊ true��

		// �ۼ�
		sum1 += jobs[index].wait_time;	  // �ܵĵȴ�ʱ��
		sum2 += jobs[index].tr_time;	  // �ܵ���תʱ��
		sum3 += jobs[index].wtr_time;	  // �ܵĴ�Ȩ��תʱ��
		finish = jobs[index].finish_time; // ������ҵ�Ľ���ʱ�丳ֵ�� finish��

		print_2(jobs[index].wait_time, jobs[index].tr_time, jobs[index].wtr_time);
	}
	// ��ƽ��ֵ�����
	print_3(sum1 / num, sum2 / num, sum3 / num);

	initial_jobs_again(); // ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��������ҵ�Ƿ񱻷��ʹ�����Ϊ false��
}

// int index = findNextHRRF(pre);
int findNextHRRF(int pre)
{
	int current = 1, i, j;
	for (i = 1; i <= num; i++)
	{
		// ����ҵû�б����ʹ���
		if (!jobs[i].visited)
		{
			// ����ҵ�ĵȴ�ʱ�� = ��һ����ҵ�����ʱ�� - ����ҵ����ʱ�䡣
			jobs[i].wait_time = jobs[pre].finish_time - jobs[i].reach_time;
			// ��Ӧ�� = �� �ȴ�ʱ�� + ִ��ʱ�� �� / ִ��ʱ�䡣
			jobs[i].excellent = (float)(jobs[i].wait_time + jobs[i].need_time) / jobs[i].need_time;
		}
	}

	for (i = 1; i <= num; i++)
	{
		if (!jobs[i].visited)
		{
			// ����ҵû�б����ʹ���
			current = i;
			// �ҵ���һ����ûִ�е���ҵ��
			break;
		}
	}

	// �ͺ������ҵ�Ƚϡ�
	for (j = i; j <= num; j++)
	{
		// ����ҵû�б����ʹ���
		if (!jobs[j].visited)
		{
			// ����ҵ�ĵִ�ʱ�� С�� pre ��ҵ�����ʱ�䣬 ��ǰ���
			if (jobs[current].reach_time < jobs[pre].finish_time)
			{
				// ����ҵ����Ӧ�� ���� ��һ��û��ִ�е���ҵ��
				if (jobs[j].excellent > jobs[current].excellent)
					// ������Ӧ�ȵ���ҵ�Ÿ�ֵ�� current��
					current = j;
			}
			// �������������һ���������֮�󵽴
			else
			{
				// ����ҵ�ĵִ�ʱ�� С�� ��һ����û��ִ�е���ҵ��
				if (jobs[j].reach_time < jobs[current].reach_time)
				{
					// �ҳ��Ƚ��絽���һ����
					current = j;
				}
				// ���ͬʱ������Ҹ���ҵ����Ӧ�� ���� ��һ����û��ִ�е���ҵ����Ӧ�ȡ�
				else if (jobs[j].reach_time == jobs[current].reach_time && jobs[j].excellent > jobs[current].excellent)
				{
					// ������Ӧ�ȵ���ҵ�Ÿ�ֵ�� current��
					current = j;
				}
			}
		}
	}
	return current; //������Ӧ����ߵ���ҵ��
}

// ����Ӧ������ HRRF --- ë����
void HRRF()
{
	int i, j;
	double sum1 = 0; // �ܵĵȴ�ʱ�䡣
	double sum2 = 0; // �ܵ���תʱ�䡣
	double sum3 = 0; // �ܵĴ�Ȩ��תʱ�䡣

	printf("����Ӧ�����ȵ����㷨: \n");
	int pre = 1;
	jobs[1].finish_time = 0; // ����һ����ҵ�����ʱ�丳ֵΪ 0��

	for (i = 1; i <= num; i++)
	{
		// ���ú��������������ҵ����Ӧ����ߵ���ҵ���ء�
		int index = findNextHRRF(pre);

		print_1(index, jobs[index].reach_time, jobs[index].need_time);
		sleep(1); // ģ����ȣ�˯��һ�룩��

		if (i == 1)
		{
			jobs[index].start_time = jobs[index].reach_time;						  // ��һ����ҵ�Ŀ�ʼʱ�� = ����ҵ�ĵִ�ʱ�䡣
			jobs[index].finish_time = jobs[index].start_time + jobs[index].need_time; // ��һ����ҵ�����ʱ�� = ����ҵ�Ŀ�ʼʱ�� + ����ҵ��ִ��ʱ�䡣
			jobs[index].tr_time = jobs[index].need_time;							  // ��һ����ҵ����תʱ�� = ����ҵ��ִ��ʱ�䡣
			jobs[index].wtr_time = 1.00;											  // ��һ����ҵ�Ĵ�Ȩ��תʱ�� = 1 ��
			jobs[index].wait_time = 0;												  // ��һ����ҵ�ĵȴ�ʱ�� = 0 ��
		}
		else
		{
			// ����ҵ�ĵִ�ʱ�� ���� pre �����ʱ�䡣
			if (jobs[index].reach_time > jobs[pre].finish_time)
			{
				jobs[index].wait_time = 0;
				jobs[index].start_time = jobs[index].reach_time; // ����ҵ�Ŀ�ʼʱ�� = ����ҵ�ĵִ�ʱ�䡣
			}
			else
			{
				jobs[index].start_time = jobs[pre].finish_time;							// ����ҵ�Ŀ�ʼʱ�� = pre �����ʱ�䡣
				jobs[index].wait_time = jobs[pre].finish_time - jobs[index].reach_time; // ����ҵ�ĵȴ�ʱ�� = pre �����ʱ�� - ����ҵ�ĵִ�ʱ�䡣
			}
			jobs[index].finish_time = jobs[index].start_time + jobs[index].need_time;	// ����ҵ�����ʱ�� = ����ҵ�Ŀ�ʼʱ�� + ����ҵ��ִ��ʱ�䡣
			jobs[index].tr_time = jobs[index].finish_time - jobs[index].reach_time;		// ��תʱ�� = ���ʱ�� - �ִ�ʱ�䡣
			jobs[index].wtr_time = (double)jobs[index].tr_time / jobs[index].need_time; // ��Ȩ��תʱ�� = ��תʱ�� / ִ��ʱ�䡣
		}

		jobs[index].visited = true;	   // ������ҵ�ķ��ʱ�־��Ϊ true��
		pre = index;				   // ��ִ�������ҵ���±긳ֵ�� pre�����ں���һ����ҵ���жϡ�
		sum1 += jobs[index].wait_time; // �ܵĵȴ�ʱ�䡣
		sum2 += jobs[index].tr_time;   // �ܵ���תʱ�� ��
		sum3 += jobs[index].wtr_time;  // �ܵĴ�Ȩ��תʱ�� ��

		print_2(jobs[index].wait_time, jobs[index].tr_time, jobs[index].wtr_time);
	}
	print_3(sum1 / num, sum2 / num, sum3 / num);

	initial_jobs_again(); // ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��������ҵ�Ƿ񱻷��ʹ�����Ϊ false��
}

// ���������ȷ���ʹ��ʱ��Ƭ��ת
vector<job> jobList; // jobList ������
int time_unit = 10;	 // ʱ��Ƭ��������Ϊ 10��

void initial_job_List()
{
	// ��������ʼ��
	for (int i = 1; i <= num; i++)
	{
		jobList.push_back(jobs[i]);
	}
}

bool cmp(job a, job b)
{
	return a.reach_time < b.reach_time; // ����С����ҵ�ִ�ʱ�䡣
}

/*
queue����һ�� FIFO�������ȳ��������ݽṹ�� 
q.empty():�ж϶��� q �Ƿ�Ϊ�գ������� q ��ʱ������ true; ����Ϊ false��ֵΪ0(false)/1(true)����
q.push():�Ὣһ��Ԫ�� a ������� q �С�
q.pop()����Ӷ��� q ���Ƴ���һ��Ԫ�ء�(����д�� pop(q)) 
*/

// ʱ��Ƭ��ת RR --- �ܼ���
void RR()
{
	double sum1 = 0; // �ܵĵȴ�ʱ��
	double sum2 = 0; // �ܵ���תʱ��
	double sum3 = 0; // �ܵĴ�Ȩ��תʱ��

	initial_job_List();						   // ���ú������������г�ʼ��
	queue<job> Ready;						   // ��������
	sort(jobList.begin(), jobList.end(), cmp); // �������Ԫ�ؽ�������,ǰ��������Ϊ��ʼ�ĵ�ַ�ͽ����ĵ�ַ
	int begin = (*jobList.begin()).reach_time; // �� jobList �����ĵ�һ����ҵ�ĵִ�ʱ�丳ֵ�� begin
	Ready.push(*jobList.begin());			   // �� jobList �����е���ҵ���� Ready ������
	jobList.erase(jobList.begin());			   // �� jobList �����еĵ�һ����ҵ���

	// jobList ������ Ready ���ж���Ϊ�ա�
	while (!jobList.empty() || !Ready.empty())
	{
		// ������ҵ�������������С�
		while (!jobList.empty() && begin >= (*jobList.begin()).reach_time)
		{
			Ready.push(*jobList.begin());	// �� jobList �����е���ҵ���� Ready �����С�
			jobList.erase(jobList.begin()); // �� jobList �����еĵ�һ����ҵ�����
		}
		// Ready �������ʱ�� + ʱ��Ƭʱ�� < ����ҵ��ִ��ʱ�䣬ʱ��Ƭ����û������,�����β��
		if (Ready.front().finish_time + time_unit < Ready.front().need_time)
		{
			printf("%d ����ҵִ���� %d\n", Ready.front().number, time_unit);
			Ready.front().finish_time += time_unit; // ����ҵ�����ʱ�� = ����ҵ�����ʱ�� + ʱ��Ƭʱ�䡣
			begin += time_unit;						// begin = begin + ʱ��Ƭʱ�䡣
			// ������ҵ�������������С�
			while (!jobList.empty() && begin >= (*jobList.begin()).reach_time)
			{
				Ready.push(*jobList.begin());	// �� jobList �����е���ҵ����Ready�����С�
				jobList.erase(jobList.begin()); // �� jobList �����еĵ�һ����ҵ�����
			}

			Ready.push(Ready.front()); // �� Ready �����еĵ�һ����ҵ���� Ready �����У��������뵽���е�β����
			Ready.pop();			   // �� Ready �������Ƴ���һ����ҵ��
			sleep(1);				   // ģ����ȣ�˯��һ�룩��
		}
		else
		{ //��ҵ������
			if (Ready.front().need_time - Ready.front().finish_time < time_unit)
			{																	  // ��ҵִ��ʱ�� - Ready ���е�һ����ҵ�����ʱ�� < ʱ��Ƭʱ�䡣
				time_unit -= Ready.front().need_time - Ready.front().finish_time; // ����ҵ��ִ�е�ʱ��Ƭ���� = ִ��ʱ�� - Ready ���е�һ����ҵ�����ʱ�䡣
			}
			else
			{
				time_unit = 10;
			}
			printf("%d ����ҵִ���� %d\n", Ready.front().number, time_unit);
			begin += time_unit;																						  // begin = begin + ʱ��Ƭʱ�䡣
			Ready.front().finish_time = begin;																		  // Ready ���е�һ����ҵ���ʱ�� = begin��
			Ready.front().wait_time = Ready.front().finish_time - Ready.front().reach_time - Ready.front().need_time; // Ready ���е�һ����ҵ�ĵȴ�ʱ�� = ���ʱ�� - �ִ�ʱ�� - ִ��ʱ�䡣
			Ready.front().tr_time = Ready.front().finish_time - Ready.front().reach_time;							  // Ready ���е�һ����ҵ��תʱ�� = ���ʱ�� - �ִ�ʱ�䡣
			Ready.front().wtr_time = (double)Ready.front().tr_time / Ready.front().need_time;						  // Ready ���е�һ����ҵ��Ȩ��תʱ�� = ��תʱ�� / ִ��ʱ�䡣
			sum1 += Ready.front().wait_time;																		  // �ܵĵȴ�ʱ�䡣
			sum2 += Ready.front().tr_time;																			  // �ܵ���תʱ�䡣
			sum3 += Ready.front().wtr_time;																			  // �ܵĴ�Ȩ��תʱ�䡣
			printf("ִ�������ҵ�� %d ����ҵ���ȴ�ʱ���� %d ,��תʱ���� %d ,��Ȩ��תʱ���� %.2f\n", Ready.front().number, Ready.front().wait_time, Ready.front().tr_time, Ready.front().wtr_time);
			Ready.pop(); // �� Ready �������Ƴ���һ����ҵ��
			// ��� Ready ����Ϊ�ղ��� jobList ������Ϊ�ա�
			if (Ready.empty() && !jobList.empty())
			{
				// �������Ԫ�ؽ�������ǰ��������Ϊ��ʼ�ĵ�ַ�ͽ����ĵ�ַ
				sort(jobList.begin(), jobList.end(), cmp);
				// �ҳ���ǰ�������ҵ��
				printf("�ҵ���ǰ�������ҵ��: %d\n", (*jobList.begin()).number);
				// ���������ҵ�ִ�ʱ�丳ֵ�� begin
				begin = (*jobList.begin()).reach_time;
			}
			if (time_unit < 10)
			{
				// ��ҵ��ִ��ʱ��Ƭ С�� 10shi ,��ʱ��Ƭ��ֵΪ 10��
				time_unit = 10;
			}
			sleep(1); // ģ����ȣ�˯��һ�룩��
		}
	}
	print_3(sum1 / num, sum2 / num, sum3 / num);
	initial_jobs_again(); // ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��������ҵ�Ƿ񱻷��ʹ�����Ϊ false��
}

// int index = findNextHPF(jobs, num, finish);
int findNextHPF(job j[MAX_NUM], int length, int time)
{
	// ����ֵԽ�� ��ʾ����ȨԽ��
	// p ���Ѿ�������ӵ���������Ȩ�Ľ��̵��±�
	// q ��û�е���Ľ�����ӵ�����絽��ʱ��Ľ��̵��±�
	int i, p, q;
	int minReachTime, minPrivilege1;
	p = q = -1;
	minPrivilege1 = minReachTime = INT_MAX;
	for (i = 1; i <= length; i++)
	{
		if (!j[i].visited)
		{
			// ����ҵû�б����ʹ���
			// ��һ���
			// ����ҵ�ĵִ�ʱ�� С�ڵ��� ǰ������������ҵ�ִ�ʱ�䣬���Ҹ���ҵ������Ȩ С�ڵ��� minPrivilege1��
			if (j[i].reach_time <= time && j[i].privilege <= minPrivilege1)
			{
				if (j[i].privilege == j[p].privilege)
				{
					// �������Ȩһ�� ������ִ�ʱ��
					if (j[i].reach_time < j[p].reach_time)
					{
						// ����ҵ�ĵִ�ʱ�� С�� Ŀǰ��ҵ�ĵִ�ʱ�䡣
						p = i;
					}
				}
				else
				{
					// i ��ҵ������Ȩ ���� p ��ҵ������Ȩ��
					p = i;
					minPrivilege1 = j[i].privilege; // �� i ��ҵ������Ȩ��ֵ�� minPrivilege1��
				}
			}
			// �ڶ������
			// ����ҵ�ĵִ�ʱ�� ���� ǰ������������ҵ�ִ�ʱ�䣬���Ҹ���ҵ�ĵִ�ʱ�� С�ڵ��� minReachTime��
			else if (j[i].reach_time > time && j[i].reach_time <= minReachTime)
			{
				q = i;
				minReachTime = j[i].reach_time; // �� i ��ҵ�ĵִ�ʱ�丳ֵ�� minReachTime��
			}
		}
	}
	// pΪ -1 ʱ,������ time ʱ�̻�û���̵����ʱѡ����һ�����絽��Ľ��� q��
	if (p != -1)
	{
		return p;
	}

	return q;
}

// �����ȼ����� HPF������ռʽ��--- �����
void HPF()
{
	int i, j;
	double sum1 = 0;	  // �ܵĵȴ�ʱ�䡣
	double sum2 = 0;	  // �ܵ���תʱ�䡣
	double sum3 = 0;	  // �ܵĴ�Ȩ��תʱ�䡣
	int finish = INT_MAX; // ��ǰ���ʱ�䡣
	for (i = 1; i <= num; i++)
	{
		//���ظ����������е���Сֵ�� ��������ҵ�ִ�ʱ�丳ֵ�� finish��
		finish = min(finish, jobs[i].reach_time);
	}
	printf("����Ȩ�������ȷ���(����ռʽ): \n");
	for (i = 1; i <= num; i++)
	{
		int index = findNextHPF(jobs, num, finish);
		print_1(index, jobs[index].reach_time, jobs[index].need_time);
		sleep(1); //ģ����ȣ�˯��һ�룩��

		// ����ҵ�ִ�ʱ�� С�ڵ��� ǰ������������ҵ�ִ�ʱ�䡣
		if (jobs[index].reach_time <= finish)
		{
			// ����ҵ�ĵȴ�ʱ�� = ǰ������������ҵ�ִ�ʱ�� - ����ҵ�ִ�ʱ�䡣
			jobs[index].wait_time = finish - jobs[index].reach_time;
			// ����ҵ�Ŀ�ʼʱ�� = ǰ������������ҵʱ�䡣
			jobs[index].start_time = finish;
		}
		else
		{
			// ����ҵ�Ŀ�ʼʱ�� = ����ҵ�ĵִ�ʱ�䡣
			jobs[index].start_time = jobs[index].reach_time;
			// �������ҵһ����ִ�У�û�еȴ���
			jobs[index].wait_time = 0;
		}
		jobs[index].finish_time = jobs[index].start_time + jobs[index].need_time;	// ��ҵ���ʱ�� = ��ҵ��ʼʱ�� + ��ҵִ��ʱ�䡣
		jobs[index].tr_time = jobs[index].finish_time - jobs[index].reach_time;		// ��תʱ�� = ����ʱ�� - �ִ�ʱ�䡣
		jobs[index].wtr_time = (double)jobs[index].tr_time / jobs[index].need_time; // ��Ȩ��תʱ�� = ��תʱ�� / ִ��ʱ�䡣
		jobs[index].visited = true;													// ������ҵ�ķ��ʱ�־��Ϊ true��
		sum1 += jobs[index].wait_time;												// �ܵĵȴ�ʱ�䡣
		sum2 += jobs[index].tr_time;												// �ܵ���תʱ�䡣
		sum3 += jobs[index].wtr_time;												// �ܵĴ�Ȩ��תʱ�䡣
		finish = jobs[index].finish_time;											// ������ҵ�Ľ���ʱ�丳ֵ�� finish��
		print_2(jobs[index].wait_time, jobs[index].tr_time, jobs[index].wtr_time);
	}
	print_3(sum1 / num, sum2 / num, sum3 / num);
	initial_jobs_again(); // ����ҵ���ʱ�䡢��Ӧ�ȡ���תʱ�䡢��Ȩ��תʱ����г�ʼ��������ҵ�Ƿ񱻷��ʹ�����Ϊ false��
}
