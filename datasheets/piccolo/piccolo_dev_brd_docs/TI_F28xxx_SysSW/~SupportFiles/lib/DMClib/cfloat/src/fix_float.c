/*=====================================================================================
 File name:       FIX_FLOAT.C                   
                    
 Originator:	Digital Control Systems Group
			Texas Instruments

 Description:  Conversion of fixed-point to/from floating-point variables                

=====================================================================================
 History:
-------------------------------------------------------------------------------------
 04-15-2005	Version 3.20
-------------------------------------------------------------------------------------*/

#include "dmctype.h"

float fix2float(int32 fix_no, int16 q)
{	

   float32 res,float_no,tmp;
   const float32 invpow2 [] = {1,0.5,0.25,0.125,0.0625,0.03125,0.015625,
                              0.0078125,0.00390625,0.001953125,0.0009765625,
                              0.00048828125,0.000244140625,0.0001220703125,
                              0.00006103515625,0.000030517578125,0.0000152587890625,
                              0.00000762939453125,0.000003814697265625,0.0000019073486328125,
                              0.00000095367431640625,0.000000476837158203125,0.0000002384185791015625,
                              0.00000011920928955078125,0.000000059604644775390625,0.0000000298023223876953125,
                              0.00000001490116119384765625,0.000000007450580596923828125,
                              0.0000000037252902984619140625,0.00000000186264514923095703125,
                              0.000000000931322574615478515625,0.0000000004656612873077392578125};
                                
   float_no = fix_no;
   tmp = invpow2[q];
   res = float_no*tmp;
   return res;
}

long float2fix(float32 float_no, int16 q)
{	

   float32 tmp;
   int32 res;
   const float32 pow2 [] = {1,2,4,8,16,32,64,128,256,516,1024,2048,4096,
                          8192,16384,32768,65536,131072,262144,524288,
                          1048576,2097152,4194304,8388608,16777216,
                          33554432,67108864,134217728,268435456,536870912,
                          1073741824,2147483648};

   tmp = pow2[q];
   tmp = float_no*tmp;  
   res = (int32)tmp;
   return res;
   
}
