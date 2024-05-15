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
void citzenState(int citizenPosition, int citizenMove, int citizenAggro) {
	if (citizenMove == 1) {
		printf("citizen: %d -> %d (aggro: %d -> %d)\n", citizenPosition + 1, citizenPosition, citizenAggro-1, citizenAggro);
	}
	else {
		printf("citizen: stay %d (aggro: %d -> %d)\n", citizenPosition, citizenAggro+1, citizenAggro);
	}
}


/* ------------- 좀비의 상태를 출력하는 함수 -------------- */
void zombieState(int citizenPosition, int zombiePosition, int madongPosition, int citizenMove, int zombieMove, int turn, int citizenAggro, int madongAggro, int stay) {
	if (zombieMove == 1) {

		// 시민이 마동석보다 어그로가 높거나 같을 때 
		if (citizenAggro >= madongAggro) { // 시민쪽으로 이동
			if (citizenPosition + 1 != zombiePosition+stay) {
				printf("zombie: %d -> %d\n\n", zombiePosition + 1, zombiePosition);
			}
			else {
				printf("zombie: 시민 인접해서 이동불가 %d\n\n", zombiePosition);
			}
			// 시민이 좀비와 인접해있을 때 이동 불가
		}
		// 마동석이 시민보다 어그로가 높을 때
		else if (citizenAggro < madongAggro) { // 마동석쪽으로 이동

			// 마동석이 좀비와 인접해있을 때 이동 불가
			if (madongPosition - 1 != zombiePosition-stay) {
				printf("zombie: %d -> %d\n\n", zombiePosition-1, zombiePosition);
			}
			else {
				printf("zombie: 마동석 인접해서 이동불가 %d\n\n", zombiePosition);
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

/* ------------- 마동석의 상태를 출력하는 함수 -------------- */
void madongState(int madongPosition, int madongMove, int stm, int madongAggro) {
	if (madongMove == 1) {
		printf("madongseok: left(aggro: %d -> %d, stamina: %d)\n", madongAggro - 1, madongAggro, stm);
	}
	else {
		printf("madongseok: stay %d(aggro: %d -> %d, stamina: %d)\n", madongPosition, madongAggro + 1, madongAggro, stm);
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
	int stay = 0;

	int citizenAggro = 1, madongAggro = 1;
	trainState(trainLength, citizenPosition, zombiePosition, madongPosition);
	while (1) {
		turn++;

		// 시민 이동 + 어그로 예외처리 추가
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

		// 좀비 이동
		zombieMove = isZombieMove(p, turn);

		if (zombieMove == 1) {
			// 시민이 마동석보다 어그로가 높거나 같을 때 
			if (citizenAggro >= madongAggro) { // 시민쪽으로 이동

				// 시민이 좀비와 인접해있을 때 이동 불가
				if (citizenPosition + 1 != zombiePosition) {
					zombiePosition -= zombieMove;
					stay = 1;
				}
				else {
					stay = 0;
				}
			}
			// 마동석이 시민보다 어그로가 높을 때
			else if (citizenAggro < madongAggro) { // 마동석쪽으로 이동

				// 마동석이 좀비와 인접해있을 때 이동 불가
				if (madongPosition - 1 != zombiePosition) {
					zombiePosition += zombieMove;
					stay = 1;
				}
				else {
					stay = 0;
				}
			}
		}


		// 열차 상태 출력
		trainState(trainLength, citizenPosition, zombiePosition, madongPosition);

		// 시민, 좀비 상태
		citzenState(citizenPosition, citizenMove, citizenAggro);
		zombieState(citizenPosition, zombiePosition, madongPosition,citizenMove, zombieMove, turn, citizenAggro, madongAggro,stay);

		// 마동석 이동 여부 입력대기
		madongMove = isMadongMove(madongPosition, zombiePosition);


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
		madongState(madongPosition, madongMove, stm, madongAggro);
	}
	

































}