#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <conio.h>
#include <stdbool.h>

#define MAX_MOVIES 6
#define CinemaMaxSizeX 10
#define CinemaMaxSizeY 21

enum States { BLANK, CHOOSE, FULL };
typedef struct PointXY {  // 필요한가?
    int x, y;
}Point;
typedef struct SeatData {   // 각 좌석의 정보...
    enum States state;
    Point point;
}Seat;
typedef struct CinemaRoom {
    Seat seats[CinemaMaxSizeX][CinemaMaxSizeY]; // 좌석 2차원 배열
    int seatCnt;    // 남은 좌석 수
    int sizeX, sizeY;   // 영화관 크기
    // prevScene;
}Cinema;
void BookingSeats(Cinema* data, int n);
bool BookingPeople(Cinema* data);

Cinema* preset;
bool ResetCinema(Cinema* preset, int x, int y) {
    int i, j;
    if (x > CinemaMaxSizeX) x = CinemaMaxSizeX;
    if (y > CinemaMaxSizeY) y = CinemaMaxSizeY;
    for(i = 0; i < x; i++){
        for (j = 0; j < y; j++) {
            preset->seats[i][j].state = BLANK;
            preset->seats[i][j].point.x = i;
            preset->seats[i][j].point.y = j;
        }
    }
    preset->sizeX = x;
    preset->sizeY = y;
    preset->seatCnt = x * y;

    return true;
}

bool ClearChooseSeats(Cinema* data) { // 현재는 그냥 밀어버림. 계정별로 구분할거라면... 어쩌구저쩌구
    int i, j;
    for (i = 0; i < data->sizeX; i += 1) {
        for (j = 0; j < data->sizeY; j += 1) {
            if (data->seats[i][j].state == CHOOSE)
                data->seats[i][j].state = BLANK;
        }
    }
    return true;
}

bool PrintSeats(Cinema* data) {
    // 매개변수에 이게 어느 지역의 어느 영화의 어느 관인지 구분할 수 있는 뭔가가 필요함.
    // 불러와서 좌석표 출력.
    //Clr();
    int i, j;
    printf("==================================================\n\n");
    for (i = 0; i < data->sizeX; i += 1) {
        for (j = 0; j < data->sizeY; j += 1) {
            switch (data->seats[i][j].state)
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
    printf("\n==================================================\n");
    return true;
}

bool BookingPeople(Cinema* data) {
    // 좌석목록 띄운 채로 몇명 예약할지 결정.
    // 뒤로가기라면 시간/관 선택 창으로.
    // 인원이 0명 초과일때만 BookingSeats 로 진행.
    int n;
A:
    //Clr();
    PrintSeats(data);
    printf("\n-1 입력시 이전 메뉴로\n- - - - - - - - - - - - - - -\n");
    printf("예약할 인원 : ");
    scanf("%d", &n);
    if (n == -1) return false;
    if (n < 1) {
        printf("1명 이상 선택해주세요.");
        goto A;
    }
    else if (n > data->seatCnt) {
        printf("예매 가능한 좌석이 부족합니다.\n남은 좌석은 %d석입니다.", data->seatCnt);
        goto A;
    }
    else {
        BookingSeats(data, n);
    }

    return true;
}

void BookingSeats(Cinema* data, int n) {
    char point[20];
    Point p;
    int cnt = 0;
    int i;
    // 몇명 예약할지 받아오고 좌석목록 띄움.
    // 좌표 입력. 동시에 뒤로가기도 받아야 함.
    // 뒤로가기라면 BookingPeople 로.
    // 올바른지 체크
    // 올바르다면 현재 카운트 체크
    // 카운트 미만이면 색 바꾸고, 아니라면 이미 n명 선택했음 띄움.
A:
    //Clr();
    PrintSeats(data);
    printf("\n-1 입력시 이전 메뉴로\n- - - - - - - - - - - - - - -\n");
    printf("예약할 좌석 좌표를 입력하세요(ex:B3) : ");
    // 이렇게 받을지, 혹은 한글자씩 받으면서 계속 초기화할지...
    // 고민은 필요해 보인다.
    fgets(point, sizeof(char)*2, stdin);
    p.x = (int)point[0];
    p.y = atoi(&point[1]);//a97 A65 z122 Z90
    if (atoi(&point[0]) == -1) {
        if (cnt > 0) {
            ClearChooseSeats(data);
        }
        return;
    }
        
    if (p.x >= 97 && p.x <= 97 + data->sizeY && p.x <= 122 && p.y > 0 && p.y <= data->sizeY) {
        if (data->seats[p.x - 97][p.y].state == BLANK) {
            data->seats[p.x - 97][p.y].state = CHOOSE;
            cnt += 1;
        }
        else if (data->seats[p.x - 97][p.y].state == CHOOSE) {
            data->seats[p.x - 97][p.y].state = BLANK;
            cnt -= 1;
        }
        else printf("이미 예매된 좌석입니다.\n");
    }
    else if (p.x >= 65 && p.x <= 65 + data->sizeY && p.x <= 90 && p.y > 0 && p.y <= data->sizeY) {
        if (data->seats[p.x - 65][p.y].state == BLANK) {
            data->seats[p.x - 65][p.y].state = CHOOSE;
            cnt += 1;
        }
        else if (data->seats[p.x - 65][p.y].state == CHOOSE) {
            data->seats[p.x - 65][p.y].state = BLANK;
            cnt -= 1;
        }
        else printf("이미 예매된 좌석입니다.\n");
    }
    else printf("선택할 수 없는 좌석입니다.\n");
    if (cnt < n) goto A;
B:
    //Clr();
    PrintSeats(data);
    printf("1. 취소 / 2. 결제 페이지로 : \n");
    scanf("%d", &i);
    switch (i)
    {
    case 1:
        goto A;
        break;
    case 2:
        data->seatCnt -= n;
        // 결제 페이지....
        // 결재 따로 안만들거면, 여기서 저장하고 리턴.
        break;
    default:
        goto B; // 재입력받음.
        break;
    }

    return;
}
/*
static void Clr() { // 다들 모든 UI 및 씬 변경시마다 이거 붙이세요.
    system("cls");  // 콘솔 창 지우는 기능.
}
*/
struct theaterAddress {
	int addresss;	// 서울 1, 인천 2, 경기 3. 지역넘버를 매겨서 사용
};

typedef struct movie {
	int num;    //영화번호
	char title[100];    //제목
    char rating[50];    //등급
	char genre[50];     //장르
    int runtime;    //상영시간
} Movie;

void playMovies(Movie movies[]) {
    int cnt;
    printf("\n==== 상영 중인 영화 목록 ====\n");
    for (int i = 0; i < cnt; i++) {
        printf("%d. %s ( %s )\n", movies[i].num, movies[i].title, movies[i].rating);
        printf(" %s / %d분 \n", movies[i].genre, movies[i].runtime);
    }
}

int choiceMovie(Movie movies[]) {
    int cnt;
    int choice;
    printf("\n영화 번호를 선택하시오 : ");
    scanf("%d", &choice);

    for (int i = 0; i < cnt; i++) {
        if (movies[i].num == choice) {
            printf("선택한 영화 : %s\n", movies[i].title);
            return i;
        }
    }
    printf("잘못된 번호입니다.\n");
    return -1;
}

const char* theaterAddress() {
	struct theaterAddress address;
	while(1) {
        printf("\n============ 영화관 ============\n");
        printf("== 영화관 지역을 선택해주세요 ==\n");
        printf("1. 서울\n");
        printf("2. 인천\n");
        printf("3. 경기\n");
        printf("0. 종료\n");
        printf(" -> ");
        scanf("%d", &address.addresss);	
        printf("================================\n");

        switch (address.addresss) {
            case 1:
                return "서울";
            case 2:
                return "인천";
            case 3:
                return "경기";
            case 0:
                return "종료"; 
                break; 
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        }  
    }

}




int main(void) {

    Movie movies[MAX_MOVIES] = {
        {1, "F1 더 무비", "12세 이상 관람가", "액션, 드라마", 155},
        {2, "킹 오브 킹스", "전체 이용가", "애니메이션", 101},
        {3, "슈퍼맨", "12세 이상 관람가", "액션", 129},
        {4, "쥬라기 월드: 새로운 시작", "12세 이상 관람가", "액션", 129},
        {5, "노이즈", "15세 이상 관람가", "모험, 액션, 공포, 스릴러", 133},
        {6, "명탐정 코난: 척안의 잔상", "12세 이상 관람가", "애니메이션", 109}
    };

    playMovies(movies);

    int choiceIndex = choiceMovie(movies);

    if (choiceIndex != -1) {

    }

	const char* selected = theaterAddress();
	printf("선택한 지역 :  %s\n", selected);
    return 0;
}