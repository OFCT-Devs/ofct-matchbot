#ifndef OFCT_TYPEDEF_H
#define OFCT_TYPEDEF_H

typedef struct match_info_s {
    char *match_channel;
    char *game_title;
} match_info_t;

typedef struct irc_ctx_s {
    match_info_t match_info;
    char *user;
    char *channel;
} irc_ctx_t;

typedef struct user_info_s {
    char *id;
    char *pwd;
} user_info_t;

typedef enum banchobot_replies {
    BANCHOBOT_UNKNOWN = -1,
    BANCHOBOT_NONE = 0,
    BANCHOBOT_MULTI_CREATED,
    
} banchobot_replies;

#endif