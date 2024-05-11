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
#define AGGRPO_MIN 0 // 어그로 범위
#define AGGROP_MAX 5

// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOBE_STAY 0

// 좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2

// 마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2












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
void citzenState(int citizenPosition, int citizenMove) {
	if (citizenMove == 1) {
		printf("citizen: %d -> %d\n", citizenPosition + 1, citizenPosition);
	}
	else {
		printf("citizen: stay %d\n", citizenPosition);
	}
}


/* ------------- 좀비의 상태를 출력하는 함수 -------------- */
void zombieState(int zombiePosition, int zombieMove, int turn) {
	if (zombieMove == 1) {
		printf("zombie: %d -> %d\n\n", zombiePosition + 1, zombiePosition);
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

/* ------------- 마동석의 상태를 출력하는 함수 -------------- */
void madongState(int madongPosition, int madongMove, int stm) {
	printf("madongseok: stay %d(stamina: %d)\n", madongPosition, stm);
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
int isZombieMove(int p, int turn) {
	int result = rand() % 100 + 1;

	if (turn % 2 == 1) {
		if (result <= p) {
			return 1; // p% 확률로 이동
		}
		else {
			return 0; // (100-p)% 확률로 이동하지 않음
		}
	}
	else {
		return 0; // 짝수 turn에선 움직이지 않기
	}
}


int isMadongMove(void) {
	int result;

	do {
		printf("madongseok move(0:stay, 1:left)>> ");
		scanf_s("%d", &result);
	} while (result != 1 && result != 0);
	

	if (result == MOVE_LEFT) {
		return 1;
	}
	else {
		return 0;
	}
}






int main() {
	srand((unsigned int)time(NULL)); // 프로그램을 실행할 때마다 다른 수열을 만들기

	// 입력
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

	int p = rand() % (PROB_MAX - prob + 1) + prob;	// 랜덤한 확률
	int citizenPosition = trainLength - 6; // 시민의 초기위치
	int zombiePosition = trainLength - 3; // 좀비의 초기위치
	int madongPosition = trainLength - 2; // 마동석의 초기위치

	int citizenMove; // 시민 이동여부 (0, 1)
	int zombieMove; // 좀비 이동여부 (0, 1)
	int madongMove; // 마동석 이동여부 (0, 1)
	int turn = 0; // 턴 수

	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);
	turn++;
	// 시민 이동
	citizenMove = isCitizenMove(p);
	citizenPosition -= citizenMove;

	// 좀비 이동
	zombieMove = isZombieMove(p, turn);
	zombiePosition -= zombieMove;

	// 열차 상태 출력
	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

	// 시민, 좀비 상태
	citzenState(citizenPosition, citizenMove);
	zombieState(zombiePosition, zombieMove, turn);

	// 마동석 이동 여부 입력대기
	madongMove = isMadongMove();
	madongPosition -= madongMove;

	// 열차 상태 출력
	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

	// 마동석 상태
	madongState(madongPosition, madongMove, stm);
}