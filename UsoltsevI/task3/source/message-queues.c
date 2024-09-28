//-----------------------------------------------------------------
//
// Message queues file translation implementation
//
//-----------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "../include/message-queues.h"

#define CHECK_CONDITION_RET(a, to_free1, to_free2, to_return)   \
    if (a) {                                \
        fprintf("%s in func %s in file %s"  \
                , #a, __func__, __FILE__);  \
        free(to_free1);                     \
        free(to_free2);                     \
        return to_return;                   \
    }

static const size_t MSG_BUF_SIZE = 1024;

typedef struct _msgbuf {
    long   mtype;
    char*  mtext;
    size_t bsize;
} msgbuf;

static msgbuf *construct_msgbuf(long mtype, size_t bsize) {
    msgbuf *buf = (msgbuf *) malloc(sizeof(msgbuf));
    CHECK_CONDITION_RET(buf == NULL, NULL, NULL, NULL);
    buf->bsize  = bsize;
    buf->mtype  = mtype;
    buf->mtext  = (char *) calloc(bsize, sizeof(char));
    CHECK_CONDITION_RET(buf->mtext == NULL, buf, NULL, NULL);
    return buf;
}

void delete_msgbuf(msgbuf *buf) {
    if (buf != NULL) {
        free(buf->mtext);
        free(buf);
    }
}

int msgq_translate_file(int fd, size_t file_size) {
    key_t key  = ftop("msgq", 0);
    int msg_id = msgget(key, IPC_CREAT | 0666);
    CHECK_CONDITION_RET(msg_id == -1, NULL, NULL, 1);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failure in msgq\n");
        return 1;

    } else if (pid > 0) {
        // int status;
        // wait(&status);
        // CHECK_CONDITION_RET(status != 0, NULL, NULL, 1);

        msgbuf *buf = construct_msgbuf(1, MSG_BUF_SIZE);

        size_t char_read = 0;
        size_t char_rcvd = 0;

        while ((char_read = msgrcv(msg_id, &buf, buf->bsize, 0, IPC_NOWAIT)) > 0) {
            char_rcvd += char_read;
            if (char_rcvd >= file_size) {
                break;
            }
        }

        delete_msgbuf(buf);

    } else {
        msgbuf *buf = construct_msgbuf(1, MSG_BUF_SIZE);

        size_t char_read = 0;
        size_t char_sent = 0;

        while ((char_read = read(fd, buf->mtext, buf->bsize)) > 0) {
            int ret = msgsnd(msg_id, &buf, char_read, IPC_NOWAIT);
            CHECK_CONDITION_RET(ret == 0, NULL, NULL, NULL)
            char_sent += char_read;
            if (char_sent >= file_size) {
                break;
            }
        }

        delete_msgbuf(buf);
        exit(0);
    }

    return 0;
}

#undef CHECK_CONDITION_RET
