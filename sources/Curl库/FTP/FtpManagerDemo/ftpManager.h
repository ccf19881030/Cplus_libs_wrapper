//ftpManager.h

#ifndef _FTP_MANAGER_H_
#define _FTP_MANAGER_H_

/*FTP OPERATION CODE*/
typedef enum FTP_STATE
{
	FTP_UPLOAD_SUCCESS,
	FTP_UPLOAD_FAILED,
	FTP_DOWNLOAD_SUCCESS,
	FTP_DOWNLOAD_FAILED
}FTP_STATE;

/*FTP OPERATIONS OPTIONS*/
typedef struct FTP_OPT
{
	char *url;		/*url of ftp*/
	char *user_key;		/*username:password*/
	char *file;		/*filepath*/
}FTP_OPT;

#ifdef __cplusplus
extern "C" {
#endif

	/*upload file to ftp server*/
	FTP_STATE ftp_upload(const FTP_OPT ftp_option);

	/*download file from ftp server*/
	FTP_STATE ftp_download(const FTP_OPT ftp_option);

#ifdef __cplusplus
}
#endif

#endif
