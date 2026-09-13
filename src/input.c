#include "ff.h"
#include <string.h>
/* 5F558: raw gameplay buttons; the first mapping wins if both IDs match. */
GDB_CALL uint32 FUN_8005F558(uint32 controller) {
 uint32 value;
 if(controller==ff_u32(0x80094288))value=ff_u32(0x80093a10);
 else if(controller==ff_u32(0x8009428c))value=ff_u32(0x80093a14);
 else return 0;
 ff_w8(0x80094114,(uint8)(value>>16));return value&65535;
}
static uint32 gameplay_action_address(uint32 player,uint32 mapping) {
 sint32 config=*(sint8*)ff_ptr(0x80094480+player,1);
 sint32 action=*(sint8*)ff_ptr(mapping+6u*(uint32)config,1);
 return 0x80094100+6u*player+(uint32)action;
}
/* 5F8F0: decay six action bytes, then latch new face-button edges for12 ticks.
 * Preserve write order and the original address-or-zero return register. */
GDB_CALL uint32 FUN_8005F8F0(uint32 player,uint32 buttons) {
 uint32 base=0x80094100+6u*player,i,address;uint8 value;
 for(i=0;i<6;i++){value=*(uint8*)ff_ptr(base+i,1);if(value)ff_w8(base+i,(uint8)(value-1));}
 if(buttons&0x8000){address=gameplay_action_address(player,0x80093d7a);ff_w8(address,12);}
 if(buttons&0x4000){address=gameplay_action_address(player,0x80093d78);ff_w8(address,12);}
 if(buttons&0x2000){address=gameplay_action_address(player,0x80093d79);ff_w8(address,12);}
 if(buttons&0x1000){address=gameplay_action_address(player,0x80093d7b);ff_w8(address,12);return address;}
 return 0;
}
GDB_CALL uint32 FUN_8005FA58(uint32 player,uint32 buttons) {
 uint32 address;
 if(buttons&0x400){address=gameplay_action_address(player,0x80093d7c);ff_w8(address,1);}
 if(buttons&0x800){address=gameplay_action_address(player,0x80093d7d);ff_w8(address,1);return address;}
 return 0;
}
uint32 ff_host_pad_state[6]={0x80094748,34,0x80094770,34,1,0};
/* InitPAD host boundary: register neutral disconnected buffers; host frame
 * polling supplies actual digital input before any game input decoder runs. */
void ff_host_pad_init(uint32 first,uint32 first_size,uint32 second,uint32 second_size) {
 uint32 p[2]={first,second},n[2]={first_size,second_size},i;
 PadInit(0);
 ff_host_pad_state[0]=first;ff_host_pad_state[1]=first_size;
 ff_host_pad_state[2]=second;ff_host_pad_state[3]=second_size;
 ff_host_pad_state[4]=1;ff_host_pad_state[5]=0;
 for(i=0;i<2;i++) {
  if(n[i])memset(ff_ptr(p[i],n[i]),128,n[i]);
  if(n[i]>0)ff_w8(p[i],255);if(n[i]>1)ff_w8(p[i]+1,0x40);if(n[i]>3)ff_w16(p[i]+2,65535);
 }
}
void ff_host_pad_publish(uint32 connected,uint32 first,uint32 second) {
 uint32 i,p,n,buttons[2]={first,second},packed=0xffffffffu;
 if(!ff_host_pad_state[4])return;
 for(i=0;i<2;++i){
  p=ff_host_pad_state[2*i];n=ff_host_pad_state[2*i+1];
  if(n){memset(ff_ptr(p,n),128,n);ff_w8(p,connected>i?0:255);}
  if(n>1)ff_w8(p+1,0x40);if(n>3)ff_w16(p+2,(uint16)~(connected>i?buttons[i]:0));
  if(connected>i){uint32 bits=(~(((buttons[i]&255)<<8)|((buttons[i]>>8)&255)))&65535;packed=(packed&~(65535u<<(16*i)))|(bits<<(16*i));}
 }
 if(ff_host_pad_state[5])ff_w32(ff_host_pad_state[5],packed);
}
static sint32 host_pad_high_init(uint32 type,uint32 buffer) {
 if(type!=0x20000000&&type!=0x20000001)return 0;
 ff_host_pad_init(0x801fd000,34,0x801fd040,34);ff_host_pad_state[5]=buffer;return 2;
}
GDB_CALL void FUN_8006CE70(void) {FF_FUNCTION_MARKER(0x8006ce70u,"GAME.EXE");ff_host_pad_state[4]=0;}
GDB_CALL sint32 FUN_8006D1A0(uint32 first,uint32 first_size,uint32 second,uint32 second_size) {FF_FUNCTION_MARKER(0x8006d1a0u,"GAME.EXE");ff_host_pad_init(first,first_size,second,second_size);return 1;}
GDB_CALL void FUN_8006D1B0(void) {FF_FUNCTION_MARKER(0x8006d1b0u,"GAME.EXE");ff_host_pad_state[4]=1;}
GDB_CALL sint32 FUN_8006D1C0(uint32 type,uint32 buffer) {FF_FUNCTION_MARKER(0x8006d1c0u,"GAME.EXE");return host_pad_high_init(type,buffer);}
GDB_CALL sint32 FUN_SLUS_80013A6C(uint32 first,uint32 first_size,uint32 second,uint32 second_size) {FF_FUNCTION_MARKER(0x80013a6cu,"SLUS_004.33");ff_host_pad_init(first,first_size,second,second_size);return 1;}
GDB_CALL void FUN_SLUS_80013A7C(void) {FF_FUNCTION_MARKER(0x80013a7cu,"SLUS_004.33");ff_host_pad_state[4]=1;}
GDB_CALL void FUN_SLUS_80013A8C(void) {FF_FUNCTION_MARKER(0x80013a8cu,"SLUS_004.33");ff_host_pad_state[4]=0;}
GDB_CALL sint32 FUN_SLUS_80013A9C(uint32 type,uint32 buffer) {FF_FUNCTION_MARKER(0x80013a9cu,"SLUS_004.33");return host_pad_high_init(type,buffer);}
uint32 FUN_8005FB00(sint32 player) {
 uint32 address=0x80094100+6u*(uint32)player;memset(ff_ptr(address,6),0,6);return address;
}
/* 80068114: mapped controller edges, deliberately does not write 800927A0. */
GDB_CALL uint32 FUN_80068114(sint32 player) {
 if(ff_services.audit_input_event)ff_services.audit_input_event(2);
 uint32 offset=(uint32)player*4,raw=(uint32)FUN_8005F5AC((sint32)ff_u32(0x80094288+offset));
 uint32 edge=(raw^ff_u32(0x80092790+offset))&raw;
 ff_w32(0x80092798+offset,edge);ff_w32(0x80092790+offset,raw);return edge;
}
#include <stdlib.h>
#include <string.h>
FF_SERVICES ff_services;
static uint8 read_byte(uint32 a) { return *(uint8*)ff_ptr(a,1); }
static void write_byte(uint32 a,uint8 v) { *(uint8*)ff_ptr(a,1)=v; }

/* GAME.EXE:800573A8. The second pad is counted only if the first is valid. */
GDB_CALL sint32 FUN_800573A8(void) {
 uint32 pad=ff_u32(0x8009355c);uint8 type=read_byte(pad+1)>>4;
 if((type!=4&&type!=7&&type!=5)||read_byte(pad)==255)return 0;
 pad=ff_u32(0x80093560);type=read_byte(pad+1)>>4;
 return ((type==4||type==7||type==5)&&read_byte(pad)!=255)?2:1;
}
/* GAME.EXE:8005EC90. Return is always one, not the controller count. */
GDB_CALL sint32 FUN_8005EC90(void) {
 ff_w32(0x8009400c,(uint32)FUN_800573A8());
 ff_w32(0x80094288,0);ff_w32(0x8009428c,1);return 1;
}
/* GAME.EXE:80056ECC. SDK SpuSetKey(0, all voices), then memset of voice flags.
 * Runtime SPU binding is WIP; no sound dummy is installed. */
GDB_CALL uint32 FUN_80056ECC(void) {
 if(!ff_services.spu_key)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 ff_services.spu_key(0,0xffffffu);
 memset(ff_ptr(0x80094708,24),0,24);return 0x80094708;
}
/* GAME.EXE:8005F5AC. Complete input decoder including grace counters and reset.
 * The reset path does not return: IDA's post-longjmp store is unreachable. */
GDB_CALL sint32 FUN_8005F5AC(sint32 controller) {
 if(ff_services.audit_input_event)ff_services.audit_input_event(0);
 uint32 index=(uint32)controller,pad=controller?0x80094770:0x80094748;
 uint32 disconnect=0x80093a1c+index,unsupported=0x80093a20+index;
 sint16 buttons=(sint16)~(uint16)ff_s16(pad+2);uint8 type=read_byte(pad+1)>>4;
 write_byte(0x80094114,type);
 if(buttons==-1)buttons=0;
 if(read_byte(pad)==255) {
  type=255;
  if((sint8)read_byte(disconnect)<0){write_byte(disconnect,(uint8)(read_byte(disconnect)+1));type=4;}
  write_byte(0x80094114,type);buttons=0;
 }else write_byte(disconnect,0);
 if(type==4)write_byte(unsupported,0);
 else if(type==7||type==5) {
  sint32 x=(sint32)read_byte(pad+6)-128,y=(sint32)read_byte(pad+7)-128;
  write_byte(unsupported,0);write_byte(0x80094114,22);
  if(x>0){x-=32;if(x<0)x=0;}else{x+=32;if(x>0)x=0;}
  if(y>0){y-=32;if(y<0)y=0;}else{y+=32;if(y>0)y=0;}
  ff_w32(0x800945d4,(uint32)((x*341)>>8));ff_w32(0x800945f8,(uint32)((y*341)>>8));
 }else {
  type=255;
  if((sint8)read_byte(unsupported)<15){write_byte(unsupported,(uint8)(read_byte(unsupported)+1));type=4;}
  write_byte(0x80094114,type);buttons=0;
 }
 if((buttons&9)==9 && !read_byte(0x80093cf8)) {
  sint16 count=(sint16)((uint16)ff_s16(0x80093a18+2u*index)+1u);
  ff_w16(0x80093a18+2u*index,(uint16)count);
  if(count>=31) {
   ff_w16(0x80093a1a,0);ff_w16(0x80093a18,0);FUN_80056ECC();
   ff_w32(0x80093c14,0xffffffffu);write_byte(0x80093cf8,1);
   if(!ff_services.game_longjmp)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
   ff_services.game_longjmp(0x80094298,0);ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
  }
 }else ff_w16(0x80093a18+2u*index,0);
 return buttons;
}
