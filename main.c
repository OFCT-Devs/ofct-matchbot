#include <libircclient.h>
#include <libirc_rfcnumeric.h>
// http://www.ulduzsoft.com/libircclient/index.html
#include <stdio.h>
#include <string.h>

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
static void event_connect(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count);
static void event_numeric(irc_session_t *session, unsigned int event, char const *origin, char const **params, unsigned int count);
static void event_dcc_chat_req(irc_session_t *session, char const *nick, char const *addr, irc_dcc_t dccid);
static void event_dcc_send_req(irc_session_t *session, char const *nick, char const *addr, char const *filename, unsigned long size, irc_dcc_t dccid);

int main() {
    irc_callbacks_t callbacks;

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
    callbacks.event_connect = event_connect;
    callbacks.event_numeric = event_numeric;
    callbacks.event_dcc_chat_req = event_dcc_chat_req;
    callbacks.event_dcc_send_req = event_dcc_send_req;

    irc_session_t *session = irc_create_session(&callbacks);

    if(!session) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        return -1;
    }
    printf("Hello, world!\n");

    irc_option_set(session, LIBIRC_OPTION_STRIPNICKS);

    if(irc_connect(session, "irc.ppy.sh", 6667, "caabe0", "A_BCDe", "A_BCDe", "A_BCDe")) {
        int err = irc_errno(session);
        fprintf(stderr, "error code %d: %s\n", err, irc_strerror(err));
        return -1;
    }

    if(irc_run(session)) {

    }

    printf("Goodbye, world!\n");

    irc_destroy_session(session);
    return 0;
}

static void event_connect(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_nick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_quit(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_join(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_part(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_mode(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_umode(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_topic(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_kick(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_channel(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_privmsg(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_notice(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_channel_notice(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_invite(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_ctcp_req(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_ctcp_rep(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_ctcp_action(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_unknown(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_connect(irc_session_t *session, char const *event, char const *origin, char const **params, unsigned int count) {

}

static void event_numeric(irc_session_t *session, unsigned int event, char const *origin, char const **params, unsigned int count) {

}

static void event_dcc_chat_req(irc_session_t *session, char const *nick, char const *addr, irc_dcc_t dccid) {

}

static void event_dcc_send_req(irc_session_t *session, char const *nick, char const *addr, char const *filename, unsigned long size, irc_dcc_t dccid) {

}
