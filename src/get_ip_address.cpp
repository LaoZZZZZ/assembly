/**
 * @file get_ip_address.cpp
 * @brief Return the local IP address.
 */
#include "mtpi_common.h"
#include <stdio.h>
#include <stdlib.h>

#if !defined WIN32
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

void convert_string2ip (const char *, int *);

/**
 * @brief get IPv4 address
 * @param [out] ip Point to a 4-field integer array.
 * @return Returns the IP address in a CHAR array. 
 */
const char *
get_ip_address(int * ip)
{
#if defined WIN32
	ip[0] = 127;
	ip[1] = 0;
	ip[2] = 0;
	ip[3] = 1;
	return "localhost";
#else
  std::stringstream ss;
  struct ifaddrs * ifAddrStruct=NULL;
  struct ifaddrs * ifa=NULL;
  void * tmpAddrPtr=NULL;

  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
     if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
       // is a valid IP4 Address
       tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
       char addressBuffer[INET_ADDRSTRLEN];
       inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
       if( ifa->ifa_name[0] == 'l' && ifa->ifa_name[1] == 'o' ) // check it is localhost
                    continue;
       // is not a 'localhost'
       ss << addressBuffer;
       convert_string2ip( ss.str().c_str(), ip );
       return ss.str().c_str();
    }
  }

  if (ifAddrStruct!=NULL)
    freeifaddrs(ifAddrStruct);

  return ss.str().c_str();
#endif
}

void
convert_string2ip (const char * s, int * ip)
{
  char * pEnd;
  ip[0] = (int) strtol (s, &pEnd, 10);
  ip[1] = (int) strtol (pEnd+1, &pEnd, 10);
  ip[2] = (int) strtol (pEnd+1, &pEnd, 10);
  ip[3] = (int) strtol (pEnd+1, NULL, 10);
}
