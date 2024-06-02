/*
	2-1 �Ϸ�
	2-2 �Ϸ�
	2-3 �Ϸ�
	2-4 �Ϸ�
	3-1 �Ϸ�
	3-2 �Ϸ�
	3-3	�Ϸ�
	3-4	�Ϸ�
*/

#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

// �Ķ����
#define LEN_MIN     15  // ���� ����
#define LEN_MAX     50
#define STM_MIN     0   // ������ ü��
#define STM_MAX     5
#define PROB_MIN    10  // Ȯ��
#define PROB_MAX    90
#define AGGRO_MIN   0   // ��׷� ����
#define AGGRO_MAX   5

// ������ �̵� ����
#define MOVE_LEFT   1
#define MOVE_STAY   0

// ������ �ൿ
#define ACTION_REST     0
#define ACTION_PROVOKE  1
#define ACTION_PULL     2

// ������ ���� ���
#define ATK_NONE        0
#define ATK_CITIZEN     1
#define ATK_DONGSEOK    2

// ���, ���� ����
int result = 0; // 0 �ΰ� ��, 1 ���� ��
int turn = 0;

// Ȯ��, ���� ����
int p;
int trainLen;

// �ù�, ����, ������ ��ǥ
int citizen;
int zombie;
int madong;

// �ù�, ����, ������ ����
int citizenNow = 0;
int zombieNow = 0;
int madongNow = 0;

// �ù�, ������ ��׷�
int citizenAggro = 0;
int madongAggro = 0;

// ������ ü��
int madongStamina;

// ���� ��ǥ
int villain = -1;
// ���� ����
int villainNow = 0;
// ���� ��׷�
int villainAggro = 0;

// �������� 3, 4 �ùε�
int citizens[100] = { -1 };
int citizenCount = 0;

// �������� 4 ����
int zombies[100] = { -1 };
int zombieCount = 0;

void printIntro();       // ��Ʈ�� ���
void inputTrainLen();    // ���� ���� �Է�
void inputStamina();     // ������ ü�� �Է�
void inputProb();        // Ȯ�� �Է�

void printTrain(int stage = 1);       // ���� ���
void printStatus();      // ���� ���

// ��������
int stage1();
int stage2();
int stage3();
int stage4();

void movePhase(int stage);       // �̵� ������
void moveCitizen(int stage);     // �ù� �̵�
void moveZombie(int stage);      // ���� �̵�
void moveMadong(int stage);      // ������ �̵�

void workPhase(int stage);       // �ൿ ������
void workCitizen(int stage);     // �ù� �ൿ
void workZombie(int stage);      // ���� �ൿ
void workMadong(int stage);      // ������ �ൿ
void workVillain(int stage);		 // ���� �ൿ

int main() {
	// ��Ʈ�� ���
	printIntro();

	// ȭ�� �����
	system("cls");

	// ���� �� ��ġ �Է�
	inputTrainLen();
	inputStamina();
	inputProb();

	printf("\n");

	srand(time(NULL));

	// �������� ����
	printf("\nSTAGE 1\n");
	if (stage1() == 0) {
		exit(0);
	}
	printf("\nSTAGE 2\n");
	if (stage2() == 0) {
		exit(0);
	}
	printf("\nSTAGE 3\n");
	if (stage3() == 0) {
		exit(0);
	}
	printf("\nSTAGE 4\n");
	if (stage4() == 0) {
		exit(0);
	}

	return 0;
}

// ��Ʈ�� ���
void printIntro() {
	// ��Ʈ�� (3�� ����)
	printf(" _____ ______   ___   _____  _   _   _____  _____  ______  _   _  _____   ___   _   _ \n");
	printf("|_   _|| ___ \\ / _ \\ |_   _|| \\ | | |_   _||  _  | | ___ \\| | | |/  ___| / _ \\ | \\ | |\n");
	printf("  | |  | |_/ // /_\\ \\  | |  |  \\| |   | |  | | | | | |_/ /| | | |\\ `--. / /_\\ \\|  \\| |\n");
	printf("  | |  |    / |  _  |  | |  | . ` |   | |  | | | | | ___ \\| | | | `--. \\|  _  || . ` |\n");
	printf("  | |  | |\\ \\ | | | | _| |_ | |\\  |   | |  \\ \\_/ / | |_/ /| |_| |/\\__/ /| | | || |\\  |\n");
	printf("  \\_/  \\_| \\_|\\_| |_/ \\___/ \\_| \\_/   \\_/   \\___/  \\____/  \\___/ \\____/ \\_| |_/\\_| \\_/\n");
	//Sleep(3000);
}

// ���� ���� �Է�
void inputTrainLen() {
	// ���� ���� �Է�
	printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
	scanf_s("%d", &trainLen, sizeof(trainLen));
	// ���� ���� �ùٸ��� ���� ��� ����
	if (!(LEN_MIN <= trainLen && trainLen <= LEN_MAX)) {
		printf("wrong train length.");
		exit(0);
	}
}

// ������ ü�� �Է�
void inputStamina() {
	// ������ ü�� �Է�
	while (1) {
		printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
		scanf_s("%d", &madongStamina, sizeof(madongStamina));
		// �ùٸ��� �ԷµǸ� ����
		if ((STM_MIN <= madongStamina && madongStamina <= STM_MAX)) {
			break;
		}
	}
}

// Ȯ�� �Է�
void inputProb() {
	// Ȯ�� �Է�
	while (1) {
		printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &p, sizeof(p));
		// �ùٸ��� �ԷµǸ� ����
		if ((PROB_MIN <= p && p <= PROB_MAX)) {
			break;
		}
	}
}

// ���� ���
void printTrain(int stage) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < trainLen; j++) {

			int find = 0;
			for (int k = 0; stage >= 3 && k < citizenCount; k++) {
				if (i == 1 && j == citizens[k]) {
					printf("C");
					find = 1;
					break;
				}
			}
			if (find) continue;

			find = 0;
			for (int k = 0; stage == 4 && k < zombieCount; k++) {
				if (i == 1 && j == zombies[k]) {
					printf("Z");
					find = 1;
					break;
				}
			}
			if (find) continue;

			if (i == 0 || i == 2 || j == 0 || j == trainLen - 1)
				printf("#");
			else if (i == 1 && j == zombie)
				printf("Z");
			else if (i == 1 && j == citizen)
				printf("C");
			else if (i == 1 && j == madong)
				printf("M");
			else if (i == 1 && j == villain)
				printf("V");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

// ���� ���
void printStatus() {
	// �ù�, ���� ���� ���
	printf("citizen: ");
	switch (citizenNow) {
	case 0:
		printf("stay %d (aggro: %d -> %d)\n", citizen, citizenAggro + 1, citizenAggro);
		break;
	case 1:
		printf("%d -> %d (aggro: %d -> %d)\n", citizen + 1, citizen, citizenAggro - 1, citizenAggro);
		break;
	}

	printf("zombie: ");
	switch (zombieNow) {
	case 0:
		printf("stay %d\n", zombie);
		break;
	case 1:
		printf("%d -> %d\n", zombie + 1, zombie);
		break;
	case 2:
		printf("stay %d (cannot move)\n", zombie);
		break;
	}
	printf("\n");
}

// �ൿ ���
void printWork() {

	// �ù� �ൿ
	switch (citizenNow) {
	case 0: printf("citizen does nothing.\n"); break;
	case 9: printf(""); break;
	}

	// ���� �ൿ
	switch (zombieNow) {
	case ATK_NONE: printf("Zombie does nothing.\n"); break;
	case ATK_DONGSEOK: printf("Zombie attacked madongseok (aggro: %d vs. %d, madongseok stamina %d -> %d)", citizenAggro, madongAggro, madongStamina + 1, madongStamina);
	}

}

// ���� �̵�
void moveZombie(int stage) {

	printf("%d\n", madongNow);
	if (madongNow == ACTION_PULL) {
		zombieNow = 2;
		return;
	}

	// �̵� ������ ��� ��׷ΰ� ���� ������ �̵�
	if (citizenAggro > madongAggro) {
		// �ù� ��׷ΰ� �� ���� ���
		if (citizen == zombie - 1) {
			// �ù��� �ٷ� ���ʿ� �ִ� ���
			zombieNow = 0;
			return;
		}
		else {
			if (citizen < zombie) {
				zombie--;
				zombieNow = 1;
			}
		}
	}
	else {
		// ������ ��׷ΰ� �� ���� ���
		if (madong == zombie + 1) {
			// �������� �ٷ� �����ʿ� �ִ� ���
			zombieNow = 0;
			return;
		}
		else {
			if (madong > zombie) {
				zombie++;
				zombieNow = 1;
			}
		}
	}
}

// �������� 1
int stage1() {

	turn = 0;

	// �ù� ���� ������ �ʱ�ȭ
	madong = trainLen - 2;
	zombie = trainLen - 3;
	citizen = trainLen - 6;

	result = -1;

	printTrain(1);

	while (true) {

		//movePhase(1);

		if (turn % 2) {
			moveZombie(1);
		}
		else {
			zombieNow = 2;
		}

		moveCitizen(1);

		// ����, ���� ���
		printTrain(1);
		printStatus();
		printf("\n");

		moveMadong(1);

		printf("\n");
		printTrain(1);

		workPhase(1);
		printWork();

		printf("\n");
		workMadong(1);
		printf("\n");

		if (result != -1) {
			break;
		}

		turn++;
	}

	// �ù� ��
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// ���� ��
	else if (result == 1) {
		printf("GAME OVER.. citizen is dead..\n\n");
		return 0;
	}
}

// �������� 2 - ����
int stage2() {

	result = -1;

	turn = 0;

	madongNow = 0;
	citizenNow = 0;
	zombieNow = 0;
	villainNow = 0;

	madongAggro = 0;
	citizenAggro = 0;
	villainAggro = 0;

	// �ù� ���� ������ �ʱ�ȭ
	madong = trainLen - 2;
	zombie = trainLen - 3;
	villain = trainLen - 5;
	citizen = trainLen - 6;

	result = -1;

	printTrain(1);

	while (true) {

		//movePhase(1);

		if (turn % 2) {
			moveZombie(2);
		}
		else {
			zombieNow = 2;
		}

		moveCitizen(2);

		// ����, ���� ���
		printTrain(1);
		printStatus();
		printf("\n");

		moveMadong(2);

		printf("\n");
		printTrain(1);

		workPhase(2);
		printWork();

		printf("\n");
		workMadong(2);
		printf("\n");

		if (result != -1) {
			break;
		}

		turn++;
	}

	// �ù� ��
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// ���� ��
	else if (result == 1) {
		printf("GAME OVER.. citizen is dead..\n\n");
		return 0;
	}
}

// �������� 3 - �ùε�
int stage3() {
	
	result = -1;

	turn = 0;

	madongNow = 0;
	citizenNow = 0;
	zombieNow = 0;

	madongAggro = 0;
	citizenAggro = 0;

	// �ù� ���� ������ �ʱ�ȭ
	madong = trainLen - 2;
	zombie = trainLen - 3;
	citizenCount = trainLen / 4;

	citizens[0] = 3;

	for (int i = 1; i < citizenCount - 1; i++) {
		citizens[i] = citizens[i - 1] + (rand() % 2 + 1);
	}
	citizens[citizenCount - 1] = trainLen - 6;

	result = -1;

	printTrain(3);

	while (true) {

		//movePhase(1);

		if (turn % 2) {
			moveZombie(3);
		}
		else {
			zombieNow = 2;
		}

		moveCitizen(3);

		// ����, ���� ���
		printTrain(3);
		printStatus();
		printf("\n");

		moveMadong(3);

		printf("\n");
		printTrain(3);

		workPhase(3);
		printWork();

		printf("\n");
		workMadong(3);
		printf("\n");

		if (result != -1) {
			break;
		}

		turn++;
	}

	// �ù� ��
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// ���� ��
	else if (result == 1) {
		printf("GAME OVER.. all citizens are dead..\n\n");
		return 0;
	}
}

// �������� 4 - ����
int stage4() {
	result = -1;

	turn = 0;

	madongNow = 0;
	citizenNow = 0;
	zombieNow = 0;

	madongAggro = 0;
	citizenAggro = 0;

	// �ù� ���� ������ �ʱ�ȭ
	madong = trainLen - 2;

	zombieCount = 1;
	zombies[0] = trainLen - 3;

	citizenCount = trainLen / 4;

	citizens[0] = 3;

	for (int i = 1; i < citizenCount - 1; i++) {
		citizens[i] = citizens[i - 1] + (rand() % 2 + 1);
	}
	citizens[citizenCount - 1] = trainLen - 6;

	result = -1;

	printTrain(4);

	while (true) {

		//movePhase(1);

		if (turn % 2) {
			moveZombie(4);
		}
		else {
			zombieNow = 2;
		}

		moveCitizen(4);

		// ����, ���� ���
		printTrain(4);
		printStatus();
		printf("\n");

		moveMadong(4);

		printf("\n");
		printTrain(4);

		workPhase(4);
		printWork();

		printf("\n");
		workMadong(4);
		printf("\n");

		if (result != -1) {
			break;
		}

		turn++;
	}

	// �ù� ��
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// ���� ��
	else if (result == 1) {
		printf("GAME OVER.. all citizens are dead..\n\n");
		return 0;
	}
}

// �̵� ������
void movePhase(int stage) {
	moveCitizen(stage);
	moveZombie(stage);
	//moveMadong(stage);
}

// �ù� �̵�
void moveCitizen(int stage) {
	if ((float)rand() / RAND_MAX * 100 < p) {
		citizenAggro--;
		if (citizenAggro < 0) citizenAggro = AGGRO_MIN;
		citizenNow = 0;
		
		// �������� 2 ����
		if (stage == 2) {
			villainAggro--;
			if (villainAggro < 0) villainAggro = AGGRO_MIN;
			villainNow = 0;
		}

	}
	else {
		// �������� 3, 4 �ùε�
		if (stage == 3 || stage == 4) {
			for (int i = 0; i < citizenCount; i++) {
				citizens[i]--;
			}
		}
		else citizen--;
		
		citizenAggro++;
		if (citizenAggro > AGGRO_MAX) citizenAggro = AGGRO_MAX;
		citizenNow = 1;

		// �������� 2 ����
		if (stage == 2) {
			villain--;
			villainAggro++;
			if (villainAggro > AGGRO_MAX) villainAggro = AGGRO_MAX;
			villainNow = 1;
		}
	}
}

// ������ �̵�
void moveMadong(int stage) {
	int move;

	while (1) {
		// ���� �ٷ� ���ʿ� �ִ� ���
		if (madong - 1 == zombie) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &move, sizeof(move));
			// �ùٸ��� �ԷµǸ� ����
			if (move == MOVE_STAY) {
				break;
			}
		}
		else {
			// ���� �ٷ� ���ʿ� ���� ���
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT);
			scanf_s("%d", &move, sizeof(move));
			// �ùٸ��� �ԷµǸ� ����
			if (MOVE_STAY <= move && move <= MOVE_LEFT) {
				break;
			}
		}
	}

	if (move == MOVE_STAY) {
		madongAggro--;
		if (madongAggro < 0) madongAggro = 0;
	}
	else {
		madong--;
		madongAggro++;
	}
}

// �ൿ ������
void workPhase(int stage) {
	workCitizen(stage);
	workZombie(stage);
	if (stage == 2) {
		workVillain(stage);
	}
}

// �ù� �ൿ
void workCitizen(int stage) {
	// �������� 3 �ùε�
	if (stage == 3) {
		if (citizens[0] == 1) {
			citizenNow = 0;
			result = 0;
		}
		else {
			citizenNow = 0;
		}
	}
	if (citizen == 1) {
		citizenNow = 9;
		result = 0;
	}
	else {
		citizenNow = 0;
	}
}

// ���� �ൿ
void workVillain(int stage) {
	if (villain == citizen + 1 && rand()%100 + 1 <= 30) {
		printf("villain interacts with citizen, swapping positions.\n");
		// ������ �ù� ��ġ ��ȯ
		int temp = citizen;
		citizen = villain;
		villain = temp;
	}
}

// ���� �ൿ
void workZombie(int stage) {

	// �������� 3 �ùε�
	if (stage == 3) {
		if (citizens[citizenCount-1] + 1 == zombie) {
			if (madongAggro > citizenAggro && madong - 1 == zombie) {
				zombieNow = ATK_DONGSEOK;
				madongStamina--;
				if (madongStamina < STM_MIN) madongStamina = STM_MIN;
			}
			else {
				printf("Citizen %d is dead...", citizenCount--);
				if (citizenCount == 0) zombieNow = ATK_CITIZEN;
				result = 1;
			}
		}
		else if (madong - 1 == zombie) {
			if (madongAggro < citizenAggro && citizenAggro + 1 == zombie) {
				printf("Citizen %d is dead...", citizenCount--);
				if (citizenCount == 0) zombieNow = ATK_CITIZEN;
				result = 1;
			}
			else {
				zombieNow = ATK_DONGSEOK;
				madongStamina--;
				if (madongStamina < STM_MIN) madongStamina = STM_MIN;
			}
		}
		else {
			zombieNow = ATK_NONE;
		}
		return;
	}

	// �������� 4 ����
	if (stage == 4) {
		if (citizens[citizenCount - 1] + 1 == zombies[zombieCount - 1]) {
			if (madongAggro > citizenAggro && madong - 1 == zombies[zombieCount - 1]) {
				zombieNow = ATK_DONGSEOK;
				madongStamina--;
				if (madongStamina < STM_MIN) madongStamina = STM_MIN;
			}
			else {
				printf("Citizen %d return into zombie...", citizenCount--);
				zombies[zombieCount++] = citizens[citizenCount - 1];
				if (citizenCount == 0) zombieNow = ATK_CITIZEN;
				result = 1;
			}
		}
		else if (madong - 1 == zombies[zombieCount - 1]) {
			if (madongAggro < citizenAggro && citizenAggro + 1 == zombies[zombieCount - 1]) {
				printf("Citizen %d return into zombie...", citizenCount--);
				zombies[zombieCount++] = citizens[citizenCount - 1];
				if (citizenCount == 0) zombieNow = ATK_CITIZEN;
				result = 1;
			}
			else {
				zombieNow = ATK_DONGSEOK;
				madongStamina--;
				if (madongStamina < STM_MIN) madongStamina = STM_MIN;
			}
		}
		else {
			zombieNow = ATK_NONE;
		}
		return;
	}


	// ���� �ù� �ٷ� �ڿ� �ִ� ���
	if (citizen + 1 == zombie) {
		if (madongAggro > citizenAggro && madong - 1 == zombie) {
			zombieNow = ATK_DONGSEOK;
			madongStamina--;
			if (madongStamina < STM_MIN) madongStamina = STM_MIN;
		}
		else {
			zombieNow = ATK_CITIZEN;
			result = 1;
		}
	}
	else if (stage == 2 && villain + 1 == zombie) {
		villain = -1;
	}
	else if (madong - 1 == zombie) {
		if (madongAggro < citizenAggro && citizenAggro + 1 == zombie) {
			zombieNow = ATK_CITIZEN;
			result = 1;
		}
		else {
			zombieNow = ATK_DONGSEOK;
			madongStamina--;
			if (madongStamina < STM_MIN) madongStamina = STM_MIN;
		}
	}
	else {
		zombieNow = ATK_NONE;
	}

}

// ������ �ൿ
void workMadong(int stage) {
	int action;

	while (1) {
		if (zombie + 1 == madong) {
			printf("madongseok action(%d:rest, %d:provoke, %d:pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
			scanf_s("%d", &action, sizeof(action));
			// �ùٸ��� �ԷµǸ� ����
			if ((ACTION_REST <= action && action <= ACTION_PULL)) {
				break;
			}
		}
		else {
			printf("madongseok action(%d:rest, %d:provoke)>> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &action, sizeof(action));
			// �ùٸ��� �ԷµǸ� ����
			if ((ACTION_REST <= action && action <= ACTION_PROVOKE)) {
				break;
			}
		}
	}

	if (action == ACTION_REST) {
		madongNow = ACTION_REST;
		madongAggro--;
		if (madongAggro < 0) madongAggro = 0;
		madongStamina++;
		if (madongStamina > STM_MAX) madongStamina = STM_MAX;

		printf("madongseok rests..");
		printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madong, madongAggro + 1, madongAggro, madongStamina - 1, madongStamina);
	}
	else if (action == ACTION_PROVOKE) {
		madongNow = ACTION_PROVOKE;
		printf("madongseok provoke..");
		printf("madongseok : %d (aggro : %d -> %d, stamina : %d)\n", madong, madongAggro, AGGRO_MAX, madongStamina);
		madongAggro = AGGRO_MAX;
	}
	else if (action == ACTION_PULL) {
		if ((float)rand() / RAND_MAX * 100 < p && madongStamina > 0) {
			madongNow = ACTION_PULL;
			int prevAggro = madongAggro;
			madongAggro += 2;
			if (madongAggro > AGGRO_MAX) madongAggro = AGGRO_MAX;
			madongStamina--;
			if (madongStamina < 0) madongStamina = 0;
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madong, prevAggro, madongAggro, madongStamina - 1, madongStamina);
		}
		else {
			madongNow = 0;
			int prevAggro = madongAggro;
			madongAggro += 2;
			if (madongAggro > AGGRO_MAX) madongAggro = AGGRO_MAX;
			madongStamina--;
			if (madongStamina < 0) madongStamina = 0;
			printf("madongseok pulled zombie... Next turn, it can't move\n");
			printf("madongseok : %d (aggro : %d -> %d, stamina : %d -> %d)\n", madong, prevAggro, madongAggro, madongStamina - 1, madongStamina);

		}
	}
}