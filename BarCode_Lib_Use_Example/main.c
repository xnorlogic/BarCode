#include <stdio.h>
#include "BarCode_Lib.h"

/*BarCode_Test structure to simulate a bar code in memory*/
typedef struct{

	unsigned char SupplierID;        	/** 1   byte  **/
	unsigned char HardwareID;			/** 1   byte  **/
	unsigned char BarCodeFormat;		/** 1   byte  **/
	unsigned char SerialNumber[17];	    /** 17  bytes **/
	unsigned char Data[53][2];			/** 106 bytes **/
	unsigned char Padding;				/** 1   byte  **/
	unsigned char CheckSum;			    /** 1   byte  **/
	/** Total  -----------------            128 bytes **/

} BarCode_Test;

/*This will load a test bar code into the memory*/
BarCode_Test MY_Barcode = {

	/* Bar code Raw Data initialization */
	0x32, /*  Supplier ID   */
	0x31, /* Hardware ID   */
	0x31, /*  Bar code Format   */
	{
	/* [0..8] */ 0x41,0x41,0x41,0x41,0x58,0x30,0x30,0x30,0x30,
	/* [9..16] */ 0x30,0x30,0x30,0x30,0x30,0x58,0x30,0x31
	}, /* Serial Number */

	{
	{
	/* [0][0..1] */0x0,0x0
	},
	{
	/* [1][0..1] */0x0,0x85
	},
	{
	/* [2][0..1] */0x0,0xA6
	},
	{
	/* [3][0..1] */0x0,0xD5
	},
	{
	/* [4][0..1] */0x1,0x6E
	},
	{
	/* [5][0..1] */0x1,0xE8
	},
	{
	/* [6][0..1] */0x2,0x8C
	},
	{
	/* [7][0..1] */0x3,0x5B
	},
	{
	/* [8][0..1] */0x4,0x5B
	},
	{
	/* [9][0..1] */0x5,0x8B
	},
	{
	/* [10][0..1] */0x6,0xF0
	},
	{
	/* [11][0..1] */0x8,0x78
	},
	{
	/* [12][0..1] */0xA,0x36
	},
	{
	/* [13][0..1] */0xC,0x45
	},
	{
	/* [14][0..1] */0xE,0x62
	},
	{
	/* [15][0..1] */0x10,0x7B
	},
	{
	/* [16][0..1] */0x15,0x11
	},
	{
	/* [17][0..1] */0x1A,0x34
	},
	{
	/* [18][0..1] */0x1F,0x4B
	},
	{
	/* [19][0..1] */0x24,0x8C
	},
	{
	/* [20][0..1] */0x29,0xBD
	},
	{
	/* [21][0..1] */0x2F,0x4D
	},
	{
	/* [22][0..1] */0x34,0x38
	},
	{
	/* [23][0..1] */0x39,0x5A
	},
	{
	/* [24][0..1] */0x3D,0xCB
	},
	{
	/* [25][0..1] */0x0,0x0
	},
	{
	/* [26][0..1] */0x0,0x15
	},
	{
	/* [27][0..1] */0x0,0x1A
	},
	{
	/* [28][0..1] */0x0,0x1F
	},
	{
	/* [29][0..1] */0x0,0x29
	},
	{
	/* [30][0..1] */0x0,0x2E
	},
	{
	/* [31][0..1] */0x0,0x33
	},
	{
	/* [32][0..1] */0x0,0x38
	},
	{
	/* [33][0..1] */0x0,0x3D
	},
	{
	/* [34][0..1] */0x0,0x42
	},
	{
	/* [35][0..1] */0x0,0x47
	},
	{
	/* [36][0..1] */0x0,0x4C
	},
	{
	/* [37][0..1] */0x0,0x51
	},
	{
	/* [38][0..1] */0x0,0x56
	},
	{
	/* [39][0..1] */0x0,0x5B
	},
	{
	/* [40][0..1] */0x0,0x60
	},
	{
	/* [41][0..1] */0x0,0x6B
	},
	{
	/* [42][0..1] */0x0,0x76
	},
	{
	/* [43][0..1] */0x0,0x80
	},
	{
	/* [44][0..1] */0x0,0x89
	},
	{
	/* [45][0..1] */0x0,0x93
	},
	{
	/* [46][0..1] */0x0,0x9D
	},
	{
	/* [47][0..1] */0x0,0xA6
	},
	{
	/* [48][0..1] */0x0,0xB0
	},
	{
	/* [49][0..1] */0x0,0xBB
	},
	{
	/* [50][0..1] */0x1,0xC9
	},
	{
	/* [51][0..1] */0x0,0x20
	},
	{
	/* [52][0..1] */0x0,0x0
	},
	}, /* Data */
	0x00, /*  Padding  */
	0xE2, /*  Checksum  */
};

unsigned char * DataPointer;

int main(){

	/*flags for the integrity check returns*/
	unsigned char integrityCheck = 0;
	unsigned char confrmCheck = 0;

	/*Index for fetching the data in the bar code*/
	unsigned char Index = 0;

	/*Array size of the data in the bar code*/
	unsigned char ArrayData_Size = 25;

	/*Pointer to the bar code in the memory*/
	DataPointer = &MY_Barcode;

	/*Call to the integrity check function*/
	integrityCheck = barCode_intg_check(DataPointer);

	/*Quick check if the integrity of the data is ok*/
	if(integrityCheck == 1){
	    /*Load the data only if the integrity data check is true*/
		confrmCheck = barCode_loadNready(DataPointer);
	}
	else{
		printf("Bar Code Integrity Failed!... No data will be loaded\n");
	}

	/*Integrity check flags print out*/
	printf("integrity Check           : %d\n", integrityCheck);
	printf("confirmation update Check : %d\n"  , confrmCheck);

	/*Usable data print out... Data A*/
	printf("Usable Data A: ");
	for(Index = 0 ; Index <ArrayData_Size ; Index++){
		printf(" %f "  , USABE_ARRAY_A[Index]);
	}
	printf("\n");

	/*Usable data print out... Data B*/
	printf("Usable Data B: ");
	for(Index = 0 ; Index <ArrayData_Size ; Index++){
		printf(" %f "  , USABE_ARRAY_B[Index]);
	}
	printf("\n");

	/*Usable data print out... Data C*/
	printf("Usable data C: %f\n" , USABE_ARRAY_C);

	/*Usable data print out... Data D*/
	printf("Usable data D: %f\n"   , USABE_ARRAY_D);

	/*Check flag to be used by the application in order to use default data or bar code data*/
	printf("Using Bar Code data: %d"   , CHECK_bool);

	return 0;
}
