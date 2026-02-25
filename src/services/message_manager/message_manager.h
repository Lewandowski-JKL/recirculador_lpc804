#ifndef MESSAGE_MANAGER_H
#define MESSAGE_MANAGER_H


#define LengthMessageQueue  (4)

enum MessageStatus
{
    MessageSizeOverload=-1,
    MessageFail,
    MessageOk
};

typedef struct
{
    unsigned char addr;
    unsigned char len;
    unsigned char replyLen;
    unsigned char message[86];
    void *ptrNext;
    void (*ptrFunc)(unsigned char *data);
}myMessage;

//extern myMessage *messageQueue;

void deleteMessage(myMessage **ptr);
/*int addMessage(myMessage **ptrQueue, unsigned char addr, unsigned char *ptrMesssage, unsigned char len,
                unsigned char replyLen, void (*ptrFunc)(unsigned char *data));*/
int lenQueue(myMessage *ptrMessageQueue);
int sizeQueue(myMessage *ptrMessageQueue);
int addNewMessage(myMessage *ptrQueue, unsigned char addr, unsigned char *ptrMesssage, unsigned char len,
                unsigned char replyLen, void (*ptrFunc)(unsigned char *data));

#endif