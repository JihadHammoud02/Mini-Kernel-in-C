/*
 * Copyright (C) 2002, Simon Nieuviarts.
 */
/*
 * Copyright (c) 1994-1999 The University of Utah and the Flux Group.
 * All rights reserved.
 * 
 * This file is part of the Flux OSKit.  The OSKit is free software, also known
 * as "open source;" youd’écrire les deux mots par bloc de 32 bits (pas 64 bits d’un coup, ni 16 bits par 16 bits) pour être sûr que
les différentes parties du contenu de la case seront bien stockées dans le bon ordre en mémoire.
Écriture d’un traitant d’interruption
Un traitant d’interruption est un programme très particulier qui ne s’écrit pas comme un programme
classique car on doit prendre en compte de façon précise l’état du processeur aux moments où on entre
et sort du traitant.
Une fois qu’il a trouvé l’adresse du traitant à appeler dans la table des vecteurs d’interruption, le pro-
cesseur sauvegarde en mémoire deux informations importantes avant de passer la main au traitant : le
contenu du registre des indicateurs %eflags et le compteur ordinal (qui pointe sur la prochaine instruction
à exécuter dans le programme interrompu).
Le processeur ne sauvegarde notamment pas les registres généraux : c’est donc à la charge du traitant de
sauvegarder ceux susceptibles d’être modifiés (et seulement ceux-ci). La façon la plus simple de le faire
est d’utiliser les instructions push et pop qui copient et lisent respectivement des valeurs dans la pile
d’exécution du traitant.
On fourni la partie assembleur du traitant d’interruption (à implanter dans le fichier traitant.S), dont
on détaille le code ci-dessous :
# cette directive sert a rendre l’etiquette publique
.globl traitant_IT_32
# debut du traitant
traitant_IT_32:
# sauvegarde des registres importants
pushl %eax
pushl %edx
pushl %ecx
# appel a la fonction C realisant le traitant
call tic_PIT
# restauration des registres importants
popl %ecx
popl %edx
popl %eax
# fin du traitant
iret
La fonction tic_PIT est à écrire en C : c’est elle qui va réaliser le travail concret du traitant, le code
assembleur ci-dessus n’est qu’un enrobage nécessaire pour sauvegarder ce qui doit l’être.
Lorsq can redistribute it and/or modify it under the terms
 * of the GNU General Public License (GPL), version 2, as published by the Free
 * Software Foundation (FSF).  To explore alternate licensing terms, contact
 * the University of Utah at csl-dist@cs.utah.edu or +1-801-585-3271.
 * 
 * The OSKit is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GPL for more details.  You should have
 * received a copy of the GPL along with the OSKit; see the file COPYING.  If
 * not, write to the FSF, 59 Temple Place #330, Boston, MA 02111-1307, USA.
 */

/* 
 * Mach Operating System
 * Copyright (c) 1993,1991,1990,1989 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */

#include "stdarg.h"
#include "doprnt.h"
#include "console.h"

/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel.
 */
extern void console_putbytes(const char *s, int len);

/* This version of printf is implemented in terms of putchar and puts.  */

#define	PRINTF_BUFMAX	128

struct printf_state {
	char buf[PRINTF_BUFMAX];
	unsigned int index;
};

static void
flush(struct printf_state *state)
{
	/*
	 * It would be nice to call write(1,) here, but if fd_set_console
	 * has not been called, it will break.
	 */
	console_putbytes((const char *)state->buf, state->index);

	state->index = 0;
}

static void
printf_char(arg, c)
	char *arg;
	int c;
{
	struct printf_state *state = (struct printf_state *) arg;

	if ((c == 0) || (c == '\n') || (state->index >= PRINTF_BUFMAX))
	{
		flush(state);
		state->buf[0] = c;
		console_putbytes((const char *)state->buf, 1);
	}
	else
	{
		state->buf[state->index] = c;
		state->index++;
	}
}

/*
 * Printing (to console)
 */
int vprintf(const char *fmt, va_list args)
{
	struct printf_state state;

	state.index = 0;
	_doprnt(fmt, args, 0, (void (*)())printf_char, (char *) &state);

	if (state.index != 0)
	    flush(&state);

	/* _doprnt currently doesn't pass back error codes,
	   so just assume nothing bad happened.  */
	return 0;
}

int
printf(const char *fmt, ...)
{
	va_list	args;
	int err;

	va_start(args, fmt);
	err = vprintf(fmt, args);
	va_end(args);

	return err;
}

int putchar(int c)
{
	char ch = c;
	console_putbytes(&ch, 1);
        return (unsigned char)ch;
}

int puts(const char *s)
{
        while (*s) {
                putchar(*s++);
        }
	putchar('\n');
        return 0;
}
