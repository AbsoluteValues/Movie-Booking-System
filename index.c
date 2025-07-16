#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

static enum States { BLANK, CHOOSE, FULL };
typedef struct Point {
    int x, y;
}Point;
typedef struct Seat
{
    States state;
    Point pnt;
}Seat;
typedef struct BookingSeats {
    Seat* seats;
    int sizeX, sizeY;
    // prevScene;
}BS;

bool PrintSeats(BS data) {
    // 매개변수에 이게 어느 지역의 어느 영화의 어느 관인지 구분할 수 있는 뭔가가 필요함.
    // 불러와서 좌석표 출력.
    int i, j;
    for (i = 0; i < data.sizeX; i += 1) {
        for (j = 0; j < data.sizeY; j += 1) {
            switch (data.seats[i][j].state)
            {
            case 0:
                printf("□");
                break;
            case 1:
                printf("▣");
                break;
            case 2:
                printf("■");
                break;
            default:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
}
bool BookingPeople() {
    Clr();
    // 좌석목록 띄운 채로 몇명 예약할지 결정.
    // 뒤로가기라면 시간/관 선택 창으로.
    // 인원이 0명 초과일때만 BookingSeats 로 진행.
}
bool BookingSeats(int num) {
    Clr();
    // 몇명 예약할지 받아오고 좌석목록 띄움.
    // 좌표 입력. 동시에 뒤로가기도 받아야 함.
    // 뒤로가기라면 BookingPeople 로.
    // 올바른지 체크
    // 올바르다면 현재 카운트 체크
    // 카운트 미만이면 색 바꾸고, 아니라면 이미 n명 선택했음 띄움.
    // 
    // 
}

static void Clr() { // 다들 모든 UI 및 씬 변경시마다 이거 붙이세요.
    system("cls");
}

struct theaterAddress {
	int addresss;	// 서울 1, 인천 2, 경기 3. 지역넘버를 매겨서 사용
};

struct movie {
	int num;
	char title[100];
	int rating;
	char genre[50];
};


void theaterAddress();



int main(void) {
	
    return 0;
}