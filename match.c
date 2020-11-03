#include "match.h"
#include "typedef.h"

#include <unistd.h>

#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include <stdio.h>


void match(irc_session_t *session) {
    printf("NOW MATCH BEGINS!\n");
    irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
    printf("%s\n", ctx->match_info.game_title);
    sleep(5);
    printf("MATCH HAS ENDED!\n");
}