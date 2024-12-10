/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
#define COLOR_DEFAULT	15
#define COLOR_CURSOR	15
#define COLOR_RESOURCE  112
#define COLOR_Harkonnen	79	// ���ڳ� ��
#define	COLOR_Atreides	23	// ��Ʈ���̵��� ��
#define	COLOR_P			7	// Plate
#define COLOR_SPICE		71	// Spice Field
#define COLOR_ROCK		135	// Rock
#define COLOR_WORM		103	// Sand Worm

// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	CURSOR cursor,
	char status_array[STATUS_MAP_HEIGHT][STATUS_MAP_WIDTH],
	char sys_array[SYS_MAP_HEIGHT][SYS_MAP_WIDTH],
	char command_array[COMMAND_MAP_HEIGHT][COMMAND_MAP_WIDTH]
);

int get_color_for_char(char ch, POSITION cursor_pos);  // ���ڿ� ���� ���� ��ȯ �Լ� ����
void clear_status();
void clear_command();
void sys_message_clear();

#endif
