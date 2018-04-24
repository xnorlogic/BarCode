# BarCode
This library checks if a particular bar code loaded in memory has good data or not. Also it extracts usable data and gives indication if its ready to use in the application software.

Structure of the bar code in memory is as follows:

	SupplierID;        	    /** 1   byte  **/
	HardwareID;			    /** 1   byte  **/
	BarCodeFormat;		    /** 1   byte  **/
	SerialNumber[17];	    /** 17  bytes **/
	Data[53][2];			/** 106 bytes **/
	Padding;				/** 1   byte  **/
	CheckSum;			    /** 1   byte  **/
    -----------------           128 bytes **/

it totals 128 bytes or 256 ASCII Characters. It is intended that the bar code string will be parsed to memory in ASCII characters (example below):

32313141414141583030303030303030305830310000008500A600D5016E01E8028C035B045B058B06F008780A360C450E62107B15111A341F4B248C29BD2F4D3438395A3DCB00000015001A001F0029002E00330038003D00420047004C00510056005B0060006B0076008000890093009D00A600B000BB01C90020000000E2

Two main callbacks in the library:

	Return: 1 or 0
	Parameter: unsigned char pointer to the bar location in the memory
	Functionality: checks the integrity of the bar code data. This is customizable to the data structure of the bar code.
	unsigned char barCode_intg_check(unsigned char *data);

	Return: 1 or 0
	Parameter: unsigned char pointer to the bar location in the memory
	Functionality: loads usable data into specific arrays for the application to use. Also has a unsigned char CHECK_bool, that can be used as a selector in the application to select between default data or data in the bar code.
	unsigned char barCode_loadNready(unsigned char *data);