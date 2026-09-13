#include "ff.h"
static uint32 bike_partner(uint32 actor) {return 0x800b4318+244u*(uint32)(sint32)ff_s16(actor+88);}
static void bike_partner_state(uint32 actor,uint32 state) {
 uint32 partner=bike_partner(actor);FUN_80019F00(partner,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(partner+52)),state);
}
GDB_CALL sint32 FUN_80055C30(sint32 index) {
 FF_FUNCTION_MARKER(0x80055c30u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table,angle,speed,pitch,target,state,partner,point[3],saved[4],x,z,result;
 sint32 volume,delta;int i;
 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));angle=2u*(uint32)(sint32)ff_s16(actor+56);
 ff_w32(actor+40,ff_u32(actor+40)+(uint32)(sint32)ff_s16(0x800802d0+angle)*ff_u32(actor+216));
 pitch=ff_u32(actor+216)<<2;ff_w32(actor+48,ff_u32(actor+48)-(uint32)(sint32)ff_s16(0x800806d0+angle)*ff_u32(actor+216));
 FUN_80056840(ff_u32(actor+220)-1,pitch+((ff_rand_8006D348()&15)?6144u:4096u));FUN_80056840(ff_u32(actor+224)-1,pitch+5120);
 for(i=0;i<4;++i)saved[i]=ff_u32(0x801fe780+4*i);
 FUN_80046884(actor+40,0x801fe780,0x801fe784);
 volume=(sint32)(ff_u32(0x801fe780)*(96u-(uint32)((sint32)ff_u32(actor+216)>>1)))/176;
 volume=FUN_80042820(volume,60);FUN_800568C8(ff_u32(actor+220)-1,volume,ff_u32(0x801fe784));
 volume=(sint32)(ff_u32(0x801fe780)*(ff_u32(actor+216)-64u))/48;
 volume=FUN_80042820(volume,0);FUN_800568C8(ff_u32(actor+224)-1,volume,ff_u32(0x801fe784));
 if(ff_s16(actor+88)!=-1){
  FUN_80049B78(index);target=ff_u32(0x800942e0);ff_w32(actor+168,target);partner=0x800b4318+244*target;
  delta=(sint16)(FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(partner+42),ff_s16(partner+50))-((uint32)(uint16)ff_s16(actor+56)<<5));
  state=ff_u32(actor+12);
  if(state==278){
   if((sint32)ff_u32(actor+216)>0)ff_w32(actor+216,ff_u32(actor+216)-4);
   if(ff_u32(actor+4)==0xffffffffu){
    ff_w32(actor+216,0u-ff_u32(actor+216));ff_w16(actor+56,(uint16)ff_s16(actor+56)^1024);FUN_80019F00(actor,table,277);
    FUN_800567F8(ff_u32(actor+228)-1);ff_w32(actor+228,0);FUN_80056670(142,159,5888);bike_partner_state(actor,279);
   }
  }else if(state==277){
   speed=ff_u32(actor+216);if((sint32)speed<0)ff_w32(actor+216,speed+4);else if((sint32)speed<96)ff_w32(actor+216,speed+1);
   if((uint16)(delta+16384)<=32768){if(delta<-256)delta=-256;if(delta>256)delta=256;ff_w16(actor+56,(uint16)(((uint16)ff_s16(actor+56)+(delta>>5))&2047));}
   else{FUN_80019F00(actor,table,278);pitch=(ff_rand_8006D348()&255)+5504;FUN_80056670(230,95,pitch);pitch=(ff_rand_8006D348()&255)|5888;FUN_80056670(230,127,pitch);bike_partner_state(actor,280);}
   if(ff_u32(actor+4)==0xffffffffu)FUN_80019F00(actor,table,277);
  }
  FUN_8001B9F4(index,1,point);ff_w32(0x801fe780,point[0]);ff_w32(0x801fe784,point[2]);
  x=point[0];z=point[2];FUN_8001A4D0(bike_partner(actor),0x801fe780,0x801fe784);
  if(ff_u32(0x801fe780)==x&&ff_u32(0x801fe784)==z){ff_w32(bike_partner(actor)+40,x);ff_w32(bike_partner(actor)+44,0x680000);ff_w32(bike_partner(actor)+48,ff_u32(0x801fe784));}
  else{bike_partner_state(actor,281);ff_w16(bike_partner(actor)+88,65535);ff_w16(actor+88,65535);}
  state=ff_u32(actor+12);result=277;
  /* Preserve the original post-detach signed -1 partner indexing. */
  if(state==278||state==277){
   ff_w16(bike_partner(actor)+56,(uint16)ff_s16(actor+56)^(state==278?1024:0));
   ff_w32(0x801fe780,0);ff_w32(0x801fe784,state==278?0x400000:0x100000);ff_w32(0x801fe788,state==278?0xff780000u:0x1000000);
   FUN_800234E8(0,0x800ba214+504u*(uint32)index,0x801fe780);
   if(state==278)result=FUN_80031B20(871,ff_u32(0x801fe780),ff_u32(0x801fe784),ff_u32(0x801fe788),0,0,0);
   else{
    FUN_80055964(index,ff_u32(0x801fe780),ff_u32(0x801fe788));result=FUN_80055B68(ff_u32(0x801fe780),ff_u32(0x801fe788));
    if(result!=0xffffffffu){bike_partner_state(actor,281);result=244u*(uint32)(sint32)ff_s16(actor+88);ff_w16(0x800b4370+result,65535);ff_w16(actor+88,65535);}
   }
  }
 }else{
  FUN_800567F8(ff_u32(actor+220)-1);FUN_800567F8(ff_u32(actor+224)-1);if(ff_u32(actor+228))FUN_800567F8(ff_u32(actor+228)-1);
  FUN_80056670(19,255,6144);angle=(uint16)ff_s16(actor+56);
  FUN_80031C50(893,ff_u32(actor+40),ff_u32(actor+44),ff_u32(actor+48));
  for(i=0;i<2;++i){x=ff_u32(actor+40)+(ff_rand_8006D348()<<8)-0x40000;z=ff_u32(actor+48)+(ff_rand_8006D348()<<8)-0x40000;FUN_80031C50(893,x,ff_u32(actor+44),z);}
  angle=(uint32)(sint32)(sint16)(angle<<5);FUN_800168BC(580,ff_u32(actor+40),0,ff_u32(actor+48),angle);
  FUN_800167C4(532,ff_u32(actor+40),0xffb00000u,ff_u32(actor+48),angle+16384);
  FUN_80016748(520,ff_u32(actor+40),0xffb00000u,ff_u32(actor+48),angle+32768);
  FUN_80016748(531,ff_u32(actor+40),0xffb00000u,ff_u32(actor+48),angle);FUN_80016748(531,ff_u32(actor+40),0xffb00000u,ff_u32(actor+48),angle+32768);
  for(i=0;i<16;++i){target=(ff_rand_8006D348()&3)+902;FUN_80031CD8(target,ff_u32(actor+40),ff_u32(actor+44),ff_u32(actor+48));}
  ff_w32(0x800941a8,16);ff_w32(0x80094190,4);ff_w32(actor,0);ff_w16(actor+52,65535);result=0xffffffffu;
 }
 for(i=0;i<4;++i)ff_w32(0x801fe780+4*i,saved[i]);return (sint32)result;
}
GDB_CALL sint32 FUN_8004D2D4(sint32 index) {
 FF_FUNCTION_MARKER(0x8004d2d4u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),state,next=0,result; sint32 drive;
 if(FUN_8004CFE4(index)<0)return FUN_80053558(index);
 drive=(sint32)ff_u32(actor+220);
 if(drive>=257){
  state=ff_u32(actor+12);ff_w32(actor+228,0x590000);
  if(state==255&&ff_u32(actor+4)==0xffffffff)FUN_8004C4D8(actor,table,254);
  state=ff_u32(actor+12); /* 4D38C reload after the first transition. */
  if(state==254){if(ff_u32(actor+4)==0xffffffff)next=254;}
  else if(state==257||state==258){if(ff_u32(actor+4)==0xffffffff)next=259;}
  else if(state==256||state==259){if(ff_u32(actor+4)==0xffffffff)next=255;}
  else if(state==266)next=255;
 }else if(drive< -320){
  state=ff_u32(actor+12);ff_w32(actor+228,0x290000);
  if(state==255||state==254){if(ff_u32(actor+4)==0xffffffff)next=256;}
  else if(state==258||state==257){if(ff_u32(actor+4)==0xffffffff)next=257;}
  else if(state==256||state==259){if(ff_u32(actor+4)==0xffffffff)next=258;}
  else if(state==266)next=258;
 }else{
  state=ff_u32(actor+12);ff_w32(actor+228,0x90000);
  if(state==254||state==255){if(ff_u32(actor+4)!=0xffffffff)return -1;next=256;}
  else if(state==257||state==258){if(ff_u32(actor+4)!=0xffffffff)return -1;next=259;}
  else if(state==256||state==259){if(ff_u32(actor+4)!=0xffffffff)return -1;next=266;}
  else if(state==266){
   if((sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+224))next=268;
   else{if(ff_u32(actor+4)!=0xffffffff)return -1;next=266;}
  }else return 266;
  return FUN_8004C4D8(actor,table,next);
 }
 if(next)FUN_8004C4D8(actor,table,next);result=ff_u32(0x80093dd0)+20;ff_w32(actor+224,result);return (sint32)result;
}

GDB_CALL sint32 FUN_8004930C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004930cu,"GAME.EXE");
 uint32 source=0x800b4318+244u*(uint32)index,item=0x8009a3c8+36u*(uint32)(sint32)*(sint8*)ff_ptr(source+126,1);
 sint32 source_type=ff_s16(source+52),type,angle,direction,distance,victim_index;uint32 kind=ff_u32(item),position[3],victim,debris;int i,detach;
 FUN_8001CF08(index,0,position);detach=source_type==32||(uint32)source_type-49u<2u||source_type==48||source_type==14;
 for(victim_index=0;victim_index<(sint32)ff_u32(0x800940b0);++victim_index){
  victim=0x800b4318+244u*(uint32)victim_index;
  if(((ff_s16(source+82)>>(victim_index&31))&1)||FUN_80048CFC(ff_u32(victim+12)))continue;
  type=ff_s16(victim+52);distance=FUN_800163F4((sint32)position[0]>>16,(sint32)position[2]>>16,ff_s16(victim+42),ff_s16(victim+50));
  angle=ff_s16(0x80094120+2u*(uint32)(sint32)ff_s16(victim+52));ff_w32(0x800941a0,(uint32)distance);if(distance>=angle)continue;
  angle=(sint16)FUN_8001614C(ff_s16(victim+42),ff_s16(victim+50),ff_s16(source+42),ff_s16(source+50));
  direction=(sint16)(((uint32)(uint16)ff_s16(victim+56)<<5)-(uint32)angle-16384u);angle=ff_s16(victim+56);
  FUN_8001697C(865,position[0],position[1],position[2]);FUN_80017C78(victim_index);FUN_800293D8(victim_index);angle^=1024;
  if(kind==478||kind==480||kind==482){
   ff_w32(item,kind+1);FUN_80056670(113,255,6400);debris=599+(kind-478)/2;
   for(i=0;i<12;++i)FUN_800315A4(debris,position[0],position[1],position[2],(uint32)angle-512);
  }
  ff_w8(victim+129,(uint8)index);
  for(i=0;i<8;++i)FUN_800310B8(position[0],position[1],position[2],(uint32)angle);
  for(i=0;i<32;++i)FUN_8003100C(position[0],position[1],position[2],(uint32)angle);
  FUN_8001F468(*(sint8*)ff_ptr(victim+129,1),victim_index,ff_u32(item));
  if(ff_s16(victim+66)<=0){
   if(detach)FUN_80029A40(index);FUN_80017E30(victim_index);FUN_8001B8DC(victim_index);
   FUN_80019F00(victim,ff_u32(0x800b3be8+4u*(uint32)type),147);ff_w16(victim+66,0);
  }else{
   ff_w16(source+82,(uint16)((uint16)ff_s16(source+82)|(1u<<(victim_index&31))));
   if(ff_s16(victim+74)<49)FUN_80019F00(victim,ff_u32(0x800b3be8+4u*(uint32)type),direction<0?133:152);
   else{if(detach)FUN_80029A40(index);FUN_80019F00(victim,ff_u32(0x800b3be8+4u*(uint32)type),direction<0?147:154);FUN_80017E30(victim_index);ff_w16(victim+74,0);}
  }
  return 1;
 }
 return 0;
}

static sint32 npc_action_call(uint32 callback,sint32 index);
GDB_CALL sint32 FUN_8004DF2C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004df2cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,events,level,volume=0,result; sint32 timer,partner;int bit,change=0;
 ff_w32(0x800947ec,(uint32)index);if(ff_s16(actor+66)>0)FUN_8004DB94(index);
 if(ff_u32(0x800944b8)==0xffffffff){
  ff_w32(0x800944b8,(uint32)FUN_80056670(181,0,5632));ff_w32(0x800944c0,(uint32)FUN_80056670(184,255,6144));ff_w32(0x800944c4,(uint32)FUN_80056670(188,0,6400));
 }
 level=ff_u32(0x800944bc);
 if(ff_u32(actor+12)-270u<2u){if((sint32)level<248){level+=8;change=1;}}
 else if((sint32)level>0){level-=8;change=1;}
 if(change){ff_w32(0x800944bc,level);FUN_80056840((sint32)ff_u32(0x800944b8),(sint32)((level<<2)+5120));FUN_80056F08(ff_u32(0x800944b8),ff_u32(0x800944bc));}
 if(ff_u32(actor+232)&1){FUN_80040CF4(index);volume=255;ff_w32(actor+232,ff_u32(actor+232)&~1u);}
 FUN_80056F08(ff_u32(0x800944c0),volume);
 timer=*(sint8*)ff_ptr(actor+119,1);
 if(timer){timer=(sint8)(timer+1);ff_w8(actor+119,(uint8)timer);if(timer>=61){ff_w32(actor,0);ff_w16(actor+52,65535);}return -1;}
 if(ff_s16(actor+66)<=0){partner=*(sint8*)ff_ptr(actor+122,1);if(partner!=-1){ff_w8(0x800b4390+244u*(uint32)partner,255);ff_w8(actor+122,255);}return FUN_8001B8DC(index);}
 if(ff_s16(actor+80)>0)return FUN_8004E3D8(index);
 events=ff_u32(actor+28);if(events&0x800)ff_w16(actor+82,0);
 for(bit=0;bit<9;++bit)if(events&(1u<<bit))if(FUN_80048DBC(index,bit+1))FUN_80052878(index);
 if((events&0x400)&&FUN_8004930C(index)){result=(ff_rand_8006D348()&1)+33;FUN_80056670(result,255,6144);}
 if(events&0x1000){ff_actor_event_call(ff_u32(0x800bb9b0+4u*ff_u32(actor+12)),index);ff_w16(actor+84,(uint16)((uint16)ff_s16(actor+84)+1));}
 result=(uint32)FUN_80054C90(index);if(result)return (sint32)result;
 return npc_action_call(ff_u32(ff_u32(actor+160)+4u*ff_u32(actor+12)),index);
}
GDB_CALL sint32 FUN_8004E7E8(sint32 index) {
 FF_FUNCTION_MARKER(0x8004e7e8u,"GAME.EXE");
 static const uint32 styles[3]={108,106,107};
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 uint32 temp=0x801fcf00,saved[8],points[8],source,matrix,target,delta,result,state=272;int i,j;
 for(i=0;i<3;++i)if(ff_u32(actor+28)&(0x2000u<<i)){
  if(!(ff_u32(actor+232)&2)){
   source=0x800923b4+32u*i;for(j=0;j<8;++j)points[j]=ff_u32(source+4u*j);
   for(j=0;j<8;++j){saved[j]=ff_u32(temp+4u*j);ff_w32(temp+4u*j,points[j]);}
   matrix=0x800ba254+504u*(uint32)index;FUN_800234E8(index,matrix,temp);FUN_800234E8(index,matrix,temp+16);
   for(j=0;j<8;++j)points[j]=ff_u32(temp+4u*j);
   FUN_8004150C(styles[i],points[4],points[5],points[6],points[0],points[1],points[2],0x01000080,0x00c0c0c0);
   for(j=0;j<8;++j)ff_w32(temp+4u*j,saved[j]);
  }
  FUN_80056670(24,128,6144);result=(uint32)FUN_8004E6AC(index);ff_w32(actor+172,ff_u32(actor+172)+result);
 }
 if((sint32)ff_u32(actor+172)>=6)ff_w32(actor+172,0);
 else{
  if(ff_u32(actor+4)!=0xffffffff)return -1;
  ff_rand_8006D348(); /* Deliberately unused draw still advances the RNG. */
  if(ff_u32(actor+164)==1){target=ff_u32(actor+168);if(target!=0xffffffff){
   delta=(uint32)FUN_800483BC(index,ff_u32(0x800b4340+244u*target),ff_u32(0x800b4348+244u*target));
   if((uint16)(delta+2047)<4095)state=271;
  }}
 }
 return FUN_8004C4D8(actor,table,state);
}

GDB_CALL uint32 FUN_8004D64C(sint32 index,uint32 scale,uint32 output[16]) {
 FF_FUNCTION_MARKER(0x8004d64cu,"GAME.EXE");
 int i;
 if(scale){ff_w32(0x80092390,0u-8u*scale);ff_w32(0x80092394,89u*scale);ff_w32(0x80092398,0u-12u*scale);}
 else{ff_w32(0x80092390,0xfff80000);ff_w32(0x80092398,0xfff40000);ff_w32(0x80092394,ff_u32(0x800b43fc+244u*index));}
 FUN_80010040();FUN_80011F74(0x800ba254+504u*index);
 FUN_800614F4(ff_u32(0x80092384),ff_u32(0x80092388),ff_u32(0x8009238c),output);
 FUN_800614F4(ff_u32(0x80092384)+ff_u32(0x80092390),ff_u32(0x80092388)+ff_u32(0x80092394),ff_u32(0x8009238c)+ff_u32(0x80092398),output+4);
 FUN_800614F4(0u-ff_u32(0x80092384),ff_u32(0x80092388),ff_u32(0x8009238c),output+8);
 FUN_800614F4(0u-(ff_u32(0x80092384)+ff_u32(0x80092390)),ff_u32(0x80092388)+ff_u32(0x80092394),ff_u32(0x8009238c)+ff_u32(0x80092398),output+12);
 FUN_80011D20();FUN_8001033C(-ff_s16(0x8008d490));FUN_800101CC(-ff_s16(0x8008d492));
 for(i=0;i<16;i+=4)FUN_800614F4(output[i],output[i+1],output[i+2],output+i);FUN_80010028();
 output[4]-=output[0];output[5]-=output[1];output[6]-=output[2];
 output[0]+=ff_u32(0x8008d4a4);output[1]+=ff_u32(0x8008d4a8);
 output[12]-=output[8];output[14]-=output[10];output[13]-=output[9];output[2]+=ff_u32(0x8008d4ac);
 output[8]+=ff_u32(0x8008d4a4);output[9]+=ff_u32(0x8008d4a8);output[10]+=ff_u32(0x8008d4ac);return output[10];
}
GDB_CALL sint32 FUN_8004D914(sint32 index) {
 FF_FUNCTION_MARKER(0x8004d914u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*index,flags=ff_u32(actor+28),point[16];int i;
 if(flags&0x1000u){if(flags&0x800u)FUN_80032EE0(index);else FUN_80056670(187,255,6144);}
 if(flags&0xe000u){
  ff_w32(actor+228,0x00c90000);
  if(!(ff_u32(actor+232)&2u)){
   FUN_8004D64C(index,0x28000,point);
   for(i=0;i<2;i++)if(flags&(0x2000u<<i)){
    uint32 *p=point+8*i;FUN_8004141C(876,p[0],p[1],p[2],p[4],p[5],p[6]);FUN_8004141C(876,p[0],p[1],p[2],p[4]<<1,p[5],p[6]<<1);
   }
   if(flags&0x8000u){FUN_8004D64C(index,0x18000,point);for(i=0;i<2;i++){uint32 *p=point+8*i;FUN_8004141C(877,p[0],p[1],p[2],p[4],p[5],p[6]);}}
  }
 }else ff_w32(actor+228,0x00590000);
 if(ff_u32(actor+4)==0xffffffffu){ff_w32(actor+228,0x00090000);return FUN_80053558(index);}return 0x00090000;
}
GDB_CALL sint32 FUN_8003B8E4(sint32 index) {
 FF_FUNCTION_MARKER(0x8003b8e4u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*index,yaw,result;sint32 target,delta,action=-1;
 if(ff_u32(0x80093d58)!=4)return FUN_80052944(index);ff_w16(actor+80,1);
 if((sint32)ff_u32(0x80093dd0)>=301){
  target=(sint32)ff_u32(0x80093dd0)>=571?5200:4800;
  if(FUN_800163F4(-128,target,ff_s16(actor+42),ff_s16(actor+50))>=257){
   yaw=(uint32)(uint16)ff_s16(actor+56)<<5;
   delta=(sint16)((uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),-128,target)-yaw+2u);delta>>=2;
   if(delta>512)delta=512;else if(delta<-512)delta=-512;ff_w16(actor+56,(uint16)(yaw+(uint32)delta)>>5);
   if(ff_u32(actor+4)!=0xffffffffu&&ff_u32(actor+12)!=69)goto finish;
   if((uint32)(uint16)ff_s16(actor+56)-961u<127u&&(sint32)ff_u32(0x800941a0)>=513)action=87;
   else if(ff_s16(actor+56)<1536)action=82;
  }
 }
 if(action==-1&&ff_u32(actor+4)==0xffffffffu)action=69;
 if(action!=-1)FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),action);
finish:
 if(ff_u32(0x80093dd0)!=630)return -1;ff_w16(actor+52,65535);result=ff_u32(0x80093dbc)-1u;ff_w32(0x80093dbc,result);return (sint32)result;
}
GDB_CALL sint32 FUN_8004DB94(sint32 index) {
 FF_FUNCTION_MARKER(0x8004db94u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*index,other,state; sint32 i,heading;int hit=0;
 if((sint32)(ff_u32(actor+44)+ff_u32(actor+16))<(sint32)0xff000000u)return 1;
 state=ff_u32(actor+12);if(state-274u<2u||state==276)return 276;
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++){
  other=0x800b4318+244u*i;if(ff_s16(other+52)==-1||ff_s16(other+66)<=0)continue;
  heading=(sint16)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50));heading>>=5;
  if((sint32)ff_u32(0x800941a0)<1280&&(sint32)ff_u32(actor+176)<(sint32)(ff_u32(0x80093dd0)+20u))ff_w32(actor+232,ff_u32(actor+232)|1u);
  if((sint32)ff_u32(0x800941a0)>=448)continue;state=ff_u32(other+12);
  if(state==147||state==148||state==252||state==251||(sint32)ff_u32(0x80093dd0)<(sint32)ff_u32(actor+176))continue;
  FUN_80040E14(index,i);FUN_80019F00(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),251);
  ff_w16(other+56,(uint16)(heading^1024));FUN_8001F230(index,i,20);FUN_80056670(185,255,6144);
  hit=1;ff_w32(other+232,ff_u32(other+232)|1u);ff_w32(other+176,ff_u32(0x80093dd0)+20u);
 }
 if(hit){ff_w32(actor+176,ff_u32(0x80093dd0)+80u);return FUN_800532B8(index);}return 0;
}
GDB_CALL sint32 FUN_8004CFE4(sint32 index) {
 FF_FUNCTION_MARKER(0x8004cfe4u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*index,target,x,z,yaw,phase,offset; sint32 slot,delta,reach,speed;
 slot=(sint32)ff_u32(actor+168);
 if(ff_u32(actor+164)==1&&slot!=-1){target=0x800b4318+244u*slot;x=ff_u32(target+40);z=ff_u32(target+48);}
 else {if(ff_u32(actor+164)!=4||slot==-1)return -1;target=0x8009a3c8+36u*slot;x=ff_u32(target+4);z=ff_u32(target+12);}
 delta=FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),(sint32)x>>16,(sint32)z>>16);
 yaw=(uint16)ff_s16(actor+56);delta=(sint16)((uint32)delta-(yaw<<5));
 yaw+=delta<-1024?(uint32)-32:delta>1024?32u:(uint32)(delta>>5);ff_w16(actor+56,(uint16)yaw);
 reach=(sint32)(ff_u32(0x800941a0)-384u);ff_w16(actor+56,(uint16)(yaw&2047));if(reach>384)reach=384;
 phase=ff_u32(actor+220);phase+=(uint32)((sint32)(((uint32)reach<<2)-phase)>>3);ff_w32(actor+220,phase);
 offset=((uint32)((sint32)phase>>5)&2047u);ff_w32(actor+216,offset);
 speed=ff_s16(0x800802d0+offset*2u)>>6;ff_w32(actor+216,(uint32)speed);
 if(speed>48)speed=48;else if(speed<-48)speed=-48;ff_w32(actor+216,(uint32)speed);
 offset=2u*(uint32)(sint32)ff_s16(actor+56);ff_w32(actor+40,ff_u32(actor+40)+(uint32)(speed*ff_s16(0x800802d0+offset)));
 ff_w32(actor+48,ff_u32(actor+48)-(uint32)(speed*ff_s16(0x800806d0+offset)));return (sint32)ff_u32(0x800941a0);
}
GDB_CALL sint32 FUN_80054598(sint32 index) {
 FF_FUNCTION_MARKER(0x80054598u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),other,random;sint32 slot,action;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;slot=*(sint8*)ff_ptr(actor+122,1);if(slot==-1)return FUN_800532B8(index);other=0x800b4318+244u*(uint32)slot;
 if(table==0x800849a4||table==0x80082e8c||table==0x80083794||table==0x80082a08)action=181;
 else{
  random=ff_rand_8006D348()&3u;if(!random)return FUN_8004C4D8(actor,table,10);if(random==1)return FUN_8004C4D8(actor,table,40);
  /* The MIPS fallback at54764 is unreachable after ANDI3 and the 0/1 gates. */
  action=table==0x80084520?182:180;
 }
 ff_w8(actor+122,255);ff_w8(other+120,255);ff_w8(other+129,(uint8)index);FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),action);
 return FUN_8004C4D8(actor,table,177);
}
GDB_CALL sint32 FUN_80055964(sint32 attacker,uint32 world_x,uint32 world_z) {
 FF_FUNCTION_MARKER(0x80055964u,"GAME.EXE");
 sint32 x=(sint32)world_x>>16,z=(sint32)world_z>>16,index,type,slot;uint32 actor,state,other,source=0x800b4318+244u*(uint32)attacker,angle;
 for(index=0;index<8;index++){
  actor=0x800b4318+244u*(uint32)index;if(ff_s16(actor+80)||*(sint8*)ff_ptr(actor+120,1)!=-1)continue;
  type=ff_s16(actor+52);if(type==-1||type==17||type==51)continue;state=ff_u32(actor+12);
  if(state==147||state==167||state==148||ff_s16(actor+66)<=0)continue;
  if(FUN_800163F4(x,z,ff_s16(actor+42),ff_s16(actor+50))>=64)continue;
  FUN_8001F230(-1,index,-100);FUN_8001B850(attacker,index);
  angle=(uint32)FUN_8001627C(ff_s16(source+42),ff_s16(source+50),ff_s16(actor+42),ff_s16(actor+50))^512u;ff_w16(actor+56,(uint16)angle);FUN_80017E30(index);
  slot=*(sint8*)ff_ptr(actor+122,1);if(slot!=-1){other=0x800b4318+244u*(uint32)slot;ff_w8(actor+122,255);ff_w8(other+120,255);
   if(state==169)FUN_80019F00(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),148);
  }
  FUN_80019F00(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),147);
 }return 0;
}
GDB_CALL sint32 FUN_8004CB88(sint32 index) {
 FF_FUNCTION_MARKER(0x8004cb88u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),other,angle,pair;sint32 slot,random,divisor;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;slot=*(sint8*)ff_ptr(actor+122,1);if(slot==-1)return FUN_800532B8(index);other=0x800b4318+244u*(uint32)slot;
 angle=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),-1216,432)-((uint32)(uint16)ff_s16(actor+56)<<5);
 if((sint32)ff_u32(0x800941a0)>=513&&(uint16)(angle+4096u)>=8193u){
  FUN_80019F00(actor,table,167);ff_w8(0x800b4391,1);return FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),172);
 }
 ff_w8(actor+122,255);ff_w8(other+120,255);ff_w8(other+129,(uint8)index);random=(sint32)ff_rand_8006D348();divisor=(sint32)ff_u32(0x80092380);
 if(!divisor||(divisor==-1&&random==(sint32)0x80000000u))ff_wip_stop(__FUNCTION__,__FILE__,__LINE__);
 pair=0x8007f784+8u*(uint32)(random%divisor);FUN_80019F00(actor,table,ff_u32(pair));
 return FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),ff_u32(pair+4));
}
GDB_CALL sint32 FUN_8004E6AC(sint32 index) {
 FF_FUNCTION_MARKER(0x8004e6acu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,temp=0x801fc380,saved[9],angle;int i,ray;sint32 hit=0;
 /* Same save/restore RAM adapter as 30310; original ray reads only
  * initialized kind/XYZ/yaw/owner fields for projectile 499. */
 for(i=0;i<9;i++)saved[i]=ff_u32(temp+4u*i);ff_w32(temp,499);ff_w16(temp+28,(uint16)index);
 for(ray=0;ray<3;ray++){
  ff_w32(temp+4,ff_u32(actor+40));ff_w32(temp+8,ff_u32(actor+44)-0x400000u);ff_w32(temp+12,ff_u32(actor+48));
  angle=(uint16)ff_s16(actor+56);if(ray==1)angle-=384u;else if(ray==2)angle+=384u;ff_w16(temp+16,(uint16)(angle<<5));
  hit=FUN_8002FD90(temp,index);if(hit)break;
 }
 for(i=0;i<9;i++)ff_w32(temp+4u*i,saved[i]);return hit!=0;
}
GDB_CALL sint32 FUN_8004CE48(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ce48u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),other,frame,angle;sint32 slot=*(sint8*)ff_ptr(actor+122,1);
 other=0x800b4318+244u*(uint32)slot;if(slot==-1)return FUN_800532B8(index);
 FUN_800483BC(index,0xfb400000u,0x01b00000);FUN_80023664(actor,other);frame=ff_u32(actor+4);if(frame!=0xffffffffu)return (sint32)frame;
 angle=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),-1216,432)-((uint32)(uint16)ff_s16(actor+56)<<5);
 if((sint32)ff_u32(0x800941a0)>=257&&(uint16)(angle+2048u)>=4097u){
  FUN_800237CC(actor,table,169);return FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),77);
 }
 ff_w8(actor+122,255);ff_w8(other+120,255);ff_w8(other+129,(uint8)index);FUN_80019F00(actor,table,178);
 return FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),184);
}
GDB_CALL sint32 FUN_800555DC(sint32 index) {
 FF_FUNCTION_MARKER(0x800555dcu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),item,point[3];sint32 slot;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;FUN_8004C4D8(actor,table,160);
 slot=*(sint8*)ff_ptr(actor+126,1);if(slot!=-1){
  ff_w8(actor+126,255);item=0x8009a3c8+36u*(uint32)slot;
  ff_w16(item+24,128);ff_w16(item+32,64);ff_w16(item+26,6528);ff_w16(item+16,(uint16)((uint32)(uint16)ff_s16(actor+56)<<5));
  FUN_8001B9F4(index,3,point);ff_w32(item+4,point[0]);ff_w32(item+8,point[1]);ff_w16(item+28,(uint16)index);ff_w32(item+12,point[2]);
 }
 if(*(sint8*)ff_ptr(actor+125,1)==-1)return -1;
 /* Original 800556D8 reloads slot 126, even after clearing it above. */
 slot=*(sint8*)ff_ptr(actor+126,1);ff_w8(actor+125,255);item=0x8009a3c8+36u*(uint32)slot;
 ff_w16(item+24,128);ff_w16(item+32,64);ff_w16(item+26,6528);ff_w16(item+16,(uint16)((uint32)(uint16)ff_s16(actor+56)<<5));
 FUN_8001B9F4(index,2,point);ff_w32(item+4,point[0]);ff_w32(item+8,point[1]);ff_w16(item+28,(uint16)index);ff_w32(item+12,point[2]);return (sint32)point[2];
}
GDB_CALL sint32 FUN_8004EDB8(sint32 index) {
 FF_FUNCTION_MARKER(0x8004edb8u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),slot,offset,dz;
 if(ff_u32(actor+164)!=1||ff_u32(actor+168)==0xffffffffu){if(ff_u32(actor+4)!=0xffffffffu)return -1;return FUN_8004C4D8(actor,table,276);}
 slot=ff_u32(actor+168);FUN_800483BC(index,ff_u32(0x800b4340+244u*slot),ff_u32(0x800b4348+244u*slot));offset=2u*(uint32)(sint32)ff_s16(actor+56);
 ff_w32(actor+40,ff_u32(actor+40)+((uint32)(sint32)ff_s16(0x800802d0+offset)<<5));dz=(uint32)(sint32)ff_s16(0x800806d0+offset)<<5;ff_w32(actor+48,ff_u32(actor+48)-dz);
 if(ff_u32(actor+4)!=0xffffffffu)return (sint32)dz;
 if((sint32)ff_u32(0x800941a0)<448){FUN_80056F08(ff_u32(0x800944c4),0);return FUN_8004C4D8(actor,table,276);}return FUN_8004C4D8(actor,table,275);
}
GDB_CALL sint32 FUN_8004DDEC(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ddecu,"GAME.EXE");
 uint32 state=ff_u32(0x800b4324+244u*(uint32)index),volume,pan;sint32 level;
 level=(state-257u<2u||state-254u<2u||state-266u<2u||state==31||state==3)?16:-16;
 level=(sint32)((uint32)level+ff_u32(0x800944bc));if(level<64)level=64;if(level>192)level=192;ff_w32(0x800944bc,(uint32)level);
 ff_spatial_sound_80046884(0x800b4340+244u*(uint32)index,&volume,&pan);volume*=ff_u32(0x800944bc);
 FUN_800568C8((sint32)ff_u32(0x800944b8),(sint32)volume>>8,(sint32)pan);FUN_80056840((sint32)ff_u32(0x800944b8),level*8+5120);return FUN_80052944(index);
}
GDB_CALL sint32 FUN_8004E574(sint32 index) {
 FF_FUNCTION_MARKER(0x8004e574u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),slot,frame;int attack=0;
 if((sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+176)&&ff_u32(actor+164)==1){slot=ff_u32(actor+168);
  if(slot!=0xffffffffu&&(uint16)((uint32)FUN_800483BC(index,ff_u32(0x800b4340+244u*slot),ff_u32(0x800b4348+244u*slot))+2047u)<4095u&&(sint32)ff_u32(0x800941a0)<1281)attack=1;
 }frame=ff_u32(actor+4);if(frame!=0xffffffffu)return attack?(sint32)frame:-1;
 return FUN_8004C4D8(actor,table,attack?271:272);
}
GDB_CALL sint32 FUN_800553F0(sint32 index) {
 FF_FUNCTION_MARKER(0x800553f0u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),slot; sint32 speed=*(sint8*)ff_ptr(actor+130,1),limit;
 ff_w8(actor+130,(uint8)(speed+4));limit=(sint32)ff_u32(0x80092934);ff_w32(actor+44,ff_u32(actor+44)+((uint32)speed<<16));
 if(limit<*(sint8*)ff_ptr(actor+130,1))ff_w8(actor+130,(uint8)limit);
 if((sint32)ff_u32(actor+44)>=0){slot=ff_u32(actor+172);ff_w32(actor+44,0);if(slot!=0xffffffffu)ff_w16(0x8009a3e2+36u*slot,1);
  FUN_8004C4D8(actor,table,ff_u32(actor+12)==203?204:206);if(ff_u32(0x80093d58)==8)FUN_80056670(229,255,6144);
 }if(ff_u32(actor+4)==0xffffffffu)return FUN_8004C4D8(actor,table,(sint32)ff_u32(actor+12));return -1;
}
GDB_CALL sint32 FUN_800551F0(sint32 index) {
 FF_FUNCTION_MARKER(0x800551f0u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),slot;sint32 state;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;state=FUN_8004C9B0(actor);
 if(state==208)return FUN_8004C4D8(actor,table,210);
 if(state==210){slot=ff_u32(actor+168);
  if(slot!=0xffffffffu&&ff_u32(actor+164)!=4&&ff_u32(0x800b4324+244u*slot)==67)return FUN_8004C4D8(actor,table,210);
  return FUN_8004C4D8(actor,table,209);
 }if(state==209)return FUN_80053558(index);return 209;
}
GDB_CALL sint32 FUN_80053CEC(sint32 index) {
 FF_FUNCTION_MARKER(0x80053cecu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return 4;
 if(ff_u32(actor+164)==4){uint32 slot=ff_u32(actor+168);if(slot!=0xffffffffu){uint32 object=0x8009a3c8+36u*slot;
  if(!ff_u32(object+8)){ff_w8(actor+126,(uint8)slot);if(ff_u32(object)==522)ff_w32(object,523);
   if(ff_u32(object)==524)ff_w32(object,525);ff_w32(object+8,1);}
 }}return FUN_8004C4D8(actor,table,122);
}
GDB_CALL sint32 FUN_8004CD5C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004cd5cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),other;sint32 slot;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;slot=*(sint8*)ff_ptr(actor+122,1);
 if(slot==-1)return FUN_800532B8(index);other=0x800b4318+244u*(uint32)slot;
 FUN_800237CC(other,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52)),77);
 FUN_800237CC(actor,table,169);return FUN_800235C4(actor,other);
}
GDB_CALL sint32 FUN_8004E484(sint32 index) {
 FF_FUNCTION_MARKER(0x8004e484u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));sint32 state;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;state=FUN_8004C9B0(actor);
 if(state==57){ff_w32(actor+4,0xffffffffu);return FUN_8004C4D8(actor,table,60);}
 if(state==60)return FUN_8004C4D8(actor,table,58);
 if(state==59)return FUN_8004C4D8(actor,table,60);
 if(state==58)return FUN_8004F000(index);return 58;
}
GDB_CALL sint32 FUN_8003B83C(sint32 index) {
 FF_FUNCTION_MARKER(0x8003b83cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index;
 if(ff_u32(0x80093d58)!=4||(sint32)ff_u32(0x80093dd0)>=660)return FUN_80052944(index);
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 return FUN_8004C4D8(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),(sint32)ff_u32(actor+200));
}
GDB_CALL sint32 FUN_80054988(sint32 index) {
 FF_FUNCTION_MARKER(0x80054988u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table;if(ff_u32(actor+4)!=0xffffffffu)return -1;
 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(table!=0x800849a4&&table!=0x80083794&&table!=0x80082a08&&table!=0x80082e8c)ff_w16(actor+56,(uint16)ff_s16(actor+56)^1024u);
 return FUN_800532B8(index);
}
GDB_CALL sint32 FUN_80054840(sint32 index) {
 FF_FUNCTION_MARKER(0x80054840u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 if(*(sint8*)ff_ptr(actor+122,1)==-1)return FUN_800532B8(index);
 return FUN_8004C4D8(actor,table,ff_rand_8006D348()&1u?12:13);
}
GDB_CALL sint32 FUN_80053A70(sint32 index) {
 FF_FUNCTION_MARKER(0x80053a70u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));sint32 slot;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;slot=*(sint8*)ff_ptr(actor+126,1);
 if(slot==-1||ff_u32(0x8009a3c8+36u*(uint32)slot)-499u>=5u)return FUN_80053558(index);
 return FUN_8004C4D8(actor,table,(sint32)(ff_rand_8006D348()&1u)+247);
}
GDB_CALL sint32 FUN_80055B68(sint32 x,sint32 z) {
 FF_FUNCTION_MARKER(0x80055b68u,"GAME.EXE");
 uint32 i,object;sint32 kind;x>>=16;z>>=16;
 for(i=0;i<64;i++){object=0x8009a3c8+36u*i;kind=(sint32)ff_u32(object);
  if(kind>=522&&kind<563&&!ff_s16(object+24)&&ff_u32(object+8)!=1&&FUN_800163F4(x,z,ff_s16(object+6),ff_s16(object+14))<64)return (sint32)i;
 }return -1;
}
static sint32 npc_claim_object(sint32 index,uint32 slot_offset,sint32 action) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return 4;
 if(ff_u32(actor+164)==4){uint32 slot=ff_u32(actor+168);if(slot!=0xffffffffu){uint32 object=0x8009a3c8+36u*slot;
  if(!ff_u32(object+8)){ff_w8(actor+slot_offset,(uint8)slot);ff_w32(object+8,1);}
 }}return FUN_8004C4D8(actor,table,action);
}
GDB_CALL sint32 FUN_800539C0(sint32 index) {
 FF_FUNCTION_MARKER(0x800539c0u,"GAME.EXE");
 return npc_claim_object(index,126,119);
}
GDB_CALL sint32 FUN_80053B40(sint32 index) {
 FF_FUNCTION_MARKER(0x80053b40u,"GAME.EXE");
 return npc_claim_object(index,126,124);
}
GDB_CALL sint32 FUN_80053BF0(sint32 index) {
 FF_FUNCTION_MARKER(0x80053bf0u,"GAME.EXE");
 return npc_claim_object(index,125,127);
}
GDB_CALL sint32 FUN_80052F70(sint32 index) {
 FF_FUNCTION_MARKER(0x80052f70u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+12)==171&&*(sint8*)ff_ptr(actor+120,1)==-1)return FUN_8004C4D8(actor,table,(sint32)ff_u32(actor+200));
 return -1;
}
static sint32 npc_action_call(uint32 callback,sint32 index);
GDB_CALL sint32 FUN_8004BE84(sint32 index) {
 FF_FUNCTION_MARKER(0x8004be84u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index;
 sint32 age;
 ff_w32(0x800947ec,(uint32)index);
 age=*(sint8*)ff_ptr(actor+119,1);
 if(age){age=(sint8)(age+1);ff_w8(actor+119,(uint8)age);if(age>=61){ff_w32(actor,0);ff_w16(actor+52,0xffff);return -1;}}
 else if(ff_s16(actor+66)<=0)return FUN_8001B8DC(index);
 return npc_action_call(ff_u32(ff_u32(actor+160)+4u*ff_u32(actor+12)),index);
}
static sint32 npc_finished_to(sint32 index,sint32 action) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return -1;return FUN_8004C4D8(actor,table,action);
}
GDB_CALL sint32 FUN_8004BF3C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004bf3cu,"GAME.EXE");
 return npc_finished_to(index,69);
}
GDB_CALL sint32 FUN_8004ED54(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ed54u,"GAME.EXE");
 return npc_finished_to(index,275);
}
GDB_CALL sint32 FUN_8004D20C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004d20cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,frame=ff_u32(actor+4),table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 ff_w32(actor+228,0x390000);if(frame!=0xffffffffu)return -1;
 ff_w32(actor+224,ff_u32(0x80093dd0)+20u);return FUN_8004C4D8(actor,table,266);
}
GDB_CALL sint32 FUN_8004E3D8(sint32 index) {
 FF_FUNCTION_MARKER(0x8004e3d8u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),result;
 if(ff_s16(actor+80)<=0)return FUN_8004F000(index);
 if(ff_u32(actor+4)==0xffffffffu)FUN_8004C4D8(actor,table,(sint32)ff_u32(actor+12));
 result=(uint16)ff_s16(actor+80)-1u;ff_w16(actor+80,(uint16)result);result<<=16;
 return result?(sint32)result:FUN_8004F000(index);
}
GDB_CALL sint32 FUN_8004ECE0(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ece0u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 FUN_8004C4D8(actor,table,274);return FUN_80056670(187,255,4096);
}
GDB_CALL sint32 FUN_80053F4C(sint32 index) {
 FF_FUNCTION_MARKER(0x80053f4cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return 156;return FUN_8004C4D8(actor,table,ff_u32(actor+12)==156?253:252);
}
static sint32 npc_after_hold(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 if(*(sint8*)ff_ptr(actor+122,1)==-1)return FUN_800532B8(index);return FUN_8004C4D8(actor,table,11);
}
GDB_CALL sint32 FUN_800547B8(sint32 index) {
 FF_FUNCTION_MARKER(0x800547b8u,"GAME.EXE");
 return npc_after_hold(index);
}
GDB_CALL sint32 FUN_80054900(sint32 index) {
 FF_FUNCTION_MARKER(0x80054900u,"GAME.EXE");
 return npc_after_hold(index);
}
GDB_CALL sint32 FUN_8005531C(sint32 index) {
 FF_FUNCTION_MARKER(0x8005531cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if(ff_u32(actor+4)!=0xffffffffu)return -1;return FUN_8004C4D8(actor,table,(sint32)(ff_rand_8006D348()&3u)+243);
}
GDB_CALL sint32 FUN_80055574(sint32 index) {
 FF_FUNCTION_MARKER(0x80055574u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index;if(ff_u32(actor+4)!=0xffffffffu)return -1;
 ff_w32(actor+176,ff_u32(0x80093dd0)+(uint32)(ff_s16(actor+74)>>2));return FUN_80053558(index);
}
GDB_CALL sint32 FUN_80056530(sint32 index) {
 FF_FUNCTION_MARKER(0x80056530u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index;sint32 count=ff_s16(actor+80);
 if(count)ff_w16(actor+80,(uint16)(count-1));if(ff_s16(actor+88)==-1)return FUN_80052944(index);return -1;
}
GDB_CALL sint32 FUN_800565A4(sint32 index) {
 FF_FUNCTION_MARKER(0x800565a4u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if((sint32)ff_u32(0x80094338)<17){FUN_8004C4D8(actor,table,(sint32)ff_u32(actor+200));ff_w16(actor+80,60);return 60;}
 if(ff_u32(actor+4)==0xffffffffu)return FUN_80019F00(actor,table,(sint32)ff_u32(actor+12));return -1;
}
GDB_CALL sint32 FUN_8004C134(sint32 index) {
 FF_FUNCTION_MARKER(0x8004c134u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004BFA0(index);
 return -1;
}
GDB_CALL sint32 FUN_8004C180(sint32 index) {
 FF_FUNCTION_MARKER(0x8004c180u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004BFA0(index);
 return -1;
}
GDB_CALL sint32 FUN_8004D288(sint32 index) {
 FF_FUNCTION_MARKER(0x8004d288u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
GDB_CALL sint32 FUN_8004E38C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004e38cu,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004F000(index);
 return -1;
}
GDB_CALL sint32 FUN_8004EC48(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ec48u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004F084(index);
 return -1;
}
GDB_CALL sint32 FUN_8004EC94(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ec94u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004F084(index);
 return -1;
}
GDB_CALL sint32 FUN_8004EF18(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ef18u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004F084(index);
 return -1;
}
GDB_CALL sint32 FUN_8004EF6C(sint32 index) {
 FF_FUNCTION_MARKER(0x8004ef6cu,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004F084(index);
 return -1;
}
GDB_CALL sint32 FUN_8004F038(sint32 index) {
 FF_FUNCTION_MARKER(0x8004f038u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_8004F084(index);
 return -1;
}
GDB_CALL sint32 FUN_80053CA0(sint32 index) {
 FF_FUNCTION_MARKER(0x80053ca0u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
GDB_CALL sint32 FUN_80053DD0(sint32 index) {
 FF_FUNCTION_MARKER(0x80053dd0u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
GDB_CALL sint32 FUN_800553A4(sint32 index) {
 FF_FUNCTION_MARKER(0x800553a4u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
GDB_CALL sint32 FUN_80055528(sint32 index) {
 FF_FUNCTION_MARKER(0x80055528u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
GDB_CALL sint32 FUN_80055918(sint32 index) {
 FF_FUNCTION_MARKER(0x80055918u,"GAME.EXE");
 if(ff_u32(0x800b431c+244u*(uint32)index)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
GDB_CALL uint32 FUN_8002AEB8(uint32 object) {
 FF_FUNCTION_MARKER(0x8002aeb8u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=36)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_8002AEE8(uint32 object) {
 FF_FUNCTION_MARKER(0x8002aee8u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=18)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_8002AF18(uint32 object) {
 FF_FUNCTION_MARKER(0x8002af18u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=32)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_8002AF48(uint32 object) {
 FF_FUNCTION_MARKER(0x8002af48u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=16)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_8002B2B4(uint32 object) {
 FF_FUNCTION_MARKER(0x8002b2b4u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=17)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_80041974(uint32 object) {
 FF_FUNCTION_MARKER(0x80041974u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=4)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_800419A4(uint32 object) {
 FF_FUNCTION_MARKER(0x800419a4u,"GAME.EXE");
 uint16 age=(uint16)((uint16)ff_s16(object+22)+1u);
 ff_w16(object+22,age);
 if((sint16)age>=32)ff_w32(object,0xffffffffu);
 return 0xffffffffu;
}
GDB_CALL uint32 FUN_8004C4A4(uint32 actor) {
 FF_FUNCTION_MARKER(0x8004c4a4u,"GAME.EXE");
 uint32 action;if(ff_u32(actor+4)==0xffffffffu)return 1;
 action=ff_u32(actor+12);return action==82u||action==87u;
}
/* 4BFA0: approach thresholds depend on current action (216 / 264). */
GDB_CALL sint32 FUN_8004BFA0(sint32 index) {
 FF_FUNCTION_MARKER(0x8004bfa0u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,target=ff_u32(actor+168);
 uint32 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),other;
 sint32 distance,result;
 if(target==0xffffffffu){
  result=(sint32)FUN_8004C4A4(actor);if(!result)return result;
  return FUN_8004C4D8(actor,table,69);
 }
 other=0x800b4318+244u*target;
 distance=FUN_800163F4((sint32)ff_u32(actor+40),(sint32)ff_u32(actor+48),(sint32)ff_u32(other+40),(sint32)ff_u32(other+48))>>16;
 FUN_800483BC(index,ff_u32(other+40),ff_u32(other+48));
 if(distance<(ff_u32(actor+12)?216:264)){
  ff_w32(actor+188,0);ff_w32(actor+164,3);return FUN_8004C4D8(actor,table,0);
 }
 if(distance<640){
  if(ff_u32(actor+4)!=0xffffffffu&&!FUN_8004C464(actor,69))return 0;
  return FUN_8004C4D8(actor,table,82);
 }
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 return FUN_8004C4D8(actor,table,FUN_8001A068(table,87)?87:82);
}
/* 4F084: preserve v0 after distance tests, not just the movement status. */
GDB_CALL sint32 FUN_8004F084(sint32 index) {
 FF_FUNCTION_MARKER(0x8004f084u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 uint32 target,mode,other,desired; sint32 old,result,slot,item,moved,distance;
 if(ff_u32(actor+28)&0x4000u)FUN_80056670(17,240,6144);
 old=FUN_8004C9B0(actor);FUN_800531C4(index);target=ff_u32(actor+168);
 if(target!=0xffffffffu){
  mode=ff_u32(actor+164);
  if(mode==2){
   other=0x800b4318+244u*target;
   result=FUN_80047CE4(index,ff_u32(other+40),ff_u32(other+48),262);if(result)return result;
  } else if(mode==1){
   slot=*(sint8*)ff_ptr(actor+126,1);item=slot==-1?-1:(sint32)ff_u32(0x8009a3c8+36u*(uint32)slot);
   if(item==-1){slot=*(sint8*)ff_ptr(actor+125,1);if(slot!=-1)item=(sint32)ff_u32(0x8009a3c8+36u*(uint32)slot);}
   other=0x800b4318+244u*ff_u32(actor+168);
   if(item==-1){
    moved=FUN_80047CE4(index,ff_u32(other+40),ff_u32(other+48),257);
    other=0x800b4318+244u*ff_u32(actor+168);
    distance=FUN_800163F4(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50));
    result=distance<641;
    if(distance>=1281){result=FUN_8004C4D8(actor,table,273);moved=4;}
    else if(distance>=641){FUN_8004C4D8(actor,table,270);result=FUN_80056670(179,192,6144);moved=4;}
   } else {result=FUN_80047CE4(index,ff_u32(other+40),ff_u32(other+48),261);moved=result;}
   if(!moved)return FUN_800532B8(index);return result;
  } else if(mode==4){
   other=0x8009a3c8+36u*target;
   result=FUN_80047CE4(index,ff_u32(other+4),ff_u32(other+12),256);if(result)return result;
   item=(sint32)ff_u32(0x8009a3c8+36u*ff_u32(actor+168));
   if(item>=522)return FUN_8004C4D8(actor,table,121);
   if(item==509||(uint32)item-507u<2u||item==506)return FUN_8004C4D8(actor,table,123);
   return FUN_8004C4D8(actor,table,item==515||item==504?126:118);
  }
 }
 desired=ff_u32(actor+200);
 if((uint32)old==desired){result=(sint32)ff_u32(actor+4);if(result!=-1)return result;}
 return FUN_8004C4D8(actor,table,(sint32)desired);
}
/* IDA misses forwarded a0 in this call-only entry. */
GDB_CALL sint32 FUN_8004F000(sint32 index) {
 FF_FUNCTION_MARKER(0x8004f000u,"GAME.EXE");
 return FUN_8004F084(index);
}
#include "wip.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define npc_wip_zero(pc) do { if(ff_wip_visit((pc),"npc",__FUNCTION__,__FILE__,__LINE__,1,"return_zero_from_function"))return 0;npc_wip_abort(pc); } while(0)
#define npc_wip_void(pc) do { if(ff_wip_visit((pc),"npc",__FUNCTION__,__FILE__,__LINE__,1,"return_from_function"))return;npc_wip_abort(pc); } while(0)
#define npc_wip_fatal(pc) do { ff_wip_visit((pc),"npc",__FUNCTION__,__FILE__,__LINE__,0,"abort");npc_wip_abort(pc); } while(0)
static void npc_wip_abort(uint32 pc){
 fprintf(stderr,"WIP NPC branch %08X\n",pc);
 if(getenv("FF_CAPTURE_WIP")) {
  FILE *f=fopen("../status/gameplay/game-loop-abort.ram","wb");
  if(f){fwrite(ff_ram,1,sizeof(ff_ram),f);fclose(f);}
  f=fopen("../status/gameplay/game-loop-abort.scratch","wb");
  if(f){fwrite(ff_ptr(0x1f800000,1024),1,1024,f);fclose(f);}
 }
 abort();
}
/* 54474: completed recovery150 turns around, then resumes normal NPC logic. */
GDB_CALL sint32 FUN_80054474(sint32 index) {
 FF_FUNCTION_MARKER(0x80054474u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index;
 if(ff_u32(actor+4)!=0xffffffffu)return 150;
 if(ff_u32(actor+12)==150)ff_w16(actor+56,(uint16)((uint16)ff_s16(actor+56)^1024u));
 return FUN_80053558(index);
}
/* 54398: signed deadline; loop animation while waiting, then recover. */
GDB_CALL sint32 FUN_80054398(sint32 index) {
 FF_FUNCTION_MARKER(0x80054398u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 sint32 state=FUN_8004C9B0(actor);
 if((sint32)ff_u32(0x80093dd0)<(sint32)ff_u32(actor+176)) {
  if(ff_u32(actor+4)==0xffffffffu)return FUN_80019F00(actor,table,state);
  return -1;
 }
 return FUN_8004C4D8(actor,table,state==253?150:(sint32)((ff_rand_8006D348()&2u)|149u));
}
GDB_CALL uint32 FUN_80048964(sint32 index) {
 uint32 p=0x800bf238+264u*(uint32)index,i;for(i=0;i<66;i++)ff_w32(p+4u*i,0xffffffffu);return p+264;
}
GDB_CALL sint32 FUN_8004C9B0(uint32 actor) {
 sint32 state=(sint32)ff_u32(actor+12);
 switch(state){case 79:case 80:case 81:case 70:case 73:case 74:case 76:return (sint32)ff_u32(actor+200);
 case 85:case 83:return (sint32)ff_u32(actor+204);case 100:case 102:return 98;case 105:case 107:return 103;
 case 115:case 117:return 113;case 110:case 112:return 108;case 161:return 159;case 162:return 160;
 case 61:return 57;case 63:return 59;case 64:return 60;case 62:return 58;case 49:return 36;default:return state;}
}
static sint32 npc_remap(uint32 table,sint32 state,sint32 from,sint32 to){return state==from&&FUN_8001A068(table,to)?to:state;}
GDB_CALL sint32 FUN_8004C4D8(uint32 actor,uint32 table,sint32 state) {
 sint32 slot;uint32 type;
 if((state==113||state==117)&&ff_u32(actor+212)==108&&(sint32)ff_u32(0x80093dd0)<(sint32)ff_u32(actor+208))state=(sint32)ff_u32(actor+200);
 if((state==108||state==112)&&ff_u32(actor+212)==113&&(sint32)ff_u32(0x80093dd0)<(sint32)ff_u32(actor+208))state=(sint32)ff_u32(actor+200);
 if(ff_s16(actor+52)==12&&state==36)state=49;
 slot=*(sint8*)ff_ptr(actor+126,1);
 if(slot!=-1){type=ff_u32(0x8009a3c8+36u*(uint32)slot);
  if(type-499u<5u)state=npc_remap(table,state,(sint32)ff_u32(actor+200),79);
  else if(type==477||type==491||type==493)state=npc_remap(table,state,(sint32)ff_u32(actor+200),80);
  else if(type==490)state=npc_remap(table,state,(sint32)ff_u32(actor+200),81);
  else if(type==509||type-507u<2u||type==506)state=npc_remap(table,state,(sint32)ff_u32(actor+200),76);
  else if((sint32)type>=522){
   state=npc_remap(table,state,(sint32)ff_u32(actor+200),70);state=npc_remap(table,state,(sint32)ff_u32(actor+204),85);
   state=npc_remap(table,state,87,85);state=npc_remap(table,state,98,102);state=npc_remap(table,state,103,107);
   state=npc_remap(table,state,113,117);state=npc_remap(table,state,108,112);state=npc_remap(table,state,159,161);state=npc_remap(table,state,160,162);
  }
 }
 if(*(sint8*)ff_ptr(actor+125,1)!=-1){
  state=npc_remap(table,state,(sint32)ff_u32(actor+200),73);state=npc_remap(table,state,(sint32)ff_u32(actor+204),83);
  state=npc_remap(table,state,87,83);state=npc_remap(table,state,98,100);state=npc_remap(table,state,103,105);
  state=npc_remap(table,state,113,115);state=npc_remap(table,state,108,110);
 }
 if(state==87&&!FUN_8001A068(table,87))state=(sint32)ff_u32(actor+204);
 if(state==113||state==117){ff_w32(actor+212,113);ff_w32(actor+208,ff_u32(0x80093dd0)+120u);}
 if(state==108||state==112){ff_w32(actor+212,108);ff_w32(actor+208,ff_u32(0x80093dd0)+120u);}
 if(ff_u32(actor+12)==(uint32)state&&FUN_8004C9B0(actor)==(sint32)ff_u32(actor+200)&&ff_u32(actor+4)!=0xffffffffu)return -1;
 return FUN_80019F00(actor,table,state);
}
/* 4AF18 consumes seven arguments; IDA's eleven-argument draft is incorrect.
 * Stack slots +10/+14/+18 are flag117, timer80 and field8 respectively. */
GDB_CALL sint32 FUN_8004AF18(sint32 type,uint32 x,uint32 z,sint32 yaw,sint32 flag,sint32 timer,uint32 extra) {
 uint32 actor=0,index,stats,hp,table,name,bank,cursor,color;
 for(index=2;index<8;index++){actor=0x800b4318+244u*index;if(ff_s16(actor+52)==-1)break;}if(index==8)return -1;
 memset(ff_ptr(actor,244),0,244);ff_w16(actor+52,(uint16)type);
 ff_w8(actor+122,255);ff_w8(actor+120,255);ff_w8(actor+125,255);ff_w8(actor+126,255);
 stats=ff_u32(0x8007f380+4u*(uint32)(sint32)ff_s16(actor+52));ff_w32(actor+180,stats);ff_w32(actor+172,0xffffffffu);
 hp=8u * *(uint8*)ff_ptr(stats+6,1);ff_w8(actor+117,(uint8)flag);ff_w16(actor+80,(uint16)timer);ff_w32(actor+8,extra);
 ff_w16(actor+66,(uint16)(hp-4u));ff_w16(actor+68,(uint16)hp);ff_w16(actor+72,(uint16)hp);
 table=ff_u32(0x800b3be8+4u*(uint32)type);ff_w16(actor+56,(uint16)yaw);ff_w32(actor+40,x);ff_w32(actor+48,z);ff_w16(actor+88,65535);
 if(type==17){ff_w32(actor+12,0xffffffffu);ff_w32(actor+4,0xffffffffu);}
 else if(type==51){FUN_8004C4D8(actor,table,277);ff_w32(actor+216,0);ff_w8(actor+130,0);ff_w32(actor+220,(uint32)FUN_80056670(143,0,6144)+1u);ff_w32(actor+224,(uint32)FUN_80056670(144,0,6144)+1u);}
 else FUN_8004C4D8(actor,table,69);
 FUN_80048964((sint32)index);type=ff_s16(actor+52);
 ff_w32(actor+200,ff_u32(0x8007f458+8u*(uint32)type));name=ff_u32(0x8007f45c+8u*(uint32)type);color=*(uint8*)ff_ptr(0x800941bc,1);
 ff_w8(actor+159,(uint8)color);ff_w8(actor+158,(uint8)color);ff_w8(actor+157,(uint8)color);ff_w32(actor+204,name);FUN_80019CA4((sint32)index);
 name=ff_u32(0x800b6e90+4u*(uint32)(sint32)ff_s16(actor+52));
 if(name==1||name==2){bank=name==1?0x8008720c:0x80087248;cursor=name==1?0x80093f38:0x80093f3c;name=ff_u32(cursor);ff_w32(actor+240,ff_u32(bank+4u*name));++name;table=ff_u32(bank+4u*name);ff_w32(cursor,name);if(!table)ff_w32(cursor,0);}
 else ff_w32(actor+240,name);
 return (sint32)index;
}

/* Actual MIPS53034, including real expiry dispatch53278->53558. */
GDB_CALL sint32 FUN_80053034(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 uint32 state=ff_u32(actor+12),y,timer; sint32 yaw;
 if(state==287){
  y=ff_u32(actor+44)+(ff_u32(0x80093dac)<<16);ff_w32(actor+44,y);
  if((sint32)y>=0){ff_w16(actor+80,28);ff_w32(actor+44,0);return FUN_8004C4D8(actor,table,288);}
  ff_w16(actor+80,2);
 } else if(state==288){
  yaw=ff_s16(actor+56);
  ff_w32(actor+40,ff_u32(actor+40)+((uint32)(sint32)ff_s16(0x800802d0+2u*(uint32)yaw)<<5));
  ff_w32(actor+48,ff_u32(actor+48)-((uint32)(sint32)ff_s16(0x800806d0+2u*(uint32)yaw)<<5));
 }
 if(!ff_s16(actor+80))return FUN_80053278(index);
 if(ff_u32(actor+4)==0xffffffffu){
  state=ff_u32(actor+12);if(state==282)state=283;else if(state==283)state=284;else if(state==284)state=ff_u32(actor+200);
  FUN_8004C4D8(actor,table,(sint32)state);
 }
 timer=(uint32)(uint16)ff_s16(actor+80)-1u;ff_w16(actor+80,(uint16)timer);timer<<=16;
 if(!timer)return FUN_80053278(index);
 return (sint32)timer;
}

GDB_CALL sint32 FUN_80048844(sint32 index,sint32 target) {
 sint32 i,count=0,role;uint32 actor,wanted=(uint32)target-2u;
 for(i=2;i<8;i++) {
  actor=0x800b4318+244u*(uint32)i;
  if(i==index||ff_s16(actor+52)==-1||ff_s16(actor+66)<=0)continue;
  role=(sint32)ff_u32(actor+164);
  if(target<2){
   if((role==1||role==3)&&ff_u32(actor+168)==(uint32)target)++count;
   if(count>=(sint32)ff_u32(0x80093e18))return i;
  }else {
   if((uint32)(sint32)*(sint8*)ff_ptr(actor+126,1)==wanted)return i;
   if(role==4&&ff_u32(actor+168)==wanted)return i;
  }
 }
 return -1;
}
GDB_CALL sint32 FUN_8004899C(sint32 index) {
 uint32 p=0x800bf238+264u*(uint32)index; sint32 i,best=0,result=-1,value;
 for(i=0;i<66;i++){value=(sint32)ff_u32(p+4u*(uint32)i);if(best<value){best=value;result=i;}}
 return result;
}
GDB_CALL sint32 FUN_800489F4(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,rank=0x800bf238+264u*(uint32)index;
 uint32 other,config,capability,bias,item,bits,angle,numerator; sint32 i,type,denominator;
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++) {
  other=0x800b4318+244u*(uint32)i;
  if(ff_s16(other+52)==-1||ff_s16(other+66)<=0)ff_w32(rank+4u*(uint32)i,0xffffffffu);
  else {
   FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50));
   ff_w32(rank+4u*(uint32)i,0x7fffu-ff_u32(0x800941a0));
  }
 }
 config=ff_u32(actor+180)+8u*ff_u32(actor+184);
 capability=0x80084f84+16u*(uint32)*(uint8*)ff_ptr(config,1);bias=8u*(uint32)*(uint8*)ff_ptr(config+4,1)-1024u;
 for(i=0;i<64;i++) {
  item=0x8009a3c8+36u*(uint32)i;type=(sint32)ff_u32(item);
  if(*(sint8*)ff_ptr(actor+126,1)!=-1||*(sint8*)ff_ptr(actor+125,1)!=-1||type<477||type>=563||ff_s16(item+24)||ff_u32(item+8)==1||ff_s16(item+22)<0)goto ineligible;
  bits=ff_u32(0x800846b8+4u*(uint32)type);
  if((bits&ff_u32(capability+12))!=(bits&65535u)||FUN_80048844(index,i+2)!=-1)goto ineligible;
  angle=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(item+6),ff_s16(item+14))-((uint32)(uint16)ff_s16(actor+56)<<5);
  if(angle&0x8000u)angle=0u-angle;
  denominator=((sint32)(angle<<16)>>20)+8192;
  numerator=(0x7fffu-ff_u32(0x800941a0))<<13;
  /* Signed16 angular fold bounds denominator6144..10239, so DIV traps are unreachable. */
  ff_w32(rank+8u+4u*(uint32)i,(uint32)((sint32)numerator/denominator)+bias);
  if((sint32)ff_u32(0x800941a0)<769)continue;
 ineligible:ff_w32(rank+8u+4u*(uint32)i,0xffffffffu);
 }
 return 0;
}
GDB_CALL sint32 FUN_800531C4(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index; sint32 target,role;
 FUN_800489F4(index);target=FUN_8004899C(index);
 if(target==-1){ff_w32(actor+168,0xffffffffu);return 1;}
 if(target>=2){ff_w32(actor+168,(uint32)target-2u);ff_w32(actor+164,4);return 4;}
 ff_w32(actor+168,(uint32)target);role=FUN_80048844(index,target)==-1?1:2;ff_w32(actor+164,(uint32)role);return role;
}

GDB_CALL sint32 FUN_800483BC(sint32 index,uint32 x,uint32 z) {
 uint32 actor=0x800b4318+244u*(uint32)index,yaw,difference; sint32 delta,step;
 FUN_8004C9B0(actor);
 difference=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),(sint32)x>>16,(sint32)z>>16);
 yaw=(uint16)ff_s16(actor+56);difference-=yaw<<5;delta=(sint16)(uint16)difference;
 step=delta < -1024 ? -32 : delta > 1024 ? 32 : (delta>>5);
 ff_w16(actor+56,(uint16)((yaw+(uint32)step)&2047u));return delta;
}
/* First matching region, strict boundaries. Miss preserves hit globals. */
GDB_CALL sint32 FUN_80019718(uint32 x,uint32 z) {
 sint32 count=(sint32)ff_u32(0x8009479c),px=(sint32)x>>16,pz=(sint32)z>>16;
 uint32 region=0x8009ec70,object,angle,dx,dz,cs,sn; sint32 kind,lx,lz,hx,hz,a,b,side;
 for(;count>0;--count,region+=24) {
  if(ff_s16(region+12)<2)continue;
  kind=ff_s16(region+10);
  if(!kind){
   if(!(ff_s16(region)<px&&px<ff_s16(region+4)&&ff_s16(region+2)<pz&&pz<ff_s16(region+6)))continue;
  }else if(kind==1){
   object=ff_u32(region+20);angle=(uint32)(uint16)ff_s16(object+16)>>5;
   cs=(uint32)(sint32)ff_s16(0x800806d0+2u*angle);sn=(uint32)(sint32)ff_s16(0x800802d0+2u*angle);
   dx=(uint32)px-(uint32)(sint32)ff_s16(object+6);dz=(uint32)pz-(uint32)(sint32)ff_s16(object+14);
   lx=(sint32)(dx*cs+dz*sn)>>15;lz=(sint32)(dz*cs-dx*sn)>>15;
   hx=ff_s16(region);hz=ff_s16(region+2);
   if(!(-hx<lx&&lx<hx&&-hz<lz&&lz<hz))continue;
   a=(sint16)(uint16)FUN_8001627C(lx,lz,0,0);b=(sint16)(uint16)FUN_8001627C(0,0,hx,-hz);
   if(a<b||a>2048-b)side=0;else if(a<1024-b)side=1;else side=a<b+1024?2:3;
   ff_w32(0x8009411c,(uint32)side);
  }else if(FUN_800163F4(px,pz,ff_s16(region+4),ff_s16(region+6))>=ff_s16(region))continue;
  ff_w32(0x80093d04,region);return 1;
 }
 return 0;
}

/* 47CE4 actual MIPS. Calls to6A0F8 omitted: original body is JR RA / NOP. */
GDB_CALL sint32 FUN_80047CE4(sint32 index,uint32 x,uint32 z,uint32 flags) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 uint32 angle,dx0,dz0,dx1,dz1,mask,other,diff,weight,sum=0,threshold; sint32 state,distance,nearby,base,i,fold,product;
 state=FUN_8004C9B0(actor);
 if((state==113||state==108||state==98||state==103||ff_u32(actor+12)==91)&&ff_u32(actor+4)!=0xffffffffu){FUN_800483BC(index,x,z);return 4;}
 distance=FUN_800163F4(ff_s16(actor+42),ff_s16(actor+50),(sint32)x>>16,(sint32)z>>16);if(!distance)distance=1;
 if(distance>=129){
  angle=((uint32)(sint32)ff_s16(actor+56)-176u)&2047u;
  dx0=(uint32)(sint32)ff_s16(0x800802d0+2u*angle)<<8;dz0=0u-((uint32)(sint32)ff_s16(0x800806d0+2u*angle)<<8);
  mask=FUN_80019718(ff_u32(actor+40)+dx0,ff_u32(actor+48)+dz0)?2u:0u;
  angle=((uint32)(sint32)ff_s16(actor+56)+176u)&2047u;
  dx1=(uint32)(sint32)ff_s16(0x800802d0+2u*angle)<<8;dz1=0u-((uint32)(sint32)ff_s16(0x800806d0+2u*angle)<<8);
  if(FUN_80019718(ff_u32(actor+40)+dx1,ff_u32(actor+48)+dz1))mask|=4u;
  if(mask){
   flags&=~4u;
   if(mask==4||mask==5||mask==6){x=ff_u32(actor+40)+dx0;z=ff_u32(actor+48)+dz0;}
   else{x=ff_u32(actor+40)+dx1;z=ff_u32(actor+48)+dz1;}
   FUN_800483BC(index,x,z);
  }
 }
 FUN_800483BC(index,x,z);
 if(!(flags&256u)&&(ff_u32(actor+212)!=32768u||(sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+208))){
  angle=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),(sint32)x>>16,(sint32)z>>16);
  nearby=(sint32)ff_u32(0x800941a0);
  if(nearby<640&&ff_u32(actor+164)!=4&&ff_u32(actor+168)!=0xffffffffu){
   base=nearby?nearby:1;
   for(i=2;i<8;i++){
    other=0x800b4318+244u*(uint32)i;
    if(i==index||ff_s16(other+52)==-1||ff_s16(other+66)<=0||ff_u32(other+164)==4||ff_u32(other+168)==0xffffffffu)continue;
    diff=angle-(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50));
    fold=(sint16)(uint16)((diff&0x8000u)?diff-16384u:16384u-diff);
    weight=(uint32)base-ff_u32(0x800941a0);if((sint32)weight<0)weight=0;
    product=(sint32)((uint32)fold*weight);
    if(base==-1&&(uint32)product==0x80000000u)npc_wip_fatal(0x80048100);
    sum+=(uint32)(product/base);
   }
   mask=((uint32)(uint16)ff_s16(actor+56)+512u)&2047u;
   dx0=(uint32)(sint32)ff_s16(0x800802d0+2u*mask)<<8;dz0=0u-((uint32)(sint32)ff_s16(0x800806d0+2u*mask)<<8);
   if((sint32)sum < -768){
    if(!FUN_80019718(ff_u32(actor+40)-dx0,ff_u32(actor+48)-dz0)){FUN_8004C4D8(actor,table,108);return 3;}
   }else if((sint32)sum>=769){
    if(!FUN_80019718(ff_u32(actor+40)+dx0,ff_u32(actor+48)+dz0)){FUN_8004C4D8(actor,table,113);return 3;}
   }
  }
 }
 if(ff_s16(actor+52)==48&&distance>=641&&ff_u32(actor+168)!=0xffffffffu)FUN_8004C4D8(actor,table,267);
 mask=flags&3u;threshold=!mask?108u:mask==1?((flags&256u)?344u:216u):mask==2?640u:1280u;
 if((sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+176)&&!(flags&256u)&&distance<(sint32)(threshold>>1)){
  FUN_8004C4D8(actor,table,103);return 3;
 }
 if(distance<(sint32)threshold)return 0;
 if(distance<640&&(flags&4u)){
  if(ff_u32(actor+4)==0xffffffffu||(uint32)state!=ff_u32(actor+204))FUN_8004C4D8(actor,table,(sint32)ff_u32(actor+204));
  return 1;
 }
 if(ff_u32(actor+4)!=0xffffffffu)return 4;
 FUN_8004C4D8(actor,table,FUN_8001A068(table,87)?87:(sint32)ff_u32(actor+204));return 2;
}

GDB_CALL sint32 FUN_800292F8(sint32 index){
 FF_FUNCTION_MARKER(0x800292f8u,"GAME.EXE");
 sint32 type=ff_s16(0x800b434c+244u*(uint32)index),sound=47,pitch=6144;
 switch(type){
 case 1:case 12:case 29:case 30:case 38:sound=67;break;
 case 2:case 32:sound=83;break;case 3:sound=103;break;case 48:sound=195;break;
 case 49:sound=176;break;case 14:sound=83;pitch=5888;break;
 }
 return FUN_80056670(sound,255,pitch);
}
/* 47AC0: mutual facing and target-state gate for the paired grapple animations. */
/* 4ADA8: hold partner opposite the actor; collision correction releases the link. */
GDB_CALL sint32 FUN_8004ADA8(sint32 index,sint32 victim){
 FF_FUNCTION_MARKER(0x8004ada8u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,other=0x800b4318+244u*(uint32)victim;
 uint32 state,distance,sine,cosine,x,z,original_x,original_z;
 if(*(sint8*)ff_ptr(other+121,1)==1)return 2;
 if(*(sint8*)ff_ptr(actor+121,1)==2)return 172;
 state=ff_u32(other+12);if(state==172||state==77)return 77;
 distance=(uint32)(sint32)ff_s16(0x800944e0+2u*(uint32)(sint32)ff_s16(actor+52));
 ff_w16(other+56,(uint16)((uint16)ff_s16(actor+56)^1024u));
 sine=(uint32)(sint32)ff_s16(0x800802d0+2u*(uint32)(sint32)ff_s16(actor+56));
 cosine=(uint32)(sint32)ff_s16(0x800806d0+2u*(uint32)(sint32)ff_s16(actor+56));
 original_x=x=ff_u32(actor+40)+distance*sine;original_z=z=ff_u32(actor+48)-distance*cosine;
 ff_collision_candidate(other,&x,&z);
 if(x!=original_x||z!=original_z){ff_w8(other+120,255);ff_w8(actor+122,255);return -1;}
 ff_w32(other+40,x);ff_w32(other+48,z);return (sint32)z;
}
GDB_CALL sint32 FUN_80047AC0(sint32 index,sint32 victim){
 FF_FUNCTION_MARKER(0x80047ac0u,"GAME.EXE");
 uint32 actor,other,table,other_table,angle,state;if(victim==-1)return 0;
 actor=0x800b4318+244u*(uint32)index;other=0x800b4318+244u*(uint32)victim;
 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 angle=FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50));
 if((uint16)(angle-((uint32)(uint16)ff_s16(actor+56)<<5)+4096u)>=8193u)return 0;
 angle=FUN_8001614C(ff_s16(other+42),ff_s16(other+50),ff_s16(actor+42),ff_s16(actor+50));
 if((uint16)(angle-((uint32)(uint16)ff_s16(other+56)<<5)+4096u)>=8193u)return 0;
 if((sint32)ff_u32(0x800941a0)>=257||!ff_s16(other+66)||*(sint8*)ff_ptr(other+120,1)!=-1)return 0;
 state=FUN_8004C9B0(other);if(state!=69&&state!=82&&state!=103&&state!=113&&state!=108)return 0;
 ff_w8(other+120,(uint8)index);ff_w8(other+121,0);ff_w8(actor+122,(uint8)victim);ff_w8(other+123,127);FUN_800292F8(index);
 other_table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(other+52));
 if(index<2)FUN_800237CC(actor,table,166);else FUN_8004C4D8(actor,table,166);
 if(victim<2)FUN_800237CC(other,other_table,171);else FUN_8004C4D8(other,other_table,171);return 1;
}
/* Actual48488: reject grapple/attachment and unavailable target states. */
GDB_CALL sint32 FUN_80048488(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index;sint32 state;
 if(*(sint8*)ff_ptr(actor+120,1)!=-1||*(sint8*)ff_ptr(actor+122,1)!=-1)return 0;
 state=FUN_8002371C(actor);return state!=33&&state!=158&&state!=39&&state!=149&&state!=150&&state!=151;
}
GDB_CALL sint32 FUN_80048530(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 uint32 script=0x80084f84+16u**(uint8*)ff_ptr(ff_u32(actor+180)+8u*ff_u32(actor+184),1),cursor,commands,next; sint32 slot,pick,count,random,eligible,result;
 eligible=(sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+176)&&ff_u32(actor+164)!=4&&ff_u32(actor+168)!=0xffffffffu;
 if(!eligible){next=ff_u32(actor+200);if(ff_u32(actor+12)==next&&ff_u32(actor+4)!=0xffffffffu)return -1;return FUN_8004C4D8(actor,table,(sint32)next);}
 if(!FUN_80048488((sint32)ff_u32(actor+168))){next=ff_u32(actor+200);if(ff_u32(actor+12)==next&&ff_u32(actor+4)!=0xffffffffu)return (sint32)ff_u32(actor+4);return FUN_8004C4D8(actor,table,(sint32)next);}
 random=(sint32)(ff_rand_8006D348()&3u);cursor=ff_u32(actor+188);commands=ff_u32(script+8);
 if(random<(sint32)ff_u32(commands+4u*cursor))ff_w32(actor+188,cursor+1u);
 else for(;;){
  slot=*(sint8*)ff_ptr(actor+126,1);if(slot==-1)slot=*(sint8*)ff_ptr(actor+125,1);
  random=(sint32)ff_rand_8006D348();count=(sint32)ff_u32(script);if(!count||(random==(-2147483647-1)&&count==-1))npc_wip_fatal(0x800486bc);
  pick=(sint32)ff_u32(ff_u32(script+4)+4u*(uint32)(random%count));ff_w32(actor+188,(uint32)pick);
  if(pick<0){ff_w32(actor+188,(uint16)pick);if(slot==-1||!(ff_u32(0x800846b8+4u*ff_u32(0x8009a3c8+36u*(uint32)slot))&((uint32)(pick>>16)&0x3fffu)))ff_w32(actor+188,0xffffffffu);}
  else if(slot!=-1)ff_w32(actor+188,0xffffffffu);
  else if(ff_u32(commands+4u*(uint32)pick)==166){if(!FUN_80047AC0(index,(sint32)ff_u32(actor+168)))ff_w32(actor+188,0xffffffffu);}
  if(ff_u32(actor+188)!=0xffffffffu)break;
 }
 cursor=ff_u32(actor+188);ff_w32(actor+188,cursor+1u);next=ff_u32(commands+4u*cursor);
 if(next==69)next=ff_u32(actor+200);
 if(ff_u32(commands+4u*cursor)!=69||ff_u32(actor+12)!=next||ff_u32(actor+4)==0xffffffffu)FUN_8004C4D8(actor,table,(sint32)next);
 cursor=ff_u32(actor+188);ff_w32(actor+188,cursor+1u);result=(sint32)(ff_u32(0x80093dd0)+ff_u32(ff_u32(script+8)+4u*cursor));ff_w32(actor+176,(uint32)result);return result;
}
GDB_CALL sint32 FUN_800532B8(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),other,item,type; sint32 angle,slot;
 FUN_8004C9B0(actor);other=0x800b4318+244u*ff_u32(actor+168);angle=FUN_800483BC(index,ff_u32(other+40),ff_u32(other+48));
 if((uint16)((uint32)angle+4096u)>=8193u){ff_w32(actor+176,ff_u32(0x80093dd0)+5u);return FUN_80048530(index);}
 slot=*(sint8*)ff_ptr(actor+126,1);
 if(slot!=-1&&(sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+176)&&(uint16)((uint32)angle+2047u)<4095u){
  item=0x8009a3c8+36u*(uint32)slot;type=ff_u32(item);
  if(type-499u<5u){if(ff_s16(item+22)<=0)return FUN_8004C4D8(actor,table,159);ff_w16(item+22,(uint16)(ff_s16(item+22)-1));return FUN_8004C4D8(actor,table,270);}
  if(type==506||type==509||type-507u<2u){if(ff_s16(item+22)<=0)return FUN_8004C4D8(actor,table,159);npc_wip_zero(0x80053444);}
 }
 slot=*(sint8*)ff_ptr(actor+125,1);
 if(slot!=-1&&(sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+176)&&(uint16)((uint32)angle+2047u)<4095u){
  item=0x8009a3c8+36u*(uint32)slot;
  if(ff_u32(item)==515){
   uint32 deadline;
   if(ff_s16(item+22)<=0)return FUN_8004C4D8(actor,table,159);
   ff_w16(item+22,(uint16)((uint16)ff_s16(item+22)-1u));FUN_800237CC(actor,table,193);FUN_80030310(index);
   deadline=ff_u32(0x80093dd0)+150u;ff_w32(actor+176,deadline);return (sint32)deadline;
  }
 }
 return FUN_80048530(index);
}
/* 5392C: state270 starts the shot once, then waits for its animation. */
GDB_CALL sint32 FUN_8005392C(sint32 index){
 FF_FUNCTION_MARKER(0x8005392cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),deadline;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 FUN_8004C4D8(actor,table,192);FUN_80030440(index);
 deadline=ff_u32(0x80093dd0)+120u;ff_w32(actor+176,deadline);return (sint32)deadline;
}
GDB_CALL sint32 FUN_80054A40(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,state=ff_u32(actor+12),table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 if((state==51||state==53)&&(ff_u32(actor+28)&0x3000u)==0x3000u)ff_w8(actor+156,*(uint8*)ff_ptr(actor+156,1)|8);
 if(ff_u32(actor+4)!=0xffffffffu)return 28;
 if(state==28&&(table==0x80082a08||table==0x80082e8c||table==0x80083794||table==0x800849a4))return FUN_8004C4D8(actor,table,149);
 return FUN_800532B8(index);
}
/* 53558 behavior selection. Close-range attack532B8 remains explicit WIP. */
GDB_CALL sint32 FUN_80053558(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),target,role,other,type;
 sint32 state,result,slot,next;
 state=FUN_8004C9B0(actor);FUN_800531C4(index);target=ff_u32(actor+168);
 if(target!=0xffffffffu){
  role=ff_u32(actor+164);
  if(role==2){
   other=0x800b4318+244u*target;result=FUN_80047CE4(index,ff_u32(other+40),ff_u32(other+48),6);if(result)return result;
  }else if(role==1){
   type=0xffffffffu;slot=*(sint8*)ff_ptr(actor+126,1);if(slot!=-1)type=ff_u32(0x8009a3c8+36u*(uint32)slot);
   if(type==0xffffffffu){slot=*(sint8*)ff_ptr(actor+125,1);if(slot!=-1)type=ff_u32(0x8009a3c8+36u*(uint32)slot);}
   other=0x800b4318+244u*ff_u32(actor+168);
   if(type-499u<5u||type==515||type==506||type==509||type-507u<2u||type-521u>=42u){
    result=FUN_80047CE4(index,ff_u32(other+40),ff_u32(other+48),5);
    if(!result)return FUN_800532B8(index);return result;
   }
   result=FUN_80047CE4(index,ff_u32(other+40),ff_u32(other+48),6);if(result)return result;
   other=0x800b4318+244u*ff_u32(actor+168);
   result=((uint32)FUN_800483BC(index,ff_u32(other+40),ff_u32(other+48))+2047u)&65535u;result=(uint32)result<4095u;
   if(result)result=FUN_8004C4D8(actor,table,159);return result;
  }else if(role==4){
   other=0x8009a3c8+36u*target;result=FUN_80047CE4(index,ff_u32(other+4),ff_u32(other+12),0);if(result)return result;
   type=ff_u32(0x8009a3c8+36u*ff_u32(actor+168));
   next=(sint32)type>=522?121:type==509||type-507u<2u||type==506?123:type==515||type==504?126:118;
   return FUN_8004C4D8(actor,table,next);
  }
 }
 next=(sint32)ff_u32(actor+200);if(state==next&&ff_u32(actor+4)!=0xffffffffu)return (sint32)ff_u32(actor+4);
 return FUN_8004C4D8(actor,table,next);
}
/* Forwarding wrapper; normalized twins differ in callee targets. */
GDB_CALL sint32 FUN_80053278(sint32 index){return FUN_80053558(index);}

GDB_CALL sint32 FUN_80048CFC(uint32 state){
 return state==147||state==154||state-149u<2u||state==41||state==132||state-177u<3u||state-180u<10u||state-130u<2u||state==77||state==167||state==169||state==33||state==148||state==205||state==203||state==172||state==36;
}
GDB_CALL sint32 FUN_80052878(sint32 index){
 sint32 type=ff_s16(0x800b434c+244u*(uint32)index),sample;
 if(type==49)sample=177;else if(type==14)sample=210;else sample=(type==32?76:33)+(sint32)ff_rand_8006D348()%3;
 return FUN_80056670(sample,255,6144);
}
GDB_CALL uint32 FUN_8001F41C(uint32 attacker,uint32 victim,uint32 state){
 uint32 damage=(uint16)ff_s16(0x800b39a0+2u*state)>>8;
 if(attacker<9u&&victim<9u)return FUN_8001F230((sint32)attacker,(sint32)victim,(sint32)damage);
 return victim<9u;
}
/* 29530: death voice. Actual MIPS uses two RNG draws except fallback
 * voice selection, which consumes a third. No indexed reuse candidates. */
GDB_CALL uint32 FUN_80029530(sint32 index) {
 sint32 type=ff_s16(0x800b434cu+244u*(uint32)index),sound,pitch=5952;
 sint32 volume=192-(sint32)(ff_rand_8006D348()&63u);
 switch(type) {
 case 0:sound=42;break;case 1:sound=69;break;case 2:sound=82;break;
 case 3:sound=104;break;case 48:sound=193;break;
 case 12:case 29:case 30:case 38:sound=14;break;
 case 49:case 50:sound=174;volume=255;break;
 case 32:sound=160;volume=255;break;
 case 14:sound=174;volume=255;pitch=5696;break;
 default:sound=(sint32)(ff_rand_8006D348()&1u)+12;break;
 }
 pitch+=(sint32)(ff_rand_8006D348()&255u);
 return FUN_80056670(sound,volume,pitch);
}
/* Original hit-test, including reached lethal hit. High-stun path remains WIP. */
GDB_CALL sint32 FUN_80048DBC(sint32 index,sint32 attachment){
 uint32 position[3],victim,source=0x800b4318+244u*(uint32)index,x,z,table,angle,state,step; sint32 i,type,j,partner; sint16 direction;
 FUN_8001B9F4(index,attachment,position);
 for(i=0;i<2;i++){
  if(i==index)continue;victim=0x800b4318+244u*(uint32)i;type=ff_s16(victim+52);
  if(type==-1||FUN_80048CFC(ff_u32(victim+12)))continue;
  FUN_8001614C(ff_s16(victim+42),ff_s16(victim+50),(sint32)position[0]>>16,(sint32)position[2]>>16);
  if(ff_s16(0x80094120+2u*(uint32)type)<(sint32)ff_u32(0x800941a0))continue;
  direction=(sint16)(((uint32)(uint16)ff_s16(victim+56)<<5)-(uint32)FUN_8001614C(ff_s16(victim+42),ff_s16(victim+50),ff_s16(source+42),ff_s16(source+50))-16384u);
  x=ff_u32(victim+40);z=ff_u32(victim+48);
  FUN_8001697C(865,x,position[1],z);FUN_80017C78(i);FUN_800293D8(i);
  angle=(uint32)(sint32)ff_s16(victim+56)^1024u;
  for(j=0;j<4;j++)FUN_800310B8(x,position[1],z,angle);
  for(j=0;j<4;j++)FUN_8003100C(x,position[1],z,angle);
  ff_w16(victim+98,0);FUN_8001F41C((uint32)index,(uint32)i,ff_u32(source+12));
  if(attachment==3&&(uint32)(uint16)ff_s16(source+52)-40u<4u){ff_w16(victim+74,127);FUN_80056670(185,255,6144);FUN_80056670(171,255,6144);ff_w32(victim+232,ff_u32(victim+232)|1u);ff_w32(victim+176,ff_u32(0x80093dd0)+20u);}
  if(ff_s16(victim+66)<=0) {
   FUN_80029530(i);ff_w16(victim+66,0);ff_w32(0x8009466c,(uint32)i);FUN_80017E30(i);
   FUN_80019F00(victim,ff_u32(0x800b3be8+4u*(uint32)type),direction<0?147:154);
   partner=*(sint8*)ff_ptr(victim+122,1);if(partner!=-1){ff_w8(0x800b4390+244u*(uint32)partner,255);ff_w8(victim+122,255);}return 1;
  }
  if(ff_s16(victim+74)>=49)npc_wip_zero(0x80049080);
  table=ff_u32(0x800b3be8+4u*(uint32)type);state=ff_u32(source+12);
  if(direction<0&&(state==11||state==40)){
   FUN_80019F00(victim,table,139);step=state==11?64u:112u;angle=2u*(uint32)(sint32)ff_s16(victim+56);
   ff_w32(victim+40,ff_u32(victim+40)-step*(uint32)(sint32)ff_s16(0x800802d0+angle));ff_w32(victim+48,ff_u32(victim+48)+step*(uint32)(sint32)ff_s16(0x800806d0+angle));
  }else FUN_80019F00(victim,table,(sint32)((ff_rand_8006D348()&1u)+(direction<0?133u:152u)));
  partner=*(sint8*)ff_ptr(victim+122,1);if(partner!=-1){ff_w8(0x800b4390+244u*(uint32)partner,255);ff_w8(victim+122,255);}return 1;
 }
 return 0;
}
/* 52944 entrance and event-free52FE8 dispatch. Other events/callbacks remain WIP. */
/* 53E1C: detach, wait for animation completion, then recover or die.
 * The no-partner/no-completion return is -16 (SLL in branch delay slot). */
GDB_CALL sint32 FUN_80053E1C(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index;
 uint32 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 sint32 partner=*(sint8*)ff_ptr(actor+122,1),result=(sint32)((uint32)partner<<4);
 if(partner!=-1){result=(sint32)(244u*(uint32)partner);ff_w8(0x800b4390+(uint32)result,255);ff_w8(actor+122,255);}
 if(ff_u32(actor+4)!=0xffffffffu)return result;
 if(ff_s16(actor+66)<=0){
  ff_w32(0x80093d24,ff_u32(0x80093d24)+1u);FUN_80017E30(index);
  result=FUN_8004C4D8(actor,table,147);ff_w32(actor+168,0xffffffffu);return result;
 }
 if(ff_s16(actor+74)<33)return FUN_80053558(index);
 FUN_8004C4D8(actor,table,103);ff_w32(actor+168,0xffffffffu);
 result=(sint32)(ff_u32(0x80093dd0)+30u);ff_w32(actor+176,(uint32)result);return result;
}
/* 53FC4: knockdown bookkeeping and recovery transitions.
 * Evidence: status/gameplay/npc-53fc4-mips-audit.md. */
GDB_CALL sint32 FUN_80053FC4(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 sint32 partner,state,hp,timer,product,divisor,result;
 uint16 angle;
 uint32 tick;
 ff_w32(0x80093d24,ff_u32(0x80093d24)+1u);
 ff_w32(actor+168,0xffffffffu);
 state=FUN_8004C9B0(actor);
 partner=*(sint8*)ff_ptr(actor+122,1);
 if(partner!=-1){ff_w8(0x800b4390+244u*(uint32)partner,255);ff_w8(actor+122,255);}
 result=(sint32)(ff_u32(actor+124)&0xffff00u);
 if(result!=0xffff00)result=FUN_80017E30(index);
 if(ff_u32(actor+4)!=0xffffffffu)return result;
 hp=ff_s16(actor+66);if(hp<=0)return hp;
 tick=ff_u32(0x80093dd0);
 timer=(sint32)ff_u32(actor+176);
 if(timer< (sint32)tick){
  uint8 factor=(uint8)*(uint8*)ff_ptr(ff_u32(actor+180)+8u*ff_u32(actor+184)+2,1);
  ff_w32(actor+168,0xffffffffu); /* 540D8: repeat after the callees. */
  product=(sint32)factor*(sint32)(ff_s16(actor+72)-ff_s16(actor+66));
  divisor=(sint32)ff_s16(actor+72);
  if(!divisor)npc_wip_fatal(0x80054110);
  if(divisor==-1&&(uint32)product==0x80000000u)npc_wip_fatal(0x80054128);
  ff_w32(actor+176,(tick+(uint32)((product/divisor)+1)));
  return (sint32)ff_u32(actor+176);
 }
 if(state==0xb5){angle=(uint16)ff_s16(actor+56);ff_w16(actor+56,angle^0x400u);return FUN_8004C4D8(actor,table,0xfc);}
 if(state==0xb7){
  if(table==0x80082a08||table==0x80082e8c||table==0x80083794||table==0x800849a4)return FUN_80053558(index);
  return FUN_8004C4D8(actor,table,0xfd);
 }
 if(state==0x8e){
  if(table==0x80082a08||table==0x80082e8c||table==0x80083794||table==0x800849a4)return FUN_8004C4D8(actor,table,0xfc);
  return FUN_8004C4D8(actor,table,0xfd);
 }
 if(state==0x8c){
  if(table==0x80082a08||table==0x80082e8c||table==0x80083794||table==0x800849a4)return FUN_8004C4D8(actor,table,0xfc);
  ff_w16(actor+56,(uint16)ff_s16(actor+56)^0x400u);
  return FUN_8004C4D8(actor,table,0xfd);
 }
 if(state==0x9a || ((uint32)state-0xb6u<2u) || state==0xf1)return FUN_8004C4D8(actor,table,0xfd);
 if(state==0x8d || state==0xb8){
  angle=(uint16)ff_s16(actor+56);
  angle=(uint16)(((uint32)(angle-0x200u))&0x7ffu);
  ff_w16(actor+56,angle);
  if(state==0x8d)ff_w16(actor+56,angle^0x400u);
  return FUN_8004C4D8(actor,table,0xfc);
 }
 if(state==0xbc || state==0x94){
  angle=(uint16)ff_s16(actor+56);
  angle=(uint16)(((uint32)(angle+0x200u))&0x7ffu);
  ff_w16(actor+56,angle);
  return FUN_8004C4D8(actor,table,0xfd);
 }
 if(state==0xbd){ff_w16(actor+56,((uint16)ff_s16(actor+56)-0x200u)&0x7ffu);return FUN_8004C4D8(actor,table,0xfc);}
 if(state==0x119){ff_w32(actor+44,0u);return FUN_8004C4D8(actor,table,0xfc);}
 return FUN_8004C4D8(actor,table,0xfc);
}
/* 55100: defensive animation chain. Indexed twin4E484 differs final callee;
 * this mapping follows current MIPS, including return58 on other endedstates. */
GDB_CALL sint32 FUN_80055100(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index;
 uint32 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52));
 sint32 state;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 state=FUN_8004C9B0(actor);
 if(state==57){ff_w32(actor+4,0xffffffffu);return FUN_8004C4D8(actor,table,60);}
 if(state==60)return FUN_8004C4D8(actor,table,58);
 if(state==59)return FUN_8004C4D8(actor,table,60);
 if(state==58)return FUN_80053558(index);
 return 58;
}
/* 55770: low-health stagger/recovery, actual signed timer comparison and
 * halfword HP addition. The return retains the pre-clamp HP value. */
GDB_CALL sint32 FUN_80055770(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),state;
 sint32 maximum,product,value;
 if(ff_u32(actor+4)!=0xffffffffu)return -1;
 state=ff_u32(actor+12);
 if(state-234u<2u){
  maximum=ff_s16(actor+72);ff_w32(actor+168,0xffffffffu);
  product=(sint32)((uint32)*(uint8*)ff_ptr(ff_u32(actor+180)+8u*ff_u32(actor+184)+2,1)*(uint32)(maximum-ff_s16(actor+66)));
  if(!maximum)npc_wip_fatal(0x80055818); /* Original BREAK7 division trap. */
  if(maximum==-1&&(uint32)product==0x80000000u)npc_wip_fatal(0x80055830);
  ff_w32(actor+176,ff_u32(0x80093dd0)+(uint32)(product/maximum)+1u);
  return FUN_8004C4D8(actor,table,236);
 }
 if(state==236)return FUN_8004C4D8(actor,table,237);
 if(state==237)return FUN_8004C4D8(actor,table,238);
 if(state==238){
  if((sint32)ff_u32(0x80093dd0)<(sint32)ff_u32(actor+176))return FUN_8004C4D8(actor,table,238);
  FUN_8004C4D8(actor,table,239);maximum=ff_s16(actor+72);value=(sint16)((uint16)ff_s16(actor+66)+32u);
  ff_w16(actor+66,(uint16)value);if(maximum<value)ff_w16(actor+66,(uint16)maximum);return value;
 }
 if(state==239)return FUN_80053558(index);
 return 239;
}
/* 29684: body-impact sound. Exact type map and one RNG pitch draw. */
GDB_CALL uint32 FUN_80029684(sint32 index){
 sint32 type=ff_s16(0x800b434c+244u*(uint32)index),sound=50;
 switch(type){case 1:case 12:case 48:case 29:case 30:case 38:sound=68;break;case 3:sound=101;break;case 49:sound=168;break;case 14:sound=209;break;}
 return FUN_80056670(sound,255,6144-(sint32)(ff_rand_8006D348()&255u));
}
/* 332BC intentionally applies damage/attribution twice, around impact sound. */
GDB_CALL sint32 FUN_800332BC(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index;
 FUN_80031C50(606,ff_u32(actor+40),0,ff_u32(actor+48));
 FUN_8001F41C((uint32)(sint32)*(sint8*)ff_ptr(actor+129,1),(uint32)index,ff_u32(actor+12));
 if(index>=2)FUN_8001B850(*(sint8*)ff_ptr(actor+129,1),index);
 FUN_80029684(index);
 FUN_8001F41C((uint32)(sint32)*(sint8*)ff_ptr(actor+129,1),(uint32)index,ff_u32(actor+12));
 return index<2?1:FUN_8001B850(*(sint8*)ff_ptr(actor+129,1),index);
}
/* 52C0C..52CE8: captured actor maintenance; continue at52CEC afterwards. */
static void npc_captured_update(uint32 actor,uint32 table,sint32 index) {
 uint32 state;
 if((ff_u32(actor+124)&0x00ffff00u)!=0x00ffff00u)FUN_80017E30(index);
 if((sint32)ff_u32(0x80093dd0)>=(sint32)ff_u32(actor+176)) {
  uint8 counter=(uint8)(*(uint8*)ff_ptr(actor+123,1)-1u);
  ff_w8(actor+123,counter);
  if((counter&128u)&&ff_u32(actor+12)==171) {
   FUN_8004C4D8(actor,table,36);return;
  }
 }
 if(ff_u32(actor+12)!=139)FUN_8004ADA8(*(sint8*)ff_ptr(actor+120,1),index);
 if(ff_u32(actor+4)!=0xffffffffu)return;
 state=ff_u32(actor+12);
 if(state==172)FUN_8004C4D8(actor,table,77);
 else if(state==77)ff_w32(actor+4,0);
 else FUN_8004C4D8(actor,table,171);
}
GDB_CALL sint32 FUN_80052944(sint32 index) {
 uint32 actor=0x800b4318+244u*(uint32)index,events,callback; sint32 type=ff_s16(actor+52),hp,result;
 uint32 table=ff_u32(0x800b3be8+4u*(uint32)type); /* original saved S0 */
 ff_w32(0x800947ec,(uint32)index);
 if(type==48&&!(ff_u32(actor+232)&2u))npc_wip_zero(0x800529c4);
 if(*(sint8*)ff_ptr(actor+119,1)){
  /* 52A4C..52B44: signed byte death age; skip ordinary AI/health paths. */
  sint32 age=(sint8)(*(uint8*)ff_ptr(actor+119,1)+1u);ff_w8(actor+119,(uint8)age);
  if(age>=61){
   uint16 phase=(uint16)ff_s16(0x80092930);uint32 kind;
   ff_w32(actor,0);ff_w16(actor+52,65535);
   if(phase&1u){
    if(*(sint8*)ff_ptr(0x8007f840+((phase>>1)&7u),1))kind=590u+(ff_rand_8006D348()&1u);
    else kind=(uint32)((sint32)ff_rand_8006D348()%4)+593u;
    FUN_80016654(kind,ff_u32(actor+40),0,ff_u32(actor+48));
   }
   ff_w16(0x80092930,(uint16)((uint16)ff_s16(0x80092930)+1u));
  }
  if(ff_u32(actor+28)&0x1000u){
   callback=ff_u32(0x800bb9b0+4u*ff_u32(actor+12));
   ff_actor_event_call(callback,index);
   result=(uint16)ff_s16(actor+84)+1;ff_w16(actor+84,(uint16)result);return result;
  }
  return 0; /* ANDI event1000 result on direct52F4C path. */
 }
 hp=ff_s16(actor+66);
 if(hp<=0){
  sint32 partner=*(sint8*)ff_ptr(actor+122,1);
  if(partner!=-1){ff_w8(0x800b4390+244u*(uint32)partner,255);ff_w8(actor+122,255);}
  return FUN_8001B8DC(index); /* 52B58..52B98: detach before death processing. */
 }
 if(type==53&&!(ff_u32(0x80093dd0)&3u)&&hp<(sint32)ff_s16(actor+72)-1)ff_w16(actor+66,(uint16)(hp+1));
 if(ff_s16(actor+80)>0)return FUN_80053034(index);
 if(*(sint8*)ff_ptr(actor+120,1)!=-1)npc_captured_update(actor,table,index);
 events=ff_u32(actor+28);if(events&2048u)ff_w16(actor+82,0);
 if(events&0x500u){fprintf(stderr,"NPC event index=%d state=%u events=%08X tick=%u\n",index,ff_u32(actor+12),events,ff_u32(0x80093dd0));npc_wip_zero(0x80052d04);}
 {uint32 bit;for(bit=0;bit<8;bit++)if(events&(1u<<bit))if(FUN_80048DBC(index,(sint32)bit+1))FUN_80052878(index);}
 /* 52EA4..52EE0: live impact callback precedes voice and eligibility. */
 if(events&0x1000u){
  callback=ff_u32(0x800bb9b0+4u*ff_u32(actor+12));
  ff_actor_event_call(callback,index);
  ff_w16(actor+84,(uint16)((uint16)ff_s16(actor+84)+1u));
 }
 /* 52EE4: one RNG draw, type49 selects alternate sample bank/volume. */
 if(events&0x4000u)FUN_80056670((type==49?87:52)+(sint32)(ff_rand_8006D348()&3u),type==49?255:96,6144);
 result=FUN_80054C90(index);if(result)return result;
 callback=ff_u32(ff_u32(actor+160)+4u*ff_u32(actor+12));
 return npc_action_call(callback,index);
}
static sint32 npc_action_call(uint32 callback,sint32 index) {
 if(callback==0x80055c30)return FUN_80055C30(index);
 if(callback==0x8004d2d4)return FUN_8004D2D4(index);
 if(callback==0x8004df2c)return FUN_8004DF2C(index);
 if(callback==0x8004e7e8)return FUN_8004E7E8(index);
 if(callback==0x8004d914)return FUN_8004D914(index);
 if(callback==0x8004cfe4)return FUN_8004CFE4(index);
 if(callback==0x8004db94)return FUN_8004DB94(index);
 if(callback==0x800544e4)return FUN_800544E4(index);
 if(callback==0x80054474)return FUN_80054474(index);
 if(callback==0x80054398)return FUN_80054398(index);
 if(callback==0x80055770)return FUN_80055770(index);
 if(callback==0x80055100)return FUN_80055100(index);
 if(callback==0x8004edb8)return FUN_8004EDB8(index);
 if(callback==0x800555dc)return FUN_800555DC(index);
 if(callback==0x8004ce48)return FUN_8004CE48(index);
 if(callback==0x8004cb88)return FUN_8004CB88(index);
 if(callback==0x80054598)return FUN_80054598(index);
 if(callback==0x8004e574)return FUN_8004E574(index);
 if(callback==0x800553f0)return FUN_800553F0(index);
 if(callback==0x800551f0)return FUN_800551F0(index);
 if(callback==0x80053cec)return FUN_80053CEC(index);
 if(callback==0x8004cd5c)return FUN_8004CD5C(index);
 if(callback==0x8004e484)return FUN_8004E484(index);
 if(callback==0x80053e1c)return FUN_80053E1C(index);
 if(callback==0x80053fc4)return FUN_80053FC4(index);
 if(callback==0x80052fe8)return FUN_80052FE8(index);
 if(callback==0x80054988)return FUN_80054988(index);
 if(callback==0x80054840)return FUN_80054840(index);
 if(callback==0x80053a70)return FUN_80053A70(index);
 if(callback==0x800539c0)return FUN_800539C0(index);
 if(callback==0x80053b40)return FUN_80053B40(index);
 if(callback==0x80053bf0)return FUN_80053BF0(index);
 if(callback==0x80052f70)return FUN_80052F70(index);
if(callback==0x8004bf3c)return FUN_8004BF3C(index);
if(callback==0x8004d20c)return FUN_8004D20C(index);
if(callback==0x8004e3d8)return FUN_8004E3D8(index);
if(callback==0x8004ece0)return FUN_8004ECE0(index);
if(callback==0x8004ed54)return FUN_8004ED54(index);
if(callback==0x80053f4c)return FUN_80053F4C(index);
if(callback==0x800547b8)return FUN_800547B8(index);
if(callback==0x80054900)return FUN_80054900(index);
if(callback==0x8005531c)return FUN_8005531C(index);
if(callback==0x80055574)return FUN_80055574(index);
if(callback==0x80056530)return FUN_80056530(index);
if(callback==0x800565a4)return FUN_800565A4(index);
 if(callback==0x8004efb8)return (sint32)FUN_8004EFB8(index);
 if(callback==0x80053298)return FUN_80053298(index);
 if(callback==0x8004c134)return FUN_8004C134(index);
 if(callback==0x8004c180)return FUN_8004C180(index);
 if(callback==0x8004d288)return FUN_8004D288(index);
 if(callback==0x8004e38c)return FUN_8004E38C(index);
 if(callback==0x8004ec48)return FUN_8004EC48(index);
 if(callback==0x8004ec94)return FUN_8004EC94(index);
 if(callback==0x8004ef18)return FUN_8004EF18(index);
 if(callback==0x8004ef6c)return FUN_8004EF6C(index);
 if(callback==0x8004f038)return FUN_8004F038(index);
 if(callback==0x80053ca0)return FUN_80053CA0(index);
 if(callback==0x80053dd0)return FUN_80053DD0(index);
 if(callback==0x800553a4)return FUN_800553A4(index);
 if(callback==0x80055528)return FUN_80055528(index);
 if(callback==0x80055918)return FUN_80055918(index);
 if(callback==0x80053558)return FUN_80053558(index);
 if(callback==0x80053278)return FUN_80053278(index);
 if(callback==0x80054a40)return FUN_80054A40(index);
 if(callback==0x8005392c)return FUN_8005392C(index);
 npc_wip_zero(callback);return 0;
}

GDB_CALL sint32 FUN_80054B44(uint32 state){return !(state<=5u||state-26u<5u||state-234u<4u);}
GDB_CALL sint32 FUN_80054BC8(uint32 state){
 if(state<=5u||state==32||state-35u<5u||state==66)return 1;
 if(state==24||state==25)return 2;if(state==166)return 3;if(state==26||state==27)return 4;return state==67?5:0;
}
/* 52744: unsigned attack-eligibility byte, indexed by target actor. */
GDB_CALL sint32 FUN_80052744(sint32 index){return *(uint8*)ff_ptr(0x80094414+(uint32)index,1);}
/* 54C90 reaction eligibility; accepted angle/distance gates precede reaction. */
GDB_CALL sint32 FUN_80054C90(sint32 index){
 uint32 actor=0x800b4318+244u*(uint32)index,target,other,angle; sint32 state,reaction;
 if(ff_u32(0x80093d68)&4u)return 0;
 if((sint32)ff_u32(0x80093dd0)<(sint32)ff_u32(actor+236))return 0;
 state=FUN_8004C9B0(actor);
 if(state==57||state==59||state==208||state==210)return 0;
 if((state==230||state==231||state==232)&&!(ff_u32(actor+28)&512u))return 0;
 target=ff_u32(actor+168);if(target==0xffffffffu||ff_u32(actor+164)==4)return 0;
 if(!FUN_80054B44(ff_u32(actor+12)))return 0;
 other=0x800b4318+244u*target;reaction=FUN_80054BC8(ff_u32(other+12));if(!reaction)return 0;
 angle=(uint32)FUN_8001614C(ff_s16(other+42),ff_s16(other+50),ff_s16(actor+42),ff_s16(actor+50))-32u*(uint32)(uint16)ff_s16(other+56);
 if(reaction==2)angle^=0x8000u;
 if((uint16)(angle+12288u)>=24577u)return 0;
 angle=(uint32)FUN_8001614C(ff_s16(actor+42),ff_s16(actor+50),ff_s16(other+42),ff_s16(other+50))-32u*(uint32)(uint16)ff_s16(actor+56);
 if((uint16)(angle+12288u)>=24577u)return 0;
 if((sint32)ff_u32(0x800941a0)>=449)return 0;
 /* 54EC4..550D4: target eligibility, probability, reaction and shared counters. */
 if(!FUN_80052744((sint32)target))return 0;
 {
  uint32 roll=ff_rand_8006D348()&255u,skill=*(uint8*)ff_ptr(ff_u32(actor+180)+3,1);
  uint32 table=ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),old;
  sint32 animation=-1,alternative=ff_u32(actor+164)==1&&target!=0xffffffffu;
  if(skill<roll){ff_w32(actor+236,ff_u32(0x80093dd0)+((255u-skill)>>5));return 0;}
  if(state==60){if(ff_u32(actor+4)==0xffffffffu)FUN_8004C4D8(actor,table,60);return 1;}
  if(reaction==1)animation=alternative&&!(ff_rand_8006D348()&1u)?230:57;
  else if(reaction==2)animation=57;
  else if(reaction==3)animation=231;
  else if(reaction==4)animation=alternative&&!(ff_rand_8006D348()&1u)?232:57;
  else if(reaction==5){if((ff_u32(actor+124)&0x00ffff00u)!=0x00ffff00u)FUN_80017E30(index);animation=208;}
  if(animation!=-1)FUN_8004C4D8(actor,table,animation);
  old=ff_u32(0x800947d8);ff_w32(0x800947d8,old+1u);
  if((sint32)(old+1u)>=3){
   uint32 level=ff_u32(0x80093e18)+1u,limit=ff_u32(0x800944dc)+1u;
   ff_w32(0x800947d8,old-1u);if((sint32)level>(sint32)limit)level=limit;
   ff_w32(0x80093e18,level);ff_w32(0x80094798,ff_u32(0x80093dd0)+100u);
  }
 }
 return 1;
}
GDB_CALL sint32 FUN_80052FE8(sint32 index){
 if(ff_u32(0x800b4318+244u*(uint32)index+4)==0xffffffffu)return FUN_80053278(index);return -1;
}

/* 4A704 NPC dispatch and stage0 environment response. Other stage responses WIP. */
GDB_CALL sint32 FUN_8004A704(void){
 uint32 tick=ff_u32(0x80093dd0),actor,callback,stage,object,desired,old; sint32 i,j,type,nearest,distance,result;
 if(ff_u32(0x80094798)==tick&&(sint32)ff_u32(0x800944dc)<(sint32)ff_u32(0x80093e18)){
  ff_w32(0x80093e18,ff_u32(0x80093e18)-1u);ff_w32(0x80094798,tick+30u);ff_w32(0x800947d8,0);
 }
 if(!(ff_u32(0x80093dd0)&15u))ff_w32(0x80093d24,ff_u32(0x80093d24)+1u);
 if((sint32)ff_u32(0x80093d24)<49)ff_w32(0x80093d20,0);
 else {ff_w32(0x80093d20,1);ff_w32(0x80093d24,ff_u32(0x80093d24)-48u);}
 FUN_8004A4CC();
 for(i=2;i<8;i++){
  actor=0x800b4318+244u*(uint32)i;type=ff_s16(actor+52);if(type==-1)continue;
  if(*(sint8*)ff_ptr(actor+119,1))ff_w32(0x80093d24,ff_u32(0x80093d24)+1u);
  if(ff_u32(0x80093d20)&&(sint32)ff_u32(actor+184)<4){ff_w32(actor+184,ff_u32(actor+184)+1u);ff_w32(actor+188,0);}
  callback=ff_u32(0x800b7060+4u*(uint32)type);
  if(callback==0x8004df2c)FUN_8004DF2C(i);else if(callback==0x80052944)FUN_80052944(i);else if(callback==0x8004ddec)FUN_8004DDEC(i);else if(callback==0x8003b83c)FUN_8003B83C(i);else if(callback==0x8003b8e4)FUN_8003B8E4(i);else if(callback==0x8004be84)FUN_8004BE84(i);else if(callback==0x80049e6c)FUN_80049E6C(i);else if(callback)npc_wip_zero(callback);
 }
 stage=ff_u32(0x80093d58);if(stage>=2u&&stage!=3&&stage!=17)return 17;
 result=(sint32)ff_u32(0x800942dc);if(result<=0)return result;
 if(stage!=0)npc_wip_zero(0x8004a8e8);
 for(i=0;i<(sint32)ff_u32(0x800942dc);i++){
  nearest=32767;
  for(j=2;j<8;j++){
   actor=0x800b4318+244u*(uint32)j;if(ff_s16(actor+52)==-1)continue;
   object=ff_u32(0x800940b8+4u*(uint32)i);
   distance=FUN_800163F4(ff_s16(object+6),ff_s16(object+14),ff_s16(actor+42),ff_s16(actor+50));
   /* Original tests entrance timer80 here, not HP66. */
   if(distance<nearest&&ff_s16(actor+80)>0)nearest=distance;
  }
  if(*(sint8*)ff_ptr(0x800941a4,1)==1){
   for(j=0;j<(sint32)ff_u32(0x800940b0);j++)
    if((sint32)ff_u32(0x800b4318+244u*(uint32)j+48)<=0x0a5fffff)nearest=0;
  }
  desired=(uint16)ff_s16((nearest<384?0x8007f358:0x8007f35c)+2u*(uint32)i);
  object=ff_u32(0x800940b8+4u*(uint32)i);old=(uint16)ff_s16(object+16);
  result=(sint32)((desired-old)<<16)>>18;ff_w16(object+16,(uint16)(old+(uint32)result));
 }
 return result;
}

/* 544E4: face stored target only in mode1; reload animation after facing. */
GDB_CALL sint32 FUN_800544E4(sint32 index) {
 FF_FUNCTION_MARKER(0x800544e4u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,target;
 if(ff_u32(actor+164)==1) {
  target=ff_u32(actor+168);
  if(target!=0xffffffffu)FUN_800483BC(index,ff_u32(0x800b4340+244u*target),ff_u32(0x800b4348+244u*target));
 }
 if(ff_u32(actor+4)==0xffffffffu)return FUN_80053558(index);
 return -1;
}
/* 33D7C: signed impact count; inherited A2/A3 are unused by31A28. */
GDB_CALL uint32 FUN_80033D7C(sint32 index) {
 FF_FUNCTION_MARKER(0x80033d7cu,"GAME.EXE");
 sint32 count=ff_s16(0x800b436c+244u*(uint32)index);
 return count?(uint32)count:FUN_80031A28(index,577);
}

/* 49B78 inline distance metric: preserve wrapped ABS including INT_MIN. */
static sint32 npc_distance_49B78(uint32 x,uint32 z,uint32 ox,uint32 oz) {
 uint32 dx=x-ox,dz=z-oz;
 if((sint32)dx<0)dx=0u-dx;if((sint32)dz<0)dz=0u-dz;
 return (sint32)((sint32)dz<(sint32)dx?dx+(uint32)((sint32)dz>>1):dz+(uint32)((sint32)dx>>1));
}
GDB_CALL sint32 FUN_80049B78(sint32 index) {
 FF_FUNCTION_MARKER(0x80049b78u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,other,x,z,yaw;sint32 i,distance;
 ff_w32(0x80094370,0x7fffffff);ff_w32(0x800942e0,0xffffffff);
 for(i=0;i<(sint32)ff_u32(0x800940b0);i++) {
  other=0x800b4318+244u*(uint32)i;if(ff_s16(other+66)<=0)continue;
  distance=npc_distance_49B78(ff_u32(actor+40),ff_u32(actor+48),ff_u32(other+40),ff_u32(other+48));
  if(distance<(sint32)ff_u32(0x80094370)){ff_w32(0x80094370,(uint32)distance);ff_w32(0x800942e0,(uint32)i);}
 }
 yaw=(uint32)(sint32)ff_s16(actor+56);ff_w32(0x8009418c,0x7fffffff);
 x=ff_u32(actor+40)+((uint32)(sint32)ff_s16(0x800802d0+2u*yaw)<<8);
 z=ff_u32(actor+48)-((uint32)(sint32)ff_s16(0x800806d0+2u*yaw)<<8);
 for(i=2;i<8;i++) {
  other=0x800b4318+244u*(uint32)i;
  if(ff_s16(other+52)==-1||ff_s16(other+66)<=0||i==index)continue;
  distance=npc_distance_49B78(x,z,ff_u32(other+40),ff_u32(other+48));
  if(distance<(sint32)ff_u32(0x8009418c))ff_w32(0x8009418c,(uint32)distance);
 }
 return 0;
}
/* 49DB0: AEE90 is an immediate table address, not a word loaded there. */
GDB_CALL sint32 FUN_80049DB0(sint32 index,sint32 mode) {
 FF_FUNCTION_MARKER(0x80049db0u,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index;
 FUN_8004C4D8(actor,ff_u32(0x800b3be8+4u*(uint32)(sint32)ff_s16(actor+52)),269);
 ff_w16(actor+80,25);ff_w32(actor+160,0x800aee90);ff_w32(actor+184,0);ff_w32(actor+188,0);
 FUN_80049B78(index);ff_w32(actor+168,ff_u32(0x800942e0));ff_w32(actor+164,mode?1:2);return mode?1:2;
}

/* 49E6C: van arrival, collision region, braking and four passenger spawns. */
GDB_CALL uint32 FUN_80049E6C(sint32 index) {
 FF_FUNCTION_MARKER(0x80049e6cu,"GAME.EXE");
 uint32 actor=0x800b4318+244u*(uint32)index,region,temp=0x801fc100,x,z,point[3],obj,n,phase;
 sint32 speed=ff_s16(actor+76),heading=ff_s16(actor+78),yaw,tilt,roll,rate,value,child;int i;
 static const sint32 spawn_frame[4]={13,16,28,31},spawn_kind[4]={5,6,7,5};
 if(!ff_u32(actor+12))heading=1024;
 ff_w32(actor+40,ff_u32(actor+40)+(uint32)((sint32)((uint32)(sint32)ff_s16(0x800802d0+2u*(uint32)heading)*(uint32)speed)>>1));
 region=0x8009ec70+24u*ff_u32(0x8009479c);
 ff_w32(actor+48,ff_u32(actor+48)-(uint32)((sint32)((uint32)(sint32)ff_s16(0x800806d0+2u*(uint32)heading)*(uint32)speed)>>1));
 FUN_8003B77C(region,66);ff_w16(temp+16,(uint16)heading);ff_w32(temp+4,ff_u32(actor+40));
 ff_w32(0x8009479c,ff_u32(0x8009479c)-1u);ff_w32(temp+12,ff_u32(actor+48));FUN_80029AFC(region,temp);
 yaw=ff_s16(actor+56);if(heading!=yaw)heading-=16;
 if(ff_u32(actor+12)) {
  if(!speed)ff_w32(actor+216,(uint32)FUN_80056670(132,0,6400)+1u);
  FUN_80046884(actor+40,temp+40,temp+44);
  FUN_800568C8((sint32)(ff_u32(actor+216)-1u),(sint32)ff_u32(temp+40),(sint32)ff_u32(temp+44));
  if(speed==240)FUN_80056670(110,255,5888);if(speed<256)speed+=16;
  if((sint32)ff_u32(actor+40)<=100663295) {
   yaw=ff_s16(actor+56);
   if(yaw!=1024)ff_w16(actor+56,(uint16)((yaw-32)&2047));
   else if((sint32)ff_u32(actor+48)>-100663296){ff_w32(actor+12,0);ff_w32(actor+4,0);ff_w32(actor,1);}
  }
 } else {
  rate=ff_s16(actor+54);
  if(yaw>=385){if(rate>=-31)ff_w16(actor+54,(uint16)(rate-2));ff_w16(actor+58,1536);}
  else if(rate<0)ff_w16(actor+54,(uint16)(rate+4));
  ff_w16(actor+56,(uint16)((uint16)ff_s16(actor+56)+(uint16)ff_s16(actor+54)));
  roll=ff_s16(actor+60);tilt=ff_s16(actor+58);tilt=(sint16)(tilt+((-roll-tilt)>>3));
  ff_w16(actor+58,(uint16)tilt);roll=(sint16)(roll+(tilt>>2));ff_w16(actor+60,(uint16)roll);
  if(roll>=1537)ff_w16(actor+60,1536);if(ff_s16(actor+60)<-1536)ff_w16(actor+60,(uint16)-1536);
  speed-=8;if(speed<0)speed=0;
  if(!speed) {
   phase=ff_u32(actor+4)+1u;ff_w32(actor+4,phase);value=(sint32)ff_u32(0x80094674);
   if((sint32)phase<39){if(value<28)ff_w32(0x80094674,(uint32)value+1u);}
   else if(value>0)ff_w32(0x80094674,(uint32)value-4u);
  }
  if((sint32)ff_u32(actor+4)>=2) {
   x=ff_u32(actor+40);z=ff_u32(actor+48);yaw=ff_s16(actor+56);
   FUN_80010040();FUN_80011D20();FUN_800101CC(32768);FUN_8001033C((sint32)((uint32)yaw<<5));
   for(i=0;i<4;i++)if(ff_u32(actor+4)==(uint32)spawn_frame[i]) {
    FUN_800614F4((i&1)?-5242880:5242880,0,i<2?524288:-6815744,point);
    child=FUN_8004AF18(spawn_kind[i],x+point[0],z+point[2],(yaw+((i&1)?-512:512))&2047,0,0,(i&1)?0:1);
    FUN_80049DB0(child,i==0);
    if(i)ff_w32(0x80093dbc,ff_u32(0x80093dbc)+1u);
    if(i==3)ff_w8(actor+117,0);
   }
   if(ff_u32(actor+4)==47) {
    obj=0x800b89f0+36u*ff_u32(0x8009403c);n=ff_u32(0x8009479c);region=0x8009ec70+24u*n;
    ff_w16(actor+52,65535);ff_w32(obj,66);ff_w32(obj+4,x);ff_w32(obj+8,0);ff_w32(obj+12,z);ff_w16(obj+16,(uint16)((uint32)yaw<<5));
    ff_w16(obj+30,(uint16)ff_u32(actor+216));ff_w32(region+20,obj);
    ff_w16(region+4,(uint16)(x>>16));ff_w16(region,(uint16)(x>>16));ff_w16(region+6,(uint16)(z>>16));ff_w16(region+2,(uint16)(z>>16));
    ff_w32(0x8009479c,n+1u);ff_w32(0x80093d60,ff_u32(0x80093d60)+1u);FUN_8003B77C(region,(sint32)ff_u32(obj));
    n=ff_u32(0x8009403c);obj+=36;ff_w32(obj,65);ff_w32(obj+4,x);ff_w32(obj+8,0);ff_w32(obj+12,z);ff_w16(obj+16,(uint16)((uint32)yaw<<5));ff_w32(0x8009403c,n+2u);
   }
   FUN_80010028();
  }
 }
 ff_w16(actor+78,(uint16)(heading&2047));ff_w16(actor+76,(uint16)speed);return (uint32)heading&2047;
}
