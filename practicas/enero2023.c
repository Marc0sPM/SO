#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>


struct task tasks_static[MAXPROC] = {
// 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
{.valid = 1, .next = {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .id
= 0}, // 0: 1 2 3
{.valid = 1, .next = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .id
= 1}, // 1: 4
{.valid = 1, .next = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .id
= 2}, // 2: 5
{.valid = 1, .next = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .id
= 3}, // 3: 5
{.valid = 1, .next = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .id
= 4}, // 4:
{.valid = 1, .next = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, .id
= 5}, // 5: 4
0
};