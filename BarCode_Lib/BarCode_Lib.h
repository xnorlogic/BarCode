/*
Barcode Structure exmple:

struct BarCode_Example{

	unsigned char SupplierID;        	
	unsigned char HardwareID;			
	unsigned char BarCodeFormat;		
	unsigned char SerialNumber[17];	    
	unsigned char Data[53][2];			
	unsigned char Padding;				
	unsigned char CheckSum;			    
};

*/

/*Parameter: unsigned char Pointer to the location of the bar code in memory*/
bool barCode_intg_check(unsigned char *data);
/*Parameter: unsigned char Pointer to the location of the bar code in memory*/
bool barCode_loadNready(unsigned char *data);
/*Parameter: Bar Code data size = 128 bytes, unsigned char Pointer to the location of the bar code in memory*/
unsigned char CheckSum_Calculator(unsigned char BarCodeData_Size, unsigned char* data);
