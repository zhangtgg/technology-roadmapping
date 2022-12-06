#include <stdio.h>

void initializeList(int list[], int number);                                           //��ʼ������
void showList(int list[], int number);                                                 //չʾ����״̬
void showMemoryList(int list[], int phyBlockNum);                                      //չʾ��ǰ�ڴ�״̬
void informationCount(int missingCount, int replaceCount, int pageNum);                //��Ϣ��ʾ
int getNextPosition(int currentPage, int currentPosition, int strList[], int pageNum); //�ҵ���ҳ���´�Ҫ���ʵ�λ��
void replacePageByOPT(int memoryList[], int phyNum, int strList[], int pageNum);       //����û��㷨���ܼ��� 
void replacePageByFIFO(int memoryList[], int phyNum, int strList[], int pageNum);      //�Ƚ��ȳ��û��㷨����ˬ����־ӱ�� 
void replacePageByLRU(int memoryList[], int phyNum, int strList[], int pageNum);       //������δʹ���û��㷨������ܡ�ë���ࣩ 

int main()
{
    //���������
    int phyBlockNum;
    printf("----------------------------------------------\n");
    printf("----------------ҳ���û��㷨------------------\n");
    printf("----------------------------------------------\n");
    printf("�����������������");
    scanf("%d", &phyBlockNum);

    //�����ڴ����
    int memoryList[phyBlockNum];
    
    //��ʼ���ڴ�״̬
    //�Ƚ� memoryList[] ȫ����ֵΪ -1 
    initializeList(memoryList, phyBlockNum);

    //ҳ������
    int pageNum;
    
    printf("������Ҫ���ʵ�ҳ��������");
    scanf("%d", &pageNum);

    //����ҳ������ô�
    int pageNumStrList[pageNum];
    
    // ������Ҫ���ʵ�ҳ��
    printf("������Ҫ���ʵ�ҳ��ţ�");
    for (int i = 0; i < pageNum; i++)
    {
        scanf("%d", &pageNumStrList[i]);
    }

    int chose;
    while (1)
    {
    	printf("----------------------------------------------\n");
        printf("��ѡ��������û��㷨:\n");
        printf("1.OPT ����û��㷨\n2.FIFO �Ƚ��ȳ��û��㷨\n3.LRU ������δʹ���û��㷨\n4.�˳�\n");
        printf("----------------------------------------------\n");
        printf("������һ����ţ�");
        scanf("%d", &chose);

        switch (chose)
        {
        case 1:
        	// չʾ����״̬
            showList(pageNumStrList, pageNum);
            //����û��㷨��memoryList�ڴ����,phyBlockNum���������,pageNumStrListҳ������ô�,pageNumҳ������
            replacePageByOPT(memoryList, phyBlockNum, pageNumStrList, pageNum);
            //���³�ʼ���ڴ�
            initializeList(memoryList, phyBlockNum);
            break;
        case 2:
        	// չʾ����״̬
            showList(pageNumStrList, pageNum);
            //�Ƚ��ȳ��û��㷨��memoryList�ڴ����,phyBlockNum���������,pageNumStrListҳ������ô�,pageNumҳ������
            replacePageByFIFO(memoryList, phyBlockNum, pageNumStrList, pageNum);
            //���³�ʼ���ڴ�
            initializeList(memoryList, phyBlockNum);
            break;
        case 3:
        	// չʾ����״̬
            showList(pageNumStrList, pageNum);
            //������δʹ���û��㷨��memoryList�ڴ����,phyBlockNum���������,pageNumStrListҳ������ô�,pageNumҳ������
            replacePageByLRU(memoryList, phyBlockNum, pageNumStrList, pageNum);
            //���³�ʼ���ڴ�
            initializeList(memoryList, phyBlockNum);
            break;
        default:
            return 0;
            break;
        }
    }

    return 0;
}

//��ʼ���ڴ����
void initializeList(int list[], int number)
{
    for (int i = 0; i < number; i++)
    {
    	// ȫ����ֵΪ -1 
        list[i] = -1;
    }
}

//չʾ����
void showList(int list[], int number)
{
    for (int i = 0; i < number; i++)
    {
        printf("%2d", list[i]);
    }
    printf("\n");
}

//չʾ��ǰ�ڴ�״̬
void showMemoryList(int list[], int phyBlockNum)
{
    for (int i = 0; i < phyBlockNum; i++)
    {
        if (list[i] == -1)
        {
            break;
        }
        printf("  ��%d�� ", list[i]);
    }
    printf("\n");
}

//��Ϣ��ʾ
void informationCount(int missingCount, int replaceCount, int pageNum)
{
    printf("ȱҳ����:%d\tȱҳ��:%d/%d\n", missingCount, missingCount, pageNum);
    double result = (double)(pageNum - missingCount) / (double)pageNum;
    printf("�û�����:%d\t������:%.2f\n", replaceCount, result);
}

//�ҵ���ҳ���´�Ҫ���ʵ�λ��
int getNextPosition(int currentPage, int currentPosition, int strList[], int pageNum)
{

    for (int i = currentPosition + 1; i < pageNum; i++)
    {
        if (strList[i] == currentPage)
        {
            return i;
        }
    }

    return 100;
}

//����û��㷨
void replacePageByOPT(int memoryList[], int phyNum, int strList[], int pageNum)
{
    //�û�����
    int replaceCount = 0;
    //ȱҳ����
    int missingCount = 0;
    //��¼���ڴ����������һ�η���λ��
    int nextPosition[phyNum];
    
    //��ʼ��
    initializeList(nextPosition, phyNum);

    //��¼��ǰҳ��ķ������: 0 δ����
    int isVisited;

    for (int i = 0; i < pageNum; i++)
    {
        isVisited = 0;
        //�ж��Ƿ���Ҫ�û�������ڴ���������Ҫ���ʵ�ҳ�治���ڴ��У���ô����Ҫ�û� 
        for (int j = 0; j < phyNum; j++)
        {
        	//�����ҳ���Ѿ������ڴ���
            if (memoryList[j] == strList[i])
            {
                //��¼��һ�η�������λ��
                nextPosition[j] = getNextPosition(memoryList[j], i, strList, pageNum);

                //�޸ķ������
                isVisited = 1;

                //չʾ
                printf("%d��  ��ҳ���Ѿ����������С�\n", strList[i]);
                break;
            }
            //ҳ�治���ڴ������ڴ�δ��
            if (memoryList[j] == -1)
            {
                //ֱ�Ӵ���
                memoryList[j] = strList[i];
                //��¼��һ�η�������λ��
                nextPosition[j] = getNextPosition(memoryList[j], i, strList, pageNum);

				//ȱҳ���� + 1 
                missingCount++;

                //�޸ķ������
                isVisited = 1;

                //չʾ
                printf("%d��", strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        //��ǰҳ�滹û���ʹ�
        if (!isVisited)
        {
            //�ڴ������ҵ�ǰ���ʲ����ڴ���->�����û�
            //1.Ѱ�ҵ�����ű����ʵ���ҳ��
            int max = 0;
            for (int k = 1; k < phyNum; k++)
            {
                if (nextPosition[max] < nextPosition[k])
                {
                    max = k;
                }
            }

            //2.����λ�õ�ҳ�滻��
            memoryList[max] = strList[i];
            //��¼��һ�η�������λ��
            nextPosition[max] = getNextPosition(memoryList[max], i, strList, pageNum);

            missingCount++;
            replaceCount++;

            //չʾ
            printf("%d��", strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    //��Ϣ��ʾ
    informationCount(missingCount, replaceCount, pageNum);
}

//�Ƚ��ȳ��û��㷨
void replacePageByFIFO(int memoryList[], int phyNum, int strList[], int pageNum)
{
    //�û�����
    int replaceCount = 0;
    //ȱҳ����
    int missingCount = 0;

    //��¼��ǰ��������ڴ���±�
    int pointer = 0;

    //��¼��ǰҳ��ķ������: 0 δ����
    int isVisited = 0;
    
    for (int i = 0; i < pageNum; i++)
    {
        isVisited = 0;

        //�ж��Ƿ���Ҫ�û�������ڴ���������Ҫ���ʵ�ҳ�治���ڴ��У���ô����Ҫ�û�
        for (int j = 0; j < phyNum; j++)
        {
        	//��ҳ���Ѿ������ڴ���
            if (memoryList[j] == strList[i])
            {
                //�޸ķ������
                isVisited = 1;
                //�޸ķ���ʱ��
                //չʾ
                printf("%d��  ��ҳ���Ѿ����������С�\n", strList[i]);
                break;
            }
            //ҳ�治���ڴ������ڴ�δ��
            if (memoryList[j] == -1)
            {
                //ֱ�Ӵ���
                memoryList[j] = strList[i];
                //�޸ķ������
                isVisited = 1;
                missingCount++;
                //չʾ
                printf("%d��", strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited)
        {
            //��ǰҳ�滹δ�����ʹ�->��Ҫ����ҳ���û�
            //ֱ�Ӱ����ҳ��浽����¼���±���
            memoryList[pointer] = strList[i];

            //�±�ָ����һ��
            pointer++;

            //����������һ�������±����
            if (pointer > phyNum - 1)
            {
                pointer = 0;
            }

            missingCount++;
            replaceCount++;

            //չʾ
            printf("%d��", strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    informationCount(missingCount, replaceCount, pageNum);
}

//������δʹ���û��㷨
void replacePageByLRU(int memoryList[], int phyNum, int strList[], int pageNum)
{

    //�û�����
    int replaceCount = 0;
    //ȱҳ����
    int missingCount = 0;

    //��¼�ڴ������һ�η��������ʱ��
    int timeRecord[phyNum];
    
    //��ʼ��
    initializeList(timeRecord, phyNum);

    //��¼��ǰҳ��ķ������: 0 δ����
    int isVisited = 0;

    //��¼�Ѿ����ڴ��е�ҳ������
    int pageCount = 0;
    
    for (int i = 0; i < pageNum; i++)
    {
        isVisited = 0;

        //ʱ���һ
        for (int p = 0; p < pageCount; p++)
        {
            if (memoryList[p] != -1)
            {
                timeRecord[p]++;
            }
        }

        //�Ƿ���Ҫ�û�
        for (int j = 0; j < phyNum; j++)
        {
            if (memoryList[j] == strList[i])
            {
                //��ҳ���Ѿ������ڴ���
                //�޸ķ������
                isVisited = 1;
                //���÷���ʱ��
                timeRecord[j] = -1;
                //չʾ
                printf("%d��  ��ҳ���Ѿ����������С�\n", strList[i]);
                break;
            }
            //ҳ�治���ڴ������ڴ�δ��
            if (memoryList[j] == -1)
            {
                //ֱ�Ӵ���
                memoryList[j] = strList[i];
                pageCount++;
                //�޸ķ������
                isVisited = 1;
                //�޸ķ���ʱ��
                timeRecord[j]++;
                missingCount++;
                //չʾ
                printf("%d��", strList[i]);
                showMemoryList(memoryList, phyNum);
                break;
            }
        }

        if (!isVisited)
        {
            //��Ҫ�û�
            //1.����ʱ���¼��Ѱ�����δ���ʵ�ҳ�����ڵ��ڴ��±�
            int max = 0;
            for (int k = 0; k < phyNum; k++)
            {
                if (timeRecord[max] < timeRecord[k])
                {
                    max = k;
                }
            }

            //2.����λ�õ�ҳ�滻��
            memoryList[max] = strList[i];
            timeRecord[max] = -1;

            missingCount++;
            replaceCount++;

            //չʾ
            printf("%d��", strList[i]);
            showMemoryList(memoryList, phyNum);
        }
    }
    informationCount(missingCount, replaceCount, pageNum);
}

/*
----------------------------------------------
----------------ҳ���û��㷨------------------
----------------------------------------------
�����������������3
������Ҫ���ʵ�ҳ��������11
������Ҫ���ʵ�ҳ��ţ�7 0 1 2 0 3 0 4 2 7 1
----------------------------------------------
��ѡ��������û��㷨:
1.OPT ����û��㷨
2.FIFO �Ƚ��ȳ��û��㷨
3.LRU ������δʹ���û��㷨
4.�˳�
----------------------------------------------
������һ����ţ�1
 7 0 1 2 0 3 0 4 2 7 1
7��  ��7��
0��  ��7��   ��0��
1��  ��7��   ��0��   ��1��
2��  ��7��   ��0��   ��2��
0��  ��ҳ���Ѿ����������С�
3��  ��3��   ��0��   ��2��
0��  ��ҳ���Ѿ����������С�
4��  ��4��   ��0��   ��2��
2��  ��ҳ���Ѿ����������С�
7��  ��7��   ��0��   ��2��
1��  ��1��   ��0��   ��2��
ȱҳ����:8      ȱҳ��:8/11
�û�����:5      ������:0.27
----------------------------------------------
��ѡ��������û��㷨:
1.OPT ����û��㷨
2.FIFO �Ƚ��ȳ��û��㷨
3.LRU ������δʹ���û��㷨
4.�˳�
----------------------------------------------
������һ����ţ�2
 7 0 1 2 0 3 0 4 2 7 1
7��  ��7��
0��  ��7��   ��0��
1��  ��7��   ��0��   ��1��
2��  ��2��   ��0��   ��1��
0��  ��ҳ���Ѿ����������С�
3��  ��2��   ��3��   ��1��
0��  ��2��   ��3��   ��0��
4��  ��4��   ��3��   ��0��
2��  ��4��   ��2��   ��0��
7��  ��4��   ��2��   ��7��
1��  ��1��   ��2��   ��7��
ȱҳ����:10     ȱҳ��:10/11
�û�����:7      ������:0.09
----------------------------------------------
��ѡ��������û��㷨:
1.OPT ����û��㷨
2.FIFO �Ƚ��ȳ��û��㷨
3.LRU ������δʹ���û��㷨
4.�˳�
----------------------------------------------
������һ����ţ�3
 7 0 1 2 0 3 0 4 2 7 1
7��  ��7��
0��  ��7��   ��0��
1��  ��7��   ��0��   ��1��
2��  ��2��   ��0��   ��1��
0��  ��ҳ���Ѿ����������С�
3��  ��2��   ��0��   ��3��
0��  ��ҳ���Ѿ����������С�
4��  ��4��   ��0��   ��3��
2��  ��4��   ��0��   ��2��
7��  ��4��   ��7��   ��2��
1��  ��1��   ��7��   ��2��
ȱҳ����:9      ȱҳ��:9/11
�û�����:6      ������:0.18
----------------------------------------------
��ѡ��������û��㷨:
1.OPT ����û��㷨
2.FIFO �Ƚ��ȳ��û��㷨
3.LRU ������δʹ���û��㷨
4.�˳�
----------------------------------------------
������һ����ţ�4

--------------------------------
Process exited after 29.08 seconds with return value 0
�밴���������. . .
*/
