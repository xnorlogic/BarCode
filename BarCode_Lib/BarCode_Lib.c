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
	/* Null pointer check bool */
	unsigned char Null_Data_Pointer = 0U;
	/* Integrity check bools */
	unsigned char CheckSum_OK = 0U;
	unsigned char DataCheck_OK = 0U;

	/* NULL pointer prevention */
	if (data != NULL) {
		Null_Data_Pointer = 1U;

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
			CheckSum_OK = 1U;
		} else {
			CheckSum_OK = 0U;
		}

		/* Data integrity check */
		float Data_Scale_Factor = 10.0f;

		/*Calibration Parameters****************/

		/*Torque Calibration parameters*/
		float LowerBound_USABE_ARRAY_A_Cal = 1350.0f;
		float UpperBound_USABE_ARRAY_A_Cal = 1582.0f;

		/*Position Calibration Parameters*/
		float LowerBound_USABE_ARRAY_B_Cal = 10.0f;
		float UpperBound_USABE_ARRAY_B_Cal = 19.1f;

		/*Kisspoint Calibration Parameters*/
		float LowerBound_USABE_ARRAY_C_Cal = 20.0f;
		float UpperBound_USABE_ARRAY_C_Cal = 70.0f;

		/*Kisspoint Offset Angle Parameters*/
		float LowerBound_USABE_ARRAY_D_Cal = 0.0f;
		float UpperBound_USABE_ARRAY_D_Cal = 10.0f;

		/**************************************/

		unsigned char USABE_ARRAY_A_MonotonicallyInc = 0U;
		unsigned char USABE_ARRAY_B_MonotonicallyInc = 0U;
		unsigned char USABE_ARRAY_A_InRange = 0U;
		unsigned char USABE_ARRAY_B_InRange = 0U;
		unsigned char USABE_ARRAY_C_InRange = 0U;
		unsigned char USABE_ARRAY_D_InRange = 0U;

		float Max_USABE_ARRAY_A = MY_APP_BarCode.Data[24U] / Data_Scale_Factor;
		float Min_USABE_ARRAY_A = MY_APP_BarCode.Data[0U] / Data_Scale_Factor;
		float Max_USABE_ARRAY_B = MY_APP_BarCode.Data[49U] / Data_Scale_Factor;
		float USABE_ARRAY_C_value = MY_APP_BarCode.Data[50U] / Data_Scale_Factor;
		float USABE_ARRAY_D_value = MY_APP_BarCode.Data[51U] / Data_Scale_Factor;

		unsigned char A_Data_Offset = 0U; /* first 25 elements of the array */
		unsigned char B_Data_Offset = 25U; /* next 25 elements of the array */
		float buffer_1 = 0.0f;
		float buffer_2 = 0.0f;
		unsigned char Monotonically_Inc_Cnt = 0U;

		/* Torque monotonically increasing check */
		ArrayDataSize = 25U;
		buffer_1 = 0.0f;
		buffer_2 = 0.0f;
		Monotonically_Inc_Cnt = 0U;
		for (Index = 0U; Index < ArrayDataSize - 1U; Index++) {
			/* Extract and scale data element from the barcode data (from torque data offset) */
			buffer_1 = MY_APP_BarCode.Data[Index + A_Data_Offset];
			buffer_1 /= Data_Scale_Factor;
			/* Extract and scale next data element from the barcode data (from torque data offset) */
			buffer_2 = MY_APP_BarCode.Data[(Index + A_Data_Offset) + 1U];
			buffer_2 /= Data_Scale_Factor;
			/* Compare for monotonically increasing data */
			if (buffer_1 <= buffer_2) {
				Monotonically_Inc_Cnt = Monotonically_Inc_Cnt + 1U;
			} else {
				/* NOP */
			}
		}

		/* Torque Monotonically increasing if and only if all data is increasing */
		if (Monotonically_Inc_Cnt == ArrayDataSize - 1U) {
			USABE_ARRAY_A_MonotonicallyInc = 1U;
		} else {
			USABE_ARRAY_A_MonotonicallyInc = 0U;
		}

		/* Position monotonically increasing check */
		ArrayDataSize = 25U;
		buffer_1 = 0.0f;
		buffer_2 = 0.0f;
		Monotonically_Inc_Cnt = 0U;
		/*ArrayDataSize, Data_Offset, Data_Scale_Factor*/
		for (Index = 0U; Index < ArrayDataSize - 1U; Index++) {
			/* Extract and scale data element from the barcode data (from position data offset) */
			buffer_1 = MY_APP_BarCode.Data[Index + B_Data_Offset];
			buffer_1 /= Data_Scale_Factor;
			/* Extract and scale next data element from the barcode data (from position data offset) */
			buffer_2 = MY_APP_BarCode.Data[(Index + B_Data_Offset) + 1U];
			buffer_2 /= Data_Scale_Factor;
			/* Compare for monotonically increasing data */
			if (buffer_1 <= buffer_2) {
				Monotonically_Inc_Cnt = Monotonically_Inc_Cnt + 1U;
			} else {
				/* NOP */
			}
		}

		/* Position Monotonically increasing if and only if all data is increasing */
		if (Monotonically_Inc_Cnt == ArrayDataSize - 1U) {
			USABE_ARRAY_B_MonotonicallyInc = 1U;
		} else {
			USABE_ARRAY_B_MonotonicallyInc = 0U;
		}

		/*Maximum torque in bounds check*/
		if ((LowerBound_USABE_ARRAY_A_Cal < Max_USABE_ARRAY_A)
				&& (Max_USABE_ARRAY_A < UpperBound_USABE_ARRAY_A_Cal) && (Min_USABE_ARRAY_A == 0U)) {
			USABE_ARRAY_A_InRange = 1U;
		} else {
			USABE_ARRAY_A_InRange = 0U;
		}

		/*Maximum angle position in range check*/
		if ((LowerBound_USABE_ARRAY_B_Cal < Max_USABE_ARRAY_B)
				&& (Max_USABE_ARRAY_B < UpperBound_USABE_ARRAY_B_Cal)) {
			USABE_ARRAY_B_InRange = 1U;
		} else {
			USABE_ARRAY_B_InRange = 0U;
		}

		/*Kisspoint in bounds check*/
		if ((LowerBound_USABE_ARRAY_C_Cal < USABE_ARRAY_C_value) && (USABE_ARRAY_C_value < UpperBound_USABE_ARRAY_C_Cal)) {
			USABE_ARRAY_C_InRange = 1U;
		} else {
			USABE_ARRAY_C_InRange = 0U;
		}

		/*Kisspoint offset angle in bounds check*/
		if ((LowerBound_USABE_ARRAY_D_Cal < USABE_ARRAY_D_value)
				&& (USABE_ARRAY_D_value < UpperBound_USABE_ARRAY_D_Cal)) {
			USABE_ARRAY_D_InRange = 1U;
		} else {
			USABE_ARRAY_D_InRange = 0U;
		}

		/* Data integrity check pass */
		if ((USABE_ARRAY_A_MonotonicallyInc == 1U) && (USABE_ARRAY_B_MonotonicallyInc == 1U)
				&& (USABE_ARRAY_A_InRange == 1U) && (USABE_ARRAY_B_InRange == 1U)
				&& (USABE_ARRAY_C_InRange == 1U) && (USABE_ARRAY_D_InRange == 1U)) {
			DataCheck_OK = 1U;
		} else {
			DataCheck_OK = 0U;
		}


	} else {
		Null_Data_Pointer = 0U;
	}

	/* Return statement for the integrity check */
	return (CheckSum_OK & 1U) & (DataCheck_OK & 1U) & (Null_Data_Pointer & 1U);
}

unsigned char barCode_loadNready(unsigned char *data) {
	/* Null pointer check bool */
	unsigned char Null_Data_Pointer = 0U;
	/* Data integrity check flag */
	unsigned char Data_Integrity_Check = 0U;
	/* Data check counter */
	unsigned char DataCheck = 0U;
	/* Data check counter */
	unsigned char Data_Loaded = 0U;
	/* Bar code Characterized total data size */
	unsigned char BarCode_D_arraySize = 52U;

	/* call to the integrity function and asign the value to the flag */
	Data_Integrity_Check = barCode_intg_check(data);

	/* NULL pointer prevention */
	if (data != NULL) {
		/* Set the NULL data pointer */
		Null_Data_Pointer = 1U;
		/* data integrity check prior to saving bar code to the algorithm */
		if(Data_Integrity_Check == 1U){
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
			/* Characterized Data Array size */
			unsigned char Characterized_D_arraySize = 25U;

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
					DataCheck = DataCheck + 0U;
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
					DataCheck = DataCheck + 0U;
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
				DataCheck = DataCheck + 0U;
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
				DataCheck = DataCheck + 0U;
			}
		}else{
			/* do nothing */
		}
		/** Final Data check **/
		/* Check if all data loaded correctly */
		if (DataCheck == BarCode_D_arraySize) {
			/* Set Data bool ready to use */
			CHECK_bool = 1U;
			Data_Loaded = 1U;
		} else {
			/* Reset Data bool ready to use */
			CHECK_bool = 0U;
			Data_Loaded = 0U;
		}
	} else {
		Null_Data_Pointer = 0U;
	}

	/* Return statement for the bar code loaded */
	/* Data loaded correctly in the algorithm = TRUE */
	/* NULL data pointer = TRUE */
	/* Data integrity check = TRUE */
	return (Data_Loaded & 1U) &
		   (Null_Data_Pointer & 1U) &
		   (Data_Integrity_Check & 1U);
}

unsigned char CheckSum_Calculator(unsigned char BarCodeData_Size,
		unsigned char* data) {
	/* Check Sum accumulator */
	unsigned char CheckSUM_calc = 0U;
	/* NULL pointer prevention */
	if (data != NULL) {
		unsigned char Index = 0U;
		unsigned char BYTE_ELEMENT = 0U;

		for (Index = 0; Index < BarCodeData_Size - 1U; Index++) {
			BYTE_ELEMENT = *(data + Index);
			CheckSUM_calc = (unsigned char) (CheckSUM_calc
					+ ((255U - BYTE_ELEMENT) + 1U));
		}
	} else {
		/* force checksum 0 */
		CheckSUM_calc = 0U;
	}

	/* Return statement for the checksum calculator */
	return CheckSUM_calc;
}
