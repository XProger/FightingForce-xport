#ifndef FF_PLATFORM_DUMMY_H
#define FF_PLATFORM_DUMMY_H
#include "psx.h"
sint32 ff_dummy_cd_search(void *file_info,const char *disc_name);
/* WIP host boundaries. No physical media or memory-card persistence. */
sint32 ff_dummy_archive_read(const char *disc_name,void *destination,uint32 capacity);
extern uint32 ff_dummy_calls[3];
extern uint32 ff_dummy_card_events[8][5];
extern uint8 ff_dummy_card_pending[8];
void ff_dummy_card_request_no_media(uint32 group);
sint32 ff_dummy_card_test_event(uint32 handle);
void ff_dummy_card_events_reset(void);
uint32 ff_dummy_card_open_event(uint32 descriptor,uint32 spec,uint32 mode,uint32 callback);
sint32 ff_dummy_card_enable_event(uint32 handle);
sint32 ff_dummy_cd_get_toc(uint8 *destination);
sint32 ff_dummy_memory_card_read(sint32 slot,void *data,uint32 bytes);
sint32 ff_dummy_cd_read(uint32 sector,void *data,uint32 bytes);
sint32 ff_dummy_cd_mix(uint8 volume);
extern uint8 ff_dummy_cd_mix_state[4],ff_dummy_cd_track_state[4];
sint32 ff_dummy_cd_control(uint32 command,const uint8 *parameter,uint8 *result);
sint32 ff_dummy_cd_pause(void);
sint32 ff_dummy_str_decode(const void *data,uint32 bytes);
#endif
