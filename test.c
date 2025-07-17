#include <stdio.h>
#include <stdlib.h>
#include "mysql_client/include/mysql.h"

int main(void) {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "localhost";
    char *user = "root";
    char *password = "1234";
    char *database = "restful";

    conn = mysql_init(NULL);
    if (!conn) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    // DB까지 포함해서 한 번만 연결 시도
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    // 쿼리 실행
    if (mysql_query(conn, "SELECT * FROM user_tbl;")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    res = mysql_use_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_use_result() failed\n");
        mysql_close(conn);
        return 1;
    }

    // 결과 출력 예시
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%s\n", row[0]); // 예시로 첫 번째 컬럼 출력
    }

    mysql_free_result(res);
    mysql_close(conn);

    return 0;
}