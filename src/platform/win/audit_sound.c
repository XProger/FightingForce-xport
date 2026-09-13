#include "ff.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static uint8 fixture[0x200000];
static uint32 trace[64];
static uint32 *event(uint32 kind){uint32 n=trace[1]++;if(n>=3)abort();trace[2+20*n]=kind;return trace+2+20*n;}
static sint32 key(sint32 enable,uint32 mask){uint32 *e=event(1);e[1]=enable;e[2]=mask;return 0x1357;}
static sint32 attr(uint32 a){uint32 *e=event(2);e[1]=a;memcpy(e+3,ff_ptr(a,64),64);return 0x2468;}
static sint32 on(uint32 a){uint32 *e=event(3);e[1]=a;memcpy(e+3,ff_ptr(a,64),64);return 0x3579;}
extern void ff_audit_end(void);
int ff_audit_sound(void) {
 FILE *jobs,*out;uint32 j[8];int i;
 if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(fixture,ff_ram,sizeof(fixture));
 jobs=fopen("../status/menu/sound-jobs.bin","rb");out=fopen("../status/menu/sound-native.bin","wb");if(!jobs||!out)return 4;
 ff_services.spu_key=key;ff_services.spu_voice_attributes=attr;ff_services.spu_key_on_attributes=on;
 while(fread(j,sizeof(j),1,jobs)==1) {
  memcpy(ff_ram,fixture,sizeof(fixture));memset(trace,0,sizeof(trace));
  ff_w32(0x80093550,j[4]);ff_w32(0x80093554,j[5]);
  for(i=0;i<24;i++)*(uint8*)ff_ptr(0x80094708+i,1)=(j[6]&(1u<<i))?2:0;
  if(j[7])ff_w32(0x800b6ab8+4*j[1],0xffffffffu);
  if(j[0]==0x80056640)trace[0]=FUN_80056640();
  else if(j[0]==0x80056670)trace[0]=(uint32)FUN_80056670(j[1],j[2],j[3]);
  else if(j[0]==0x800567f8)trace[0]=(uint32)FUN_800567F8(j[1]);
  else if(j[0]==0x80056840)trace[0]=(uint32)FUN_80056840(j[1],j[2]);
  else if(j[0]==0x80056f08)trace[0]=(uint32)FUN_80056F08(j[1],j[2]);
  else if(j[0]==0x800568c8)trace[0]=(uint32)FUN_800568C8(j[1],j[2],j[3]);
  else if(j[0]==0x80043520){ff_w16(j[1]+18,(uint16)j[2]);trace[0]=(uint32)FUN_80043520(j[1]);}
  else return 5;
  fwrite(trace,sizeof(trace),1,out);fwrite(ff_ptr(0x8009354c,12),1,12,out);fwrite(ff_ptr(0x800946c0,120),1,120,out);fwrite(ff_ptr(0x800a79c8,1024),1,1024,out);fwrite(ff_ptr(0x800b8b10,36),1,36,out);fwrite(ff_ptr(0x800941a0,4),1,4,out);
 }
 fclose(jobs);fclose(out);ff_audit_end();return 0;
}
