//test.c

#include <stdio.h>
#include <stdlib.h>
#include "ftpManager.h"

int main()
{
	FTP_OPT ftp_opt;
	ftp_opt.url = "ftp://127.0.0.1/AVORS/upload.txt";
	ftp_opt.user_key = "hbemc:hbemc";
	ftp_opt.file = "G:\\project\\ftpManager\\upload_local.txt";

	// FTP上传（将本地文件upload_local.txt上传到指定的服务器目录下）
	if (FTP_UPLOAD_SUCCESS == ftp_upload(ftp_opt))
		printf("Upload success.\n");
	else
		printf("Upload failed.\n");

	ftp_opt.url = "ftp://127.0.0.1/AVORS/download.txt";
	ftp_opt.file = "G:\\project\\ftpManager\\download_local.txt";
	// FTP下（从指定的服务器目录下载对应的文件）

	if (FTP_DOWNLOAD_SUCCESS == ftp_download(ftp_opt))
		printf("Download success.\n");
	else
		printf("Download failed.\n");

	system("pause");

	return 0;
}
