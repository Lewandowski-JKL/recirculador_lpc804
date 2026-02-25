#include "message_manager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*myMessage *_newMessage(unsigned char addr, unsigned char *ptrMesssage, unsigned char len,
                unsigned char replyLen, void (*ptrFunc)(unsigned char *data))
{
    myMessage *ptrAux = (myMessage*)malloc(sizeof(myMessage));
    ptrAux->addr = addr;
    ptrAux->message = (unsigned char*)malloc(sizeof(unsigned char)*len);
    memcpy(&(ptrAux->message), ptrMesssage, len);
    ptrAux->len = len;
    ptrAux->replyLen = replyLen;
    ptrAux->ptrFunc = ptrFunc;
    ptrAux->ptrNext = NULL;
    return ptrAux;
}*/
/**
 * @brief Adiciona uma nova mensagem a fila
 * 
 * @param ptrQueue Fila de mensagens
 * @param addr endereço que será destinada a mensagem
 * @param ptrMesssage mensagem que será adicionada
 * @param len tamanho da mensagem
 * @param replyLen tamanho da resposta
 * @param ptrFunc função que irá tratar a resposta
 * @return int 
 */
int addNewMessage(myMessage *ptrQueue, unsigned char addr, unsigned char *ptrMesssage, unsigned char len,
                unsigned char replyLen, void (*ptrFunc)(unsigned char *data))
{
    myMessage *ptrAux = ptrQueue;
    if (lenQueue(ptrAux) >= LengthMessageQueue)
        return MessageSizeOverload;
    int i = 0;
    while (ptrAux->len)
    {
        i++;
        ptrAux = (myMessage *)ptrAux->ptrNext;
        if (i >= LengthMessageQueue)
            return 0;
    }
    ptrAux->addr=addr;
    ptrAux->len = len;
    ptrAux->replyLen = replyLen;
    ptrAux->ptrFunc = ptrFunc;
    memcpy(ptrAux->message, ptrMesssage, len);
    return MessageOk;
}
/**
 * @brief 'Deleta' a mensagem
 *          Na pratica zera os valores e aponta pra a proxima mensagem.
 *          Inicialmente era utilizado alocação dinamica mas o micro não esta aceitando muito bem esse processo
 * @param ptr mensagem que será deletada
 */
void deleteMessage(myMessage **ptr)
{
    (*ptr)->addr = 0;
    (*ptr)->len = 0;
    (*ptr)->ptrFunc = NULL;
    (*ptr)->replyLen = 0;
    //free((*ptrQueue)->message);
    (*ptr) = (myMessage*)((*ptr)->ptrNext);
    /*myMessage *ptrAux = *ptrMessageQueue;
    ptrMessageQueue = (myMessage**)(&(*ptrMessageQueue)->ptrNext);
    free(ptrAux->message);
    free(ptrAux);*/
}
/*int addMessage(myMessage **ptrQueue, unsigned char addr, unsigned char *ptrMesssage, unsigned char len,
                unsigned char replyLen, void (*ptrFunc)(unsigned char *data))
{
    myMessage **ptrAux = ptrQueue;
    int sizeAux = sizeQueue(*ptrAux)+20+len;
    if (sizeAux>1024)
        return MessageSizeOverload;
    int i = 0;
    while (*ptrAux != NULL)
    {
        i++;
        ptrAux = (myMessage **)&((*ptrAux)->ptrNext);
        if (i == 0xFF)
            return MessageSizeOverload;
    }
    *ptrAux = _newMessage(addr, ptrMesssage, len, replyLen, ptrFunc);
    return MessageOk;
}*/
/**
 * @brief Retorna o tamanho da fila de mensagem
 * 
 * @param ptrMessageQueue fila de mensagem
 * @return int 
 */
int lenQueue(myMessage *ptrMessageQueue)
{
    int i = 0;
    myMessage *ptrAux = ptrMessageQueue;
    while (ptrAux->len)
    {
        i++;
        ptrAux = (myMessage*)(ptrAux->ptrNext);
        if (i >= LengthMessageQueue)
            return LengthMessageQueue;
    }
    return i;
}
/**
 * @brief Retorna o tanto de memoria alocada
 * 
 * @param ptrMessageQueue fila de mensagem
 * @return int 
 */
int sizeQueue(myMessage *ptrMessageQueue)
{
    int sizeAux = 0;
    short i = 0;
    myMessage *ptrAux = ptrMessageQueue;
    while (ptrAux->len)
    {
        i++;
        sizeAux += sizeof(myMessage);
        ptrAux = (myMessage*)(ptrAux->ptrNext);
        if (i >= LengthMessageQueue)
            return MessageSizeOverload;
    }
    return sizeAux;
}

