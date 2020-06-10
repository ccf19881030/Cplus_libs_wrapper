#pragma once

#define XL_SUCCESS                     0
#define XL_ERROR_FAIL                  0x10000000

// ��δ���г�ʼ��
#define XL_ERROR_UNINITAILIZE          XL_ERROR_FAIL+1

// ��֧�ֵ�Э�飬Ŀǰֻ֧��HTTP
#define XL_ERROR_UNSPORTED_PROTOCOL    XL_ERROR_FAIL+2

// ��ʼ������ͼ��ʧ��
#define XL_ERROR_INIT_TASK_TRAY_ICON_FAIL  XL_ERROR_FAIL+3

// �������ͼ��ʧ��
#define XL_ERROR_ADD_TASK_TRAY_ICON_FAIL   XL_ERROR_FAIL+4

// ָ��Ϊ��
#define XL_ERROR_POINTER_IS_NULL    XL_ERROR_FAIL+5

// �ַ����ǿմ�
#define XL_ERROR_STRING_IS_EMPTY    XL_ERROR_FAIL+6

// �����·��û�а����ļ���
#define XL_ERROR_PATH_DONT_INCLUDE_FILENAME    XL_ERROR_FAIL+7

// ����Ŀ¼ʧ��
#define XL_ERROR_CREATE_DIRECTORY_FAIL    XL_ERROR_FAIL+8

// �ڴ治��
#define XL_ERROR_MEMORY_ISNT_ENOUGH    XL_ERROR_FAIL+9

// �������Ϸ�
#define XL_ERROR_INVALID_ARG    XL_ERROR_FAIL+10

// ���񲻴���
#define XL_ERROR_TASK_DONT_EXIST    XL_ERROR_FAIL+11

// �ļ������Ϸ�
#define XL_ERROR_FILE_NAME_INVALID   XL_ERROR_FAIL+12

// û��ʵ��
#define XL_ERROR_NOTIMPL    XL_ERROR_FAIL+13

// �Ѿ��������������ﵽ������������޷�������������
#define XL_ERROR_TASKNUM_EXCEED_MAXNUM    XL_ERROR_FAIL+14

// ��������δ֪
#define XL_ERROR_INVALID_TASK_TYPE    XL_ERROR_FAIL+15

// �ļ��Ѿ�����
#define XL_ERROR_FILE_ALREADY_EXIST   XL_ERROR_FAIL+16

// �ļ�������
#define XL_ERROR_FILE_DONT_EXIST      XL_ERROR_FAIL+17

// ��ȡcfg�ļ�ʧ��
#define XL_ERROR_READ_CFG_FILE_FAIL   XL_ERROR_FAIL+18

// д��cfg�ļ�ʧ��
#define XL_ERROR_WRITE_CFG_FILE_FAIL   XL_ERROR_FAIL+19

// �޷��������񣬿����ǲ�֧�ֶϵ�������Ҳ�п����������Ѿ�ʧ��
// ͨ����ѯ����״̬��ȷ������ԭ��
#define XL_ERROR_CANNOT_CONTINUE_TASK  XL_ERROR_FAIL+20

// �޷���ͣ���񣬿����ǲ�֧�ֶϵ�������Ҳ�п����������Ѿ�ʧ��
// ͨ����ѯ����״̬��ȷ������ԭ��
#define XL_ERROR_CANNOT_PAUSE_TASK  XL_ERROR_FAIL+21

// ������̫С
#define XL_ERROR_BUFFER_TOO_SMALL   XL_ERROR_FAIL+22

// ����XLInitDownloadEngine���̣߳��ڵ���XLUninitDownloadEngine֮ǰ�Ѿ�������
// ��ʼ�����������̣߳��ڵ���XLUninitDownloadEngine֮ǰ�����뱣��ִ��״̬��
#define XL_ERROR_INIT_THREAD_EXIT_TOO_EARLY XL_ERROR_FAIL+23

// TP����
#define XL_ERROR_TP_CRASH XL_ERROR_FAIL+24

// ���񲻺Ϸ�������XLContinueTaskFromTdFile���������ڲ������л�ʧ��ʱ��������������
#define XL_ERROR_TASK_INVALID XL_ERROR_FAIL+25
