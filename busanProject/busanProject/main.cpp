/*
	2-1 완료
	2-2 완료
	2-3 완료
	2-4 완료
	3-1 완료
	3-2 완료
	3-3	완료
	3-4	완료
*/

#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

// 파라미터
#define LEN_MIN     15  // 기차 길이
#define LEN_MAX     50
#define STM_MIN     0   // 마동석 체력
#define STM_MAX     5
#define PROB_MIN    10  // 확률
#define PROB_MAX    90
#define AGGRO_MIN   0   // 어그로 범위
#define AGGRO_MAX   5

// 마동석 이동 방향
#define MOVE_LEFT   1
#define MOVE_STAY   0

// 마동석 행동
#define ACTION_REST     0
#define ACTION_PROVOKE  1
#define ACTION_PULL     2

// 좀비의 공격 대상
#define ATK_NONE        0
#define ATK_CITIZEN     1
#define ATK_DONGSEOK    2

// 결과, 현재 차례
int result = 0; // 0 인간 승, 1 좀비 승
int turn = 0;

// 확률, 열차 길이
int p;
int trainLen;

// 시민, 좀비, 마동석 좌표
int citizen;
int zombie;
int madong;

// 시민, 좀비, 마동석 상태
int citizenNow = 0;
int zombieNow = 0;
int madongNow = 0;

// 시민, 마동석 어그로
int citizenAggro = 0;
int madongAggro = 0;

// 마동석 체력
int madongStamina;

// 빌런 좌표
int villain = -1;
// 빌런 상태
int villainNow = 0;
// 빌런 어그로
int villainAggro = 0;

// 스테이지 3, 4 시민들
int citizens[100] = { -1 };
int citizenCount = 0;

// 스테이지 4 좀비
int zombies[100] = { -1 };
int zombieCount = 0;

void printIntro();       // 인트로 출력
void inputTrainLen();    // 열차 길이 입력
void inputStamina();     // 마동석 체력 입력
void inputProb();        // 확률 입력

void printTrain(int stage = 1);       // 열차 출력
void printStatus();      // 상태 출력

// 스테이지
int stage1();
int stage2();
int stage3();
int stage4();

void movePhase(int stage);       // 이동 페이즈
void moveCitizen(int stage);     // 시민 이동
void moveZombie(int stage);      // 좀비 이동
void moveMadong(int stage);      // 마동석 이동

void workPhase(int stage);       // 행동 페이즈
void workCitizen(int stage);     // 시민 행동
void workZombie(int stage);      // 좀비 행동
void workMadong(int stage);      // 마동석 행동
void workVillain(int stage);		 // 빌런 행동

int main() {
	// 인트로 출력
	printIntro();

	// 화면 지우기
	system("cls");

	// 게임 전 수치 입력
	inputTrainLen();
	inputStamina();
	inputProb();

	printf("\n");

	srand(time(NULL));

	// 스테이지 시작
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

// 인트로 출력
void printIntro() {
	// 인트로 (3초 유지)
	printf(" _____ ______   ___   _____  _   _   _____  _____  ______  _   _  _____   ___   _   _ \n");
	printf("|_   _|| ___ \\ / _ \\ |_   _|| \\ | | |_   _||  _  | | ___ \\| | | |/  ___| / _ \\ | \\ | |\n");
	printf("  | |  | |_/ // /_\\ \\  | |  |  \\| |   | |  | | | | | |_/ /| | | |\\ `--. / /_\\ \\|  \\| |\n");
	printf("  | |  |    / |  _  |  | |  | . ` |   | |  | | | | | ___ \\| | | | `--. \\|  _  || . ` |\n");
	printf("  | |  | |\\ \\ | | | | _| |_ | |\\  |   | |  \\ \\_/ / | |_/ /| |_| |/\\__/ /| | | || |\\  |\n");
	printf("  \\_/  \\_| \\_|\\_| |_/ \\___/ \\_| \\_/   \\_/   \\___/  \\____/  \\___/ \\____/ \\_| |_/\\_| \\_/\n");
	//Sleep(3000);
}

// 열차 길이 입력
void inputTrainLen() {
	// 열차 길이 입력
	printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
	scanf_s("%d", &trainLen, sizeof(trainLen));
	// 열차 길이 올바르지 않을 경우 종료
	if (!(LEN_MIN <= trainLen && trainLen <= LEN_MAX)) {
		printf("wrong train length.");
		exit(0);
	}
}

// 마동석 체력 입력
void inputStamina() {
	// 마동석 체력 입력
	while (1) {
		printf("madongseok stamina(%d~%d)>> ", STM_MIN, STM_MAX);
		scanf_s("%d", &madongStamina, sizeof(madongStamina));
		// 올바르게 입력되면 종료
		if ((STM_MIN <= madongStamina && madongStamina <= STM_MAX)) {
			break;
		}
	}
}

// 확률 입력
void inputProb() {
	// 확률 입력
	while (1) {
		printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
		scanf_s("%d", &p, sizeof(p));
		// 올바르게 입력되면 종료
		if ((PROB_MIN <= p && p <= PROB_MAX)) {
			break;
		}
	}
}

// 열차 출력
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

// 상태 출력
void printStatus() {
	// 시민, 좀비 상태 출력
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

// 행동 출력
void printWork() {

	// 시민 행동
	switch (citizenNow) {
	case 0: printf("citizen does nothing.\n"); break;
	case 9: printf(""); break;
	}

	// 좀비 행동
	switch (zombieNow) {
	case ATK_NONE: printf("Zombie does nothing.\n"); break;
	case ATK_DONGSEOK: printf("Zombie attacked madongseok (aggro: %d vs. %d, madongseok stamina %d -> %d)", citizenAggro, madongAggro, madongStamina + 1, madongStamina);
	}

}

// 좀비 이동
void moveZombie(int stage) {

	printf("%d\n", madongNow);
	if (madongNow == ACTION_PULL) {
		zombieNow = 2;
		return;
	}

	// 이동 가능한 경우 어그로가 높은 쪽으로 이동
	if (citizenAggro > madongAggro) {
		// 시민 어그로가 더 높은 경우
		if (citizen == zombie - 1) {
			// 시민이 바로 왼쪽에 있는 경우
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
		// 마동석 어그로가 더 높은 경우
		if (madong == zombie + 1) {
			// 마동석이 바로 오른쪽에 있는 경우
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

// 스테이지 1
int stage1() {

	turn = 0;

	// 시민 좀비 마동석 초기화
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

		// 열차, 상태 출력
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

	// 시민 승
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// 좀비 승
	else if (result == 1) {
		printf("GAME OVER.. citizen is dead..\n\n");
		return 0;
	}
}

// 스테이지 2 - 빌런
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

	// 시민 좀비 마동석 초기화
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

		// 열차, 상태 출력
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

	// 시민 승
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// 좀비 승
	else if (result == 1) {
		printf("GAME OVER.. citizen is dead..\n\n");
		return 0;
	}
}

// 스테이지 3 - 시민들
int stage3() {
	
	result = -1;

	turn = 0;

	madongNow = 0;
	citizenNow = 0;
	zombieNow = 0;

	madongAggro = 0;
	citizenAggro = 0;

	// 시민 좀비 마동석 초기화
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

		// 열차, 상태 출력
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

	// 시민 승
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// 좀비 승
	else if (result == 1) {
		printf("GAME OVER.. all citizens are dead..\n\n");
		return 0;
	}
}

// 스테이지 4 - 변이
int stage4() {
	result = -1;

	turn = 0;

	madongNow = 0;
	citizenNow = 0;
	zombieNow = 0;

	madongAggro = 0;
	citizenAggro = 0;

	// 시민 좀비 마동석 초기화
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

		// 열차, 상태 출력
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

	// 시민 승
	if (result == 0) {
		printf("CITIZEN WIN!!!\n\n");
		return 1;
	}
	// 좀비 승
	else if (result == 1) {
		printf("GAME OVER.. all citizens are dead..\n\n");
		return 0;
	}
}

// 이동 페이즈
void movePhase(int stage) {
	moveCitizen(stage);
	moveZombie(stage);
	//moveMadong(stage);
}

// 시민 이동
void moveCitizen(int stage) {
	if ((float)rand() / RAND_MAX * 100 < p) {
		citizenAggro--;
		if (citizenAggro < 0) citizenAggro = AGGRO_MIN;
		citizenNow = 0;
		
		// 스테이지 2 빌런
		if (stage == 2) {
			villainAggro--;
			if (villainAggro < 0) villainAggro = AGGRO_MIN;
			villainNow = 0;
		}

	}
	else {
		// 스테이지 3, 4 시민들
		if (stage == 3 || stage == 4) {
			for (int i = 0; i < citizenCount; i++) {
				citizens[i]--;
			}
		}
		else citizen--;
		
		citizenAggro++;
		if (citizenAggro > AGGRO_MAX) citizenAggro = AGGRO_MAX;
		citizenNow = 1;

		// 스테이지 2 빌런
		if (stage == 2) {
			villain--;
			villainAggro++;
			if (villainAggro > AGGRO_MAX) villainAggro = AGGRO_MAX;
			villainNow = 1;
		}
	}
}

// 마동석 이동
void moveMadong(int stage) {
	int move;

	while (1) {
		// 좀비가 바로 왼쪽에 있는 경우
		if (madong - 1 == zombie) {
			printf("madongseok move(%d:stay)>> ", MOVE_STAY);
			scanf_s("%d", &move, sizeof(move));
			// 올바르게 입력되면 종료
			if (move == MOVE_STAY) {
				break;
			}
		}
		else {
			// 좀비가 바로 왼쪽에 없는 경우
			printf("madongseok move(%d:stay, %d:left)>> ", MOVE_STAY, MOVE_LEFT);
			scanf_s("%d", &move, sizeof(move));
			// 올바르게 입력되면 종료
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

// 행동 페이즈
void workPhase(int stage) {
	workCitizen(stage);
	workZombie(stage);
	if (stage == 2) {
		workVillain(stage);
	}
}

// 시민 행동
void workCitizen(int stage) {
	// 스테이지 3 시민들
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

// 빌런 행동
void workVillain(int stage) {
	if (villain == citizen + 1 && rand()%100 + 1 <= 30) {
		printf("villain interacts with citizen, swapping positions.\n");
		// 빌런과 시민 위치 교환
		int temp = citizen;
		citizen = villain;
		villain = temp;
	}
}

// 좀비 행동
void workZombie(int stage) {

	// 스테이지 3 시민들
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

	// 스테이지 4 변이
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


	// 좀비가 시민 바로 뒤에 있는 경우
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

// 마동석 행동
void workMadong(int stage) {
	int action;

	while (1) {
		if (zombie + 1 == madong) {
			printf("madongseok action(%d:rest, %d:provoke, %d:pull)>> ", ACTION_REST, ACTION_PROVOKE, ACTION_PULL);
			scanf_s("%d", &action, sizeof(action));
			// 올바르게 입력되면 종료
			if ((ACTION_REST <= action && action <= ACTION_PULL)) {
				break;
			}
		}
		else {
			printf("madongseok action(%d:rest, %d:provoke)>> ", ACTION_REST, ACTION_PROVOKE);
			scanf_s("%d", &action, sizeof(action));
			// 올바르게 입력되면 종료
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