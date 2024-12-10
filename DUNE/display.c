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
char status_back[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH] = { 0 };
char status_front[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH] = { 0 };
char sys_back[SYS_MAP_HEIGHT][SYS_MAP_WIDTH] = { 0 };
char sys_front[SYS_MAP_HEIGHT][SYS_MAP_WIDTH] = { 0 };
char command_back[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH] = { 0 };
char command_front[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH] = { 0 };

void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);
void display_cursor(CURSOR cursor);
void display_status(status_array);
void display_sys(sys_array);
void display_command(command_array);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor,
	char status_array[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH],
	char sys_array[SYS_MAP_HEIGHT][SYS_MAP_WIDTH],
	char command_array[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH])
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_status(status_array);
	display_sys(sys_array);
	display_command(command_array);
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
						printc(padd(map_pos, pos), backbuf[i][j], COLOR_Atreides);
					}
					else {
						printc(padd(map_pos, pos), backbuf[i][j], COLOR_Harkonnen);
					}
				}
				else if (backbuf[i][j] == 'H') {
					POSITION pos = { i, j };
					if (j <= MAP_WIDTH / 2) {
						printc(padd(map_pos, pos), backbuf[i][j], COLOR_Atreides);
					}
					else {
						printc(padd(map_pos, pos), backbuf[i][j], COLOR_Harkonnen);
					}
				}
				else if (backbuf[i][j] == 'P') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_P);
				}
				else if (backbuf[i][j] == 'S') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_SPICE);
				}
				else if (backbuf[i][j] == 'R') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_ROCK);
				}
				else if (backbuf[i][j] == 'W') {
					POSITION pos = { i, j };
					printc(padd(map_pos, pos), backbuf[i][j], COLOR_WORM);
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

void project_status(char src[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH], char dest[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH]) {
	for (int i = 0; i < STATUS_MAP_HEIGHT; i++) {
		for (int j = 0; j < STATUS_MAP_WIDTH; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

void display_status(char status_array[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH]) {
	project_status(status_array, status_back);

	for (int i = 0; i < STATUS_MAP_HEIGHT; i++) {
		for (int j = 0; j < STATUS_MAP_WIDTH; j++) {
			if (status_front[i][j] != status_back[i][j]) {
				POSITION pos = { 1 + i, MAP_WIDTH + 1 + j };
				printc(pos, status_back[i][j], COLOR_DEFAULT);
			}
			status_front[i][j] = status_back[i][j];
		}
	}
}

void clear_status(void) {
	POSITION position;
	for (int i = 2; i < STATUS_MAP_HEIGHT - 2; i++) {
		for (int j = 2; j < STATUS_MAP_WIDTH - 2; j++) {
			position.row = i;
			position.column = MAP_WIDTH + j;
			gotoxy(position);
			printf(" ");
		}
	}
}

void clear_command(void) {
	POSITION position;
	for (int i = 2; i < COMMAND_MAP_HEIGHT - 2; i++) {
		for (int j = 2; j < COMMAND_MAP_WIDTH - 2; j++) {
			position.row = MAP_HEIGHT + 1 + i;
			position.column = MAP_WIDTH + j;
			gotoxy(position);
			printf(" ");
		}
	}
}

void project_sys(char src[SYS_MAP_HEIGHT][SYS_MAP_WIDTH], char dest[SYS_MAP_HEIGHT][SYS_MAP_WIDTH]) {
	for (int i = 0; i < SYS_MAP_HEIGHT; i++) {
		for (int j = 0; j < SYS_MAP_WIDTH; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

void display_sys(char sys_array[SYS_MAP_HEIGHT][SYS_MAP_WIDTH]) {
	project_sys(sys_array, sys_back);

	for (int i = 0; i < SYS_MAP_HEIGHT; i++) {
		for (int j = 0; j < SYS_MAP_WIDTH; j++) {
			if (sys_front[i][j] != sys_back[i][j]) {
				POSITION pos = { 2 + MAP_HEIGHT + i, j };
				printc(pos, sys_back[i][j], COLOR_DEFAULT);
			}
			sys_front[i][j] = sys_back[i][j];
		}
	}
}

void project_command(char src[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH], char dest[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH]) {
	for (int i = 0; i < COMMAND_MAP_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_MAP_WIDTH; j++) {
			dest[i][j] = src[i][j];
		}
	}
}

void display_command(char command_array[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH]) {
	project_command(command_array, command_back);

	for (int i = 0; i < COMMAND_MAP_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_MAP_WIDTH; j++) {
			if (command_front[i][j] != command_back[i][j]) {
				POSITION pos = { 2 + MAP_HEIGHT + i, MAP_WIDTH + 1 + j };
				printc(pos, command_back[i][j], COLOR_DEFAULT);
			}
			command_front[i][j] = command_back[i][j];
		}
	}
}

int get_color_for_char(char ch, POSITION cursor_pos) {
	// MAP_WIDTH�� �ݰ� ���
	int mid_column = MAP_WIDTH / 2;

	// 'B'�� 'H'�� ���ؼ� ���� �ٸ��� ����
	
	if (ch == 'H') {
		if (cursor_pos.column <= mid_column) {
			return COLOR_Atreides;
		}
		else {
			return COLOR_Harkonnen;
		}
	}
	

	if (ch == 'B') {
		if (cursor_pos.column <= mid_column) {
			// ���� �ݿ��� ���� 79 (��: Atreides Base)
			return COLOR_Atreides;  // �Ķ��� ���, ��� ���� (�Ǵ� ���ϴ� ��)
		}
		else {
			// ������ �ݿ��� ���� 23 (��: Harkonnen Base)
			return COLOR_Harkonnen;  // ������ ���, ��� ���� (�Ǵ� ���ϴ� ��)
		}
	}

	// ������ ���ڿ� ���� ���� ����
	switch (ch) {
	case 'P': return COLOR_P;    // Plate
	case 'S': return COLOR_SPICE;   // Spice Field
	case 'R': return COLOR_ROCK;  // Rock
	case 'W': return COLOR_WORM;  // Sand Worm
	case ' ': return COLOR_DEFAULT + 220;  // �� ����
	default: return COLOR_DEFAULT;  // ��Ÿ ����
	}
}

// �ý��� �޽��� â�� �� ���� �ֱٲ� �� �Ʒ� �����Ȱ��� ����� �ڵ�
void sys_message_clear(void) {
	POSITION pos = { MAP_HEIGHT + 4, 2 };
	
}