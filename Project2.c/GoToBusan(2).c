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

/* 2-1 ���� : �λ���(1) �ڵ带 �Լ��� ���� */
/* 2-2 �λ���(1)���� ���� */

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
void citzenState(int citizenPosition, int citizenMove, int citizenAggro, int cPriorAggro) {
	if (citizenMove == 1) {
		if (citizenAggro == cPriorAggro) {
			printf("citizen: %d -> %d (aggro: %d)\n", citizenPosition + 1, citizenPosition, citizenAggro);
		}
		else {
			printf("citizen: %d -> %d (aggro: %d -> %d)\n", citizenPosition + 1, citizenPosition, cPriorAggro, citizenAggro);
		}
	}
	else {
		if (citizenAggro == cPriorAggro) {
			printf("citizen: stay %d (aggro: %d)\n", citizenPosition, citizenAggro);
		}
		else {
			printf("citizen: stay %d (aggro: %d -> %d)\n", citizenPosition, cPriorAggro, citizenAggro);
		}
	}
}


/* ------------- ������ ���¸� ����ϴ� �Լ� -------------- */
void zombieState(int citizenPosition, int zombiePosition, int madongPosition,int zombieMove, int turn, int move) {
	
	if (zombieMove == 1) { // �ù������� �̵�
		if (citizenPosition + 1 == zombiePosition+move) { // �����ϴٸ� �̵� �Ұ�
			printf("zombie: ����(��) %d\n\n", zombiePosition);
		}
		else {
			printf("zombie: %d -> %d\n\n", zombiePosition + 1, zombiePosition);
		}
	}
	else if (zombieMove == -1) { // ������������ �̵�
		if (madongPosition - 1 == zombiePosition-move) { // �����ϴٸ� �̵� �Ұ�
			printf("zombie: ����(��) %d\n\n", zombiePosition);
		}
		else {
			printf("zombie: %d -> %d\n\n", zombiePosition - 1, zombiePosition);
		}
	}
	else if (zombieMove == 100) {
		printf("madongseok pulled zombie (cannot move)\n\n");
	}
	else { // ��� ����
		if (turn % 2 == 0) { // ��� ������ ¦������ ��
			printf("zombie: stay %d (cannot move)\n\n", zombiePosition);
		}
		else {
			printf("zombie: stay %d\n\n", zombiePosition);
		}
	}
}

/* ------------- �������� ���¸� ����ϴ� �Լ� -------------- */
void madongState(int madongPosition, int madongMove, int stm, int madongAggro, int mPriorAggro) {
	if (madongMove == 1) {
		if (madongAggro == mPriorAggro) {
			printf("madongseok: left(aggro: %d, stamina: %d)\n\n", madongAggro, stm);
		}
		else {
			printf("madongseok: left(aggro: %d -> %d, stamina: %d)\n\n", mPriorAggro, madongAggro, stm);
		}
	}
	else {
		if (madongAggro == mPriorAggro) {
			printf("madongseok: stay(aggro: %d, stamina: %d)\n\n", madongAggro, stm);
		}
		else {
			printf("madongseok: stay(aggro: %d -> %d, stamina: %d)\n\n", mPriorAggro, madongAggro, stm);
		}
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
int isZombieMove(int p, int turn, int madongPull, int citizenAggro, int madongAggro) {
	int result = rand() % 100 + 1;
	int move;

	if (turn % 2 == 1) {
		if (result <= p) {
			if (citizenAggro >= madongAggro) {
				move = 1;
			}
			else {
				move = -1;
			}
		}
		else {
			move = 0; // (100-p)% Ȯ���� �̵����� ����
		}

		if (madongPull == 1) {
			move = 100;
		}
	}
	else {
		move = 0; // ¦�� turn���� �������� �ʱ�
	}
	return move;
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

int isMadongPull(int p) {
	int result = rand() % 100 + 1;

	if (result <= p) {
		return 0; // p% Ȯ���� ����
	}
	else {
		return 1; // (100-p)% Ȯ���� ����
	}
}






int main() {
	srand((unsigned int)time(NULL)); // ���α׷��� ������ ������ �ٸ� ������ �����

	// �Է�
	int trainLength, stm, prob;

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
	int move = 0;
	int citizenAggro = 1, madongAggro = 1;
	int cPriorAggro, mPriorAggro;

	int zombieAttack = ATK_NONE;
	int madongAction;
	int madongPull = 0;
	
	// �ʱ� �������� ���
	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

	while (1) {
		turn++;

		/* 2-2 : 2 Phases */

		/* 2-3 : �̵� */

		// �ù� �̵�
		citizenMove = isCitizenMove(p); // [���: 0 , �̵�: 1]
		citizenPosition -= citizenMove;
		cPriorAggro = citizenAggro;

		// �ù� �̵� -> �ù� ��׷� ����, �ù� ��� -> �ù� ��׷� ����
		if (citizenMove == 1) {
			citizenAggro++;
			if (citizenAggro > AGGRO_MAX) citizenAggro = AGGRO_MAX;
		}
		else {
			citizenAggro--;
			if (citizenAggro < AGGRO_MIN) citizenAggro = AGGRO_MIN;
		}

		// ���� �̵�
		zombieMove = isZombieMove(p,turn, madongPull, citizenAggro, madongAggro);
		// [���: 0 , �ù��� �̵�: 1, �������� �̵�: -1]

		if (zombieMove == 1) { // �ù� �̵�
			if (citizenPosition + 1 != zombiePosition) { // ������ �� �����ϰ� �̵�
				zombiePosition -= zombieMove;
				move = 1;
			}
			else move = 0;
		}
		else if (zombieMove == -1) { // �������� �̵�
			if (madongPosition - 1 != zombiePosition) { // ������ �� �����ϰ� �̵�
				zombiePosition -= zombieMove;
				move = 1;
			}
			else move = 0;
		}

		// ���� ���� ���
		trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

		// �ù�, ���� ����
		citzenState(citizenPosition, citizenMove, citizenAggro, cPriorAggro);
		zombieState(citizenPosition, zombiePosition, madongPosition, zombieMove, turn, move);

		// ������ �̵� ���� �Է´��
		madongMove = isMadongMove(madongPosition, zombiePosition);
		mPriorAggro = madongAggro;

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
		madongState(madongPosition, madongMove, stm, madongAggro, mPriorAggro);


		/* 2-4 : �ൿ */

		// �ù��� �ൿ ���
		printf("\n");
		if (citizenPosition == 1) {
			printf("YOU WIN!\n");
			break;
		}
		else {
			printf("citizen does nothing.\n");
		}

		// ������ �ൿ ���
		if (citizenPosition + 1 == zombiePosition && madongPosition - 1 == zombiePosition) {
			if (citizenAggro <= madongAggro) {
				stm--;
				if (stm <= STM_MIN) {
					printf("GAME OVER madongseok dead...\n");
					break;
				}
				else {
					printf("zombie attacked madongseok(aggro: %d vs %d, madongseok stamina: %d -> %d)\n", citizenAggro, madongAggro, stm + 1, stm);
				}
			}
			else if (citizenAggro > madongAggro) {
				printf("GAME OVER citizen dead...\n");
				break;
			}
		}
		else if (citizenPosition + 1 == zombiePosition) {
			printf("GAME OVER citizen dead...\n");
			break;
		}
		else if (madongPosition - 1 == zombiePosition) {
			stm--;
			if (stm <= STM_MIN) {
				printf("GAME OVER madongseok dead...\n");
				break;
			}
			else {
				printf("zombie attacked madongseok(madongseok stamina: %d -> %d)\n", stm + 1, stm);
			}
		}
		else {
			printf("zombie attacked nobody.\n");
		}
		printf("\n");
		// �������� �ൿ
		madongPull = 0;
		if (madongPosition - 1 == zombiePosition) { // ����� ������ ���
			do {
				printf("madongseok action(%d:rest, %d:provoke, %d:pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
				scanf_s("%d", &madongAction);
			} while (madongAction != ACTION_REST && madongAction != ACTION_PROVOKE && madongAction != ACTION_PULL);

		}
		else { // �������� ���� ���
			do {
				printf("madongseok action(%d:rest, %d:provoke)>> ", ACTION_REST, ACTION_PROVOKE);
				scanf_s("%d", &madongAction);
			} while (madongAction != ACTION_REST && madongAction != ACTION_PROVOKE);
		}
		printf("\n");

		if (madongAction == ACTION_REST) {
			madongAggro -= 1;
			if (madongAggro < AGGRO_MIN) madongAggro = AGGRO_MIN;
			stm += 1;
			if (stm > STM_MAX) stm = STM_MAX;

			printf("madongseok rests...\n");
			printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n\n", madongPosition, madongAggro, stm - 1, stm);
		}
		else if (madongAction == ACTION_PROVOKE) {
			madongAggro = AGGRO_MAX;
			printf("madongseok provoked zombie...\n");
			printf("madongseok: %d (aggro: %d, stamina: %d)\n\n", madongPosition, madongAggro, stm);
		}
		else {
			madongAggro += 2;
			if (madongAggro > AGGRO_MAX) madongAggro = AGGRO_MAX;
			stm--;
			if (stm < STM_MIN) stm = STM_MIN;

			madongPull = isMadongPull(p);
			if (madongPull == 0) { // ����
				printf("madongseok failed to pull zombie\n");
				printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n\n", madongPosition, madongAggro, stm+1, stm);
			}
			else {
				printf("madongseok pulled zombie... Next turn, it can't move\n\n");
				madongPull = 1;
			}
		}
	}
}