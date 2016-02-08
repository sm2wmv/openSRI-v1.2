#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_

#include <stdint.h>

//! The size of the event queue
#define EVENT_LIST_SIZE 15

//! Event message used for timing of events
typedef struct {
	//!The function we wish to run at the specified time
	void (*func)(void);
	//!The target time where we wish to event to occur
	uint16_t time_target;
	//!The event id, can be used to drop a certain type of messages
	uint8_t id;
  //!The event priority flag
  uint8_t priority;
} EVENT_MESSAGE;

//! Event list with size EVENT_LIST_SIZE
EVENT_MESSAGE event_list[EVENT_LIST_SIZE];

/*! Sequencer message types, this is used to we can keep track of different
    messages in the event queue. So if an event is aborted we can easily just 
    remove the upcoming events from the queue, that doesn't/shouldn't need to be executed */

//! Event that the inhibit output should be on
#define SEQUENCER_EVENT_TYPE_PTT_INHIBIT_ON         1
//! Event that the radio should be PTT:ed
#define SEQUENCER_EVENT_TYPE_PTT_RADIO_ON           2
//! Event that the amp should be PTT:ed
#define SEQUENCER_EVENT_TYPE_PTT_AMP_ON             3
//! Event that the antenna should be deactivated
#define SEQUENCER_EVENT_TYPE_PTT_ANT_ON             4
//! Event that the inhibit should be deactiated
#define SEQUENCER_EVENT_TYPE_PTT_INHIBIT_OFF        5
//! Event that the radio should be deactivated
#define SEQUENCER_EVENT_TYPE_PTT_RADIO_OFF          6
//! Event that the amp should be deactivated
#define SEQUENCER_EVENT_TYPE_PTT_AMP_OFF            7
//! Event that the antenna should be deactivated
#define SEQUENCER_EVENT_TYPE_PTT_ANT_OFF            8

#define EEPROM_EVENT_TYPE_WRITE                     9

//! We are ok to run the event from the main loop
#define EVENT_PRIORITY_LEVEL_MAINLOOP               0
//! Timing critical, will be run from the interrupt. However in this mode
//! it is not allowed to take up a lot of execution time
#define EVENT_PRIORITY_LEVEL_REALTIME               1

void event_queue_init(void);
int8_t event_queue_add(EVENT_MESSAGE event);
EVENT_MESSAGE event_queue_get(void);
void event_queue_drop(void);
unsigned char event_queue_count(void);
void event_queue_dropall(void);
unsigned char event_in_queue(void);
void event_queue_wrap(unsigned int remove_val);
int event_queue_drop_id(unsigned char id);
unsigned char event_queue_check_id(unsigned char id);

#endif
