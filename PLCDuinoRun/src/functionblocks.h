#ifndef FUNCTIONBLOCKS_H_
#define FUNCTIONBLOCKS_H_

void SRScan(unsigned char  *p_in, unsigned char  *p_out);
void RSScan(unsigned char  *p_in, unsigned char  *p_out);
void TONScan(unsigned char  *p_in, unsigned char  *p_out, int p_DeltaTime);
void TOFScan(unsigned char  *p_in, unsigned char  *p_out, int p_DeltaTime);
void TPScan(unsigned char *p_in, unsigned char *p_out, int p_DeltaTime);
void CTUDScan(unsigned char *p_in, unsigned char *p_out);

#endif /* FUNCTIONBLOCKS_H_ */
