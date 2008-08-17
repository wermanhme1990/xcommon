// VC++ 6.0 √¸¡Ó––±‡“Î∑Ω∑®£∫
// cl GetHDIdInfo1.cpp /link Kernel32.lib User32.lib

//  GetHDIdInfo3.cpp

//  for displaying the details of hard drives in 

//  06/11/2000  Lynn McGuire  written with many contributions from others,
//                            IDE drives only under Windows NT/2K and 9X,
//                            maybe SCSI drives later


#define PRINTING_TO_CONSOLE_ALLOWED


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>



#define  TITLE   "DiskId32"


//  Required to ensure correct PhysicalDrive IOCTL structure setup
#pragma pack(1)


//  Max number of drives assuming primary/secondary, master/slave topology
#define  MAX_IDE_DRIVES  4
#define  IDENTIFY_BUFFER_SIZE  512


//  IOCTL commands
#define  DFP_GET_VERSION          0x00074080
#define  DFP_SEND_DRIVE_COMMAND   0x0007c084
#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088

#define  FILE_DEVICE_SCSI              0x0000001b
#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition



//  GETVERSIONOUTPARAMS contains the data returned from the 
//  Get Driver Version function.
typedef struct _GETVERSIONOUTPARAMS
{
  BYTE bVersion;      // Binary driver version.
  BYTE bRevision;     // Binary driver revision.
  BYTE bReserved;     // Not used.
  BYTE bIDEDeviceMap; // Bit map of IDE devices.
  DWORD fCapabilities; // Bit mask of driver capabilities.
  DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;


//  Bits returned in the fCapabilities member of GETVERSIONOUTPARAMS 
#define  CAP_IDE_ID_FUNCTION             1  // ATA ID command supported
#define  CAP_IDE_ATAPI_ID                2  // ATAPI ID command supported
#define  CAP_IDE_EXECUTE_SMART_FUNCTION  4  // SMART commannds supported


//  IDE registers
typedef struct _IDEREGS
{
  BYTE bFeaturesReg;       // Used for specifying SMART "commands".
  BYTE bSectorCountReg;    // IDE sector count register
  BYTE bSectorNumberReg;   // IDE sector number register
  BYTE bCylLowReg;         // IDE low order cylinder value
  BYTE bCylHighReg;        // IDE high order cylinder value
  BYTE bDriveHeadReg;      // IDE drive/head register
  BYTE bCommandReg;        // Actual IDE command.
  BYTE bReserved;          // reserved for future use.  Must be zero.
} IDEREGS, *PIDEREGS, *LPIDEREGS;


//  SENDCMDINPARAMS contains the input parameters for the 
//  Send Command to Drive function.
typedef struct _SENDCMDINPARAMS
{
  DWORD     cBufferSize;   //  Buffer size in bytes
  IDEREGS   irDriveRegs;   //  Structure with drive register values.
  BYTE bDriveNumber;       //  Physical drive number to send 
  //  command to (0,1,2,3).
  BYTE bReserved[3];       //  Reserved for future expansion.
  DWORD     dwReserved[4]; //  For future use.
  BYTE      bBuffer[1];    //  Input buffer.
} SENDCMDINPARAMS, *PSENDCMDINPARAMS, *LPSENDCMDINPARAMS;


//  Valid values for the bCommandReg member of IDEREGS.
#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.


// Status returned from driver
typedef struct _DRIVERSTATUS
{
  BYTE  bDriverError;  //  Error code from driver, or 0 if no error.
  BYTE  bIDEStatus;    //  Contents of IDE Error register.
  //  Only valid when bDriverError is SMART_IDE_ERROR.
  BYTE  bReserved[2];  //  Reserved for future expansion.
  DWORD  dwReserved[2];  //  Reserved for future expansion.
} DRIVERSTATUS, *PDRIVERSTATUS, *LPDRIVERSTATUS;


// Structure returned by PhysicalDrive IOCTL for several commands
typedef struct _SENDCMDOUTPARAMS
{
  DWORD         cBufferSize;   //  Size of bBuffer in bytes
  DRIVERSTATUS  DriverStatus;  //  Driver status structure.
  BYTE          bBuffer[1];    //  Buffer of arbitrary length in which to store the data read from the                                                       // drive.
} SENDCMDOUTPARAMS, *PSENDCMDOUTPARAMS, *LPSENDCMDOUTPARAMS;


// The following struct defines the interesting part of the IDENTIFY
// buffer:
typedef struct _IDSECTOR
{
  USHORT  wGenConfig;
  USHORT  wNumCyls;
  USHORT  wReserved;
  USHORT  wNumHeads;
  USHORT  wBytesPerTrack;
  USHORT  wBytesPerSector;
  USHORT  wSectorsPerTrack;
  USHORT  wVendorUnique[3];
  CHAR    sSerialNumber[20];
  USHORT  wBufferType;
  USHORT  wBufferSize;
  USHORT  wECCSize;
  CHAR    sFirmwareRev[8];
  CHAR    sModelNumber[40];
  USHORT  wMoreVendorUnique;
  USHORT  wDoubleWordIO;
  USHORT  wCapabilities;
  USHORT  wReserved1;
  USHORT  wPIOTiming;
  USHORT  wDMATiming;
  USHORT  wBS;
  USHORT  wNumCurrentCyls;
  USHORT  wNumCurrentHeads;
  USHORT  wNumCurrentSectorsPerTrack;
  ULONG   ulCurrentSectorCapacity;
  USHORT  wMultSectorStuff;
  ULONG   ulTotalAddressableSectors;
  USHORT  wSingleWordDMA;
  USHORT  wMultiWordDMA;
  BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;


typedef struct _SRB_IO_CONTROL
{
  ULONG HeaderLength;
  UCHAR Signature[8];
  ULONG Timeout;
  ULONG ControlCode;
  ULONG ReturnCode;
  ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;


// Define global buffers.
BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];


char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex);
void PrintIdeInfo (int drive, DWORD diskdata [256]);
BOOL DoIDENTIFY (HANDLE, PSENDCMDINPARAMS, PSENDCMDOUTPARAMS, BYTE, BYTE,
                 PDWORD);


int ReadPhysicalDriveInNT (void)
{
  int done = FALSE;
  int drive = 0;
  
  for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
  {
    HANDLE hPhysicalDriveIOCTL = 0;
    
    //  Try to get a handle to PhysicalDrive IOCTL, report failure
    //  and exit if can't.
    char driveName [256];
    
    sprintf (driveName, "\\\\.\\PhysicalDrive%d", drive);
    
    //  Windows NT, Windows 2000, must have admin rights
    hPhysicalDriveIOCTL = CreateFile (driveName,
      GENERIC_READ | GENERIC_WRITE, 
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
      OPEN_EXISTING, 0, NULL);
    // if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
    //    printf ("Unable to open physical drive %d, error code: 0x%lX\n",
    //            drive, GetLastError ());
    
    if (hPhysicalDriveIOCTL != INVALID_HANDLE_VALUE)
    {
      GETVERSIONOUTPARAMS VersionParams;
      DWORD               cbBytesReturned = 0;
      
      // Get the version, etc of PhysicalDrive IOCTL
      memset ((void*) &VersionParams, 0, sizeof(VersionParams));
      
      if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
        NULL, 
        0,
        &VersionParams,
        sizeof(VersionParams),
        &cbBytesReturned, NULL) )
      {         
        // printf ("DFP_GET_VERSION failed for drive %d\n", i);
        // continue;
      }
      
      // If there is a IDE device at number "i" issue commands
      // to the device
      if (VersionParams.bIDEDeviceMap > 0)
      {
        BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
        SENDCMDINPARAMS  scip;
        //SENDCMDOUTPARAMS OutCmd;
        
        // Now, get the ID sector for all IDE devices in the system.
        // If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
        // otherwise use the IDE_ATA_IDENTIFY command
        bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? \
IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;
        
        memset (&scip, 0, sizeof(scip));
        memset (IdOutCmd, 0, sizeof(IdOutCmd));
        
        if ( DoIDENTIFY (hPhysicalDriveIOCTL, 
          &scip, 
          (PSENDCMDOUTPARAMS)&IdOutCmd, 
          (BYTE) bIDCmd,
          (BYTE) drive,
          &cbBytesReturned))
        {
          DWORD diskdata [256];
          int ijk = 0;
          USHORT *pIdSector = (USHORT *)
            ((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;
          
          for (ijk = 0; ijk < 256; ijk++)
            diskdata [ijk] = pIdSector [ijk];
          
          PrintIdeInfo (drive, diskdata);
          
          done = TRUE;
        }
      }
      
      CloseHandle (hPhysicalDriveIOCTL);
    }
  }
  
  return done;
}


// DoIDENTIFY
// FUNCTION: Send an IDENTIFY command to the drive
// bDriveNum = 0-3
// bIDCmd = IDE_ATA_IDENTIFY or IDE_ATAPI_IDENTIFY
BOOL DoIDENTIFY (HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
                 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
                 PDWORD lpcbBytesReturned)
{
  // Set up data structures for IDENTIFY command.
  pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
  pSCIP -> irDriveRegs.bFeaturesReg = 0;
  pSCIP -> irDriveRegs.bSectorCountReg = 1;
  pSCIP -> irDriveRegs.bSectorNumberReg = 1;
  pSCIP -> irDriveRegs.bCylLowReg = 0;
  pSCIP -> irDriveRegs.bCylHighReg = 0;
  
  // Compute the drive number.
  pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);
  
  // The command can either be IDE identify or ATAPI identify.
  pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
  pSCIP -> bDriveNumber = bDriveNum;
  pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
  
  return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
    (LPVOID) pSCIP,
    sizeof(SENDCMDINPARAMS) - 1,
    (LPVOID) pSCOP,
    sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
    lpcbBytesReturned, NULL) );
}


//  ---------------------------------------------------

// (* Output Bbuffer for the VxD (rt_IdeDinfo record) *)
typedef struct _rt_IdeDInfo_
{
  BYTE IDEExists[4];
  BYTE DiskExists[8];
  WORD DisksRawInfo[8*256];
} rt_IdeDInfo, *pt_IdeDInfo;


// (* IdeDinfo "data fields" *)
typedef struct _rt_DiskInfo_
{
  BOOL DiskExists;
  BOOL ATAdevice;
  BOOL RemovableDevice;
  WORD TotLogCyl;
  WORD TotLogHeads;
  WORD TotLogSPT;
  char SerialNumber[20];
  char FirmwareRevision[8];
  char ModelNumber[40];
  WORD CurLogCyl;
  WORD CurLogHeads;
  WORD CurLogSPT;
} rt_DiskInfo;


#define  m_cVxDFunctionIdesDInfo  1


//  ---------------------------------------------------


int ReadDrivePortsInWin9X (void)
{
  int done = FALSE;
  
  HANDLE VxDHandle = 0;
  pt_IdeDInfo pOutBufVxD = 0;
  DWORD lpBytesReturned = 0;
  
		//  set the thread priority high so that we get exclusive access to the disk
  BOOL status =
    // SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    SetPriorityClass (GetCurrentProcess (), REALTIME_PRIORITY_CLASS);
		// SetPriorityClass (GetCurrentProcess (), HIGH_PRIORITY_CLASS);
#ifdef PRINTING_TO_CONSOLE_ALLOWED
  
  if (0 == status) 
	   // printf ("\nERROR: Could not SetThreadPriority, LastError: %d\n", GetLastError ());
     printf ("\nERROR: Could not SetPriorityClass, LastError: %d\n", GetLastError ());
  
#endif
  
  // 1. Make an output buffer for the VxD
  rt_IdeDInfo info;
  pOutBufVxD = &info;
  
  // *****************
  // KLUDGE WARNING!!!
  // HAVE to zero out the buffer space for the IDE information!
  // If this is NOT done then garbage could be in the memory
  // locations indicating if a disk exists or not.
  ZeroMemory (&info, sizeof(info));
  
  // 1. Try to load the VxD
  //  must use the short file name path to open a VXD file
  //char StartupDirectory [2048];
  //char shortFileNamePath [2048];
  //char *p = NULL;
  //char vxd [2048];
  //  get the directory that the exe was started from
  //GetModuleFileName (hInst, (LPSTR) StartupDirectory, sizeof (StartupDirectory));
  //  cut the exe name from string
  //p = &(StartupDirectory [strlen (StartupDirectory) - 1]);
  //while (p >= StartupDirectory && *p && '\\' != *p) p--;
  //*p = '\0';   
  //GetShortPathName (StartupDirectory, shortFileNamePath, 2048);
  //sprintf (vxd, "\\\\.\\%s\\IDE21201.VXD", shortFileNamePath);
  //VxDHandle = CreateFile (vxd, 0, 0, 0,
  //               0, FILE_FLAG_DELETE_ON_CLOSE, 0);   
  VxDHandle = CreateFile ("\\\\.\\IDE21201.VXD", 0, 0, 0, 0, 
    FILE_FLAG_DELETE_ON_CLOSE, 0);
  
  if (VxDHandle != INVALID_HANDLE_VALUE)
  {
    // 2. Run VxD function
    DeviceIoControl (VxDHandle, m_cVxDFunctionIdesDInfo,
      0, 0, pOutBufVxD, sizeof(pt_IdeDInfo), &lpBytesReturned, 0);
    
    // 3. Unload VxD
    CloseHandle (VxDHandle);
  }
  else
  {
    MessageBox (NULL, "ERROR: Could not open IDE21201.VXD file", 
    TITLE, MB_ICONSTOP);
  }
  // 4. Translate and store data
  unsigned long int i = 0;
  for (i=0; i < 8; i++)
  {
    if((pOutBufVxD->DiskExists[i]) && (pOutBufVxD->IDEExists[i/2]))
    {
      DWORD diskinfo [256];
      for (int j = 0; j < 256; j++) 
        diskinfo [j] = pOutBufVxD -> DisksRawInfo [i * 256 + j];
      
      // process the information for this buffer
      PrintIdeInfo (i, diskinfo);
      done = TRUE;
    }
  }
  
		//  reset the thread priority back to normal
  // SetThreadPriority (GetCurrentThread(), THREAD_PRIORITY_NORMAL);
  SetPriorityClass (GetCurrentProcess (), NORMAL_PRIORITY_CLASS);
  
  return done;
}


#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE


int ReadIdeDriveAsScsiDriveInNT (void)
{
  int done = FALSE;
  int controller = 0;
  
  for (controller = 0; controller < 2; controller++)
  {
    HANDLE hScsiDriveIOCTL = 0;
    char   driveName [256];
    
    //  Try to get a handle to PhysicalDrive IOCTL, report failure
    //  and exit if can't.
    sprintf (driveName, "\\\\.\\Scsi%d:", controller);
    
    //  Windows NT, Windows 2000, any rights should do
    hScsiDriveIOCTL = CreateFile (driveName,
      GENERIC_READ | GENERIC_WRITE, 
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
      OPEN_EXISTING, 0, NULL);
    // if (hScsiDriveIOCTL == INVALID_HANDLE_VALUE)
    //    printf ("Unable to open SCSI controller %d, error code: 0x%lX\n",
    //            controller, GetLastError ());
    
    if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
    {
      int drive = 0;
      
      for (drive = 0; drive < 2; drive++)
      {
        char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
        SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
        SENDCMDINPARAMS *pin =
          (SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
        DWORD dummy;
        
        memset (buffer, 0, sizeof (buffer));
        p -> HeaderLength = sizeof (SRB_IO_CONTROL);
        p -> Timeout = 10000;
        p -> Length = SENDIDLENGTH;
        p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
        strncpy ((char *) p -> Signature, "SCSIDISK", 8);
        
        pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
        pin -> bDriveNumber = drive;
        
        if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
          buffer,
          sizeof (SRB_IO_CONTROL) +
          sizeof (SENDCMDINPARAMS) - 1,
          buffer,
          sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
          &dummy, NULL))
        {
          SENDCMDOUTPARAMS *pOut =
            (SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
          IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
          if (pId -> sModelNumber [0])
          {
            DWORD diskdata [256];
            int ijk = 0;
            USHORT *pIdSector = (USHORT *) pId;
            
            for (ijk = 0; ijk < 256; ijk++)
              diskdata [ijk] = pIdSector [ijk];
            
            PrintIdeInfo (controller * 2 + drive, diskdata);
            
            done = TRUE;
          }
        }
      }
      CloseHandle (hScsiDriveIOCTL);
    }
  }
  
  return done;
}


char HardDriveSerialNumber [1024];


void PrintIdeInfo (int drive, DWORD diskdata [256])
{
  char string1 [1024];
  __int64 sectors = 0;
  __int64 bytes = 0;
  
  //  copy the hard drive serial number to the buffer
  strcpy (string1, ConvertToString (diskdata, 10, 19));
  if (0 == HardDriveSerialNumber [0] &&
    //  serial number must be alphanumeric
    //  (but there can be leading spaces on IBM drives)
    (isalnum (string1 [0]) || isalnum (string1 [19])))
    strcpy (HardDriveSerialNumber, string1);
  
#ifdef PRINTING_TO_CONSOLE_ALLOWED
  
  switch (drive / 2)
  {
  case 0: printf ("\nPrimary Controller - ");
    break;
  case 1: printf ("\nSecondary Controller - ");
    break;
  case 2: printf ("\nTertiary Controller - ");
    break;
  case 3: printf ("\nQuaternary Controller - ");
    break;
  }
  
  switch (drive % 2)
  {
  case 0: printf ("Master drive\n\n");
    break;
  case 1: printf ("Slave drive\n\n");
    break;
  }
  
  printf ("Drive Model Number________________: %s\n",
    ConvertToString (diskdata, 27, 46));
  printf ("Drive Serial Number_______________: %s\n",
    ConvertToString (diskdata, 10, 19));
  printf ("Drive Controller Revision Number__: %s\n",
    ConvertToString (diskdata, 23, 26));
  
  printf ("Controller Buffer Size on Drive___: %u bytes\n",
    diskdata [21] * 512);
  
  printf ("Drive Type________________________: ");
  if (diskdata [0] & 0x0080)
    printf ("Removable\n");
  else if (diskdata [0] & 0x0040)
    printf ("Fixed\n");
  else printf ("Unknown\n");
  
		//  calculate size based on 28 bit or 48 bit addressing
		//  48 bit addressing is reflected by bit 10 of word 83
  if (diskdata [83] & 0x400) 
    sectors = diskdata [103] * 65536I64 * 65536I64 * 65536I64 + 
    diskdata [102] * 65536I64 * 65536I64 + 
    diskdata [101] * 65536I64 + 
    diskdata [100];
  else
    sectors = diskdata [61] * 65536 + diskdata [60];
		//  there are 512 bytes in a sector
  bytes = sectors * 512;
  printf ("Drive Size________________________: %I64d bytes\n",
    bytes);
  
#else   //  PRINTING_TO_CONSOLE_ALLOWED
  
  //  nothing to do
  
#endif  // PRINTING_TO_CONSOLE_ALLOWED
  
}


char *ConvertToString (DWORD diskdata [256], int firstIndex, int lastIndex)
{
  static char string [1024];
  int index = 0;
  int position = 0;
  
  //  each integer has two characters stored in it backwards
  for (index = firstIndex; index <= lastIndex; index++)
  {
    //  get high byte for 1st character
    string [position] = (char) (diskdata [index] / 256);
    position++;
    
    //  get low byte for 2nd character
    string [position] = (char) (diskdata [index] % 256);
    position++;
  }
  
  //  end the string 
  string [position] = '\0';
  
  //  cut off the trailing blanks
  for (index = position - 1; index > 0 && ' ' == string [index]; index--)
    string [index] = '\0';
  
  return string;
}


long getHardDriveComputerID ()
{
  int done = FALSE;
  // char string [1024];
  __int64 id = 0;
  OSVERSIONINFO version;
  
  strcpy (HardDriveSerialNumber, "");
  
  memset (&version, 0, sizeof (version));
  version.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
  GetVersionEx (&version);
  if (version.dwPlatformId == VER_PLATFORM_WIN32_NT)
  {
		  //  this works under WinNT4 or Win2K if you have admin rights
#ifdef PRINTING_TO_CONSOLE_ALLOWED
    printf ("\nTrying to read the drive IDs using physical access\n");
#endif
    done = ReadPhysicalDriveInNT ();
    
    //  this should work in WinNT or Win2K if previous did not work
    //  this is kind of a backdoor via the SCSI mini port driver into
    //     the IDE drives
#ifdef PRINTING_TO_CONSOLE_ALLOWED
    printf ("\nTrying to read the drive IDs using the SCSI back door\n");
#endif
    // if ( ! done) 
    done = ReadIdeDriveAsScsiDriveInNT ();
  }
  else
  {
    //  this works under Win9X and calls a VXD
    int attempt = 0;
    
    //  try this up to 10 times to get a hard drive serial number
    for (attempt = 0; attempt < 10 && ! done && 0 == HardDriveSerialNumber [0];  attempt++)
    {
      done = ReadDrivePortsInWin9X ();
    }
  }
  
  if (done)
  {
    char *p = HardDriveSerialNumber;
    
    //WriteConstantString ("HardDriveSerialNumber", HardDriveSerialNumber);
    
    //  ignore first 5 characters from western digital hard drives if
    //  the first four characters are WD-W
    if ( ! strncmp (HardDriveSerialNumber, "WD-W", 4)) p += 5;
    for ( ; p && *p; p++)
    {
      if ('-' == *p) continue;
      id *= 10;
      switch (*p)
      {
      case '0': id += 0; break;
      case '1': id += 1; break;
      case '2': id += 2; break;
      case '3': id += 3; break;
      case '4': id += 4; break;
      case '5': id += 5; break;
      case '6': id += 6; break;
      case '7': id += 7; break;
      case '8': id += 8; break;
      case '9': id += 9; break;
      case 'a': case 'A': id += 10; break;
      case 'b': case 'B': id += 11; break;
      case 'c': case 'C': id += 12; break;
      case 'd': case 'D': id += 13; break;
      case 'e': case 'E': id += 14; break;
      case 'f': case 'F': id += 15; break;
      case 'g': case 'G': id += 16; break;
      case 'h': case 'H': id += 17; break;
      case 'i': case 'I': id += 18; break;
      case 'j': case 'J': id += 19; break;
      case 'k': case 'K': id += 20; break;
      case 'l': case 'L': id += 21; break;
      case 'm': case 'M': id += 22; break;
      case 'n': case 'N': id += 23; break;
      case 'o': case 'O': id += 24; break;
      case 'p': case 'P': id += 25; break;
      case 'q': case 'Q': id += 26; break;
      case 'r': case 'R': id += 27; break;
      case 's': case 'S': id += 28; break;
      case 't': case 'T': id += 29; break;
      case 'u': case 'U': id += 30; break;
      case 'v': case 'V': id += 31; break;
      case 'w': case 'W': id += 32; break;
      case 'x': case 'X': id += 33; break;
      case 'y': case 'Y': id += 34; break;
      case 'z': case 'Z': id += 35; break;
      }                            
    }
  }
  
#ifdef PRINTING_TO_CONSOLE_ALLOWED
  
  printf ("\nComputer ID_______________________: %I64d\n", id);
  
#endif
  
  return (long) id;
}


int main (int argc, char * argv [])
{
  long id = getHardDriveComputerID ();
  
  return 0;
}
