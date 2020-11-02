#include <libircclient.h>
#include <libirc_rfcnumeric.h>

// http://www.ulduzsoft.com/libircclient/index.html
#include <stdio.h>
#include <string.h>

typedef struct {
    char *user;
    char *channel;
} irc_ctx_t;

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

int main() {
    irc_callbacks_t callbacks;
    irc_ctx_t ctx;

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

    ctx.channel = "#korean";
    ctx.user = "A_BCDe";

    irc_session_t *session = irc_create_session(&callbacks);

    if(!session) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        return -1;
    }
    printf("Hello, world!\n");

    irc_set_ctx(session, &ctx);
    irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);

    if(irc_connect(session, "irc.ppy.sh", 6667, "caabe0", "A_BCDe", "A_BCDe", "A_BCDe")) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        return -1;
    }

    if(irc_run(session)) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
    }

    printf("Goodbye, world!\n");

    irc_destroy_session(session);
    return 0;
}

static void event_connect(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("Connected to server!\n");
    irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
    printf("Trying to connect to %s...", ctx->channel);
    if(irc_cmd_join(session, ctx->channel, NULL)) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        irc_disconnect(session);
    }
}

static void event_nick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s changed to %s!\n", origin, params[0]);
}

static void event_quit(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    //printf("%s has quit due to %s.\n", origin, params[0]);
}

static void event_join(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s has joined the channel %s.\n", origin, params[0]);
    irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
    if(strcmp(origin, ctx->user) == 0) {
        printf("%s has joined the channel %s", origin, params[0]);
        if(irc_cmd_msg(session, params[0], "Hello!")) {
            int err = irc_errno(session);
            printf("error code %d: %s\n", err, irc_strerror(err));
            printf("Sending message \"Hello\" failed!\n");
            irc_disconnect(session);
        }
    }
}

static void event_part(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    printf("%s has left the channel %s.\n", origin, params[0]);
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
        printf("%s has sent a message to channel %s:\n%s\n", origin, params[0], params[1]);
        irc_ctx_t *ctx = (irc_ctx_t*)irc_get_ctx(session);
        if(strcmp(origin, ctx->user) != 0 && strcmp(params[1], "Hello!") == 0) {
            printf("Got the right message!\n");
            irc_disconnect(session);
        }
    }
}

static void event_privmsg(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {
    if(count == 1) {
        printf("%s has sent you a message.", origin);
    }
    else {
        printf("%s has sent you a message:\n%s\n", origin, params[1]);
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
    //printf("%s:\n", origin);
    for(unsigned int i = 1; i < count; i++) {
        printf("%s\n", params[i]);
    }
    //printf("%d %d\n", strcmp(origin, "cho.ppy.sh"), strcmp(params[1], "- boat:   https://twitter.com/banchoboat"));
    /*
    if(strcmp(origin, "cho.ppy.sh") == 0 && strcmp(params[1], "boat:   https://twitter.com/banchoboat") == 0) {
        irc_dcc_t dccid;
        if(irc_dcc_chat(session, NULL, "BanchoBot", dcc_callback, &dccid)) {
            int err = irc_errno(session);
            fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
            exit(-1);
        }
    }
    */
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
