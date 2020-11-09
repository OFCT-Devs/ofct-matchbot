#ifndef OFCT_IRCBOT_MATCH_H
#define OFCT_IRCBOT_MATCH_H

#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include <threads.h>

#include "typedef.h"

int match(void *__session);

#endif