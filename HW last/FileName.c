#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define LEN_MIN 15 // 기차 길이
#define LEN_MAX 50
#define STM_MIN 0 // M 체력
#define STM_MAX 5
#define PROB_MIN 10 // 확률
#define PROB_MAX 90
#define AGGRO_MIN 1 // 어그로 범위
#define AGGRO_MAX 5 
#define MOVE_LEFT 1 // 이동 방향
#define MOVE_STAY 0 
#define ACTION_REST 0  // 행동
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int LEN, PROB, STM;
int cPosition, vPosition, zPosition, mPosition;
int cAggro = 1,vAggro = 1, mAggro = 1;
int zTurnCount = 0;
int zStunned = 0;
int mAction;  // M의 행동을 저장할 변수
int zAction;
int prevCPosition;
int prevVPosition;
int prevZPosition; // 이전 좀비의 위치를 저장하는 변수
int change;
int stage = 1;
int changec = 1;

// 기차 상태 출력 함수
void printPattern(int length) {
    for (int i = 0; i < length; i++) {
        printf("#");
    }
    printf("\n");
}

// 기차와 캐릭터 출력 함수
void printPatternWithCharacters(int length, int cPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}

// 입력 버퍼를 비우는 함수
void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void MAXMIN() {
    // M의 스테미나가 STM_MAX를 초과하지 않도록 보정
    if (STM > STM_MAX) STM = STM_MAX;
    if (STM < STM_MIN) STM = STM_MIN;
    // 어그로 수치가 MIN과 MAX를 벗어나지 않도록 보정
    if (mAggro < AGGRO_MIN) mAggro = AGGRO_MIN;
    if (mAggro > AGGRO_MAX) mAggro = AGGRO_MAX;
    if (cAggro < AGGRO_MIN) cAggro = AGGRO_MIN;
    if (cAggro > AGGRO_MAX) cAggro = AGGRO_MAX;
    if (vAggro < AGGRO_MIN) vAggro = AGGRO_MIN;
    if (vAggro > AGGRO_MAX) vAggro = AGGRO_MAX;
}


// 게임 초기화 함수
void initializeGame() {
    MAXMIN();


    // 입력값이 범위를 벗어날 때까지 반복하여 입력 받음 
    while (1) {
        printf("train length(%d~%d)>> ", LEN_MIN, LEN_MAX);
        if (scanf_s("%d", &LEN) != 1 || LEN < LEN_MIN || LEN > LEN_MAX) {

         

            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    // 나머지 입력값도 동일하게 처리
    while (1) {
        printf("M stamina(%d~%d)>> ", STM_MIN, STM_MAX);
        if (scanf_s("%d", &STM) != 1 || STM < STM_MIN || STM > STM_MAX) {

            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    while (1) {
        printf("percentile probability 'p'(%d~%d)>> ", PROB_MIN, PROB_MAX);
        if (scanf_s("%d", &PROB) != 1 || PROB < PROB_MIN || PROB > PROB_MAX) {

            clearInputBuffer();
        }
        else {
            clearInputBuffer();
            break;
        }
    }

    cPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    cAggro = 1; // 시민의 aggro 기본값
    mAggro = 1; // M의 aggro 기본값

    printf("\n\n");
    printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
    printf("\n");


    //이전 애들
    prevCPosition = cPosition;
    prevZPosition = zPosition; // 초기화 후 이전 좀비 위치 설정

}



// 시민 이동 함수 + 2스테이지 빌런
void moveCitizen() {

    if (stage == 1) {
        int moveProb = rand() % 100;
        if (moveProb < (100 - PROB)) {
            if (cPosition > 0) {
                cPosition--; // 왼쪽으로 1칸 이동  
   
            }
        }


    }
    else if (stage == 2) {
        if (vPosition - cPosition >= 0) {
            int moveProb = rand() % 100;
            if (moveProb < (100 - PROB)) {
                if (cPosition > 0) {
                    cPosition--; // 왼쪽으로 1칸 이동  
                    if (stage == 2) {
                        vPosition--;
                    }
                }
            }
        }
        else{
            // 안함
        }
    }
}

// 좀비 이동 함수
void moveZombie() {
    if (zStunned == 1) {
        return;  // M이 pull을 성공했을 때 기절 상태이므로 움직이지 않음
    }

    if (zTurnCount % 2 != 0) {
        return;  // 짝수 턴이 아니면 움직이지 않음
    }

    // 기존 로직 추가
    int moveProb = rand() % 100;
    if (moveProb >= PROB) {
        return;  // 확률에 의해 움직이지 않음
    }

    // 이동할 수 있는 경우, 좀비의 이동 로직 추가
    if (cAggro >= mAggro) {
        if (zPosition > 0 && zPosition - 1 != cPosition && zPosition - 1 != mPosition) {
            zPosition--;  // 시민이 더 높은 aggro를 가질 때 왼쪽으로 이동
        }
    }
    else {
        if (zPosition < LEN - 1 && zPosition + 1 != cPosition && zPosition + 1 != mPosition) {
            zPosition++;  // M이 더 높은 aggro를 가지고 오른쪽으로 이동
        }
    }
}

// M 이동 함수
void moveM() {

    // 좀비와 붙어있을 경우
    if (abs(mPosition - zPosition) == 1) {
        // 이동하지 않도록 선택지 0번만 제공
        printf("M's move (0: stay)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || mAction != MOVE_STAY) {

                clearInputBuffer();
            }
            else {

                clearInputBuffer();
                break;
            }
        }
    }
    else {
        // 좀비와 붙어있지 않을 경우에만 이동할 방향을 선택하도록 요청
        printf("M's move (0: stay, 1: move left)>> ");
        while (1) {
            if (scanf_s("%d", &mAction) != 1 || (mAction != MOVE_STAY && mAction != MOVE_LEFT)) {

                clearInputBuffer();
            }
            else {

                clearInputBuffer();
                break;
            }
        }

        // 선택에 따라 위치 변경
        if (mAction == MOVE_LEFT && mPosition > 0) {
            mPosition--;
        }

    }



}

// 시민 행동 처리 함수
void performCAction() {
    //not thing
}

// 좀비 행동 처리 함수
void performZAction() {
    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            zAction = 1;  // 좀비가 시민을 공격
        }
        else {

            zAction = 2;  // 좀비가 M을 공격
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        zAction = 1;  // 좀비가 시민을 공격
    }
    else if (abs(zPosition - mPosition) == 1) {
        ;
        zAction = 2;  // 좀비가 M을 공격
    }
    else if (abs(zPosition - vPosition) == 1) {
        zAction = 3;  // 좀비가 빌런을 공격
    }
    else {
        zAction = 0;  // 좀비가 아무것도 하지 않음
    }
}


// M 행동 처리 함수
void performMAction() {
    int STMa = STM;
    int mAggrom = mAggro;

    // 붙어있는 경우
    if (abs(mPosition - zPosition) == 1) {
        printf("M's action(0: rest, 1: provoke, 2: pull)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice < 0 || choice > 2)) {

                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }
    else {
        // 붙어있지 않은 경우에는 이동이나 provoke 선택할 수 있도록
        printf("M's action(0: rest, 1: provoke)>> ");
        while (1) {
            int choice;
            if (scanf_s("%d", &choice) != 1 || (choice != ACTION_REST && choice != ACTION_PROVOKE)) {

                clearInputBuffer();
            }
            else {
                mAction = choice;
                clearInputBuffer();
                break;
            }
        }
    }
    printf("\n");
    // 선택에 따라 aggro 수치 조정
    if (mAction == ACTION_REST) {
        printf("\nM's rests...\n");
        STM++; // 휴식 시 stamina 증가
        mAggro--; // 휴식 시 aggro 수치 감소
        MAXMIN();
        printf("M's : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);

    }
    else if (mAction == ACTION_PROVOKE) {
        printf("\nM's provoked zombie...\n");
        MAXMIN();
        int mAggrom = mAggro;
        printf("M's : %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro = AGGRO_MAX, STM);
        mAggro = AGGRO_MAX;
    }
    else if (mAction == ACTION_PULL) {
        int mAggrom = mAggro;

        mAggro += 2; // PULL 시 어그로 2 증가
        STM--; // PULL 시 스태미너 1 감소
        MAXMIN();
        int pullSuccess = rand() % (100 - PROB); // PULL 성공 확률 계산
        if (pullSuccess) {
            zStunned = 1; // 좀비가 기절함을 표시
            printf("M's pulled zombie... Next turn, it can't move\n");
            printf("M's : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }
        else {
            zStunned = 0;
            printf("M's failed to pull zombie\n");
            printf("M's : %d (aggro: %d -> %d, stamina: %d -> %d)\n", mPosition, mAggrom, mAggro, STMa, STM);
        }

    }
    printf("\n");
}


// 좀비 행동 결과 출력 함수
void printZActionResult() {

    int STMa = STM;

    if (abs(zPosition - cPosition) == 1 && abs(zPosition - mPosition) == 1) {
        if (cAggro >= mAggro) {
            printf("Zombie attacks citizen!\nGAME OVER!\n");
            exit(0); // 게임 종료
        }

        else {
            STM--;
            MAXMIN();
            printf("Zombie attacks M! (stamina: %d -> %d)\n", STMa, STM);
            if (STM == 1) {
                printf("GAME OEVER! m dead...");
                exit(0);
            }
        }
    }
    else if (abs(zPosition - cPosition) == 1) {
        printf("Zombie attacks citizen!\nGAME OVER!\n");
        exit(0); // 게임 종료
    }
    else if (abs(zPosition - mPosition) == 1) {
        STM--;
        MAXMIN();
        printf("Zombie attacks M! (stamina: %d -> %d)\n", STMa, STM);
        if (STM == 1) {
            printf("GAME OEVER! m dead...");
            exit(0);
        }


    }

    if ((zAction) == 0) {
        printf("zombie attcked nobody.\n");
    }
    if ((zAction) == 3) {
        printf("zombie attcked villain.\n");
    }


}



// 이전 위치와 현재 위치를 비교하여 달라졌는지를 확인하는 함수
int hasMoved(int prevPosition, int currentPosition) {
    return prevPosition != currentPosition;
}



// 시민 이동 결과 출력 함수
void printCitizenMoveResult(int prevPosition) {

    int cAggroc = cAggro;
    int vAggroc = vAggro;

    if (prevPosition != cPosition) {

        cAggro++;
        vAggro++;
        MAXMIN();
        printf("Citizen: %d -> %d (aggro: %d -> %d)\n", prevPosition, cPosition, cAggroc, cAggro);
        if (stage == 2) {
            printf("villain: %d -> %d (aggro: %d -> %d)\n", vPosition +1, vPosition, vAggroc, vAggro);
        }
    }
    else {

        cAggro--;
        vAggro--;
        MAXMIN();
        printf("Citizen: stay %d (aggro: %d -> %d)\n", cPosition, cAggroc, cAggro);
        if (stage == 2) {
            printf("villain: stay %d (aggro: %d -> %d)\n",  vPosition, vAggroc, vAggro);
        }
    }
}



// 좀비 이동 결과 출력 함수
void printZombieMoveResult(int prevPosition) {
    MAXMIN();
    if (zStunned == 1 && mAction == ACTION_PULL) {
        printf("Zombie: stay %d (cannot move PULL)\n", zPosition);
    }
    else if (zTurnCount % 2 != 0) {
        printf("Zombie: stay %d (cannot move)\n", zPosition);
    }
    else {
        if (prevPosition != zPosition) {
            printf("Zombie: %d -> %d\n", prevPosition, zPosition);
        }
        else {
            printf("Zombie: stay %d\n", zPosition);
        }
    }

    //이전 애들
    prevCPosition = cPosition;
    prevZPosition = zPosition; // 초기화 후 이전 좀비 위치 설정

    zStunned = 0;
}



// M 이동 결과 출력 함수
void printMMoveResult() {

    int mAggrom = mAggro;

    if (mAction == MOVE_LEFT && mPosition > 0) {
        mAggro++;
        MAXMIN();
        printf("M's: %d -> %d (aggro: %d -> %d, stamina: %d)\n", mPosition + 1, mPosition, mAggrom, mAggro, STM);

    }
    else {
        mAggro--;
        MAXMIN();
        printf("M's: stay %d (aggro: %d -> %d, stamina: %d)\n", mPosition, mAggrom, mAggro, STM);

    }
}



// 기차와 캐릭터 출력 함수 2스테이지 
void printPatternWithCharactersSECOND(int length, int cPos, int vPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == vPos) {
            printf("V");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}

// 기차와 캐릭터 출력 함수 2스테이지 체인지
void printPatternWithCharactersSECONDCHANGE(int length, int vPos, int cPos, int zPos, int mPos) {
    printPattern(length);
    for (int i = 0; i < length; i++) {
        if (i == 0 || i == length - 1) {
            printf("#");
        }
        else if (i == vPos) {
            printf("V");
        }
        else if (i == cPos) {
            printf("C");
        }
        else if (i == zPos) {
            printf("Z");
        }
        else if (i == mPos) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("\n");
    printPattern(length);
    printf("\n");
}

// 게임 초기화 함수 2스테이지
void initializeGameSECOND() {
    MAXMIN();
  
    cPosition = LEN - 7;
    vPosition = LEN - 6;
    zPosition = LEN - 3;
    mPosition = LEN - 2;

    cAggro = 1; // 시민의 aggro 기본값
    vAggro = 1;
    mAggro = 1; // M의 aggro 기본값

    printf("\n\n");
    if (changec == 1) {
        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
    }else if (changec == 2) {
        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
    }
    printf("\n");


    //이전 애들
    prevCPosition = cPosition;
    prevVPosition = vPosition;
    prevZPosition = zPosition; // 초기화 후 이전 좀비 위치 설정

}



// 시민 행동 결과 출력 함수
void printCActionResult() {
    if (cPosition == 1) {
        if (stage == 1) {
            printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
            printf("YOU WIN! ···\n");

        } 
        else if (stage == 2) {
            if (changec == 1) {
                printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                printf("YOU WIN! ···\n");

            }
            else if (changec == 2) {
                printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                printf("YOU WIN! ···\n");
            }
        }

    }else {
        if (stage == 1) {
            printf("Citizen does nothing.\n");

        }else if (stage == 2) {

            if (changec == 1) {
                
                    int changeProb = rand() % 100;
                    if (changeProb < (50)) {
                        changec = 2;
                        change = cPosition;
                        cPosition = vPosition;
                        vPosition = change;
                        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("Villain changed place a citizen!\n");
                       
                    }
                    else {
                        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("villain does nothing.\n");
                    }
            }
            else if (changec == 2) {
                if (stage == 2) {
                   
                    int changeProb = rand() % 100;
                    if (changeProb < (50)) {
                        changec = 1;
                        change = vPosition;
                        vPosition = cPosition;
                        cPosition = change;
                        printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("Villain changed place a citizen!\n");
                       
                    }
                    else {
                        printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
                        printf("Citizen does nothing.\n");
                        printf("villain does nothing.\n");
                    }

                }
            }
        }
    }
}



// stage1: 첫 번째 스테이지를 처리하는 함수
void stage1() {
    initializeGame(); // 게임 초기화
    
    while (1) {
        //초기 화면 <이동>페이즈 
        moveCitizen();
        moveZombie();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        //<이동>페이즈 결과
        moveM();
        printPatternWithCharacters(LEN, cPosition, zPosition, mPosition);
        printMMoveResult();
        printf("\n");

        //<행동>페이즈 

        performCAction();
        performZAction();
        //<행동>페이즈 결과
        printCActionResult();
        printZActionResult();

        // 게임 종료 조건 확인
        if (cPosition == 1) {
            printf("Stage 1 Clear!\n");
            stage++;
            break; // 다음 스테이지로 이동
        }


        performMAction();

        printf("\n");


        // zTurnCount 증가
        zTurnCount++;
    }
}

// stage2: 두 번째 스테이지를 처리하는 함수
void stage2() {
    initializeGameSECOND(); // 게임 초기화

    while (1) {
        //초기 화면 <이동>페이즈 
        moveCitizen();
        moveZombie();
        if (changec == 1) {
            printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
        }
        else if (changec == 2) {
            printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
        }
        printCitizenMoveResult(prevCPosition);
        printZombieMoveResult(prevZPosition);

        printf("\n");

        //<이동>페이즈 결과
        moveM();
        if (changec == 1) {
            printPatternWithCharactersSECOND(LEN, cPosition, vPosition, zPosition, mPosition);
        }
        else if (changec == 2) {
            printPatternWithCharactersSECONDCHANGE(LEN, vPosition, cPosition, zPosition, mPosition);
        }
        printMMoveResult();
        printf("\n");

        //<행동>페이즈 

        performCAction();
        performZAction();
        //<행동>페이즈 결과
        printCActionResult();
        printZActionResult();

        // 게임 종료 조건 확인
        if (cPosition == 1) {
            stage++;
            printf("Stage 2 Clear!\n");
            break; // 다음 스테이지로 이동
        }


        performMAction();

        printf("\n");


        // zTurnCount 증가
        zTurnCount++;
    }
}


// stage3: 세 번째 스테이지를 처리하는 함수
void stage3() {
    // 세 번째 스테이지의 처리를 위한 코드 작성
}

// 메인 함수
int main(void) {
    srand((unsigned int)time(NULL));


    stage1(); // 첫 번째 스테이지 실행
    stage2(); // 두 번째 스테이지 실행
    // stage3(); // 세 번째 스테이지 실행
    //changec = 1;
    return 0;
}