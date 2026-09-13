#include "ff.h"
#include "wip.h"
#include "platform_dummy.h"
#include "ff_gpu.h"
#include "ff_audio.h"
#include "audio/spu_core.h"
#include "audio_waveout.h"
#include "windows_compat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
GDB_CALL void ff_audit_end(void) { }
static uint32 collision_audit_event[7];
static sint32 collision_audit_callback(uint32 target,uint32 obstacle,uint32 object,const uint32 *position,uint32 actor) {
 collision_audit_event[0]++;collision_audit_event[1]=target;collision_audit_event[2]=obstacle;collision_audit_event[3]=object;
 memcpy(collision_audit_event+4,position,12);return (sint32)0x1234abcd;
}
int ff_audit_input(void);
int ff_audit_menu(void);
int ff_audit_character(void);
int ff_audit_options(void);
int ff_audit_screen(void);
int ff_audit_controller(void);
int ff_audit_scores(void);
int ff_audit_preframe(void);
int ff_audit_sound(void);
int ff_menu_runtime(const char *script,uint32 limit,int headless);
int ff_title_runtime(void);
int ff_sequence_preview(int headless);
int ff_game_prefix_preview(void);
int ff_game_loop_preview(void);
int ff_sequence_game_loop_preview(void);
int ff_menu_startup_runtime(uint32 limit,int headless);
int ff_menu_cold_runtime(uint32 limit,int headless);
int ff_menu_cold_script_runtime(const char *script,int headless);
int ff_menu_startup_script_runtime(const char *script,int headless);
/* Integration preview of audited text routines; not the game menu callback.
 * Resource/bootstrap and frame dispatch remain WIP. */
static int text_preview_frame(void) {
 int i,n=(sint32)ff_u32(0x8007f80c);uint32 arena=0x1d0000,ot=0x1cb000;
 ff_w32(0x8008d4b0,arena);ff_w32(0x8008d4b4,ot);ff_w32(0x8008d4d8,0x801ce000);
 for(i=0;i<2048;i++)ff_w32(ot+4u*i,i?ot+4u*(i-1):0xffffffu);
 ff_w16(0x801ce000,8188);ff_w16(0x801ce008,8184);
 FUN_80061C68(96,16);FUN_80061ADC(ff_u32(0x80094698),214);
 for(i=0;i<n;i++) {
  uint32 t=ff_u32(0x80094600+4u*i);
  FUN_80062C0C(t,160-(FUN_8005EFB8(t)>>1),138-10*n+20*i,i==(sint32)ff_u32(0x80092730)?0x8007f81c:0x80092804);
 }
 return ff_gpu_ot(ot+8188)>=0;
}
/* Isolated traffic-hit audit SDK boundary record; production uses real SPU. */
static uint32 hit_spu_trace[64];
static uint32 *hit_spu_event(uint32 kind){uint32 n=hit_spu_trace[0]++;if(n>=3)abort();hit_spu_trace[1+20*n]=kind;return hit_spu_trace+1+20*n;}
static sint32 hit_spu_key(sint32 on,uint32 mask){uint32 *e=hit_spu_event(1);e[1]=on;e[2]=mask;return 0x1357;}
static sint32 hit_spu_attr(uint32 a){uint32 *e=hit_spu_event(2);e[1]=a;memcpy(e+3,ff_ptr(a,64),64);return 0x2468;}
static sint32 hit_spu_on(uint32 a){uint32 *e=hit_spu_event(3);e[1]=a;memcpy(e+3,ff_ptr(a,64),64);return 0x3579;}
int main(int argc,char **argv) {
 PSX_CONFIG config;uint32 frame=0,limit=0;int headless=0,preview=0;
 if(!ff_wip_options(&argc,argv))return 2;
 if(argc==1)return ff_menu_cold_runtime(0,0);
 if(argc>1&&!strcmp(argv[1],"--audit-player-footstep")) {
  FILE *in,*out;uint32 v[2];
  if(!ff_load_ram("../status/gameplay/original-loop-start.ram"))return 2;
  in=fopen("../status/gameplay/player-footstep-jobs.bin","rb");out=fopen("../status/gameplay/player-footstep-native.bin","wb");if(!in||!out)return 3;
  while(fread(v,sizeof(v),1,in)==1){ff_w32(0x80094e88,v[1]);ff_player_footstep_800262B4((sint32)v[0]);fwrite(ff_ptr(0x80094e88,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-separation")) {
  FILE *in,*out;uint32 v[11],actor,result;
  if(!ff_load_ram("../status/gameplay/original-loop-start.ram"))return 2;
  in=fopen("../status/gameplay/player-separation-jobs.bin","rb");out=fopen("../status/gameplay/player-separation-native.bin","wb");if(!in||!out)return 3;
  while(fread(v,sizeof(v),1,in)==1) {
   actor=0x800b4318+244u*v[0];ff_w32(0x800940b0,v[1]);ff_w32(actor+12,v[2]);ff_w32(actor+36,v[3]);ff_w16(actor+56,(uint16)v[4]);
   ff_w16(0x800b435a,(uint16)v[5]);ff_w16(0x800b444e,(uint16)v[6]);
   ff_w32(0x800b4340,v[7]);ff_w32(0x800b4348,v[8]);ff_w32(0x800b4434,v[9]);ff_w32(0x800b443c,v[10]);
   result=(uint32)FUN_8001A07C((sint32)v[0]);fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,488),488,1,out);
  }
  fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,out);fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-direction")) {
  FILE *in,*out;uint32 v[8],flags,buttons;
  if(!ff_load_ram("../status/gameplay/original-loop-start.ram"))return 2;
  in=fopen("../status/gameplay/player-direction-jobs.bin","rb");out=fopen("../status/gameplay/player-direction-native.bin","wb");if(!in||!out)return 3;
  while(fread(v,sizeof(v),1,in)==1) {
   ff_w32(0x800b431c,v[6]);ff_w32(0x800b4324,v[5]);
   ff_w16(0x800b4350,(uint16)v[1]);ff_w16(0x800b4356,(uint16)v[2]);ff_w16(0x80093d34,(uint16)v[3]);
   buttons=v[0];flags=ff_player_direction_800247A4(0x800b4318,(sint32)v[4],&buttons,v[7]);
   fwrite(&flags,4,1,out);fwrite(&buttons,4,1,out);fwrite(ff_ptr(0x800b4318,244),244,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-game-loop"))return ff_game_loop_preview();
 if(argc>1&&!strcmp(argv[1],"--audit-sequence-game-loop"))return ff_sequence_game_loop_preview();
 if(argc>1&&!strcmp(argv[1],"--audit-game-prefix"))return ff_game_prefix_preview();
 if(argc>1&&!strcmp(argv[1],"--sequence"))return ff_sequence_preview(argc>2&&!strcmp(argv[2],"--headless"));
 if(argc>1&&!strcmp(argv[1],"--audit-score")) {
  FILE *in,*out;uint32 result;char value[16];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/score-jobs.bin","rb");out=fopen("../status/gameplay/score-native.bin","wb");if(!in||!out)return 3;
  while(fread(value,16,1,in)==1) {
   memcpy(ff_ptr(0x800b4318,16),value,16);
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);ff_w32(0x8008d4d8,0x801ce000);ff_w16(0x801ce000,8188);ff_w32(0x801ccffc,0x00ffffff);memset(ff_ptr(0x801d0000,512),0xa5,512);
   result=FUN_80061D0C(0x800b4318,64,16);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x801ccffc,4),4,1,out);fwrite(ff_ptr(0x801d0000,512),512,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-wobble")) {
  FILE *in,*out;uint32 value[3],result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/wobble-jobs.bin","rb");out=fopen("../status/gameplay/wobble-native.bin","wb");if(!in||!out)return 3;
  while(fread(value,12,1,in)==1) {
   memset(ff_ptr(0x800b4318,36),0xa5,36);ff_w32(0x800b4318,value[0]);ff_w16(0x800b432a,(uint16)value[1]);ff_w16(0x800b432e,(uint16)value[2]);result=(uint32)FUN_8002D5D4(0x800b4318);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,36),36,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-damage")) {
  FILE *in,*out;uint32 j[9],result,i,a;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/damage-jobs.bin","rb");out=fopen("../status/gameplay/damage-native.bin","wb");if(!in||!out)return 3;
  while(fread(j,sizeof(j),1,in)==1) {
   memset(ff_ptr(0x800b4318,1952),0xa5,1952);
   for(i=0;i<8;i++){a=0x800b4318+244*i;ff_w32(a+180,0x801c0000);ff_w32(a+184,0);ff_w16(a+66,400);ff_w16(a+74,32766);ff_w8(a+122,(uint8)j[8]);}
   ff_w8(0x801c0005,(uint8)j[3]);ff_w8(0x801c0007,(uint8)j[4]);ff_w8(0x80093d74,(uint8)j[5]);ff_w32(0x80093d68,j[6]);ff_w32(0x80093d5c,j[7]);
   result=FUN_8001F230((sint32)j[0],(sint32)j[1],(sint32)j[2]);fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-pain-sound")) {
  FILE *in,*out;uint32 j[2];sint32 p[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/pain-sound-jobs.bin","rb");out=fopen("../status/gameplay/pain-sound-native.bin","wb");if(!in||!out)return 3;
  while(fread(j,8,1,in)==1){ff_w16(0x800b434c,(uint16)j[0]);ff_w32(0x80094e88,j[1]);ff_pain_sound_parameters_800293D8(0,p);fwrite(p,12,1,out);fwrite(ff_ptr(0x80094e88,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-number")) {
  FILE *in,*out;uint32 value,result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/number-jobs.bin","rb");out=fopen("../status/gameplay/number-native.bin","wb");if(!in||!out)return 3;
  while(fread(&value,4,1,in)==1) {
   memset(ff_ptr(0x800b4318,4),0xa5,4);result=FUN_80061E0C((sint32)value,0x800b4318);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-rightbar")) {
  FILE *in,*out;uint32 job[6],result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/rightbar-jobs.bin","rb");out=fopen("../status/gameplay/rightbar-native.bin","wb");if(!in||!out)return 3;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w16(0x800b434c,(uint16)job[1]);ff_w16(0x800b435a,(uint16)job[2]);ff_w16(0x800b435c,(uint16)job[3]);ff_w16(0x800b435e,(uint16)job[4]);ff_w16(0x800b4360,(uint16)job[5]);
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);ff_w32(0x801ccffc,0x00ffffff);
   ff_w32(0x8008d4d8,0x801ce000);ff_w16(0x801ce000,8188);
   memset(ff_ptr(0x801d0000,32768),0xa5,32768);result=FUN_8006351C(job[0],0x800b4318);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x801ccffc,4),4,1,out);fwrite(ff_ptr(0x801d0000,32768),32768,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-healthbar")) {
  FILE *in,*out;uint32 job[6],result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/healthbar-jobs.bin","rb");out=fopen("../status/gameplay/healthbar-native.bin","wb");if(!in||!out)return 3;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w16(0x800b434c,(uint16)job[1]);ff_w16(0x800b435a,(uint16)job[2]);ff_w16(0x800b435c,(uint16)job[3]);ff_w16(0x800b435e,(uint16)job[4]);ff_w16(0x800b4360,(uint16)job[5]);
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);ff_w32(0x801ccffc,0x00ffffff);
   memset(ff_ptr(0x801d0000,128),0xa5,128);result=FUN_80063100(job[0],0x800b4318);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x801ccffc,4),4,1,out);fwrite(ff_ptr(0x801d0000,128),128,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-character-visible")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/character-visible-before.ram"))return 2;
  f=fopen("../status/gameplay/character-visible-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=ff_characters_800593FC();
  f=fopen("../status/gameplay/character-visible-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("character_visible_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-prepare-scene")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/prepare-scene-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=ff_prepare_scene_800587A8_stage0();
  f=fopen("../status/gameplay/prepare-scene-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("prepare_scene_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-stage-tick")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/stage-tick-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=ff_game_stage_8001E168_stage0();
  f=fopen("../status/gameplay/stage-tick-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("stage_tick_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-drop-items")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/character-visible-before.ram"))return 2;
  f=fopen("../status/gameplay/drop-items-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  ff_w8(0x800b4396,0);ff_w8(0x800b4395,1);result=FUN_80017E30(0);
  f=fopen("../status/gameplay/drop-items-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("drop_items_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-traffic-hit")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/traffic-hit-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  {uint32 i;for(i=1;i<8;i++)ff_w16(0x800b434c+244*i,65535);}
  ff_w32(0x800b4340,ff_u32(0x800b9024));ff_w32(0x800b4348,ff_u32(0x800b902c));ff_w16(0x800b4368,0);
  ff_services.spu_key=hit_spu_key;ff_services.spu_voice_attributes=hit_spu_attr;ff_services.spu_key_on_attributes=hit_spu_on;
  result=FUN_80029AFC(0x8009ef40,0x800b9020);
  f=fopen("../status/gameplay/traffic-hit-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  f=fopen("../status/gameplay/traffic-hit-native-spu.bin","wb");if(!f)return 7;fwrite(hit_spu_trace,256,1,f);fclose(f);
  printf("traffic_hit_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-held-animation")) {
  FILE *in,*out;uint32 j[4],result,table,slot71,flag;uint8 actor[244],globals[1280];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  memcpy(actor,ff_ptr(0x800b4318,244),244);memcpy(globals,ff_ptr(0x80093d00,1280),1280);flag=ff_u32(0x8008e4ec);
  table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(0x800b434c));slot71=ff_u32(table+284);
  in=fopen("../status/gameplay/held-animation-jobs.bin","rb");out=fopen("../status/gameplay/held-animation-native.bin","wb");if(!in||!out)return 3;
  while(fread(j,16,1,in)==1) {
   memcpy(ff_ptr(0x800b4318,244),actor,244);memcpy(ff_ptr(0x80093d00,1280),globals,1280);ff_w32(0x8008e4ec,flag);
   ff_w8(0x800b4395,j[0]==0xffffffffu?255:0);ff_w8(0x800b4396,j[1]==0xffffffffu?255:1);
   ff_w32(0x8009a3c8,j[0]);ff_w32(0x8009a3ec,j[1]);ff_w32(table+284,j[3]?0:slot71);
   result=(uint32)FUN_800237CC(0x800b4318,table,(sint32)j[2]);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,244),244,1,out);fwrite(ff_ptr(0x80093d00,1280),1280,1,out);fwrite(ff_ptr(0x8008e4ec,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-action-input")) {
  FILE *in,*out;uint32 j[7],result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/action-input-jobs.bin","rb");out=fopen("../status/gameplay/action-input-native.bin","wb");if(!in||!out)return 3;
  while(fread(j,28,1,in)==1) {
   if(fread(ff_ptr(0x80093d78,18),18,1,in)!=1||fread(ff_ptr(0x80094480,2),2,1,in)!=1||fread(ff_ptr(0x800940f8,32),32,1,in)!=1)return 4;
   ff_w32(0x80094288,j[3]);ff_w32(0x8009428c,j[4]);ff_w32(0x80093a10,j[5]);ff_w32(0x80093a14,j[6]);
   result=j[0]==0?FUN_8005F558(j[1]):j[0]==1?FUN_8005F8F0(j[1],j[2]):FUN_8005FA58(j[1],j[2]);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x800940f8,32),32,1,out);fwrite(ff_ptr(0x80093d78,18),18,1,out);fwrite(ff_ptr(0x80094480,2),2,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-target-selection")) {
  FILE *in,*out;uint32 j[3],result;uint8 globals[2128];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  memcpy(globals,ff_ptr(0x80094000,2128),2128);
  in=fopen("../status/gameplay/target-selection-jobs.bin","rb");out=fopen("../status/gameplay/target-selection-native.bin","wb");if(!in||!out)return 3;
  while(fread(j,12,1,in)==1) {
   if(fread(ff_ptr(0x800b4318,1952),1952,1,in)!=1)return 4;
   memcpy(ff_ptr(0x80094000,2128),globals,2128);ff_w32(0x80093d5c,j[1]);ff_w32(0x8009a3c8,j[2]);result=FUN_80049864((sint32)j[0]);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);fwrite(ff_ptr(0x80094000,2128),2128,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-ai-roles")) {
  FILE *in,*out;uint32 result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/ai-roles-jobs.bin","rb");out=fopen("../status/gameplay/ai-roles-native.bin","wb");if(!in||!out)return 3;
  while(fread(ff_ptr(0x800b4318,1952),1952,1,in)==1) {
   if(fread(ff_ptr(0x80093f40,32),32,1,in)!=1)return 4;
   result=FUN_8004A4CC();fwrite(&result,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);fwrite(ff_ptr(0x80093f40,32),32,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-item-selection")) {
  FILE *in,*out;uint32 j[5],result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/item-selection-jobs.bin","rb");out=fopen("../status/gameplay/item-selection-native.bin","wb");if(!in||!out)return 3;
  while(fread(j,20,1,in)==1) {
   if(fread(ff_ptr(0x800b4318,488),488,1,in)!=1||fread(ff_ptr(0x8009a3c8,2304),2304,1,in)!=1)return 4;
   ff_w32(0x800940b0,j[2]);ff_w32(0x80094550,j[3]);ff_w32(0x80094554,j[4]);result=FUN_80017FE0((sint32)j[0],(sint32)j[1]);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x80094550,8),8,1,out);fwrite(ff_ptr(0x800b4318,488),488,1,out);fwrite(ff_ptr(0x8009a3c8,2304),2304,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-script-prefix")) {
  FILE *in,*out;uint32 result,i;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/ai-first-original.bin","rb");if(!in)return 3;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,in)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,in)!=1)return 4;fclose(in);
  out=fopen("../status/gameplay/script-prefix-native.bin","wb");if(!out)return 5;
  for(i=0;i<2;i++) {
   result=(uint32)FUN_8004B228();fwrite(&result,4,1,out);
   fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,out);fwrite(ff_ptr(0x1f800000,1024),1024,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-script-create-chain")) {
  FILE *in,*out;uint32 result,i;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  in=fopen("../status/gameplay/ai-first-original.bin","rb");if(!in)return 3;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,in)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,in)!=1)return 4;fclose(in);
  out=fopen("../status/gameplay/script-create-chain-native.bin","wb");if(!out)return 5;
  for(i=0;i<3;i++) {
   result=(uint32)FUN_8004B228();fwrite(&result,4,1,out);
   fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,out);fwrite(ff_ptr(0x1f800000,1024),1024,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-create")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-create-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-spawn-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_8004AF18(7,0x01800000,0xf7800000,0,0,10,0);
  f=fopen("../status/gameplay/npc-create-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_create_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-entrance")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-entrance-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80053034(2);
  f=fopen("../status/gameplay/npc-entrance-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_entrance_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-expiry-call")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-expiry-call-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w16(0x800b4500+80,1);result=FUN_80053034(2);
  f=fopen("../status/gameplay/npc-expiry-call-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_expiry_call_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-handler-first")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-handler-first-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w16(0x800b4500+80,1);result=FUN_80052944(2);
  f=fopen("../status/gameplay/npc-handler-first-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_handler_first_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-handler-live")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-handler-live-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w16(0x800b4500+80,0);result=FUN_80052944(2);
  f=fopen("../status/gameplay/npc-handler-live-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_handler_live_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-ai-outer")) {
  FILE *f,*out;uint32 i,result;const char *inputs[]={"player-first-original.bin","script-create-original.bin","script-create-original.bin","script-create-original.bin"};char path[256];
  out=fopen("../status/gameplay/ai-outer-native.bin","wb");if(!out)return 2;
  for(i=0;i<4;i++){
   if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 3;
   sprintf(path,"../status/gameplay/%s",inputs[i]);f=fopen(path,"rb");if(!f)return 4;
   if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 5;fclose(f);
   if(i>=2)ff_w16(0x800b4500+80,i==2?1:0);
   result=FUN_8004A704();fwrite(&result,4,1,out);fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,out);fwrite(ff_ptr(0x1f800000,1024),1024,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-target")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-target-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_800531C4(2);
  f=fopen("../status/gameplay/npc-target-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_target_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sprint-effect871-lifetime")) {
  FILE *f;uint32 result=0,i; sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/sprint-effect871-lifetime-input.ram"))return 2;
  f=fopen("../status/gameplay/sprint-effect871-lifetime-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  for(i=0;i<27;i++)result=ff_object_call(0x8002ae64,0x8009d1f4);
  f=fopen("../status/gameplay/sprint-effect871-lifetime-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("sprint_effect871_lifetime_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sprint-effect871-render")) {
  FILE *f;uint32 result=0; sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/sprint-effect871-render-input.ram"))return 2;
  f=fopen("../status/gameplay/sprint-effect871-render-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_8005C808(0x8009d1f4);
  f=fopen("../status/gameplay/sprint-effect871-render-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("sprint_effect871_render_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-impact-sprite")) {
  FILE *f;uint32 result=0,i; sint32 setup[3],position[4]={160,128,400,256};
  if(!ff_load_ram("../status/gameplay/hit185-input.ram"))return 2;
  f=fopen("../status/gameplay/impact-sprite-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  for(i=0;i<7;i++)FUN_80012594(position,ff_s16(0x8009acd6+2*i));
  ff_w32(0x8009a458,865);ff_w32(0x8009a45c,ff_u32(0x800b4340));ff_w32(0x8009a460,0);ff_w32(0x8009a464,ff_u32(0x800b4348));
  ff_w32(0x8009a468,0);ff_w32(0x8009a46c,0);
  for(i=0;i<7;i++){ff_w16(0x8009a46e,(uint16)i);result=FUN_8005C808(0x8009a458);}
  f=fopen("../status/gameplay/impact-sprite-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("impact_sprite_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-particle-lifetime")) {
  FILE *f;uint32 result=0,i,j;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/hit185-input.ram"))return 2;
  f=fopen("../status/gameplay/particle-lifetime-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  for(i=0;i<4;i++)FUN_800310B8(ff_u32(0x800b4340),0,ff_u32(0x800b4348),(uint32)(sint32)ff_s16(0x800b4350)^1024u);
  for(i=0;i<4;i++)FUN_8003100C(ff_u32(0x800b4340),0,ff_u32(0x800b4348),(uint32)(sint32)ff_s16(0x800b4350)^1024u);
  for(j=0;j<72;j++)for(i=0;i<8;i++)FUN_8002BEC0(0x8009d1f4+36*i);
  for(i=0;i<8;i++)FUN_8002B28C(0x8009a458);
  FUN_800121A8(123,77,400,0x7fff);FUN_80012240(0,255,1024,0x3e0);
  result=FUN_800122E0(319,0,40,0x7c1f);
  f=fopen("../status/gameplay/particle-lifetime-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("particle_lifetime_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-hit-particles")) {
  FILE *f;uint32 result,i;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/hit185-input.ram"))return 2;
  f=fopen("../status/gameplay/hit-particles-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  for(i=0;i<4;i++)FUN_800310B8(ff_u32(0x800b4340),0,ff_u32(0x800b4348),(uint32)(sint32)ff_s16(0x800b4350)^1024u);
  for(i=0;i<4;i++)result=FUN_8003100C(ff_u32(0x800b4340),0,ff_u32(0x800b4348),(uint32)(sint32)ff_s16(0x800b4350)^1024u);
  f=fopen("../status/gameplay/hit-particles-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("hit_particles_result %08X\n",result);ff_audit_end();return 0;
 }

 if(argc>1&&!strcmp(argv[1],"--audit-stationary-items")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/stationary-items-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_8001ED1C();
  f=fopen("../status/gameplay/stationary-items-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("stationary_items_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-camera-target")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/camera-target-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80047600();
  f=fopen("../status/gameplay/camera-target-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("camera_target_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-hit-effects-prefix")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/hit185-input.ram"))return 2;
  f=fopen("../status/gameplay/hit-effects-prefix-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  FUN_8001697C(865,ff_u32(0x800b4340),0,ff_u32(0x800b4348));result=FUN_80017C78(0);
  f=fopen("../status/gameplay/hit-effects-prefix-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("hit_effects_prefix_result %08X\n",result);ff_audit_end();return 0;
 }



 if(argc>1&&!strcmp(argv[1],"--audit-camera-chain")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/camera-target-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/camera-target-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80027DF8();
  f=fopen("../status/gameplay/camera-chain-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("camera_chain_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-move-call")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-move-call-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80047CE4(2,0xf8680000,0xf1f00000,5);
  f=fopen("../status/gameplay/npc-move-call-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_move_call_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-navigation")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/npc-navigation-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  f=fopen("../status/gameplay/script-create-original.bin","rb");if(!f)return 8;
  if(fread(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1)return 9;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80019718(0x013e3200,0xf7123700);printf("navigation_query %08X\n",result);
  result=FUN_80019718(0x01c1ce00,0xf7123700);printf("navigation_query %08X\n",result);
  result=FUN_800483BC(2,0xf8680000,0xf1f00000);
  f=fopen("../status/gameplay/npc-navigation-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_navigation_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-walk-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-walk-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-walk-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-walk-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_walk_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-state152-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-state152-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-state152-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-state152-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_state152_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-state152")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-state152-input.ram"))return 2;
  f=fopen("../status/gameplay/player-state152-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-state152-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_state152_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-cross")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-cross-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-cross-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80054C90(2);
  f=fopen("../status/gameplay/npc-cross-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_cross_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-hit")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-hit-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-hit-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-hit-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_hit_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-pause-flow")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/pause-flow-input.ram"))return 2;
  f=fopen("../status/gameplay/pause-flow-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_w8(0x80093dd8,(uint8)setup[0]);
  result=FUN_8005DBBC();
  f=fopen("../status/gameplay/pause-flow-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("pause_flow_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-pause-audio")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/pause-audio-input.ram"))return 2;
  f=fopen("../status/gameplay/pause-audio-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  FUN_8005703C();result=FUN_800570F0();
  f=fopen("../status/gameplay/pause-audio-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("pause_audio_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-event")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-event-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-event-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-event-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_event_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-circle")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-circle-input.ram"))return 2;
  f=fopen("../status/gameplay/player-circle-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-circle-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_circle_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-backward")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-backward-input.ram"))return 2;
  f=fopen("../status/gameplay/player-backward-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-backward-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_backward_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-backward-held")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-backward-held-input.ram"))return 2;
  f=fopen("../status/gameplay/player-backward-held-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-backward-held-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_backward_held_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-backward-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-backward-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-backward-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-backward-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_backward_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle-chain-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-chain-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-chain-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-chain-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_chain_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-batch19-call")) {
  FILE *f;uint32 result=0,address,a0,a1;sint32 setup[3];
  if(argc!=8)return 1;
  if(!ff_load_ram(argv[2]))return 2;
  f=fopen(argv[3],"rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1){fclose(f);return 4;}fclose(f);
  address=(uint32)strtoul(argv[5],0,0);a0=(uint32)strtoul(argv[6],0,0);a1=(uint32)strtoul(argv[7],0,0);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  switch(address) {
  case 0x80011fec:FUN_80011FEC();break;
  case 0x80024008:result=FUN_80024008();break;
  case 0x8002ae10:result=FUN_8002AE10(a0);break;
  case 0x8002bdd0:result=FUN_8002BDD0(a0);break;
  case 0x8002d7c8:result=FUN_8002D7C8(a0);break;
  case 0x8002dba0:result=FUN_8002DBA0(a0);break;
  case 0x8002ddb8:result=FUN_8002DDB8(a0);break;
  case 0x80033d7c:result=FUN_80033D7C((sint32)a0);break;
  case 0x80049b78:result=(uint32)FUN_80049B78((sint32)a0);break;
  case 0x80049db0:result=(uint32)FUN_80049DB0((sint32)a0,(sint32)a1);break;
  case 0x80049e6c:result=FUN_80049E6C((sint32)a0);break;
  case 0x800544e4:result=(uint32)FUN_800544E4((sint32)a0);break;
  case 0x80056840:result=(uint32)FUN_80056840((sint32)a0,(sint32)a1);break;
  case 0x800593fc:result=(uint32)ff_characters_800593FC();break;
  case 0x8005b178:result=FUN_8005B178(a0);break;
  case 0x8005c808:result=FUN_8005C808(a0);break;
  case 0x8005d67c:result=FUN_8005D67C(a0);break;
  case 0x8005fb44:result=(uint32)FUN_8005FB44();break;
  case 0x80061b28:result=(uint32)FUN_80061B28(a0,(sint32)a1);break;
  default:return 7;
  }
  f=fopen(argv[4],"wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1){fclose(f);return 6;}fclose(f);
  printf("batch19_call_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&(!strcmp(argv[1],"--audit-player-fixture")||!strcmp(argv[1],"--audit-npc-fixture")||!strcmp(argv[1],"--audit-camera-fixture")||!strcmp(argv[1],"--audit-characters-fixture")||!strcmp(argv[1],"--audit-objects-fixture")||!strcmp(argv[1],"--audit-script-fixture")||!strcmp(argv[1],"--audit-transition-fixture"))) {
  FILE *f;uint32 result;sint32 setup[3];
  if(argc!=(!strcmp(argv[1],"--audit-npc-fixture")?6:5))return 1;
  if(!ff_load_ram(argv[2]))return 2;
  f=fopen(argv[3],"rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1){fclose(f);return 4;}fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=!strcmp(argv[1],"--audit-transition-fixture")?FUN_80027DF8():!strcmp(argv[1],"--audit-script-fixture")?FUN_8004B228():!strcmp(argv[1],"--audit-objects-fixture")?FUN_8001ED1C():!strcmp(argv[1],"--audit-npc-fixture")?FUN_80052944(atoi(argv[5])):!strcmp(argv[1],"--audit-camera-fixture")?FUN_800476E8():!strcmp(argv[1],"--audit-characters-fixture")?ff_characters_800593FC():FUN_80024008();
  f=fopen(argv[4],"wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1){fclose(f);return 6;}fclose(f);
  printf("player_fixture_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle-chain-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-chain-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-chain-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-chain-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_chain_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square-chain-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-chain-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-chain-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-chain-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_chain_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-chain-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-chain-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-chain-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-chain-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_chain_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-third-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-third-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-third-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-third-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_third_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-fourth")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-fourth-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-fourth-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-fourth-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_fourth_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-cross-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-cross-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-cross-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-cross-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_cross_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-cross-held")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-cross-held-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-cross-held-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-cross-held-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_cross_held_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-cross-effect")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-cross-effect-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-cross-effect-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-cross-effect-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_cross_effect_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-held")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-held-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-held-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-held-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_held_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-square-held")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-square-held-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-square-held-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-square-held-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_square_held_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-square-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-square-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-square-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-square-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_square_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-engage-cross-hurt")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-engage-cross-hurt-input.ram"))return 2;
  f=fopen("../status/gameplay/player-engage-cross-hurt-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-engage-cross-hurt-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_engage_cross_hurt_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-ground-attack-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-ground-attack-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-ground-attack-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-ground-attack-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_ground_attack_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-ground-attack-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-ground-attack-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-ground-attack-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-ground-attack-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_ground_attack_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-ground-attack-lethal")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-ground-attack-lethal-input.ram"))return 2;
  f=fopen("../status/gameplay/player-ground-attack-lethal-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-ground-attack-lethal-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_ground_attack_lethal_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-mace-hit-reaction")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/mace-hit-reaction-input.ram"))return 2;
  f=fopen("../status/gameplay/mace-hit-reaction-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/mace-hit-reaction-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("mace_hit_reaction_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-engage-contact")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-engage-contact-input.ram"))return 2;
  f=fopen("../status/gameplay/player-engage-contact-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-engage-contact-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_engage_contact_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-square-entry")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-square-entry-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-square-entry-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-square-entry-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_square_entry_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-cross-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-cross-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-cross-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-cross-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_cross_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-cycle")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-cycle-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-cycle-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-cycle-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_cycle_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-sprint-release")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-sprint-release-input.ram"))return 2;
  f=fopen("../status/gameplay/player-sprint-release-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-sprint-release-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_sprint_release_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-death-impact")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-death-impact-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-death-impact-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(3);
  f=fopen("../status/gameplay/npc-death-impact-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_death_impact_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-death-remove")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-death-remove-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-death-remove-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(3);
  f=fopen("../status/gameplay/npc-death-remove-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_death_remove_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-ground-lethal-update")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-ground-lethal-update-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-ground-lethal-update-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(3);
  f=fopen("../status/gameplay/npc-ground-lethal-update-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_ground_lethal_update_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-engage-stagger")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-engage-stagger-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-engage-stagger-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(3);
  f=fopen("../status/gameplay/npc-engage-stagger-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_engage_stagger_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-engage-defense")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-engage-defense-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-engage-defense-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(2);
  f=fopen("../status/gameplay/npc-engage-defense-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_engage_defense_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-sprint-recover")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-sprint-recover-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-sprint-recover-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(3);
  f=fopen("../status/gameplay/npc-sprint-recover-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_sprint_recover_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-sprint-caller")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-sprint-caller-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-sprint-caller-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80052944(3);
  f=fopen("../status/gameplay/npc-sprint-caller-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_sprint_caller_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-sprint-lethal")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-sprint-lethal-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-sprint-lethal-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80048DBC(3,3);
  f=fopen("../status/gameplay/npc-sprint-lethal-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_sprint_lethal_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-cross-third-reaction")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-cross-third-reaction-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-cross-third-reaction-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80054C90(2);
  f=fopen("../status/gameplay/npc-cross-third-reaction-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_cross_third_reaction_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-npc-cross-third-reaction-next")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/npc-cross-third-reaction-next-input.ram"))return 2;
  f=fopen("../status/gameplay/npc-cross-third-reaction-next-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80054C90(2);
  f=fopen("../status/gameplay/npc-cross-third-reaction-next-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("npc_cross_third_reaction_next_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-third-trail")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-third-trail-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-third-trail-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-third-trail-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_third_trail_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-third-trail-shift")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-third-trail-shift-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-third-trail-shift-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-third-trail-shift-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_third_trail_shift_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-third")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-third-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-third-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-third-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_third_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-third-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-third-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-third-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-third-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_third_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-chain")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-chain-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-chain-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-chain-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_chain_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross-chain-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-chain-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-chain-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-chain-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_chain_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square-chain")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-chain-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-chain-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-chain-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_chain_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square-chain-hit")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-chain-hit-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-chain-hit-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-chain-hit-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_chain_hit_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square-chain-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-chain-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-chain-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-chain-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_chain_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle-chain-hit")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-chain-hit-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-chain-hit-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-chain-hit-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_chain_hit_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle-chain")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-chain-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-chain-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-chain-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_chain_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-triangle-hit")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-triangle-hit-input.ram"))return 2;
  f=fopen("../status/gameplay/player-triangle-hit-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-triangle-hit-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_triangle_hit_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-circle-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-circle-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-circle-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-circle-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_circle_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-circle-effect-render")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/circle-effect-render-input.ram"))return 2;
  f=fopen("../status/gameplay/circle-effect-render-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=ff_object_call(0x8005d488,0x8009d1f4);
  f=fopen("../status/gameplay/circle-effect-render-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("circle_effect_render_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-circle-land")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-circle-land-input.ram"))return 2;
  f=fopen("../status/gameplay/player-circle-land-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-circle-land-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_circle_land_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-circle-motion")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-circle-motion-input.ram"))return 2;
  f=fopen("../status/gameplay/player-circle-motion-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-circle-motion-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_circle_motion_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-circle-motion-right")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-circle-motion-right-input.ram"))return 2;
  f=fopen("../status/gameplay/player-circle-motion-right-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-circle-motion-right-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_circle_motion_right_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-circle-event")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-circle-event-input.ram"))return 2;
  f=fopen("../status/gameplay/player-circle-event-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-circle-event-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_circle_event_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square-end")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-end-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-end-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-end-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_end_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-square-hit")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-square-hit-input.ram"))return 2;
  f=fopen("../status/gameplay/player-square-hit-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  ff_audio_init_empty();InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-square-hit-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_square_hit_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-cross")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-cross-input.ram"))return 2;
  f=fopen("../status/gameplay/player-cross-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-cross-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_cross_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-walk-first")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-walk-first-input.ram"))return 2;
  f=fopen("../status/gameplay/player-walk-first-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-walk-first-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_walk_first_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-hit186")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/player-hit186-input.ram"))return 2;
  f=fopen("../status/gameplay/player-hit186-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-hit186-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_hit186_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-first")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/player-first-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  result=FUN_80024008();
  f=fopen("../status/gameplay/player-first-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("player_first_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-ground-items")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/ground-items-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=FUN_8001EE44();
  f=fopen("../status/gameplay/ground-items-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("ground_items_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-stage0-update")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/stage0-update-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=ff_sequence_update_8001D640();
  f=fopen("../status/gameplay/stage0-update-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("stage0_update_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-traffic-update")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/traffic-update-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=FUN_8002F59C(0x800b9020);
  f=fopen("../status/gameplay/traffic-update-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("traffic_update_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-traffic-collision")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/traffic-collision-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=FUN_80029AFC(0x8009ef40,0x800b9020);
  f=fopen("../status/gameplay/traffic-collision-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("traffic_collision_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-hud")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/hud-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=ff_hud_800620BC_stage0();
  f=fopen("../status/gameplay/hud-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("hud_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-character-render")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/character-render-before.ram"))return 2;
  f=fopen("../status/gameplay/character-render-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=ff_characters_800593FC();
  f=fopen("../status/gameplay/character-render-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("character_render_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sequence-render")) {
  FILE *f;uint32 result;sint32 setup[3];
  if(!ff_load_ram("../status/gameplay/sequence-update-first.ram"))return 2;
  f=fopen("../status/gameplay/sequence-render-setup.bin","rb");if(!f)return 3;
  if(fread(setup,12,1,f)!=1||fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
  ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);
  memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);
  result=FUN_8005C17C();
  f=fopen("../status/gameplay/sequence-render-native.bin","wb");if(!f)return 5;
  if(fwrite(ff_ptr(0x80080000,0x170000),0x170000,1,f)!=1||fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 6;fclose(f);
  printf("sequence_render_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sequence-update")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/sequence-update-first.ram"))return 2;
  f=fopen("../status/gameplay/sequence-update-first.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 4;fclose(f);
  result=ff_sequence_update_8001D640();
  f=fopen("../status/gameplay/sequence-update-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,sizeof(ff_ram),1,f)!=1)return 6;fclose(f);
  f=fopen("../status/gameplay/sequence-update-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1024,1,f)!=1)return 8;fclose(f);
  printf("sequence_update_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-regions")) {
  FILE *in,*out;uint32 job[4],result;uint8 actors[1952];
  if(!ff_load_ram("../status/gameplay/sequence-update-first.ram"))return 2;
  memcpy(actors,ff_ptr(0x800b4318,1952),1952);
  in=fopen("../status/gameplay/region-jobs.bin","rb");out=fopen("../status/gameplay/region-native.bin","wb");if(!in||!out)return 3;
  while(fread(job,sizeof(job),1,in)==1) {
   uint32 actor=0x800b4318+244u*job[3];memcpy(ff_ptr(0x800b4318,1952),actors,1952);
   ff_w16(actor+42,(uint16)job[0]);ff_w16(actor+50,(uint16)job[1]);ff_w16(actor+56,(uint16)job[2]);
   result=FUN_80019CA4((sint32)job[3]);
   if(fwrite(&result,4,1,out)!=1||fwrite(ff_ptr(actor,244),244,1,out)!=1||fwrite(ff_ptr(0x80094290,8),8,1,out)!=1)return 4;
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sky")) {
  FILE *in,*out;uint32 job[5],row[3];
  if(!ff_load_ram("../status/gameplay/sequence-init-after.ram"))return 2;
  in=fopen("../status/gameplay/sky-jobs.bin","rb");out=fopen("../status/gameplay/sky-native.bin","wb");if(!in||!out)return 3;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w32(0x80093d58,job[0]);ff_w32(0x8008d4d0,job[1]);ff_w16(0x8008d490,(uint16)job[2]);
   ff_w32(0x8008d4c8,job[3]);*(uint8*)ff_ptr(0x800872ec,1)=(uint8)job[4];
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);ff_w32(0x801cb000,0xffffff);
   memset(ff_ptr(0x801d0000,128),0xa5,128);row[0]=FUN_8006494C();row[1]=ff_u32(0x8008d4b0);row[2]=ff_u32(0x801cb000);
   if(fwrite(row,sizeof(row),1,out)!=1||fwrite(ff_ptr(0x801d0000,128),128,1,out)!=1)return 4;
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sequence-frames")) {
  FILE *f,*out;uint32 i,result;static const sint32 edge_frames[]={549,550,551,0x7fffffff};
  if(!ff_load_ram("../status/gameplay/sequence-init-after.ram"))return 2;
  f=fopen("../status/gameplay/sequence-init-after.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  ff_w32(0x80093dd0,0);ff_w32(0x80093d58,27);
  out=fopen("../status/gameplay/sequence-frames-native.bin","wb");if(!out)return 5;
  for(i=0;i<554;i++) {
   result=i<550?ff_sequence_frame_8001559C_stage27():(uint32)FUN_8004799C(edge_frames[i-550]);
   if(fwrite(&result,4,1,out)!=1||fwrite(ff_ptr(0x8008d490,32),32,1,out)!=1||
      fwrite(ff_ptr(0x80093dd0,4),4,1,out)!=1||fwrite(ff_ptr(0x8009403c,4),4,1,out)!=1||
      fwrite(ff_ptr(0x800b92f0,24),24,1,out)!=1){fclose(out);return 6;}
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-stage-scripts")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/sequence-level-entry.ram"))return 2;
  f=fopen("../status/gameplay/sequence-level-entry.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_8004C1CC();
  f=fopen("../status/gameplay/stage-scripts-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/stage-scripts-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("stage_scripts_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-initial-placement")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/player-resources-after.ram"))return 2;
  f=fopen("../status/gameplay/player-resources-after.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_80014808();
  f=fopen("../status/gameplay/initial-placement-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/initial-placement-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("initial_placement_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-level-ready")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/sequence-level-entry.ram"))return 2;
  f=fopen("../status/gameplay/sequence-level-entry.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  ff_audio_init_empty();InitGeom();
  result=ff_level_init_80014A34_stage0();ff_level_ready_80014B70_stage0();
  f=fopen("../status/gameplay/level-ready-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/level-ready-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("level_ready_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-level-prefix")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/sequence-level-entry.ram"))return 2;
  f=fopen("../status/gameplay/sequence-level-entry.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  ff_audio_init_empty();InitGeom();
  result=ff_level_init_80014A34_stage0();
  f=fopen("../status/gameplay/level-prefix-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/level-prefix-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("level_prefix_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-stage0-mpd")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/level-models-before.ram"))return 2;
  f=fopen("../status/gameplay/level-models-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  {uint32 i;for(i=0;i<64;i++)ff_w32(0x8009a3c8+36u*i,0xffffffffu);}
  f=fopen("assets/STAGE11.WAD","rb");if(!f)return 9;fseek(f,66516,SEEK_SET);
  if(fread(ff_ptr(0x801d0000,8750),8750,1,f)!=1)return 10;fclose(f);
  result=ff_menu_objects_800173B0(0x801d0000);
  f=fopen("../status/gameplay/stage0-mpd-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/stage0-mpd-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("stage0_mpd_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-game-input")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/first-loop-entry.ram"))return 2;
  f=fopen("../status/gameplay/first-loop-entry.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=ff_game_input_8005F23C();
  f=fopen("../status/gameplay/game-input-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/game-input-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("game_input_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-object-regions")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/player-resources-after.ram"))return 2;
  f=fopen("../status/gameplay/player-resources-after.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_80017BB0();
  f=fopen("../status/gameplay/object-regions-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/object-regions-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("object_regions_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-player-resources")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/player-resources-before.ram"))return 2;
  f=fopen("../status/gameplay/player-resources-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_8001440C();
  f=fopen("../status/gameplay/player-resources-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/player-resources-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("player_resources_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-mace-player-resources")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/mace-player-resources-before.ram"))return 2;
  f=fopen("../status/gameplay/mace-player-resources-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_8001440C();
  f=fopen("../status/gameplay/mace-player-resources-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/mace-player-resources-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("player_resources_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-smasher-player-resources")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/smasher-player-resources-before.ram"))return 2;
  f=fopen("../status/gameplay/smasher-player-resources-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_8001440C();
  f=fopen("../status/gameplay/smasher-player-resources-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/smasher-player-resources-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("player_resources_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-enemy-resources")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/enemy-resources-before.ram"))return 2;
  f=fopen("../status/gameplay/enemy-resources-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_80034530();
  f=fopen("../status/gameplay/enemy-resources-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/enemy-resources-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("enemy_resources_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-level-models")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/level-models-before.ram"))return 2;
  f=fopen("../status/gameplay/level-models-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_80016C18();
  f=fopen("../status/gameplay/level-models-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/level-models-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("level_models_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sequence-init")) {
  FILE *f;uint32 result;
  if(!ff_load_ram("../status/gameplay/sequence-init-before.ram"))return 2;
  f=fopen("../status/gameplay/sequence-init-before.scratch","rb");if(!f)return 3;
  if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
  result=FUN_80039578();
  f=fopen("../status/gameplay/sequence-init-native.ram","wb");if(!f)return 5;
  if(fwrite(ff_ram,1,sizeof(ff_ram),f)!=sizeof(ff_ram)){fclose(f);return 6;}fclose(f);
  f=fopen("../status/gameplay/sequence-init-native.scratch","wb");if(!f)return 7;
  if(fwrite(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 8;}fclose(f);
  printf("sequence_init_result %08X\n",result);ff_audit_end();return 0;
 }
 if(argc>2&&!strcmp(argv[1],"--menu-startup-script"))return ff_menu_startup_script_runtime(argv[2],argc>3&&!strcmp(argv[3],"--headless"));
 if(argc>2&&!strcmp(argv[1],"--menu-cold-script"))return ff_menu_cold_script_runtime(argv[2],argc>3&&!strcmp(argv[3],"--headless"));
 if(argc>1&&!strcmp(argv[1],"--menu-cold"))return ff_menu_cold_runtime(argc>2?(uint32)strtoul(argv[2],0,0):0,argc>3&&!strcmp(argv[3],"--headless"));
 if(argc>1&&!strcmp(argv[1],"--menu-startup"))return ff_menu_startup_runtime(argc>2?(uint32)strtoul(argv[2],0,0):0,argc>3&&!strcmp(argv[3],"--headless"));
 if(argc>1&&!strcmp(argv[1],"--title"))return ff_title_runtime();
 if(argc>1&&strcmp(argv[1],"--audit-model-packet")==0) {
  FILE *in,*out;uint32 h[5],result;uint8 record[12],xy[16],depth[8],uv[12];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/model-packet-jobs.bin","rb");out=fopen("../status/menu/model-packet-native.bin","wb");if(!in||!out)return 4;
  while(fread(h,20,1,in)==1){
   if(fread(record,12,1,in)!=1||fread(xy,16,1,in)!=1||fread(depth,8,1,in)!=1||fread(uv,12,1,in)!=1)return 5;
   memcpy(ff_ptr(0x801c8000,12),record,12);memcpy(ff_ptr(0x801c8100,16),xy,16);memcpy(ff_ptr(0x801c8200,8),depth,8);memcpy(ff_ptr(0x800a8768,12),uv,12);
   ff_w32(0x8008d4b0,h[3]);ff_w32(0x8008d4b4,0x801cb000);ff_w32(0x8008d4bc,h[1]);ff_w32(0x8008d4c0,h[2]);memset(ff_ptr(h[3],40),0xa5,40);FUN_80011D50(0x801cb000);
   result=ff_model_packets_80010CA0(1,0x801c8000,0x801c8100,0x801c8200,(sint32)h[0]);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(h[3],40),40,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-actor-render")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,result;
  in=fopen("../status/menu/actor-render-setup.bin","rb");out=fopen("../status/menu/actor-render-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<6;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   ff_w32(0x801c8000,0);ff_w32(0x801c8004,1);ff_w32(0x8009272c,i>=3?3:0);ff_w32(0x8009278c,i==5?40:0);ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_80058E24(0x801c8000,i%3);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210,2016),2016,1,out);fwrite(ff_ptr(0x800b4318,976),976,1,out);fwrite(ff_ptr(0x8009357c,8),8,1,out);fwrite(ff_ptr(0x801fe400,2184),2184,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-object-compound")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,model,result;
  in=fopen("../status/menu/object-compound-setup.bin","rb");out=fopen("../status/menu/object-compound-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<6;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   model=0x800b8b10;ff_w32(0x80093dd0,i*8193);if(i==1)ff_w32(model+8,0xe0000000);if(i==2)ff_w32(model+8,0xdfffffff);if(i>=3){ff_w16(model+16,4096);ff_w16(model+18,8192);ff_w16(model+20,0xf000);}ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_800451A0(model);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-object-effects")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,model,result;
  in=fopen("../status/menu/object-effects-setup.bin","rb");out=fopen("../status/menu/object-effects-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<8;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   model=0x800b8b34+36*(i/4);ff_w32(0x80093dd0,(i%4)*2);if(i>=3){ff_w16(model+16,4096);ff_w16(model+18,8192);ff_w16(model+20,0xf000);}ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_800452AC(model);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-effect-render")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,result;
  in=fopen("../status/menu/effect-render-setup.bin","rb");out=fopen("../status/menu/effect-render-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   ff_w32(0x80098270+4*454,i==2?0:0x8002a0c4);ff_w32(0x80098270+4*455,0);
   if(i){memcpy(ff_ptr(0x8009d1d0+36*(i==1?0:127),36),ff_ptr(0x800b89f0,36),36);}
   if(i==3){memcpy(ff_ptr(0x8009d1d0,36),ff_ptr(0x800b89f0,36),36);ff_w32(0x8009d1d0+36,455);}
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8001EF68();
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-dynamic-render")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,j,result;
  in=fopen("../status/menu/dynamic-render-setup.bin","rb");out=fopen("../status/menu/dynamic-render-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   for(j=0;j<8;j++)ff_w16(0x800b4395+244*j,65535);
   if(i==2)*(uint8*)ff_ptr(0x800b4395,1)=63;
   if(i==3)*(uint8*)ff_ptr(0x800b4396+244*7,1)=0;
   ff_w32(0x800bff00+4*454,0x8002a0c4);ff_w32(0x800bff00+4*455,0);
   if(i){memcpy(ff_ptr(0x8009a3c8+36*(i==1?0:63),36),ff_ptr(0x800b89f0,36),36);}
   if(i==3){memcpy(ff_ptr(0x8009a3c8,36),ff_ptr(0x800b89f0,36),36);}
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8001EE44();
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-dynamic-loop")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,result;
  in=fopen("../status/menu/dynamic-loop-setup.bin","rb");out=fopen("../status/menu/dynamic-loop-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   ff_w32(0x800bbf78+4*454,0x8002a0c4);ff_w32(0x800bbf78+4*455,0);
   if(i){memcpy(ff_ptr(0x8009a3c8+36*(i==1?0:63),36),ff_ptr(0x800b89f0,36),36);}
   if(i==3){memcpy(ff_ptr(0x8009a3c8,36),ff_ptr(0x800b89f0,36),36);}
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8001ED1C();
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-effect-loop")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,result;
  in=fopen("../status/menu/effect-loop-setup.bin","rb");out=fopen("../status/menu/effect-loop-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   ff_w32(0x800be5e0+4*454,0x8002a0c4);ff_w32(0x800be5e0+4*455,0);
   if(i){memcpy(ff_ptr(0x8009d1d0+36*(i==1?0:127),36),ff_ptr(0x800b89f0,36),36);}
   if(i==3){memcpy(ff_ptr(0x8009d1d0,36),ff_ptr(0x800b89f0,36),36);ff_w32(0x8009d1d0+36,455);}
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8001EDB8();
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-scene-render")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,result;
  in=fopen("../status/menu/scene-render-setup.bin","rb");out=fopen("../status/menu/scene-render-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   ff_w32(0x80093dd0,2*i);if(i==1)ff_w32(0x8009403c,0);if(i==2)ff_w32(0x8009403c,0xffffffff);ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8005C17C();
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-object-basic")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,model,result;
  in=fopen("../status/menu/object-basic-setup.bin","rb");out=fopen("../status/menu/object-basic-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<6;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   model=0x800b89f0+36*(i%3);if(i>=3){ff_w16(model+16,4096);ff_w16(model+18,8192);ff_w16(model+20,0xf000);}ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8002A0C4(model);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-model-static")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,model,result;
  in=fopen("../status/menu/model-static-setup.bin","rb");out=fopen("../status/menu/model-static-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<3;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   model=ff_u32(i==0?0x80082490:i==1?0x800824fc:0x80082500);ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_80067B8C(model,0,0);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-model-full")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,model,result;
  in=fopen("../status/menu/model-full-setup.bin","rb");out=fopen("../status/menu/model-full-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   model=ff_u32(0x80081720+4u*(uint32)(sint32)ff_s16(0x800b4318+244*i+52));ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801cb000);memset(ff_ptr(0x801d0000,65536),0xa5,65536);FUN_80011D50(0x801cb000);result=FUN_8001060C(model,0x800ba210+504*i);
   fwrite(ff_ptr(0x801cb000,8192),8192,1,out);fwrite(ff_ptr(0x801d0000,65536),65536,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,8),8,1,out);fwrite(&result,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-model-transform")==0) {
  FILE *in,*out;uint8 scratch[1024];sint32 setup[3];uint32 i,model;
  in=fopen("../status/menu/model-transform-setup.bin","rb");out=fopen("../status/menu/model-transform-native.bin","wb");if(!in||!out)return 4;
  if(fread(setup,12,1,in)!=1||fread(scratch,1024,1,in)!=1)return 5;fclose(in);
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x1f800000,1024),scratch,1024);InitGeom();SetGeomOffset(setup[0],setup[1]);SetGeomScreen(setup[2]);
   model=ff_u32(0x80081720+4u*(uint32)(sint32)ff_s16(0x800b4318+244*i+52));ff_model_transform_8001060C(model,0x800ba210+504*i);
   fwrite(ff_ptr(0x1fe800,1456),1456,1,out);fwrite(ff_ptr(0x1fe400,728),728,1,out);fwrite(ff_ptr(0x800ba210+504*i,504),504,1,out);
   fwrite(ff_ptr(0x1f800000,1024),1024,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-model-matrix")==0) {
  FILE *in,*out;uint32 h[2];uint8 state[52],zero[30]={0};MATRIX m;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/model-matrix-jobs.bin","rb");out=fopen("../status/menu/model-matrix-native.bin","wb");if(!in||!out)return 4;
  while(fread(h,8,1,in)==1){
   if(fread(state,52,1,in)!=1)return 5;memcpy(ff_ptr(0x1f8003c0,52),state,52);ff_w32(0x8008d494,0x1f8003c0);InitGeom();
   if(h[0]==0)FUN_800104A0((sint32)h[1]);else if(h[0]==1)ff_roll_index_800104B4(h[1]);else if(h[0]==2)FUN_80010170();else if(h[0]==3)ff_pitch_index_800101E0(h[1]);else ff_yaw_index_80010350(h[1]);
   fwrite(ff_ptr(0x1f8003c0,52),52,1,out);
   if(h[0]==2){ReadRotMatrix(&m);fwrite(m.m,18,1,out);fwrite(m.t,12,1,out);}else fwrite(zero,30,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-actor-loop")==0) {
  FILE *out;uint32 frame,r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  out=fopen("../status/menu/actor-loop-native.bin","wb");if(!out)return 4;
  for(frame=0;frame<120;frame++){
   r=(uint32)FUN_80050CC8();fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);fwrite(ff_ptr(0x800ba210,2016),2016,1,out);
   fwrite(ff_ptr(0x80093d54,4),4,1,out);fwrite(ff_ptr(0x80094e88,4),4,1,out);fwrite(ff_ptr(0x800941a0,4),4,1,out);fwrite(ff_ptr(0x8008e4ec,4),4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-actor-update")==0) {
  FILE *in,*out;uint32 h[4],r;uint8 actors[1952],pose[2016];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/actor-update-jobs.bin","rb");out=fopen("../status/menu/actor-update-native.bin","wb");if(!in||!out)return 4;
  while(fread(h,16,1,in)==1){
   if(fread(actors,1952,1,in)!=1||fread(pose,2016,1,in)!=1)return 5;memcpy(ff_ptr(0x800b4318,1952),actors,1952);memcpy(ff_ptr(0x800ba210,2016),pose,2016);
   ff_w32(0x80093d58,h[2]);ff_w32(0x800940b0,h[3]);ff_w32(0x80093d54,0);ff_w32(0x80094e88,0x12345678);ff_w32(0x800941a0,0);ff_w32(0x8008e4ec,0);
   r=(uint32)(h[0]?FUN_8001AEF4((sint32)h[1]):FUN_80023C80((sint32)h[1]));fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);fwrite(ff_ptr(0x800ba210,2016),2016,1,out);
   fwrite(ff_ptr(0x80093d54,4),4,1,out);fwrite(ff_ptr(0x80094e88,4),4,1,out);fwrite(ff_ptr(0x800941a0,4),4,1,out);fwrite(ff_ptr(0x8008e4ec,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-collision-actors")==0) {
  FILE *in,*out;uint32 h[4],r,i;uint8 actors[1952],radii[128],tables[256];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  memcpy(radii,ff_ptr(0x80094120,128),128);memcpy(tables,ff_ptr(0x800b3be8,256),256);
  in=fopen("../status/menu/collision-actors-jobs.bin","rb");out=fopen("../status/menu/collision-actors-native.bin","wb");if(!in||!out)return 4;
  while(fread(h,16,1,in)==1){
   if(fread(actors,1952,1,in)!=1)return 5;memcpy(ff_ptr(0x800b4318,1952),actors,1952);
   memcpy(ff_ptr(0x80094120,128),radii,128);memcpy(ff_ptr(0x800b3be8,256),tables,256);
   ff_w32(0x80093d54,h[1]);ff_w32(0x80094e88,h[2]);ff_w32(0x800941a0,0x12345678);ff_w32(0x8008e4ec,0);
   if(!h[3]){for(i=0;i<64;i++){ff_w16(0x80094120+2*i,50);ff_w32(0x800b3be8+4*i,0x801c9000);}memset(ff_ptr(0x801c9000,1024),0,1024);ff_w32(0x801c924c,1);}
   r=(uint32)FUN_8001B2C4((sint32)h[0]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);
   fwrite(ff_ptr(0x80093d54,4),4,1,out);fwrite(ff_ptr(0x80094e88,4),4,1,out);fwrite(ff_ptr(0x800941a0,4),4,1,out);fwrite(ff_ptr(0x8008e4ec,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-collision-root")==0) {
  FILE *in,*out;uint32 a[2],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/collision-root-jobs.bin","rb");out=fopen("../status/menu/collision-root-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,8,1,in)==1){ff_w32(0x80094e88,a[1]);r=a[0]?ff_rand_8006D348():(uint32)FUN_80012B5C(a[1]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x80094e88,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-collision-world")==0) {
  FILE *in,*out;uint32 h[4],r,xa,za;uint8 actor[244],obstacles[144],object[32];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/collision-world-jobs.bin","rb");out=fopen("../status/menu/collision-world-native.bin","wb");if(!in||!out)return 4;
  while(fread(h,16,1,in)==1){
   if(fread(actor,244,1,in)!=1||fread(obstacles,144,1,in)!=1||fread(object,32,1,in)!=1)return 5;
   memcpy(ff_ptr(0x801d0000,244),actor,244);memcpy(ff_ptr(0x8009ec70,144),obstacles,144);memcpy(ff_ptr(0x801c9000,32),object,32);
   ff_w32(0x8009479c,h[0]);ff_w32(0x800941a0,0x12345678);xa=h[1]?0x801d0028:0x801c8000;za=h[1]?0x801d0030:0x801c8004;
   ff_w32(xa,h[2]);ff_w32(za,h[3]);r=(uint32)FUN_8001A4D0(0x801d0000,xa,za);
   fwrite(&r,4,1,out);fwrite(ff_ptr(0x801d0000,244),244,1,out);fwrite(ff_ptr(xa,4),4,1,out);fwrite(ff_ptr(za,4),4,1,out);
   fwrite(ff_ptr(0x800941a0,4),4,1,out);fwrite(ff_ptr(0x8009ec70,144),144,1,out);fwrite(ff_ptr(0x801c9000,32),32,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-collision-gate")==0) {
  FILE *in,*out;uint32 a[6],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  ff_services.collision_event=collision_audit_callback;
  in=fopen("../status/menu/collision-gate-jobs.bin","rb");out=fopen("../status/menu/collision-gate-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,24,1,in)==1){
   memset(collision_audit_event,0,sizeof(collision_audit_event));
   ff_w32(0x1d000c,a[0]);ff_w16(0x1d0052,(uint16)a[1]);ff_w32(0x1c8014,a[2]?0x801c9000:0);ff_w16(0x1c800c,(uint16)a[3]);
   ff_w32(0x1c9004,a[4]);ff_w32(0x1c900c,a[5]);r=(uint32)FUN_8001A3F8(0x801c8000,0x801d0000);
   fwrite(&r,4,1,out);r=(uint16)ff_s16(0x1d0052);fwrite(&r,4,1,out);fwrite(collision_audit_event,28,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-collision-distance")==0) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/collision-distance-jobs.bin","rb");out=fopen("../status/menu/collision-distance-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1){r=(uint32)FUN_800163F4((sint32)a[0],(sint32)a[1],(sint32)a[2],(sint32)a[3]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x800941a0,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-spatial-audio")==0) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/spatial-audio-jobs.bin","rb");out=fopen("../status/menu/spatial-audio-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1){ff_w16(0x8008d4a6,(uint16)a[0]);ff_w16(0x8008d4ae,(uint16)a[1]);ff_w16(0x801c8002,(uint16)a[2]);ff_w16(0x801c800a,(uint16)a[3]);ff_w16(0x8008d490,(uint16)(a[0]+a[3]));r=(uint32)FUN_80046884(0x801c8000,0x801c8020,0x801c8024);fwrite(&r,4,1,out);fwrite(ff_ptr(0x800941a0,4),4,1,out);fwrite(ff_ptr(0x801c8020,8),8,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-texture-uv-fix")) {
  FILE *in,*out;uint32 a[4];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/texture-uv-fix-jobs.bin","rb");out=fopen("../status/menu/texture-uv-fix-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1) {
   ff_w32(0x80093d58,a[0]);ff_w16(0x800947ce,a[3]);
   memset(ff_ptr(0x80094400,512),0xa5,512);memset(ff_ptr(0x801d0000,12),0xa5,12);
   ff_texture_uv_fix_80060B90(a[1],a[2],0x801d0000);
   fwrite(ff_ptr(0x80094400,512),512,1,out);fwrite(ff_ptr(0x801d0000,12),12,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-texture-descriptor")) {
  FILE *in,*out;uint32 a[10];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/texture-descriptor-jobs.bin","rb");out=fopen("../status/menu/texture-descriptor-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,40,1,in)==1) {
   ff_w32(0x80094838,64);ff_w32(0x8009440c,5);ff_w16(0x8009acd2,0xa5a5);
   memset(ff_ptr(0x800a8a5c,48),0xa5,48);ff_w16(0x800947cc,a[8]);ff_w16(0x800947ce,a[9]);
   ff_texture_descriptor_800609B8(a[0],a[1],a[2],a[3],a[4],a[5],a[6],a[7]);
   fwrite(ff_ptr(0x800a8a5c,48),48,1,out);fwrite(ff_ptr(0x8009acd2,2),2,1,out);
   fwrite(ff_ptr(0x80094838,4),4,1,out);fwrite(ff_ptr(0x800947cc,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-effect-palette")) {
  FILE *in,*out;uint32 stage;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/effect-palette-jobs.bin","rb");out=fopen("../status/menu/effect-palette-native.bin","wb");if(!in||!out)return 4;
  while(fread(&stage,4,1,in)==1) {
   if(fread(ff_ptr(0x801d0000,512),512,1,in)!=1)return 4;
   ff_w32(0x80093d58,stage);memset(ff_ptr(0x800a1870,288),0xa5,288);
   memset(ff_ptr(0x800947f8,30),0xa5,30);memset(ff_ptr(0x80094418,48),0xa5,48);
   ff_effect_palette_8006055C(0x801d0000);
   fwrite(ff_ptr(0x801d0000,512),512,1,out);fwrite(ff_ptr(0x800a1870,288),288,1,out);
   fwrite(ff_ptr(0x800947f8,30),30,1,out);fwrite(ff_ptr(0x80094418,48),48,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-texture-pack")) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/texture-pack-jobs.bin","rb");out=fopen("../status/menu/texture-pack-native.bin","wb");if(!in||!out)return 4;
  FUN_80066EF4();
  while(fread(a,16,1,in)==1) {
   memset(ff_ptr(0x801d0000,8),0xa5,8);
   r=a[0]?FUN_80066D14():FUN_80066ACC(a[1],a[2],a[3],0x801d0000);
   fwrite(&r,4,1,out);fwrite(ff_ptr(0x800947cc,4),4,1,out);fwrite(ff_ptr(0x801d0000,8),8,1,out);
  }
  fwrite(ff_ptr(0x800e1140,0x4000),0x4000,1,out);fwrite(ff_ptr(0x80094208,128),128,1,out);
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-texture-map-reset")) {
  FILE *out;uint32 i,r;if(!ff_load_ram("FF-menu.ram"))return 2;
  out=fopen("../status/menu/texture-map-reset-native.bin","wb");if(!out)return 4;
  for(i=0;i<3;i++) {
   memset(ff_ptr(0x800e1130,0x4020),i?0xff:0xa5,0x4020);
   memset(ff_ptr(0x800941f8,160),i?0xff:0xa5,160);
   r=FUN_80066EF4();if(i==2)r=FUN_80066EF4();
   fwrite(&r,4,1,out);fwrite(ff_ptr(0x800e1130,0x4020),0x4020,1,out);
   fwrite(ff_ptr(0x800941f8,160),160,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-resource-exists")) {
  FILE *in,*out;uint8 name[16];uint32 count,r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/resource-exists-jobs.bin","rb");out=fopen("../status/menu/resource-exists-native.bin","wb");if(!in||!out)return 4;
  while(fread(name,16,1,in)==1) {
   if(fread(&count,4,1,in)!=1)return 4;
   memcpy(ff_ptr(0x801d0000,16),name,16);ff_w32(ff_u32(0x80094038),FUN_80011F20(count));ff_w32(0x80094570,0xa5a5a5a5);
   r=FUN_80061140(0x801d0000);fwrite(&r,4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-terrain-descriptors")) {
  FILE *in,*out;uint32 base,p;
  in=fopen("../status/menu/terrain-descriptors-jobs.bin","rb");out=fopen("../status/menu/terrain-descriptors-native.bin","wb");if(!in||!out)return 4;
  while(fread(&base,4,1,in)==1) {
   if(!ff_load_ram("FF-menu.ram"))return 2;
   p=0x800a8768+12*base;memset(ff_ptr(p,3096),0xa5,3096);ff_w32(0x80094838,base);
   ff_terrain_descriptors_80060DAC();
   fwrite(ff_ptr(0x800af4b0,4),4,1,out);fwrite(ff_ptr(0x800947cc,4),4,1,out);fwrite(ff_ptr(0x80094838,4),4,1,out);fwrite(ff_ptr(p,3096),3096,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-mpd-objects")) {
  FILE *in,*out;uint8 data[8303];uint32 r;
  in=fopen("../status/menu/mpd-objects-jobs.bin","rb");out=fopen("../status/menu/mpd-objects-native.bin","wb");if(!in||!out)return 4;
  while(fread(data,8303,1,in)==1) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(ff_ptr(0x801d0000,8303),data,8303);
   r=ff_menu_objects_800173B0(0x801d0000);fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b89f0,4608),4608,1,out);fwrite(ff_ptr(0x8009ec70,3072),3072,1,out);fwrite(ff_ptr(0x80094000,2048),2048,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-scene-adjust")) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/scene-adjust-jobs.bin","rb");out=fopen("../status/menu/scene-adjust-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1) {
   memset(ff_ptr(0x801d0000,360),0xa5,360);memset(ff_ptr(0x80094000,2048),0xa5,2048);ff_w32(0x8009403c,7);ff_w32(0x80094e88,a[3]);
   r=FUN_800406C4(0x801d0024,a[0]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x80094e88,4),4,1,out);fwrite(ff_ptr(0x80094000,2048),2048,1,out);fwrite(ff_ptr(0x801d0000,360),360,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-scene-expand-primary")) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/scene-expand-primary-jobs.bin","rb");out=fopen("../status/menu/scene-expand-primary-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1) {
   memset(ff_ptr(0x801d0000,360),0xa5,360);memset(ff_ptr(0x80094000,2048),0xa5,2048);ff_w32(0x8009403c,7);
   r=FUN_80034244(0x801d0024,a[0],a[1],a[2]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x80094000,2048),2048,1,out);fwrite(ff_ptr(0x801d0000,360),360,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-scene-expand")) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/scene-expand-jobs.bin","rb");out=fopen("../status/menu/scene-expand-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1) {
   memset(ff_ptr(0x801d0000,144),0xa5,144);ff_w32(0x800940ac,0x12345678);
   r=FUN_8003EECC(0x801d0024,a[0],a[1],a[2]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x800940ac,4),4,1,out);fwrite(ff_ptr(0x801d0000,144),144,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-terrain-relocate")) {
  FILE *in,*out;uint32 offset,r;uint8 data[8192];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/terrain-relocate-jobs.bin","rb");out=fopen("../status/menu/terrain-relocate-native.bin","wb");if(!in||!out)return 4;
  while(fread(&offset,4,1,in)==1) {
   if(fread(data,8192,1,in)!=1)return 4;
   memcpy(ff_ptr(0x8009f870,8192),data,8192);ff_w16(0x800af4b2,(uint16)offset);
   r=ff_terrain_relocate_8006109C();fwrite(&r,4,1,out);fwrite(ff_ptr(0x8009f870,8192),8192,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-terrain-map")) {
  FILE *in,*out;uint32 r;uint8 data[8192];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/terrain-map-pixels.bin","rb");if(!in)return 4;
  if(fread(ff_ptr(0x80100000,65536),65536,1,in)!=1)return 4;fclose(in);
  in=fopen("../status/menu/terrain-map-jobs.bin","rb");out=fopen("../status/menu/terrain-map-native.bin","wb");if(!in||!out)return 4;
  while(fread(data,8192,1,in)==1) {
   memcpy(ff_ptr(0x801d0000,8192),data,8192);memset(ff_ptr(0x8009f870,8192),0xa5,8192);memset(ff_ptr(0x800a3290,16384),0xa5,16384);
   r=ff_terrain_map_80063AE8(0x801d0000,0x80100000);fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x8009f870,8192),8192,1,out);fwrite(ff_ptr(0x800a3290,16384),16384,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sound-init")) {
  FILE *out;uint32 stage,slot,prior,i,result;SPU_voice_registers r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  out=fopen("../status/menu/sound-init-native.bin","wb");if(!out)return 4;
  for(stage=0;stage<2;stage++)for(slot=0;slot<2;slot++)for(prior=0;prior<2;prior++) {
   ff_audio_init_empty();ff_w32(0x80093d58,stage?17:26);ff_w32(0x80093550,slot?15:0);ff_w32(0x80093558,prior);
   ff_w32(0x8008cff4,0);ff_w32(0x8008cfb8,3);ff_w32(0x8008cf90,0x801e0000);
   memset(ff_ptr(0x800a79c8,1024),0xa5,1024);memset(ff_ptr(0x80094820,16),0xa5,16);
   result=(uint32)FUN_80056BC8();fwrite(&result,4,1,out);
   fwrite(ff_ptr(0x8008cb28,96),96,1,out);fwrite(ff_ptr(0x8008cfa8,40),40,1,out);fwrite(ff_ptr(0x8008cfec,12),12,1,out);fwrite(ff_ptr(0x800adbc8,20),20,1,out);
   fwrite(ff_ptr(0x80093550,12),12,1,out);fwrite(ff_ptr(0x800a79c8,1024),1024,1,out);fwrite(ff_ptr(0x80094820,16),16,1,out);
   for(i=0;i<24;i++){spu_core_get_voice_registers(i,&r);fwrite(&r,14,1,out);}
   fwrite(ff_spu_common_state,10,1,out);fwrite(ff_spu_reverb_registers,64,1,out);fwrite(ff_spu_reverb_state,6,1,out);fwrite(ff_spu_reverb_channels_state,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-stage-reverb")) {
  FILE *in,*out;uint32 job[4],result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/stage-reverb-jobs.bin","rb");out=fopen("../status/menu/stage-reverb-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w32(0x80093d58,job[0]);ff_w32(0x80093558,0xa5a5a5a5);
   memset(ff_ptr(0x8008cb30,32),0x5a,32);ff_w32(0x8008cb40,1);ff_w32(0x8008cf88,0);
   ff_w32(0x8008cfb8,3);ff_w32(0x8008cff4,0x801d1000);
   ff_w32(0x801d1000,job[1]?0x1000:0x40000000);ff_w32(0x801d1004,0x7f000);ff_w32(0x801d1008,0x40000000);ff_w32(0x801d100c,0);
   memset(ff_spu_reverb_registers,0xa5,64);memset(ff_spu_reverb_state,0xa5,6);ff_spu_common_state[4]=(uint16)job[2];memcpy(ff_spu_reverb_channels_state,job+3,4);
   result=(uint32)FUN_80056A18();fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008cb30,32),32,1,out);fwrite(ff_ptr(0x80093558,4),4,1,out);
   fwrite(ff_spu_reverb_registers,64,1,out);fwrite(ff_spu_reverb_state,6,1,out);fwrite(ff_spu_common_state+4,2,1,out);fwrite(ff_spu_reverb_channels_state,4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-reverb-presets")) {
  FILE *in,*out;uint32 job[10],result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/reverb-presets-jobs.bin","rb");out=fopen("../status/menu/reverb-presets-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   memcpy(ff_ptr(0x801d0000,20),job,20);memset(ff_spu_reverb_registers,0xa5,64);memset(ff_spu_reverb_state,0xa5,6);
   ff_spu_common_state[4]=(uint16)job[5];memset(ff_ptr(0x8008cb38,24),0x5a,24);ff_w32(0x8008cb40,job[6]);
   ff_w32(0x8008cfb8,3);ff_w32(0x8008cff4,0x801d1000);
   ff_w32(0x801d1000,job[7]);ff_w32(0x801d1004,job[8]);ff_w32(0x801d1008,0x40000000);ff_w32(0x801d100c,0);
   result=(uint32)ff_spu_reverb_attributes(0x801d0000);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008cb38,24),24,1,out);fwrite(ff_spu_reverb_registers,64,1,out);fwrite(ff_spu_reverb_state,6,1,out);fwrite(ff_spu_common_state+4,2,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-reverb-registers")) {
  FILE *in,*out;uint8 job[132];uint32 result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/reverb-registers-jobs.bin","rb");out=fopen("../status/menu/reverb-registers-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   memcpy(ff_ptr(0x801d0000,68),job,68);memcpy(ff_spu_reverb_registers,job+68,64);
   result=ff_spu_reverb_register_write(0x801d0000);fwrite(&result,4,1,out);fwrite(ff_spu_reverb_registers,64,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-reverb-control")) {
  FILE *in,*out;uint32 job[16],result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/reverb-control-jobs.bin","rb");out=fopen("../status/menu/reverb-control-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w32(0x8008cfb8,job[2]);ff_w32(0x8008cff4,job[3]?0x801d0000:0);
   ff_w32(0x8008cb38,job[4]);ff_w32(0x8008cb34,job[5]);ff_w32(0x8008cb30,job[6]);
   ff_spu_common_state[4]=(uint16)job[7];memcpy(ff_ptr(0x801d0000,32),job+8,32);
   result=(uint32)(job[0]==0?ff_spu_reverb_conflict(job[1]):job[0]==1?ff_spu_reverb_reserve((sint32)job[1]):ff_spu_reverb_enable((sint32)job[1]));
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008cb30,12),12,1,out);fwrite(ff_spu_common_state+4,2,1,out);fwrite(ff_ptr(0x801d0000,32),32,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-spu-reset")) {
  FILE *out;uint32 seed,i,result;SPU_voice_registers r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  out=fopen("../status/menu/spu-reset-native.bin","wb");if(!out)return 4;
  for(seed=0;seed<3;seed++) {
   uint8 fill=(uint8)(seed*0x55);ff_audio_init_empty();
   memset(ff_ptr(0x8008cb28,96),fill,96);memset(ff_ptr(0x8008cfa8,40),fill,40);
   memset(ff_ptr(0x8008cfec,12),fill,12);memset(ff_ptr(0x800adbc8,20),fill,20);
   ff_w32(0x8008cf90,0x801e0000);
   for(i=0;i<24;i++){memset(&r,fill,sizeof(r));spu_core_set_voice_registers(i,&r);}
   result=ff_spu_reset_startup();fwrite(&result,4,1,out);
   fwrite(ff_ptr(0x8008cb28,96),96,1,out);fwrite(ff_ptr(0x8008cfa8,40),40,1,out);
   fwrite(ff_ptr(0x8008cfec,12),12,1,out);fwrite(ff_ptr(0x800adbc8,20),20,1,out);
   for(i=0;i<24;i++){spu_core_get_voice_registers(i,&r);fwrite(&r,14,1,out);}
   fwrite(ff_spu_common_state,10,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-voice-defaults")) {
  static const uint32 masks[]={0,0xffffff,0x12345678};FILE *out;uint32 test,i,result;SPU_voice_registers r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  out=fopen("../status/menu/voice-defaults-native.bin","wb");if(!out)return 4;
  for(test=0;test<3;test++) {
   ff_audio_init_empty();ff_w32(0x8008cf88,0);ff_w32(0x8008cb28,masks[test]);memset(ff_ptr(0x801d0000,64),0xa5,64);
   result=(uint32)ff_sound_voice_defaults_80056C30(0x801d0000);fwrite(&result,4,1,out);fwrite(ff_ptr(0x801d0000,64),64,1,out);
   for(i=0;i<24;i++){spu_core_get_voice_registers(i,&r);fwrite(&r,14,1,out);}
   fwrite(ff_ptr(0x8008cb28,4),4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-spu-common")) {
  FILE *in,*out;uint8 job[42];uint32 result;sint16 left,right;
  if(!ff_load_ram("FF-menu.ram"))return 2;ff_audio_init_empty();
  in=fopen("../status/menu/spu-common-jobs.bin","rb");out=fopen("../status/menu/spu-common-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,42,1,in)==1) {
   memcpy(ff_ptr(0x801d0000,40),job,40);ff_spu_common_state[4]=(uint16)(job[40]|job[41]<<8);
   result=(uint32)ff_spu_common_startup(0x801d0000);spu_core_get_master_volume(&left,&right);
   if((uint16)left!=ff_spu_common_state[0]||(uint16)right!=ff_spu_common_state[1])return 5;
   fwrite(&result,4,1,out);fwrite(ff_spu_common_state,10,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-spu-startup-voices")) {
  FILE *in,*out;uint8 job[64];uint32 i,result;SPU_voice_registers r;
  if(!ff_load_ram("FF-menu.ram"))return 2;ff_audio_init_empty();
  in=fopen("../status/menu/spu-startup-voices-jobs.bin","rb");out=fopen("../status/menu/spu-startup-voices-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,64,1,in)==1) {
   for(i=0;i<24;i++){r.volume_left=(sint16)(0x1100+i);r.volume_right=(sint16)(0x2200+i);r.pitch=(uint16)(0x3300+i);r.start_address=(uint16)(0x4400+i);r.adsr1=(uint16)(0x5500+i);r.adsr2=(uint16)(0x6600+i);r.repeat_address=(uint16)(0x7700+i);spu_core_set_voice_registers(i,&r);}
   memset(ff_ptr(0x8008cb58,48),0xa5,48);memcpy(ff_ptr(0x801d0000,64),job,64);
   result=(uint32)ff_services.spu_voice_attributes(0x801d0000);fwrite(&result,4,1,out);
   for(i=0;i<24;i++){spu_core_get_voice_registers(i,&r);fwrite(&r,14,1,out);}
   fwrite(ff_ptr(0x8008cb58,48),48,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-score-format")) {
  FILE *in,*out;uint32 job[3],result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/score-format-jobs.bin","rb");out=fopen("../status/menu/score-format-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w32(0x80094030,job[1]);ff_w32(0x80094034,job[2]);memset(ff_ptr(0x80094730,32),0xa5,32);
   result=(uint32)FUN_8005E9E8((sint32)job[0]);fwrite(&result,4,1,out);fwrite(ff_ptr(0x80094730,32),32,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-menu-prep")) {
  FILE *in,*out;uint32 job[3],result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/menu-prep-jobs.bin","rb");out=fopen("../status/menu/menu-prep-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   memset(ff_ptr(0x800940f8,32),0xa5,32);memset(ff_ptr(0x80093bac,12),0xa5,12);*(uint8*)ff_ptr(0x80093d6f,1)=(uint8)job[2];
   memset(ff_dummy_cd_mix_state,0xa5,4);memset(ff_dummy_cd_track_state,0xa5,4);ff_dummy_calls[1]=0;
   result=job[0]==0?FUN_8005FB00((sint32)job[1]):job[0]==1?(uint32)FUN_80063870((sint32)job[1]):(uint32)FUN_80067670(0,(sint32)job[1]);
   fwrite(&result,4,1,out);fwrite(ff_ptr(0x80093bac,12),12,1,out);fwrite(ff_ptr(0x800940f8,32),32,1,out);
   fwrite(ff_dummy_cd_mix_state,4,1,out);fwrite(ff_dummy_cd_track_state,4,1,out);fwrite(ff_dummy_calls+1,4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-startup-table")) {
  FILE *out;uint32 result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  ff_w32(0x8008d4d8,0x801d0000);memset(ff_ptr(0x801cfff0,16416),0xa5,16416);
  result=FUN_8006A50C();out=fopen("../status/menu/startup-table-native.bin","wb");if(!out)return 4;
  fwrite(&result,4,1,out);fwrite(ff_ptr(0x801cfff0,16416),16416,1,out);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sound-load")) {
  FILE *out;uint32 r,i;SPU_voice_registers v;
  if(!ff_load_ram("FF-menu.ram"))return 2;ff_audio_init_empty();
  ff_w16(0x80093566,0);ff_w32(0x80093d58,26);ff_w32(0x8008cf90,0x801e0000);ff_w32(0x8008cfa0,0x801e1000);
  r=(uint32)FUN_80056CCC(197);out=fopen("../status/menu/sound-load-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x8008cb20,0x4e0),0x4e0,1,out);fwrite(ff_ptr(0x8009354c,16),16,1,out);
  fwrite(ff_ptr(0x80093e0c,4),4,1,out);fwrite(ff_ptr(0x80094000,4096),4096,1,out);fwrite(ff_ptr(0x800b6ab8,1446),1446,1,out);
  fwrite(ff_ptr(0x800a79c8,1024),1024,1,out);fwrite(ff_ptr(0x80116000,143712),143712,1,out);
  for(i=0;i<24;i++){spu_core_get_voice_registers(i,&v);fwrite(&v,14,1,out);}
  fwrite(ff_spu_common_state,10,1,out);fwrite(ff_spu_reverb_registers,64,1,out);fwrite(ff_spu_reverb_state,6,1,out);fwrite(ff_spu_reverb_channels_state,4,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-sound-resources")) {
  FILE *out;uint32 r;
  if(!ff_load_ram("FF-menu.ram"))return 2;ff_audio_init_empty();
  ff_w16(0x80093566,0);memset(ff_ptr(0x800b6ab8,984),0xff,984);
  ff_spu_allocation_init(2,0x80094820);FUN_80061448(197,0x116000);
  r=(uint32)ff_sound_resources_80056D2C();out=fopen("../status/menu/sound-resources-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x8008cb20,0x4e0),0x4e0,1,out);fwrite(ff_ptr(0x8009354c,4),4,1,out);
  fwrite(ff_ptr(0x80093e0c,4),4,1,out);fwrite(ff_ptr(0x80094000,4096),4096,1,out);fwrite(ff_ptr(0x800b6ab8,1446),1446,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-spu-allocation")) {
  FILE *in,*out;uint32 job[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/spu-allocation-jobs.bin","rb");out=fopen("../status/menu/spu-allocation-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   memset(ff_ptr(0x801d0000,32),0xa5,32);memset(ff_ptr(0x8008cfec,12),0xa5,12);
   ff_w32(0x8008cb34,job[2]);ff_w32(0x8008cb38,job[3]);ff_w32(0x8008cfb8,3);ff_w32(0x8008cfc0,7);
   r=(uint32)ff_spu_allocation_init((sint32)job[0],0x801d0000);fwrite(&r,4,1,out);
   r=job[0]&&(sint32)job[0]>0?(uint32)ff_spu_allocate_startup(job[1]):0;fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x801d0000,32),32,1,out);fwrite(ff_ptr(0x8008cfec,12),12,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-resource-progress")) {
  FILE *in,*out;uint32 job[7],r;
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/resource-progress-jobs.bin","rb");out=fopen("../status/menu/resource-progress-native.bin","wb");if(!in||!out)return 4;
  while(fread(job,sizeof(job),1,in)==1) {
   ff_w16(0x80093566,(uint16)job[0]);ff_w32(0x80093e0c,job[1]);ff_w32(0x8008d4c4,job[2]);memcpy(ff_ptr(0x801d0000,16),job+3,16);
   r=FUN_800611F8(0x801d0000);fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);fwrite(ff_ptr(0x8008d4c4,4),4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&(!strcmp(argv[1],"--audit-loading-init")||!strcmp(argv[1],"--audit-progress-preview"))) {
  FILE *out;uint32 r,i;int progress=!strcmp(argv[1],"--audit-progress-preview");
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
  ff_w32(0x8008d4c4,0);ff_w32(0x80093c14,0xffffffff);
  r=FUN_8005ECCC(64);
  if(progress) {
   memcpy(ff_ptr(0x801d0000,11),"TITLE1.PDT",11);
   for(i=0;i<63;i++)if(FUN_800611F8(0x801d0000)!=0x116040)return 5;
   if(ff_u32(0x80093e0c)!=63||ff_u32(0x8008d4c4)!=129||ff_u32(0x80094570)!=163296)return 5;
  }
  out=fopen(progress?"../status/menu/resource-progress-preview-native.bin":"../status/menu/loading-init-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x80080000,0x96000),0x96000,1,out);fwrite(ff_ptr(0x80116000,175104),175104,1,out);
  fclose(out);if(!ff_gpu_save_frame(progress?"../status/menu/resource-progress-frame.bgrx":"../status/menu/loading-init-frame.bgrx"))return 4;ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-menu-transition")) {
  FILE *out;uint32 test,r,frame;
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  out=fopen("../status/menu/menu-transition-native.bin","wb");if(!out)return 4;
  for(test=0;test<30;test++) {
   if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
   {static const uint32 frames[]={0,1,0xffffffff};static const sint32 counts[]={-1,0,1,2,3};
   frame=frames[test%3];ff_w32(0x8008d4c4,frame);ff_w32(0x800940b0,(uint32)counts[test/6]);}
   ff_w32(0x80094030,123450);ff_w32(0x80094034,(uint32)-987650);
   memset(ff_ptr(0x80094738,16),0xa5,16);ff_w32(0x80093c14,test%6>=3?0:0xffffffff);
   memset(ff_ptr(0x800e5140,16384),0xa5,16384);memset(ff_ptr(0x800e9140,4096),0xa5,4096);memset(ff_ptr(0x800ff8a0,4096),0xa5,4096);
   r=FUN_80057630();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x800e5140,16384),16384,1,out);fwrite(ff_ptr(0x800e9140,4096),4096,1,out);fwrite(ff_ptr(0x800ff8a0,4096),4096,1,out);
   fwrite(ff_ptr(0x8008d4b0,28),28,1,out);fwrite(ff_ptr(0x800947a0,40),40,1,out);fwrite(ff_ptr(0x80094738,16),16,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-title-packets")) {
  FILE *out;uint32 test,i,r,frame;
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  out=fopen("../status/menu/title-packets-native.bin","wb");if(!out)return 4;
  for(test=0;test<7;test++) {
   if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
   frame=test==6?0xffffffff:test&1;
   ff_w32(0x8008d4c4,frame);*(uint8*)ff_ptr(0x800872ec,1)=(uint8)(test/2);
   ff_w32(0x80093c14,test&1?0:0xffffffff);ff_w16(0x800af4ac,0);ff_w16(0x8009acc8,5);
   ff_w16(0x80093d38,test&1?3:0);ff_w16(0x80093d3a,test&1?65534:0);
   for(i=0;i<3;i++){uint32 d=0x800a8768+12*(5+i);memset(ff_ptr(d,12),0,12);*(uint8*)ff_ptr(d,1)=(uint8)(i*7);*(uint8*)ff_ptr(d+1,1)=(uint8)(i*11);ff_w16(d+6,0x100+i);}
   {static const sint32 fade[7][3]={{0,0,0},{255,128,64},{-1,1,0},{0,1,0},{-32768,0,32767},{128,128,128},{0,0,0}};for(i=0;i<3;i++)ff_w16(0x8001000c+2*i,(uint16)fade[test][i]);}
   memset(ff_ptr(0x800e5140,16384),0xa5,16384);memset(ff_ptr(0x800e9140,4096),0xa5,4096);memset(ff_ptr(0x800ff8a0,4096),0xa5,4096);
   r=FUN_80069CC8();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x800e5140,16384),16384,1,out);fwrite(ff_ptr(0x800e9140,4096),4096,1,out);fwrite(ff_ptr(0x800ff8a0,4096),4096,1,out);
   fwrite(ff_ptr(0x8008d4b0,28),28,1,out);fwrite(ff_ptr(0x800947a0,40),40,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-loading-screen")) {
  FILE *out;uint32 test,i,r,frame;
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  out=fopen("../status/menu/loading-screen-native.bin","wb");if(!out)return 4;
  for(test=0;test<7;test++) {
   if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
   frame=test==6?0xffffffff:test&1;
   ff_w32(0x8008d4c4,frame);*(uint8*)ff_ptr(0x800872ec,1)=(uint8)(test/2);
   ff_w32(0x80093c14,test&1?0:0xffffffff);ff_w16(0x800af4ae,0);ff_w16(0x8009acc8,5);
   ff_w16(0x80093d38,test&1?3:0);ff_w16(0x80093d3a,test&1?65534:0);
   for(i=0;i<3;i++){uint32 d=0x800a8768+12*(5+i);memset(ff_ptr(d,12),0,12);*(uint8*)ff_ptr(d,1)=(uint8)(i*7);*(uint8*)ff_ptr(d+1,1)=(uint8)(i*11);ff_w16(d+6,0x100+i);}
   memset(ff_ptr(0x800e5140,16384),0xa5,16384);memset(ff_ptr(0x800e9140,4096),0xa5,4096);memset(ff_ptr(0x800ff8a0,4096),0xa5,4096);
   r=FUN_80063C54();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x800e5140,16384),16384,1,out);fwrite(ff_ptr(0x800e9140,4096),4096,1,out);fwrite(ff_ptr(0x800ff8a0,4096),4096,1,out);
   fwrite(ff_ptr(0x8008d4b0,28),28,1,out);fwrite(ff_ptr(0x800947a0,40),40,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-language-init")) {
  uint32 i,r;FILE *out=fopen("../status/menu/language-init-native.bin","wb");if(!out)return 4;
  for(i=0;i<3;i++) {
   if(!ff_load_game_image("GAME.EXE"))return 2;FUN_8003EE4C();FUN_8003EF78();FUN_80046854();ff_w32(0x80093d70,i);
   r=FUN_80018E50();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80080000,0x96000),0x96000,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-device-init")) {
  uint32 i,j,r;FILE *out=fopen("../status/menu/device-init-native.bin","wb");
  static const uint32 spans[][2]={{0x800872ec,128},{0x80093cb0,32},{0x80094748,80},{0x8008b6d4,4}};
  if(!out)return 4;memset(&config,0,sizeof(config));config.headless=1;psx_configure(&config);
  for(i=0;i<2;i++) {
   if(!ff_load_game_image("GAME.EXE"))return 2;
   for(j=0;j<4;j++)memset(ff_ptr(spans[j][0],spans[j][1]),i?0xa5:0,spans[j][1]);
   r=(uint32)FUN_8005717C();fwrite(&r,4,1,out);
   for(j=0;j<4;j++)fwrite(ff_ptr(spans[j][0],spans[j][1]),spans[j][1],1,out);
   fwrite(ff_dummy_card_events,160,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-cd-toc")) {
  uint32 i,r;FILE *out=fopen("../status/menu/cd-toc-native.bin","wb");if(!out)return 4;
  for(i=0;i<2;i++) {
   if(!ff_load_game_image("GAME.EXE"))return 2;
   memset(ff_ptr(0x80094948,120),i?0xa5:0,120);ff_w32(0x8008b6c8,i?0xa5a5a5a5:0);
   r=(uint32)FUN_800675FC();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093ba8,4),4,1,out);
   fwrite(ff_ptr(0x80094948,120),120,1,out);fwrite(ff_ptr(0x8008b6c8,4),4,1,out);fwrite(ff_dummy_cd_mix_state,4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-display-setup")) {
  uint32 i,j;FILE *out=fopen("../status/menu/display-setup-native.bin","wb");
  static const uint32 spans[][2]={{0x800b8938,184},{0x800947a0,40},{0x800e5140,16384},{0x8008d494,4},{0x8008d4c4,4},{0x1f800000,1024}};
  if(!out)return 4;
  for(i=0;i<6;i++) {
   sint32 x,y,h;
   if(!ff_load_game_image("GAME.EXE"))return 2;
   for(j=0;j<6;j++)memset(ff_ptr(spans[j][0],spans[j][1]),i&1?0xa5:0,spans[j][1]);
   ff_w32(0x8008cb18,i&1);ff_w8(0x800872ec,(uint8)(i/2));
   ff_w16(0x80093d38,(uint16)(i*12345));ff_w16(0x80093d3a,(uint16)(0xffff-i));
   ff_w16(0x8008d490,(uint16)(i*7001));ff_w16(0x8008d492,(uint16)(i*11003));
   ff_display_setup_800574B4();
   for(j=0;j<6;j++)fwrite(ff_ptr(spans[j][0],spans[j][1]),spans[j][1],1,out);
   ReadGeomOffset(&x,&y);h=ReadGeomScreen();if(x!=160||y!=120||h!=256)return 7;
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-cold-tables")) {
  uint32 i,r;FILE *out;
  if(!ff_load_game_image("GAME.EXE"))return 2;
  out=fopen("../status/menu/cold-tables-native.bin","wb");if(!out)return 4;fwrite(ff_ptr(0x80000000,0x200000),0x200000,1,out);
  for(i=0;i<3;i++){r=i==0?FUN_8003EE4C():i==1?FUN_8003EF78():FUN_80046854();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093f40,0x2ce28),0x2ce28,1,out);}
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-early-tables")) {
  uint32 i,r;FILE *out=fopen("../status/menu/early-tables-native.bin","wb");if(!out)return 4;
  for(i=0;i<4;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memset(ff_ptr(0x80093f40,0x2ce28),i&1?0xa5:0,0x2ce28);
   r=i<2?FUN_8003D370():FUN_8003EE4C();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093f40,0x2ce28),0x2ce28,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-constant-init")) {
  uint32 i,r;FILE *out=fopen("../status/menu/constant-init-native.bin","wb");if(!out)return 4;
  for(i=0;i<14;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;memset(ff_ptr(0x80093f40,0x2ce28),i&1?0xa5:0,0x2ce28);
   r=i<2?FUN_800456CC():i<4?FUN_8003EF78():i<6?FUN_8003ED10():i<8?FUN_80046244():i<10?FUN_8003BB1C():i<12?FUN_80045BE4():FUN_80046854();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093f40,0x2ce28),0x2ce28,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-menu-startup")) {
  uint32 r;FILE *out;
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;ff_audio_init_empty();
  ff_w16(0x80093566,0);ff_w32(0x80093c14,0xffffffff);ff_w32(0x8008d4c4,0);
  r=ff_menu_startup_8004FBCC();out=fopen("../status/menu/menu-startup-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x80080000,0x180000),0x180000,1,out);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-title-loop")) {
  FILE *out;uint32 r;
  memset(&config,0,sizeof(config));config.headless=1;config.window_width=320;config.window_height=240;config.refresh_rate=60;psx_configure(&config);
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
  ff_w16(0x80093566,0);ff_w32(0x80093c14,0xffffffff);ff_w32(0x8008d4c4,0);
  if(argc>2)ff_w32(0x800940b0,(uint32)atoi(argv[2]));
  {uint32 i;for(i=0;i<2;i++){uint32 a=0x80094748+40*i;memset(ff_ptr(a,8),128,8);*(uint8*)ff_ptr(a,1)=0;*(uint8*)ff_ptr(a+1,1)=0x40;ff_w16(a+2,65535);}}
  r=FUN_80069F78();out=fopen("../status/menu/title-loop-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093dd0,4),4,1,out);fwrite(ff_ptr(0x8008d4c4,4),4,1,out);
  fwrite(ff_ptr(0x80080000,0x96000),0x96000,1,out);fwrite(ff_ptr(0x80116000,175104),175104,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-startup-archives")) {
  static const uint32 indices[]={0,31,66,197,170};FILE *out;uint32 i,r;
  out=fopen("../status/menu/startup-archives-native.bin","wb");if(!out)return 4;
  for(i=0;i<5;i++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;
   memset(ff_ptr(0x80093bac,12),0xa5,12);
   r=FUN_80067724();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093bac,12),12,1,out);
   r=FUN_80061448(indices[i],0x80116000);fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x80116000,r),r,1,out);
   fwrite(ff_ptr(0x80094818,4),4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
   fwrite(ff_ptr(0x800947d0,4),4,1,out);fwrite(ff_ptr(0x80094038,4),4,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-graphics-reset")) {
  FILE *out;uint32 parity,r;
  out=fopen("../status/menu/graphics-reset-native.bin","wb");if(!out)return 4;
  for(parity=0;parity<2;parity++) {
   if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
   ff_w32(0x8008d4c4,parity);ff_w32(0x8008d4b4,parity?0xe5140:0xe7140);
   memset(ff_ptr(0x800e5140,16384),0xa5,16384);
   r=FUN_8005ED78();fwrite(&r,4,1,out);r=FUN_800645E8();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x800e5140,16384),16384,1,out);fwrite(ff_ptr(0x8008d4b0,32),32,1,out);
   fwrite(ff_ptr(0x80093564,4),4,1,out);fwrite(ff_ptr(0x80093e10,6),6,1,out);
   fwrite(ff_ptr(0x80094190,28),28,1,out);fwrite(ff_ptr(0x8001000c,6),6,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-menu-models")) {
  FILE *out;uint32 r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  ff_w32(0x80094844,0x800c1734);ff_w16(0x80093566,0);memset(ff_ptr(0x800c1734,100000),0xa5,100000);
  r=FUN_80040AA0();out=fopen("../status/menu/menu-models-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x800c1734,100000),100000,1,out);
  fwrite(ff_ptr(0x80081720,4000),4000,1,out);fwrite(ff_ptr(0x800990d8,4000),4000,1,out);
  fwrite(ff_ptr(0x800adc70,4636),4636,1,out);fwrite(ff_ptr(0x800b39a0,578),578,1,out);
  fwrite(ff_ptr(0x80094000,4096),4096,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-actor-assets")) {
  FILE *in,*out,*asset;uint8 name[16];uint32 a[4],r,table;
  in=fopen("../status/menu/actor-assets-jobs.bin","rb");out=fopen("../status/menu/actor-assets-native.bin","wb");if(!in||!out)return 4;
  while(fread(name,16,1,in)==1) {
   if(fread(a,16,1,in)!=1)return 4;if(!ff_load_ram("FF-menu.ram"))return 2;
   asset=fopen("assets/MODELSFE.WAD","rb");if(!asset)return 2;if(fread(ff_ptr(0x800c1734,59180),59180,1,asset)!=1)return 2;fclose(asset);
   table=a[0]?0x800adc70:a[1]==0xffffffff?0x8008409c:a[1]==0xfffffffe?0x80084520:a[1]==0xfffffffd?0x800849a4:ff_u32(0x800b3be8+4*a[1]);
   memset(ff_ptr(table,1156),0xa5,1156);memset(ff_ptr(0x800b39a0,578),0xa5,578);memset(ff_ptr(a[3],16384),0xa5,16384);
   ff_w32(0x80094844,a[3]);ff_w16(0x80093566,0);memcpy(ff_ptr(0x801d0000,16),name,16);
   r=a[0]?FUN_80060110(0x801d0000,a[1],a[2]):FUN_8001721C(0x801d0000,(sint32)a[1]);
   fwrite(&r,4,1,out);fwrite(ff_ptr(0x800c1734,59180),59180,1,out);fwrite(ff_ptr(a[3],16384),16384,1,out);
   fwrite(ff_ptr(table,1156),1156,1,out);fwrite(ff_ptr(0x800b39a0,578),578,1,out);
   if(a[0]){fwrite(ff_ptr(0x80081720+4*a[1],4),4,1,out);fwrite(ff_ptr(0x800990d8+4*a[1],4),4,1,out);}else {uint32 z[2]={0};fwrite(z,8,1,out);}
   fwrite(ff_ptr(0x80094844,4),4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-model-load")) {
  FILE *in,*out,*asset;uint8 name[16];uint32 a[3],r;
  in=fopen("../status/menu/model-load-jobs.bin","rb");out=fopen("../status/menu/model-load-native.bin","wb");if(!in||!out)return 4;
  while(fread(name,16,1,in)==1) {
   if(fread(a,12,1,in)!=1)return 4;if(!ff_load_ram("FF-menu.ram"))return 2;
   asset=fopen("assets/MODELSFE.WAD","rb");if(!asset)return 2;
   if(fread(ff_ptr(0x800c1734,59180),59180,1,asset)!=1)return 2;fclose(asset);
   ff_w16(0x80093566,0);memcpy(ff_ptr(0x801d0000,16),name,16);
   r=a[2]?FUN_800600F0(0x801d0000,a[0],a[1]):FUN_8005FF2C(0x801d0000,a[0],a[1]);
   fwrite(&r,4,1,out);fwrite(ff_ptr(0x800c1734,59180),59180,1,out);
   fwrite(ff_ptr(0x80081720+4*a[0],4),4,1,out);fwrite(ff_ptr(0x800990d8+4*a[0],4),4,1,out);
   fwrite(ff_ptr(0x80094570,4),4,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-stage-reset")) {
  FILE *out;uint32 stage,j,r;
  out=fopen("../status/menu/stage-reset-native.bin","wb");if(!out)return 4;
  for(stage=0;stage<27;stage++) {
   if(!ff_load_ram("FF-menu.ram"))return 2;
   memset(ff_ptr(0x80093d00,2816),0xa5,2816);ff_w32(0x80093d58,stage);
   memset(ff_ptr(0x8009d1d0,4608),0xa5,4608);memset(ff_ptr(0x8009a3c8,2304),0xa5,2304);
   for(j=0;j<8;j++)ff_w16(0x800b435a+244*j,(uint16)(j-4));
   r=FUN_800526F0();fwrite(&r,4,1,out);r=FUN_80018318();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x80093d00,2816),2816,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);
   fwrite(ff_ptr(0x8009d1d0,4608),4608,1,out);fwrite(ff_ptr(0x8009a3c8,2304),2304,1,out);fwrite(ff_ptr(0x8008d490,8),8,1,out);
  }
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-startup-reset")) {
  FILE *in,*out;uint32 a[4],r,j;
  in=fopen("../status/menu/startup-reset-jobs.bin","rb");out=fopen("../status/menu/startup-reset-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1) {
   if(!ff_load_ram("FF-menu.ram"))return 2;
   memset(ff_ptr(0x800adc60,4656),0xa5,4656);memset(ff_ptr(0x800ba200,4064),0xa5,4064);
   ff_w16(0x800b434c,a[0]);ff_w16(0x800b4440,a[1]);ff_w32(0x800940b0,a[2]);
   for(j=0;j<8;j++)ff_w16(0x800b435a+244*j,a[3]+j);
   r=FUN_80013A30();fwrite(&r,4,1,out);r=FUN_80013848();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x800adc60,4656),4656,1,out);fwrite(ff_ptr(0x800ba200,4064),4064,1,out);
   fwrite(ff_ptr(0x800b4318,1952),1952,1,out);fwrite(ff_ptr(0x800a8588,80),80,1,out);fwrite(ff_ptr(0x800947f0,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-scene-resources")) {
  FILE *asset,*out;uint32 r;size_t n;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  asset=fopen("assets/STAGEFE.WAD","rb");if(!asset)return 2;
  n=fread(ff_ptr(0x80116000,511180),1,511180,asset);fclose(asset);if(n!=511180)return 2;
  ff_w32(0x80094038,0x80116000);ff_w32(0x80094818,0x80116000);ff_w16(0x80093566,0);
  ff_w32(0x800947f0,0x80116000);FUN_8005E7C0();
  r=FUN_80060394(1);out=fopen("../status/menu/scene-resources-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);
  fwrite(ff_ptr(0x80094000,4096),4096,1,out);
  fwrite(ff_ptr(0x80093a58,4),4,1,out);
  fwrite(ff_ptr(0x80093e0c,4),4,1,out);
  fwrite(ff_ptr(0x800a8768,13176),13176,1,out);
  fwrite(ff_ptr(0x8009acc8,4658),4658,1,out);
  fwrite(ff_ptr(0x800af418,160),160,1,out);
  fwrite(ff_ptr(0x800b4ab8,8192),8192,1,out);
  fwrite(ff_ptr(0x8009f870,8192),8192,1,out);
  fwrite(ff_ptr(0x800e1140,16384),16384,1,out);
  fwrite(ff_ptr(0x800a1870,288),288,1,out);
  fwrite(ff_ptr(0x800a3290,16384),16384,1,out);fwrite(ff_ptr(0x800b89f0,4608),4608,1,out);fwrite(ff_ptr(0x8009ec70,3072),3072,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-texture-loader")) {
  FILE *asset,*out;uint32 r;size_t n;
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
  asset=fopen("assets/STAGEFE.WAD","rb");if(!asset)return 2;
  n=fread(ff_ptr(0x80116000,511180),1,511180,asset);fclose(asset);if(n!=511180)return 2;
  ff_w32(0x80094038,0x80116000);ff_w32(0x80094818,0x80116000);ff_w16(0x80093566,0);
  memcpy(ff_ptr(0x8009f870,8192),ff_ptr(0x80116000+66556,8192),8192);
  r=FUN_80060394(1);out=fopen("../status/menu/texture-loader-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);
  fwrite(ff_ptr(0x80094000,4096),4096,1,out);
  fwrite(ff_ptr(0x80093a58,4),4,1,out);
  fwrite(ff_ptr(0x80093e0c,4),4,1,out);
  fwrite(ff_ptr(0x800a8768,13176),13176,1,out);
  fwrite(ff_ptr(0x8009acc8,4658),4658,1,out);
  fwrite(ff_ptr(0x800af418,160),160,1,out);
  fwrite(ff_ptr(0x800b4ab8,8192),8192,1,out);
  fwrite(ff_ptr(0x8009f870,8192),8192,1,out);
  fwrite(ff_ptr(0x800e1140,16384),16384,1,out);
  fwrite(ff_ptr(0x800a1870,288),288,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-terrain-loader")) {
  FILE *asset,*out;uint32 r;size_t n;
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
  asset=fopen("assets/STAGEFE.WAD","rb");if(!asset)return 2;
  n=fread(ff_ptr(0x80116000,511180),1,511180,asset);fclose(asset);if(n!=511180)return 2;
  ff_w32(0x80094038,0x80116000);ff_w32(0x80094818,0x80116000);ff_w16(0x80093566,0);
  memcpy(ff_ptr(0x801d0000,10),"STGFE.PBL",10);memcpy(ff_ptr(0x801d0020,10),"STGFE.MPD",10);
  ff_w32(0x800947f0,0x80116000);r=FUN_80063900(0x801d0000,0x801d0020);
  out=fopen("../status/menu/terrain-loader-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4cb8,512),512,1,out);
  fwrite(ff_ptr(0x800947f0,4),4,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
  fwrite(ff_ptr(ff_u32(0x800947f0),16384),16384,1,out);
  fwrite(ff_ptr(0x8009f870,8192),8192,1,out);fwrite(ff_ptr(0x800a3290,16384),16384,1,out);
  fwrite(ff_ptr(0x800b89f0,4608),4608,1,out);fwrite(ff_ptr(0x8009ec70,3072),3072,1,out);fwrite(ff_ptr(0x80094000,2048),2048,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-terrain-textures")) {
  FILE *asset,*out;uint32 r;size_t n;
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
  asset=fopen("assets/STAGEFE.WAD","rb");if(!asset)return 2;
  n=fread(ff_ptr(0x80116000,511180),1,511180,asset);fclose(asset);if(n!=511180)return 2;
  ff_w32(0x80094038,0x80116000);ff_w32(0x80094818,0x80116000);ff_w16(0x80093566,0);
  memcpy(ff_ptr(0x801d0000,10),"STGFE.PBL",10);memcpy(ff_ptr(0x801d0020,10),"STGFE.PPL",10);
  ff_w32(0x800947f0,0x80116000);r=ff_terrain_textures_80063900(0x801d0000);
  out=fopen("../status/menu/terrain-textures-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4cb8,512),512,1,out);
  fwrite(ff_ptr(0x800947f0,4),4,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
  fwrite(ff_ptr(ff_u32(0x800947f0),16384),16384,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&!strcmp(argv[1],"--audit-background")) {
  FILE *asset,*out;uint32 r;size_t n;
  if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
  asset=fopen("assets/STAGEFE.WAD","rb");if(!asset)return 2;
  n=fread(ff_ptr(0x80116000,511180),1,511180,asset);fclose(asset);if(n!=511180)return 2;
  ff_w32(0x80094038,0x80116000);ff_w32(0x80094818,0x80116000);ff_w16(0x80093566,0);
  memcpy(ff_ptr(0x801d0000,10),"STGFE.PDT",10);memcpy(ff_ptr(0x801d0020,10),"STGFE.PPL",10);
  r=FUN_80067108(0x801d0000,0x801d0020);
  out=fopen("../status/menu/background-native.bin","wb");if(!out)return 4;
  fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4ab8,512),512,1,out);fwrite(ff_ptr(0x8008d4c8,8),8,1,out);
  fwrite(ff_ptr(0x80093b9c,8),8,1,out);fwrite(ff_ptr(0x80093e0c,4),4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&(!strcmp(argv[1],"--audit-resource-find")||!strcmp(argv[1],"--audit-stage-resource-find"))) {
  FILE *in,*out;uint8 name[16];uint32 r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  if(!strcmp(argv[1],"--audit-stage-resource-find")) {
   FILE *asset=fopen("assets/STAGEFE.WAD","rb");size_t n;if(!asset)return 2;
   n=fread(ff_ptr(0x80116000,511180),1,511180,asset);fclose(asset);if(n!=511180)return 2;
   ff_w32(0x80094038,0x80116000);ff_w32(0x80094818,0x80116000);
  }
  in=fopen("../status/menu/resource-find-jobs.bin","rb");out=fopen("../status/menu/resource-find-native.bin","wb");if(!in||!out)return 4;
  while(fread(name,16,1,in)==1) {
   memcpy(ff_ptr(0x801d0000,16),name,16);ff_w32(0x80094570,0xa5a5a5a5);
   r=ff_resource_find_80061384(0x801d0000);fwrite(&r,4,1,out);fwrite(ff_ptr(0x80094570,4),4,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-actor-init")==0) {
  FILE *in,*out;uint32 a[8],r,i;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/actor-init-jobs.bin","rb");out=fopen("../status/menu/actor-init-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,32,1,in)==1) {
   memset(ff_ptr(0x800b4318,1952),0xa5,1952);memset(ff_ptr(0x80093d00,1280),0xa5,1280);
   for(i=0;i<8;i++)ff_w16(0x800b434c+244*i,(uint16)a[i]);
   r=FUN_80014498();fwrite(&r,4,1,out);fwrite(ff_ptr(0x800b4318,1952),1952,1,out);fwrite(ff_ptr(0x80093d00,1280),1280,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-overlay")==0) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/overlay-jobs.bin","rb");out=fopen("../status/menu/overlay-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1) {
   ff_w32(0x8008d4b0,0x801d0000);ff_w32(0x8008d4b4,0x801c0000);
   memset(ff_ptr(0x801d0000,64),0xa5,64);ff_w32(0x801c1ffc,0x12abcdef);
   ff_w16(0x8001000c,(uint16)a[1]);ff_w16(0x8001000e,(uint16)a[2]);ff_w16(0x80010010,(uint16)a[3]);ff_w32(0x80094190,a[1]);
   r=a[0]?FUN_80064820():FUN_8006471C();fwrite(&r,4,1,out);
   fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x801c1ffc,4),4,1,out);fwrite(ff_ptr(0x801d0000,64),64,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-display-offsets")==0) {
  FILE *in,*out;uint32 a[4];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/display-offsets-jobs.bin","rb");out=fopen("../status/menu/display-offsets-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1){ff_w32(0x800941a8,a[0]);ff_w32(0x8008d4c4,a[1]);ff_w16(0x80093d38,(uint16)a[2]);ff_w16(0x80093d3a,(uint16)a[3]);memset(ff_ptr(0x800947a0,40),0xa5,40);ff_display_offsets_800586D0();fwrite(ff_ptr(0x800947a0,40),40,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-frame-buffers")==0) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/frame-buffers-jobs.bin","rb");out=fopen("../status/menu/frame-buffers-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1){ff_w32(0x8008d4c4,a[0]);r=(uint32)FUN_80064684();fwrite(&r,4,1,out);fwrite(ff_ptr(0x8008d4b0,12),12,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-menu-stage")==0) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/menu-stage-jobs.bin","rb");out=fopen("../status/menu/menu-stage-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1){ff_w32(0x80093dfc,a[0]);r=(uint32)ff_menu_stage_8001E168();fwrite(&r,4,1,out);fwrite(ff_ptr(0x80093dfc,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-actor-direction")==0) {
  FILE *in,*out;uint32 a[4],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/actor-direction-jobs.bin","rb");out=fopen("../status/menu/actor-direction-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,16,1,in)==1){r=(uint32)FUN_8001627C((sint32)a[0],(sint32)a[1],(sint32)a[2],(sint32)a[3]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x800941a0,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-actor-math")==0) {
  FILE *in,*out;uint32 a[5],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/actor-math-jobs.bin","rb");out=fopen("../status/menu/actor-math-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,20,1,in)==1){ff_w32(0x1d000c,a[1]);r=(uint32)(a[0]==0?FUN_8002371C(0x1d0000):a[0]==1?FUN_800164CC((sint32)a[1]):FUN_80016478((sint32)a[1],(sint32)a[2],(sint32)a[3],(sint32)a[4]));fwrite(&r,4,1,out);fwrite(ff_ptr(0x1d000c,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-animation-decode")==0) {
  FILE *in,*out;uint32 bones;uint8 actor[244],stream[1024],pose[504];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/animation-decode-jobs.bin","rb");out=fopen("../status/menu/animation-decode-native.bin","wb");if(!in||!out)return 4;
  while(fread(&bones,4,1,in)==1){if(fread(actor,244,1,in)!=1||fread(stream,1024,1,in)!=1||fread(pose,504,1,in)!=1)return 5;ff_w32(0x1c9014,bones);memcpy(ff_ptr(0x1d0000,244),actor,244);memcpy(ff_ptr(0x1ca000,1024),stream,1024);memcpy(ff_ptr(0x1c8000,504),pose,504);FUN_80011A50(0x1c9000,0x1d0000,0x1c8000);fwrite(ff_ptr(0x1d0000,244),244,1,out);fwrite(ff_ptr(0x1c8000,504),504,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-animation-start")==0) {
  FILE *in,*out;uint32 a[6],r,actor;uint8 state[244],stream[128],history[32];int i;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/animation-start-jobs.bin","rb");out=fopen("../status/menu/animation-start-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,24,1,in)==1){if(fread(state,244,1,in)!=1||fread(stream,128,1,in)!=1||fread(history,32,1,in)!=1)return 5;actor=0x800b4318+244*a[1];memcpy(ff_ptr(actor,244),state,244);memcpy(ff_ptr(0x1ca000,128),stream,128);memcpy(ff_ptr(0x80094640,32),history,32);for(i=0;i<2;i++){*(uint8*)ff_ptr(0x80094374+i,1)=7;*(uint8*)ff_ptr(0x80094414+i,1)=0xa5;}ff_w32(0x8008e4ec,0xa5a5a5a5);ff_w32(0x80093d58,a[5]);ff_w32(0x1c8000+4*a[2],a[4]);ff_w16(0x800b39a0+2*a[2],(uint16)a[3]);r=a[0]?(uint32)FUN_80019F00(actor,0x1c8000,(sint32)a[2]):(uint32)FUN_80052758((sint32)a[1],(sint32)a[2]);fwrite(&r,4,1,out);fwrite(ff_ptr(actor,244),244,1,out);fwrite(ff_ptr(0x80094640,32),32,1,out);fwrite(ff_ptr(0x80094374,2),2,1,out);fwrite(ff_ptr(0x80094414,2),2,1,out);fwrite(ff_ptr(0x8008e4ec,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-animation-base")==0) {
  FILE *in,*out;uint32 a[3],r;uint8 actor[244],stream[128];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/animation-base-jobs.bin","rb");out=fopen("../status/menu/animation-base-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,12,1,in)==1){if(fread(actor,244,1,in)!=1||fread(stream,128,1,in)!=1)return 5;memcpy(ff_ptr(a[1],244),actor,244);memcpy(ff_ptr(0x1ca000,128),stream,128);ff_w32(0x80093d58,a[2]);if(a[0])r=(uint32)FUN_8001AE7C(a[1]);else {FUN_80011A04(a[1]);r=0;}fwrite(&r,4,1,out);fwrite(ff_ptr(a[1],244),244,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-terrain-scene")==0) {
  FILE *in,*out;uint32 a[8],r;int i;
  const uint32 ranges[][2]={{0x8008d4b0,16},{0x1cb000,8192},{0x1fe800,1160},{0x1f800000,1024},{0x1d0000,65536},{0x800a1990,6400},{0x8008d494,4}};
  if(!ff_load_ram("FF-menu.ram"))return 2;InitGeom();
  in=fopen("../status/menu/terrain-scene-jobs.bin","rb");out=fopen("../status/menu/terrain-scene-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,32,1,in)==1){for(i=0;i<7;i++)memset(ff_ptr(ranges[i][0],ranges[i][1]),0xa5,ranges[i][1]);memcpy(ff_ptr(0x8008d4a4,12),a,12);ff_w16(0x8008d490,(uint16)a[3]);ff_w16(0x8008d492,(uint16)a[4]);ff_w32(0x8008d4c4,a[5]);ff_w32(0x80093d58,a[6]);ff_w32(0x8008d4d0,a[7]);ff_w32(0x8008d4b0,0x1d0000);ff_w32(0x8008d4b4,0x1cb000);ff_w32(0x8008d4bc,0x2d000000);FUN_80011D50(0x1cb000);r=(uint32)FUN_80063EEC();fwrite(&r,4,1,out);for(i=0;i<7;i++)fwrite(ff_ptr(ranges[i][0],ranges[i][1]),ranges[i][1],1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-distant-texture")==0) {
  FILE *in,*out;uint32 a[5],r;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/distant-texture-jobs.bin","rb");out=fopen("../status/menu/distant-texture-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,20,1,in)==1){ff_w32(0x8008d4a4,a[0]);ff_w32(0x8008d4a8,a[1]);ff_w32(0x8008d4ac,a[2]);ff_w16(0x8008d490,(uint16)a[3]);ff_w32(0x8008d4c4,a[4]);memset(ff_ptr(0x800a1990,6400),0xa5,6400);r=FUN_80064150();fwrite(&r,4,1,out);fwrite(ff_ptr(0x800a1990,6400),6400,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-distant-quad")==0) {
  FILE *in,*out;uint32 a[11],r,desc,bucket;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/distant-quad-jobs.bin","rb");out=fopen("../status/menu/distant-quad-native.bin","wb");if(!in||!out)return 4;
  while(fread(a,44,1,in)==1){memcpy(ff_ptr(0x1cf000,16),a,16);desc=0x800a8768+a[4]*12;memcpy(ff_ptr(desc,12),a+5,12);memset(ff_ptr(0x1d0000,48),0xa5,48);ff_w32(0x8008d4b0,a[9]);ff_w32(0x8008d4bc,a[8]);ff_w32(0x8008d4b4,0x1cc000);ff_w32(0x8008d4d8,0x1ce400);ff_w16(0x1ce400+((uint32)((sint32)a[2]>>2)<<1),(uint16)a[10]);bucket=0x1cc000+(sint16)a[10];ff_w32(bucket,0x81001234);r=FUN_800128F4(0x1cf000,(sint32)a[4]);fwrite(&r,4,1,out);fwrite(ff_ptr(0x8008d4b0,4),4,1,out);fwrite(ff_ptr(0x8008d4bc,4),4,1,out);fwrite(ff_ptr(bucket,4),4,1,out);fwrite(ff_ptr(0x1d0000,48),48,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-terrain-grid")==0) {
  FILE *in,*out;uint32 args[3],result;uint8 matrix[32],tiles[8192];
  const uint32 ranges[][2]={{0x8008d4b0,16},{0x1cb000,8},{0x1fe800,1160},{0x1f800000,580},{0x1d0000,10240}};int i;
  if(!ff_load_ram("FF-menu.ram"))return 2;InitGeom();
  in=fopen("../status/menu/terrain-grid-jobs.bin","rb");out=fopen("../status/menu/terrain-grid-native.bin","wb");if(!in||!out)return 4;
  while(fread(args,12,1,in)==1){if(fread(matrix,32,1,in)!=1||fread(tiles,8192,1,in)!=1)return 5;memcpy(ff_ptr(0x1f8003c0,32),matrix,32);memcpy(ff_ptr(0x8009f870,8192),tiles,8192);for(i=0;i<5;i++)memset(ff_ptr(ranges[i][0],ranges[i][1]),0xa5,ranges[i][1]);ff_w32(0x8008d4b0,0x1d0000);ff_w32(0x8008d4b4,0x1cb000);ff_w32(0x8008d4bc,args[2]);ff_w32(0x1cb004,0x10018);result=FUN_8001100C(0,(sint32)args[0],(sint32)args[1]);fwrite(&result,4,1,out);for(i=0;i<5;i++)fwrite(ff_ptr(ranges[i][0],ranges[i][1]),ranges[i][1],1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-camera-matrix")==0) {
  FILE *in,*out;uint32 args[3];uint8 state[52];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/camera-matrix-jobs.bin","rb");out=fopen("../status/menu/camera-matrix-native.bin","wb");if(!in||!out)return 4;
  while(fread(args,12,1,in)==1){if(fread(state,52,1,in)!=1)return 5;memcpy(ff_ptr(0x1f8003c0,52),state,52);ff_w32(0x8008d494,args[0]==2?0x1f800380:0x1f8003c0);ff_w16(0x8008d490,(uint16)args[1]);ff_w16(0x8008d492,(uint16)args[2]);if(args[0]==0)FUN_800101CC((sint32)args[1]);else if(args[0]==1)FUN_8001033C((sint32)args[1]);else FUN_80011C60();fwrite(ff_ptr(0x1f8003c0,52),52,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-matrix-stack")==0) {
  FILE *in,*out;uint32 args[5],result;uint8 state[96];MATRIX m;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/matrix-stack-jobs.bin","rb");out=fopen("../status/menu/matrix-stack-native.bin","wb");if(!in||!out)return 4;
  while(fread(args,20,1,in)==1){if(fread(state,96,1,in)!=1)return 5;InitGeom();memcpy(ff_ptr(args[1]-32,96),state,96);ff_w32(0x8008d494,args[1]);result=args[0]==0?FUN_80010028():args[0]==1?FUN_80010040():FUN_800100C4((sint32)args[2],(sint32)args[3],(sint32)args[4]);fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008d494,4),4,1,out);fwrite(ff_ptr(args[1]-32,96),96,1,out);ReadRotMatrix(&m);fwrite(m.m,18,1,out);fwrite(m.t,12,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-ordering")==0) {
  FILE *in,*out;uint32 args[2],result;uint8 state[8192];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/ordering-jobs.bin","rb");out=fopen("../status/menu/ordering-native.bin","wb");if(!in||!out)return 4;
  while(fread(args,8,1,in)==1){if(fread(state,8192,1,in)!=1)return 5;memcpy(ff_ptr(0x1cb000,8192),state,8192);ff_w32(0x8008d4b4,args[1]);result=args[0]?FUN_80011D9C():FUN_80011D50(args[1]);fwrite(&result,4,1,out);fwrite(ff_ptr(0x1cb000,8192),8192,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-horizon")==0) {
  FILE *in,*out;sint32 pitch;uint8 state[52];
  if(!ff_load_ram("FF-menu.ram"))return 2;InitGeom();memset(state,0xa5,52);
  in=fopen("../status/menu/horizon-jobs.bin","rb");out=fopen("../status/menu/horizon-native.bin","wb");if(!in||!out)return 4;
  while(fread(&pitch,4,1,in)==1){ff_w16(0x8008d492,(uint16)pitch);memcpy(ff_ptr(0x1f8003c0,52),state,52);ff_menu_horizon_80058844();fwrite(ff_ptr(0x1f8003c0,52),52,1,out);fwrite(ff_ptr(0x8008d4d0,4),4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-projection")==0) {
  FILE *in,*out;uint32 args[6],result;uint8 state[32];
  if(!ff_load_ram("FF-menu.ram"))return 2;InitGeom();
  in=fopen("../status/menu/projection-jobs.bin","rb");out=fopen("../status/menu/projection-native.bin","wb");if(!in||!out)return 4;
  while(fread(args,24,1,in)==1){if(fread(state,32,1,in)!=1)return 5;memcpy(ff_ptr(0x1f8003c0,32),state,32);SetGeomOffset((sint32)args[3],(sint32)args[4]);SetGeomScreen((sint32)args[5]);result=FUN_800120BC((sint32)args[0],(sint32)args[1],(sint32)args[2],0x801fc000);fwrite(ff_ptr(0x801fc000,16),16,1,out);fwrite(&result,4,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-rotation")==0) {
  FILE *in,*out;uint32 angle;uint8 state[52];
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/rotation-jobs.bin","rb");out=fopen("../status/menu/rotation-native.bin","wb");if(!in||!out)return 4;
  while(fread(&angle,4,1,in)==1){if(fread(state,52,1,in)!=1)return 5;memcpy(ff_ptr(0x1f8003c0,52),state,52);if(angle==0x80000000)FUN_80011D20();else FUN_800101CC((sint32)angle);fwrite(ff_ptr(0x1f8003c0,52),52,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-tail")==0) {
  static uint8 fixture[0x200000];const uint32 ranges[][2]={{0x8001000c,6},{0x8009272c,260},{0x80093a18,4992},{0x800b4318,488}};
  FILE *in,*out;uint32 skip,result;int i;
  if(!ff_load_ram("FF-menu.ram"))return 2;memcpy(fixture,ff_ram,sizeof(fixture));
  in=fopen("../status/menu/tail-jobs.bin","rb");out=fopen("../status/menu/tail-native.bin","wb");if(!in||!out)return 4;
  while(fread(&skip,4,1,in)==1){memcpy(ff_ram,fixture,sizeof(fixture));for(i=0;i<4;i++)if(fread(ff_ptr(ranges[i][0],ranges[i][1]),ranges[i][1],1,in)!=1)return 5;
   result=ff_menu_tail_80050090((sint32)skip);fwrite(&result,4,1,out);for(i=0;i<4;i++)fwrite(ff_ptr(ranges[i][0],ranges[i][1]),ranges[i][1],1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--menu")==0)return ff_menu_runtime(NULL,argc>2?(uint32)strtoul(argv[2],0,0):0,0);
 if(argc>2&&strcmp(argv[1],"--menu-script")==0)return ff_menu_runtime(argv[2],0,argc>3&&strcmp(argv[3],"--headless")==0);
 if(argc>1&&strcmp(argv[1],"--audit-camera")==0) {
  FILE *in,*out;uint8 state[82];sint32 result;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/camera-jobs.bin","rb");out=fopen("../status/menu/camera-native.bin","wb");if(!in||!out)return 4;
  while(fread(state,sizeof(state),1,in)==1) {
   memcpy(ff_ptr(0x8008d490,32),state,32);memcpy(ff_ptr(0x800927b8,50),state+32,50);
   result=FUN_8004F520();fwrite(&result,4,1,out);fwrite(ff_ptr(0x8008d490,32),32,1,out);fwrite(ff_ptr(0x800927b8,50),50,1,out);
  }
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-input")==0)return ff_audit_input();
 if(argc>1&&strcmp(argv[1],"--audit-preframe")==0)return ff_audit_preframe();
 if(argc>1&&strcmp(argv[1],"--audit-scores")==0)return ff_audit_scores();
 if(argc>1&&strcmp(argv[1],"--audit-controller")==0)return ff_audit_controller();
 if(argc>1&&strcmp(argv[1],"--audit-screen")==0)return ff_audit_screen();
 if(argc>1&&strcmp(argv[1],"--audit-options")==0)return ff_audit_options();
 if(argc>1&&strcmp(argv[1],"--audit-menu")==0)return ff_audit_menu();
 if(argc>1&&strcmp(argv[1],"--audit-character")==0)return ff_audit_character();
 if(argc>1&&strcmp(argv[1],"--audit-sound")==0)return ff_audit_sound();
 if(argc>1&&strcmp(argv[1],"--sound-preview")==0) {
  FILE *out;int i;static sint16 samples[44100*2];
  if(!ff_load_ram("FF-menu.ram")||!ff_audio_init())return 2;
  out=fopen("../status/menu/menu-effects.pcm","wb");if(!out)return 4;
  for(i=0;i<2;i++){if(i)FUN_8004F490();else FUN_8004F468();ff_audio_render(samples,44100);fwrite(samples,sizeof(samples),1,out);}
  fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--sound-output")==0) {
  FILE *f;if(!ff_load_ram("FF-menu.ram")||!ff_audio_init()||!waveout_init())return 2;
  FUN_8004F468();Sleep(1000);FUN_8004F490();Sleep(1000);waveout_shutdown();
  f=fopen("../status/menu/sound-output-counters.txt","w");if(!f)return 4;
  fprintf(f,"buffers %u nonzero %u peak %u overruns %u\n",g_waveout_submitted_buffers,g_waveout_nonzero_buffers,g_waveout_peak,g_waveout_callback_overruns);fclose(f);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-pitch")==0) {
  FILE *in,*out;uint32 args[4];uint16 value;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  in=fopen("../status/menu/pitch-jobs.bin","rb");out=fopen("../status/menu/pitch-native.bin","wb");if(!in||!out)return 4;
  while(fread(args,sizeof(args),1,in)==1){value=ff_note_to_pitch(args[0],(uint16)args[1],args[2],(uint16)args[3]);fwrite(&value,2,1,out);}
  fclose(in);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-packets")==0) {
  FILE *jobs,*out;unsigned int function,arg;int x,y,z;uint32 result,used,head;const uint32 arena=0x801d0000;
  if(!ff_load_ram("FF-menu.ram"))return 2;
  jobs=fopen("../status/menu/packet-jobs.txt","r");out=fopen("../status/menu/packets-native.bin","wb");
  if(!jobs||!out)return 4;
  while(fscanf(jobs,"%x %x %d %d %d",&function,&arg,&x,&y,&z)==5) {
   sint32 position[3]={x,y,z};
   memset(ff_ptr(arena,65536),0xa5,65536);ff_w32(0x8008d4b0,arena);
   ff_w32(0x8008d4b4,0x801cb000);ff_w32(0x8008d4d8,0x801ce000);
   ff_w16(0x801ce000+(function==0x80012390?(uint32)(z>>2)*2:0),0);
   ff_w16(0x801ce008,0);
   ff_w32(0x801cb000,0x00ffffff);ff_w32(0x801ccffc,0x00ffffff);
   if(function==0x80012390)result=FUN_80012390(position,(sint32)arg);
   else if(function==0x80061b7c)result=(uint32)FUN_80061B7C(arg,x,y);
   else if(function==0x80061adc)result=(uint32)FUN_80061ADC(arg,y);
   else if(function==0x80062c0c)result=(uint32)FUN_80062C0C(arg,x,y,(uint32)z);
   else if(function==0x80061c68)result=FUN_80061C68(x,y);
   else if(function==0x80062ab4)result=FUN_80062AB4(x,y,(sint32)arg);
   else if(function==0x8006686c){ff_w32(0x8008d4a4,(uint32)x);ff_w32(0x8008d4d0,(uint32)y);ff_w32(0x8008d4c8,arg);ff_w16(0x8008d490,(uint16)z);result=FUN_8006686C();}
   else return 5;
   used=ff_u32(0x8008d4b0)-arena;head=ff_u32((function==0x80062c0c||function==0x80062ab4)?0x801ccffc:0x801cb000);
   if(used>65536)return 6;
   fwrite(&result,4,1,out);fwrite(&used,4,1,out);fwrite(&head,4,1,out);fwrite(ff_ptr(arena,used),1,used,out);
  }
  fclose(jobs);fclose(out);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--audit-text")==0) {
  int i;if(!freopen("../status/menu/text-native-results.txt","w",stdout))return 4;
  if(!ff_load_ram(argc>2?argv[2]:"FF-menu.ram"))return 2;
  for(i=-128;i<256;i++)printf("glyph %d %d\n",i,FUN_80018670(i));
  for(i=3;i<argc;i++){uint32 a=(uint32)strtoul(argv[i],0,16);printf("width %08X %d\n",a,FUN_8005EFB8(a));}fflush(stdout);ff_audit_end();return 0;
 }
 if(argc>1&&strcmp(argv[1],"--platform-smoke")==0){limit=argc>2?(uint32)strtoul(argv[2],0,0):120;headless=argc>3&&strcmp(argv[3],"--headless")==0;}
 else if(argc>1&&strcmp(argv[1],"--text-preview")==0){preview=1;limit=argc>2?(uint32)strtoul(argv[2],0,0):120;headless=argc>3&&strcmp(argv[3],"--headless")==0;if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;}
 else {fprintf(stderr,"Menu translation is WIP. Use --platform-smoke to test the imported host layer.\n");return 3;}
 memset(&config,0,sizeof(config));config.window_title="Fighting Force - platform test";
 config.window_width=960;config.window_height=720;config.refresh_rate=60;config.headless=headless;
 psx_configure(&config);ResetGraph(0);
 while(!psx_quit_requested()&&frame<limit){POLY_G3 p;
  ff_gpu_begin();if(preview){if(!text_preview_frame())return 7;}else{memset(&p,0,sizeof(p));setPolyG3(&p);
  p.r0=255;p.g1=255;p.b2=255;p.x0=160;p.y0=20;p.x1=30;p.y1=210;p.x2=290;p.y2=210;
  ff_gpu_packet(&p);}ff_gpu_present();VSync(0);++frame;
 }
 printf("platform_frames %u pad %08X\n",frame,PadRead(0));
 if(headless&&!ff_gpu_save_frame(preview?"../status/menu/text-frame.bgrx":"../status/menu/platform-frame.bgrx"))return 8;
 return 0;
}
