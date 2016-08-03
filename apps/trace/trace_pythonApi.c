/*
 * trace_pythonApi.c
 *
 *  Created on: Nov 16, 2014
 *      Author: jweiner
 */

#include <stdio.h>
#include <string.h>

#include "trace.h"

int get_num_apps()
{
	int num_apps;
	FILE *fp = popen("python " PYTHON_INTERFACE_APP " num_apps", "r");

	fscanf(fp, "%d", &num_apps);
	pclose(fp);

	return num_apps;
}

int get_num_levels()
{
	int num_levels;
	FILE *fp = popen("python " PYTHON_INTERFACE_APP " num_levels", "r");

	fscanf(fp, "%d", &num_levels);
	pclose(fp);

	return num_levels;
}

static int get_base_port()
{
	int base_port;
	FILE *fp = popen("python " PYTHON_INTERFACE_APP " base_port", "r");

	fscanf(fp, "%d", &base_port);
	pclose(fp);

	return base_port;
}

int get_apps(app_t * app, int num_apps){
	FILE *fp = popen("python " PYTHON_INTERFACE_APP " apps", "r");
	char *line = 0;
	size_t len = 0;
	ssize_t read_len;
	int app_idx, base_port;

	if (!app) {
		fprintf(stderr, "NULL app pointer\n");
		return -1;
	}

	base_port = get_base_port();

	for (app_idx = 0; app_idx < num_apps; app_idx++) {
		read_len = getline(&line, &len, fp);
		if (read_len < 0) {
			fprintf(stderr, "Error parsing app name from python api\n");
			return -1;
		}
		line[read_len-1] = 0;
		memcpy(app[app_idx].app_name, line, sizeof(app[app_idx].app_name));
		app[app_idx].port_num = base_port + app_idx;
	}

	return 0;
}

int get_levels(level_t * level, int num_levels){
	FILE *fp = popen("python " PYTHON_INTERFACE_APP " levels", "r");
	char *line = 0;
	size_t len = 0;
	ssize_t read_len;
	int level_idx;

	if (!level) {
		fprintf(stderr, "NULL level pointer\n");
		return -1;
	}

	for (level_idx = 0; level_idx < num_levels; level_idx++) {
		read_len = getline(&line, &len, fp);
		if (read_len < 0) {
			fprintf(stderr, "Error parsing level name from python api\n");
			return -1;
		}
		line[read_len-1] = 0;
		memcpy(level[level_idx].level, line, sizeof(level[level_idx].level));
		level[level_idx].val = level_idx;
	}

	return 0;
}

int get_level_val(const char *str_level, level_t * levels, int num_levels) {
	int i, val = -1;

	for (i = 0; i < num_levels; i++) {
		if (strcasecmp(levels[i].level, str_level) == 0){
			val = i;
			break;
		}
	}

	return val;
}
