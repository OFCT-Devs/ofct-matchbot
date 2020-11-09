#include "match.h"
#include "typedef.h"

#include <unistd.h>

#include <libircclient.h>
#include <libirc_rfcnumeric.h>
#include <stdio.h>


int match(void *__session) {
    irc_session_t *session = (irc_session_t*)__session;
    printf("NOW MATCH BEGINS!\n");
    irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
    printf("%s\n", ctx->match_info.game_title);

    char const *channel = ctx->match_info.match_channel;
printf("channel = %s\n", channel);
/*
    irc_cmd_msg(session, channel, "!mp invite A BCDe");
    //irc_cmd_msg(session, channel, "!mp invite ");
    irc_cmd_msg(session, channel, "!mp map 1775314");
    sleep(60);
    irc_cmd_msg(session, channel, "!mp start 10");
    sleep(300);
*/
    if(irc_cmd_msg(session, channel, "Sleeping for 10 seconds...")) {
        int err = irc_errno(session);
        printf("error code %d: %s\n", err, irc_strerror(err));
        printf("Sending message \"%s\" failed!\n", "!mp close");
        irc_disconnect(session);
        return -1;
    }
    printf("Sent message \"Sleeping for 10 seconds...\"\n");
    sleep(10);
    irc_cmd_msg(session, channel, "10 seconds passed!");
    printf("Sent message \"10 seconds passed!\"\n");

    irc_cmd_msg(session, channel, "!roll");
    printf("rolled!\n");
    sleep(60);

    printf("MATCH HAS ENDED!\n");

    printf("Now closing the channel...\n");
    if(irc_cmd_msg(session, ctx->match_info.match_channel, "!mp close")) {
        int err = irc_errno(session);
        printf("error code %d: %s\n", err, irc_strerror(err));
        printf("Sending message \"%s\" failed!\n", "!mp close");
        irc_disconnect(session);
        return 0;
    }
}