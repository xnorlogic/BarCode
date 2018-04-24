#include "BarCode_Lib.h"

/*BW_BarCode*/
typedef struct {
	unsigned char SupplierID; /* Supplier ID 1 Byte  */
	unsigned char HardwareID; /* Hardware ID 1 Byte  */
	unsigned char Format; /* Format 1 Byte  */
	unsigned char SerialNumber[17]; /* Serial Number 17 Bytes */
	unsigned short int Data[53]; /* Payload 106 Bytes */
	unsigned char Padding; /* Padding 1 Byte  */
	unsigned char CheckSum; /* Check Sum 1 Byte  */
    /* Total 128 Bytes */
} BarCode;

/* 128 bytes bar code */
BarCode MY_APP_BarCode = {
/* Supplier ID 		1   Byte  */
0,
/* Hardware ID 		1   Byte  */
0,
/* Format 			1   Byte  */
0,
/* Serial Number 	17  Bytes */
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
/* Payload 			106 Bytes */
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0 },
/* Padding 			1   Byte  */
0,
/* Check Sum 		1   Byte  */
0, };

unsigned char CHECK_bool = 0;

float USABE_ARRAY_A[25] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0 };

float USABE_ARRAY_B[25] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0 };

float USABE_ARRAY_C = 0;
float USABE_ARRAY_D = 0;

unsigned char barCode_intg_check(unsigned char *data) {
	/* Array index variable */
	unsigned char Index = 0U;
	unsigned char ArrayDataSize = 0U;

	/* Byte offset variables */
	unsigned char OffSet = 0U;
	unsigned char ByteOffset = 0U;

	/* 2 byte variable construct */
	unsigned char LowByte = 0U;
	unsigned char HighByte = 0U;

	/* Checksum calculation variable */
	unsigned char CheckSUM_calc = 0U;

	/* Integrity check bools */
	unsigned char CheckSum_OK = 0;
	unsigned char DataCheck_OK = 0;

	/*******************************************
	 ** Description  : Supplier ID
	 ** Byte offset  : 1
	 ** Offset start : 0
	 *******************************************/
	OffSet = 0U;
	MY_APP_BarCode.SupplierID = *(data + OffSet);

	/*******************************************
	 ** Description  : Hardware ID
	 ** Byte offset  : 1
	 ** Offset start : 1
	 *******************************************/
	OffSet = 1U;
	MY_APP_BarCode.HardwareID = *(data + OffSet);

	/*******************************************
	 ** Description  : Bar Code Format
	 ** Byte offset  : 1
	 ** Offset start : 2
	 *******************************************/
	OffSet = 2U;
	MY_APP_BarCode.Format = *(data + OffSet);

	/*******************************************
	 ** Description  : Serial Number
	 ** Bytes        : 1 per data value
	 (17 bytes total)
	 ** Offset start : 3
	 ** Data size    : 17
	 *******************************************/
	ByteOffset = 1U;
	OffSet = 3U;
	ArrayDataSize = 17U;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* Assemble the array element */
		MY_APP_BarCode.SerialNumber[Index] = *(data + OffSet);
		/* one byte offset */
		OffSet = OffSet + ByteOffset;
	}

	/*******************************************
	 ** Description  : Supplier Data
	 ** Bytes        : 2 per data value
	 (106 bytes total)
	 ** Offset start : 20
	 ** Data size    : 53
	 *******************************************/
	ByteOffset = 2U;
	OffSet = 20U;
	ArrayDataSize = 53U;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* Extract Low and high byte */
		HighByte = *(data + OffSet);
		LowByte = *(data + OffSet + 1);
		/* Assemble the array element */
		MY_APP_BarCode.Data[Index] = (unsigned short int) ((HighByte << 8)
				| LowByte);
		/* two byte offset */
		OffSet = OffSet + ByteOffset;
	}

	/*******************************************
	 ** Description  : Padding
	 ** Bytes        : 1
	 ** Offset start : 126
	 *******************************************/
	OffSet = 126U;
	MY_APP_BarCode.Padding = *(data + OffSet);

	/*******************************************
	 ** Description : Checksum
	 ** Bytes       : 1
	 ** Offset start: 127
	 *******************************************/
	OffSet = 127U;
	MY_APP_BarCode.CheckSum = *(data + OffSet);

	/*******************************************
	 ** Integrity logic check
	 *******************************************/

	/* 2's Complement Checksum calculator */
	CheckSUM_calc = CheckSum_Calculator(128U, data);

	/* Checksum integrity check pass */
	if (CheckSUM_calc == MY_APP_BarCode.CheckSum) {
		CheckSum_OK = 1;
	} else {
		CheckSum_OK = 0;
	}

	/* Data integrity check */
	/* TBD */

	/* Data integrity check pass */
	/* TBD change criteria */
	if (1) {
		DataCheck_OK = 1;
	} else {
		DataCheck_OK = 0;
	}

	/* Integrity bool check */
	if ((CheckSum_OK == 1U) && (DataCheck_OK == 1U)) {
		return 1;
	} else {
		return 0;
	}
}

unsigned char barCode_loadNready(unsigned char *data) {
	/*******************************************
	 unsigned char Local Variables
	 *******************************************/
	/* Array index variable */
	unsigned char Index = 0U;
	unsigned char ArrayDataSize = 0U;
	/* Byte offset variables */
	unsigned char OffSet = 0U;
	unsigned char ByteOffset = 0U;
	/* 2 byte variable construct */
	unsigned char LowByte = 0U;
	unsigned char HighByte = 0U;
	/* Supplier Data Array offsets */
	unsigned char A_Data_Offset = 0U; /* first 25 elements of the array */
	unsigned char B_Data_Offset = 25U; /* next 25 elements of the array */
	unsigned char C_Data_Offset = 50U; /* next element of the array */
	unsigned char D_Data_Offset = 51U; /* next element of the array */
	/* Data check counter */
	unsigned char DataCheck = 0U;
	/* Characterized Data Array size */
	unsigned char Characterized_D_arraySize = 25U;
	/* Bar code Characterized total data size */
	unsigned char BarCode_D_arraySize = 52U;

	/*******************************************
	 float Local Variables
	 *******************************************/
	/* Data buffer variable */
	float buffer = 0.0f;
	/* Data Scale Factor */
	float Data_Scale_Factor = 10.0f;

	/*******************************************
	 ** Description  : Supplier Data
	 ** Bytes        : 2 per data value
	 (106 bytes total)
	 ** Offset start : 20
	 ** Data size    : 53
	 *******************************************/
	OffSet = 20U;
	ByteOffset = 2U;
	ArrayDataSize = 53U;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* Extract Low and high byte */
		HighByte = *(data + OffSet);
		LowByte = *(data + OffSet + 1U);
		/* Assemble the array element */
		MY_APP_BarCode.Data[Index] = (unsigned short int) ((HighByte << 8U)
				| LowByte);
		/* two byte offset */
		OffSet = OffSet + ByteOffset;
	}

	/***************************************************************
	 DataSave Section
	 ***************************************************************/
	/** Save the Bar Code data into USABE_ARRAY_A (array size is 25) **/
	/** Offset = 0 **/
	ArrayDataSize = Characterized_D_arraySize;
	buffer = 0.0f;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* extract data from the bar code data */
		buffer = MY_APP_BarCode.Data[Index + A_Data_Offset];
		/* scale data according to Bar Code Spec */
		buffer /= Data_Scale_Factor;
		/* Assign input with Characterized data */
		USABE_ARRAY_A[Index] = buffer;
	}
	/** Save the Bar Code data into USABE_ARRAY_B (array size is 25) **/
	/** Offset = 26 **/
	ArrayDataSize = Characterized_D_arraySize;
	buffer = 0U;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* extract data from the bar code data */
		buffer = MY_APP_BarCode.Data[Index + B_Data_Offset];
		/* scale data according to Bar Code Spec */
		buffer /= Data_Scale_Factor;
		/* Assign input with Characterized data */
		USABE_ARRAY_B[Index] = buffer;
	}

	/** Save the barcode data into USABE_ARRAY_C (array size is 1) **/
	/** Offset = 51 **/
	Index = 0U;
	/* extract data from the bar code data */
	buffer = MY_APP_BarCode.Data[Index + C_Data_Offset];
	/* scale data according to Bar Code Spec */
	buffer /= Data_Scale_Factor;
	/* Assign input with Characterized data */
	USABE_ARRAY_C = buffer;

	/** Save the barcode data into USABE_ARRAY_D (array size is 1) **/
	/** Offset = 52 **/
	Index = 0U;
	/* extract data from the bar code data */
	buffer = MY_APP_BarCode.Data[Index + D_Data_Offset];
	/* scale data according to Bar Code Spec */
	buffer /= Data_Scale_Factor;
	/* Assign input with Characterized data */
	USABE_ARRAY_D = buffer;

	/***************************************************************
	 DataCheck Section
	 ***************************************************************/
	/** Double check Characterized torque in the extracted array **/
	ArrayDataSize = Characterized_D_arraySize;
	buffer = 0.0f;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* extract data from the bar code data */
		buffer = MY_APP_BarCode.Data[Index + A_Data_Offset];
		/* scale data according to Bar Code Spec */
		buffer /= Data_Scale_Factor;
		/* Check the assigned element */
		if (USABE_ARRAY_A[Index] == buffer) {
			/* Increase the data check accumulator */
			DataCheck = DataCheck + 1U;
		} else {
			DataCheck = DataCheck;
		}
	}

	/** Double check Characterized position in the extracted array **/
	ArrayDataSize = Characterized_D_arraySize;
	buffer = 0.0f;
	for (Index = 0U; Index < ArrayDataSize; Index++) {
		/* extract data from the bar code data */
		buffer = MY_APP_BarCode.Data[Index + B_Data_Offset];
		/* scale data according to Bar Code Spec */
		buffer /= Data_Scale_Factor;
		/* Check the assigned element */
		if (USABE_ARRAY_B[Index] == buffer) {
			/* Increase the data check accumulator */
			DataCheck = DataCheck + 1U;
		} else {
			DataCheck = DataCheck;
		}
	}

	/** Double check Characterized Kisspoint in the extracted variable **/
	buffer = 0.0f;
	Index = 0U;
	/* extract data from the bar code data */
	buffer = MY_APP_BarCode.Data[Index + C_Data_Offset];
	/* scale data according to Bar Code Spec */
	buffer /= Data_Scale_Factor;
	/* Check the assigned element */
	if (USABE_ARRAY_C == buffer) {
		/* Increase the data check accumulator */
		DataCheck = DataCheck + 1U;
	} else {
		DataCheck = DataCheck;
	}

	/** Double check Characterized Kisspoint offset angle in the extracted variable **/
	buffer = 0.0f;
	Index = 0U;
	/* extract data from the bar code data */
	buffer = MY_APP_BarCode.Data[Index + D_Data_Offset];
	/* scale data according to Bar Code Spec */
	buffer /= Data_Scale_Factor;
	/* Check the assigned element */
	if (USABE_ARRAY_D == buffer) {
		/* Increase the data check accumulator */
		DataCheck = DataCheck + 1U;
	} else {
		DataCheck = DataCheck;
	}

	/** Final Data check **/
	/* Check if all data loaded correctly */
	if (DataCheck == BarCode_D_arraySize) {
		/* Set Data bool ready to use */
		CHECK_bool = 1;
		return 1;
	} else {
		CHECK_bool = 0;
		/* Reset Data bool ready to use */
		return 0;
	}
}

unsigned char CheckSum_Calculator(unsigned char BarCodeData_Size,
		unsigned char* data) {
	unsigned char Index = 0U;
	unsigned char CheckSUM_calc = 0U;
	unsigned char BYTE_ELEMENT = 0U;

	for (Index = 0; Index < BarCodeData_Size - 1U; Index++) {
		BYTE_ELEMENT = *(data + Index);
		CheckSUM_calc = (unsigned char) (CheckSUM_calc
				+ ((255U - BYTE_ELEMENT) + 1U));
	}

	return CheckSUM_calc;
}
