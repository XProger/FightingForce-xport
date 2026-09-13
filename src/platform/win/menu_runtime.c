/* Native integration of audited callbacks. WIP: fixture bootstrap and missing
 * outer stage/scene tail. Unsupported callbacks stop explicitly. */
#include "ff.h"
#include "ff_gpu.h"
#include "ff_audio.h"
#include "audio_waveout.h"
#include "platform_dummy.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
static jmp_buf menu_exit;
#include "diagnostic_state.h"
static uint32 diag_segments[128][3],diag_count,diag_active,diag_title,diag_enabled;
static uint16 diag_pad=65535;
static LONGLONG diag_gpu_ticks;
static void diagnostic_input(uint32 event) {
 uint32 i,tick=ff_u32(0x80093dd0);
 if(event==3)diag_title=0;
 if(event==2&&diag_title)ff_w16(0x8009474a,tick==2?0xfff7:0xffff);
 if(event==1) {
  diag_active=1;diag_pad=65535;
  for(i=0;i<diag_count;i++)if(tick>=diag_segments[i][0]&&tick<=diag_segments[i][1])diag_pad=(uint16)~diag_segments[i][2];
 }
 if(diag_active&&(event==0||event==1))ff_w16(0x8009474a,diag_pad);
}
static int diagnostic_init(void) {
 const char *path=getenv("FF_AUDIT_PAD_SCHEDULE");FILE *f;uint32 i;
 diag_enabled=0;ff_services.audit_input_event=NULL;if(!path)return 1;
 f=fopen(path,"rb");if(!f)return 0;
 diag_count=(uint32)fread(diag_segments,1,sizeof(diag_segments),f);
 if(diag_count%12||fgetc(f)!=EOF||ferror(f)){fclose(f);return 0;}fclose(f);diag_count/=12;
 for(i=0;i<diag_count;i++)if(diag_segments[i][0]>diag_segments[i][1]||diag_segments[i][1]>=900||diag_segments[i][2]>65535)return 0;
 diag_enabled=1;diag_title=1;diag_active=0;diag_pad=65535;ff_services.audit_input_event=diagnostic_input;return 1;
}
static int diagnostic_checkpoint(const char *path,int load) {
 FILE *f;int ok;uint32 magic=0x31434646;char build[64]=__DATE__ " " __TIME__,actual[64];
 /* Stop the output worker before snapshotting RAM and SPU together. */
 waveout_shutdown();f=fopen(path,load?"rb":"wb");if(!f)return 0;
 if(load){ok=fread(&magic,4,1,f)==1&&magic==0x31434646&&fread(actual,1,64,f)==64&&!memcmp(build,actual,64);}
 else ok=fwrite(&magic,4,1,f)==1&&fwrite(build,1,64,f)==64;
 ok=ok&&ff_state_block(f,ff_ram,sizeof(ff_ram),load)&&ff_state_block(f,ff_ptr(0x1f800000,1024),1024,load)
  &&psx_state_io(f,load)&&ff_gpu_state_io(f,load)&&ff_audio_state_io(f,load)&&FF_STATE(f,ff_dummy_calls,load)
  &&FF_STATE(f,ff_dummy_card_pending,load)&&FF_STATE(f,ff_sdk_interrupts_enabled,load)&&FF_STATE(f,ff_host_pad_state,load)&&FF_STATE(f,ff_sdk_counter_state,load);
 if(load&&fgetc(f)!=EOF)ok=0;
 if(fclose(f))ok=0;
 if(!waveout_init())ok=0;
 return ok;
}
static const char *diagnostic_output(void) {
 const char *p=diag_enabled?getenv("FF_AUDIT_OUTPUT"):NULL;
 return p?p:"../status/gameplay/frontend-game-loop-final.ram";
}

/* Physical CD/card services remain dummy; route game-side menu logic through
 * the translated entries so their markers and UI branches are retained. */
static sint32 menu_aux(uint32 function,sint32 a,sint32 b) {
 (void)b;
 if(function==0x800569bc)return FUN_800569BC(a);
 if(function==0x80052428)return FUN_80052428();
 if(function==0x80052458)return FUN_80052458();
 if(function==0x80052484)return FUN_80052484();
 fprintf(stderr,"WIP: untranslated menu service %08X\n",function);longjmp(menu_exit,1);
}
static void exit_menu(uint32 buffer,sint32 value) {
 fprintf(stderr,"WIP: original longjmp %08X (%d), startup continuation pending.\n",buffer,value);
 longjmp(menu_exit,1);
}
extern void ff_audit_end(void);
/* Native equivalent of asynchronous PAD refresh in the original busy wait. */
GDB_CALL void ff_pause_refresh_pad(void){
 uint32 pad=PadRead(0),buttons=((pad&255)<<8)|((pad>>8)&255);
 if(psx_quit_requested())longjmp(menu_exit,1);
 ff_host_pad_publish(1,buttons,0);
}
static void title_host_frame(void) {
 uint32 i,pad=PadRead(0),buttons=((pad&255)<<8)|((pad>>8)&255);
 if(psx_quit_requested())longjmp(menu_exit,1);
 ff_host_pad_publish(1,buttons,0);
 VSync(0);
}
static uint32 prepare_host_frame(int gameplay) {
 LARGE_INTEGER gpu_begin,gpu_end;
 uint32 i,pad=PadRead(0),buttons=((pad&255)<<8)|((pad>>8)&255),parity,display;
 if(psx_quit_requested())longjmp(menu_exit,1);
 ff_host_pad_publish(1,buttons,0);
 if(!ff_gpu_load_clut(0x800b50b8,0,499)||!ff_gpu_load_image(0x80093568,0x800a1990))longjmp(menu_exit,1);
 FUN_80067820();
 FUN_80011D9C();VSync(2);parity=ff_u32(0x8008d4c4)&1;display=0x800947a0+20*parity;
 FUN_80064684();ff_display_offsets_800586D0();ff_gpu_begin();
 ff_gpu_draw_env(0x800b8938+92*parity,320*parity,0);
 if(diag_enabled)QueryPerformanceCounter(&gpu_begin);
 if(ff_gpu_ot(ff_u32(0x8008d4b8))<0)longjmp(menu_exit,1);
 ff_gpu_display_offset(ff_s16(display+8),ff_s16(display+10));ff_gpu_present();FUN_80011D50(ff_u32(0x8008d4b4));
 if(diag_enabled){QueryPerformanceCounter(&gpu_end);diag_gpu_ticks+=gpu_end.QuadPart-gpu_begin.QuadPart;}
 if(gameplay==1)return ff_prepare_scene_800587A8_stage0();
 if((sint32)ff_u32(0x80093dd4)>=192)FUN_80061ADC(0x80093570,110);
 if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
 if(ff_u32(0x80094190))FUN_80064820();ff_menu_horizon_80058844();
 ff_draw_stage_background();
 if((sint32)ff_u32(0x80093dd0)<33) {
  sint32 fade=(sint32)((ff_u32(0x80093dd0)<<3)-256u);FUN_80011CCC(fade,fade,fade);
 }
 return 0;
}
static void sequence_prepare_frame(void) { prepare_host_frame(0); }
GDB_CALL uint32 ff_game_prepare_80058634_stage0(void) { return prepare_host_frame(1); }
GDB_CALL uint32 ff_menu_prepare_80058634_stage26(void) { return prepare_host_frame(2); }
int ff_game_prefix_preview(void) {
 PSX_CONFIG config;FILE *f;int result=9;
 if(!ff_load_ram("../status/gameplay/sequence-level-entry.ram"))return 2;
 f=fopen("../status/gameplay/sequence-level-entry.scratch","rb");if(!f)return 3;
 if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
 ff_gpu_init_empty();ff_audio_init_empty();
 memset(&config,0,sizeof(config));config.window_title="Fighting Force gameplay audit";
 config.window_width=960;config.window_height=720;config.refresh_rate=60;config.headless=1;
 psx_configure(&config);ResetGraph(0);InitGeom();if(!waveout_init())return 5;
 ff_services.game_longjmp=exit_menu;ff_services.menu_aux=menu_aux;ff_services.frontend_frame=title_host_frame;
 if(!setjmp(menu_exit)) {
  ff_level_init_80014A34_stage0();ff_level_ready_80014B70_stage0();
  ff_game_frame_prefix_80014DF8_stage0();result=0;
 }
 ff_services.frontend_frame=NULL;waveout_shutdown();
 printf("game_prefix_result %d tick %u\n",result,ff_u32(0x80093dd0));ff_audit_end();return result;
}
/* Native continuation, no RAM/VRAM/SPU fixture loads or host reset. */
static int save_actor_boundary(FILE *f) {
 return fwrite(ff_ptr(0x80093dd0,4),1,4,f)==4&&fwrite(ff_ptr(0x800b4318,1952),1,1952,f)==1952&&fwrite(ff_ptr(0x8008d490,32),1,32,f)==32;
}
/* Evidence stream: tick,scene count,world pools/script/RNG, then OT payloads.
 * Skip empty links and addresses, retain every GPU command/payload word. */
static int save_world_boundary(FILE *f) {
 uint32 count=ff_u32(0x8009403c),p=ff_u32(0x8008d4b4)&0xffffff,tag,n,guard=0,zero=0;
 if(fwrite(ff_ptr(0x80093dd0,4),4,1,f)!=1||fwrite(&count,4,1,f)!=1)return 0;
 if(fwrite(ff_ptr(0x8009a3c8,2304),1,2304,f)!=2304||fwrite(ff_ptr(0x8009d1d0,4608),1,4608,f)!=4608)return 0;
 if(fwrite(ff_ptr(0x800b89f0,36*count),36,count,f)!=count||fwrite(ff_ptr(0x800bcde0,6144),1,6144,f)!=6144)return 0;
 if(fwrite(ff_ptr(0x800946b8,4),4,1,f)!=1||fwrite(ff_ptr(0x80094e88,4),4,1,f)!=1)return 0;
 while(p!=0x10018) {
  if(++guard>100000)return 0;tag=ff_u32(p);n=tag>>24;
  if(n&&(fwrite(&n,4,1,f)!=1||fwrite(ff_ptr(p+4,4*n),4,n,f)!=n))return 0;
  p=tag&0xffffff;
 }
 return fwrite(&zero,4,1,f)==1;
}
static int run_loaded_game(int sequence,uint32 limit,const char *output) {
 FILE *f,*actors=NULL,*world=NULL;char actor_path[260];int result=9;uint32 frame,vblank_start,audio_start,nonzero_start;LARGE_INTEGER started,finished,frequency;
  const char *resume=diag_enabled?getenv("FF_CHECKPOINT_LOAD"):NULL,*checkpoint=diag_enabled?getenv("FF_CHECKPOINT_SAVE"):NULL;
  const char *checkpoint_tick_text=diag_enabled?getenv("FF_CHECKPOINT_TICK"):NULL;
  uint32 checkpoint_tick=checkpoint_tick_text?(uint32)strtoul(checkpoint_tick_text,NULL,10):0;
  LONGLONG prefix_ticks=0,tail_ticks=0,record_ticks=0;LARGE_INTEGER p0,p1,p2,p3;
  if(sequence&&!resume) {
   result=ff_sequence_run_80015720_stage0(sequence_prepare_frame);
   printf("continuous_sequence_result %d tick %u stage %u\n",result,ff_u32(0x80093dd0),ff_u32(0x80093d58));fflush(stdout);
   /* 15EC0..15EE8 ignores15720's result, including user skip0. */
   result=9;
  }
  if(resume){if(!diagnostic_checkpoint(resume,1))return 15;diag_title=0;}
  else {ff_level_init_80014A34_stage0();ff_level_ready_80014B70_stage0();}
  if(checkpoint&&checkpoint_tick==ff_u32(0x80093dd0)&&!diagnostic_checkpoint(checkpoint,0))return 15;
  /* A resumed audit retains absolute game ticks and ends at the same boundary. */
  if(resume&&limit){uint32 tick=ff_u32(0x80093dd0);if(tick>=limit)return 15;limit-=tick;}
  if(limit){snprintf(actor_path,sizeof(actor_path),"%s.actors",output);actors=fopen(actor_path,"wb");if(!actors)return 7;if(!save_actor_boundary(actors)){fclose(actors);return 8;}}
  if(limit&&getenv("FF_TRACE_WORLD")){snprintf(actor_path,sizeof(actor_path),"%s.world",output);world=fopen(actor_path,"wb");if(!world||!save_world_boundary(world))return 8;}
  diag_gpu_ticks=0;QueryPerformanceFrequency(&frequency);QueryPerformanceCounter(&started);
  vblank_start=(uint32)VSync(-1);audio_start=g_waveout_submitted_buffers;nonzero_start=g_waveout_nonzero_buffers;
  for(frame=0;!limit||frame<limit;frame++) {
   if(checkpoint&&checkpoint_tick!=0&&checkpoint_tick==ff_u32(0x80093dd0)) {
    if(!diagnostic_checkpoint(checkpoint,0))return 15;
    printf("checkpoint_saved tick %u path %s\n",checkpoint_tick,checkpoint);
    if(getenv("FF_CHECKPOINT_STOP"))break;
   }
   if(!diag_enabled){printf("game_frame_begin %u\n",frame);fflush(stdout);}
   QueryPerformanceCounter(&p0);
   ff_game_frame_prefix_80014DF8_stage0();QueryPerformanceCounter(&p1);result=ff_game_frame_tail_80015038_stage0();QueryPerformanceCounter(&p2);
   if(actors&&!save_actor_boundary(actors)){fclose(actors);return 8;}
   if(world&&!save_world_boundary(world)){fclose(world);return 8;}
   QueryPerformanceCounter(&p3);prefix_ticks+=p1.QuadPart-p0.QuadPart;tail_ticks+=p2.QuadPart-p1.QuadPart;record_ticks+=p3.QuadPart-p2.QuadPart;
   if(!diag_enabled){printf("game_frame_complete %u tick %u\n",frame,ff_u32(0x80093dd0));fflush(stdout);}
   if(result){++frame;break;}
   if(frame==299)ff_gpu_save_frame("../status/gameplay/game-frame-300.bgrx");
   if(frame==599)ff_gpu_save_frame("../status/gameplay/game-frame-600.bgrx");
  }
  QueryPerformanceCounter(&finished);
  if(diag_enabled)printf("diagnostic_profile prefix_ms %.3f tail_ms %.3f record_ms %.3f gpu_submit_ms %.3f\n",1000.0*prefix_ticks/frequency.QuadPart,1000.0*tail_ticks/frequency.QuadPart,1000.0*record_ticks/frequency.QuadPart,1000.0*diag_gpu_ticks/frequency.QuadPart);
  if(actors)fclose(actors);
  if(world)fclose(world);
  printf("gameplay_metrics frames %u vblanks %u seconds %.6f audio_buffers %u nonzero %u overruns %u\n",frame,(uint32)VSync(-1)-vblank_start,(double)(finished.QuadPart-started.QuadPart)/(double)frequency.QuadPart,g_waveout_submitted_buffers-audio_start,g_waveout_nonzero_buffers-nonzero_start,g_waveout_callback_overruns);
  ff_gpu_save_frame("../status/gameplay/game-frame-900.bgrx");
  f=fopen(output,"wb");
  if(!f){return 7;}
  if(fwrite(ff_ptr(0x80000000,0x200000),1,0x200000,f)!=0x200000){fclose(f);return 8;}fclose(f);
 return result;
}
static int game_loop_preview(int sequence) {
 PSX_CONFIG config;FILE *f;int result=9;
 if(!ff_load_ram(sequence?"../status/gameplay/sequence-start.ram":"../status/gameplay/sequence-level-entry.ram"))return 2;
 f=fopen(sequence?"../status/gameplay/sequence-start.scratch":"../status/gameplay/sequence-level-entry.scratch","rb");if(!f)return 3;
 if(fread(ff_ptr(0x1f800000,1024),1,1024,f)!=1024){fclose(f);return 4;}fclose(f);
 ff_gpu_init_empty();if(sequence){if(!ff_audio_init())return 3;}else ff_audio_init_empty();memset(&config,0,sizeof(config));
 config.window_title="Fighting Force gameplay loop audit";config.window_width=960;config.window_height=720;config.refresh_rate=60;config.headless=1;
 psx_configure(&config);ResetGraph(0);InitGeom();if(!waveout_init())return 5;
 ff_services.game_longjmp=exit_menu;ff_services.menu_aux=menu_aux;ff_services.frontend_frame=title_host_frame;
 if(!setjmp(menu_exit)) {
  result=run_loaded_game(sequence,900,sequence?"../status/gameplay/sequence-game-loop-final.ram":"../status/gameplay/game-loop-final.ram");
 }
 ff_services.frontend_frame=NULL;waveout_shutdown();
 printf("game_loop_result %d tick %u\n",result,ff_u32(0x80093dd0));ff_audit_end();return result;
}
int ff_game_loop_preview(void) {return game_loop_preview(0);}
int ff_sequence_game_loop_preview(void) {return game_loop_preview(1);}
/* Isolated sequence fixture audit; frontend uses run_loaded_game directly. */
int ff_sequence_preview(int headless) {
 PSX_CONFIG config;int result=9;
 if(!ff_load_ram("../status/gameplay/sequence-start.ram"))return 2;
 ff_gpu_init_empty();if(!ff_audio_init())return 3;
 memset(&config,0,sizeof(config));config.window_title="Fighting Force - sequence WIP";
 config.window_width=960;config.window_height=720;config.refresh_rate=60;config.headless=headless;
 psx_configure(&config);ResetGraph(0);InitGeom();if(!waveout_init())return 5;
 ff_services.game_longjmp=exit_menu;ff_services.menu_aux=menu_aux;ff_services.frontend_frame=title_host_frame;
 if(!setjmp(menu_exit))result=ff_sequence_run_80015720_stage0(sequence_prepare_frame);
 ff_services.frontend_frame=NULL;waveout_shutdown();
 printf("sequence_result %d tick %u stage %u\n",result,ff_u32(0x80093dd0),ff_u32(0x80093d58));ff_audit_end();return result==1?0:result;
}
/* Interactive title preview. Initial configuration still comes from fixtures. */
int ff_title_runtime(void) {
 PSX_CONFIG config;uint32 result=0;
 if(!ff_load_ram("FF-menu.ram")||!ff_gpu_load_vram("FF-menu.vram"))return 2;
 memset(&config,0,sizeof(config));config.window_title="Fighting Force - title WIP";
 config.window_width=960;config.window_height=720;config.refresh_rate=60;psx_configure(&config);
 ResetGraph(0);InitGeom();ff_w32(0x800940b0,1);ff_w16(0x80093566,0);ff_w32(0x80093c14,0xffffffff);
 ff_services.game_longjmp=exit_menu;ff_services.frontend_frame=title_host_frame;
 if(!setjmp(menu_exit)){title_host_frame();result=FUN_80069F78();}
 ff_services.frontend_frame=NULL;
 printf("title_result %u timer %u\n",result,ff_u32(0x80093dd0));ff_audit_end();return 0;
}
/* Scripted menu jobs end at confirmation; subsequent gameplay has no input.
 * Keep physical keyboard input for the ordinary interactive runtime. */
static uint32 scripted_neutral_pad(void *user,sint32 controller) {(void)user;(void)controller;return 0;}
static int menu_runtime_impl(const char *script,uint32 limit,int headless,int startup) {
 PSX_CONFIG config;FILE *input=NULL,*trace=NULL;uint32 frame=0;int result=0,i;
 if(!diagnostic_init())return 14;
 if(diag_enabled&&getenv("FF_CHECKPOINT_LOAD")){}
 else if(startup==2){if(!ff_load_game_image("GAME.EXE"))return 2;}
 else if(!ff_load_ram("FF-menu.ram"))return 2;
 if(startup){ff_gpu_init_empty();ff_audio_init_empty();}
 else if(!ff_gpu_load_vram("FF-menu.vram")||!ff_audio_init())return 2;
 if(script){input=fopen(script,"rb");trace=fopen("../status/menu/runtime-trace.bin","wb");if(!input||!trace)return 4;}
 memset(&config,0,sizeof(config));config.window_title="Fighting Force - menu WIP";
 config.window_width=960;config.window_height=720;config.refresh_rate=60;config.headless=headless;
 if(script)config.host.pad_read=scripted_neutral_pad;
 psx_configure(&config);ResetGraph(0);InitGeom();if(!waveout_init())return 5;
 ff_services.game_longjmp=exit_menu;ff_services.menu_aux=menu_aux;
 /* Resume at the reference callback entry; host pads supply original active-low bytes. */
 ff_w32(0x800927ec,0x80050270);ff_w32(0x8009355c,0x80094748);ff_w32(0x80093560,0x80094770);
 if(setjmp(menu_exit)){result=9;goto done;}
 if(diag_enabled&&getenv("FF_CHECKPOINT_LOAD")) {
  ff_services.frontend_frame=title_host_frame;
  result=run_loaded_game(0,900,diagnostic_output());
  printf("frontend_game_result %d tick %u\n",result,ff_u32(0x80093dd0));goto done;
 }
 if(startup) {
  if(startup==2) {
   FUN_8006A50C();memset(ff_ptr(0x8009e858,1044),0,1044);
   FUN_8003EE4C();FUN_8003EF78();FUN_80046854();
  }
  FUN_8005717C();
  FUN_800675FC();
  ff_display_setup_800574B4();
  ff_services.frontend_frame=title_host_frame;title_host_frame();
  if(startup==2) {
   FUN_80064684();FUN_80018E50();
   /* WIP69554: no memory-card startup UI or saved configuration is loaded. */
   ff_dummy_memory_card_read(0,NULL,0);
   FUN_8005EC90();FUN_80056ECC();ff_w32(0x800941a8,0);FUN_8005ED78();
  }
  result=(int)ff_menu_startup_8004FBCC();ff_services.frontend_frame=NULL;
  if(result)goto done;
 }
 while(!psx_quit_requested()&&(!limit||frame<limit)) {
  uint32 job[3]={1,0,0},callback,display;int cleanup;sint32 display_x,display_y;
  if(input){if(fread(job,sizeof(job),1,input)!=1)break;}
  else {uint32 pad=PadRead(0);job[1]=((pad&255)<<8)|((pad>>8)&255);}
  ff_host_pad_publish(job[0],job[1],job[2]);

  /* Fixture PC is already at the first callback, after this prefix. */
  if(frame||startup) {
   FUN_80058C14();FUN_8004F520();
   ff_w16(0x80093d34,(uint16)ff_s16(0x8008d490));ff_w16(0x80093d36,(uint16)ff_s16(0x8008d492));
   for(i=0;i<3;i++)ff_w32(0x80093d28+4u*i,ff_u32(0x8008d4a4+4u*i));
   ff_w32(0x80093dd0,ff_u32(0x80093dd0)+1);
   /* 80058650..80058660 uploads the current 256-color palette first. */
   if(!ff_gpu_load_clut(0x800b50b8,0,499)||!ff_gpu_load_image(0x80093568,0x800a1990)){result=12;break;}
   FUN_80011D9C();
  }
  display=0x800947a0+20*(ff_u32(0x8008d4c4)&1);
  display_x=ff_s16(display+8);display_y=ff_s16(display+10);
  FUN_80064684();ff_display_offsets_800586D0();
  /* Original5868C..587A4: compress prior OT before swapping, submit it,
   * then initialize the new current OT. Fixture frame0 was already compressed. */
  ff_gpu_begin();
  /* 8005876C..80058794: DrawOTagEnv(previous OT, DrawEnv[parity]).
   * Save State 1 uses horizontal VRAM draw surfaces at x=0 and x=320. */
  ff_gpu_draw_env(0x800b8938+92*(ff_u32(0x8008d4c4)&1),320*(ff_u32(0x8008d4c4)&1),0);
  if(ff_gpu_ot(ff_u32(0x8008d4b8))<0){result=7;break;}
  ff_gpu_display_offset(display_x,display_y);ff_gpu_present();
  FUN_80011D50(ff_u32(0x8008d4b4));
  if(frame||startup) {
   if(ff_s16(0x8001000c)+ff_s16(0x8001000e)+ff_s16(0x80010010))FUN_8006471C();
   if(ff_u32(0x80094190))FUN_80064820();
   ff_menu_horizon_80058844();
  }
  /* Frame zero retains the entry fixture; subsequent horizons are projected. */
  FUN_8006686C();FUN_80063EEC();
  /* 58BD0..58BF4 runs after packet construction; affects the next frame. */
  if((frame||startup)&&(sint32)ff_u32(0x80093dd0)<33) {
   sint32 fade=(sint32)((ff_u32(0x80093dd0)<<3)-256u);FUN_80011CCC(fade,fade,fade);
  }
  callback=ff_u32(0x800927ec);
  if(!ff_u32(0x80092788)) {
  if(callback==0x80050270)FUN_80050270();
  else if(callback==0x80051e50)FUN_80051E50();
  else if(callback==0x800520ac)FUN_800520AC();
  else if(callback==0x80050798)FUN_80050798();
  else if(callback==0x80050d98)FUN_80050D98();
  else if(callback==0x80051c98)FUN_80051C98();
  else if(callback==0x800519d4)FUN_800519D4();
  else if(callback==0x80052624)FUN_80052624();
  else {fprintf(stderr,"WIP: untranslated menu callback %08X\n",callback);result=10;break;}
  }
  /* 8004FFB4..8004FFCC: update then draw actors with original selections/count. */
  if(!ff_u32(0x80092734)) {
   ff_menu_stage_8001E168();FUN_80050CC8();FUN_80058E24(0x8009277c,(sint32)ff_u32(0x80092784));
   /* 8004FFD0..80050034: scene update callbacks, followed by object rendering. */
   for(i=0;i<(sint32)ff_u32(0x8009403c);i++) {
    uint32 object=0x800b89f0+36u*i,target=ff_u32(0x800bbf78+4*ff_u32(object));
    if(target)ff_object_call(target,object);
   }
   FUN_8005C17C();FUN_8001EE44();
   /* 80050040..8005007C: camera matrix, effect rendering, then effect update.
    * Dynamic object rendering has run immediately before this matrix setup. */
   FUN_80011D20();FUN_800101CC(ff_s16(0x8008d492));FUN_8001033C(ff_s16(0x8008d490));
   FUN_8001EF68();FUN_8001EDB8();FUN_8001ED1C();
  }
  cleanup=ff_menu_tail_80050090(ff_u32(0x80092734)!=0);
  if(trace){uint32 row[8]={frame,callback,ff_u32(0x800927ec),ff_u32(0x80092730),ff_u32(0x8009277c),ff_u32(0x80092780),ff_u32(0x8009272c),ff_u32(0x80092788)};fwrite(row,sizeof(row),1,trace);}
  ++frame;
  if(cleanup){
   FUN_8005F214();FUN_80056ECC();
   if(ff_u32(0x80093d58)!=0||ff_u32(0x80093d1c)!=0xffffffffu){fprintf(stderr,"WIP: non-stage0 menu continuation\n");result=11;break;}
   ff_services.frontend_frame=title_host_frame;
   result=run_loaded_game(1,script?900:0,diagnostic_output());
   printf("frontend_game_result %d tick %u\n",result,ff_u32(0x80093dd0));
   if(result==1){
    /* 15EF0..15EF8, then16114..15CFC..15D14 on explicit quit. */
    ff_w32(0x800941a8,0);FUN_8005ED78();
    FUN_80056ECC();ff_w32(0x800941a8,0);FUN_8005ED78();
    result=(int)ff_menu_startup_8004FBCC();ff_services.frontend_frame=NULL;
    printf("frontend_menu_return %d stage %u\n",result,ff_u32(0x80093d58));fflush(stdout);
    if(result)break;continue;
   }
   break;
  }
  VSync(0);
 }
done:
 ff_services.frontend_frame=NULL;waveout_shutdown();if(input)fclose(input);if(trace)fclose(trace);
 ff_gpu_save_frame("../status/menu/runtime-frame.bgrx");
 printf("menu_frames %u result %d audio_buffers %u nonzero %u peak %u\n",frame,result,g_waveout_submitted_buffers,g_waveout_nonzero_buffers,g_waveout_peak);
 ff_audit_end();return result;
}

int ff_menu_runtime(const char *script,uint32 limit,int headless) {return menu_runtime_impl(script,limit,headless,0);}
int ff_menu_startup_runtime(uint32 limit,int headless) {return menu_runtime_impl(NULL,limit,headless,1);}
int ff_menu_cold_runtime(uint32 limit,int headless) {return menu_runtime_impl(NULL,limit,headless,2);}
int ff_menu_cold_script_runtime(const char *script,int headless) {return menu_runtime_impl(script,0,headless,2);}

int ff_menu_startup_script_runtime(const char *script,int headless) {return menu_runtime_impl(script,0,headless,1);}
