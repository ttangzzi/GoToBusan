//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
//#include <Windows.h>
//
//#define LEN_MIN 15 // ���� ����
//#define LEN_MAX 50
//#define PROB_MIN 10 // Ȯ��
//#define PROB_MAX 90
//
//
///*	�� �� ������ ������ ���¸� ����ϴ� �Լ� 
//	������ ����, �ùλ���, ������¸� ���ڷ� �ޱ� */
//void trainState(int len, int citizen, int zombie) {
//	
//	// ���� õ��
//	for (int i = 0; i < len; i++) {
//		printf("#");
//	}
//	printf("\n");
//
//	// ���� ����
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
//	// ���� �ٴ�
//	printf("\n");
//	for (int i = 0; i < len; i++) {
//		printf("#");
//	}
//	printf("\n\n");
//}
//
//
///* Ȯ�� p�� ���� �ù� �̵����� ���� �Լ� */
//int citizenMove(int p) {
//	int result = rand() % 100 + 1; // 100������ ������ ���� ����
//
//	if (result <= p) {
//		return 0; // p% Ȯ���� �̵���������
//	}
//	else {
//		return 1; // (100-p)% Ȯ���� �̵�
//	}
//}
//
//
///* Ȯ�� p�� �� ���� ���� ���� �̵����� ���� �Լ� */
//int zombieMove(int p, int turn) {
//	int result = rand() % 100 + 1;
//
//	if (turn % 2 == 1) {
//		if (result <= p) {
//			return 1; // p% Ȯ���� �̵�
//		}
//		else {
//			return 0; // (100-p)% Ȯ���� �̵����� ����
//		}
//	}
//	else {
//		return 0;
//	}
//}
//
///* �� �� ������ �ùΰ� ���� ���¸� ����ϴ� �Լ� */
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
//	// ��Ʈ��
//	printf(" _____  _____   _____  _____  ______  _   _  _____   ___   _   _ \n");
//	printf("|  __ \\|  _  | |_   _||  _  | | ___ \\| | | |/  ___| / _ \\ | \\ | |\n");
//	printf("| |  \\/| | | |   | |  | | | | | |_/ /| | | |\\ `--. / /_\\ \\|  \\| |\n");
//	printf("| | __ | | | |   | |  | | | | | ___ \\| | | | `--. \\|  _  || . ` |\n");
//	printf("| |_\\ \\\\ \\_/ /   | |  \\ \\_/ / | |_/ /| |_| |/\\__/ /| | | || |\\  |\n");
//	printf(" \\____/ \\___/    \\_/   \\___/  \\____/  \\___/ \\____/ \\_| |_/\\_| \\_/\n");
//	printf("\n");
//	Sleep(3000);
//	system("cls");
//	// ������ ���̿� Ȯ�� p�� �Է¹���
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
//	srand((unsigned int)time(NULL)); // ���α׷��� ������ ������ �ٸ� ������ �����
//	int trainLength = rand() % (LEN_MAX - len + 1) + len;// ������ ������ ���̸� ������ ���
//	int p = rand() % (PROB_MAX - prob + 1) + prob;	// ������ Ȯ���� ������ ���
//	int citizenPosition = trainLength - 5; // �ù��� �ʱ���� ����
//	int zombieState = trainLength - 3; // ������ �ʱ���� ����
//	int citMove = 0; // �ù� �̵����� (0, 1) ���� ����
//	int zomMove = 0; // ���� �̵����� (0, 2) ���� ����
//	int count = 0; // �� ���� ���� ���� ����
//
//	
//
//	// ���� �ʱ� ���� ���
//	trainState(trainLength, citizenPosition, zombieState);
//	printf("\n\n\n");
//
//	while (1) {
//		count++;
//
//		// �ù� �̵�
//		citMove = citizenMove(p);
//		citizenPosition -= citMove;
//
//		// ���� �̵�
//		zomMove = zombieMove(p, count);
//		zombieState -= zomMove;
//
//		// ���� ���� ���
//		trainState(trainLength, citizenPosition, zombieState);
//
//		// �ù�, ���� ����
//		citzomState(count, citizenPosition, zombieState, citMove, zomMove);
//
//		// �������� �˻�
//		// �ù��� ���񿡰� �������� �� (�ùλ��� == ��������� ��) [���� ����]
//		if (citizenPosition == zombieState) {
//			break;
//		}
//		// �ù��� ���� ��(1��)�� �������� �� [���� ����]
//		else if (citizenPosition == 1) {
//			break;
//		}
//		Sleep(4000);
//	}
//
//	// ��Ȳ�� �´� �޼��� ���
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
//	// �ƿ�Ʈ��
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