﻿#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
void press_space(POSITION pos);
void press_esc(void);
void press_H(POSITION pos);
POSITION sample_obj_next_position(void);
POSITION current_space_pos = { 0, 0 };
POSITION current_H_pos = { 0,0 };

DWORD getMilliseconds(SYSTEMTIME time) {
	return (DWORD)(time.wSecond * 1000 + time.wMilliseconds);
}

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
char status_array[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH] = { 0 };
char sys_array[SYS_MAP_HEIGHT][SYS_MAP_WIDTH] = { 0 };
char command_array[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH] = { 0 };

RESOURCE resource = { 
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.move_period = 300,
	.next_move_time = 300
};

OBJECT_CON sand_warms = {
	.pos1 = {MAP_HEIGHT - (MAP_HEIGHT - 3), MAP_WIDTH - (MAP_WIDTH - 11)},
	.pos2 = {MAP_HEIGHT - 8, MAP_WIDTH - (MAP_WIDTH - 39)},
	.layer = 1,
	.repr = 'W'
};

OBJECT_CON Atreides_base = {
	.pos1 = {MAP_HEIGHT - 3, MAP_WIDTH - (MAP_WIDTH - 1)},
	.pos2 = {MAP_HEIGHT - 3, MAP_WIDTH - (MAP_WIDTH - 2)},
	.pos3 = {MAP_HEIGHT - 2, MAP_WIDTH - (MAP_WIDTH - 1)},
	.pos4 = {MAP_HEIGHT - 2, MAP_WIDTH - (MAP_WIDTH - 2)},
	.layer = 0,
	.repr = 'B'
};

OBJECT_CON A_plate = {
	.pos1 = {MAP_HEIGHT - 3, MAP_WIDTH - (MAP_WIDTH - 3)},
	.pos2 = {MAP_HEIGHT - 3, MAP_WIDTH - (MAP_WIDTH - 4)},
	.pos3 = {MAP_HEIGHT - 2, MAP_WIDTH - (MAP_WIDTH - 3)},
	.pos4 = {MAP_HEIGHT - 2, MAP_WIDTH - (MAP_WIDTH - 4)},
	.layer = 0,
	.repr = 'P'
};

OBJECT_CON A_Harvester = {
	.pos1 = {MAP_HEIGHT - 4, MAP_WIDTH - (MAP_WIDTH - 1)},
	.layer = 1,
	.repr = 'H'
};

OBJECT_CON Harkonnen_base = {
	.pos1 = {MAP_HEIGHT - (MAP_HEIGHT - 1), MAP_WIDTH - 3},
	.pos2 = {MAP_HEIGHT - (MAP_HEIGHT - 1), MAP_WIDTH - 2},
	.pos3 = {MAP_HEIGHT - (MAP_HEIGHT - 2), MAP_WIDTH - 3},
	.pos4 = {MAP_HEIGHT - (MAP_HEIGHT - 2), MAP_WIDTH - 2},
	.layer = 0,
	.repr = 'B'
};

OBJECT_CON H_plate = {
	.pos1 = {MAP_HEIGHT - (MAP_HEIGHT - 1), MAP_WIDTH - 5},
	.pos2 = {MAP_HEIGHT - (MAP_HEIGHT - 1), MAP_WIDTH - 4},
	.pos3 = {MAP_HEIGHT - (MAP_HEIGHT - 2), MAP_WIDTH - 5},
	.pos4 = {MAP_HEIGHT - (MAP_HEIGHT - 2), MAP_WIDTH - 4},
	.layer = 0,
	.repr = 'P'
};

OBJECT_CON H_Harvester = {
	.pos1 = {MAP_HEIGHT - (MAP_HEIGHT - 3), MAP_WIDTH - 2},
	.layer = 1,
	.repr = 'H'
};

OBJECT_CON spice = {
	.pos1 = {MAP_HEIGHT - 8, MAP_WIDTH - (MAP_WIDTH - 1)},
	.pos2 = {MAP_HEIGHT - (MAP_HEIGHT - 8), MAP_WIDTH - 2},
	.layer = 0,
	.repr = '5'
};

OBJECT_CON small_rock = {
	.pos1 = {MAP_HEIGHT - 10, MAP_WIDTH - (MAP_WIDTH - 16)},
	.pos2 = {MAP_HEIGHT - (MAP_HEIGHT - 6), MAP_WIDTH - 19},
	.pos3 = {MAP_HEIGHT - 4, MAP_WIDTH - 16},
	.layer = 0,
	.repr = 'R'
};

OBJECT_CON big_rock1 = {
	.pos1 = {MAP_HEIGHT - 5, MAP_WIDTH - 29},
	.pos2 = {MAP_HEIGHT - 5, MAP_WIDTH - 30},
	.pos3 = {MAP_HEIGHT - 6, MAP_WIDTH - 29},
	.pos4 = {MAP_HEIGHT - 6, MAP_WIDTH - 30},
	.layer = 0,
	.repr = 'R'
};

OBJECT_CON big_rock2 = {
	.pos1 = {MAP_HEIGHT - (MAP_HEIGHT - 4), MAP_WIDTH - (MAP_WIDTH - 18)},
	.pos2 = {MAP_HEIGHT - (MAP_HEIGHT - 4), MAP_WIDTH - (MAP_WIDTH - 19)},
	.pos3 = {MAP_HEIGHT - (MAP_HEIGHT - 5), MAP_WIDTH - (MAP_WIDTH - 18)},
	.pos4 = {MAP_HEIGHT - (MAP_HEIGHT - 5), MAP_WIDTH - (MAP_WIDTH - 19)},
	.layer = 0,
	.repr = 'R'
};

//아트레이데스 정보
void Atreides_info(void) {
	POSITION pos;
	pos.row = 3;
	pos.column = MAP_WIDTH + 4;
	gotoxy(pos);
	printf("[ 정보 ]");
	pos.row += 1;
	gotoxy(pos);
	printf("아트레이데스 본진");
	pos.row += 1;
	gotoxy(pos);
	printf("건설 비용 : 없음");
	pos.row += 1;
	gotoxy(pos);
	printf("내구도 : 50");
}
//하코넨 정보
void Harkonnen_info(void) {
	POSITION pos;
	pos.row = 3;
	pos.column = MAP_WIDTH + 4;
	gotoxy(pos);
	printf("[ 정보 ]");
	pos.row += 1;
	gotoxy(pos);
	printf("하코넨 본진");
	pos.row += 1;
	gotoxy(pos);
	printf("건설 비용 : 없음");
	pos.row += 1;
	gotoxy(pos);
	printf("내구도 : 50");
}
//장판 정보
void Plate_info(void) {
	POSITION pos;
	pos.row = 3;
	pos.column = MAP_WIDTH + 4;
	gotoxy(pos);
	printf("[ 정보 ]");
	pos.row += 1;
	gotoxy(pos);
	printf("장판(P:plate)");
	pos.row += 1;
	gotoxy(pos);
	printf("설명 : 건물 짓기 전에 깔기");
	pos.row += 1;
	gotoxy(pos);
	printf("건설 비용 : 1");
	pos.row += 1;
	gotoxy(pos);
	printf("내구도 : 없음");
}

void Construction(void) {
	//아트레이데스 베이스
	map[Atreides_base.layer][Atreides_base.pos1.row][Atreides_base.pos1.column] = Atreides_base.repr;
	map[Atreides_base.layer][Atreides_base.pos2.row][Atreides_base.pos2.column] = Atreides_base.repr;
	map[Atreides_base.layer][Atreides_base.pos3.row][Atreides_base.pos3.column] = Atreides_base.repr;
	map[Atreides_base.layer][Atreides_base.pos4.row][Atreides_base.pos4.column] = Atreides_base.repr;

	//하코넨 베이스
	map[Harkonnen_base.layer][Harkonnen_base.pos1.row][Harkonnen_base.pos1.column] = Harkonnen_base.repr;
	map[Harkonnen_base.layer][Harkonnen_base.pos2.row][Harkonnen_base.pos2.column] = Harkonnen_base.repr;
	map[Harkonnen_base.layer][Harkonnen_base.pos3.row][Harkonnen_base.pos3.column] = Harkonnen_base.repr;
	map[Harkonnen_base.layer][Harkonnen_base.pos4.row][Harkonnen_base.pos4.column] = Harkonnen_base.repr;

	//아트레이데스 장판
	map[A_plate.layer][A_plate.pos1.row][A_plate.pos1.column] = A_plate.repr;
	map[A_plate.layer][A_plate.pos2.row][A_plate.pos2.column] = A_plate.repr;
	map[A_plate.layer][A_plate.pos3.row][A_plate.pos3.column] = A_plate.repr;
	map[A_plate.layer][A_plate.pos4.row][A_plate.pos4.column] = A_plate.repr;

	//하코넨 장판
	map[H_plate.layer][H_plate.pos1.row][H_plate.pos1.column] = H_plate.repr;
	map[H_plate.layer][H_plate.pos2.row][H_plate.pos2.column] = H_plate.repr;
	map[H_plate.layer][H_plate.pos3.row][H_plate.pos3.column] = H_plate.repr;
	map[H_plate.layer][H_plate.pos4.row][H_plate.pos4.column] = H_plate.repr;

	//아트레이데스 하베스터 위치
	map[A_Harvester.layer][A_Harvester.pos1.row][A_Harvester.pos1.column] = A_Harvester.repr;

	//하코넨 하베스터 위치
	map[H_Harvester.layer][H_Harvester.pos1.row][H_Harvester.pos1.column] = H_Harvester.repr;

	//스파이스 위치
	map[spice.layer][spice.pos1.row][spice.pos1.column] = spice.repr;
	map[spice.layer][spice.pos2.row][spice.pos2.column] = spice.repr;

	//작은 돌 위치
	map[small_rock.layer][small_rock.pos1.row][small_rock.pos1.column] = small_rock.repr;
	map[small_rock.layer][small_rock.pos2.row][small_rock.pos2.column] = small_rock.repr;
	map[small_rock.layer][small_rock.pos3.row][small_rock.pos3.column] = small_rock.repr;

	//큰 돌1 위치
	map[big_rock1.layer][big_rock1.pos1.row][big_rock1.pos1.column] = big_rock1.repr;
	map[big_rock1.layer][big_rock1.pos2.row][big_rock1.pos2.column] = big_rock1.repr;
	map[big_rock1.layer][big_rock1.pos3.row][big_rock1.pos3.column] = big_rock1.repr;
	map[big_rock1.layer][big_rock1.pos4.row][big_rock1.pos4.column] = big_rock1.repr;

	//큰 돌2 위치
	map[big_rock2.layer][big_rock2.pos1.row][big_rock2.pos1.column] = big_rock2.repr;
	map[big_rock2.layer][big_rock2.pos2.row][big_rock2.pos2.column] = big_rock2.repr;
	map[big_rock2.layer][big_rock2.pos3.row][big_rock2.pos3.column] = big_rock2.repr;
	map[big_rock2.layer][big_rock2.pos4.row][big_rock2.pos4.column] = big_rock2.repr;

	//샌드웜 위치
	map[sand_warms.layer][sand_warms.pos1.row][sand_warms.pos1.column] = sand_warms.repr;
	map[sand_warms.layer][sand_warms.pos2.row][sand_warms.pos2.column] = sand_warms.repr;
}

/* ================= main() =================== */
int main(void) {
	SYSTEMTIME tVal1, tVal2;		// 키보드가 입력된 시간 확인
	DWORD time_gap;					// 두 키보드 입력간 시간 차
	int PreKey = 0;					// 앞에서 입력받은 방향키
	srand((unsigned int)time(NULL));

	init();
	intro();
	Construction();
	display(resource, map, cursor, status_array, sys_array, command_array);

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();

		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			GetSystemTime(&tVal2);	// 방향키가 눌린 시점 시간
			cursor_move(ktod(key));
			if (PreKey != 0 && PreKey == key) {
				time_gap = getMilliseconds(tVal2) - getMilliseconds(tVal1);	// 방향키가 눌린 시간 차 구하기
				if (time_gap < DOUBLE_CLICK_GAP) {
					for (int i = 0; i < 3; i++) {	
						display_cursor(cursor);
						cursor_move(ktod(key));
					}
				}
			}
			GetSystemTime(&tVal1);	// 현 시점 시간을 저장
			PreKey = key;			// 방향키값 저장
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_space: 
				current_space_pos = cursor.current;
				press_space(current_space_pos);
				break;
			case k_esc: 
				press_esc();
				break;
			case k_H:
				current_H_pos = cursor.current;
				press_H(current_H_pos);
				break;
			case k_quit: outro();
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sample_obj_move();

		// 화면 출력
		display(resource, map, cursor, status_array, sys_array, command_array);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("DUNE 1.5\n");
	printf("..............................\n");
	printf(".DDDDD              1   5555 .\n");
	printf(".D     D           11   5    .\n");
	printf(".D     D            1   5555 .\n");
	printf(".D     D            1      5 .\n");
	printf(".D     D            1 . 5555 .\n");
	printf(".D     D                     .\n");
	printf(".D     D  U   U  Nnnn    eee .\n");
	printf(".D     D  U   U  N   n  e   e.\n");
	printf(".D     D  U   U  N   n  eeeee.\n");
	printf(".D     D  U   U  N   n  e    .\n");
	printf(".DDDDD     UUUU  N   n   eee .\n");
	printf("..............................\n");
	Sleep(1500);
	system("cls");
}

void outro(void) {
	set_color(COLOR_DEFAULT);
	system("cls");
	printf("exiting...\n");
	printf("exiting......\n");
	printf("exiting............\n");
	printf("exiting.....................\n");
	printf("exiting..............................\n");
	exit(0);
}

void init(void) {
	// layer 0(map[0])에 지형 생성
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';
		for (int j = 1; j < MAP_WIDTH-1; j++) {
			map[0][i][j] = ' ';
		}
	}

	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	// 상태창
	for (int j = 0; j < STATUS_MAP_WIDTH; j++) {
		status_array[0][j] = '#';  // 첫 번째 행 (상단 경계)
		status_array[STATUS_MAP_HEIGHT - 1][j] = '#';  // 마지막 행 (하단 경계)
	}

	// 좌측과 우측 경계
	for (int i = 1; i < STATUS_MAP_HEIGHT - 1; i++) {
		status_array[i][0] = '#';  // 첫 번째 열 (좌측 경계)
		status_array[i][STATUS_MAP_WIDTH - 1] = '#';  // 마지막 열 (우측 경계)

		// 내부는 공백으로 채우기
		for (int j = 1; j < STATUS_MAP_WIDTH - 1; j++) {
			status_array[i][j] = ' ';  // 내부 부분
		}
	}

	// 시스템 메시지 창
	for (int j = 0; j < SYS_MAP_WIDTH; j++) {
		sys_array[0][j] = '#';
		sys_array[SYS_MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < SYS_MAP_HEIGHT - 1; i++) {
		sys_array[i][0] = '#';
		sys_array[i][SYS_MAP_WIDTH - 1] = '#';
		
		for (int j = 1; j < SYS_MAP_WIDTH - 1; j++) {
			sys_array[i][j];
		}
	}

	// 명령어 창
	for (int j = 0; j < COMMAND_MAP_WIDTH; j++) {
		command_array[0][j] = '#';
		command_array[COMMAND_MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < COMMAND_MAP_HEIGHT - 1; i++) {
		command_array[i][0] = '#';
		command_array[i][COMMAND_MAP_WIDTH - 1] = '#';

		for (int j = 1; j < COMMAND_MAP_WIDTH - 1; j++) {
			command_array[i][j];
		}
	}
	// object sample
	map[1][obj.pos.row][obj.pos.column] = 'o';
}


// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
    POSITION curr = cursor.current;
    POSITION new_pos = pmove(curr, dir);

    // 새 위치가 유효한 경우에만 이동
    if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 && \
        1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
        
        cursor.previous = cursor.current;  // 이전 위치 저장
        cursor.current = new_pos;          // 현재 위치 갱신

        // 이전 위치 복원
        char original_char = map[0][cursor.previous.row][cursor.previous.column];
        int color = get_color_for_char(original_char, cursor.previous); // 문자에 맞는 색상 가져오기
		printc(padd(map_pos, cursor.previous), original_char, color);
		
    }
}

// 스페이스바를 눌렀을 때
void press_space(POSITION pos) {
	POSITION prt = { 3, MAP_WIDTH + 4 };
	POSITION com = { MAP_HEIGHT + 4, MAP_WIDTH + 4 };
	char repr = map[0][pos.row][pos.column];
	char repr2 = map[1][pos.row][pos.column];
	int mid_column = MAP_WIDTH / 2;

	if (repr == 'B') {
		if (pos.column <= mid_column) {
			clear_status();
			clear_command();
			Atreides_info();
			gotoxy(com);
			printf("H : 하베스터 생산");
		}
		else {
			clear_status();
			clear_command();
			Harkonnen_info();
			gotoxy(com);
			printf("H : 하베스터 생산");
		}
	}
	else if (repr2 == 'H') {
		if (pos.column <= mid_column) {
			clear_status();
			clear_command();
			gotoxy(prt);
			printf("아트레이데스 하베스터...");
		}
		else {
			clear_status();
			clear_command();
			gotoxy(prt);
			printf("하코넨 하베스터...");
		}
	}
	else if (repr == 'R') {
		clear_status();
		clear_command();
		gotoxy(prt);
		printf("바위...");
	}
	else if (repr == 'S') {
		clear_status();
		clear_command();
		gotoxy(prt);
		printf("스파이스...");
	}
	else if (repr == 'P') {
		clear_status();
		clear_command();
		Plate_info();
	}
	else if (repr2 == 'W') {
		clear_status();
		clear_command();
		gotoxy(prt);
		printf("샌드웜...");
	}
	else if (repr == ' ') {
		clear_status();
		clear_command();
		sys_message_clear();
		gotoxy(prt);
		printf("사막 지형...");
	}
}

// esc눌렀을 때
void press_esc(void) {
	clear_status();
	clear_command();
}

void press_H(POSITION pos) {
	POSITION Atre = { MAP_HEIGHT - 3, MAP_WIDTH - (MAP_WIDTH - 2) };
	POSITION Hark = { MAP_HEIGHT - (MAP_HEIGHT - 4), MAP_WIDTH - 3 };
	char repr = map[0][pos.row][pos.column];
	int mid_column = MAP_WIDTH / 2;

	if (repr == 'B') {
		if (pos.column <= mid_column) {
			gotoxy(Atre);
			printf("H");
		}
		else {
			gotoxy(Hark);
			printf("H");
		}
	}
}
/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}
	
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {
		
		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.move_period;
}



