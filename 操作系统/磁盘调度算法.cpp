#include <stdio.h>

// num���ŵ�����sum��Ѱ�����ȣ�kai����ʼ�ŵ���max����ŵ��� 
int num, sum, kai, max;
int m = 0;
int n = 0;
int s[100];		// ����������� 
int s1[100];
int c1[50];		// С����ʼ�ŵ����� 
int c2[50];		// ���ڵ�����ʼ�ŵ����� 

void MENU(); 	// �˵� 
void creat();	// ����
void FCFS();	// �����ȷ��� FCFS���ܼ��� 
void SSTF();	// ���Ѱ�� SSTF ����ˬ����־ӱ�� 
void SCAN();	// ɨ���㷨 SCAN��ë���ࡢ����ܣ� 
void CSCAN();	// ѭ��ɨ���㷨 CSCAN���ܼ��� 


int main()
{
    MENU();
    return 0;
}

// �˵� 
void MENU()
{
    printf("**************���̵����㷨*************\n");
    printf("***************************************\n");
    printf("1.�����ŵ�\n2.�����ȷ��� FCFS\n3.���Ѱ�� SSTF\n4.ɨ���㷨 SCAN\n5.ѭ��ɨ���㷨 CSCAN\n6.�˳� EXIT\n");
    printf("***************************************\n");
    printf("�������ţ�");
     
    int menuchoice;
    scanf("%d", &menuchoice);
    
    if (menuchoice != 1 && menuchoice != 6)
    {
        printf("���ȴ����ŵ�\n");
    }
    if (menuchoice == 6)
    {
        printf("ллʹ�ã�");
    }
    else
    {
        creat();
    P: 
        printf("***************************************\n");
        printf("1.�����ȷ��� FCFS\n2.���Ѱ�� SSTF\n3.ɨ���㷨 SCAN\n4.ѭ��ɨ���㷨 CSCAN\n5.�˳� EXIT\n");
        printf("***************************************\n");
        printf("�������ţ�");
        
        scanf("%d", &menuchoice);
        switch (menuchoice)
        {
        case 1:
            FCFS();
            goto P;
        case 2:
            SSTF();
            goto P;
        case 3:
            SCAN();
            goto P;
        case 4:
            CSCAN();
            goto P;
        case 5:
            printf("ллʹ�ã�");
            break;
        }
    }
}

// ���ݴ�����ʼ�� 
void creat()
{
    printf("---------------------------------------\n");
    printf("��������ĸ��ŵ���ʼ��");
    scanf("%d", &kai);
    printf("---------------------------------------\n");
    printf("��������ŵ��ţ�");
    scanf("%d", &max);
    printf("---------------------------------------\n");
    printf("������ŵ��ĸ�����");
    scanf("%d", &num);
    printf("---------------------------------------\n");

    for (int j = 0; j < num; j++)
    {
        printf("�������%d���ŵ���", j + 1);
        scanf("%d", &s[j]);
        if (s[j] > max)
        {
            printf("ERROR\n");
            break;
        }
        // �����ظ����� 
        for (int i = 0; i < j; i++)
            if (s[j] == s[i])
                j--;
    }

	//	��ʼ�ŵ���kai
    int su = kai;
    int t;
    for (int i = 0; i < num; i++)
    	// ���С����ʼ�ŵ� 
        if (su > s[i])
            c1[m++] = s[i];
        // ������ڵ�����ʼ�ŵ� 
        else  
            c2[n++] = s[i];
            
    // ��С����ʼ�ŵ��Ĳ��ֽ������� 
    for (int i = 0; i < m; i++)
        for (int j = i; j < m; j++)
            if (c1[i] < c1[j])
            {
                t = c1[i];
                c1[i] = c1[j];
                c1[j] = t;
            }
            
    // ���ڴ��ڵ�����ʼ�ŵ��Ĳ��ֽ������� 
    for (int i = 0; i < n; i++)
        for (int j = i; j < n; j++)
            if (c2[i] > c2[j])
            {
                t = c2[i];
                c2[i] = c2[j];
                c2[j] = t;
            }
}

// �����ȷ��� 
void FCFS()
{
    printf("�����ȷ��� FCFS\n");
    printf("�������ʵ���һ���ŵ���\t���ŵ����ƶ����롿\n");
    int su = kai;
    sum = 0;
    for (int i = 0; i < num; i++)
    {
        if (su < s[i])
            s1[i] = s[i] - su;
        else
            s1[i] = su - s[i];
        su = s[i];
        sum += s1[i];
    }

    for (int i = 0; i < num; i++)
    {
        printf("\t%d\t\t\t%d\n", s[i], s1[i]);
    }
    printf("Ѱ�����ȣ�%d\n", sum);
}

// ���Ѱ��ʱ������
void SSTF()
{
    printf("���Ѱ�� SSTF:\n");
    printf("�������ʵ���һ���ŵ���\t���ŵ����ƶ����롿\n");
    int su = kai;
    int s2[100];
    sum = 0;

    for (int i = 0; i < m; i++)
        s2[i] = c1[i];
    for (int i = 0; i < n; i++)
        s2[i + m] = c2[i];
        
    for (int i = 0; i < num; i++)
    {
        if (su < s2[i])
            s1[i] = s2[i] - su;
        else
            s1[i] = su - s2[i];
        su = s2[i];
        sum += s1[i];
    }
    for (int i = 0; i < num; i++)
    {
        printf("\t%d\t\t\t%d\t\t\n", s2[i], s1[i]);
    }
    printf("Ѱ������:%d\n", sum);
}

// ɨ���㷨/�����㷨
void SCAN()
{
    printf("ɨ���㷨/�����㷨 SCAN:\n");
    printf("�������ʵ���һ���ŵ���\t���ŵ����ƶ����롿\n");
    int su = kai;
    int s2[100];
    sum = 0;
    
    for (int i = 0; i < n; i++)
        s2[i] = c2[i];
    for (int i = 0; i < m; i++)
        s2[i + n] = c1[i];
        
    for (int i = 0; i < num; i++)
    {
        if (su < s2[i])
            s1[i] = s2[i] - su;
        else
            s1[i] = su - s2[i];
        su = s2[i];
        sum += s1[i];
    }
    for (int i = 0; i < num; i++)
    {
        printf("\t%d\t\t\t%d\t\t\n", s2[i], s1[i]);
    }
    printf("Ѱ�����ȣ�%d\n", sum);
}

// ѭ��ɨ���㷨 
void CSCAN()
{
    printf("ѭ��ɨ�� CSCAN:\n");
    printf("�������ʵ���һ���ŵ���\t���ŵ����ƶ����롿\n");
    int su = kai;
    int j = 0;
    int s2[100];
    sum = 0;
    for (int i = 0; i < n; i++)
        s2[i] = c2[i];
    for (int i = m - 1; i >= 0; j++, i--)
        s2[j + n] = c1[i];
    for (int i = 0; i < num; i++)
    {
        if (su < s2[i])
            s1[i] = s2[i] - su;
        else
            s1[i] = su - s2[i];
        su = s2[i];
        sum += s1[i];
    }
    // sum=sum/num;
    for (int i = 0; i < num; i++)
    {
        printf("\t%d\t\t\t%d\t\t\n", s2[i], s1[i]);
    }
    printf("Ѱ������:%d\n", sum);
}

