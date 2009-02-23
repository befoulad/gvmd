/* Test 0 of OMP START_TASK.
 * $Id$
 * Description: Test the OMP START_TASK command.
 *
 * Authors:
 * Matthew Mundell <matt@mundell.ukfsn.org>
 *
 * Copyright:
 * Copyright (C) 2009 Intevation GmbH
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * or, at your option, any later version as published by the Free
 * Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#define TRACE 0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "../tracef.h"

int
main ()
{
  int socket;
  gnutls_session_t session;
  unsigned int id;

  socket = connect_to_manager (&session);
  if (socket == -1) return EXIT_FAILURE;

  if (authenticate (&session, "mattm", "mattm")) goto fail;

  /* Create a task. */

  if (create_task_from_rc_file (&session,
                                "new_task_empty_rc",
                                "Task for omp_start_task_0",
                                "Test omp_start_task_0 task.",
                                &id))
    goto fail;

  /* Start the task. */

#if 0
  if (authenticate (&session, "mattm", "mattm")) goto fail;
#endif

  gchar* msg = g_strdup_printf ("<start_task>"
                                "<task_id>%u</task_id>"
                                "</start_task>",
                                id);
  int ret = send_to_manager (&session, msg);
  g_free (msg);
  if (ret == -1)
    goto fail;

  /* Read the response. */

  entity_t entity = NULL;
  read_entity (&session, &entity);

  /* Compare response to expected response. */

  entity_t expected = add_entity (NULL, "start_task_response", NULL);
  add_entity (&expected->entities, "status", "201");

  if (compare_entities (entity, expected))
    {
      free_entity (expected);
      free_entity (entity);
 fail:
      close_manager_connection (socket, session);
      return EXIT_FAILURE;
    }

  free_entity (expected);
  free_entity (entity);
  close_manager_connection (socket, session);
  return EXIT_SUCCESS;
}
