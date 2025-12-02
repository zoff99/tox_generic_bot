/**
 *
 * tox_generic_bot
 * (C)Zoff <zoff@zoff.cc> in 2023
 *
 * https://github.com/zoff99/tox_generic_bot
 *
 *
 */
/*
 * Copyright © 2023 Zoff <zoff@zoff.cc>
 *
 * tox_generic_bot is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tox_generic_bot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; If not, see <https://www.gnu.org/licenses/>.
 */

/*

 linux compile:

 gcc -O3 -std=c99 -g -flto -fPIC tox_generic_bot.c -fno-omit-frame-pointer -fsanitize=address -static-libasan -Wl,-Bstatic $(pkg-config --cflags --libs libsodium) -Wl,-Bdynamic -pthread -o tox_generic_bot

 linux clang compile:

 clang-15 -O3 -std=c99 -Wall -Wextra -Weverything -g -flto -fPIC tox_generic_bot.c -fno-omit-frame-pointer -fsanitize=address -Wl,-Bstatic $(pkg-config --cflags --libs libsodium) -Wl,-Bdynamic -pthread -o tox_generic_bot




 linux compile with FORTIFY:

 gcc -O3 -std=c99 -g -D_FORTIFY_SOURCE=2 -flto -fPIC tox_generic_bot.c -Wl,-Bstatic $(pkg-config --cflags --libs libsodium) -Wl,-Bdynamic -pthread -o tox_generic_bot




 linux compile with TokTok toxcore:

 gcc -O3 -std=c99 -DUSE_TOKTOK_TOXCORE -g -flto -fPIC tox_generic_bot.c -fno-omit-frame-pointer -fsanitize=address -static-libasan -Wl,-Bstatic $(pkg-config --cflags --libs libtoxcore libsodium) -Wl,-Bdynamic -pthread -o tox_generic_bot


*/


#define _GNU_SOURCE

// ----------- version -----------
// ----------- version -----------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wunused-macros"
#define VERSION_MAJOR 0
#define VERSION_MINOR 99
#define VERSION_PATCH 2
#pragma GCC diagnostic push
static const char global_version_string[] = "0.99.2";
// ----------- version -----------
// ----------- version -----------

#include <ctype.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <sodium.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>

// define this before including toxcore amalgamation -------
#ifdef MIN_LOGGER_LEVEL
#undef MIN_LOGGER_LEVEL
#endif
#define MIN_LOGGER_LEVEL LOGGER_LEVEL_INFO
// define this before including toxcore amalgamation -------


// ------------------- toxcore amalgamation ----------------
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wpragmas"
#pragma clang diagnostic ignored "-Wunknown-warning-option"
#pragma clang diagnostic ignored "-Wmost"
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wformat"
#pragma clang diagnostic ignored "-Wint-conversion"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wunused-variable"
//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wmost"
#pragma GCC diagnostic ignored "-Weverything"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wmissing-variable-declarations"

#ifdef USE_TOKTOK_TOXCORE
#include "tox/tox.h"
#else
// include toxcore amalgamation no ToxAV --------
#include "toxcore_amalgamation_no_toxav.c"
// include toxcore amalgamation no ToxAV --------
#endif
#pragma GCC diagnostic pop
//
#pragma clang diagnostic pop
// ------------------- toxcore amalgamation ----------------


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-macros"
#pragma GCC diagnostic ignored "-Wunused-function"

enum CUSTOM_LOG_LEVEL {
  CLL_ERROR = 0,
  CLL_WARN = 1,
  CLL_INFO = 2,
  CLL_DEBUG = 9,
};

#define BOT_NAME "ToxGenericBot"
#define CURRENT_LOG_LEVEL CLL_INFO // 0 -> error, 1 -> warn, 2 -> info, 9 -> debug
#define PROXY_HOST_TOR_DEFAULT "127.0.0.1"
#define PROXY_PORT_TOR_DEFAULT 9050
static const uint8_t *bot_name_str = (const uint8_t *)(BOT_NAME);
static uint32_t bot_name_len = strlen(BOT_NAME);
static FILE *logfile = NULL;
static const char *log_filename = "tox_generic_bot.log";
static const char *savedata_filename = "savedata.tox";
static const char *savedata_tmp_filename = "savedata.tox.tmp";
static int self_online = 0;
static bool main_loop_running = true;
static int switch_tcponly = 0;
static int use_tor = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wpadded"
static struct Node1 {
    char *ip;
    char *key;
    uint16_t udp_port;
    uint16_t tcp_port;
} nodes1[] = {
{ "2604:a880:1:20::32f:1001", "BEF0CFB37AF874BD17B9A8F9FE64C75521DB95A37D33C5BDB00E9CF58659C04F", 33445, 33445 },
{ "2400:8902::f03c:93ff:fe69:bf77", "F76A11284547163889DDC89A7738CF271797BF5E5E220643E97AD3C7E7903D55", 33445, 443 },
{"139.162.110.188","F76A11284547163889DDC89A7738CF271797BF5E5E220643E97AD3C7E7903D55",33445,443},
{ "46.101.197.175", "CD133B521159541FB1D326DE9850F5E56A6C724B5B8E5EB5CD8D950408E95707", 33445, 3389 },
{ "144.217.167.73","7E5668E0EE09E19F320AD47902419331FFEE147BB3606769CFBE921A2A2FD34C",33445,33445},
{ "tox1.mf-net.eu", "B3E5FA80DC8EBD1149AD2AB35ED8B85BD546DEDE261CA593234C619249419506", 33445, 3389 },
{ "bg.tox.dcntrlzd.network", "20AD2A54D70E827302CDF5F11D7C43FA0EC987042C36628E64B2B721A1426E36", 33445, 33445 },
{"91.219.59.156","8E7D0B859922EF569298B4D261A8CCB5FEA14FB91ED412A7603A585A25698832",33445,33445},
{"85.143.221.42","DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43",33445,33445},
{"tox.initramfs.io","3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25",33445,33445},
{"144.217.167.73","7E5668E0EE09E19F320AD47902419331FFEE147BB3606769CFBE921A2A2FD34C",33445,3389},
{"tox.abilinski.com","10C00EB250C3233E343E2AEBA07115A5C28920E9C8D29492F6D00B29049EDC7E",33445,33445},
{"tox.novg.net","D527E5847F8330D628DAB1814F0A422F6DC9D0A300E6C357634EE2DA88C35463",33445,33445},
{"198.199.98.108","BEF0CFB37AF874BD17B9A8F9FE64C75521DB95A37D33C5BDB00E9CF58659C04F",33445,33445},
{"tox.kurnevsky.net","82EF82BA33445A1F91A7DB27189ECFC0C013E06E3DA71F588ED692BED625EC23",33445,33445},
{"81.169.136.229","E0DB78116AC6500398DDBA2AEEF3220BB116384CAB714C5D1FCD61EA2B69D75E",33445,33445},
{"205.185.115.131","3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68",53,53},
{"bg.tox.dcntrlzd.network","20AD2A54D70E827302CDF5F11D7C43FA0EC987042C36628E64B2B721A1426E36",33445,33445},
{"46.101.197.175","CD133B521159541FB1D326DE9850F5E56A6C724B5B8E5EB5CD8D950408E95707",33445,33445},
{"tox1.mf-net.eu","B3E5FA80DC8EBD1149AD2AB35ED8B85BD546DEDE261CA593234C619249419506",33445,33445},
{"tox2.mf-net.eu","70EA214FDE161E7432530605213F18F7427DC773E276B3E317A07531F548545F",33445,33445},
{"195.201.7.101","B84E865125B4EC4C368CD047C72BCE447644A2DC31EF75BD2CDA345BFD310107",33445,33445},
{"tox4.plastiras.org","836D1DA2BE12FE0E669334E437BE3FB02806F1528C2B2782113E0910C7711409",33445,443},
{"gt.sot-te.ch","F4F4856F1A311049E0262E9E0A160610284B434F46299988A9CB42BD3D494618",33445,33445},
{"188.225.9.167","1911341A83E02503AB1FD6561BD64AF3A9D6C3F12B5FBB656976B2E678644A67",33445,33445},
{"122.116.39.151","5716530A10D362867C8E87EE1CD5362A233BAFBBA4CF47FA73B7CAD368BD5E6E",33445,33445},
{"195.123.208.139","534A589BA7427C631773D13083570F529238211893640C99D1507300F055FE73",33445,33445},
{"104.225.141.59","933BA20B2E258B4C0D475B6DECE90C7E827FE83EFA9655414E7841251B19A72C",43334,43334},
{"198.98.49.206","28DB44A3CEEE69146469855DFFE5F54DA567F5D65E03EFB1D38BBAEFF2553255",33445,33445},
{"172.105.109.31","D46E97CF995DC1820B92B7D899E152A217D36ABE22730FEA4B6BF1BFC06C617C",33445,33445},
{"ru.tox.dcntrlzd.network","DBB2E896990ECC383DA2E68A01CA148105E34F9B3B9356F2FE2B5096FDB62762",33445,33445},
{"91.146.66.26","B5E7DAC610DBDE55F359C7F8690B294C8E4FCEC4385DE9525DBFA5523EAD9D53",33445,33445},
{"tox01.ky0uraku.xyz","FD04EB03ABC5FC5266A93D37B4D6D6171C9931176DC68736629552D8EF0DE174",33445,33445},
{"tox02.ky0uraku.xyz","D3D6D7C0C7009FC75406B0A49E475996C8C4F8BCE1E6FC5967DE427F8F600527",33445,33445},
{"kusoneko.moe","BE7ED53CD924813507BA711FD40386062E6DC6F790EFA122C78F7CDEEE4B6D1B",33445,33445},
{"tox2.plastiras.org","B6626D386BE7E3ACA107B46F48A5C4D522D29281750D44A0CBA6A2721E79C951",33445,33445},
{"172.104.215.182","DA2BD927E01CD05EBCC2574EBE5BEBB10FF59AE0B2105A7D1E2B40E49BB20239",33445,33445},
    { NULL, NULL, 0, 0 }
};
#pragma GCC diagnostic push


static void dbg(enum CUSTOM_LOG_LEVEL level, const char *fmt, ...)
{
    char *level_and_format = NULL;
    char *fmt_copy = NULL;

    if (fmt == NULL)
    {
        return;
    }

    if (strlen(fmt) < 1)
    {
        return;
    }

    if (!logfile)
    {
        return;
    }

    if (((int)level < 0) || ((int)level > 9))
    {
        level = 0;
    }

    if (strlen(fmt) < 1)
    {
        return;
    }

    level_and_format = calloc(1, strlen(fmt) + 3 + 1);
    if (!level_and_format)
    {
        return;
    }

    fmt_copy = level_and_format + 2;
    strcpy(fmt_copy, fmt);
    level_and_format[1] = ':';

    if (level == 0)
    {
        level_and_format[0] = 'E';
    }
    else if (level == 1)
    {
        level_and_format[0] = 'W';
    }
    else if (level == 2)
    {
        level_and_format[0] = 'I';
    }
    else
    {
        level_and_format[0] = 'D';
    }

    level_and_format[(strlen(fmt) + 2)] = '\0'; // '\0' or '\n'
    level_and_format[(strlen(fmt) + 3)] = '\0';
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t t3 = time(NULL);
    struct tm tm3 = *localtime(&t3);
    char *level_and_format_2 = calloc(1, strlen(level_and_format) + 5 + 3 + 3 + 1 + 3 + 3 + 3 + 7 + 1);
    level_and_format_2[0] = '\0';
    snprintf(level_and_format_2, (strlen(level_and_format) + 5 + 3 + 3 + 1 + 3 + 3 + 3 + 7 + 1),
             "%04d-%02d-%02d %02d:%02d:%02d.%06ld:%s",
             tm3.tm_year + 1900, tm3.tm_mon + 1, tm3.tm_mday,
             tm3.tm_hour, tm3.tm_min, tm3.tm_sec, (long)tv.tv_usec, level_and_format);

    if (level <= CURRENT_LOG_LEVEL)
    {
        va_list ap;
        va_start(ap, fmt);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
        vfprintf(logfile, level_and_format_2, ap);
#pragma GCC diagnostic pop
        va_end(ap);
    }

    if (level_and_format)
    {
        free(level_and_format);
    }

    if (level_and_format_2)
    {
        free(level_and_format_2);
    }
}

static void tox_log_cb__custom(Tox *tox, TOX_LOG_LEVEL level, const char *file, uint32_t line, const char *func,
                        const char *message, void *user_data)
{
    enum CUSTOM_LOG_LEVEL toxcore_wrapped_level = CLL_INFO;
    if (level < TOX_LOG_LEVEL_INFO) {
        toxcore_wrapped_level = CLL_DEBUG;
    } else if (level == TOX_LOG_LEVEL_WARNING) {
        toxcore_wrapped_level = CLL_WARN;
    } else if (level == TOX_LOG_LEVEL_ERROR) {
        toxcore_wrapped_level = CLL_ERROR;
    }
    dbg(toxcore_wrapped_level, "TOX:%d:%d:%s:%s\n", (int)level, (int)line, func, message);
}

/**
 * @brief Converts a hexadecimal string to binary format
 *
 * @param hex_string The hexadecimal string to be converted, must be NULL terminated
 * @param output Pointer to the binary format output buffer
 */
static void hex_string_to_bin2(const char *hex_string, uint8_t *output)
{
    size_t len = strlen(hex_string) / 2;
    size_t i = len;
    if (!output)
    {
        return;
    }
    const char *pos = hex_string;
    for (i = 0; i < len; ++i, pos += 2)
    {
        sscanf(pos, "%2hhx", &output[i]);
    }
}

static unsigned int char_to_int(char c)
{
    if (c >= '0' && c <= '9') {
        return (uint8_t)c - '0';
    }

    if (c >= 'A' && c <= 'F') {
        return 10 + (uint8_t)c - 'A';
    }

    if (c >= 'a' && c <= 'f') {
        return 10 + (uint8_t)c - 'a';
    }

    return (unsigned int)(' ');
}

static bool pubkeys_hex_equal(const uint8_t *pubkey1_hex_str, const uint8_t *pubkey2_hex_str)
{
    if (strncmp((const char *)pubkey1_hex_str, (const char *)pubkey2_hex_str, (TOX_PUBLIC_KEY_SIZE * 2)) == 0) {
        return true;
    } else {
        return false;
    }
}

static bool pubkeys_bin_equal(const uint8_t *pubkey1_bin, const uint8_t *pubkey2_bin) {
    return (memcmp(pubkey1_bin, pubkey2_bin, TOX_PUBLIC_KEY_SIZE) == 0);
}

static uint8_t *hex_string_to_bin(const char *hex_string)
{
    size_t len = TOX_ADDRESS_SIZE;
    uint8_t *val = calloc(1, len);
    for (size_t i = 0; i != len; ++i)
    {
        val[i] = (uint8_t)((16 * char_to_int(hex_string[2 * i])) + (char_to_int(hex_string[2 * i + 1])));
    }
    return val;
}

/**
 * @brief Converts binary data to uppercase hexadecimal string using libsodium
 *
 * @param bin Pointer to binary data
 * @param bin_size Size of binary data
 * @param hex Pointer to hexadecimal string
 * @param hex_size Size of hexadecimal string
 */
static void bin2upHex(const uint8_t *bin, uint32_t bin_size, char *hex, uint32_t hex_size)
{
    sodium_bin2hex(hex, hex_size, bin, bin_size);
    for (size_t i = 0; i < hex_size - 1; i++) {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wdisabled-macro-expansion"
        hex[i] = (char)toupper(hex[i]);
#pragma GCC diagnostic pop
    }
}

/**
 * @brief Delays the execution of the current thread for a specified number of milliseconds.
 *
 * @param ms The number of milliseconds to delay the execution of the current thread.
 */
static void yieldcpu(uint32_t ms)
{
    usleep(1000 * ms);
}

// -------- Tox related functions --------

static void update_tox_savedata(const Tox *tox)
{
    size_t size = tox_get_savedata_size(tox);
    uint8_t *savedata = calloc(1, size);
    tox_get_savedata(tox, savedata);
    FILE *f = fopen(savedata_tmp_filename, "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);
    rename(savedata_tmp_filename, savedata_filename);
    free(savedata);
}

static void self_connection_change_callback(Tox *tox, TOX_CONNECTION status, void *userdata)
{
    switch (status) {
        case TOX_CONNECTION_NONE:
            dbg(CLL_INFO, "Lost connection to the Tox network.\n");
            self_online = 0;
            break;
        case TOX_CONNECTION_TCP:
            dbg(CLL_INFO, "Connected using TCP.\n");
            self_online = 1;
            break;
        case TOX_CONNECTION_UDP:
            dbg(CLL_INFO, "Connected using UDP.\n");
            self_online = 2;
            break;
    }
}

static void friendlist_onConnectionChange(Tox *tox, uint32_t friend_number, TOX_CONNECTION status, void *user_data)
{
    switch (status) {
        case TOX_CONNECTION_NONE:
            dbg(CLL_INFO, "Lost connection to friend %d.\n", friend_number);
            break;
        case TOX_CONNECTION_TCP:
            dbg(CLL_INFO, "Connected to friend %d using TCP.\n", friend_number);
            break;
        case TOX_CONNECTION_UDP:
            dbg(CLL_INFO, "Connected to friend %d using UDP.\n", friend_number);
            break;
    }
}

static void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data)
{
    tox_friend_add_norequest(tox, public_key, NULL);
    update_tox_savedata(tox);
}

static void friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                       size_t length, void *user_data)
{
    if (type == TOX_MESSAGE_TYPE_NORMAL)
    {
        if ((message != NULL) && (length > 0))
        {
            char *message2 = calloc(1, length + 1);
            if (message2)
            {
                memcpy(message2, message, length);
                dbg(CLL_INFO, "incoming message: fnum=%d text=%s\n", friend_number, message2);
                free(message2);
            }
        }
    }
}

static void group_invite_cb(Tox *tox, uint32_t friend_number, const uint8_t *invite_data, size_t length,
                                 const uint8_t *group_name, size_t group_name_length, void *userdata)
{
    Tox_Err_Group_Invite_Accept error;
    tox_group_invite_accept(tox, friend_number, invite_data, length,
                                 bot_name_str, bot_name_len,
                                 NULL, 0,
                                 &error);
    dbg(CLL_INFO, "tox_group_invite_accept:%d\n", error);
    update_tox_savedata(tox);
}

static void group_self_join_cb(Tox *tox, uint32_t group_number, void *userdata)
{
    dbg(CLL_INFO, "You joined group %d\n", group_number);
    tox_group_self_set_name(tox, group_number,
                        bot_name_str, bot_name_len,
                        NULL);
    update_tox_savedata(tox);
}

static void group_peer_join_cb(Tox *tox, uint32_t group_number, uint32_t peer_id, void *user_data)
{
    dbg(CLL_INFO, "Peer %d joined group %d\n", peer_id, group_number);
    update_tox_savedata(tox);
}

static void group_peer_exit_cb(Tox *tox, uint32_t group_number, uint32_t peer_id, Tox_Group_Exit_Type exit_type,
                                    const uint8_t *name, size_t name_length, const uint8_t *part_message, size_t length, void *user_data)
{
    switch (exit_type) {
        case TOX_GROUP_EXIT_TYPE_QUIT:
        dbg(CLL_INFO, "Peer %d left group %d reason: %d TOX_GROUP_EXIT_TYPE_QUIT\n", peer_id, group_number, exit_type);
            break;
        case TOX_GROUP_EXIT_TYPE_TIMEOUT:
        dbg(CLL_INFO, "Peer %d left group %d reason: %d TOX_GROUP_EXIT_TYPE_TIMEOUT\n", peer_id, group_number, exit_type);
            break;
        case TOX_GROUP_EXIT_TYPE_DISCONNECTED:
        dbg(CLL_INFO, "Peer %d left group %d reason: %d TOX_GROUP_EXIT_TYPE_DISCONNECTED\n", peer_id, group_number, exit_type);
            break;
        case TOX_GROUP_EXIT_TYPE_SELF_DISCONNECTED:
        dbg(CLL_INFO, "Peer %d left group %d reason: %d TOX_GROUP_EXIT_TYPE_SELF_DISCONNECTED\n", peer_id, group_number, exit_type);
            break;
        case TOX_GROUP_EXIT_TYPE_KICK:
        dbg(CLL_INFO, "Peer %d left group %d reason: %d TOX_GROUP_EXIT_TYPE_KICK\n", peer_id, group_number, exit_type);
            break;
        case TOX_GROUP_EXIT_TYPE_SYNC_ERROR:
        dbg(CLL_INFO, "Peer %d left group %d reason: %d TOX_GROUP_EXIT_TYPE_SYNC_ERROR\n", peer_id, group_number, exit_type);
            break;
    }
    update_tox_savedata(tox);
}

static void group_join_fail_cb(Tox *tox, uint32_t group_number, Tox_Group_Join_Fail fail_type, void *user_data)
{
    dbg(CLL_INFO, "Joining group %d failed. reason: %d\n", group_number, fail_type);
    update_tox_savedata(tox);
}

static void group_moderation_cb(Tox *tox, uint32_t group_number, uint32_t source_peer_id, uint32_t target_peer_id,
                                     Tox_Group_Mod_Event mod_type, void *user_data)
{
    dbg(CLL_INFO, "group moderation event, group %d srcpeer %d tgtpeer %d type %d\n",
        group_number, source_peer_id, target_peer_id, mod_type);
    update_tox_savedata(tox);
}

static void group_peer_status_cb(Tox *tox, uint32_t group_number, uint32_t peer_id, Tox_User_Status status,
                                      void *user_data)
{
    dbg(CLL_INFO, "group peer status event, group %d peer %d status %d\n",
        group_number, peer_id, status);
    update_tox_savedata(tox);
}

static void group_message_cb(Tox *tox, uint32_t group_number, uint32_t peer_id, Tox_Message_Type type,
                                  const uint8_t *message, size_t length, uint32_t message_id, void *user_data)
{
    dbg(CLL_INFO, "group peer message, group %d peer %d\n", group_number, peer_id);
}

static void set_tox_callbacks(Tox *tox)
{
    // ----- CALLBACKS -----
#ifdef TOX_HAVE_TOXUTIL
    tox_utils_callback_self_connection_status(tox, self_connection_change_callback);
    tox_callback_self_connection_status(tox, tox_utils_self_connection_status_cb);
    tox_utils_callback_friend_connection_status(tox, friendlist_onConnectionChange);
    tox_callback_friend_connection_status(tox, tox_utils_friend_connection_status_cb);
#else
    tox_callback_self_connection_status(tox, self_connection_change_callback);
#endif
    tox_callback_friend_request(tox, friend_request_cb);
    tox_callback_friend_message(tox, friend_message_cb);

    tox_callback_group_invite(tox, group_invite_cb);
    tox_callback_group_peer_join(tox, group_peer_join_cb);
    tox_callback_group_self_join(tox, group_self_join_cb);
    tox_callback_group_peer_exit(tox, group_peer_exit_cb);
    tox_callback_group_join_fail(tox, group_join_fail_cb);
    tox_callback_group_moderation(tox, group_moderation_cb);
    tox_callback_group_peer_status(tox, group_peer_status_cb);
    tox_callback_group_message(tox, group_message_cb);
    // ----- CALLBACKS -----
}

static Tox* create_tox(void)
{
    struct Tox_Options options;
    tox_options_default(&options);
    // ----- set options ------
    options.ipv6_enabled = true;
    options.local_discovery_enabled = true;
    options.hole_punching_enabled = true;
    options.udp_enabled = true;
    options.tcp_port = 0; // disable tcp relay function!
    options.log_callback = tox_log_cb__custom;
    // ----- set options ------

    if (switch_tcponly == 0) {
        options.udp_enabled = true; // UDP mode
        dbg(CLL_INFO, "setting UDP mode\n");
    } else {
        options.udp_enabled = false; // TCP mode
        dbg(CLL_INFO, "setting TCP mode (tcp option)\n");
    }

    if (use_tor == 1) {
        options.udp_enabled = false; // TCP mode
        options.local_discovery_enabled = false;
        dbg(CLL_INFO, "setting TCP mode (tor option)\n");
    }

    if (use_tor == 1) {
        dbg(CLL_INFO, "setting Tor Relay mode\n");
        const char *proxy_host = PROXY_HOST_TOR_DEFAULT;
        dbg(CLL_INFO, "setting proxy_host %s\n", proxy_host);
        uint16_t proxy_port = PROXY_PORT_TOR_DEFAULT;
        dbg(CLL_INFO, "setting proxy_port %d\n", (int)proxy_port);
        options.proxy_type = TOX_PROXY_TYPE_SOCKS5;
        options.proxy_host = proxy_host;
        options.proxy_port = proxy_port;
    } else {
        options.proxy_type = TOX_PROXY_TYPE_NONE;
    }

    FILE *f = fopen(savedata_filename, "rb");
    uint8_t *savedata = NULL;
    if (f)
    {
        fseek(f, 0, SEEK_END);
        size_t savedataSize = (size_t)ftell(f);
        fseek(f, 0, SEEK_SET);
        savedata = calloc(1, savedataSize);
        if (!savedata)
        {
            fclose(f);
            return NULL;
        }

        size_t ret = fread(savedata, savedataSize, 1, f);
        if (ret != 1)
        {
            free(savedata);
            fclose(f);
            return NULL;
        }
        fclose(f);
        options.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
        options.savedata_data = savedata;
        options.savedata_length = savedataSize;
    }

    Tox_Err_New error_tox;
#ifndef TOX_HAVE_TOXUTIL
    dbg(CLL_INFO, "init Tox\n");
    Tox *tox = tox_new(&options, &error_tox);
#else
    dbg(CLL_INFO, "init Tox [TOXUTIL]\n");
    Tox *tox = tox_utils_new(&options, &error_tox);
#endif
    dbg(CLL_INFO, "init Tox res:%d\n", error_tox);
    free(savedata);

    return tox;
}

static void bootstrap_tox(Tox *tox)
{
    // ----- bootstrap -----
    dbg(CLL_INFO, "Tox bootstrapping\n");
    // dummy node to bootstrap
    tox_bootstrap(tox, "local", 7766, (uint8_t *)"2AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA1", NULL);
    for (int i = 0; nodes1[i].ip; i++) {
        uint8_t *key = (uint8_t *)calloc(1, 100);
        if (!key) {
            dbg(CLL_INFO, "bootstrap_tox:continue ...\n");
            continue;
        }
        hex_string_to_bin2(nodes1[i].key, key);
        if (use_tor == 0) {
            tox_bootstrap(tox, nodes1[i].ip, nodes1[i].udp_port, key, NULL);
        }
        if (nodes1[i].tcp_port != 0) {
            tox_add_tcp_relay(tox, nodes1[i].ip, nodes1[i].tcp_port, key, NULL);
        }
        free(key);
    }
    // ----- bootstrap -----
}

static void print_tox_id(Tox *tox)
{
    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(tox, tox_id_bin);
    const uint32_t tox_address_hex_size = (TOX_ADDRESS_SIZE) * 2 + 1;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wvla"
    char tox_id_hex[tox_address_hex_size];
#pragma GCC diagnostic pop
    bin2upHex(tox_id_bin, tox_address_size(), tox_id_hex, tox_address_hex_size);
    printf("--------------------\n");
    printf("--------------------\n");
    printf("ToxID: %s\n", tox_id_hex);
    dbg(CLL_INFO, "ToxID: %s\n", tox_id_hex);
    printf("--------------------\n");
    printf("--------------------\n");
}

// -------- Tox related functions --------


static void cmd_args_and_options(int argc, char *argv[])
{
    int opt;
    const char     *short_opt = "hvTt";
    struct option   long_opt[] =
    {
        {"help",          no_argument,       NULL, 'h'},
        {"version",       no_argument,       NULL, 'v'},
        {NULL,            0,                 NULL,  0 }
    };

    while ((opt = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch (opt)
        {
            case -1:       /* no more arguments */
            case 0:        /* long options toggles */
                break;
            case 't':
                switch_tcponly = 1;
                break;
            case 'T':
                use_tor = 1;
                break;
            case 'v':
                printf("%s version: %s\n", bot_name_str, global_version_string);
                exit(0);
            case 'h':
                printf("Usage: %s [OPTIONS]\n", argv[0]);
                printf("  -t,                                  tcp only mode\n");
                printf("  -T,                                  use tor proxy\n");
                printf("  -v, --version                        show version\n");
                printf("  -h, --help                           print this help and exit\n");
                printf("\n");
                exit(0);
            case ':':
            case '?':
                fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
                exit(-2);
            default:
                fprintf(stderr, "%s: invalid option -- %c\n", argv[0], opt);
                fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
                exit(-2);
        }
    }
}

// signal handlers --------------------------------------------------
/**
 * @brief Signal handler for INT signal
 *
 * This function is called when the program receives an INT signal.
 * It sets the main loop running flag to false.
 *
 * @param sig The signal number
 */
void INThandler(int sig)
{
    signal(sig, SIG_IGN);
    printf("_\n");
    printf("INT signal\n");
    main_loop_running = false;
}
// signal handlers --------------------------------------------------


int main(int argc, char *argv[])
{
    cmd_args_and_options(argc, argv);

    logfile = fopen(log_filename, "wb");
    setvbuf(logfile, NULL, _IOLBF, 0);
    dbg(CLL_INFO, "-LOGGER-\n");

    dbg(CLL_INFO, "version:%s\n", global_version_string);

    Tox *tox = create_tox();
    if (tox == NULL) {
        if (logfile)
        {
            fclose(logfile);
            logfile = NULL;
        }
        exit(-1);
    }
    tox_self_set_name(tox, bot_name_str, bot_name_len, NULL);
    update_tox_savedata(tox);

    print_tox_id(tox);
    set_tox_callbacks(tox);

    main_loop_running = true;

    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wdisabled-macro-expansion"
    sa.sa_handler = INThandler;
#pragma GCC diagnostic pop
    sa.sa_flags = 0;// not SA_RESTART!;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    // ----------- main loop -----------
    uint32_t loops = 0;
    while (main_loop_running)
    {
        if (self_online == 0) {
            loops++;
            if ((loops % 100) == 0) {
                bootstrap_tox(tox);
                update_tox_savedata(tox);
            }
        }
        tox_iterate(tox, NULL);
        yieldcpu(tox_iteration_interval(tox));
    }
    // ----------- main loop -----------

    // ----- shutdown -----
    dbg(CLL_INFO, "shutdown ...\n");

#ifndef TOX_HAVE_TOXUTIL
    tox_kill(tox);
    dbg(CLL_INFO, "killed Tox\n");
#else
    tox_utils_kill(tox);
    dbg(CLL_INFO, "killed Tox [TOXUTIL]\n");
#endif

    if (logfile)
    {
        fclose(logfile);
        logfile = NULL;
    }

    // HINT: for gprof you need an "exit()" call
    exit(0);
}


#pragma GCC diagnostic pop
