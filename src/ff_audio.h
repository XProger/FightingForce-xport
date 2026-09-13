#ifndef FF_AUDIO_H
#define FF_AUDIO_H
#include "ff.h"
sint32 ff_audio_init(void);
void ff_audio_init_empty(void);
/* Cold SDK reset CPU/core path; platform IRQ/event setup is a host boundary. */
GDB_CALL uint32 ff_spu_reset_startup(void);
/* Master L/R, CD L/R and control register for the startup common adapter. */
extern uint16 ff_spu_common_state[5];
/* SPU1C0..1FE preset words, retained for the pending reverb mixer. */
extern uint16 ff_spu_reverb_registers[32];
/* Reverb output L/R and work-area register. */
extern uint16 ff_spu_reverb_state[3];
GDB_CALL sint32 ff_spu_reverb_attributes(uint32 attributes);
sint32 ff_spu_reverb_attributes_host(const uint32 attributes[5]);
extern uint16 ff_spu_reverb_channels_state[2];
GDB_CALL uint32 ff_spu_reverb_channels(sint32 enabled,uint32 mask);
GDB_CALL uint32 ff_spu_reverb_register_write(uint32 attributes);
GDB_CALL sint32 ff_spu_common_startup(uint32 attributes);
sint32 ff_spu_common_startup_host(const uint32 attributes[10]);
void ff_spu_cd_volume_host(sint16 left,sint16 right);
GDB_CALL sint32 ff_spu_transfer_mode(sint32 mode);
GDB_CALL sint32 ff_spu_reverb_conflict(uint32 address_units);
GDB_CALL sint32 ff_spu_reverb_reserve(sint32 enabled);
/* Control/register state only; the reverb mixer remains WIP. */
GDB_CALL sint32 ff_spu_reverb_enable(sint32 enabled);
uint32 ff_spu_transfer_address(uint32 address);
uint32 ff_spu_transfer_write(uint32 source,uint32 bytes);
void ff_spu_transfer_wait(void);
GDB_CALL sint32 ff_spu_allocation_init(sint32 entries,uint32 table);
GDB_CALL sint32 ff_spu_allocate_startup(uint32 bytes);
void ff_audio_render(sint16 *stereo,uint32 frames);
GDB_CALL uint16 ff_note_to_pitch(sint32 base,uint16 fine,sint32 note,uint16 note_fine);
#endif
