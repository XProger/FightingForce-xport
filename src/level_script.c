#include "ff.h"
#include "wip.h"
#include <stdio.h>
#include <stdlib.h>
#define script_wip_zero(pc) do { if(ff_wip_visit((pc),"level_script",__FUNCTION__,__FILE__,__LINE__,1,"return_zero_from_function"))return 0;script_wip_abort(pc); } while(0)
#define script_wip_void(pc) do { if(ff_wip_visit((pc),"level_script",__FUNCTION__,__FILE__,__LINE__,1,"return_from_function"))return;script_wip_abort(pc); } while(0)
#define script_wip_fatal(pc) do { ff_wip_visit((pc),"level_script",__FUNCTION__,__FILE__,__LINE__,0,"abort");script_wip_abort(pc); } while(0)
static void script_wip_abort(uint32 pc){fprintf(stderr,"WIP level script4B228 branch %08X\n",pc);abort();}
GDB_CALL sint32 FUN_8004BE28(void) {
 sint32 count=0,type;uint32 i,actor;
 for(i=2;i<8;i++){actor=0x800b4318+244u*i;type=ff_s16(actor+52);if(type!=51&&type!=-1&&ff_s16(actor+66)>0)++count;}
 return count;
}
/* 4B228 command1, delay and ordinary NPC creation, audited against MIPS.
 * Vehicle companions, special entrances and other commands remain explicit WIP. */
static sint32 script_word(void) {
 uint32 p=ff_u32(0x800946b8);ff_w32(0x800946b8,p+2);return ff_s16(p);
}
static sint32 script_weapon(sint32 type,uint32 capability,uint32 x,uint32 z) {
 uint32 bits;if(type==-1)return -1;
 bits=ff_u32(0x800846b8+4u*(uint32)type);
 if((bits&ff_u32(capability+12))!=(bits&0xffffu))return -1;
 return FUN_800167C4(type,x,1,z,0);
}
static void script_create(void) {
 sint32 type,xword,zword,yaw,flag,timer,index,role,target,behavior,item,animation;
 uint32 x,z,table,actor,capability,stats;
 type=script_word();xword=script_word();zword=script_word();yaw=script_word();flag=script_word();timer=script_word();
 x=(uint32)xword<<16;z=(uint32)zword<<16;
 if(*(sint8*)ff_ptr(0x80093e00,1)){x+=ff_u32(0x80093df4)<<16;z+=ff_u32(0x80093df8)<<16;}
 table=ff_u32(0x800b3be8+4u*(uint32)type);
 index=FUN_8004AF18(type,x,z,yaw,flag,timer,0);
 /* Original indexes -1 unchecked if full: fail explicitly outside supported state. */
 if(index<2||index>=8)script_wip_fatal(0x8004b938);
 actor=0x800b4318+244u*(uint32)index;
 if(type==44||type==45||type==46)script_wip_fatal(0x8004b97c);
 ff_w32(actor+160,type==28?0x800b3e90:type==32?0x8009cd48:type==48?0x80099f40:type==50?0x8009e3d0:0x800aee90);
 role=script_word();target=script_word();ff_w32(actor+168,(uint32)target);
 if((uint32)role-1u<2u&&target>=(sint32)ff_u32(0x800940b0)) {ff_w32(actor+168,ff_u32(0x800940b0)-1u);role=2;}
 ff_w32(actor+164,(uint32)role);stats=ff_u32(actor+180);
 behavior=script_word();ff_w32(actor+184,(uint32)behavior);ff_w32(actor+188,0);
 capability=0x80084f84+16u*(uint32)*(uint8*)ff_ptr(stats+8u*(uint32)behavior,1);
 item=script_word();*(uint8*)ff_ptr(actor+126,1)=(uint8)script_weapon(item,capability,x,z);
 item=script_word();*(uint8*)ff_ptr(actor+125,1)=(uint8)script_weapon(item,capability,x,z);
 animation=script_word();
 if(animation==-1){ff_w32(actor,0);ff_w32(actor+12,0xffffffffu);ff_w32(actor+4,0xffffffffu);}
 else {
  if(animation==203||animation==205||animation==207||animation==287)script_wip_fatal(0x8004bbdc);
  FUN_8004C4D8(actor,table,animation);
  FUN_80011A50(ff_u32(0x80081720+4u*(uint32)(sint32)ff_s16(actor+52)),actor,0x800ba210+504u*(uint32)index);
  FUN_8001AE7C(actor);
 }
 if(*(sint8*)ff_ptr(0x80093e00,1)&&ff_u32(ff_u32(0x800940ac))==782&&ff_u32(0x80093d60)==1) {
  ff_w16(actor+52,0xffff);ff_w32(0x80093dbc,ff_u32(0x80093dbc)-1u);
 }
 ff_w32(0x80093dc0,ff_u32(0x80093dc0)-1u);
}
GDB_CALL sint32 FUN_8004B228(void) {
 uint32 p,remaining; sint32 timer,command,a,b,c;
 timer=(sint32)ff_u32(0x80093d40);
 if(timer>0){timer=(sint32)((uint32)timer+1u);ff_w32(0x80093d40,(uint32)timer);if(timer>=31)script_wip_fatal(0x8004b270);}
 if(*(sint8*)ff_ptr(0x80093dd9,1))return 0;
 if((uint32)FUN_8004BE28()==ff_u32(0x80093e1c))return 0;
 if(!ff_u32(0x80093dbc)) {
  p=ff_u32(0x800946b8);command=ff_s16(p);if(command==-1)return -1;
  ff_w32(0x800946b8,p+2);command=ff_s16(p);
  if(command==3) {
   /* 4B328..4B354/4B3B0, then shared group parameters at4B784. */
   ff_w8(0x80093dd9,1);
   if(ff_u32(0x80093d58)==6)ff_w32(0x80093d40,1);
   else ff_w32(0x80093d60,1);
  } else if(command!=1)script_wip_fatal(0x8004b320);
  p=ff_u32(0x800946b8);ff_w32(0x800946b8,p+2);a=ff_s16(p);
  ff_w32(0x800946b8,p+4);ff_w32(0x80093dbc,(uint32)a);b=ff_s16(p+2);
  ff_w32(0x800946b8,p+6);ff_w32(0x80093e1c,(uint32)b);c=ff_s16(p+4);
  ff_w32(0x80094798,0xffffffffu);ff_w32(0x800947d8,0);ff_w32(0x80093dc0,(uint32)a);
  ff_w32(0x80093e18,(uint32)c);ff_w32(0x800944dc,(uint32)c);return 0;
 }
 if((sint32)ff_u32(0x80093dc0)<=0)return 0;
 remaining=ff_u32(0x80093dc4);
 if(!remaining){p=ff_u32(0x800946b8);ff_w32(0x800946b8,p+2);ff_w32(0x80093dc4,(uint32)(sint32)ff_s16(p));return 0;}
 --remaining;ff_w32(0x80093dc4,remaining);if(remaining)return 0;
 script_create();return 0;
}
