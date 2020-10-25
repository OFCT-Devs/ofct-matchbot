#include <libircclient.h>
#include <libirc_rfcnumeric.h>
// http://www.ulduzsoft.com/libircclient/index.html
#include <stdio.h>
#include <string.h>

int main() {
    irc_callbacks_t callbacks;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.event_connect = NULL; //event_connect;
    callbacks.event_numeric = NULL; //event_numeric;

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
