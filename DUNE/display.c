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

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], 
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	// display_system_message()
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

	char ch = frontbuf[prev.row][prev.column];
	if (ch == 'B' || ch == 'H') {
		if (prev.column < MAP_WIDTH / 2) {
			printc(padd(map_pos, prev), ch, 79);
		}
		else {
			printc(padd(map_pos, prev), ch, 23);
		}
	}
	else if (ch == 'P') {
		printc(padd(map_pos, prev), ch, 7);
	}
	else if (ch == '5') {
		printc(padd(map_pos, prev), ch, 71);
	}
	else if (ch == 'R') {
		printc(padd(map_pos, prev), ch, 135);
	}
	else if (ch == 'W') {
		printc(padd(map_pos, prev), ch, 103);
	}
	else {
		printc(padd(map_pos, prev), ch, COLOR_CURSOR);
	}

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

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH], CURSOR cursor) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {

			if (i == cursor.current.row && j == cursor.current.column) {
				continue;
			}

			if (frontbuf[i][j] != backbuf[i][j]) {

				if (backbuf[i][j] == ' ') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_DEFAULT + 220);
				}
				else if (backbuf[i][j] == 'B') {
					POSITION pos = { i, j };
					if (j <= MAP_WIDTH / 2) {
						printc(padd(map_pos, pos), backbuf[i][j], 79);
					}
					else {
						printc(padd(map_pos, pos), backbuf[i][j], 23);
					}
				}
				else if (backbuf[i][j] == 'H') {
					POSITION pos = { i, j };
					if (j <= MAP_WIDTH / 2) {
						printc(padd(map_pos, pos), backbuf[i][j], 79);
					}
					else {
						printc(padd(map_pos, pos), backbuf[i][j], 23);
					}
				}
				else if (backbuf[i][j] == 'P') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], 7);
				}
				else if (backbuf[i][j] == '5') {
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



