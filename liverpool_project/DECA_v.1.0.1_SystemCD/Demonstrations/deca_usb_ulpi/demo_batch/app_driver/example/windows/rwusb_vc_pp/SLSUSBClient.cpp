// SLSUSBClient.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "slsusbinterface.h"
#include <iostream>
#include "conio.h"
#include "malloc.h"
#include "ctype.h"
#include "Winbase.h"
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>

using namespace std; 

typedef struct Usb_Control_Request
{
    struct bmrequesttype
    {
        unsigned char Recepient     :5;
        unsigned char RequestType   :2;
        unsigned char Direction     :1;

    }bmRequestType;
     unsigned char bRequest;
     union wvalue
    {
        unsigned short int  Value;
        unsigned char       Byte[2];

    }wValue;
     union windex
    {
        unsigned short int  Value;
        unsigned char       Byte[2];

    }wIndex;

    unsigned short int wLength;


}*PUSB_CONTROL_REQUEST,USB_CONTROL_REQUEST;
int main(int argc, char *argv[])
  {
  DWORD dwNumOfDev;
  SLS_HANDLE hDevice;
  char cBuffer[16];
  unsigned char *bWBuf;
  DWORD junk;
  char ch;
  char *cDevBuf[15];
  DWORD iIndex;
  int iNoOfByteToREad = 64;
  int iSelection;
  int EpNo;
  int count = 100000;
  do
    {
    cout << "0: Get Number of devices                             " << endl;
    cout << "1: Get Single Device SerialNumber                    " << endl;
    cout << "2: Get Single Device ProductDescription     " << endl;
    cout << "3: Get All Device SerialNumber              " << endl;
    cout << "4: Get All Device ProductDescription                              " << endl;
    cout << "5: WriteData to Device                               " << endl;
    cout << "6: ReadData from Device                               " << endl;
	cout << "7: Send Vendor read request			   "<<endl;
	cout << "8: Send Vendor write request			   "<<endl;
	cout << "9: Send set interface request			   "<<endl;
	
    cout << "Enter your selection index :";
    cin >> iSelection;
    switch (iSelection)
      {
      case 0:
          {
          if (SLS_ListDevices(&dwNumOfDev, NULL, SLS_LIST_NUMBER_ONLY) == SLS_OK)
            {
            cout << "Number of devices  are :" << dwNumOfDev << endl;
            }
          else
            {
            cout << "Device Not Fount " << endl;
            }
          break;
          }
      case 1:
          {
          cout << "Enter Device Number :";
          cin >> dwNumOfDev;
          if (SLS_ListDevices(&dwNumOfDev, cBuffer, SLS_LIST_BY_INDEX | SLS_OPEN_BY_SERIAL_NUMBER) 
              == SLS_OK)
            {
            cout << "Device Serial Number :" << cBuffer << endl;
            }
          else
            {
            cout << "Device Not Found " << endl;
            }
          break;
          }
      case 2:
          {
          cout << "Enter Device Number :";
          cin >> dwNumOfDev;
          if (SLS_ListDevices(&dwNumOfDev, cBuffer, SLS_LIST_BY_INDEX | SLS_OPEN_BY_DESCRIPTION) ==
              SLS_OK)
            {
            cout << "Device Product Description :" << cBuffer << endl;
            }
          else
            {
            cout << "Device Not Found " << endl;
            }
          break;
          }
      case 3:
          {
          if (SLS_ListDevices(cDevBuf, &dwNumOfDev, SLS_LIST_ALL | SLS_OPEN_BY_SERIAL_NUMBER) ==
              SLS_OK)
            {
            for (iIndex = 0; iIndex < dwNumOfDev; iIndex++)
              {
              cout << "Device No:" << iIndex << endl;
              cout << "Device Serial Number :" << cDevBuf[iIndex] << endl;
              }
            }
          else
            {
            cout << "Device Not Found " << endl;
            }
          break;
          }
      case 4:
          {
          if (SLS_ListDevices(cDevBuf, &dwNumOfDev, SLS_LIST_ALL | SLS_OPEN_BY_DESCRIPTION) ==
              SLS_OK)
            {
            for (iIndex = 0; iIndex < dwNumOfDev; iIndex++)
              {
              cout << "Device No:" << iIndex << endl;
              cout << "Device Product Description :" << cDevBuf[iIndex] << endl;
              }
            }
          else
            {
            cout << "Device Not Found " << endl;
            }
          break;
          break;
          }
      case 5:
          {
          int iDevNo, loop;
          cout << "Enter Device No to access :";
          cin >> iDevNo;
		  cout << "Enter Endpoint No to access :";
          cin >> EpNo;
          cout << "Enter no of times :";
          cin >> loop;
          clock_t start_time, diff_time;
          long diff_ms;
          if (SLS_ListDevices(&iDevNo, cBuffer, SLS_LIST_BY_INDEX | SLS_OPEN_BY_SERIAL_NUMBER) ==
              SLS_OK)
            {
            hDevice = SLS_W32_CreateFile(cBuffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL,
                                         OPEN_EXISTING, SLS_OPEN_BY_SERIAL_NUMBER, NULL, EpNo);
            printf("hDevice = %x\n", hDevice);
            if (hDevice == (PVOID)SLS_INVALID_HANDLE)
              {
              cout << "Invalid Handle Value " << GetLastError() << endl;
              break;
              }
            cout << "No of Bytes to write :";
            cin >> iNoOfByteToREad;
            bWBuf = (BYTE*)malloc(iNoOfByteToREad);
            for (int c = 0; c < iNoOfByteToREad; c++)
              {
              bWBuf[c] = c;
              }
              {
              float speed = 0;
			  start_time = clock();
              for (int ii = 1; ii <= loop; ii++)
                {
                if (!SLS_W32_WriteFile(hDevice, bWBuf, iNoOfByteToREad, &junk, NULL))
                  {
                  cout << "Write operation fail GetlastError :" << GetLastError() << endl;
                  }
                else
                  {
                  //	cout<<"Loop"<<ii;
                  //	cout<<"No of bytes written : "<<junk<<endl;
                  }
                if (ii % 100 == 0)
                  {
                  if (ii == 100)
                    {
                    fprintf(stderr, "\nAvrage Speed(IN Mbits/second) =         ");
                    }
                  diff_time = clock();
                  diff_time -= start_time;
                  diff_ms = (long)(((double)diff_time / CLOCKS_PER_SEC) *1000.0);
                  speed = ((float)((float)(((float)ii)*((float)iNoOfByteToREad)*((float)8)*(float)
                           1000)) / ((float)(((float)1024)*((float)1024)*((float)diff_ms))));
                  if (((int)speed) > 99)
                    {
                    fprintf(stderr, "\b\b\b\b\b\b\b%6.3f", speed);
                    }
                  else
                    {
                    fprintf(stderr, "\b\b\b\b\b\b%6.3f", speed);
                    }
                  }
                }
              }
            diff_time = clock();
            diff_time -= start_time;
            diff_ms = (long)(((double)diff_time / CLOCKS_PER_SEC) *1000.0);
            cout << "\nSpeed = " << ((float)((float)(((float)loop)*((float)iNoOfByteToREad)*((float)
                                     8)*(float)1000)) / ((float)(((float)1024)*((float)1024)*(
                                     (float)diff_ms))));
            cout << "Mbits/second\n";
            SLS_W32_ClosePipeHandle(hDevice);
            free(bWBuf);
            }
          else
            {
            cout << "Device Not Fount" << endl;
            }
          break;
          }
      case 6:
          {
          int iDevNo, loop;
          cout << "Enter Device No to access :";
          cin >> iDevNo;
		  cout << "Enter Endpoint No to access :";
          cin >> EpNo;
          cout << "Enter no of times :";
          cin >> loop;
          clock_t start_time, diff_time;
          long diff_ms;
		  EpNo = EpNo | 0x80;
          if (SLS_ListDevices(&iDevNo, cBuffer, SLS_LIST_BY_INDEX | SLS_OPEN_BY_DESCRIPTION) ==
              SLS_OK)
            {
            hDevice = SLS_W32_CreateFile(cBuffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL,
                                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |
                                         SLS_OPEN_BY_DESCRIPTION, NULL, EpNo);
            printf("hDevice = %x\n", hDevice);
            if (hDevice == INVALID_HANDLE_VALUE)
              {
              cout << "Invalid Handle Value " << GetLastError() << endl;
              break;
              }
            cout << "No of Bytes to Read :";
            cin >> iNoOfByteToREad;
            bWBuf = (BYTE*)malloc(iNoOfByteToREad);
            float speed = 0;
            start_time = clock();
            for (int ii = 1; ii < loop + 1; ii++)
              {
              if (!SLS_W32_ReadFile(hDevice, bWBuf, iNoOfByteToREad, &junk, NULL))
                {
                cout << "Read operation fail GetlastError :" << GetLastError() << endl;
                }
              else
                {
                //cout<<"No of bytes Read :"<<junk<<endl;
                //cout<<"i="<<ii<<endl;
                }
              if (ii % 100 == 0)
                {
                if (ii == 100)
                  {
                  fprintf(stderr, "\nAvrage Speed(IN Mbits/second) =         ");
                  }
                diff_time = clock();
                diff_time -= start_time;
                diff_ms = (long)(((double)diff_time / CLOCKS_PER_SEC) *1000.0);
                speed = ((float)((float)(((float)ii)*((float)iNoOfByteToREad)*((float)8)*(float)
                         1000)) / ((float)(((float)1024)*((float)1024)*((float)diff_ms))));
                if (((int)speed) > 99)
                  {
                  fprintf(stderr, "\b\b\b\b\b\b\b%6.3f", speed);
                  }
                else
                  {
                  fprintf(stderr, "\b\b\b\b\b\b%6.3f", speed);
                  }
                }
              }
            diff_time = clock();
            diff_time -= start_time;
            diff_ms = (long)(((double)diff_time / CLOCKS_PER_SEC) *1000.0);
            cout << "\nSpeed = " << ((float)((float)(((float)loop)*((float)iNoOfByteToREad)*((float)
                                     8)*(float)1000)) / ((float)(((float)1024)*((float)1024)*(
                                     (float)diff_ms))));
            cout << "Mbits/second\n";
            SLS_W32_ClosePipeHandle(hDevice);
            free(bWBuf);
            }
          else
            {
            cout << "Device Not Fount" << endl;
            }
          break;
          }
	  case 7 :
			{
				/*************************************************************************
				*						VENDOR READ REQUEST
				*
				**************************************************************************/
				int iDevNo;
				cout<<"Enter Device No to access:";
				cin>>iDevNo;
				cout << "Enter Endpoint No to access :";
				cin >> EpNo;
				EpNo = EpNo | 0x80;
				PUSB_CONTROL_REQUEST pctrlreq= (PUSB_CONTROL_REQUEST)malloc(sizeof(USB_CONTROL_REQUEST));

				pctrlreq->bmRequestType.Direction = 1;  //means Data stage directin is device to host
				pctrlreq->bmRequestType.Recepient = 0;  //recipient is device
				pctrlreq->bmRequestType.RequestType = 2;//vendor specific reuqest
				pctrlreq->wIndex.Value = 0x0000;
				pctrlreq->wValue.Value = 0x0100;
				pctrlreq->bRequest= 6;					//vendor request code(vendor specific)
				pctrlreq->wLength= 18;					//No of bytes to receive from device

				unsigned char* inbuf = (unsigned  char*)pctrlreq;
				unsigned char* outbuf = (unsigned  char*)malloc((pctrlreq->wLength)*sizeof(char));
				

				ULONG BytesReturned=0;;

				if (SLS_ListDevices(&iDevNo, cBuffer, SLS_LIST_BY_INDEX | SLS_OPEN_BY_DESCRIPTION) ==
              SLS_OK)
				{

					
					hDevice = SLS_W32_CreateFile(cBuffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL,
                                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |
                                         SLS_OPEN_BY_DESCRIPTION, NULL, EpNo);
					printf("hDevice = %x\n",hDevice);
					if(hDevice==(PVOID)SLS_INVALID_HANDLE)
					{
						cout<<"Invalid Handle Value "<<GetLastError()<<endl;
						break;
					}
					
					if(SLS_W32_SendVendorRequest(hDevice,
											(LPCSTR)inbuf,
											8,
											(LPCSTR)outbuf,
											pctrlreq->wLength,
											&BytesReturned
											)==SLS_OK)
						{
							cout<<"successful completation of vendor read request\n";

							printf("BytesReturned-%x\n",BytesReturned);

							for(unsigned int i=0;i<BytesReturned;i++)
							{
								printf("_%x_",outbuf[i]);
							}

						}
						else
						{
							cout<<"Error in IO operation "<<GetLastError()<<endl;
							break;
						}

					
				

				}
				
				else
				{
				   cout<<"Device Not Found"<<endl;
				}

				SLS_W32_ClosePipeHandle(hDevice);

				break;
			}
	  case 8 :
			{
				/*************************************************************************
				*						VENDOR WRITE REQUEST
				*
				**************************************************************************/
				int iDevNo;
				int i;

				cout<<"Enter Device No to access:";
				cin>>iDevNo;
				cout << "Enter Endpoint No to access :";
				cin >> EpNo;
				EpNo = EpNo | 0x80;
				PUSB_CONTROL_REQUEST pctrlreq= (PUSB_CONTROL_REQUEST)malloc(sizeof(USB_CONTROL_REQUEST));

				pctrlreq->bmRequestType.Direction = 0;	//means Data stage direction is host to device
				pctrlreq->bmRequestType.Recepient = 0;  //recipient is device
				pctrlreq->bmRequestType.RequestType = 2;//vendor specific reuqest
				pctrlreq->wIndex.Value = 0x0000;
				pctrlreq->wValue.Value = 0x0100;
				pctrlreq->bRequest=26;					//vendor request code(vendor specific)
				char Data[16];							//array that contains data to send to device
				pctrlreq->wLength= sizeof(Data);		//data buffer size.
				unsigned char* temp = (unsigned  char*)pctrlreq;
				
				/***************************************************************************
				* Preparing input buffer which contains first eight byte vendor specific
				* control request(pctrlreq) and after that data that appended to send to 
				* device
				*
				*****************************************************************************/
				
				
				for(i=0;i<sizeof(Data);i++)
					*(Data+i)=i;
				unsigned char* inbuf = (unsigned  char*)malloc((pctrlreq->wLength)*sizeof(char));
				
				for(i=0;i<8;i++)
					*(inbuf+i)=*(temp+i);
				
				for(i=8;i<sizeof(Data)+8;i++)
					*(inbuf+i)=*(Data+i-8);

				ULONG BytesReturned=0;;

				if (SLS_ListDevices(&iDevNo, cBuffer, SLS_LIST_BY_INDEX | SLS_OPEN_BY_DESCRIPTION) ==
              SLS_OK)
				{

					
					hDevice = SLS_W32_CreateFile(cBuffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL,
                                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |
                                         SLS_OPEN_BY_DESCRIPTION, NULL, EpNo);
					printf("hDevice = %x\n",hDevice);
					if(hDevice==(PVOID)SLS_INVALID_HANDLE)
					{
						cout<<"Invalid Handle Value "<<GetLastError()<<endl;
						break;
					}
					
					if(SLS_W32_SendVendorRequest(hDevice,
					                    (LPCSTR)inbuf,
										pctrlreq->wLength + 8,//8 byte control request + size of data buffer
										NULL,
										NULL,
										&BytesReturned
										)==SLS_OK)
					{
						cout<<"successful completation of vendor write  request\n";

						

					}
					else
					{
						cout<<"Error in IO operation "<<GetLastError()<<endl;;
					}



				}
				
				else
				{
				   cout<<"Device Not Found"<<endl;
				}

				SLS_W32_ClosePipeHandle(hDevice);

				break;
			}
	  case 9 :
			{
				
				/*************************************************************************
				*						SET INTERFACE REQUEST
				*
				**************************************************************************/
				int iDevNo;
				cout<<"Enter Device No to access:";
				cin>>iDevNo;
				cout << "Enter Endpoint No to access :";
				cin >> EpNo;
				//EpNo = EpNo | 0x80;
				ULONG BytesReturned=0;;
				PUSB_CONTROL_REQUEST pctrlreq= (PUSB_CONTROL_REQUEST)malloc(sizeof(USB_CONTROL_REQUEST));

				pctrlreq->bmRequestType.Direction = 0;
				pctrlreq->bmRequestType.Recepient = 1;
				pctrlreq->bmRequestType.RequestType = 0;
				pctrlreq->wIndex.Value = 0x00;//interface no
				pctrlreq->wValue.Value = 0x01;//alternate setting
				pctrlreq->bRequest= 0x0b;
				pctrlreq->wLength= 0;
				unsigned char* inbuf = (unsigned  char*)pctrlreq;
				
				if(SLS_ListDevices(&iDevNo,cBuffer,SLS_LIST_BY_INDEX|SLS_OPEN_BY_DESCRIPTION)==SLS_OK)
				{

					
					hDevice = SLS_W32_CreateFile(cBuffer, GENERIC_READ | GENERIC_WRITE, NULL, NULL,
                                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |
                                         SLS_OPEN_BY_DESCRIPTION, NULL, EpNo);
					printf("hDevice = %x\n",hDevice);
					
					if(hDevice==(PVOID)SLS_INVALID_HANDLE)
					{
						cout<<"Invalid Handle Value "<<GetLastError()<<endl;
						break;
					}
					
					if(SLS_W32_SetInterface(hDevice,
					                    (LPCSTR)inbuf,
										8,
										NULL,
										NULL,
										&BytesReturned
										)==SLS_OK)
					{
						cout<<"successful completation of set interface  request\n";

						printf("BytesReturned-%x\n",BytesReturned);

						/*for(int i=0;i<BytesReturned;i++)
						{
							printf("_%x_",outbuf[i]);
						}*/

					}
					else
					{
						cout<<"Error in set interface operation "<<GetLastError()<<endl;;
					}

				}
				
				else
				{
				   cout<<"Device Not Found"<<endl;
				}

				SLS_W32_ClosePipeHandle(hDevice);

				break;
			}

	  
      default:
        cout << "Wrong Selection" << endl;
        break;
      }
    cout << "EXIT Y:N";
    cin >> ch;
    }
  while (ch == 'N' || ch == 'n');
  
  return 0;
}
