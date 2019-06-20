//ftp-manager.c

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "ftpManager.h"

/*****************util api******************/
int get_file_size(FILE *file)
{
	int size = 0;
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	return size;
}

/******************curl api****************/
CURL *curl_init()
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *curl = curl_easy_init();
	if (NULL == curl)
	{
		fprintf(stderr, "Init curl failed.\n");
		exit(1);
	}
	return curl;
}

void curl_set_upload_opt(CURL *curl, const char *url, const char *user_key, FILE *file)
{
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
	curl_easy_setopt(curl, CURLOPT_READDATA, file);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
	curl_easy_setopt(curl, CURLOPT_INFILESIZE, get_file_size(file));
	curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
}

void curl_set_download_opt(CURL *curl, const char *url, const char *user_key, FILE *file)
{
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERPWD, user_key);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	//	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
}

void curl_exit(CURL *curl)
{
	curl_easy_cleanup(curl);
	curl_global_cleanup();
}

CURLcode curl_perform(CURL *curl)
{
	CURLcode ret = curl_easy_perform(curl);
	if (ret != 0)
	{
		fprintf(stderr, "Perform curl failed.\n");
		curl_exit(curl);
		exit(1);
	}
	return ret;
}

/****************ftp upload & download api******************/
FTP_STATE ftp_upload(const FTP_OPT ftp_option)
{
	FTP_STATE state;
	CURL *curl;;
	FILE *fp = fopen(ftp_option.file, "r");
	if (NULL == fp)
	{
		fprintf(stderr, "Open file failed at %s:%d\n", __FILE__, __LINE__);
		return FTP_UPLOAD_FAILED;
	}

	curl = curl_init();
	curl_set_upload_opt(curl, ftp_option.url, ftp_option.user_key, fp);
	if (CURLE_OK == curl_perform(curl))
		state = FTP_UPLOAD_SUCCESS;
	else
		state = FTP_UPLOAD_FAILED;

	curl_exit(curl);
	fclose(fp);

	return state;
}

FTP_STATE ftp_download(const FTP_OPT ftp_option)
{
	FTP_STATE state;
	CURL *curl;
	FILE *fp = fopen(ftp_option.file, "w");
	if (NULL == fp)
	{
		fprintf(stderr, "Open file failed at %s:%d\n", __FILE__, __LINE__);
		return FTP_UPLOAD_FAILED;
	}

	curl = curl_init();
	curl_set_download_opt(curl, ftp_option.url, ftp_option.user_key, fp);
	if (CURLE_OK == curl_perform(curl))
		state = FTP_DOWNLOAD_SUCCESS;
	else
		state = FTP_DOWNLOAD_FAILED;

	curl_exit(curl);
	fclose(fp);

	return state;
}
