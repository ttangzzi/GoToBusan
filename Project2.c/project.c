#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LEN_MIN 10 // ���� ����
#define LEN_MAX 50
#define PROB_MIN 20 // Ȯ��
#define PROB_MAX 90

// �Լ� ����
void trainState(int len, int citizen, int zombie);
int citizenMove(int p);
int zombieMove(int p, int turn);
void citzomState(int turn, int citizen, int zombie, int citMove, int zomMove);

int main(void) {
	srand((unsigned int)time(NULL)); // ���α׷��� ������ ������ �ٸ� ������ �����
	int trainLength = rand() % (LEN_MAX-LEN_MIN+1)+LEN_MIN;// ������ ������ ���̸� ������ ���
	int p = rand() % (PROB_MAX - PROB_MIN + 1) + PROB_MIN;	// ������ Ȯ���� ������ ���
	int citizenState = trainLength - 5; // �ù��� �ʱ���� ����
	int zombieState = trainLength - 3; // ������ �ʱ���� ����
	int citMove = 0; // �ù� �̵����� (0, 1) ���� ����
	int zomMove = 0; // ���� �̵����� (0, 2) ���� ����
	int count = 0; // �� ���� ���� ���� ����

	// ��Ʈ�� (�̱���)

	// ���� �ʱ� ���� ���
	trainState(trainLength, citizenState, zombieState);
	printf("\n\n\n");
	
	while (1) {
		count++;

		// �ù� �̵�
		citMove = citizenMove(p);
		citizenState -= citMove;

		// ���� �̵�
		zomMove = zombieMove(p, count);
		zombieState -= zomMove;

		// ���� ���� ���
		trainState(trainLength, citizenState, zombieState);

		// �ù�, ���� ����
		citzomState(count,citizenState, zombieState, citMove, zomMove);

		// �������� �˻�
		// �ù��� ���񿡰� �������� �� (�ùλ��� == ��������� ��) [���� ����]
		if (citizenState == zombieState) {
			break;
		}
		// �ù��� ���� ��(1��)�� �������� �� [���� ����]
		else{
			break;
		}
		Sleep(2000);
	}

	// ��Ȳ�� �´� �޼��� ���
	if (citizenState == zombieState) {
		printf("GAME OVER\n");
		printf("Citizen(s) has(have) been attacked by a Zombie\n");
	}
	
	else if (citizenState == 1) {
		printf("SUCCESS!\n");
		printf("citizen(s) escaped to the next train");
	}

	return 0;
}


/*	�� �� ������ ������ ���¸� ����ϴ� �Լ� 
	������ ����, �ùλ���, ������¸� ���ڷ� �ޱ� */
void trainState(int len, int citizen, int zombie) {
	
	// ���� õ��
	for (int i = 0; i < len; i++) {
		printf("#");
	}
	printf("\n");

	// ���� ����
	for (int i = 0; i < len; i++) {
		if (i == 0 || i == len - 1) {
			printf("#");
		}
		else if (i == len - 2) {
			printf("M");
		}
		else if (i == zombie) {
			printf("Z");
		}
		else if (i == citizen) {
			printf("C");
		}
		else {
			printf(" ");
		}
	}

	// ���� �ٴ�
	printf("\n");
	for (int i = 0; i < len; i++) {
		printf("#");
	}
	printf("\n\n");
}


/* Ȯ�� p�� ���� �ù� �̵����� ���� �Լ� */
int citizenMove(int p) {
	int result = rand() % 100 + 1; // 100������ ������ ���� ����

	if (result <= p) {
		return 0; // p% Ȯ���� �̵���������
	}
	else {
		return 1; // (100-p)% Ȯ���� �̵�
	}
}


/* Ȯ�� p�� �� ���� ���� ���� �̵����� ���� �Լ� */
int zombieMove(int p, int turn) {
	int result = rand() % 100 + 1;

	if (turn % 2 == 1) {
		if (result <= p) {
			return 1; // p% Ȯ���� �̵�
		}
		else {
			return 0; // (100-p)% Ȯ���� �̵����� ����
		}
	}
	else {
		return 0;
	}
}

/* �� �� ������ �ùΰ� ���� ���¸� ����ϴ� �Լ� */
void citzomState(int turn, int citizen, int zombie, int citMove, int zomMove) {
	if (citMove == 1) {
		printf("citizen: %d -> %d\n", citizen + 1, citizen);
	}
	else {
		printf("citizen: stay %d\n", citizen);
	}

	if (zomMove == 1) {
		printf("zombie: %d -> %d\n\n", zombie + 1, zombie);
	}
	else {
		if (turn % 2 == 0) {
			printf("zombie: stay %d (cannot move)\n\n", zombie);
	}
		else {
			printf("zombie: stay %d\n\n", zombie);
	}
	}
}