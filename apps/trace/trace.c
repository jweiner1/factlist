/*
 * trace.c
 *
 *  Created on: Nov 16, 2014
 *      Author: jweiner
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <fcntl.h>

#include "trace.h"

#define RECV_BUFFER_SIZE	10 * 1024	// 10k

typedef enum {
	ANSI_COLOR_RED = 0,
	ANSI_COLOR_GREEN,
	ANSI_COLOR_YELLOW,
	ANSI_COLOR_BLUE,
	ANSI_COLOR_MAGENTA,
	ANSI_COLOR_CYAN,
	ANSI_COLOR_WHITE,
	ANSI_COLOR_RESET,
} color_enums;

const char *szColors[]={
		"\x1b[31m",
		"\x1b[32m",
		"\x1b[33m",
		"\x1b[34m",
		"\x1b[35m",
		"\x1b[36m",
		"\x1b[37m",
		"\x1b[0m",
};

static int init_apps(trace_app_t * trace)
{
	int err;

	fprintf(stdout, " = Build date: %s - %s =\n", __TIME__, __DATE__);
	trace->num_apps = get_num_apps();

	trace->apps = calloc(trace->num_apps, sizeof(app_t));
	if (!trace->apps) {
		fprintf(stderr, "Memory for trace app array could not be allocated (%d):%s\n", errno, strerror(errno));
		return -1;
	}

	err = get_apps(trace->apps, trace->num_apps);
	if (err) {
		fprintf(stderr, "Unable to retrieve app list from utils file\n");
		return -1;
	}

	return 0;
}

static int init_levels(trace_app_t * trace)
{
	int err;

	trace->num_levels = get_num_levels();

	trace->levels = calloc(trace->num_levels, sizeof(level_t));

	err = get_levels(trace->levels, trace->num_levels);
	if (err) {
		fprintf(stderr, "Unable to retrieve level list from utils file\n");
		return -1;
	}

	return 0;
}

static int init_recv_sockets(trace_app_t *trace)
{
	app_t *app = trace->trace_app;
	struct sockaddr_in recv_addr;

	app->socket = socket(AF_INET, SOCK_DGRAM, 0);
	if (app->socket < 0) {
		fprintf(stderr, "Error initializing socket (%d):%s\n", errno, strerror(errno));
		return -1;
	}

	fprintf(stdout, "Socket initialized\n");

	recv_addr.sin_family = AF_INET;
	if (trace->b_local_trace)
		recv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	else
		recv_addr.sin_addr.s_addr = inet_addr("10.0.0.53");
	recv_addr.sin_port = htons(app->port_num);

	if (bind(app->socket, (struct sockaddr*)&recv_addr, sizeof(recv_addr)) < 0){
		fprintf(stderr, "Socket bind failed (%d):%s\n", errno, strerror(errno));
		return -1;
	}

	fprintf(stdout, "Socket bound\n");

	return 0;
}

int parse_args(trace_app_t *trace, int argc, char **argv)
{
	int i;

	if (argc < 3) {
		return -1;
	}
	else {
		char *app = argv[1];
		char *level = argv[2];

		for (i = 0; i < trace->num_apps; i++)
			if (strcasecmp(app, trace->apps[i].app_name) == 0)
				trace->trace_app = &trace->apps[i];

		for (i = 0; i < trace->num_levels; i ++)
			if (strcasecmp(level, trace->levels[i].level) == 0)
				trace->trace_level = &trace->levels[i];

		if (!trace->trace_app) {
			fprintf(stderr, "Could not find matching app\n");
			return -1;
		}

		if (!trace->trace_level) {
			fprintf(stderr, "Could not find matching level\n");
			return -1;
		}
	}
	if (argc == 4) {
		if (strncasecmp(argv[3], "local", 5) == 0){
			fprintf(stdout, "Setting up trace of localhost\n");
			trace->b_local_trace = 1;
		}
	}

	return 0;
}

void show_usage(trace_app_t *trace)
{
	int i;

	printf("trace <app> <lvl> [local]\n");
	printf(" apps:\n");
	for (i = 0; i < trace->num_apps; i++ ) {
		printf("  %s\n", trace->apps[i].app_name);
	}
	printf(" levels:\n");
	for (i = 0; i < trace->num_levels; i++ ) {
		printf("  %s\n", trace->levels[i].level);
	}
}

void parse_trace(trace_app_t *trace, const char *buffer, level_t **trace_level)
{
	char *parsed_level;
	int i;
	char *buffer_shadow;
	size_t len;

	len = strlen(buffer);
	buffer_shadow = malloc(len);
	memcpy(buffer_shadow, buffer, len);

	strtok(buffer_shadow, ":");
	strtok(0, ":");
	strtok(0, ":");
	strtok(0, ":");
	parsed_level = strtok(0, ":");

	for (i = 0; i < trace->num_levels; i++) {
		if (strcasecmp(parsed_level, trace->levels[i].level) == 0)
			*trace_level = &trace->levels[i];
	}
}

int main(int argc, char **argv) {
	trace_app_t trace;
	int err;
	char buffer[RECV_BUFFER_SIZE];

	err = init_apps(&trace);
	if (err) {
		fprintf(stderr, "Unable to initialize app list\n");
		return -1;
	}

	err = init_levels(&trace);
	if (err) {
		fprintf(stderr, "Unable to initialize level list\n");
		return -1;
	}

	err = parse_args(&trace, argc, argv);
	if (err) {
		show_usage(&trace);
		return -1;
	}

	err = init_recv_sockets(&trace);
	if (err) {
		fprintf(stderr, "Unable to initialize sockets\n");
		return -1;
	}

	while (1) {
		size_t recv_len;
		level_t *rcvd_level;
		const char *p_szColor;

		memset(buffer, 0x00, sizeof(buffer));

		recv_len = recv(trace.trace_app->socket, buffer, sizeof(buffer), 0);
		if (recv_len < 0) {
			fprintf(stderr, "Socket receive failed (%d):%s\n", errno, strerror(errno));
			return -1;
		}

		parse_trace(&trace, buffer, &rcvd_level);
		if (rcvd_level->val > trace.trace_level->val)
			continue;
		p_szColor = szColors[ANSI_COLOR_RESET];
		if (rcvd_level->val == get_level_val("error", trace.levels, trace.num_levels)){
			p_szColor = szColors[ANSI_COLOR_RED];
		}
		else if (rcvd_level->val == get_level_val("warn", trace.levels, trace.num_levels)){
			p_szColor = szColors[ANSI_COLOR_YELLOW];
		}
		else if (rcvd_level->val == get_level_val("info", trace.levels, trace.num_levels)){
			p_szColor = szColors[ANSI_COLOR_WHITE];
		}
		fprintf(stdout, "%s - %s%s%s\n", trace.b_local_trace?"L":"R", p_szColor, buffer, szColors[ANSI_COLOR_RESET]);
	}
	return 0;
}
