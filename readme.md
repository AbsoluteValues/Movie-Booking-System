# MYSQL 설치 필수

# 이하는 필수 쿼리문
CREATE DATABASE cinema;

USE cinema;

CREATE TABLE movie (
    num INT PRIMARY KEY,
    title VARCHAR(100),
    rating VARCHAR(50),
    genre VARCHAR(100),
    runtime INT
);

CREATE TABLE booking (
    id INT AUTO_INCREMENT PRIMARY KEY,
    theater VARCHAR(100),
    movie VARCHAR(100),
    seat_row CHAR(1),
    seat_col INT
);

INSERT INTO movie (num, title, rating, genre, runtime) VALUES
(1, 'F1 더 무비', '12세 이상 관람가', '액션, 드라마', 155),
(2, '킹 오브 킹스', '전체 이용가', '애니메이션', 101),
(3, '슈퍼맨', '12세 이상 관람가', '액션', 129),
(4, '쥬라기 월드: 새로운 시작', '12세 이상 관람가', '액션', 129),
(5, '노이즈', '15세 이상 관람가', '모험, 액션, 공포, 스릴러', 133),
(6, '명탐정 코난: 척안의 잔상', '12세 이상 관람가', '애니메이션', 109);