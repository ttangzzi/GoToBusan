#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50
#define STM_MIN 0 // 마동석 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 0 // 어그로 범위
#define AGGRO_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

/* 2-1 정리 : 부산헹(1) 코드를 함수로 정리 */
/* 2-2 부산헹(1)에서 수정 */

/* ------------ 열차의 상태를 출력하는 함수 ------------ */
void trainState(int trainLength, int citizenPosition, int zombiePosition, int madongPosition) {
	// 기차 천장
	for (int i = 0; i < trainLength; i++) printf("#");
	printf("\n");

	// 기차 내부
	for (int i = 0; i < trainLength; i++) {
		if (i == 0 || i == trainLength - 1) printf("#");
		else if (i == madongPosition) printf("M");
		else if (i == zombiePosition) printf("Z");
		else if (i == citizenPosition) printf("C");
		else printf(" ");
	}
	printf("\n");
	
	// 기차 바닥
	for (int i = 0; i < trainLength; i++) printf("#");
	printf("\n\n");
}


/* ------------- 시민의 상태를 출력하는 함수 -------------- */
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


/* ------------- 좀비의 상태를 출력하는 함수 -------------- */
void zombieState(int citizenPosition, int zombiePosition, int madongPosition,int zombieMove, int turn, int move) {
	
	if (zombieMove == 1) { // 시민쪽으로 이동
		if (citizenPosition + 1 == zombiePosition+move) { // 인접하다면 이동 불가
			printf("zombie: 인접(시) %d\n\n", zombiePosition);
		}
		else {
			printf("zombie: %d -> %d\n\n", zombiePosition + 1, zombiePosition);
		}
	}
	else if (zombieMove == -1) { // 마동석쪽으로 이동
		if (madongPosition - 1 == zombiePosition-move) { // 인접하다면 이동 불가
			printf("zombie: 인접(마) %d\n\n", zombiePosition);
		}
		else {
			printf("zombie: %d -> %d\n\n", zombiePosition - 1, zombiePosition);
		}
	}
	else if (zombieMove == 100) {
		printf("madongseok pulled zombie (cannot move)\n\n");
	}
	else { // 대기 상태
		if (turn % 2 == 0) { // 대기 이유가 짝수턴일 때
			printf("zombie: stay %d (cannot move)\n\n", zombiePosition);
		}
		else {
			printf("zombie: stay %d\n\n", zombiePosition);
		}
	}
}

/* ------------- 마동석의 상태를 출력하는 함수 -------------- */
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


/* ---- 확률 p에 따른 시민 이동여부 함수 -------- */
int isCitizenMove(int p) {
	int result = rand() % 100 + 1; // 100까지의 랜덤한 숫자 생성

	if (result <= p) {
		return 0; // p% 확률로 이동하지않음
	}
	else {
		return 1; // (100-p)% 확률로 이동
	}
}


/* ---- 확률 p와 턴 수에 따른 좀비 이동여부 함수 ---- */
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
			move = 0; // (100-p)% 확률로 이동하지 않음
		}

		if (madongPull == 1) {
			move = 100;
		}
	}
	else {
		move = 0; // 짝수 turn에선 움직이지 않기
	}
	return move;
}


int isMadongMove(int madongPosition, int zombiePosition) {
	int result;
	// 마동석과 좀비가 인접한 상태일 때
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
		return 0; // p% 확률로 실패
	}
	else {
		return 1; // (100-p)% 확률로 성공
	}
}






int main() {
	srand((unsigned int)time(NULL)); // 프로그램을 실행할 때마다 다른 수열을 만들기

	// 입력
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

	int p = rand() % (PROB_MAX - prob + 1) + prob;	// 랜덤한 확률
	int citizenPosition = trainLength - 6; // 시민의 초기위치
	int zombiePosition = trainLength - 3; // 좀비의 초기위치
	int madongPosition = trainLength - 2; // 마동석의 초기위치

	int citizenMove; // 시민 이동여부 (0, 1)
	int zombieMove; // 좀비 이동여부 (0, 1)
	int madongMove; // 마동석 이동여부 (0, 1)
	int turn = 0; // 턴 수
	int move = 0;
	int citizenAggro = 1, madongAggro = 1;
	int cPriorAggro, mPriorAggro;

	int zombieAttack = ATK_NONE;
	int madongAction;
	int madongPull = 0;
	
	// 초기 열차상태 출력
	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

	while (1) {
		turn++;

		/* 2-2 : 2 Phases */

		/* 2-3 : 이동 */

		// 시민 이동
		citizenMove = isCitizenMove(p); // [대기: 0 , 이동: 1]
		citizenPosition -= citizenMove;
		cPriorAggro = citizenAggro;

		// 시민 이동 -> 시민 어그로 증가, 시민 대기 -> 시민 어그로 감소
		if (citizenMove == 1) {
			citizenAggro++;
			if (citizenAggro > AGGRO_MAX) citizenAggro = AGGRO_MAX;
		}
		else {
			citizenAggro--;
			if (citizenAggro < AGGRO_MIN) citizenAggro = AGGRO_MIN;
		}

		// 좀비 이동
		zombieMove = isZombieMove(p,turn, madongPull, citizenAggro, madongAggro);
		// [대기: 0 , 시민쪽 이동: 1, 마동석쪽 이동: -1]

		if (zombieMove == 1) { // 시민 이동
			if (citizenPosition + 1 != zombiePosition) { // 인접할 때 제외하고 이동
				zombiePosition -= zombieMove;
				move = 1;
			}
			else move = 0;
		}
		else if (zombieMove == -1) { // 마동석쪽 이동
			if (madongPosition - 1 != zombiePosition) { // 인접할 때 제외하고 이동
				zombiePosition -= zombieMove;
				move = 1;
			}
			else move = 0;
		}

		// 열차 상태 출력
		trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

		// 시민, 좀비 상태
		citzenState(citizenPosition, citizenMove, citizenAggro, cPriorAggro);
		zombieState(citizenPosition, zombiePosition, madongPosition, zombieMove, turn, move);

		// 마동석 이동 여부 입력대기
		madongMove = isMadongMove(madongPosition, zombiePosition);
		mPriorAggro = madongAggro;

		if (madongMove == 1) { // 왼쪽으로 이동 + 어그로 증가
			madongPosition -= madongMove;
			madongAggro++;
			if (madongAggro > AGGRO_MAX) madongAggro = AGGRO_MAX;

		}
		else { // 대기 + 어그로 감소
			madongAggro--;
			if (madongAggro < AGGRO_MIN) madongAggro = AGGRO_MIN;
		}

		// 열차 상태 출력
		trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

		// 마동석 상태
		madongState(madongPosition, madongMove, stm, madongAggro, mPriorAggro);


		/* 2-4 : 행동 */

		// 시민의 행동 출력
		printf("\n");
		if (citizenPosition == 1) {
			printf("YOU WIN!\n");
			break;
		}
		else {
			printf("citizen does nothing.\n");
		}

		// 좀비의 행동 출력
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
		// 마동석의 행동
		madongPull = 0;
		if (madongPosition - 1 == zombiePosition) { // 좀비와 인접한 경우
			do {
				printf("madongseok action(%d:rest, %d:provoke, %d:pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
				scanf_s("%d", &madongAction);
			} while (madongAction != ACTION_REST && madongAction != ACTION_PROVOKE && madongAction != ACTION_PULL);

		}
		else { // 인접하지 않은 경우
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
			if (madongPull == 0) { // 실패
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