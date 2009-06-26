/*
 * Copyright (C) 2009 Niek Linnenbank
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <API/ProcessCtl.h>
#include <Macros.h>
#include <Config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    struct stat st;
    const char *progs[] =
    {
	"/img/srv/filesystem/proc/procfs",
	"/img/srv/serial/serial",
	"/img/srv/pci/pci",
	"/img/bin/sh/sh",
    };

    /*
     * TODO: give up all priviledges: run us in priviledge level 0.
     */

    /* Wait for filesystem image. */
    while (stat("/img", &st) == -1)
    {
	ProcessCtl(ANY, Schedule);
    }
    /* Initialize terminal as standard I/O. */
    for (int i = 0; i < 3; i++)
    {
        while (open("/dev/tty0", ZERO) == -1) ;
    }
    /* Temporarily start a hardcoded list of drivers. */
    for (int i = 0; i < 4; i++)
    {
	const char *av[] = { progs[i], 0 };
	forkexec(progs[i], av);
    }
    /* Exit immediately. */
    return EXIT_SUCCESS;
}
