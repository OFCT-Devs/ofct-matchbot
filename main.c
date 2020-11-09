#include <libircclient.h>
#include <libirc_rfcnumeric.h>

#include <stdio.h>
#include <string.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

#include "typedef.h"
#include "match.h"

/* There should be some unnecesary events;
   Will handle them when it is really not needed. */

static void event_connect(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_nick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_quit(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_join(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_part(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_mode(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_umode(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_topic(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_kick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_channel(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_privmsg(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_notice(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_channel_notice(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_invite(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_ctcp_req(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_ctcp_rep(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_ctcp_action(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_unknown(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_numeric(irc_session_t *session, unsigned int event, char const *origin, char const **params, unsigned int count);
static void event_dcc_chat_req(irc_session_t *session, char const *nick, char const *addr, irc_dcc_t dccid);
static void event_dcc_send_req(irc_session_t *session, char const *nick, char const *addr, char const *filename, unsigned long size, irc_dcc_t dccid);

static void dcc_callback(irc_session_t *session, irc_dcc_t id, int status, void *ctx, char const *data, unsigned int length);

/****************************************************************************************************************************************************/

static void get_user_info(user_info_t *const user);
static void free_user_info(user_info_t *const user);

static void free_ctx(irc_ctx_t *ctx);

/* osu! specific functions */
static void *banchobot_message_parse(irc_session_t *session, char const *message);
//static int free_parsed_message(void *message);

static char *init_channel = "#lobby";

int main() {
    irc_callbacks_t callbacks;
    irc_ctx_t ctx;
    user_info_t user;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.event_connect = event_connect;
    callbacks.event_nick = event_nick;
    callbacks.event_quit = event_quit;
    callbacks.event_join = event_join;
    callbacks.event_part = event_part;
    callbacks.event_mode = event_mode;
    callbacks.event_umode = event_umode;
    callbacks.event_topic = event_topic;
    callbacks.event_kick = event_kick;
    callbacks.event_channel = event_channel;
    callbacks.event_privmsg = event_privmsg;
    callbacks.event_notice = event_notice;
    callbacks.event_channel_notice = event_channel_notice;
    callbacks.event_invite = event_invite;
    callbacks.event_ctcp_req = event_ctcp_req;
    callbacks.event_ctcp_rep = event_ctcp_rep;
    callbacks.event_ctcp_action = event_ctcp_action;
    callbacks.event_unknown = event_unknown;
    callbacks.event_numeric = event_numeric;
    callbacks.event_dcc_chat_req = event_dcc_chat_req;
    callbacks.event_dcc_send_req = event_dcc_send_req;

    get_user_info(&user);

    // initialize ctx
    memset(&ctx, 0, sizeof(ctx));
    ctx.match_info.game_title = "TEST";
    ctx.user = (char*)malloc(sizeof(char) * (strlen(user.id) + 1));
    strcpy(ctx.user, user.id);

    irc_session_t *session = irc_create_session(&callbacks);

    if(!session) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        return err;
    }
    printf("Hello, world!\n");

    irc_set_ctx(session, &ctx);
    irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);

    if(irc_connect(session, "irc.ppy.sh", 6667, user.pwd, user.id, user.id, user.id)) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        return err;
    }

    free_user_info(&user);

    if(irc_run(session)) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
    }

    printf("Goodbye, world!\n");
    return 0;
}

static void event_connect(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("Connected to server!\n");
    irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
    if(ctx->channel == NULL) {
        printf("Trying to connect to %s...\n", init_channel);
        if(irc_cmd_join(session, init_channel, NULL)) {
            int err = irc_errno(session);
            fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
            irc_disconnect(session);
            return;
        }
    }
}

static void event_nick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s changed to %s!\n", origin, params[0]);
}

static void event_quit(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    //printf("%s has quit due to %s.\n", origin, params[0]);
}

static void event_join(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
    if(strcmp(origin, ctx->user) == 0) {

        /* Before making any channel, the bot first joins the #lobby. */
        if(strcmp(params[0], init_channel) == 0) {
            printf("%s has joined the channel %s\n", origin, params[0]);
            char make_game[512]; // = "/join #mp_69170086";
            sprintf(make_game, "!mp make %s", ctx->match_info.game_title); // currently has title of the match
            if(irc_cmd_msg(session, params[0], make_game)) {
                int err = irc_errno(session);
                printf("error code %d: %s\n", err, irc_strerror(err));
                printf("Sending message \"%s\" failed!\n", make_game);
                irc_disconnect(session);
                return;
            }
        }

        /* There may be some cases when the setting is not complete. */
        else if(ctx->match_info.match_channel == NULL) {
            printf("The channel is %s.\n", params[0]);
            printf("Match is not set properly yet.\n");
            printf("This may be a part of a progress.\n");
            printf("Leaving the channel.\n");
            if(irc_cmd_part(session, params[0])) {
                int err = irc_errno(session);
                fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
                irc_disconnect(session);
                return;
            }
        }

        /* You are joining to the match you have made! */
        else if(strcmp(params[0], ctx->match_info.match_channel) == 0) {
            printf("Successfully joined to the match channel %s!\n", ctx->match_info.match_channel);

            thrd_t match_thread;
            
            switch(thrd_create(&match_thread, match, session)) {
                case thrd_success: {
                    thrd_detach(match_thread);
                    printf("Thread started running in the other thread!\n");
                } return;
                case thrd_nomem: {
                    fprintf(stderr, "Not enough memory to run the match!\n");
                } break;
                case thrd_error: {
                    fprintf(stderr, "Thread error!\n");
                } break;
                default: {
                    fprintf(stderr, "Unknown error: Faulty return of thrd_create().\n");
                    exit(1);
                }
            }

            //match(session);

            printf("Closing the channel due to error...\n");
            if(irc_cmd_msg(session, ctx->match_info.match_channel, "!mp close")) {
                int err = irc_errno(session);
                printf("error code %d: %s\n", err, irc_strerror(err));
                printf("Sending message \"%s\" failed!\n", "!mp close");
                irc_disconnect(session);
                return;
            }
        }

        /* You are not supposed to be in this channel. */
        else {
            printf("Someone called to the channel %s, but you are not supposed to be here.\n", params[0]);
            if(irc_cmd_part(session, params[0])) {
                int err = irc_errno(session);
                fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
                irc_disconnect(session);
                return;
            }
        }
    }
}

static void event_part(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    //printf("%s has left the channel %s.\n", origin, params[0]);
}

static void event_mode(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s has changed the mode of the channel %s to %s.\n", origin, params[0], params[1]);
}

static void event_umode(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s has changed your user mode in channel %s to %s.\n", origin, params[0], params[1]);
}

static void event_topic(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 2) {
        printf("%s has changed the topic of channel %s to %s.\n", origin, params[0], params[1]);
    }
    else {
        printf("%s has changed the topic of channel %s.\n", origin, params[0]);
    }
}

static void event_kick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has kicked from channel %s.\n", origin, params[0]);
    }
    else if(count == 2) {
        printf("%s has kicked %s from channel %s.\n", origin, params[1], params[0]);
    }
    else {
        printf("%s has kicked %s from channel %s:\n%s\n", origin, params[1], params[0], params[2]);
    }
}

static void event_channel(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has sent a message to channel %s.\n", origin, params[0]);
    }
    else {
        irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
        if(strcmp(params[0], init_channel) != 0) {
            printf("%s has sent a message to channel %s:\n%s\n", origin, params[0], params[1]);
            if(strcmp(origin, "BanchoBot") == 0 && strcmp(params[0], ctx->match_info.match_channel) == 0 && strcmp(params[1], "Closed the match") == 0) {
                printf("Closed!\n");
                free_ctx(ctx);
                irc_disconnect(session);
            }
        }
    }
}

static void event_privmsg(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has sent you a private message.", origin);
    }
    else {
        printf("%s has sent you a private message:\n%s\n", origin, params[1]);
        if(strcmp(origin, "BanchoBot") == 0) {
            void *message = banchobot_message_parse(session, params[1]);
            switch((banchobot_replies)((char**)message)[0]) {

                /* Got a message by BanchoBot:
                   Created the tournament match https://osu.ppy.sh/mp/NUMBER GAME_TITLE
                   
                   char **message has:
                   [ (char*)BANCHOBOT_MULTI_CREATED | (char*)NUMBER | (char*)GAME_TITLE ] */
                case BANCHOBOT_MULTI_CREATED: {
                    irc_ctx_t *ctx = irc_get_ctx(session);

                    ctx->match_info.match_channel = (char*)malloc(sizeof(char) * (strlen(((char**)message)[1]) + 6));
                    sprintf(ctx->match_info.match_channel, "#mp_%s", ((char**)message)[1]);
                    ctx->match_info.game_title = ((char**)message)[2];

                    printf("match channel : %s\n", ctx->match_info.match_channel);
                    printf("game title : %s\n", ctx->match_info.game_title);

                    // freeing unnecessary memory
                    free(((char**)message)[1]);
                    free(message);

                    if(irc_cmd_join(session, ctx->match_info.match_channel, NULL)) {
                        int err = irc_errno(session);
                        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
                        irc_disconnect(session);
                    }
                } break;

                default: {
                    printf("Not yet implemented, or invalid value!");
                    free(message);
                    irc_disconnect(session);
                }
            }
        }
    }
}

static void event_notice(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has sent a notice to %s.\n", origin, params[0]);
    }
    else {
        printf("%s has sent a notice to %s:\n%s\n", origin, params[0], params[1]);
    }
}

static void event_channel_notice(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has sent a notice to channel %s.\n", origin, params[0]);
    }
    else {
        printf("%s has sent a notice to %s:\n%s\n", origin, params[0], params[1]);
    }
}

static void event_invite(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has sent you an invite.\n", origin);
    }
    else {
        printf("%s has sent you an invite:\n%s\n", origin, params[1]);
    }
}

static void event_ctcp_req(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s generated a CTCP message:\n%s\n", origin, params[0]);
}

static void event_ctcp_rep(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s has generated and CTCP response:\n%s\n", origin, params[0]);
}

static void event_ctcp_action(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s: %s\n", origin, params[0]);
}

static void event_unknown(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s:\n", origin);
    for(unsigned int i = 0; i < count; i++) {
        printf("%s\n", params[i]);
    }
}

static void event_numeric(irc_session_t *session, unsigned int event, char const *origin, char const **params, unsigned int count) {
    /* Used to print the fancy Bancho in osu! irc. */
}

static void event_dcc_chat_req(irc_session_t *session, char const *nick, char const *addr, irc_dcc_t dccid) {
    printf("dcc chat req: %s: %u\n", nick, dccid);
}

static void event_dcc_send_req(irc_session_t *session, char const *nick, char const *addr, char const *filename, unsigned long size, irc_dcc_t dccid) {
    printf("dcc send req: %s: %u\n", nick, dccid);
}

static void dcc_callback(irc_session_t *session, irc_dcc_t dccid, int status, void *ctx, char const *data, unsigned int length) {
    printf("Callback called!\n");
    if(status) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
    }
    else if(length == 0) {
        irc_dcc_msg(session, dccid, "!mp make test");
    }
    else {
        printf("Remote party said: %s\n", data);
    }
}

/****************************************************************************************************************************************************/

static void get_user_info(user_info_t *const user) {
    char buffer[512] = {};
    FILE *file = fopen("manage.cfg", "r");
    size_t l;

    fscanf(file, "%s", buffer);
    l = strlen(buffer);
    user->id = (char*)malloc(sizeof(char) * (l + 1));
    strncpy(user->id, buffer, l);

    fscanf(file, "%s", buffer);
    l = strlen(buffer);
    user->pwd = (char*)malloc(sizeof(char) * (l + 1));
    strncpy(user->pwd, buffer, l);

    fclose(file);
}

static void free_user_info(user_info_t *const user) {
    free(user->id);
    free(user->pwd);
}

static void free_ctx(irc_ctx_t *ctx) {
    free(ctx->channel);
    free(ctx->user);
    //free(ctx->match_info.game_title);
    free(ctx->match_info.match_channel);
}

/* osu! specific functions */

static void *banchobot_message_parse(irc_session_t *session, char const *message) {
    printf("BanchoBot said: %s\n", message);

    /*
        1.  BANCHOBOT_MULTI_CREATED
            Created the tournament match https://osu.ppy.sh/mp/NUMBER GAME_TITLE

            Return: [ (char*)banchobot_replies | (char*)NUMBER | (char*)GAME_TITLE ]
    */
    if(strncmp("Created the tournament match https://osu.ppy.sh/mp/", message, 51) == 0) {
        if(irc_cmd_part(session, init_channel)) {
            int err = irc_errno(session);
            fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
            irc_disconnect(session);
        }

        size_t idx = strlen("Created the tournament match https://osu.ppy.sh/mp/");
        size_t number_len = 0;

        while(message[idx + number_len] >= '0' && message[idx + number_len] <= '9') number_len++;
        char *match_number = (char*)malloc(sizeof(char) * (number_len + 1));
        memcpy(match_number, message + idx, number_len + 1);
        match_number[number_len] = '\0';

        idx += number_len + 1;
        number_len = strlen(message + idx);
        char *game_title = (char*)malloc(sizeof(char) * (number_len + 1));
        memcpy(game_title, message + idx, number_len + 1);

        void *ret = malloc(sizeof(char*) * 3);
        ((char**)ret)[0] = (char*)BANCHOBOT_MULTI_CREATED;
        ((char**)ret)[1] = match_number;
        ((char**)ret)[2] = game_title;

        return ret;
    }

    /**/

    

    //if(1) {}

    /*
        -1, 0. BANCHOBOT_UNKNOWN
        All replies that aren't contained in any cases of above
    */
    else {
        void *ret = malloc(sizeof(char*));
        ((char**)ret)[0] = (char*)BANCHOBOT_UNKNOWN;

        return ret;
    }
}