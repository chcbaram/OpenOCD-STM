/***************************************************************************
 *   Copyright (C) 2005 by Dominic Rath                                    *
 *   Dominic.Rath@gmx.de                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef COMMAND_H
#define COMMAND_H

#include <stdio.h>

enum command_mode
{
	COMMAND_EXEC,
	COMMAND_CONFIG,
	COMMAND_ANY,
};

typedef struct command_context_s
{
	enum command_mode mode;
	struct command_s *commands;
	int current_target;
	/* Execute a command.
	 * 
	 * If the command fails, it *MUST* return a value != ERROR_OK
	 * (many commands break this rule, patches welcome!)
	 * 
	 * This is *especially* important for commands such as writing
	 * to flash or verifying memory. The reason is that those commands
	 * can be used by programs to determine if the operation succeded
	 * or not. If the operation failed, then a program can try
	 * an alternative approach.
	 * 
	 * Returning ERROR_COMMAND_SYNTAX_ERROR will have the effect of
	 * printing out the syntax of the command.
	 */
	int (*output_handler)(struct command_context_s *context, char* line);
	void *output_handler_priv;
} command_context_t;

typedef struct command_s
{
	char *name;
	struct command_s *parent;
	struct command_s *children;
	int (*handler)(struct command_context_s *context, char* name, char** args, int argc);
	enum command_mode mode;
	char *help;
	int unique_len;
	struct command_s *next;
} command_t;

extern command_t* register_command(command_context_t *context, command_t *parent, char *name, int (*handler)(struct command_context_s *context, char* name, char** args, int argc), enum command_mode mode, char *help);
extern int unregister_command(command_context_t *context, char *name);
extern int unregister_all_commands(command_context_t *context);
extern void command_set_output_handler(command_context_t* context, int (*output_handler)(struct command_context_s *context, char* line), void *priv);
extern command_context_t* copy_command_context(command_context_t* context);
extern command_context_t* command_init();
extern int command_done(command_context_t *context);
extern void command_print(command_context_t *context, char *format, ...);
extern void command_print_sameline(command_context_t *context, char *format, ...);
extern int command_run_line(command_context_t *context, char *line);
extern int command_run_linef(command_context_t *context, char *format, ...);
extern int command_run_line_internal(command_context_t *context, char *line);
extern int command_run_file(command_context_t *context, FILE *file, enum command_mode mode);


#define		ERROR_COMMAND_CLOSE_CONNECTION		(-600)
#define		ERROR_COMMAND_SYNTAX_ERROR			(-601)


extern int fast_and_dangerous;

#endif /* COMMAND_H */
