#include "ff.h"
#include <stdlib.h>
/* 56F94 ignores a1: preserve the original two successive master products. */
GDB_CALL sint32 FUN_80056F94(sint32 voice,sint32 unused,sint32 right) {
 uint32 master=*(uint8*)ff_ptr(0x80093d6e,1),attr; sint32 first,second,result;
 (void)unused;first=(sint32)((uint32)right*master);attr=FUN_80056640();
 second=(sint32)((uint32)(first>>8)*master);
 ff_w32(attr+4,3);ff_w32(attr,1u<<((uint32)voice&31));
 ff_w16(attr+8,(uint16)((uint32)(first>>8)<<6));ff_w16(attr+10,(uint16)((uint32)(second>>8)<<6));
 if(!ff_services.spu_voice_attributes)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);result=ff_services.spu_voice_attributes(attr);
 ff_w8(0x800946c0+(uint32)voice,(uint8)(first>>9));ff_w8(0x80094720+(uint32)voice,(uint8)(second>>9));return result;
}
GDB_CALL sint32 FUN_8005703C(void) {
 sint32 i,type;uint32 attr;
 for(i=0;i<24;i++){
  type=*(sint8*)ff_ptr(0x80094708+(uint32)i,1);
  if(type==1)FUN_800567F8(i);
  else if(type==2){attr=FUN_80056640();ff_w32(attr+4,3);ff_w32(attr,1u<<i);ff_w16(attr+10,0);ff_w16(attr+8,0);
   if(!ff_services.spu_voice_attributes)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);ff_services.spu_voice_attributes(attr);}
 }return 0;
}
GDB_CALL sint32 FUN_800570F0(void) {
 sint32 i;
 for(i=0;i<24;i++)if(*(sint8*)ff_ptr(0x80094708+(uint32)i,1)==2)
  FUN_80056F94(i,2*(sint32)*(sint8*)ff_ptr(0x800946c0+(uint32)i,1),2*(sint32)*(sint8*)ff_ptr(0x80094720+(uint32)i,1));
 return 0;
}
#include "ff_audio.h"
#include "platform_dummy.h"
#include <stdlib.h>
GDB_CALL sint32 FUN_800675FC(void) {
 sint32 count=ff_dummy_cd_get_toc((uint8*)ff_ptr(0x80094948,112));
 ff_w32(0x80093ba8,(uint32)count);
 if(count<=0)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__); /* Original67620 spins forever; fixed disc TOC is positive. */
 return FUN_80067640(0);
}
sint32 FUN_80067640(sint32 volume){return ff_dummy_cd_mix((uint8)volume);}
sint32 FUN_800569BC(sint32 volume){return FUN_80067640(volume);}
sint32 FUN_80067670(sint32 asynchronous,sint32 track) {
 uint8 mode=7,status=0;sint32 result;
 ff_dummy_cd_control(14,&mode,NULL);
 result=ff_dummy_cd_control(3,(const uint8*)ff_ptr(0x80094948+4u*(uint32)track,4),NULL);
 ff_w32(0x80093bb0,(uint32)asynchronous);ff_w32(0x80093bac,(uint32)track);ff_w32(0x80093bb4,0);
 if(!asynchronous) {
  do {while(!ff_dummy_cd_control(1,&mode,&status)){};}while(!(status&0x80)||(status&0x40));
  result=status&0x40;
 }
 return result;
}
sint32 FUN_80063870(sint32 track) {
 FUN_80067640(0);FUN_80067670(1,track);return FUN_800569BC(*(uint8*)ff_ptr(0x80093d6f,1));
}
/* WIP dummy for729DC CD hardware reset. Native archives use host files. */
static void ff_cd_reset_WIP(void) {}
sint32 FUN_80056CCC(sint32 archive) {
 uint32 i;for(i=0;i<246;i++)ff_w32(0x800b6e8c-4*i,0xffffffff);
 FUN_80056BC8();ff_cd_reset_WIP();FUN_80061448(archive,0x116000);
 return ff_sound_resources_80056D2C();
}
sint32 FUN_800569DC(void) {
 const uint32 a[5]={1,0,0,0,0};
 if(!ff_u32(0x80093558))return 0;
 ff_spu_reverb_attributes_host(a);return ff_spu_reverb_enable(0);
}
sint32 FUN_80056BC8(void) {
 uint32 slot=FUN_80056640();const uint32 common[10]={0x2c3,0x3fff3fff,0,0,0x3fff3fff,0,1,0,0,0};
 FUN_800569DC();FUN_8007676C();ff_spu_allocation_init(2,0x80094820);
 ff_spu_common_startup_host(common);ff_spu_transfer_mode(0);ff_sound_voice_defaults_80056C30(slot);
 return FUN_80056A18();
}
/* GAME56A18: choose stage preset, then configure output/reservation/channels. */
sint32 FUN_80056A18(void) {
 uint32 stage=ff_u32(0x80093d58),table=0x8007ffd0,a[5]={0};sint32 enabled;
 ff_w32(0x80093558,1);
 if(stage==1||stage==2||stage==3||stage==7||stage==10||stage==11||stage==17)table=0x8007ffe8;
 else if(stage==16||stage==21||stage==24)table=0x80080000;
 else if(stage==12||stage==13||stage==22||stage==23)table=0x80080018;
 a[0]=0x19;a[1]=ff_u32(table+12);a[3]=ff_u32(table+16);a[4]=ff_u32(table+20);
 ff_spu_reverb_attributes_host(a);
 a[0]=6;a[2]=(ff_u32(table+4)&65535)|(ff_u32(table+8)<<16);ff_spu_reverb_attributes_host(a);
 enabled=(sint32)ff_u32(table);ff_spu_reverb_reserve(enabled);ff_spu_reverb_enable(enabled);ff_spu_reverb_channels(enabled,0xffffff);
 return ff_u32(0x80093d58)==17?FUN_80056BA0():17;
}
GDB_CALL sint32 FUN_80056B78(void){
 FF_FUNCTION_MARKER(0x80056b78u,"GAME.EXE");
 return (sint32)ff_spu_reverb_channels(1,0xffffff);
}
sint32 FUN_80056BA0(void){return (sint32)ff_spu_reverb_channels(0,0xffffff);}
/* 56BC8:80056C30..80056CA8, after s0=3FFF/s1=1 and slot selection. */
sint32 ff_sound_voice_defaults_80056C30(uint32 a) {
 uint32 i;
 ff_w32(a+4,0xff1f);ff_w16(a+20,0x1000);ff_w32(a,0xffffff);
 ff_w16(a+8,0x3fff);ff_w16(a+10,0x3fff);ff_w16(a+14,0);ff_w16(a+12,0);
 ff_w32(a+36,1);ff_w32(a+40,1);ff_w32(a+44,3);
 for(i=48;i<=56;i+=2)ff_w16(a+i,0);
 if(!ff_services.spu_voice_attributes||!ff_services.spu_key)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 ff_services.spu_voice_attributes(a);
 for(i=0;i<24;i++)ff_services.spu_key(0,1u<<i);
 /* 56C9C slti overwrites v0: the final loop condition is false. */
 return 0;
}
/* 56CCC:80056D2C..80056EA8. Caller has reset offsets/SPU allocator and
 * loaded the sound archive. Hardware initialization prefix remains WIP. */
sint32 ff_sound_resources_80056D2C(void) {
 uint32 table,name,id,size,total=0,offset=0,source;
 for(table=0x8007f848;ff_u32(table+4)!=0xffffffff;table+=8) {
  name=ff_u32(table);if(!FUN_80061140(name))continue;
  FUN_800611F8(name);size=(ff_u32(0x80094570)+15)&~15u;
  total=total-48+size;ff_w32(0x80094570,size);
 }
 ff_w32(0x8009354c,(uint32)ff_spu_allocate_startup(total+1024));ff_spu_transfer_wait();
 for(table=0x8007f848;ff_u32(table+4)!=0xffffffff;table+=8) {
  name=ff_u32(table);id=ff_u32(table+4);if(!FUN_80061140(name))continue;
  source=FUN_800611F8(name);size=(ff_u32(0x80094570)+15)&~15u;
  ff_w32(0x800b6ab8+4*id,offset);ff_w32(0x80094570,size);
  ff_spu_transfer_address(ff_u32(0x8009354c)+offset);ff_spu_transfer_write(source+48,size-48);ff_spu_transfer_wait();
  offset=offset-48+ff_u32(0x80094570);
  *(uint8*)ff_ptr(0x800b6f68+id,1)=(*(uint8*)ff_ptr(name,1)=='L'&&*(uint8*)ff_ptr(name+1,1)=='_')?2:1;
 }
 return -1;
}
/* GAME.EXE:80056640. Return old slot, increment/wrap the signed ring cursor. */
GDB_CALL uint32 FUN_80056640(void) {
 uint32 old=ff_u32(0x80093550),next=old+1;
 ff_w32(0x80093550,(sint32)next>=16?0:next);return 0x800a79c8+(old<<6);
}
/* GAME.EXE:80056F08. Products wrap before signed shifts; volume stores are SH. */
GDB_CALL sint32 FUN_80056F08(sint32 voice,sint32 volume) {
 sint32 product=(sint32)((uint32)volume * *(uint8*)ff_ptr(0x80093d6e,1)),result;
 uint32 attr=FUN_80056640(),level=(uint32)(product>>8)<<6;
 ff_w32(attr+4,3);ff_w32(attr,1u<<((uint32)voice&31));ff_w16(attr+8,(uint16)level);ff_w16(attr+10,(uint16)level);
 if(!ff_services.spu_voice_attributes)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);result=ff_services.spu_voice_attributes(attr);
 *(uint8*)ff_ptr(0x800946c0+(uint32)voice,1)=(uint8)(product>>9);
 *(uint8*)ff_ptr(0x80094720+(uint32)voice,1)=(uint8)(product>>9);return result;
}
/* GAME.EXE:800567F8. Delay slot explicitly passes volume=0 to 80056F08. */
GDB_CALL sint32 FUN_800567F8(sint32 voice) {
 *(uint8*)ff_ptr(0x80094708+(uint32)voice,1)=0;FUN_80056F08(voice,0);
 if(!ff_services.spu_key)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return ff_services.spu_key(0,1u<<((uint32)voice&31));
}
/* GAME.EXE:80056670. Voice search is the original circular allocator; reserved
 * flag 2 is skipped. The original has no termination when all voices are reserved. */
GDB_CALL sint32 FUN_80056670(sint32 effect,sint32 volume,sint32 pitch) {
 uint32 master=*(uint8*)ff_ptr(0x80093d6e,1),attr=FUN_80056640(),voice,sample;
 do {
  voice=ff_u32(0x80093554)+1;if((sint32)voice>=24)voice=0;ff_w32(0x80093554,voice);
 }while(*(sint8*)ff_ptr(0x80094708+voice,1)==2);
 sample=ff_u32(0x800b6ab8+4u*(uint32)effect);
 if(sample==0xffffffffu)FUN_800567F8((sint32)voice);
 else {
  sint32 level=(sint32)((uint32)(volume>>1)*master)>>8;
  sint32 scaled=(sint32)((uint32)pitch*1536u);
  uint32 note=(uint32)(scaled/1024)-1536u;
  *(uint8*)ff_ptr(0x80094708+voice,1)=*(uint8*)ff_ptr(0x800b6f68+(uint32)effect,1);
  *(uint8*)ff_ptr(0x80094720+voice,1)=(uint8)level;*(uint8*)ff_ptr(0x800946c0+voice,1)=(uint8)level;
  ff_w32(attr+4,227);ff_w16(attr+8,(uint16)((uint32)level<<7));ff_w16(attr+10,(uint16)((uint32)level<<7));
  ff_w16(attr+24,21584);ff_w16(attr+22,(uint16)(((note<<1)&0xffffff00u)+(note&127)));
  ff_w32(attr,1u<<(voice&31));ff_w32(attr+28,ff_u32(0x8009354c)+sample);
  if(!ff_services.spu_key_on_attributes)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);ff_services.spu_key_on_attributes(attr);
 }
 return (sint32)ff_u32(0x80093554);
}

/* 80046884: camera-relative pan and sine-table distance attenuation. */
GDB_CALL sint32 ff_spatial_sound_80046884(uint32 position,uint32 *volume,uint32 *pan) {
 sint32 x=ff_s16(0x8008d4a6),z=ff_s16(0x8008d4ae),px=ff_s16(position+2),pz=ff_s16(position+10),distance,p,v;
 uint32 angle=(uint32)FUN_8001614C(x,z,px,pz)-(uint16)ff_s16(0x8008d490);
 distance=FUN_800163F4(x,z,px,pz)>>5;if(distance>=512)distance=511;
 p=127-(ff_s16(0x800802d0+((angle>>4)&0xffe))>>9);
 if(p<0)p=0;else if((sint16)p>=256)p=255;
 *pan=(uint32)(sint32)(sint16)p;
 v=((uint16)ff_s16(0x800802d0+2*(511-distance))>>7)&255;
 *volume=(uint32)v;return v;
}
GDB_CALL sint32 FUN_80046884(uint32 position,uint32 volume,uint32 pan) {
 return ff_spatial_sound_80046884(position,(uint32*)ff_ptr(volume,4),(uint32*)ff_ptr(pan,4));
}

/* 800568C8: wrapped signed products, cached channel levels and SPU attributes. */
GDB_CALL sint32 FUN_800568C8(sint32 voice,sint32 volume,sint32 pan) {
 uint32 master=*(uint8*)ff_ptr(0x80093d6e,1),attr=FUN_80056640(),index=(uint32)pan<<3;
 sint32 left=(sint32)((uint32)(sint32)ff_s16(0x800802d0+index)*(uint32)volume)>>15;
 sint32 right=(sint32)((uint32)(sint32)ff_s16(0x800806d0+index)*(uint32)volume)>>15;
 left=(sint32)((uint32)left*master)>>8;right=(sint32)((uint32)right*master)>>8;
 *(uint8*)ff_ptr(0x800946c0+(uint32)voice,1)=(uint8)left;
 *(uint8*)ff_ptr(0x80094720+(uint32)voice,1)=(uint8)right;
 ff_w32(attr+4,3);ff_w32(attr,1u<<((uint32)voice&31));
 ff_w16(attr+8,(uint16)((uint32)left<<6));ff_w16(attr+10,(uint16)((uint32)right<<6));
 if(!ff_services.spu_voice_attributes)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);return ff_services.spu_voice_attributes(attr);
}

/* 80043520: orbiting scene object and its two spatial sound channels. */
GDB_CALL sint32 FUN_80043520(uint32 object) {
 uint32 angle=(uint16)ff_s16(object+18),saved[2];sint32 volume,result;
 ff_w32(object+8,ff_u32(object+8)+((uint32)(sint32)ff_s16(0x800802d0+2*(angle>>5))<<8));
 ff_w32(object+12,ff_u32(object+12)-((uint32)(sint32)ff_s16(0x800802d0+2*(((ff_s16(object+18)>>5)+512)&2047))<<8));
 ff_w32(object+4,ff_u32(object+4)+((uint32)(sint32)ff_s16(0x800802d0+2*(angle>>5))<<6));
 ff_w16(object+18,(uint16)(angle-128));ff_w16(object+20,2048);
 saved[0]=ff_u32(0x801fc090);saved[1]=ff_u32(0x801fc094);
 FUN_80046884(object+4,0x801fc090,0x801fc094);
 FUN_800568C8((sint32)ff_u32(0x80092738),(sint32)ff_u32(0x801fc090),(sint32)ff_u32(0x801fc094));
 FUN_80046884(object+4,0x801fc090,0x801fc094);
 volume=(sint32)((ff_u32(0x801fc090)-128)<<1);if(volume<0)volume=0;
 result=FUN_800568C8((sint32)ff_u32(0x80092744),volume,(sint32)ff_u32(0x801fc094));
 ff_w32(0x801fc090,saved[0]);ff_w32(0x801fc094,saved[1]);return result;
}

/* 293D8 selection prefix. Keep RNG calls separate and ordered before56670. */
GDB_CALL void ff_pain_sound_parameters_800293D8(sint32 index,sint32 parameters[3]) {
 sint32 type=ff_s16(0x800b434c+244u*(uint32)index),effect;
 switch(type) {
 case 0:effect=40;break;
 case 1:case 12:case 29:case 30:case 38:effect=64+(ff_rand_8006D348()&1);break;
 case 2:case 49:effect=84;break;
 case 3:effect=99+(ff_rand_8006D348()&1);break;
 case 48:effect=190+(ff_rand_8006D348()&1);break;
 case 32:effect=159;break;
 case 14:effect=208;break;
 default:effect=(sint32)ff_rand_8006D348()%3+9;break;
 }
 parameters[0]=effect;parameters[1]=255-(ff_rand_8006D348()&63);parameters[2]=5952+(ff_rand_8006D348()&255);
}
GDB_CALL sint32 FUN_800293D8(sint32 index) {
 sint32 p[3];ff_pain_sound_parameters_800293D8(index,p);return FUN_80056670(p[0],p[1],p[2]);
}

/* 56840: voice pitch attribute; signed division after wrapped multiply. */
GDB_CALL sint32 FUN_80056840(sint32 voice,sint32 pitch) {
 FF_FUNCTION_MARKER(0x80056840u,"GAME.EXE");
 uint32 attr=FUN_80056640();
 uint32 note=(uint32)((sint32)((uint32)pitch*1536u)/1024)-1536u;
 ff_w32(attr,1u<<((uint32)voice&31));ff_w32(attr+4,32);
 ff_w16(attr+22,(uint16)(((note<<1)&0xffffff00u)+(note&127)));
 if(!ff_services.spu_voice_attributes)ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 return ff_services.spu_voice_attributes(attr);
}
