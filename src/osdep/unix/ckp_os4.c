/*
 * Program:	OSF/1 (Digital UNIX) 4 check password
 *
 * Author:	Mark Crispin
 *		Networks and Distributed Computing
 *		Computing & Communications
 *		University of Washington
 *		Administration Building, AG-44
 *		Seattle, WA  98195
 *		Internet: MRC@CAC.Washington.EDU
 *
 * Date:	1 August 1988
 * Last Edited:	13 September 1999
 *
 * Copyright 1999 by the University of Washington
 *
 *  Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both the
 * above copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the University of Washington not be
 * used in advertising or publicity pertaining to distribution of the software
 * without specific, written prior permission.  This software is made available
 * "as is", and
 * THE UNIVERSITY OF WASHINGTON DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED,
 * WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT LIMITATION ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, AND IN
 * NO EVENT SHALL THE UNIVERSITY OF WASHINGTON BE LIABLE FOR ANY SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, TORT
 * (INCLUDING NEGLIGENCE) OR STRICT LIABILITY, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

/* Dummy collection routine
 * Accepts: how long to wait for user
 *	    how to run parameter collection
 *	    title
 *	    number of prompts
 *	    prompts
 * Returns: collection status
 *
 * Because Spider Boardman, who wrote SIA, says that it's needed for buggy SIA
 * mechanisms, that's why.
 */

static int checkpw_collect (int timeout,int rendition,uchar_t *title,
			    int nprompts,prompt_t *prompts)
{
  switch (rendition) {
  case SIAONELINER: case SIAINFO: case SIAWARNING: return SIACOLSUCCESS;
  }
  return SIACOLABORT;		/* another else is bogus */
}


/* Check password
 * Accepts: login passwd struct
 *	    password string
 *	    argument count
 *	    argument vector
 * Returns: passwd struct if password validated, NIL otherwise
 */

struct passwd *checkpw (struct passwd *pw,char *pass,int argc,char *argv[])
{
  int i;
  char *s;
  char *name = cpystr (pw->pw_name);
  char *host = cpystr (tcp_clienthost ());
  struct passwd *ret = NIL;
				/* tie off address */
  if (s = strchr (host,' ')) *s = '\0';
  if (*host == '[') {		/* convert [a.b.c.d] to a.b.c.d */
    memmove (host,host+1,i = strlen (host + 2));
    host[i] = '\0';
  }
				/* validate password */
  if (sia_validate_user (checkpw_collect,argc,argv,host,name,NIL,NIL,NIL,pass)
      == SIASUCCESS) ret = getpwnam (name);
  fs_give ((void **) &name);
  fs_give ((void **) &host);
  return ret;
}
