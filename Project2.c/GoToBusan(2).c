#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LEN_MIN 15 // ���� ����
#define LEN_MAX 50
#define STM_MIN 0 // ������ ü��
#define STM_MAX 5
#define PROB_MIN 10 // Ȯ��
#define PROB_MAX 90
#define AGGRO_MIN 0 // ��׷� ����
#define AGGRO_MAX 5

// ������ �̵� ����
#define MOVE_LEFT 1
#define MOVE_STAY 0

// ������ ���� ���
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// ������ �ൿ
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2












/* ------------ ������ ���¸� ����ϴ� �Լ� ------------ */
void trainState(int trainLength, int citizenPosition, int zombiePosition, int madongPosition) {
	// ���� õ��
	for (int i = 0; i < trainLength; i++) printf("#");
	printf("\n");

	// ���� ����
	for (int i = 0; i < trainLength; i++) {
		if (i == 0 || i == trainLength - 1) printf("#");
		else if (i == madongPosition) printf("M");
		else if (i == zombiePosition) printf("Z");
		else if (i == citizenPosition) printf("C");
		else printf(" ");
	}
	printf("\n");
	
	// ���� �ٴ�
	for (int i = 0; i < trainLength; i++) printf("#");
	printf("\n\n");
}


/* ------------- �ù��� ���¸� ����ϴ� �Լ� -------------- */
void citzenState(int citizenPosition, int citizenMove, int citizenAggro) {
	if (citizenMove == 1) {
		printf("citizen: %d -> %d (aggro: %d -> %d)\n", citizenPosition + 1, citizenPosition, citizenAggro-1, citizenAggro);
	}
	else {
		printf("citizen: stay %d (aggro: %d -> %d)\n", citizenPosition, citizenAggro+1, citizenAggro);
	}
}


/* ------------- ������ ���¸� ����ϴ� �Լ� -------------- */
void zombieState(int citizenPosition, int zombiePosition, int madongPosition, int citizenMove, int zombieMove, int turn, int citizenAggro, int madongAggro, int stay) {
	if (zombieMove == 1) {

		// �ù��� ���������� ��׷ΰ� ���ų� ���� �� 
		if (citizenAggro >= madongAggro) { // �ù������� �̵�
			if (citizenPosition + 1 != zombiePosition+stay) {
				printf("zombie: %d -> %d\n\n", zombiePosition + 1, zombiePosition);
			}
			else {
				printf("zombie: �ù� �����ؼ� �̵��Ұ� %d\n\n", zombiePosition);
			}
			// �ù��� ����� ���������� �� �̵� �Ұ�
		}
		// �������� �ùκ��� ��׷ΰ� ���� ��
		else if (citizenAggro < madongAggro) { // ������������ �̵�

			// �������� ����� ���������� �� �̵� �Ұ�
			if (madongPosition - 1 != zombiePosition-stay) {
				printf("zombie: %d -> %d\n\n", zombiePosition-1, zombiePosition);
			}
			else {
				printf("zombie: ������ �����ؼ� �̵��Ұ� %d\n\n", zombiePosition);
			}
		}
	}
	else {
		if (turn % 2 == 0) {
			printf("zombie: stay %d (cannot move)\n\n", zombiePosition);
		}
		else {
			printf("zombie: stay %d\n\n", zombiePosition);
		}
	}
}

/* ------------- �������� ���¸� ����ϴ� �Լ� -------------- */
void madongState(int madongPosition, int madongMove, int stm, int madongAggro) {
	if (madongMove == 1) {
		printf("madongseok: left(aggro: %d -> %d, stamina: %d)\n", madongAggro - 1, madongAggro, stm);
	}
	else {
		printf("madongseok: stay %d(aggro: %d -> %d, stamina: %d)\n", madongPosition, madongAggro + 1, madongAggro, stm);
	}
	
}


/* ---- Ȯ�� p�� ���� �ù� �̵����� �Լ� -------- */
int isCitizenMove(int p) {
	int result = rand() % 100 + 1; // 100������ ������ ���� ����

	if (result <= p) {
		return 0; // p% Ȯ���� �̵���������
	}
	else {
		return 1; // (100-p)% Ȯ���� �̵�
	}
}


/* ---- Ȯ�� p�� �� ���� ���� ���� �̵����� �Լ� ---- */
int isZombieMove(int p, int turn) {
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
		return 0; // ¦�� turn���� �������� �ʱ�
	}
}


int isMadongMove(int madongPosition, int zombiePosition) {
	int result;
	// �������� ���� ������ ������ ��
	if (madongPosition-1 == zombiePosition) {
		do {
			printf("madongseok move(0:stay)>> ");
			scanf_s("%d", &result);
		} while (result != MOVE_STAY);
	}
	else {
		do {
			printf("madongseok move(0:stay, 1:left)>> ");
			scanf_s("%d", &result);
		} while (result != MOVE_LEFT && result != MOVE_STAY);
	}
	

	if (result == MOVE_LEFT) {
		return 1;
	}
	else {
		return 0;
	}
}






int main() {
	srand((unsigned int)time(NULL)); // ���α׷��� ������ ������ �ٸ� ������ �����

	// �Է�
	int trainLength = 0, stm = 0, prob = 0;

	do {
		printf("train length(%d~%d) >> ", LEN_MIN, LEN_MAX);
		scanf_s("%d", &trainLength);
	} while (trainLength < LEN_MIN || trainLength > LEN_MAX);

	do {
		printf("madongseok stamina(%d~%d) >> ", STM_MIN, STM_MAX);
		scanf_s("%d", &stm);
	} while (stm < STM_MIN || stm > STM_MAX);

	do {
		printf("percentile probability \'p\' (%d~%d) >> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &prob);
	} while (prob < PROB_MIN || prob > PROB_MAX);
	printf("\n\n");

	int p = rand() % (PROB_MAX - prob + 1) + prob;	// ������ Ȯ��
	int citizenPosition = trainLength - 6; // �ù��� �ʱ���ġ
	int zombiePosition = trainLength - 3; // ������ �ʱ���ġ
	int madongPosition = trainLength - 2; // �������� �ʱ���ġ

	int citizenMove; // �ù� �̵����� (0, 1)
	int zombieMove; // ���� �̵����� (0, 1)
	int madongMove; // ������ �̵����� (0, 1)
	int turn = 0; // �� ��
	int stay = 0;

	int citizenAggro = 1, madongAggro = 1;
	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);
	while (1) {
		turn++;

		// �ù� �̵� + ��׷� ����ó�� �߰�
		citizenMove = isCitizenMove(p);

		if (citizenMove == 1) {
			citizenPosition -= citizenMove;
			citizenAggro++;

			if (citizenAggro > AGGRO_MAX) citizenAggro = AGGRO_MAX;

		}
		else {
			citizenAggro--;
			if (citizenAggro < AGGRO_MIN) citizenAggro = AGGRO_MIN;
		}

		// ���� �̵�
		zombieMove = isZombieMove(p, turn);

		if (zombieMove == 1) {
			// �ù��� ���������� ��׷ΰ� ���ų� ���� �� 
			if (citizenAggro >= madongAggro) { // �ù������� �̵�

				// �ù��� ����� ���������� �� �̵� �Ұ�
				if (citizenPosition + 1 != zombiePosition) {
					zombiePosition -= zombieMove;
					stay = 1;
				}
				else {
					stay = 0;
				}
			}
			// �������� �ùκ��� ��׷ΰ� ���� ��
			else if (citizenAggro < madongAggro) { // ������������ �̵�

				// �������� ����� ���������� �� �̵� �Ұ�
				if (madongPosition - 1 != zombiePosition) {
					zombiePosition += zombieMove;
					stay = 1;
				}
				else {
					stay = 0;
				}
			}
		}


		// ���� ���� ���
		trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

		// �ù�, ���� ����
		citzenState(citizenPosition, citizenMove, citizenAggro);
		zombieState(citizenPosition, zombiePosition, madongPosition,citizenMove, zombieMove, turn, citizenAggro, madongAggro,stay);

		// ������ �̵� ���� �Է´��
		madongMove = isMadongMove(madongPosition, zombiePosition);


		if (madongMove == 1) { // �������� �̵� + ��׷� ����
			madongPosition -= madongMove;
			madongAggro++;

			if (madongAggro > AGGRO_MAX) madongAggro = AGGRO_MAX;

		}
		else { // ��� + ��׷� ����
			madongAggro--;
			if (madongAggro < AGGRO_MIN) madongAggro = AGGRO_MIN;
		}


		// ���� ���� ���
		trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

		// ������ ����
		madongState(madongPosition, madongMove, stm, madongAggro);
	}
	

































}