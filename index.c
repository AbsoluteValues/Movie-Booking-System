#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <conio.h>
// ^ 윈도우 환경 전용이라 임시 주석 처리
#include <stdbool.h>
#include <time.h>
#include "mysql_client/include/mysql.h"

#define MAX_MOVIES 6
#define CinemaMaxSizeX 10
#define CinemaMaxSizeY 21



MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;

// MYSQL 계정 정보
char *server = "localhost";
char *user = "root";
char *password = "1234";
char *database = "cinema";
// ^ 각자 로컬 환경에 맞게 수정 필요

void InitMySQL() {
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        printf("MySQL 연결 실패: %s\n", mysql_error(conn));
        exit(1);
    }
}

void CloseMySQL() {
    mysql_close(conn);
}



typedef struct {
	int num;
	char title[100];
    char rating[50];
	char genre[50];
    int runtime;
} Movie;

typedef struct { 
    char region[100];
    char theater[100];
} TheaterAddress;

enum States { BLANK, CHOOSE, FULL };

typedef struct {
    int x, y;
} Point;

typedef struct {
    enum States state;
    Point point;
} Seat;

typedef struct {
    Seat seats[CinemaMaxSizeX][CinemaMaxSizeY];
    int seatCnt;
    int sizeX, sizeY;
} Cinema;



static void Clr() {
    system("cls");  // 콘솔 창 지우는 기능
}

int BookingPeople(Cinema* data);
int BookingSeats(Cinema* data, int n);
bool SearchAddress(TheaterAddress* ad);

int LoadMoviesFromDB(Movie* movie) {
    int count = 0;
    if (mysql_query(conn, "SELECT * FROM movie")) {
        printf("쿼리 오류: %s\n", mysql_error(conn));
        return 0;
    }
    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)) && count < MAX_MOVIES) {
        movie[count].num = atoi(row[0]);
        strcpy(movie[count].title, row[1]);
        strcpy(movie[count].rating, row[2]);
        strcpy(movie[count].genre, row[3]);
        movie[count].runtime = atoi(row[4]);
        count++;
    }
    mysql_free_result(res);
    return count;
}

void LoadBookedSeats(Cinema* cinema, const char* theater, const char* movieTitle) {
    char query[512];
    sprintf(query, "SELECT seat_row, seat_col FROM booking WHERE theater='%s' AND movie='%s'", theater, movieTitle);
    
    if (mysql_query(conn, query)) {
        printf("예약 좌석 불러오기 실패: %s\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    while ((row = mysql_fetch_row(res)) != NULL) {
        int rowIndex = row[0][0] - 'A';  // seat_row = 'A' ~ 'J'
        int colIndex = atoi(row[1]) - 1; // seat_col = 1 ~ 21 → 0 ~ 20
        if (rowIndex >= 0 && rowIndex < CinemaMaxSizeX && colIndex >= 0 && colIndex < CinemaMaxSizeY) {
            cinema->seats[rowIndex][colIndex].state = FULL;
            cinema->seatCnt--;
        }
    }
    mysql_free_result(res);
}

void PrintMovies(Movie* movie, int count) {
    printf("\n==== 상영 중인 영화 목록 ====\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s (%s) %s / %d분\n", movie[i].num, movie[i].title, movie[i].rating, movie[i].genre, movie[i].runtime);
    }
}

int ChooseMovie(Movie* movie, int count) {
    A:
    ;
    int choice;
    printf("\n영화 번호를 선택하시오(종료 : -1) : ");
    scanf("%d%*c", &choice);
    if (choice == -1) return -1;
    for (int i = 0; i < count; i++) {
        if (movie[i].num == choice) {
            printf("선택한 영화 : %s\n", movie[i].title);
            return i;
        }
    }
    printf("잘못된 번호입니다.\n");
    goto A;
}

bool theaterAddressSeoul(TheaterAddress *address) {
    int choice;
	while (1) {
        printf("\n============ 서울 ============\n");
        printf("== 영화관을 선택해주세요 ==\n");
        printf("1. 가산디지털\n");
        printf("2. 강동\n");
        printf("3. 건대입구\n");
        printf("4. 김포공항\n");
        printf("5. 노원\n");
        printf("6. 서울대입구\n");
        printf("7. 수유\n");
        printf("8. 신도림\n");
        printf("-1. 뒤로가기\n");
        printf(" -> ");
        scanf("%d%*c", &choice);	
        printf("================================\n");

        switch (choice) {
            case 1:
                strcpy(address->theater, "가산디지털");
                return true;
                break;
            case 2:
                strcpy(address->theater, "강동");
                return true;
                break;
            case 3:
                strcpy(address->theater, "건대입구");
                return true;
                break;
            case 4:
                strcpy(address->theater, "김포공항");
                return true;
                break;
            case 5:
                strcpy(address->theater, "노원");
                return true;
                break;
            case 6:
                strcpy(address->theater, "서울대입구");
                return true;
                break;
            case 7:
                strcpy(address->theater, "수유");
                return true;
                break;
            case 8:
                strcpy(address->theater, "신도림");
                return true;
                break;
            case -1:
                return false;
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        } 
        
        printf("선택한 영화관 :  %s\n", address->theater);
    }
}

bool theaterAddresGyeonggi(TheaterAddress *address){
    int choice;

	while (1) {
        printf("\n============ 경기 ============\n");
        printf("== 영화관을 선택해주세요 ==\n");
        printf("1. 광명\n");
        printf("2. 광명아울렛\n");
        printf("3. 동탄\n");
        printf("4. 부천\n");
        printf("5. 수원(수원역)\n");
        printf("6. 안산\n");
        printf("7. 안성\n");
        printf("8. 안양\n");
        printf("-1. 뒤로가기\n");
        printf(" -> ");
        scanf("%d%*c", &choice);	
        printf("================================\n");

        switch (choice) {
            case 1:
                strcpy(address->theater, "광명");
                break;
            case 2:
                strcpy(address->theater, "광명아울렛");
                break;
            case 3:
                strcpy(address->theater, "동탄");
                break;
            case 4:
                strcpy(address->theater, "부천");
                break;
            case 5:
                strcpy(address->theater, "수원(수원역)");
                break;
            case 6:
                strcpy(address->theater, "안산");
                break;
            case 7:
                strcpy(address->theater, "안성");
                break;
            case 8:
                strcpy(address->theater, "안양");
                break;
            case -1:
                return false;
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        } 
        
        printf("선택한 영화관 :  %s\n", address->theater);
    }
}

bool theaterAddressIncheon(TheaterAddress *address){
    int choice;

	while (1) {
        printf("\n============ 인천 ============\n");
        printf("== 영화관을 선택해주세요 ==\n");
        printf("1. 부평\n");
        printf("2. 부평갈산\n");
        printf("3. 부평역사\n");
        printf("-1. 뒤로가기\n");
        printf(" -> ");
        scanf("%d%*c", &choice);	
        printf("================================\n");

        switch (choice) {
            case 1:
                strcpy(address->theater, "부평");
                return true;
                break;
            case 2:
                strcpy(address->theater, "부평갈산");
                return true;
                break;
            case 3:
                strcpy(address->theater, "부평역사");
                return true;
                break;
            case -1:
                return false;
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
        } 
        
        printf("선택한 영화관 :  %s\n", address->theater);
    }
}

bool theaterAddress(TheaterAddress *address) {
    int regionChoice;
	while (1) {
        printf("\n============ 영화관 ============\n");
        printf("== 영화관 지역을 선택해주세요 ==\n");
        printf("1. 서울\n");
        printf("2. 경기\n");
        printf("3. 인천\n");
        printf("4. 검색\n");
        printf("-1. 뒤로가기\n");
        printf(" -> ");
        scanf("%d%*c", &regionChoice);	
        printf("================================\n");

        switch (regionChoice) {
            case 1:
                strcpy(address->region,"서울");
                if(theaterAddressSeoul(address)) return true;
                break;
            case 2:
                strcpy(address->region,"경기");
                if(theaterAddresGyeonggi(address)) return true;
                break;
            case 3:
                strcpy(address->region,"인천");
                if(theaterAddressIncheon(address)) return true;
                break;
            case 4:
                if (SearchAddress(address)) return true;
                else break;
            case -1:
                return false;
                break;
            default:
                printf("잘못된 입력입니다. 다시 선택해주세요.\n");
                break;
        }
        printf("선택한 지역 :  %s\n", address->region);
    }
}

void ResetCinema(Cinema* preset, int x, int y) {
    if (x > CinemaMaxSizeX) x = CinemaMaxSizeX;
    if (y > CinemaMaxSizeY) y = CinemaMaxSizeY;
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            preset->seats[i][j].state = BLANK;
            preset->seats[i][j].point.x = i;
            preset->seats[i][j].point.y = j;
        }
    }
    preset->sizeX = x;
    preset->sizeY = y;
    preset->seatCnt = x * y;
}

void ClearChooseSeats(Cinema* data) {
    for (int i = 0; i < data->sizeX; i++) {
        for (int j = 0; j < data->sizeY; j++) {
            if (data->seats[i][j].state == CHOOSE)
                data->seats[i][j].state = BLANK;
        }
    }
}

void RandResetCinema(Cinema* preset, int x, int y) {
    int i, j;
    preset->seatCnt = x * y;
    srand((unsigned)time(NULL));
    if (x > CinemaMaxSizeX) x = CinemaMaxSizeX;
    if (y > CinemaMaxSizeY) y = CinemaMaxSizeY;
    for (i = 0; i < x; i++) {
        for (j = 0; j < y; j++) {
            if (rand() % 4 > 2){
                preset->seats[i][j].state = FULL;
                preset->seatCnt -= 1;
            }
            else {
                preset->seats[i][j].state = BLANK;
            }
            preset->seats[i][j].point.x = i;
            preset->seats[i][j].point.y = j;
        }
    }
    preset->sizeX = x;
    preset->sizeY = y;
}

void PrintSeats(Cinema* data) {
    // 매개변수에 이게 어느 지역의 어느 영화의 어느 관인지 구분할 수 있는 뭔가가 필요함.
    // 불러와서 좌석표 출력.

    char lineX[CinemaMaxSizeX] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J' };
    int lineY[CinemaMaxSizeY] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21 };

    Clr();
    int i, j;
    printf("==================================================\n\n");
    printf("  ");
    for (i = 0; i < data->sizeY; i++) printf("%02d ", lineY[i]);
    printf("\n");
    for (i = 0; i < data->sizeX; i += 1) {
        printf("%c ", lineX[i]);
        for (j = 0; j < data->sizeY; j += 1) {
            switch (data->seats[i][j].state)
            {
            case BLANK:
                printf("□  ");
                break;
            case CHOOSE:
                printf("▣  ");
                break;
            case FULL:
                printf("■  ");
                break;
            default:
                printf("   ");
                break;
            }
        }
        printf("\n");
    }
    printf("\n==================================================\n");
}

int BookingPeople(Cinema* data) {
    // 좌석목록 띄운 채로 몇명 예약할지 결정.
    // 뒤로가기라면 시간/관 선택 창으로.
    // 인원이 0명 초과일때만 BookingSeats 로 진행.
    int n;
A:
    Clr();
    PrintSeats(data);
    printf("\n-1 입력시 이전 메뉴로\n- - - - - - - - - - - - - - -\n");
    printf("예약할 인원 : ");
    scanf("%d%*c", &n);
    if (n == -1) return -1;
    if (n < 1) {
        printf("1명 이상 선택해주세요.\n");
        system("pause");
        goto A;
    }
    else if (n > data->seatCnt) {
        printf("예매 가능한 좌석이 부족합니다.\n남은 좌석은 %d석입니다.\n", data->seatCnt);
        system("pause");
        goto A;
    }
    else {
        int result = BookingSeats(data, n);
        if (result == -1) goto A;
    }
    return 1;
}

int BookingSeats(Cinema* data, int n) {
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
    Clr();
    PrintSeats(data);
    printf("\n-1 입력시 이전 메뉴로\n- - - - - - - - - - - - - - -\n");
    printf("예약할 좌석 좌표를 입력하세요(ex:B3) : ");
    // 이렇게 받을지, 혹은 한글자씩 받으면서 계속 초기화할지...
    // 고민은 필요해 보인다.
    fgets(point, sizeof(char) * 20, stdin);
    p.x = (int)point[0];
    p.y = atoi(&point[1]) - 1;//a97 A65 z122 Z90
    if (atoi(&point[0]) == -1) {
        if (cnt > 0) {
            ClearChooseSeats(data);
        }
        return -1;
    }
    if (p.x >= 97 && p.x <= 97 + data->sizeX && p.x <= 122 && p.y >= 0 && p.y < data->sizeY) {
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
    else if (p.x >= 65 && p.x <= 65 + data->sizeX && p.x <= 90 && p.y >= 0 && p.y < data->sizeY) {
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
    system("pause");
    if (cnt < n) goto A;
B:
    Clr();
    PrintSeats(data);
    printf("1. 취소 / 2. 예매하기 : ");
    scanf("%d%*c", &i);
    switch (i)
    {
    case 1:
        goto A;
        break;
    case 2:
        data->seatCnt -= n;
        break;
    default:
        goto B; // 재입력받음.
        break;
    }

    return 0;
}

void SaveBookingToDB(Cinema* cinema, const char* theater, const char* movieTitle) {
    char query[512];
    for (int i = 0; i < cinema->sizeX; i++) {
        for (int j = 0; j < cinema->sizeY; j++) {
            if (cinema->seats[i][j].state == CHOOSE) {
                char row = 'A' + i;
                int col = j + 1;
                sprintf(query, "INSERT INTO booking (theater, movie, seat_row, seat_col) VALUES ('%s', '%s', '%c', %d)",
                    theater, movieTitle, row, col);
                if (mysql_query(conn, query)) {
                    printf("좌석 저장 실패: %s\n", mysql_error(conn));
                }
            }
        }
    }
}

bool SearchAddress(TheaterAddress* address) {
    char region[3][8][20] = {
        { "가산디지털", "강동", "건대입구", "김포공항", "노원", "서울대입구", "수유", "신도림" },
        {"광명", "광명아울렛", "동탄", "부천", "수원(수원역)", "안산", "안성", "안양"},
        {"부평", "부평갈산", "부평역사", " ", " ", " ", " ", " "}
    };
    char s[3][10] = { "서울", "인천", "부천" };
    int i, j;
    char c[20];

A:
    Clr();
    printf("영화관 위치 검색(-1 입력 시 이전 메뉴로) : ");
    fgets(c, sizeof(char) * 20, stdin);
    c[strlen(c) - 1] = '\0';
    if (atoi(&c[0]) == -1)
        return false;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 8; j++) {
            if (strcmp(region[i][j], " ") == 0)
                continue;
            if (strcmp(c, region[i][j]) == 0) {
                strcpy(address->region, s[i]);
                strcpy(address->theater, region[i][j]);
                printf("선택한 지역 :  %s\n", address->region);
                printf("선택한 영화관 :  %s\n", address->theater);
                return true;
            }
        }
    }
    printf("해당 지역에는 상영관이 없습니다. 다시 입력해주세요.\n");
    system("pause");
    goto A;
}

int main(void) {

    InitMySQL();

    Movie movie[MAX_MOVIES];
    int movieCount = LoadMoviesFromDB(movie);
    if (movieCount == 0) return 1;

    A:
    PrintMovies(movie, movieCount);
    int choice = ChooseMovie(movie, movieCount);
    if (choice == -1) return 1;

    B:
    ;
    TheaterAddress *address = (TheaterAddress*)malloc(sizeof(TheaterAddress));
    bool result = theaterAddress(address);
    if (result == 0) {
        goto A;
    }

    Cinema* preset = (Cinema*)malloc(sizeof(Cinema));
    ResetCinema(preset, 10, 21);
    LoadBookedSeats(preset, address->theater, movie[choice].title);
    int asdf = BookingPeople(preset);
    if (asdf == -1) {
        goto B;
    }

    SaveBookingToDB(preset, address->theater, movie[choice].title);
    printf("예매를 완료 했습니다.\n");

    free(preset);
    free(address);
    CloseMySQL();

    return 0;
}