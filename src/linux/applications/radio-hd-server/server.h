/*

  Copyright © Grame 2001

  Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
  grame@grame.fr

*/

#ifndef __server__
#define __server__

#define kTCPPort	255

#ifdef __cplusplus
extern "C" {
#endif

void sendinfos (short ref, char * file);
void CnxInit (short ref);


#ifdef __cplusplus
}
#endif


#endif
