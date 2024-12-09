/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
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

// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	// ���� ��ġ�� ���ڸ� ���� �������� ����
	char ch = frontbuf[prev.row][prev.column];
	int color = get_color_for_char(ch, prev);
	printc(padd(map_pos, prev), ch, color);

	// ���� ��ġ�� ���ڸ� Ŀ�� �������� ���
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
	// MAP_WIDTH�� �ݰ� ���
	int mid_column = MAP_WIDTH / 2;

	// 'B'�� 'H'�� ���ؼ� ���� �ٸ��� ����
	if (ch == 'B' || ch == 'H') {
		if (cursor_pos.column <= mid_column) {
			// ���� �ݿ��� ���� 79 (��: Atreides Base)
			return 23;  // �Ķ��� ���, ��� ���� (�Ǵ� ���ϴ� ��)
		}
		else {
			// ������ �ݿ��� ���� 23 (��: Harkonnen Base)
			return 79;  // ������ ���, ��� ���� (�Ǵ� ���ϴ� ��)
		}
	}

	// ������ ���ڿ� ���� ���� ����
	switch (ch) {
	case 'P': return 7;    // Plate
	case 'S': return 71;   // Spice Field
	case 'R': return 135;  // Rock
	case 'W': return 103;  // Sand Worm
	case ' ': return COLOR_DEFAULT + 220;  // �� ����
	default: return COLOR_DEFAULT;  // ��Ÿ ����
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