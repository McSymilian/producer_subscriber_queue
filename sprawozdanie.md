---
title:    Publish-subscribe
subtitle: Programowanie systemowe i współbieżne
author:   Maksymilian Ryder 160345 \<<maksymilian.ryder@student.put.poznan.pl>\>
date:     v1.1, 2025-01-29
lang:     pl-PL
---

# Struktury danych


1. Elementy listy subskrybentów `SubscriberNode`:

   ```C
    typedef struct SubscriberNode {
      pthread_t data;
      struct SubscriberNode *prev;
      struct SubscriberNode *next;
    } SubscriberNode;
   ```

1. Zmienne `prev` i `next` wskazują odpowiednio na: następny i poprzedni element:

   ```C
   struct SubscriberNode *prev;
   struct SubscriberNode *next;
   ```

1. List subskrybentów `SubscribersList`:

   ```C
    typedef struct SubscribersList {
      SubscriberNode *head;
      SubscriberNode *tail;
      int size;
    } SubscribersList;
   ```

1. Zmienne `head` i `tail` wskazują odpowiednio na: pierwszy i ostatni element:

   ```C
    SubscriberNode *head;
    SubscriberNode *tail;
   ```
   Zmienna `size` mówi o wielkości listy:
   ```C
   int size;
   ```

1. Elementy listy wiadomości `MessageNode`:

   ```C
    typedef struct MessageNode {
      void *message;
      SubscribersList *subscribers;
      struct MessageNode *prev;
      struct MessageNode *next;
    } MessageNode;
   ```

1. Zmienne `prev` i `next` wskazują odpowiednio na: następny i poprzedni element:

   ```C
   struct MessageNode *prev;
   struct MessageNode *next;
   ```
   Zmienna `subscribers` wskazuje na listę subskrybentów którzy nie odebrali jeszcze wiadomości:
   ```C
   SubscribersList *subscribers;
   ```

1. List wiadomości `MessageList`:

   ```C
    typedef struct MessagesList {
      MessageNode *head;
      MessageNode *tail;
      int size;
    } MessagesList;
   ```

1. Zmienne `head` i `tail` wskazują odpowiednio na: pierwszy i ostatni element:

   ```C
   MessageNode *head;
   MessageNode *tail;
   ```
   Zmienna `size` mówi o wielkości listy:
   ```C
   int size;
   ```

1. Subskrybowalna kolejka wiadomości `TQueue`:
   ```C
    struct TQueue {
      int maxSize;
      struct SubscribersList *allSubscribers;
      struct MessagesList *messages;
 
      pthread_mutex_t operational_mutex;
      pthread_cond_t msg_posted_cond;
      pthread_cond_t msg_fully_received_cond;
    };
   ``` 

1. Zmienna `allSubscribers` to lista wszystkich aktualnych subskrybentów
   ```c
   struct SubscribersList *allSubscribers;
   ```
   Zmienna `maxSize` to aktualna maksymalna długości wiadmości w tej kolejce
   ```c
   int maxSize;
   ```
   Zmienna `messages` to lista wszystkich wiadomości
   ```c
   struct MessagesList *messages;
   ```
   Zmienna `operational_mutex` to kłódka umożliwiajaca synchronizację zapytań do kolejki
   ```c
   struct MessagesList *messages;
   ```
   Zmienna `msg_posted_cond` służy do informowania o nadaniu nowej wiadmości
   ```c
   pthread_cond_t msg_posted_cond;
   ```
   Zmienna `msg_fully_received_cond` służy do informowania o zwolnieniu miejsca w kolejce
   ```c
   pthread_cond_t msg_posted_cond;
   ```


# Funkcje


1. `SubscriberNode* createSubscriberNode(pthread_t subscriber)` -- stworzenie nowego elementu `SubscriberNode` zawierający podany wątek `subscriber`

1. `SubscribersList* createSubscribersList()` -- stworzenie nowej listy subskrybentów

1. `void appendSubscriber(SubscribersList *list, pthread_t subscriber)` -- dodanie do listy subskrybentów `list` nowego wątku `subscriber`

1. `SubscriberNode* findSubscriberBidirectional(SubscribersList *list, pthread_t subscriber)` -- wyszukiwanie dwukierunkowe elementu `subscriber` w liście subskrybentów 

1. `void removeSubscriber(SubscribersList *list, pthread_t subscriber)` -- usunięcie z listy subskrybentów `list` wątku `subscriber`

1. `void destroySubscribersList(SubscribersList *list)` -- usunięcie listy subskrybentów `list` wraz z czyszczeniem pamięci po jej elementach

1. `void copySubscribersList(SubscribersList *from, SubscribersList *to)` -- głębokie kopowianie zawartości listy subskrybentów `form` do listy subskrybentów `to`

1. `bool isSubscriber(SubscribersList *list, pthread_t subscriber)` -- sprawdzenie czy podany wątek `subscriber` należy do listy subskrybentów `list`

1. `MessageNode* createMessageNode(void *message, SubscribersList *subscribers)` -- stworzenie nowego elementu `MessageNode` zawierający wiadomość `message` z listą subskrybentów `subscribers`

1. `MessagesList* createMessagesList()` -- stworzenie nowej listy wiadomości

1. `void appendMessage(MessagesList *list, void *message, SubscribersList *subscribers)` -- dodanie do listy wiadomości `list` nowej wiadomości `message` z listą subskrybentów `subscribers`

1. `void removeMessageNode(MessagesList *list, MessageNode *node)` -- usunięcie z listy wiadomości `list` elementu `node`

1. `MessageNode* findMessageBidirectional(MessagesList *list, const void *message)` -- wyszukiwanie dwukierunkowe elementu `message` w liście wiadmości

1. `void removeMessage(MessagesList *list, void *message)` -- usunięcie z listy wiadomości `list` wiadomości `message`

1. `void destroyMessagesList(MessagesList *list)` -- usunięcie listy wiadomości `list` wraz z czyszczeniem pamięci po jej elementach


# Algorytm / dodatkowy opis


Dzięki synchronizacji względem `operational_mutex` każdego zapytania nie ma problemów z działaniami wielowątkowymi. 
Gdy wątek chce odebrać wiadomość, a nie jest subskrybentem, to `operational_mutex` jest zwalniany i zwracany jest NULL.
Jeśli wiątek pobiera wiadomość, a nie ma jeszcze nieodebranej wiadomości to jest usypiany i czeka na nadanie sygnały o dodaniu nowej wiadomości.
Gdy kolejka jest przepełniona, a chcemy wysłać nową wiadomość to wątek jest usypiany i czeka na sygnał o wolnym miejscy na wiadomość. 


# Przykład użycia

```C 
 TQueue *queue = createQueue(10);

 pthread_t* thread = malloc(sizeof(pthread_t));
 pthread_create(thread, NULL, threadBusyWait, NULL);
 subscribe(queue, *thread);

 int *msg = malloc(sizeof(int));
 *msg = 10;
 addMsg(queue, msg);

 const int available = getAvailable(queue, *thread);
 const int* msg2 = getMsg(queue, *thread);
 const int available2 = getAvailable(queue, *thread);
```
-------------------------------------------------------------------------------

Wyniki:
1. `*msg2` => 10

1. `available` => 1

1. `available2` => 0