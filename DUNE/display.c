/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };

char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char back[SYS_MAP_HEIGHT][SYS_MAP_WIDTH] = { 0 };
char front[SYS_MAP_HEIGHT][SYS_MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_sys_message(sys_array);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor,
	char sys_array[SYS_MAP_HEIGHT][SYS_MAP_WIDTH])
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_sys_message(sys_array);
	// display_object_info()
	// display_commands()
	// ...
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	// 이전 위치의 문자를 원래 색상으로 복원
	char ch = frontbuf[prev.row][prev.column];
	int color = get_color_for_char(ch, prev);
	printc(padd(map_pos, prev), ch, color);

	// 현재 위치의 문자를 커서 색상으로 출력
	ch = frontbuf[curr.row][curr.column];
	printc(padd(map_pos, curr), ch, COLOR_CURSOR);
}


// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			if (frontbuf[i][j] != backbuf[i][j]) {

				if (backbuf[i][j] == ' ') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 220);
				}
				else if (backbuf[i][j] == 'B') {
					POSITION pos = { i, j };
					if (j <= MAP_WIDTH / 2) {
						printc(padd(map_pos, pos), backbuf[i][j], 23);
					}
					else {
						printc(padd(map_pos, pos), backbuf[i][j], 79);
					}
				}
				else if (backbuf[i][j] == 'H') {
					POSITION pos = { i, j };
					if (j <= MAP_WIDTH / 2) {
						printc(padd(map_pos, pos), backbuf[i][j], 23);
					}
					else {
						printc(padd(map_pos, pos), backbuf[i][j], 79);
					}
				}
				else if (backbuf[i][j] == 'P') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], 7);
				}
				else if (backbuf[i][j] == 'S') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], 71);
				}
				else if (backbuf[i][j] == 'R') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], 135);
				}
				else if (backbuf[i][j] == 'W') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], 103);
				}
				else {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT);
				}
			}
			frontbuf[i][j] = backbuf[i][j];
		}
	}
}

void project_sys_message(char src[SYS_MAP_HEIGHT][SYS_MAP_WIDTH], char dest[SYS_MAP_HEIGHT][SYS_MAP_WIDTH]) {
	for (int i = 0; i < SYS_MAP_HEIGHT; i++) {
		for (int j = 0; j < SYS_MAP_WIDTH; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

void display_sys_message(char sys_array[SYS_MAP_HEIGHT][SYS_MAP_WIDTH]) {
	project_sys_message(sys_array, back);

	for (int i = 0; i < SYS_MAP_HEIGHT; i++) {
		for (int j = 0; j < SYS_MAP_WIDTH; j++) {
			if (front[i][j] != back[i][j]) {
				POSITION pos = { 1 + i, MAP_WIDTH + 1 + j };
				printc(pos, back[i][j], COLOR_DEFAULT);
			}
			front[i][j] = back[i][j];
		}
	}
}

int get_color_for_char(char ch, POSITION cursor_pos) {
	// MAP_WIDTH의 반값 계산
	int mid_column = MAP_WIDTH / 2;

	// 'B'와 'H'에 대해서 색상 다르게 설정
	if (ch == 'B' || ch == 'H') {
		if (cursor_pos.column <= mid_column) {
			// 왼쪽 반에서 색상 79 (예: Atreides Base)
			return 23;  // 파란색 배경, 흰색 글자 (또는 원하는 색)
		}
		else {
			// 오른쪽 반에서 색상 23 (예: Harkonnen Base)
			return 79;  // 빨간색 배경, 흰색 글자 (또는 원하는 색)
		}
	}

	// 나머지 문자에 대한 색상 설정
	switch (ch) {
	case 'P': return 7;    // Plate
	case 'S': return 71;   // Spice Field
	case 'R': return 135;  // Rock
	case 'W': return 103;  // Sand Worm
	case ' ': return COLOR_DEFAULT + 220;  // 빈 공간
	default: return COLOR_DEFAULT;  // 기타 문자
	}
}
	
void clear_sys_message(void) {
	POSITION position;
	for (int i = 2; i < SYS_MAP_HEIGHT - 2; i++) {
		for (int j = 2; j < SYS_MAP_WIDTH - 2; j++) {
			position.row = i;
			position.column = MAP_WIDTH + j;
			gotoxy(position);
			printf(" ");
		}
	}
}