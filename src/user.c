#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "discord.h"
#include "discord-internal.h"
#include "discord-request.h"

CCORDcode
discord_get_current_user(struct discord *client, struct discord_ret_user *ret)
{
    struct discord_request req = { 0 };

    DISCORD_REQ_INIT(req, discord_user, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_GET,
                               "/users/@me");
}

CCORDcode
discord_get_user(struct discord *client,
                 u64snowflake user_id,
                 struct discord_ret_user *ret)
{
    struct discord_request req = { 0 };

    CCORD_EXPECT(client, user_id != 0, CCORD_BAD_PARAMETER, "");

    DISCORD_REQ_INIT(req, discord_user, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_GET,
                               "/users/%" PRIu64, user_id);
}

CCORDcode
discord_modify_current_user(struct discord *client,
                            struct discord_modify_current_user *params,
                            struct discord_ret_user *ret)
{
    struct discord_request req = { 0 };
    struct sized_buffer body;
    char buf[1024];

    CCORD_EXPECT(client, params != NULL, CCORD_BAD_PARAMETER, "");

    body.size = discord_modify_current_user_to_json(buf, sizeof(buf), params);
    body.start = buf;

    DISCORD_REQ_INIT(req, discord_user, ret);

    return discord_adapter_run(&client->adapter, &req, &body, HTTP_PATCH,
                               "/users/@me");
}

CCORDcode
discord_get_current_user_guilds(struct discord *client,
                                struct discord_ret_guilds *ret)
{
    struct discord_request req = { 0 };

    DISCORD_REQ_LIST_INIT(req, discord_guilds, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_GET,
                               "/users/@me/guilds");
}

CCORDcode
discord_leave_guild(struct discord *client,
                    u64snowflake guild_id,
                    struct discord_ret *ret)
{
    struct discord_request req = { 0 };
    struct sized_buffer body = { "{}", 2 };

    CCORD_EXPECT(client, guild_id != 0, CCORD_BAD_PARAMETER, "");

    DISCORD_REQ_BLANK_INIT(req, ret);

    return discord_adapter_run(&client->adapter, &req, &body, HTTP_DELETE,
                               "/users/@me/guilds/%" PRIu64, guild_id);
}

CCORDcode
discord_create_dm(struct discord *client,
                  struct discord_create_dm *params,
                  struct discord_ret_channel *ret)
{
    struct discord_request req = { 0 };
    struct sized_buffer body;
    char buf[128];

    CCORD_EXPECT(client, params != NULL, CCORD_BAD_PARAMETER, "");

    body.size = discord_create_dm_to_json(buf, sizeof(buf), params);
    body.start = buf;

    DISCORD_REQ_INIT(req, discord_channel, ret);

    return discord_adapter_run(&client->adapter, &req, &body, HTTP_POST,
                               "/users/@me/channels");
}

CCORDcode
discord_create_group_dm(struct discord *client,
                        struct discord_create_group_dm *params,
                        struct discord_ret_channel *ret)
{
    struct discord_request req = { 0 };
    struct sized_buffer body;
    char buf[1024];

    CCORD_EXPECT(client, params != NULL, CCORD_BAD_PARAMETER, "");
    CCORD_EXPECT(client, params->access_tokens != NULL, CCORD_BAD_PARAMETER,
                 "");
    CCORD_EXPECT(client, params->nicks != NULL, CCORD_BAD_PARAMETER, "");

    body.size = discord_create_group_dm_to_json(buf, sizeof(buf), params);
    body.start = buf;

    DISCORD_REQ_INIT(req, discord_channel, ret);

    return discord_adapter_run(&client->adapter, &req, &body, HTTP_POST,
                               "/users/@me/channels");
}

CCORDcode
discord_get_user_connections(struct discord *client,
                             struct discord_ret_connections *ret)
{
    struct discord_request req = { 0 };

    DISCORD_REQ_LIST_INIT(req, discord_connections, ret);

    return discord_adapter_run(&client->adapter, &req, NULL, HTTP_GET,
                               "/users/@me/connections");
}
