/*
 * trace.h
 *
 *  Created on: Nov 16, 2014
 *      Author: jweiner
 */

#ifndef TRACE_H_
#define TRACE_H_

#define PYTHON_INTERFACE_APP "logs/gettrace.py"

#include <stdint.h>
#include <sys/select.h>

typedef struct {
	char app_name[128];
	int port_num;
	int socket;
	int default_level;
} app_t;

typedef struct {
	char level[128];
	int val;
} level_t;

typedef struct {
	app_t *apps;
	int num_apps;
	level_t *levels;
	int num_levels;
	fd_set recv_fd_set;
	int max_fd;
	int num_traces;
	app_t *trace_app;
	level_t *trace_level;
	uint8_t b_local_trace;
} trace_app_t;

int get_num_apps(void);
int get_num_levels(void);
int get_apps(app_t * app, int num_apps);
int get_levels(level_t * level, int num_levels);
int get_level_val(const char *str_level, level_t * levels, int num_levels);


#endif /* TRACE_H_ */
