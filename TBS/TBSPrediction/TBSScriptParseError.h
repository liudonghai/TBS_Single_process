#pragma once
#include "stdafx.h"

//define the erroe code for parsing script.
#define SCRIPT_EOF				0	//�ű������βδ�ܶ�ȡ��������־��
#define SCRIPT_CANNOT_OPEN		1	//�ű��ļ���ʧ��
#define SCRIPT_LINE				2	//����ƥ�����
#define SCRIPT_EXC_LOOP_END		3	//�����ѭ������
#define SCRIPT_EXC_COND_END		4	//�������������
#define SCRIPT_LOOP_NOTEND		5	//ѭ��δ����
#define SCRIPT_COND_NOTEND		6	//����δ����
#define SCRIPT_EXC_FININSH		7	//����Ľű�������־
#define SCRIPT_SINGLE			8	//<...>������
#define SCRIPT_KEYWORD			9	//����Ĺؼ���
#define SCRIPT_NOT_MATCH		10	//���б�ǩ��ʼ�ͽ�����ƥ��
#define SCRIPT_NOT_NUM			11	//���ֱ��ʽ���г��ַ������ַ�
#define SCRIPT_FINISH			12	//�ű�������FINISH��ǩ
#define SCRIPT_CODE_ERROR		13	//�ű����������ʱֻ֧��ANSI��UTF-8
CString cstrErrorInf[] = {L"�ű������βδ�ܶ�ȡ��������־",L"�ű���ʧ��",L"�Ƿ���",L"����ѭ���������",L"����������������",
							L"ѭ��δ����",L"����δ����",L"����Ľű�������־",L"<***>������",
							L"����Ĺؼ���",L"���б�ǩ��ʼ�ͽ�����ƥ��", L"���ֱ��ʽ���г��ַ������ַ�", L"�ű�������FINISH��ǩ",L"�������"};