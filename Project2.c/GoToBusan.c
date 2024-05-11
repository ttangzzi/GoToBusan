//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <Windows.h>
//
//#define LEN_MIN 15 // 기차 길이
//#define LEN_MAX 50
//#define PROB_MIN 10 // 확률
//#define PROB_MAX 90
//
//
///*	각 턴 마다의 열차의 상태를 출력하는 함수 
//	열차의 길이, 시민상태, 좀비상태를 인자로 받기 */
//void trainState(int len, int citizen, int zombie) {
//	
//	// 기차 천장
//	for (int i = 0; i < len; i++) {
//		printf("#");
//	}
//	printf("\n");
//
//	// 기차 내부
//	for (int i = 0; i < len; i++) {
//		if (i == 0 || i == len - 1) {
//			printf("#");
//		}
//		else if (i == len - 2) {
//			printf("M");
//		}
//		else if (i == zombie) {
//			printf("Z");
//		}
//		else if (i == citizen) {
//			printf("C");
//		}
//		else {
//			printf(" ");
//		}
//	}
//
//	// 기차 바닥
//	printf("\n");
//	for (int i = 0; i < len; i++) {
//		printf("#");
//	}
//	printf("\n\n");
//}
//
//
///* 확률 p에 따른 시민 이동여부 리턴 함수 */
//int citizenMove(int p) {
//	int result = rand() % 100 + 1; // 100까지의 랜덤한 숫자 생성
//
//	if (result <= p) {
//		return 0; // p% 확률로 이동하지않음
//	}
//	else {
//		return 1; // (100-p)% 확률로 이동
//	}
//}
//
//
///* 확률 p와 턴 수에 따른 좀비 이동여부 리턴 함수 */
//int zombieMove(int p, int turn) {
//	int result = rand() % 100 + 1;
//
//	if (turn % 2 == 1) {
//		if (result <= p) {
//			return 1; // p% 확률로 이동
//		}
//		else {
//			return 0; // (100-p)% 확률로 이동하지 않음
//		}
//	}
//	else {
//		return 0;
//	}
//}
//
///* 각 턴 마다의 시민과 좀비 상태를 출력하는 함수 */
//void citzomState(int turn, int citizen, int zombie, int citMove, int zomMove) {
//	if (citMove == 1) {
//		printf("citizen: %d -> %d\n", citizen + 1, citizen);
//	}
//	else {
//		printf("citizen: stay %d\n", citizen);
//	}
//
//	if (zomMove == 1) {
//		printf("zombie: %d -> %d\n\n", zombie + 1, zombie);
//	}
//	else {
//		if (turn % 2 == 0) {
//			printf("zombie: stay %d (cannot move)\n\n", zombie);
//	}
//		else {
//			printf("zombie: stay %d\n\n", zombie);
//	}
//	}
//}
//
//int main(void) {
//	int len = 0;
//	int prob = 0;
//	// 인트로
//	printf(" _____  _____   _____  _____  ______  _   _  _____   ___   _   _ \n");
//	printf("|  __ \\|  _  | |_   _||  _  | | ___ \\| | | |/  ___| / _ \\ | \\ | |\n");
//	printf("| |  \\/| | | |   | |  | | | | | |_/ /| | | |\\ `--. / /_\\ \\|  \\| |\n");
//	printf("| | __ | | | |   | |  | | | | | ___ \\| | | | `--. \\|  _  || . ` |\n");
//	printf("| |_\\ \\\\ \\_/ /   | |  \\ \\_/ / | |_/ /| |_| |/\\__/ /| | | || |\\  |\n");
//	printf(" \\____/ \\___/    \\_/   \\___/  \\____/  \\___/ \\____/ \\_| |_/\\_| \\_/\n");
//	printf("\n");
//	Sleep(3000);
//	system("cls");
//	// 열차의 길이와 확률 p를 입력받음
//	printf("train length(%d~%d) >> ",LEN_MIN,LEN_MAX);
//	scanf_s("%d", &len);
//	printf("percentile probability 'p' (%d~%d) >> ",PROB_MIN, PROB_MAX);
//	scanf_s("%d", &prob);
//	if (len < LEN_MIN || len > LEN_MAX) {
//		exit(1);
//	}
//
//	if (prob < PROB_MIN || prob > PROB_MAX) {
//		exit(1);
//	}
//	system("cls");
//	
//
//	srand((unsigned int)time(NULL)); // 프로그램을 실행할 때마다 다른 수열을 만들기
//	int trainLength = rand() % (LEN_MAX - len + 1) + len;// 랜덤한 기차의 길이를 변수에 담기
//	int p = rand() % (PROB_MAX - prob + 1) + prob;	// 랜덤한 확률을 변수에 담기
//	int citizenPosition = trainLength - 5; // 시민의 초기상태 선언
//	int zombieState = trainLength - 3; // 좀비의 초기상태 선언
//	int citMove = 0; // 시민 이동여부 (0, 1) 저장 변수
//	int zomMove = 0; // 좀비 이동여부 (0, 2) 저장 변수
//	int count = 0; // 턴 수를 세기 위한 변수
//
//	
//
//	// 열차 초기 상태 출력
//	trainState(trainLength, citizenPosition, zombieState);
//	printf("\n\n\n");
//
//	while (1) {
//		count++;
//
//		// 시민 이동
//		citMove = citizenMove(p);
//		citizenPosition -= citMove;
//
//		// 좀비 이동
//		zomMove = zombieMove(p, count);
//		zombieState -= zomMove;
//
//		// 열차 상태 출력
//		trainState(trainLength, citizenPosition, zombieState);
//
//		// 시민, 좀비 상태
//		citzomState(count, citizenPosition, zombieState, citMove, zomMove);
//
//		// 종료조건 검사
//		// 시민이 좀비에게 붙잡혔을 때 (시민상태 == 좀비상태일 때) [구출 실패]
//		if (citizenPosition == zombieState) {
//			break;
//		}
//		// 시민이 왼쪽 끝(1번)에 도달했을 때 [구출 성공]
//		else if (citizenPosition == 1) {
//			break;
//		}
//		Sleep(4000);
//	}
//
//	// 상황에 맞는 메세지 출력
//	if (citizenPosition == zombieState) {
//		printf("GAME OVER\n");
//		printf("Citizen(s) has(have) been attacked by a Zombie\n");
//		Sleep(2000);
//	}
//
//	else if (citizenPosition == 1) {
//		printf("SUCCESS!\n");
//		printf("citizen(s) escaped to the next train");
//		Sleep(2000);
//	}
//	
//	system("cls");
//	// 아웃트로
//	printf(" _____  _____   _____  _____  ______  _   _  _____   ___   _   _ \n");
//	printf("|  __ \\|  _  | |_   _||  _  | | ___ \\| | | |/  ___| / _ \\ | \\ | |\n");
//	printf("| |  \\/| | | |   | |  | | | | | |_/ /| | | |\\ `--. / /_\\ \\|  \\| |\n");
//	printf("| | __ | | | |   | |  | | | | | ___ \\| | | | `--. \\|  _  || . ` |\n");
//	printf("| |_\\ \\\\ \\_/ /   | |  \\ \\_/ / | |_/ /| |_| |/\\__/ /| | | || |\\  |\n");
//	printf(" \\____/ \\___/    \\_/   \\___/  \\____/  \\___/ \\____/ \\_| |_/\\_| \\_/\n");
//	printf("\n\n");
//
//	printf(" _____  _   _   ___   _   _  _   __ __   __ _____  _   _   _ \n");
//	printf("|_   _|| | | | / _ \\ | \\ | || | / / \\ \\ / /|  _  || | | | | |\n");
//	printf("  | |  | |_| |/ /_\\ \\|  \\| || |/ /   \\ V / | | | || | | | | |\n");
//	printf("  | |  |  _  ||  _  || . ` ||    \\    \\ /  | | | || | | | | |\n");
//	printf("  | |  | | | || | | || |\\  || |\\  \\   | |  \\ \\_/ /| |_| | |_|\n");
//	printf("  \\_/  \\_| |_/\\_| |_/\\_| \\_/\\_| \\_/   \\_/   \\___/  \\___/  (_)\n");
//	printf("\n");
//
//	Sleep(2000);
//	return 0;
//}